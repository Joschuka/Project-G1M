#pragma once

#ifndef G1T_H
#define G1T_H

enum class EG1TPlatform : uint32_t
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
	XOne = 0xC,
	NSwitch = 0x10,
};

enum class EG1TASTCFormat : uint8_t //Most of these are educated guesses based on a few confirmed ones
{
	ASTC_4_4 = 0x0, //Confirmed
	ASTC_5_4 = 0x1,
	ASTC_5_5 = 0x2,
	ASTC_6_5 = 0x3,
	ASTC_6_6 = 0x4,
	ASTC_8_5 = 0x5,
	ASTC_8_6 = 0x6,
	ASTC_8_8 = 0x7, //Confirmed
	ASTC_10_5 = 0x8,
	ASTC_10_6 = 0x9,
	ASTC_10_8 = 0xA,
	ASTC_10_10 = 0xB,
	ASTC_12_10 = 0xC,
	ASTC_12_12 = 0xD,
};

template <bool bBigEndian>
struct G1TASTCExtraInfo
{
	uint16_t unk0;
	uint16_t unk1;
	EG1TASTCFormat format;
	uint8_t unk2;
	uint16_t unk3;

	G1TASTCExtraInfo(G1TASTCExtraInfo* ptr) : G1TASTCExtraInfo(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(unk0);
			LITTLE_BIG_SWAP(unk1);
			LITTLE_BIG_SWAP(unk3);
		}
	}
};

template <bool bBigEndian>
struct G1TTextureInfo
{
	uint8_t mipSys;
	uint32_t subsys, mip_count;
	uint8_t textureFormat;
	uint8_t dxdy;
	uint8_t unk1;
	uint8_t unk2;
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

		//compute mip_count and subsys
		mip_count = ((mipSys >> 4) & 0xF);
		subsys = ((mipSys >> 0) & 0xF);

		//compute width and height
		height = pow(2, dxdy >> 4);
		width = pow(2, dxdy & 0xF);
		if (bBigEndian)
		{
			uint32_t temp = width;
			width = height;
			height = temp;

			temp = subsys;
			subsys = mip_count;
			mip_count = temp;
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
	uint32_t ASTCExtraInfoSize;

	G1THeader(G1THeader* ptr) : G1THeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(tableOffset);
			LITTLE_BIG_SWAP(textureCount);
			LITTLE_BIG_SWAP(platform);
			LITTLE_BIG_SWAP(ASTCExtraInfoSize);
		}
	}
};

template <bool bBigEndian>
struct G1T
{
	std::vector<uint32_t> offsetList;
	std::vector<G1TASTCExtraInfo<bBigEndian>> ASTCExtraInfoList;
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
		offset += header.textureCount * 4;

		//Get ASTC extra info
		for (auto i = 0; i < header.ASTCExtraInfoSize/4; i++)
		{
			ASTCExtraInfoList.push_back(reinterpret_cast<G1TASTCExtraInfo<bBigEndian>*>(buffer + offset + 8*i));
		}

		//Get operators ready
		//PS4 swizzle
		typedef void(*NoesisMisc_Untile1dThin_p)(uint8_t* pDest, const uint32_t destSize, const uint8_t* pSrc, const uint32_t srcSize, const uint32_t w, const uint32_t h, const uint32_t bitsPerTexel, bool isBC, noeRAPI_t* pRapi);
		NoesisMisc_Untile1dThin_p NoesisMisc_Untile1dThin = NULL;
		NoesisMisc_Untile1dThin = (NoesisMisc_Untile1dThin_p)g_nfn->NPAPI_GetUserExtProc("NoesisMisc_Untile1dThin");

		//Process textures
		for (auto i = 0; i < offsetList.size(); i++)
		{
			uint32_t offs = offsetList[i];
			offset = header.tableOffset + offs;
			G1TTextureInfo<bBigEndian> texHeader = G1TTextureInfo<bBigEndian>(buffer, offset);

			bool bNormalized = true;
			bool bSpecialCaseETC = false;
			bool b3DSAlpha = false;
			bool bETCAlpha = false;
			bool bNeedsX360EndianSwap = false;
			std::string rawFormat = "";
			int fourccFormat = -1;

			int32_t computedSize = -1;
			uint32_t mortonWidth = 0;
			uint32_t width = texHeader.width;
			uint32_t height = texHeader.height;
			uint32_t originalSize = computedSize;
			uint32_t pvrtcBpp = 0;
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
				bNeedsX360EndianSwap = true;
				break;
			case 0x7:
				fourccFormat = FOURCC_DXT3;
				break;
			case 0x8:
				fourccFormat = FOURCC_DXT5;
				break;
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
			case 0x35:
				computedSize = width * height * 2;
				rawFormat = "a1b5g5r5";
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
			case 0x57:
				rawFormat = "PVRTC";
				computedSize = width * height / 4;
				pvrtcBpp = 2;
				break;
			case 0x58:
				computedSize = width * height / 2;
				rawFormat = "PVRTC";
				pvrtcBpp = 4;
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
				originalSize = computedSize;
				for (auto j = 1; j < texHeader.mip_count; j++) //Mipmap size, skip the first entry which is the full-sized texture
				{
					originalSize = originalSize / 4;
					computedSize += originalSize;
				}
				height *= 2;
				if (i < offsetList.size() - 1)
					offsetList[i + 1] = offsetList[i] + texHeader.headerSize + computedSize;
				break;
			case 0x71:
				rawFormat = "ETC1_rgba";
				computedSize = width * height;
				bETCAlpha = true;
				break;
			case 0x7D:
				rawFormat = "ASTC";
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

			if (header.platform == EG1TPlatform::X360)
			{
				if (bNeedsX360EndianSwap || mortonWidth > 0)
				{	//Swap endian for x360 textures
					uint16_t* tmp = (uint16_t*)(buffer + offset);
					for (auto i = 0; i < dataSize / 2; i++)
					{
						LITTLE_BIG_SWAP(tmp[i]);
					}
				}
			}

			//Untile/Unswizzle the data if relevant
			if (mortonWidth > 0)
			{
				switch (header.platform)
				{
				case EG1TPlatform::X360:
				{
					untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize);
					if (bRaw)
						rapi->Noesis_UntileImageRAW(untiledTexData, buffer + offset, dataSize, width, height, mortonWidth);
					else
						rapi->Noesis_UntileImageDXT(untiledTexData, buffer + offset, dataSize, width, height, mortonWidth * 2);
					break;
				}
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
				case EG1TPlatform::NSwitch:
				{
					untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize * 4);
					int blockWidth = 4;
					int blockHeight = (height <= 256) ? 3 : 4;
					blockHeight = (height <= 128) ? 2 : blockHeight;
					blockHeight = (height <= 64) ? 1 : blockHeight;
					int widthInBlocks = (width + (blockWidth - 1)) / blockWidth;
					int temp = (height + (blockHeight - 1));
					int	heightInBlocks = (height + (blockHeight - 1)) / blockHeight;
					int maxBlockHeight = rapi->Image_TileCalculateBlockLinearGOBBlockHeight(height, blockHeight);
					rapi->Image_UntileBlockLinearGOBs(untiledTexData, dataSize, buffer + offset, dataSize, widthInBlocks, heightInBlocks, maxBlockHeight, mortonWidth * 2);
					break;
				}
				default:
					untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize);
					if (bRaw)
						rapi->Image_MortonOrder(buffer + offset, untiledTexData, width, height, mortonWidth, 0);
					else
						rapi->Image_MortonOrder(buffer + offset, untiledTexData, width >> 1, height >> 2, mortonWidth, 1);
					break;
				}
			}

			//Decompress PVRTC
			if (!rawFormat.rfind("PVRTC", 0))
			{
				untiledTexData = rapi->Image_DecodePVRTC(buffer + offset, dataSize, width, height, pvrtcBpp);
				rawFormat = "r8g8b8a8";
				dataSize *= 16;
			}

			//Decompress ASTC
			if (!rawFormat.rfind("ASTC", 0))
			{				
				switch (ASTCExtraInfoList[i].format)
				{
				case EG1TASTCFormat::ASTC_4_4:
					rawFormat = "ASTC_4_4";
					break;
				case EG1TASTCFormat::ASTC_5_4:
					rawFormat = "ASTC_5_4";
					break;
				case EG1TASTCFormat::ASTC_5_5:
					rawFormat = "ASTC_5_5";
					break;
				case EG1TASTCFormat::ASTC_6_5:
					rawFormat = "ASTC_6_5";
					break;
				case EG1TASTCFormat::ASTC_6_6:
					rawFormat = "ASTC_6_6";
					break;
				case EG1TASTCFormat::ASTC_8_5:
					rawFormat = "ASTC_8_5";
					break;
				case EG1TASTCFormat::ASTC_8_6:
					rawFormat = "ASTC_8_6";
					break;
				case EG1TASTCFormat::ASTC_8_8:
					rawFormat = "ASTC_8_8";
					break;
				case EG1TASTCFormat::ASTC_10_5:
					rawFormat = "ASTC_10_5";
					break;
				case EG1TASTCFormat::ASTC_10_6:
					rawFormat = "ASTC_10_6";
					break;
				case EG1TASTCFormat::ASTC_10_8:
					rawFormat = "ASTC_10_8";
					break;
				case EG1TASTCFormat::ASTC_10_10:
					rawFormat = "ASTC_10_10";
					break;
				case EG1TASTCFormat::ASTC_12_10:
					rawFormat = "ASTC_12_10";
					break;
				case EG1TASTCFormat::ASTC_12_12:
					rawFormat = "ASTC_12_12";
					break;
				default:
					assert(0);
				}

				std::regex reg("_");
				std::sregex_token_iterator iter(rawFormat.begin(), rawFormat.end(), reg, -1);
				std::sregex_token_iterator end;
				std::vector<std::string> blockSizes(iter, end);
				int ASTCblock1 = stoi(blockSizes[1]);
				int ASTCblock2 = stoi(blockSizes[2]);

				int pBlockDims[3] = { ASTCblock1, ASTCblock2, 1 };
				int pImageSize[3] = { width, height, 1 };
				untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize * 16);
				rapi->Image_DecodeASTC(untiledTexData, buffer + offset, dataSize, pBlockDims, pImageSize);
				rawFormat = "r8g8b8a8";
				dataSize *= 16;
			}

			//Decompress ETC
			if (!rawFormat.rfind("ETC", 0))
			{
				untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(dataSize * 8);
				if (bETCAlpha)
				{
					rapi->Image_DecodeETC(untiledTexData, buffer + offset, dataSize, width, height, "RGBA");
					dataSize *= 8;
				}
				else
				{
					rapi->Image_DecodeETC(untiledTexData, buffer + offset, dataSize, width, height, "RGB");
					dataSize *= 8;
				}
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
				rapi->Image_DecodePICA200ETC(untiledTexData, buffer + offset, width, height, b3DSAlpha, 0, 0);
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
					convertDxtExParams_t* params = (convertDxtExParams_t*)rapi->Noesis_UnpooledAlloc(sizeof(convertDxtExParams_t));
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

			if (bShouldFreeUntiled)
				rapi->Noesis_UnpooledFree(untiledTexData);
		}
	}
};


#endif // !G1T_H