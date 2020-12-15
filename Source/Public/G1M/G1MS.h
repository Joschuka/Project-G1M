#pragma once

#ifndef G1MS_
#define G1MS_

template<bool bBigEndian>
struct G1MSHeader
{
	uint32_t jointInfoOffset;
	uint32_t unk1;
	uint16_t jointCount;
	uint16_t jointIndicesCount;
	uint16_t layer; //May be skeleton type too
	uint16_t padding;

	G1MSHeader(G1MSHeader* ptr) : G1MSHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(jointInfoOffset);
			LITTLE_BIG_SWAP(jointCount);
			LITTLE_BIG_SWAP(jointIndicesCount);
			LITTLE_BIG_SWAP(layer);
		}
	}
};

struct G1MSJoint
{	
	RichVec3 scale;	
	uint32_t parent;
	RichQuat rotation;
	RichVec3 position;
	float wCoord;
};

template<bool bBigEndian>
struct G1MS
{
	bool bIsInternal = true;
	uint32_t skeletonLayer;
	std::map<uint16_t, uint16_t> localIDToGlobalID;
	std::map<uint16_t, uint16_t> globalIDToLocalID;
	std::vector<G1MSJoint> joints;
	std::vector<uint16_t> jointLocalIndexToExtract;
	G1MS(BYTE* buffer, size_t startOffset)
	{
		size_t offset = startOffset;
		GResourceHeader sectionHeader = reinterpret_cast<GResourceHeader<bBigEndian>*>(buffer + offset);
		offset += 12;
		//Read header
		G1MSHeader<bBigEndian> header = reinterpret_cast<G1MSHeader<bBigEndian>*>(buffer + offset);
		skeletonLayer = header.layer;
		
		if (sectionHeader.chunkVersion > 0x30303330) //No global indices before it seems, need to process differently
		{
			//Global and local indices map
			offset += sizeof(G1MSHeader<bBigEndian>);
			//Check if the skeleton is internal or not using the 0x80000000 flag
			uint32_t parentFlag = *reinterpret_cast<uint32_t*> (buffer + startOffset + header.jointInfoOffset + 12);
			if (bBigEndian)
				LITTLE_BIG_SWAP(parentFlag);
			bIsInternal = !(parentFlag == 0x80000000);
			for (auto i = 0; i < header.jointIndicesCount; i++)
			{
				uint16_t localID = *reinterpret_cast<uint16_t*> (buffer + offset + 2 * i);
				if (localID == 0xFFFF)
					continue;
				if (bBigEndian)
					LITTLE_BIG_SWAP(localID);
				localIDToGlobalID[localID] = i;
				globalIDToLocalID[i] = localID;
			}
		}
		else
		{
			offset = startOffset + header.jointInfoOffset;
			for (auto i = 0; i < header.jointCount; i++)
			{
				localIDToGlobalID[i] = i;
				globalIDToLocalID[i] = i;
			}

		}
		//Joints
		joints.resize(header.jointCount);
		memcpy(joints.data(), buffer + startOffset + header.jointInfoOffset, header.jointCount * sizeof(G1MSJoint));
		if (bBigEndian)
		{
			for (auto& j : joints)
			{
				j.scale.ChangeEndian();
				LITTLE_BIG_SWAP(j.parent);
				j.rotation.ChangeEndian();
				j.position.ChangeEndian();
				LITTLE_BIG_SWAP(j.wCoord);
			}
		}
	}
};

#endif // !G1MS_


