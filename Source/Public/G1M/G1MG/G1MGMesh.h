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

// ---------------------------------------------------------------------------
// This is ktgl::S_G1M_GEOMETRY_SUBSET_H, 36 bytes, and the engine declares it
// field for field:
//
//     skelID, usage, lodLevel, colPrimsetNum, alpPrimsetNum,
//     selfMatrixDataStartIndex,   selfMatrixDataNum,
//     parentMatrixDataStartIndex, parentMatrixDataNum
//
// Three of the community names this plugin inherited were on the wrong fields,
// which is why they are corrected here rather than just annotated:
//
//     +0x00  was "LOD"             is skelID      <- NOT the LOD level
//     +0x04  was "Group"           is usage
//     +0x08  was "GroupEntryIndex" is lodLevel    <- the real LOD level
//     +0x0C  was "submeshCount1"   is colPrimsetNum (opaque pass)
//     +0x10  was "submeshCount2"   is alpPrimsetNum (alpha pass)
//     +0x14  was "lodRangeStart"   is selfMatrixDataStartIndex
//     +0x18  was "lodRangeLength"  is selfMatrixDataNum
//     +0x1C  unread                   parentMatrixDataStartIndex
//     +0x20  unread                   parentMatrixDataNum
//
// So the long standing "type 53 / type 61" submesh split is simply the opaque
// and alpha primset counts, and the two trailing unknowns are the parent matrix
// range. The fields are renamed outright rather than aliased; the only consumer
// was the LOD naming block in Source.cpp, which has been updated with them.
//
// Source: ktgl::S_G1M_GEOMETRY_SUBSET_H as declared by the engine. See
// samples/G1M.bt for the surrounding geometry headers and their provenance.
// ---------------------------------------------------------------------------
template <bool bBigEndian>
struct G1MGMeshGroup
{
	uint32_t skelID;					// was LOD
	uint32_t usage;						// was Group
	uint32_t lodLevel;					// was GroupEntryIndex; the actual LOD
	uint32_t colPrimsetNum;				// was submeshCount1; opaque pass
	uint32_t alpPrimsetNum;				// was submeshCount2; alpha pass
	uint32_t selfMatrixDataStartIndex;	// was lodRangeStart
	uint32_t selfMatrixDataNum;			// was lodRangeLength
	uint32_t parentMatrixDataStartIndex;	// present in the file, never read before
	uint32_t parentMatrixDataNum;

	std::vector<G1MGMesh<bBigEndian>> meshes;

	G1MGMeshGroup(BYTE* buffer, uint32_t& offset,uint32_t version)
	{
		//Could have done a header struct here but it's easier to get to the attributes
		skelID = usage = lodLevel = colPrimsetNum = alpPrimsetNum = 0;
		selfMatrixDataStartIndex = selfMatrixDataNum = 0;
		parentMatrixDataStartIndex = parentMatrixDataNum = 0;

		if (version > 0x30303330)
		{
			skelID = *reinterpret_cast<uint32_t*>(buffer + offset);
			usage = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
			lodLevel = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
			colPrimsetNum = *reinterpret_cast<uint32_t*>(buffer + offset + 12);
			alpPrimsetNum = *reinterpret_cast<uint32_t*>(buffer + offset + 16);
			if (version > 0x30303430)
			{
				selfMatrixDataStartIndex = *reinterpret_cast<uint32_t*>(buffer + offset + 20);
				selfMatrixDataNum = *reinterpret_cast<uint32_t*>(buffer + offset + 24);
				parentMatrixDataStartIndex = *reinterpret_cast<uint32_t*>(buffer + offset + 28);
				parentMatrixDataNum = *reinterpret_cast<uint32_t*>(buffer + offset + 32);
				offset += 36;
			}
			else
			{
				// The old code here read "lodRangeStart, lodRangeLength = 0",
				// which is a comma expression that only assigns the last one.
				// Everything is zeroed above now, so the intent holds.
				offset += 20;
			}
		}
		else
		{
			skelID = *reinterpret_cast<uint32_t*>(buffer + offset);
			colPrimsetNum = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
			alpPrimsetNum = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
			offset += 12;
		}

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(skelID);
			LITTLE_BIG_SWAP(usage);
			LITTLE_BIG_SWAP(lodLevel);
			LITTLE_BIG_SWAP(colPrimsetNum);
			LITTLE_BIG_SWAP(alpPrimsetNum);
			LITTLE_BIG_SWAP(selfMatrixDataStartIndex);
			LITTLE_BIG_SWAP(selfMatrixDataNum);
			LITTLE_BIG_SWAP(parentMatrixDataStartIndex);
			LITTLE_BIG_SWAP(parentMatrixDataNum);
		}
		for (auto i = 0; i < colPrimsetNum + alpPrimsetNum; i++)
		{
			meshes.push_back(G1MGMesh<bBigEndian>(buffer, offset));
		}
	}
};

#endif // !G1M_G_MESH
