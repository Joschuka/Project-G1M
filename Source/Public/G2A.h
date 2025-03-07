#pragma once

#ifndef G2A_H
#define G2A_H

//helper struct cause pointers become invalid after push_backs
struct keyFramedValueIndex
{
	int32_t rotIndex = -1;
	int32_t posIndex = -1;
	int32_t scaleIndex = -1;
};

template<bool bBigEndian>
struct G2AHeader
{
	float framerate;
	uint32_t animationLength;
	uint32_t boneInfoSectionSize;
	uint32_t timingSectionSize;
	uint32_t entryCount;
	uint32_t boneInfoCount;
	bool bIsG2A5;
	bool bIsG2A4;
	G2AHeader(BYTE* buffer, int bufferLen, uint32_t& offset)
	{
		GResourceHeader<bBigEndian> sectionHeader = reinterpret_cast<GResourceHeader<bBigEndian>*>(buffer);
		offset += sizeof(GResourceHeader<bBigEndian>);
		bIsG2A5 = sectionHeader.chunkVersion == 0x30303530 ? true : false;
		bIsG2A4 = sectionHeader.chunkVersion == 0x30303430 ? true : false;
		framerate = *(float*)(buffer + offset);
		if (bBigEndian)
			LITTLE_BIG_SWAP(framerate);
		offset += 4;

		uint32_t packedInfo = *(uint32_t*)(buffer + offset);
		if (bBigEndian)
			LITTLE_BIG_SWAP(packedInfo);
		offset += 4;
		if (bBigEndian)
		{
			animationLength = packedInfo >> 18;
			boneInfoSectionSize = (packedInfo & 0x3FFF) << 2;
		}
		else
		{
			animationLength = packedInfo & 0x3FFF;
			boneInfoSectionSize = (packedInfo >> 18) & 0x3FFC;
		}
		timingSectionSize = *(uint32_t*)(buffer + offset);
		entryCount = *(uint32_t*)(buffer + offset + 4);
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(timingSectionSize);
			LITTLE_BIG_SWAP(entryCount);
		}
		offset += 8;
		boneInfoCount = boneInfoSectionSize >> 2;
		if (bIsG2A5 || bIsG2A4)
			offset += 4;
	}
};

template<bool bBigEndian>
struct G2A
{
	G2A(BYTE* buffer, int bufferLen, std::string& animName, modelBone_t* joints, int jointCount, std::map<uint32_t, uint32_t>& globalToFinal, CArrayList<noesisAnim_t*>& animList,
		std::vector<void*>& pointersToFree, int& framerate, bool bAdditive, noeRAPI_t* rapi)
	{
		uint32_t offset = 0;
		G2AHeader<bBigEndian> header = G2AHeader<bBigEndian>(buffer, bufferLen, offset);
		uint32_t lastID = 0;
		uint32_t globalOffset = 0;
		uint32_t checkpoint = offset;
		std::vector<float> animationData;
		std::vector<noeKeyFramedBone_t> keyFramedBones;
		std::vector<noeKeyFrameData_t> keyFramedValues;
		std::vector<keyFramedValueIndex> keyFramedValuesIndices;
		for (auto i = 0; i < header.boneInfoCount; i++)
		{
			offset = checkpoint + i * 4;
			uint32_t splineTypeCount, boneID, boneTimingDataOffset;
			uint32_t packedInfo = *(uint32_t*)(buffer + offset);
			if (bBigEndian)
				LITTLE_BIG_SWAP(packedInfo);
			if (bBigEndian)
			{
				splineTypeCount = packedInfo >> 28;
				boneID = (packedInfo >> 16) & 0xFFF;
				boneTimingDataOffset = (packedInfo & 0xFFFF) << 2;
				if (boneID < lastID)
					globalOffset += 1;
				lastID = boneID;
				boneID += globalOffset * (header.bIsG2A5 ? 256 : 1024);
			}
			else
			{
				splineTypeCount = packedInfo & 0xF;
				boneID = header.bIsG2A5 ? (packedInfo >> 4) & 0xFF : (packedInfo >> 4) & 0x3FF;
				boneTimingDataOffset = header.bIsG2A5 ? (packedInfo >> 12) : (packedInfo >> 14);
				if (boneID < lastID)
					globalOffset += 1;
				lastID = boneID;
				boneID += globalOffset * (header.bIsG2A5 ? 256 : 1024);
			}
			offset = checkpoint + header.boneInfoSectionSize + boneTimingDataOffset;
			RevAlignOffset(offset, 4);

			if (globalToFinal.find(boneID) == globalToFinal.end()) //If the bone index isn't in the skeleton
				continue;

			//Create the bone
			noeKeyFramedBone_t kfBone;
			memset(&kfBone, 0, sizeof(noeKeyFramedBone_t));
			kfBone.boneIndex = globalToFinal[boneID];
			kfBone.rotationInterpolation = kfBone.translationInterpolation = kfBone.scaleInterpolation = NOEKF_INTERPOLATE_NEAREST;
			kfBone.translationType = NOEKF_TRANSLATION_VECTOR_3;
			kfBone.rotationType = NOEKF_ROTATION_QUATERNION_4;
			kfBone.scaleType = NOEKF_SCALE_VECTOR_3;
			keyFramedValueIndex valueIndex;
			if (bAdditive)
				kfBone.flags |= KFBONEFLAG_ADDITIVE; //simply premultiply by bind matrix, see python script to see how done manually


			//Getting data
			for (auto j = 0; j < splineTypeCount; j++)
			{
				std::vector<uint16_t> keyFrameTimings;
				std::vector<std::array<uint64_t, 4>> quantizedData;
				uint16_t opcode = *(uint16_t*)(buffer + offset);
				uint16_t keyFrameCount = *(uint16_t*)(buffer + offset + 2);
				uint32_t firstDataIndex = *(uint32_t*)(buffer + offset + 4);
				if (bBigEndian)
				{
					LITTLE_BIG_SWAP(opcode);
					LITTLE_BIG_SWAP(keyFrameCount);
					LITTLE_BIG_SWAP(firstDataIndex);
				}
				offset += 8;
				keyFrameTimings.resize(keyFrameCount);
				quantizedData.resize(keyFrameCount);
				memcpy(keyFrameTimings.data(), buffer + offset, sizeof(uint16_t) * keyFrameCount);
				if (bBigEndian)
				{
					for (auto& kft : keyFrameTimings)
						LITTLE_BIG_SWAP(kft);
				}
				offset += sizeof(uint16_t) * keyFrameCount;
				AlignOffset(offset, 4);
				uint32_t checkpoint2 = offset;
				offset = checkpoint + header.boneInfoSectionSize + header.timingSectionSize + firstDataIndex * 32;

				memcpy(quantizedData.data(), buffer + offset, 4 * sizeof(uint64_t) * keyFrameCount);
				if (bBigEndian)
				{
					for (auto& qd : quantizedData)
					{
						for (auto& v : qd)
							LITTLE_BIG_SWAP(v);
					}
				}
				bool bNeedsExtra = false;
				uint32_t helper = keyFramedValues.size();
				if (keyFrameCount == 1)
				{
					RichVec3 temp1;
					RichQuat temp2;
					if (opcode == 0) //rotation
					{
						kfBone.numRotationKeys++;
						function1(quantizedData[0], temp1, 0, 1);
						function2(temp1, temp2);
						temp2.Transpose();
						for (auto& q : temp2.q)
							animationData.push_back(std::move(q));
						noeKeyFrameData_t noeKfValue;
						memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
						noeKfValue.time = 0.01;
						noeKfValue.dataIndex = animationData.size() - 4;
						keyFramedValues.push_back(std::move(noeKfValue));
					}
					else if (opcode == 1) //position
					{
						kfBone.numTranslationKeys++;
						function1(quantizedData[0], temp1, 0, 1);
						for (auto& v : temp1.v)
							animationData.push_back(std::move(v));
						noeKeyFrameData_t noeKfValue;
						memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
						noeKfValue.time = 0.01;
						noeKfValue.dataIndex = animationData.size() - 3;
						keyFramedValues.push_back(std::move(noeKfValue));
					}
					else if (opcode == 2) //scale
					{
						kfBone.numScaleKeys++;
						function1(quantizedData[0], temp1, 0, 1);
						for (auto& v : temp1.v)
							animationData.push_back(std::move(v));
						noeKeyFrameData_t noeKfValue;
						memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
						noeKfValue.time = 0.01;
						noeKfValue.dataIndex = animationData.size() - 3;
						keyFramedValues.push_back(std::move(noeKfValue));
					}
				}
				else if (keyFrameTimings.back() != header.animationLength)
				{
					keyFrameTimings.push_back(header.animationLength);
					bNeedsExtra = true;
				}

				for (auto k = 0; k < keyFrameTimings.size() - 1; k++)
				{
					uint32_t keyframe1 = keyFrameTimings[k];
					uint32_t keyframe2 = keyFrameTimings[k + 1];
					RichVec3 temp1;
					RichQuat temp2;
					if (opcode == 0) //rotation
					{
						kfBone.numRotationKeys += keyframe2 - keyframe1;
						for (auto l = 0; l < keyframe2 - keyframe1; l++)
						{
							function1(quantizedData[k], temp1, l, keyframe2 - keyframe1);
							function2(temp1, temp2);
							temp2.Transpose();
							for (auto& q : temp2.q)
								animationData.push_back(std::move(q));
							noeKeyFrameData_t noeKfValue;
							memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
							noeKfValue.time = (keyframe1 + l) / header.framerate;
							noeKfValue.dataIndex = animationData.size() - 4;
							keyFramedValues.push_back(std::move(noeKfValue));
						}
					}
					else if (opcode == 1) //position
					{
						kfBone.numTranslationKeys += keyframe2 - keyframe1;
						for (auto l = 0; l < keyframe2 - keyframe1; l++)
						{
							function1(quantizedData[k], temp1, l, keyframe2 - keyframe1);
							for (auto& v : temp1.v)
								animationData.push_back(std::move(v));
							noeKeyFrameData_t noeKfValue;
							memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
							noeKfValue.time = (keyframe1 + l) / header.framerate;
							noeKfValue.dataIndex = animationData.size() - 3;
							keyFramedValues.push_back(std::move(noeKfValue));
						}
					}
					else if (opcode == 2) //scale
					{
						kfBone.numScaleKeys += keyframe2 - keyframe1;
						for (auto l = 0; l < keyframe2 - keyframe1; l++)
						{
							function1(quantizedData[k], temp1, l, keyframe2 - keyframe1);
							for (auto& v : temp1.v)
								//animationData.push_back(std::move(v));
								animationData.push_back(std::move(v));
							noeKeyFrameData_t noeKfValue;
							memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
							noeKfValue.time = (keyframe1 + l) / header.framerate;
							noeKfValue.dataIndex = animationData.size() - 3;
							keyFramedValues.push_back(std::move(noeKfValue));
						}
					}
				}
				switch (opcode)
				{
				case 0:
					valueIndex.rotIndex = helper;
					break;
				case 1:
					valueIndex.posIndex = helper;
					break;
				case 2:
					valueIndex.scaleIndex = helper;
					break;
				default:
					break;
				}
				offset = checkpoint2;
			}

			keyFramedBones.push_back(kfBone);
			keyFramedValuesIndices.push_back(valueIndex);
		}

		//Assign the pointers
		for (auto u = 0; u < keyFramedValuesIndices.size(); u++)
		{
			keyFramedValueIndex& valueIndex = keyFramedValuesIndices[u];
			noeKeyFramedBone_t& kfBone = keyFramedBones[u];
			if (valueIndex.rotIndex >= 0)
				kfBone.rotationKeys = &keyFramedValues[valueIndex.rotIndex];
			if (valueIndex.posIndex >= 0)
				kfBone.translationKeys = &keyFramedValues[valueIndex.posIndex];
			if (valueIndex.scaleIndex >= 0)
				kfBone.scaleKeys = &keyFramedValues[valueIndex.scaleIndex];
		}

		noeKeyFramedAnim_t * keyFramedAnim = (noeKeyFramedAnim_t*)(rapi->Noesis_UnpooledAlloc(sizeof(noeKeyFramedAnim_t)));
		pointersToFree.push_back(keyFramedAnim);
		memset(keyFramedAnim, 0, sizeof(noeKeyFramedAnim_t));
		//Animation data
		keyFramedAnim->name = rapi->Noesis_PooledString(const_cast<char*>(animName.c_str()));
		keyFramedAnim->framesPerSecond = header.framerate;
		keyFramedAnim->numBones = globalToFinal.size();
		keyFramedAnim->kfBones = keyFramedBones.data();
		keyFramedAnim->numKfBones = keyFramedBones.size();
		keyFramedAnim->data = animationData.data();
		keyFramedAnim->numDataFloats = animationData.size();

		framerate = header.framerate;

		noesisAnim_t * noeAnim = rapi->Noesis_AnimFromBonesAndKeyFramedAnim(joints, jointCount, keyFramedAnim, true);

		if (noeAnim)
		{
			noeAnim->filename = rapi->Noesis_PooledString(const_cast<char*>(animName.c_str()));
			noeAnim->flags |= NANIMFLAG_FILENAMETOSEQ;
			noeAnim->shouldFreeData = true;

			/*noeAnim->aseq = rapi->Noesis_AnimSequencesAlloc(1, header.animationLength);
			noeAnim->aseq->s->startFrame = 0;
			noeAnim->aseq->s->endFrame = header.animationLength - 1;
			noeAnim->aseq->s->frameRate = header.framerate;
			noeAnim->aseq->s->name = rapi->Noesis_PooledString(const_cast<char*>(animName.c_str()));*/

			animList.Append(noeAnim);


		}
	}
};

#endif // !G2A_H
