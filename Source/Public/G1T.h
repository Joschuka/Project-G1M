#pragma once

#ifndef G1T_H
#define G1T_H

enum EG1TPlatform : uint32_t
{
	PS2 = 0x0,
	PS3 = 0x1,
	X360 = 0x2,
	NWii = 0x3,
	NDS = 0x4,
	N3DS = 0x5,
	PSVita = 0x6,
	Android = 0x7,
	iOS = 0x8,
	NWiiU = 0x9,
	Windows = 0xA,
	PS4 = 0xB,
	XOne = 0xC
};

template <bool bBigEndian>
struct G1TTextureInfo
{
	uint8_t mipSys;
	uint8_t textureFormat;
	uint8_t dxdy;
	uint8_t extraHeaderVersion;
	uint32_t headerSize = 0x8;

	uint32_t height, width;
	bool bNormalized = true;
	bool bSpecialCaseETC2 = false;

	G1TTextureInfo(BYTE* buffer, uint32_t& offset)
	{
		//Read basic info
		mipSys = *(uint8_t*)(buffer + offset);
		textureFormat = *(uint8_t*)(buffer + offset + 1);
		dxdy = *(uint8_t*)(buffer + offset + 2);
		extraHeaderVersion = *(uint8_t*)(buffer + offset + 7);
		offset += 8;

		//compute width and height
		height = pow(2, dxdy >> 4);
		width = pow(2, dxdy & 0xF);
		if (bBigEndian)
		{
			uint32_t temp = width;
			width = height;
			height = temp;
		}

		//Extra header
		if (extraHeaderVersion > 0)
		{
			uint32_t extraDataSize = *(uint32_t*)(buffer + offset);
			if (bBigEndian)
				LITTLE_BIG_SWAP(extraDataSize);
			headerSize += extraDataSize;
			offset += 12; //unk
			if (extraDataSize >= 0x10)
			{
				width = *(uint32_t*)(buffer + offset);
				if (bBigEndian)
					LITTLE_BIG_SWAP(width);
				offset += 4;
			}
			if (extraDataSize >= 0x14)
			{
				height = *(uint32_t*)(buffer + offset);
				if (bBigEndian)
					LITTLE_BIG_SWAP(height);
				offset += 4;
			}
		}
	}
};

template <bool bBigEndian>
struct G1THeader
{
	GResourceHeader<bBigEndian> sectionHeader;
	uint32_t tableOffset;
	uint32_t textureCount;
	EG1TPlatform platform;

	G1THeader(G1THeader* ptr) : G1THeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(tableOffset);
			LITTLE_BIG_SWAP(textureCount);
			LITTLE_BIG_SWAP(platform);
		}
	}
};

template <bool bBigEndian>
struct G1T
{
	std::vector<uint32_t> offsetList;
	G1T(BYTE* buffer, int bufferLen, CArrayList<noesisTex_t*>& noeTex, noeRAPI_t* rapi)
	{
		//Read header
		uint32_t offset = 0;
		G1THeader<bBigEndian> header = reinterpret_cast<G1THeader<bBigEndian>*>(buffer);
		offset = header.tableOffset;

		//Get offsets
		offsetList.resize(header.textureCount);
		memcpy(offsetList.data(), buffer + offset, 4 * header.textureCount);
		if (bBigEndian)
		{
			for (auto& offs : offsetList)
				LITTLE_BIG_SWAP(offs);
		}

		//Get operators ready
		//PS4 swizzle
		typedef void(*NoesisMisc_Untile1dThin_p)(uint8_t * pDest, const uint32_t destSize, const uint8_t * pSrc, const uint32_t srcSize, const uint32_t w, const uint32_t h, const uint32_t bitsPerTexel,bool isBC, noeRAPI_t * pRapi);
		NoesisMisc_Untile1dThin_p NoesisMisc_Untile1dThin = NULL;
		NoesisMisc_Untile1dThin = (NoesisMisc_Untile1dThin_p)g_nfn->NPAPI_GetUserExtProc("NoesisMisc_Untile1dThin");

		//Process textures
		for (auto i =0;i<offsetList.size(); i++)
		{
			uint32_t offs = offsetList[i];
			offset = header.tableOffset + offs;
			G1TTextureInfo<bBigEndian> texHeader = G1TTextureInfo<bBigEndian>(buffer,offset);

			bool bNormalized = true;
			bool bSpecialCaseETC = false;
			bool b3DSAlpha = false;
			bool bETCAlpha = false;
			std::string rawFormat ="";
			int fourccFormat = -1;

			int32_t computedSize = -1;
			uint32_t mortonWidth = 0;
			uint32_t width = texHeader.width;
			uint32_t height = texHeader.height;

			switch (texHeader.textureFormat)
			{
			case 0x0:
				rawFormat = "r8g8b8a8";
				computedSize = width * height * 4;
				break;
			case 0x1:
				rawFormat = "b8g8r8a8";
				computedSize = width * height * 4;
				break;
			case 0x2:
				rawFormat = "r32";
				computedSize = width * height * 4;
				break;
			case 0x3:
				rawFormat = "r16g16b16a16";
				computedSize = width * height * 4;
				break;
			case 0x4:
				rawFormat = "r32g32b32a32";
				computedSize = width * height * 4;
				break;
			case 0x6:
				fourccFormat = FOURCC_DXT1;
				break;
			case 0x7:
				fourccFormat = FOURCC_DXT3;
				break;
			case 0x8:
				fourccFormat = FOURCC_DXT5;
				break;
			case 0x9:
			case 0xA:
				rawFormat = "b8g8r8a8";
				computedSize = width * height * 4;
				mortonWidth = 0x20;
				break;
			case 0xB:
				rawFormat = "r32";
				computedSize = width * height;
				break;
			case 0xD:
				rawFormat = "r32g32b32a32";
				computedSize = width * height;
				break;
			case 0xF:
				rawFormat = "a8";
				computedSize = width * height;
				break;
			case 0x10:
				fourccFormat = FOURCC_DXT1;
				mortonWidth = 0x4;
				break;
			case 0x12:
				fourccFormat = FOURCC_DXT5;
				mortonWidth = 0x8;
				break;
			case 0x34:
				rawFormat = "b5g6r5";
				computedSize = width * height * 2;
				break;
			case 0x36:
				rawFormat = "a4b4g4r4";
				computedSize = width * height * 2;
				break;
			case 0x3C:
				fourccFormat = FOURCC_DXT1;
				break;
			case 0x3D:
				fourccFormat = FOURCC_DXT1;
				break;
			case 0x47:
				rawFormat = "3DS_rgb";
				computedSize = width * height * 4;
				break;
			case 0x48:
				rawFormat = "3DS_rgb";
				computedSize = width * height * 4;
				b3DSAlpha = true;
				break;
			case 0x56:
				rawFormat = "ETC1_rgb";
				computedSize = width * height / 2;
				break;
			case 0x59:
				fourccFormat = FOURCC_DXT1;
				break;
			case 0x5B:
				fourccFormat = FOURCC_DXT5;
				break;
			case 0x5C:
				fourccFormat = FOURCC_ATI1;
				bNormalized = false;
				break;
			case 0x5D:
				fourccFormat = FOURCC_ATI2;
				bNormalized = false;
				break;
			case 0x5E:
				fourccFormat = FOURCC_BC6H;
				bNormalized = false;
				break;
			case 0x5F:
				fourccFormat = FOURCC_BC7;
				bNormalized = false;
				break;
			case 0x60:
				fourccFormat = FOURCC_DXT1;
				mortonWidth = 0x4;
				break;
			case 0x62:
				fourccFormat = FOURCC_DXT5;
				mortonWidth = 0x8;
				break;
			case 0x63:
				fourccFormat = FOURCC_BC4;
				mortonWidth = 0x4;
				bNormalized = false;
				break;
			case 0x64:
				fourccFormat = FOURCC_BC5;
				mortonWidth = 0x8;
				bNormalized = false;
				break;
			case 0x65:
				fourccFormat = FOURCC_BC6H;
				mortonWidth = 0x8;
				bNormalized = false;
				break;
			case 0x66:
				fourccFormat = FOURCC_BC7;
				mortonWidth = 0x8;
				bNormalized = false;
				break;
			case 0x6F:
				rawFormat = "ETC1_rgb";
				computedSize = width * height;
				bSpecialCaseETC = true;
				height *= 2;
				if (i < offsetList.size() - 1)
					offsetList[i + 1] = offsetList[i] + texHeader.headerSize + computedSize;
				break;
			case 0x71:
				rawFormat = "ETC1_rgba";
				computedSize = width * height;
				bETCAlpha = true;
				break;
			default:
				break;
			}

			bool bRaw = (rawFormat != "") ? true : false;
			BYTE* untiledTexData = nullptr;
			bool bShouldFreeUntiled = false;
			BYTE* texData = nullptr;

			//Get the data size
			uint32_t dataSize;
			if (computedSize >= 0)
				dataSize = computedSize;
			else
			{
				if (i < header.textureCount - 1)
					dataSize = offsetList[i + 1] - offsetList[i] - texHeader.headerSize;
				else
					dataSize = bufferLen - offsetList[i] - texHeader.headerSize - header.tableOffset;
			}

			//Swap endian for x360 textures
			if (header.platform == EG1TPlatform::X360)
			{
				uint16_t* tmp = (uint16_t*)(buffer + offset);
				for (auto i = 0; i < dataSize / 2; i++)
				{
					LITTLE_BIG_SWAP(tmp[i]);
				}
			}

			//Untile/Unswizzle the data if relevant
			if (mortonWidth > 0)
			{
				switch (header.platform)
				{
				case EG1TPlatform::PS4:
					untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize);
					if (bRaw)
						NoesisMisc_Untile1dThin(untiledTexData, dataSize, buffer + offset, dataSize, width, height, mortonWidth, 0, rapi);
					else
						NoesisMisc_Untile1dThin(untiledTexData, dataSize, buffer + offset, dataSize, width, height, mortonWidth, 1, rapi);
					break;
				case EG1TPlatform::NWiiU:
					untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize);
					break;
				default:
					untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize);
					if (bRaw)
						rapi->Image_MortonOrder(buffer + offset, untiledTexData, width, height, mortonWidth, 0);
					else
						rapi->Image_MortonOrder(buffer + offset, untiledTexData, width>>1, height>>2, mortonWidth, 1);
					break;
				}
			}

			//Decompress ETC
			if (!rawFormat.rfind("ETC",0))
			{
				untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize*8);
				if (bETCAlpha)
				{
					rapi->Image_DecodeETC(untiledTexData, buffer + offset, dataSize, width, height, "RGBA");
					dataSize *= 8;
				}
				else
					rapi->Image_DecodeETC(untiledTexData, buffer + offset, dataSize, width, height, "RGB");
				if (bSpecialCaseETC)
				{
					height /= 2;
					uint32_t alphaOffset = width * height * 4;
					for (auto j = 0; j < width * height; j++)
						untiledTexData[4 * j + 3] = untiledTexData[alphaOffset + 4 * j];
					dataSize *= 8;
				}
				rawFormat = "r8g8b8a8";
			}

			//Decompress 3DS
			if (!rawFormat.rfind("3DS", 0))
			{
				untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(width * height * 16);
				rapi->Image_DecodePICA200ETC(untiledTexData, buffer + offset, width, height, b3DSAlpha,0,0);
				flip_vertically(untiledTexData, width, height, 4);
				rawFormat = "r8g8b8a8";
			}

			if (untiledTexData)
				bShouldFreeUntiled = true;
			else
				untiledTexData = buffer + offset;
			//Decode the data
			if (bRaw)
			{
				texData = rapi->Noesis_ImageDecodeRaw(untiledTexData, dataSize, width, height, &rawFormat[0]);
			}
			else
			{
				if (bNormalized)
				{
					texData = rapi->Noesis_ConvertDXT(width, height, untiledTexData, fourccFormat);
				}
				else 
				{
					convertDxtExParams_t* params = (convertDxtExParams_t *)rapi->Noesis_UnpooledAlloc(sizeof(convertDxtExParams_t));
					params->ati2ZScale = 0.0;
					params->ati2NoNormalize = true;
					params->decodeAsSigned = false;
					params->resvBB = false;
					params->resvBC = false;
					memset(params->resv, 0, 15 * sizeof(TResvInt));
					texData = rapi->Noesis_ConvertDXTEx(width, height, untiledTexData, fourccFormat, dataSize, params, 0);
					rapi->Noesis_UnpooledFree(params);
				}
			}

			//Create the texture
			char texName[128];
			snprintf(texName, 128, "%d.dds", noeTex.Num());
			noesisTex_t* texture = rapi->Noesis_TextureAlloc(texName, width, height, texData, NOESISTEX_RGBA32);
			texture->shouldFreeData = true;
			texture->globalIdx = noeTex.Num();
			noeTex.Append(texture);
			//g_nfn->NPAPI_DebugLogStr(texName);
			if(bShouldFreeUntiled)
				rapi->Noesis_UnpooledFree(untiledTexData);
		}
	}
};


#endif // !G1T_H
