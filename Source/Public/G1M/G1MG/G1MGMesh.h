#pragma once

#ifndef G1M_G_MESH
#define G1M_G_MESH

template <bool bBigEndian>
struct G1MGMesh
{
	//char name[16];
	uint16_t meshType;
	//uint16_t unk1;
	uint32_t externalID;
	uint32_t indexCount;
	std::vector<uint32_t> indices;
	G1MGMesh(BYTE* buffer, uint32_t& offset)
	{
		offset += 16; //we don't care about the name for importing
		meshType = *reinterpret_cast<uint16_t*>(buffer + offset);
		externalID = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
		indexCount = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
		offset += 12;
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(meshType);
			LITTLE_BIG_SWAP(externalID);
			LITTLE_BIG_SWAP(indexCount);
		}
		if (indexCount > 0)
		{
			indices.resize(indexCount);
			memcpy(indices.data(), buffer + offset, indexCount * 4);
			if (bBigEndian)
			{
				for (auto& index : indices)
					LITTLE_BIG_SWAP(index);
			}
			offset += indexCount * 4;
		}
		else 
			offset += 4;
	}
};

template <bool bBigEndian>
struct G1MGMeshGroup
{
	uint32_t LOD;
	uint32_t Group;
	uint32_t GroupEntryIndex;
	uint32_t submeshCount1; //number of submeshes with 53 id
	uint32_t submeshCount2; //number of submeshes with 61 id
	uint32_t lodRangeStart;
	uint32_t lodRangeLength;
	//uint32_t unk1;
	//uint32_t unk2;
	std::vector<G1MGMesh<bBigEndian>> meshes;

	G1MGMeshGroup(BYTE* buffer, uint32_t& offset,uint32_t version)
	{
		//Could have done a header struct here but it's easier to get to the attributes
		if (version > 0x30303330)
		{
			LOD = *reinterpret_cast<uint32_t*>(buffer + offset);
			Group = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
			GroupEntryIndex = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
			submeshCount1 = *reinterpret_cast<uint32_t*>(buffer + offset + 12);
			submeshCount2 = *reinterpret_cast<uint32_t*>(buffer + offset + 16);
			if (version > 0x30303430)
			{
				lodRangeStart = *reinterpret_cast<uint32_t*>(buffer + offset + 20);
				lodRangeLength = *reinterpret_cast<uint32_t*>(buffer + offset + 24);
				offset += 36;
			}
			else
			{
				lodRangeStart, lodRangeLength = 0;
				offset += 20;
			}
		}
		else
		{
			LOD = *reinterpret_cast<uint32_t*>(buffer + offset);
			submeshCount1 = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
			submeshCount2 = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
			Group, GroupEntryIndex, lodRangeStart, lodRangeLength = 0;
			offset += 12;
		}
		
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(LOD);
			LITTLE_BIG_SWAP(Group);
			LITTLE_BIG_SWAP(GroupEntryIndex);
			LITTLE_BIG_SWAP(submeshCount1);
			LITTLE_BIG_SWAP(submeshCount2);
			LITTLE_BIG_SWAP(lodRangeStart);
			LITTLE_BIG_SWAP(lodRangeLength);
		}
		for (auto i = 0; i < submeshCount1 + submeshCount2; i++)
		{
			meshes.push_back(G1MGMesh<bBigEndian>(buffer, offset));
		}
	}
};

#endif // !G1M_G_MESH
