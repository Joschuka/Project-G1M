#pragma once

#ifndef KHM_H
#define KHM_H


template <bool bBigEndian>
struct KHMHeader
{
	char magic[4];
	uint32_t version;
	uint32_t unk;
	uint32_t size;
	uint16_t width;
	uint16_t height;
	float floorLevel;
	float midLevel;
	float ceilingLevel;

	KHMHeader(KHMHeader* ptr) : KHMHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(version);
			LITTLE_BIG_SWAP(size);
			LITTLE_BIG_SWAP(width);
			LITTLE_BIG_SWAP(height);
			LITTLE_BIG_SWAP(floorLevel);
			LITTLE_BIG_SWAP(midLevel);
			LITTLE_BIG_SWAP(ceilingLevel);
		}
	}
};

template <bool bBigEndian>
struct KHM
{
	KHM(BYTE* buffer, int bufferLen, CArrayList<noesisTex_t*>& noeTex, noeRAPI_t* rapi)
	{
		//Read header
		uint32_t offset = 0;
		KHMHeader<bBigEndian> header = reinterpret_cast<KHMHeader<bBigEndian>*>(buffer);
		offset += sizeof(KHMHeader<bBigEndian>);
		std::string rawFormat = "r8g8b8";
		header.width += 1;
		header.height += 1;
		int dataSize = header.width * header.height * 3;
		
		BYTE* BWTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize);
		for (auto i = 0; i < header.width * header.height; i++)
		{
			uint8_t value = (uint8_t)int(255 * (float(*(uint32_t*)(buffer + offset))) / 0xFFFFFFFF);
			BWTexData[3*i] = value;
			BWTexData[3*i + 1] = value;
			BWTexData[3*i + 2] = value;
			offset += 4;

		}
		BYTE* texData = rapi->Noesis_ImageDecodeRaw(BWTexData, dataSize, header.width, header.height, &rawFormat[0]);;

		//Create the texture
		char texName[128];
		snprintf(texName, 128, "%d.dds", noeTex.Num());
		noesisTex_t* texture = rapi->Noesis_TextureAlloc(texName, header.width, header.height, texData, NOESISTEX_RGBA32);
		texture->shouldFreeData = true;
		texture->globalIdx = noeTex.Num();
		noeTex.Append(texture);
		//Free the temp data
		rapi->Noesis_UnpooledFree(BWTexData);
	}
};


#endif // !KHM_H
