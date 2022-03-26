#pragma once

#ifndef G1A_H
#define G1A_H

template<bool bBigEndian>
struct G1AHeader
{
	uint16_t animationType; //not sure
	float duration; //in seconds
	uint32_t dataSectionOffset;	
	uint16_t boneInfoCount;
	uint16_t boneMaxID;
	uint32_t chunkVersion;
	G1AHeader(BYTE* buffer, int bufferLen, uint32_t& offset)
	{
		GResourceHeader<bBigEndian> sectionHeader = reinterpret_cast<GResourceHeader<bBigEndian>*>(buffer);
		chunkVersion = sectionHeader.chunkVersion;
		offset += sizeof(GResourceHeader<bBigEndian>);	
		animationType = *(uint16_t*)(buffer + offset);
		offset += 4; //skip unknown
		duration = *(float*)(buffer + offset);
		dataSectionOffset = *(uint32_t*)(buffer + offset + 4) * 0x10;
		offset += 32;
		boneInfoCount = *(uint16_t*)(buffer + offset);
		boneMaxID = *(uint16_t*)(buffer + offset + 2);
		offset += 4;
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(animationType);
			LITTLE_BIG_SWAP(duration);
			LITTLE_BIG_SWAP(dataSectionOffset);
			LITTLE_BIG_SWAP(boneInfoCount);
			LITTLE_BIG_SWAP(boneMaxID);
		}
	}
};

template<bool bBigEndian>
struct G1A
{
	G1A(BYTE* buffer, int bufferLen, std::string& animName, modelBone_t* joints, int jointCount, std::map<uint32_t, uint32_t>& globalToFinal, CArrayList<noesisAnim_t*>& animList,
		std::vector<void*>& pointersToFree, int& framerate,bool bAdditive, noeRAPI_t* rapi)
	{
		uint32_t offset = 0;
		std::vector<float> animationData;
		std::vector<noeKeyFramedBone_t> keyFramedBones;
		std::vector<noeKeyFrameData_t> keyFramedValues;
		std::vector<keyFramedValueIndex> keyFramedValuesIndices;

		G1AHeader<bBigEndian> header = G1AHeader<bBigEndian>(buffer, bufferLen, offset);
		uint32_t checkpoint = offset;
		for (auto i = 0; i < header.boneInfoCount; i++)
		{
			offset = checkpoint + i * 8;
			uint32_t boneID = *(uint32_t*)(buffer + offset);
			if (bBigEndian)
				LITTLE_BIG_SWAP(boneID);
			if (globalToFinal.find(boneID) == globalToFinal.end()) //If the bone index isn't in the skeleton
				continue;

			//Create the bone
			noeKeyFramedBone_t kfBone;
			memset(&kfBone, 0, sizeof(noeKeyFramedBone_t));
			kfBone.boneIndex = globalToFinal[boneID];
			kfBone.rotationInterpolation = kfBone.translationInterpolation = kfBone.scaleInterpolation = NOEKF_INTERPOLATE_LINEAR;
			kfBone.translationType = NOEKF_TRANSLATION_VECTOR_3;
			kfBone.rotationType = NOEKF_ROTATION_QUATERNION_4;
			kfBone.scaleType = NOEKF_SCALE_VECTOR_3;
			keyFramedValueIndex valueIndex;
			if(bAdditive)
				kfBone.flags |= KFBONEFLAG_ADDITIVE;

			uint32_t splineInfoOffset = *(uint32_t*)(buffer + offset + 4);
			if (bBigEndian)
				LITTLE_BIG_SWAP(splineInfoOffset);
			offset = checkpoint - 4 + splineInfoOffset * 0x10;
			uint32_t checkpoint2 = offset;
			uint32_t opcode = *(uint32_t*)(buffer + offset);
			if (bBigEndian)
				LITTLE_BIG_SWAP(opcode);
			//Containers
			std::vector<std::vector<std::array<float,4>>> chanValues; //...
			std::vector<std::vector<float>> chanTimes;
			//helping variables
			int32_t componentCount = -1, indexs = -1, indexr = -1, indexl = -1;
			switch (opcode)
			{
			case 0x1:
				componentCount = 2;
				break;
			case 0x2:
				componentCount = 4;
				indexr = 0;
				break;
			case 0x4:
				componentCount = 7;
				indexr = 0;
				indexl = 4;
				break;
			case 0x6:
				componentCount = 10;
				indexs = 0;
				indexr = 3;
				indexl = 7;
				break;
			case 0x8:
				componentCount = 7;
				indexs = 0;
				indexr = 3;
				break;
			default:
				continue;
				break;
			}

			for (auto j = 0; j < componentCount; j++)
			{
				std::vector<std::array<float, 4>> data;
				std::vector<float> times;

				offset = checkpoint2 + 4 + j * 8;
				uint32_t keyFrameCount = *(uint32_t*)(buffer + offset);
				uint32_t dataOffset = *(uint32_t*)(buffer + offset + 4);
				if (bBigEndian)
				{
					LITTLE_BIG_SWAP(keyFrameCount);
					LITTLE_BIG_SWAP(dataOffset);
				}
				offset = checkpoint2 + dataOffset * 0x10;

				if (header.chunkVersion > 0x30303430)
				{	//components before timing
					for (auto k = 0; k < keyFrameCount; k++)
					{
						std::array<float, 4> temp;
						memcpy(temp.data(), buffer + offset, 16);
						if (bBigEndian)
						{
							for (auto& e : temp)
								LITTLE_BIG_SWAP(e);
						}
						data.push_back(std::move(temp));
						offset += 16;
					}
					for (auto k = 0; k < keyFrameCount; k++)
					{
						float temp = *(float*)(buffer + offset);
						if (bBigEndian)
							LITTLE_BIG_SWAP(temp);
						times.push_back(temp);
						offset += 4;
					}
				}
				else
				{
					//timing before components
					for (auto k = 0; k < keyFrameCount; k++)
					{
						float temp = *(float*)(buffer + offset);
						if (bBigEndian)
							LITTLE_BIG_SWAP(temp);
						times.push_back(temp);
						offset += 4;
					}
					for (auto k = 0; k < keyFrameCount; k++)
					{
						std::array<float, 4> temp;
						memcpy(temp.data(), buffer + offset, 16);
						if (bBigEndian)
						{
							for (auto& e : temp)
								LITTLE_BIG_SWAP(e);
						}
						data.push_back(std::move(temp));
						offset += 16;
					}
				}
				
				chanValues.push_back(std::move(data));
				chanTimes.push_back(std::move(times));
			}

			if (indexr >= 0)
			{
				std::vector<float> allValues;
				std::set<float> allTimes;
				uint32_t stride;
				function3(chanValues, chanTimes, indexr, 4, allTimes, allValues, stride);
				valueIndex.rotIndex = keyFramedValues.size();
				kfBone.numRotationKeys = allTimes.size();
				uint32_t u= 0;
				for(auto& t: allTimes)
				{
					RichQuat quat = RichQuat(allValues[u], allValues[u + stride], allValues[u + 2 * stride], allValues[u + 3 * stride]).GetTranspose();
					/*animationData.push_back(allValues[u]);
					animationData.push_back(allValues[u + stride]);
					animationData.push_back(allValues[u + 2*stride]);
					animationData.push_back(allValues[u + 3*stride]);*/
					for (auto& c : quat.q)
						animationData.push_back(c);

					noeKeyFrameData_t noeKfValue;
					memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
					noeKfValue.time = t;
					noeKfValue.dataIndex = animationData.size() - 4;
					keyFramedValues.push_back(std::move(noeKfValue));
					u++;
				}
			}
			if (indexl >= 0)
			{
				std::vector<float> allValues;
				std::set<float> allTimes;
				uint32_t stride;
				function3(chanValues, chanTimes, indexl, 3, allTimes, allValues, stride);
				valueIndex.posIndex = keyFramedValues.size();
				kfBone.numTranslationKeys = allTimes.size();
				uint32_t u = 0;
				for (auto& t : allTimes)
				{
					animationData.push_back(allValues[u]);
					animationData.push_back(allValues[u + stride]);
					animationData.push_back(allValues[u + 2 * stride]);
					noeKeyFrameData_t noeKfValue;
					memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
					noeKfValue.time = t;
					noeKfValue.dataIndex = animationData.size() - 3;
					keyFramedValues.push_back(std::move(noeKfValue));
					u++;
				}
			}
			if (indexs >= 0)
			{
				std::vector<float> allValues;
				std::set<float> allTimes;
				uint32_t stride;
				function3(chanValues, chanTimes, indexs, 3, allTimes, allValues, stride);
				valueIndex.scaleIndex = keyFramedValues.size();
				kfBone.numScaleKeys = allTimes.size();
				uint32_t u = 0;
				for (auto& t : allTimes)
				{
					animationData.push_back(allValues[u]);
					animationData.push_back(allValues[u + stride]);
					animationData.push_back(allValues[u + 2 * stride]);
					noeKeyFrameData_t noeKfValue;
					memset(&noeKfValue, 0, sizeof(noeKeyFrameData_t));
					noeKfValue.time = t;
					noeKfValue.dataIndex = animationData.size() - 3;
					keyFramedValues.push_back(std::move(noeKfValue));
					u++;
				}
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
		keyFramedAnim->framesPerSecond = 30;
		keyFramedAnim->numBones = globalToFinal.size();
		keyFramedAnim->kfBones = keyFramedBones.data();
		keyFramedAnim->numKfBones = keyFramedBones.size();
		keyFramedAnim->data = animationData.data();
		keyFramedAnim->numDataFloats = animationData.size();

		framerate = 30;

		noesisAnim_t* noeAnim = rapi->Noesis_AnimFromBonesAndKeyFramedAnim(joints, jointCount, keyFramedAnim, true);

		if (noeAnim)
		{
			noeAnim->filename = rapi->Noesis_PooledString(const_cast<char*>(animName.c_str()));
			noeAnim->flags |= NANIMFLAG_FILENAMETOSEQ;
			noeAnim->shouldFreeData = true;
			animList.Append(noeAnim);
		}
	}
};

#endif // !G1A_H

