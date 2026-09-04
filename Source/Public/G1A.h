#pragma once

#ifndef G1A_H
#define G1A_H

// ---------------------------------------------------------------------------
// The engine's own names for this header, from a Windows build that ships the
// declarations (SAN14_EN.exe.unpacked.exe, imagebase 0x140000000):
//
//   ktgl::S_G1A_HEADER            48 bytes
//     +0x00 S_RESOURCE_HEADER resHeader     magic 'G1A_' + version
//     +0x08 unsigned int      fileQWC       whole file, in 16 byte units
//     +0x0C char              dataType      KTGL_G1A_DATA_TYPE
//     +0x0D unsigned __int8   timeType   : 4   KTGL_G1A_TIME_TYPE
//           unsigned __int8   isScalable : 1
//           unsigned __int8   isQuaternion : 1
//           unsigned __int8   isSimAnimation : 1
//     +0x0E unsigned __int16  other         [OES2] calls this "levels"
//     +0x10 float             maxTime
//     +0x14 int               headQWC       curve data starts at 16 * this
//     +0x18 unsigned int      ktglAddress[2]   runtime scratch, 0 on disk
//     +0x20 (the level offset table begins here)
//
//   enum KTGL_G1A_DATA_TYPE { MODEL = 0, CAMERA = 1, LIGHT = 2, SHAPE = 4 };
//   enum KTGL_G1A_TIME_TYPE { FRAME = 0, SEC = 1 };
//   enum KTGL_G1A_VERSION   { 0042, 0043, 0044, 0045 };  // CURRENT = 0045
//
// The offsets this struct already used line up with that exactly: "duration" is
// maxTime and "dataSectionOffset" is headQWC * 16. Only the names were missing.
//
// Container shape, from the address arithmetic shared by GetCameraElementsOf,
// GetLightElementsOf and GetShapeElementsOf:
//     levelTable = fileStart + 32                 one u32 per level
//     level i    = levelTable + 16 * levelTable[i]
//     object j   = level + 16 * level.entry[j].offset
// This struct assumes a single level whose offset is 1, which puts the jump
// table at +48 and is what "boneInfoCount" reads. Every sample seen so far is
// single level, so that holds, but a multi level file would need the table walk.
//
// See samples/G1A_G2A.bt for the validated write up and the sample counts.
// ---------------------------------------------------------------------------

// ktgl::S_G1A_HEADER::dataType. A G1A is not always a bone animation: the same
// container carries camera, light and shape animations, distinguished only by
// this byte. Their objects use a different opcode range entirely (101/102 for
// cameras, 201/202/203 for lights, 401 for shapes) rather than the 0..8 bone
// opcodes, so a non MODEL file yields no bones at all.
enum KTGL_G1A_DATA_TYPE
{
	KTGL_G1A_DATA_TYPE_MODEL = 0,
	KTGL_G1A_DATA_TYPE_CAMERA = 1,
	KTGL_G1A_DATA_TYPE_LIGHT = 2,
	KTGL_G1A_DATA_TYPE_SHAPE = 4,
};

// ktgl::S_G1A_HEADER, the timeType nibble at +0x0D.
enum KTGL_G1A_TIME_TYPE
{
	KTGL_G1A_TIME_TYPE_FRAME = 0,
	KTGL_G1A_TIME_TYPE_SEC = 1,
};

template<bool bBigEndian>
struct G1AHeader
{
	uint16_t animationType;		// dataType | (timeType/isScalable/isQuaternion << 8)
	float duration;				// S_G1A_HEADER::maxTime
	uint32_t dataSectionOffset;	// S_G1A_HEADER::headQWC * 0x10
	uint16_t boneInfoCount;		// object count of level 0's jump table
	uint16_t boneMaxID;
	uint32_t chunkVersion;

	// Split out of animationType so callers do not have to know the packing.
	uint8_t dataType() const { return static_cast<uint8_t>(animationType & 0xFF); }
	uint8_t timeType() const { return static_cast<uint8_t>((animationType >> 8) & 0x0F); }
	bool isQuaternion() const { return ((animationType >> 8) & 0x20) != 0; }

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

		// Only a MODEL animation describes bones. Camera, light and shape files
		// use the same container and the same header, and are told apart only by
		// dataType; their objects carry opcodes 101/102, 201/202/203 and 401,
		// none of which the bone opcode table below accepts. Bailing out here
		// makes that explicit instead of quietly walking a jump table of camera
		// objects and producing an animation with no bones in it.
		if (header.dataType() != KTGL_G1A_DATA_TYPE_MODEL)
			return;

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
			// Channel layout per opcode, taken from the engine's own switch in
			// ktgl::S_MODEL_MOTION_SET::GetModelElementsOf, which reads the
			// curves back one index at a time. Order is always scale, then
			// rotation, then translation.
			//
			//   0  nothing animated (bind pose)                        0 curves
			//   1  rotation xyz                                        3
			//   2  quaternion xyzw                                     4
			//   3  rotation xyz + translation xyz                      6
			//   4  quaternion xyzw + translation xyz                   7
			//   5  scale xyz + rotation xyz + translation xyz          9
			//   6  scale xyz + quaternion xyzw + translation xyz      10
			//   7  scale xyz + rotation xyz                            6
			//   8  scale xyz + quaternion xyzw                         7
			//
			// Odd opcodes carry a 3 component rotation, even ones a quaternion;
			// the engine reports which through S_MODEL_MOTION_SET's return value
			// and the file says so up front via the header's isQuaternion bit.
			//
			// Whatever an opcode omits comes from the bind pose, which the engine
			// takes from a 48 byte record per bone (scale, quat, translation) and
			// points the missing slot straight at. 7 and 8 are the mirror of 1 and
			// 2: they animate scale where those two animate nothing but rotation.
			//
			// The previous table here was wrong in three ways: opcode 1 was one
			// component short, and opcodes 3 and 5 fell through to the default and
			// dropped the whole bone. Opcodes 7 and 8 were also missing, and were
			// likewise dropping every bone that used them.
			int32_t componentCount = -1, indexs = -1, indexr = -1, indexl = -1;
			int32_t rotComponents = 4;
			switch (opcode)
			{
			case 0:		// bind pose, nothing to read
				continue;
			case 1:
				componentCount = 3;
				indexr = 0; rotComponents = 3;
				break;
			case 2:
				componentCount = 4;
				indexr = 0;
				break;
			case 3:
				componentCount = 6;
				indexr = 0; rotComponents = 3;
				indexl = 3;
				break;
			case 4:
				componentCount = 7;
				indexr = 0;
				indexl = 4;
				break;
			case 5:
				componentCount = 9;
				indexs = 0;
				indexr = 3; rotComponents = 3;
				indexl = 6;
				break;
			case 6:
				componentCount = 10;
				indexs = 0;
				indexr = 3;
				indexl = 7;
				break;
			case 7:
				componentCount = 6;
				indexs = 0;
				indexr = 3; rotComponents = 3;
				break;
			case 8:
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

			// The 3 component form is a rotation vector, not a quaternion with a
			// dropped w: the engine leaves q.w untouched for those opcodes and
			// does not normalise, so it cannot be reconstructed the way a packed
			// quaternion could. The Android build of the engine only ever runs
			// the quaternion path (CModelObjectSkeleton::ApplyMotion asserts
			// _motionData->IsQuaternion()), and it ships no vector to quaternion
			// converter, so the exact encoding is not pinned down here and no
			// sample using it was available to test against.
			//
			// Rather than emit a rotation that is probably wrong, skip just the
			// rotation and keep the rest. That is still a clear improvement:
			// these bones used to be dropped whole, losing their translation and
			// scale as well. Fill this in once a non quaternion sample turns up.
			if (indexr >= 0 && rotComponents != 4)
			{
				indexr = -1;
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

