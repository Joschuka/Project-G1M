#pragma once

#ifndef G1M_G_MATERIAL
#define G1M_G_MATERIAL

template<bool bBigEndian>
struct G1MGTexture
{
	uint16_t index; //texture index in g1t file
	uint16_t layer; //TEXCOORD layer
	uint16_t textureType;
	uint16_t otherType;
	uint16_t tileModex;
	uint16_t tileModey;

	G1MGTexture(G1MGTexture* ptr) : G1MGTexture(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(index);
			LITTLE_BIG_SWAP(layer);
			LITTLE_BIG_SWAP(textureType);
			LITTLE_BIG_SWAP(otherType);
			LITTLE_BIG_SWAP(tileModex);
			LITTLE_BIG_SWAP(tileModey);
		}
	}

};

template<bool bBigEndian>
struct G1MGMaterial
{
	uint32_t textureCount;
	std::vector<G1MGTexture<bBigEndian>> g1mgTextures;
	G1MGMaterial(BYTE* buffer, uint32_t &offset)
	{
		offset += 4; //skipping unk1
		textureCount = *(uint32_t*)(buffer + offset);
		if (bBigEndian)
			LITTLE_BIG_SWAP(textureCount);
		offset += 12;
		for (auto i=0; i < textureCount; i++)
		{
			G1MGTexture text = reinterpret_cast<G1MGTexture<bBigEndian>*>(buffer + offset);
			g1mgTextures.push_back(std::move(text));
			offset += sizeof(G1MGTexture<bBigEndian>);
		}
	}
};

#endif // !G1M_G_MATERIAL
