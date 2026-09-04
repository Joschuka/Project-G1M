#pragma once

#ifndef G2A_H
#define G2A_H

// ---------------------------------------------------------------------------
// The engine's own names for what this file parses, so a future reader can find
// them again. Recovered from a Windows build that ships the declarations, not
// just the code (SAN14_EN.exe.unpacked.exe, imagebase 0x140000000).
//
//   ktgl::S_G2A_HEADER            32 bytes
//     +0x00 S_RESOURCE_HEADER resHeader     magic 'G2A_' + version
//     +0x08 unsigned int      fileSize
//     +0x0C float             fps
//     +0x10 unsigned __int16  frameNum
//     +0x12 unsigned __int16  type   : 4    KTGL_G2A_MOTION_TYPE
//           unsigned __int16  objNum : 12
//     +0x14 unsigned int      keyInfoSize
//     +0x18 unsigned int      curveDataNum
//     +0x1C unsigned __int32  shapeId      : 8
//           unsigned __int32  simAnimation : 1
//
//   ktgl::S_G2A_OBJECT_INFO       4 bytes, one per animated target
//     dataNum : 4      spline count for this target
//     targetId: 10     bone index. TEN bits, see the note in the loop below
//     offset  : 18     into the key info block, in DWORDS
//
//   ktgl::S_G2A_KEY_FRAME_INFO    12 bytes + flexible array
//     unsigned __int16 dataType        0 rotation, 1 translation, 2 scale
//     unsigned __int16 keyFrameNum
//     unsigned int     curveIndex
//     unsigned __int16 keyFrame[]
//
//   ktgl::S_G2A_CURVE_DATA        32 bytes, unsigned __int64[4]
//
//   enum KTGL_G2A_MOTION_TYPE { MDL = 0, CAM = 1, SHAPE = 2 };
//   enum KTGL_G2A_VERSION     { 0010, 0020, 0030, 0040, 0050 };  // no 0000
//
// Engine functions worth re-reading against a future build (match the symbol
// name first, the addresses only mean anything in the build they came from):
//   ktgl::CMotionData2::CreateMotionData     header and section order
//   ktgl::CMotionData2::GetMatrix            object info bit split, record stride
//   ktgl::CMotionData2::GetBoneTranslation   the 10 bit targetId comparison
//   ktgl::CMotionData2::GetCurveData         key search, curveIndex arithmetic
//   ktgl::CMotionData2::GetCameraElements    the camera reading of dataType
//   ktgl::RefMotionData2Impl::EvaluateG2AFunctionCurve   the packed cubic
//
// A fuller write up, with the sample validation behind each claim, is in
// samples/G1A_G2A.bt.
// ---------------------------------------------------------------------------

// KTGL_G2A_MOTION_TYPE. A G2A is not necessarily a skeletal animation: the same
// container holds camera and shape motions, and for those the per spline
// dataType below means something completely different (for a camera, 0 is the
// eye position, 1 the look at target and 2 packs roll and fov). Importing one of
// those as a skeleton produces garbage, so they are skipped.
enum KTGL_G2A_MOTION_TYPE
{
	KTGL_G2A_MOTION_TYPE_MDL = 0,
	KTGL_G2A_MOTION_TYPE_CAM = 1,
	KTGL_G2A_MOTION_TYPE_SHAPE = 2,
};

// ktgl::S_G2A_KEY_FRAME_INFO::dataType, for a MDL motion.
enum KTGL_G2A_DATA_TYPE
{
	KTGL_G2A_DATA_TYPE_ROTATION = 0,	// exponential map, not a quaternion
	KTGL_G2A_DATA_TYPE_TRANSLATION = 1,
	KTGL_G2A_DATA_TYPE_SCALE = 2,
};

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
	float framerate;					// S_G2A_HEADER::fps
	uint32_t animationLength;			// S_G2A_HEADER::frameNum
	uint32_t motionType;				// S_G2A_HEADER::type, KTGL_G2A_MOTION_TYPE
	uint32_t boneInfoSectionSize;		// objNum * 4
	uint32_t timingSectionSize;			// S_G2A_HEADER::keyInfoSize
	uint32_t entryCount;				// S_G2A_HEADER::curveDataNum
	uint32_t boneInfoCount;				// S_G2A_HEADER::objNum
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
			motionType = KTGL_G2A_MOTION_TYPE_MDL; //bit split unverified on big endian
		}
		else
		{
			// S_G2A_HEADER: frameNum is the whole low halfword, then type:4 and
			// objNum:12. The old 0x3FFF mask on frameNum was two bits short.
			animationLength = packedInfo & 0xFFFF;
			motionType = (packedInfo >> 16) & 0xF;
			boneInfoSectionSize = (packedInfo >> 18) & 0x3FFC; //objNum * 4
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

		// Only a model motion describes bones. A camera or shape motion reuses
		// the identical container and the identical per spline dataType values,
		// but they mean different things: CMotionData2::GetCameraElements reads
		// dataType 0 as the eye position, 1 as the look at target and 2 as three
		// loose scalars (roll, fov, one more). Feeding those to the skeletal path
		// converts a world space position through an exponential map to
		// quaternion and produces nonsense, silently. Nothing else in the file
		// distinguishes them, so this check is the only guard there is.
		if (header.motionType != KTGL_G2A_MOTION_TYPE_MDL)
			return;
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
				// S_G2A_OBJECT_INFO is dataNum:4, targetId:10, offset:18 for
				// EVERY little endian version. targetId is TEN bits, not eight:
				// CMotionData2::GetBoneTranslation searches this array comparing
				// ((w >> 4) & 0x3FF) against the bone it wants.
				//
				// The old code masked v0050 to 8 bits. That is a real bug, not a
				// cosmetic one: 53 of 535 v0050 samples carry target ids above
				// 255 (the highest seen is 347), so those bones aliased onto
				// id & 0xFF. Worse, 28 of those files then looked like their ids
				// went backwards, which fired the wrap compensation below and
				// shifted every later bone by another 256. With the correct mask
				// not one sample shows a decreasing id, so the wrap path stays
				// dormant, which is what it is for: files with over 1024 bones.
				splineTypeCount = packedInfo & 0xF;
				boneID = (packedInfo >> 4) & 0x3FF;
				// Engine: offset = (w >> 12) & 0xFFFFC, i.e. (w >> 14) * 4.
				// Pre 0050 files store this word as two u16 that the loader
				// repacks first, and on the raw dword ">> 14" happens to yield
				// exactly the same byte offset, so both branches stay as they
				// were. The RevAlignOffset below clears the low bits either way.
				boneTimingDataOffset = header.bIsG2A5 ? (packedInfo >> 12) : (packedInfo >> 14);
				if (boneID < lastID)
					globalOffset += 1;
				lastID = boneID;
				boneID += globalOffset * 1024;
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
