#pragma once

#ifndef G1M_G_SUBM
#define G1M_G_SUBM

template <bool bBigEndian>
struct G1MGSubmesh
{
	uint32_t submeshType; //53 or 61
	uint32_t vertexBufferIndex;
	uint32_t bonePaletteIndex;
	uint32_t unk1; //mat.palID according to vago's research
	uint32_t unk2;
	uint32_t unk3;
	uint32_t materialIndex;
	uint32_t indexBufferIndex;
	uint32_t unk4;
	uint32_t indexBufferPrimType;
	uint32_t vertexBufferOffset;
	uint32_t vertexCount;
	uint32_t indexBufferOffset;
	uint32_t indexCount;

	G1MGSubmesh(G1MGSubmesh* ptr) : G1MGSubmesh(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(submeshType);
			LITTLE_BIG_SWAP(vertexBufferIndex);
			LITTLE_BIG_SWAP(bonePaletteIndex);
			LITTLE_BIG_SWAP(materialIndex);
			LITTLE_BIG_SWAP(indexBufferIndex);
			LITTLE_BIG_SWAP(indexBufferPrimType);
			LITTLE_BIG_SWAP(vertexBufferOffset);
			LITTLE_BIG_SWAP(vertexCount);
			LITTLE_BIG_SWAP(indexBufferOffset);
			LITTLE_BIG_SWAP(indexCount);
		}
	}
};

#endif // !G1M_G_SUBM