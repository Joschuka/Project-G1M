#pragma once

#ifndef G1T_H
#define G1T_H

///////////////////////////////
//           ENUMS           //
///////////////////////////////

enum class PLATFORM : uint32_t
{
	                // Endian             Swizzle
	DX9     = 0x00, // Little Endian - ❎ No known g1t files to test (aka x32)
	PS3     = 0x01, // Big Endian    - ❎ didnt find sizzle
	XB2     = 0x02, // Big Endian    - ✅ X360 Working swizzle
	RVL     = 0x03, // Big Endian    - ✅ NWii Working swizzle
    PSP     = 0x04, // Little Endian - ❎ No known g1t files to test
	CTR     = 0x05, // Little Endian - ❎ N3DS didnt find sizzle
	NGP     = 0x06, // Little Endian - ✅ PSVita Working swizzle (they have palette textures but were never found)
	AND     = 0x07, // Little Endian - ❎ Android No known swizzle
	IOS     = 0x08, // Little Endian - ❎ No known swizzle
	CAFE    = 0x09, // Big Endian    - ✅ NWiiU Working swizzle
	DX11    = 0x0A, // Little Endian - ✅ Windows / Mac Working swizzle (aka x64)
	PS4     = 0x0B, // Little Endian - ✅ Working swizzle (aka orbis)
    XB3     = 0x0C, // Little Endian - 🔲 Xbox One 
    NACL    = 0x0D, //               - 🔲 Google Native Client (chrome os)
	DX12    = 0x0E, // Little Endian - ✅ Working swizzle
    WEBGL   = 0x0F, //               - 🔲 Web client
	NX      = 0x10, // Little Endian - ✅ Switch Working swizzle
  //???     = 0x11, //               - 🔲 Switch Lite? Oculus?
  //???     = 0x12, //               - 🔲 XB4? Xbox x? aka Chuckwalla
	PS5     = 0x13, // Little Endian - ✅ Working-ish swizzle (some issues) (aka perspero)
  //XBXS    = 0x14, //               - 🔲 XB4? Xbox x? aka Chuckwalla
};

const char* PLATFORM_STR[] = 
{
	"DX9",
	"PS3",
	"X360",
	"NWii",
	"PSP",
	"N3DS",
	"PSVita",
	"Android",
	"iOS",
	"NWiiU",
	"DX11",
	"PS4",
	"XOne",
	"Chrome",
	"DX12",
	"WebGL",
	"NSwitch",
	"",
	"",
	"PS5",
	"",
	"",
	"",
	"",
	""
};

enum class TEX_EX_TYPE : uint16_t
{
	// These are sometime directly hardcoded to the client.
	// Meaning that they don't appear in the file, 
	// but are hardcoded on every g1t the game reads
	ENVMAP                = 0x00, // none
    IBL_SPECULAR          = 0x01, // channel swap Can also mean don't swizzle in other games
    IBL_DIFFUSE           = 0x02, // NO_ALPHA Unsure if this is found here
	NORMALMAP             = 0x03, // Found on normal maps
    IBL_SHADOW            = 0x04, // COLOR_TEST Ive seen on 4 layer arrays, maybe a single color from each slice RGBA?
	TLUT                  = 0x05, // N3DS_Wii_SWIZZLE Don't need the data on these for the swizzle
	CAFE_SWIZZLE          = 0x06, // WiiU Reads data but likely doesn't need it
	PS4_PLANE_ARRAY       = 0x07, // PS4_PLANE_ARRAY No idea. Might also be a KTID? These textures have crazy high layers too
	SH_IRRADIANCE_VOLUME  = 0x08, // PSVITA_PS5_SWIZZLE
	CUBICARRAY_SHADOW     = 0x09, // 
	PLANARARRAY_SHADOW    = 0x0A, // 
	CUBICARRAY_ESM        = 0x0B, //
	PLANARARRAY_ESM       = 0x0C, //
	METALNESSMAP          = 0x0D, //
	AMBIENT_SPECULAR      = 0x0E, //
	SDF                   = 0x0F, //
	TONEMAP               = 0x10, //
	LIGHTMAP              = 0x11, //
	//???                 = 0x12, //
	//???                 = 0x13, //
	//???                 = 0x14, //
	ASTC_TYPE             = 0x15
};

const char* EX_TYPE_STR[] = 
{
	"NONE",
	"CHANNEL_SWAP",
	"NO_ALPHA",
	"NORMAL_MAP",
	"COLOR_TEST",
	"N3DS_Wii_SWIZZLE",
	"WiiU_SWIZZLE",
	"PS4_PLANE_ARRAY",
	"PSVITA_PS5_SWIZZLE",
	"CUBICARRAY_SHADOW",
	"PLANARARRAY_SHADOW",
	"CUBICARRAY_ESM",
	"PLANARARRAY_ESM",
	"METALNESSMAP",
	"AMBIENT_SPECULAR",
	"SDF",
	"TONEMAP",
	"LIGHTMAP",
	"",
	"",
	"",
	"ASTC_TYPE",
	"",
	"",
	"",
	"",
	""
};

enum class EX_SWIZZLE_TYPE : uint8_t
{
	NONE            = 0x00,
	DX12_64kb       = 0x01,
  //???             = 0x02,
	ZLIB_COMPRESSED = 0x03  // ver 66 and above
};

const char* EX_SWIZZLE_TYPE_STR[] = 
{
	"NONE",
	"DX12_64kb",
	"",
	"ZLIB_COMPRESSED",
	"",
	"",
	"",
	""
};

enum class S_GT1_LOAD_TYPE : uint8_t
{
	PLANAR      = 0x0,
	CUBE        = 0x1,
	VOLUME      = 0x2,
	PLANE_ARRAY = 0x3,
	CUBE_ARRAY  = 0x4     // ver 63 and above
};

const char* LOAD_TYPE_STR[] = 
{
	"PLANAR",
	"CUBE",
	"VOLUME",
	"PLANE_ARRAY",
	"CUBE_ARRAY",
	"",
	"",
	"",
	"",
	"",
};

enum class EG1TASTCFormat : uint32_t
{
	NONE       = 0xFF,
	ASTC_4x4   = 0x0,
	ASTC_5x4   = 0x1,
	ASTC_5x5   = 0x2,
	ASTC_6x5   = 0x3,
	ASTC_6x6   = 0x4,
	ASTC_8x5   = 0x5,
	ASTC_8x6   = 0x6,
	ASTC_8x8   = 0x7,
	ASTC_10x5  = 0x8,
	ASTC_10x6  = 0x9,
	ASTC_10x8  = 0xA,
	ASTC_10x10 = 0xB,
	ASTC_12x10 = 0xC,
	ASTC_12x12 = 0xD,
};

const char* ASTC_FORMAT_STR[] = 
{ 
	"ASTC_4x4", 
	"ASTC_5x4", 
	"ASTC_5x5", 
	"ASTC_6x5",
	"ASTC_6x6",
	"ASTC_8x5",
	"ASTC_8x6",
	"ASTC_8x8",
	"ASTC_10x5",
	"ASTC_10x6",
	"ASTC_10x8",
	"ASTC_10x10",
	"ASTC_12x10",
	"ASTC_12x12",
	"",
	"",
	"",
	"",
	""
};

enum class KTGL_GD_COLOR_SPACE_TYPE : uint8_t
{
	NONE = 0x0,
	sRGB_FROM_LINEAR = 0x1,
};

const char* COLOR_SPACE_STR[] = 
{
	"NONE",
	"sRGB_FROM_LINEAR",
	"",
	"",
	"",
	"",
	""
};

///////////////////////////////
//     HELPER FUNCTIONS      //
///////////////////////////////

// Helper function to swap nibbles
void swapNibbles(uint8_t* a, uint8_t* b) {
	uint8_t temp = *a;
	*a = *b;
	*b = temp;
}

// Helper function to swap float array values
void SwapFloatArray(float* arr, size_t count)
{
	for (size_t i = 0; i < count; i++) 
	{
		uint32_t* p = reinterpret_cast<uint32_t*>(&arr[i]);
		LITTLE_BIG_SWAP(*p);
	}
}

// for image math checks
bool isPowerOfTwo(int n) 
{
	if (n <= 0) 
	{
		return false;
	}
	return (n & (n - 1)) == 0;
}

// for string indexes
int clamp_index(int value, int array_size) 
{
	if (value < 0) return 0;
	if (value >= array_size) return array_size - 1;
	return value;
}

int Align(uint32_t num, uint32_t n) {
	uint32_t value = num % n;

	if (value)
	{
		return num + (n - value);
	}
	else
	{
		return num;
	}
}

// Helper function for data size
uint32_t TextureSizeWithMips(
	uint32_t width, 
	uint32_t height, 
	uint32_t depth,
	uint32_t mipCount,
	uint32_t arrayCount, 
	uint32_t faceCount,
	uint32_t blockByteSize, 
	uint32_t blockWidth, 
	uint32_t blockHeight,
	uint32_t bCompressedFormat, 
	double bitsPerPixel,
	uint32_t pAlignment
)
{
	uint32_t totalSize = 0;
	uint32_t w = width;
	uint32_t h = height;
	uint32_t d = depth; if (d == 0) d = 1;
	uint32_t mip = 0;
	uint32_t blocksX = 0;
	uint32_t blocksY = 0;
	uint32_t mipSize = 0;

	for (mip = 0; mip < mipCount; ++mip)
	{
		mipSize = 0;
		w = width >> mip;  if (w == 0) w = 1;
		h = height >> mip; if (h == 0) h = 1;
		d = depth >> mip;  if (d == 0) d = 1;

		if (bCompressedFormat)
		{
			blocksX = (w + blockWidth - 1) / blockWidth;   if (blocksX == 0) blocksX = 1;
			blocksY = (h + blockHeight - 1) / blockHeight; if (blocksY == 0) blocksY = 1;

			mipSize = blocksX * blocksY * blockByteSize;

			// Sanity check for smallest block size
			if (mipSize < blockByteSize)
			{
				mipSize = blockByteSize;
			}

			if (pAlignment)
			{
				mipSize = Align(mipSize, pAlignment);
			}

			mipSize *= d;

			totalSize += mipSize;
		}
		else
		{
			mipSize = (uint32_t)((w * h) * (bitsPerPixel / 8));

			// Sanity check for smallest pixel size
			if (mipSize < blockByteSize)
			{
				mipSize = blockByteSize;
			}

			if (pAlignment)
			{
				mipSize = Align(mipSize, pAlignment);
			}

			mipSize *= d;

			totalSize += mipSize;
		}
	}

	return totalSize * arrayCount * faceCount;
}

// Nintendo Switch Size Code
static uint32_t alignUp(uint32_t value, uint32_t alignment) {
	return (value + alignment - 1) & ~(alignment - 1);
}

// Nintendo Switch Size Code
static uint32_t block_height_enum(uint32_t blocksY) {
	uint32_t h = blocksY + (blocksY / 2);
	if (h >= 128) return 16;
	if (h >= 64) return 8;
	if (h >= 32) return 4;
	if (h >= 16) return 2;
	return 1;
}

// Nintendo Switch Size Code
static uint32_t mip_block_height(uint32_t mipHeight, uint32_t blockHeightEnum) {
	while (mipHeight <= (blockHeightEnum / 2) * 8 && blockHeightEnum > 1) {
		blockHeightEnum /= 2;
	}
	return blockHeightEnum;
}

// Nintendo Switch Size Code
static size_t swizzled_mip_size(uint32_t blocksX, uint32_t blocksY, uint32_t blocksZ,
	uint32_t blockHeightEnum, uint32_t blockSizeBytes) {
	uint32_t width_in_gobs = alignUp(blocksX * blockSizeBytes, 64) / 64;
	uint32_t height_in_gobs = (blocksY + (blockHeightEnum * 8 - 1)) / (blockHeightEnum * 8);
	return (size_t)width_in_gobs * height_in_gobs * blocksZ * blockHeightEnum * 512; // GOB_SIZE
}

// Nintendo Switch Size Code
static size_t align_layer_size(size_t layer_size, uint32_t height, uint32_t depth, uint32_t block_height_enum) {
	uint32_t gob_height = block_height_enum;
	uint32_t gob_depth = 1;

	while (height <= (gob_height / 2) * 8 && gob_height > 1) 
	{
		gob_height /= 2;
	}
	while (depth <= (gob_depth / 2) && gob_depth > 1) 
	{
		gob_depth /= 2;
	}

	uint32_t block_of_gobs_size = gob_height * gob_depth * 512; // GOB_SIZE
	size_t size_in_blocks = layer_size / block_of_gobs_size;

	if (layer_size != size_in_blocks * block_of_gobs_size) 
	{
		layer_size = (size_in_blocks + 1) * block_of_gobs_size;
	}

	return layer_size;
}

typedef struct {
	size_t totalSize;
	size_t layerSize;
	size_t offset; // offset of target layer/slice/depth
} TextureSizeResult;

TextureSizeResult SwitchTextureSizeWithMips(
	uint32_t width, 
	uint32_t height, 
	uint32_t depth,
	uint32_t mipCount, 
	uint32_t layers,
	uint32_t blockWidth, 
	uint32_t blockHeight,
	uint32_t blockSizeBytes, 
	uint32_t bitsPerPixel,
	int isCompressed, 
	S_GT1_LOAD_TYPE textureType,
	uint32_t targetLayer, // use this as target index for offset
	uint32_t targetDepth  // target depth slice
)
{
	size_t totalSize = 0;
	size_t layerSize = 0;
	// Adjust offset for targetLayer and targetDepth
	size_t layerOffset = 0;

	uint32_t blocksY_full = (height + blockHeight - 1) / blockHeight;
	uint32_t blockHeightEnum = block_height_enum(blocksY_full);

	for (uint32_t mip = 0; mip < mipCount; ++mip) 
	{
		size_t mipSize = 0;

		if (isCompressed) 
		{

			uint32_t mipWidth = width >> mip;
			uint32_t mipHeight = height >> mip;
			uint32_t mipDepth = depth >> mip;

			if (mipWidth == 0) mipWidth = 1;
			if (mipHeight == 0) mipHeight = 1;
			if (mipDepth == 0) mipDepth = 1;

			uint32_t blocksX = (mipWidth + blockWidth - 1) / blockWidth;
			uint32_t blocksY = (mipHeight + blockHeight - 1) / blockHeight;
			uint32_t blocksZ = (mipDepth + 1 - 1); // Assume blockDepth = 1

			uint32_t mipBlockHeight = mip_block_height(blocksY, blockHeightEnum);
			mipSize = swizzled_mip_size(blocksX, blocksY, blocksZ, mipBlockHeight, blockSizeBytes);
		}
		else 
		{
			if (mip == 0 && height < 8) height = 8;

			uint32_t mipWidth = width >> mip;
			uint32_t mipHeight = height >> mip; 
			uint32_t mipDepth = depth >> mip;

			if (mipWidth == 0) mipWidth = 1;
			if (mipHeight == 0) mipHeight = 1;
			if (mipDepth == 0) mipDepth = 1;

			mipSize = ((size_t)mipWidth * mipHeight * mipDepth * bitsPerPixel) / 8;
		}
		layerSize += mipSize;
	}

	if ( textureType == S_GT1_LOAD_TYPE::PLANE_ARRAY || 
		 textureType == S_GT1_LOAD_TYPE::CUBE ||
		 textureType == S_GT1_LOAD_TYPE::CUBE_ARRAY
		) 
	{
		layerSize = align_layer_size(layerSize, height, depth, blockHeightEnum);
	}

	uint32_t totalLayers = layers;
	if ( textureType == S_GT1_LOAD_TYPE::CUBE || 
		 textureType == S_GT1_LOAD_TYPE::CUBE_ARRAY
		) 
	{
		totalLayers *= 6;
		if (totalLayers < 6)
		{
			totalLayers = 6;
		}
	}

	if (targetLayer < totalLayers && targetDepth >= 0)
	{
		layerOffset = layerSize * targetLayer + (targetDepth * layerSize / depth);
	}
	
	TextureSizeResult result;
	result.totalSize = layerSize * totalLayers;
	result.layerSize = layerSize;
	result.offset = layerOffset;
	return result;
}

///////////////////////////////
//      HEADER STRUCTS       //
///////////////////////////////

template <bool bBigEndian>
struct S_G1T_HEADER
{
	char MAGIC[4] = {};
	uint32_t VERSION = 0;
	uint32_t VERSION_NUMBER = 0;
	uint32_t FILE_SIZE = 0;
	uint32_t TEX_OFFSET = 0;
	uint32_t TEX_COUNT = 0;
	// VERSION_NUMBER > 30 always 0 for SYSTEM
	PLATFORM SYSTEM = PLATFORM::DX9;
	// VERSION_NUMBER > 50 no EX Headers
	uint32_t HEADER_EX_SIZE = 0;
	uint32_t HEADER_SIZE = 20;
	bool bPanic = false;

	S_G1T_HEADER(BYTE* buffer, uint32_t& offset, noeRAPI_t* rapi, int bufferLen)
	{
		// panic check
		if (HEADER_SIZE > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		MAGIC[0] = (*(char*)(buffer + offset)); offset += 1;
		MAGIC[1] = (*(char*)(buffer + offset)); offset += 1;
		MAGIC[2] = (*(char*)(buffer + offset)); offset += 1;
		MAGIC[3] = (*(char*)(buffer + offset)); offset += 1;

		const char EXPECTED_BIG_MAGIC[4] = { 'G', '1', 'T', 'G' };
		const char EXPECTED_LITTLE_MAGIC[4] = { 'G', 'T', '1', 'G' };

		// panic check
		if ( !(memcmp(MAGIC, EXPECTED_BIG_MAGIC, 4) == 0 ||
			 memcmp(MAGIC, EXPECTED_LITTLE_MAGIC, 4) == 0)
			)
		{
			bPanic = true;
			return;
		}

		VERSION = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(VERSION);

		// get the version number
		VERSION_NUMBER = 
			(((VERSION >> 24 ) & 0xFF) - 0x30) * 1000 +
			(((VERSION >> 16 ) & 0xFF) - 0x30) * 100 +
			(((VERSION >> 8  ) & 0xFF) - 0x30) * 10 +
			  (VERSION & 0xFF) - 0x30;

		FILE_SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(FILE_SIZE);

		// panic check
		if (FILE_SIZE > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		TEX_OFFSET = *(uint32_t*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(TEX_OFFSET);

		// panic check
		if (TEX_OFFSET > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		TEX_COUNT = *(uint32_t*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(TEX_COUNT);

		if (VERSION_NUMBER < 30)
		{
			SYSTEM = PLATFORM::DX9;
			HEADER_EX_SIZE = 0;
			HEADER_SIZE = 20;
		}
		else if (VERSION_NUMBER < 50)
		{
			HEADER_SIZE = 24;
			// panic check
			if (HEADER_SIZE > (uint32_t)bufferLen)
			{
				bPanic = true;
				return;
			}
			SYSTEM = *(PLATFORM*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(SYSTEM);
			HEADER_EX_SIZE = 0;
		}
		else
		{
			HEADER_SIZE = 28;
			// panic check
			if (HEADER_SIZE > (uint32_t)bufferLen)
			{
				bPanic = true;
				return;
			}
			SYSTEM = *(PLATFORM*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(SYSTEM);
			HEADER_EX_SIZE = *(uint32_t*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(HEADER_EX_SIZE);
		}

		if (bDebugLog)
		{
			// File name for texture renaming
			std::string inFile = rapi->Noesis_GetLastCheckedName();
			std::filesystem::path pathObj(inFile);
			std::string fileName = pathObj.stem().string();
			LogDebug("G1T_FILE_NAME:\t\t\t%s\n", fileName.c_str());
			LogDebug("G1T_HEADER:\n");
			LogDebug("\tMAGIC:\t\t\t%c%c%c%c (%s)\n", MAGIC[0], MAGIC[1], MAGIC[2], MAGIC[3], bBigEndian ? "Big Endian" : "Little Endian");
			LogDebug("\tVERSION:\t\t%d\n", VERSION_NUMBER);
			LogDebug("\tFILE_SIZE:\t\t%d\n", FILE_SIZE);
			LogDebug("\tTEX_OFFSET:\t\t%d\n", TEX_OFFSET);
			LogDebug("\tTEX_COUNT:\t\t%d\n", TEX_COUNT);
			LogDebug("\tSYSTEM:\t\t\t%d (%s)\n", SYSTEM, PLATFORM_STR[clamp_index((int)SYSTEM, sizeof(PLATFORM_STR))]);
			LogDebug("\tHEADER_SIZE:\t\t%d\n", HEADER_SIZE);
			LogDebug("\tHEADER_EX_SIZE:\t\t%d\n", HEADER_EX_SIZE);
		}
	}
};

struct S_G1T_TEX_META
{
	bool bSwizzled = false;
	uint8_t EXCount = 0;
	bool bNormalMap = false;
	bool bNormalWithAlpha = false;
	bool bBigEndianShortSwap = false;
	bool bBigEndianLongSwap = false;
	bool bChannelFlip = false;
	// for formats that are not unsigned
	bool bSigned = false;
	// for blocks format 
	bool bCompressedFormat = false;
	bool bNormalized = true;
	bool bIsETC1 = false;
	bool bPICAETC = false;
	bool bIsETC2 = false;
	// For images that have a hidden alpha channel texture below (double height)
	// Info here: https://arm-software.github.io/opengl-es-sdk-for-android/compressed_alpha_channels.html
	bool bHasAlphaAtlas = false;
	// for 0x71 for some reason
	bool bSkipAlphaAtlas = false;
	// 3DS ETC Alpha
	bool b3DSAlpha = false;
	bool bIsPVRTC = false;
	// TODO: 32 byte palette
	bool bPalette4 = false;
	// TODO: 512 byte palette
	bool bPalette8 = false;
	// TODO: 32,768 byte palette
	bool bPalette14 = false;
	bool bFloat = false;
	bool bHalfFloat = false;
	// For a2b#F10g#F10r#F10 to r#F32g#F32b#F32a#F32
	bool bConvert10BitFloat = false;
	// For r#F11g#F11b#F10 to r#F32g#F32b#F32
	bool bConvert11Bit10BitFloat = false;
	// for textures that are depth and stencil
	bool bIsDepth = false;
	// unsigned normalized
	bool bIsUNorm = false;
	// for ASTC sub format
	bool bIsASTC = false;
	// for d24s8 textures to r24b24g24a8
	bool bD24_8Convert = false;
	// for d16 to r16b16g16
	bool bD16Convert = false;
	// for d32 to r#F32B#F32g#F32
	bool bD32Convert = false;
	// for d#F32p24s8 to r#F32b#F32g#F32a#F32
	bool bD32FloatConvert = false;
	// for depth and alpha
	bool bD8A8Convert = false;
	// for image data compressed with zlib
	bool bZlibCompressed = false;
	// for some systems where they don't swizzle these
	bool bIsPower2 = false;
	bool bUseSwitchSize = false;
	bool bUsePS5Size = false;
	uint32_t pAlignment = 0;
	EG1TASTCFormat KT_ASTC_FORMAT = EG1TASTCFormat::NONE;
	uint32_t WiiU_SWIZZLE = 0; // for extra data
	uint32_t Wii_SWIZZLE = 0; // for extra data
	uint32_t ARRAY_ID1 = 0;
	uint32_t ARRAY_DEPTH = 1; // for extra data 
	uint32_t xBOX360_w = 4;
	uint32_t xBOX360_h = 8;
	std::vector<uint32_t> EXTRA_INTS; // for extra data
};

// Likely deprecated
template <bool bBigEndian>
struct S_G1T_TEX_ATTR_HEADER
{
	TEX_EX_TYPE TYPE = TEX_EX_TYPE::ENVMAP;
	uint8_t COUNT = 0;
	uint8_t CONSUMED = 0;
	uint32_t headerSize = 4;
	bool bPanic = false;

	S_G1T_TEX_ATTR_HEADER(BYTE* buffer, uint32_t& offset, int bufferLen)
	{
		// panic check
		if ((offset + headerSize) > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		// it's just the type but the value is a int not a short so I did this to not have to make another type
		if (bBigEndian)
		{
			/*CONSUMED = *(uint8_t*)(buffer + offset);*/ offset += 1;
			COUNT = *(uint8_t*)(buffer + offset); offset += 1;
			TYPE = *(TEX_EX_TYPE*)(buffer + offset); offset += 2; if (bBigEndian) LITTLE_BIG_SWAP(TYPE);
		}
		else
		{
			TYPE = *(TEX_EX_TYPE*)(buffer + offset); offset += 2; if (bBigEndian) LITTLE_BIG_SWAP(TYPE);
			COUNT = *(uint8_t*)(buffer + offset); offset += 1;
			/*CONSUMED = *(uint8_t*)(buffer + offset);*/ offset += 1;
		}
	}
};

template <bool bBigEndian>
struct S_G1T_HEADER_EX
{
	uint32_t ID;
	TEX_EX_TYPE TYPE = TEX_EX_TYPE::ENVMAP;
	uint8_t COUNT = 0;
	uint8_t CONSUMED = 0;
	uint32_t headerSize = 4;
	EG1TASTCFormat KT_ASTC_FORMAT = EG1TASTCFormat::NONE; // -1
	uint32_t WiiU_SWIZZLE = 0;
	uint32_t Wii_SWIZZLE = 0;
	uint32_t xBOX360_w = 4;
	uint32_t xBOX360_h = 8;
	bool bPanic = false;

	// PS4 array data
	uint32_t ID1 = 0;
	float FLOAT1[4] = {};
	uint32_t ID2 = 0;
	float FLOAT2[4] = {};
	uint32_t ID3 = 0;
	float FLOAT3[5] = {};
	uint32_t ID4 = 0;
	float FLOAT4[4] = {};
	uint32_t ID5 = 0;
	float FLOAT5[4] = {};
	uint32_t ID6 = 0;
	float FLOAT6[5] = {};
	uint32_t ARRAY_DEPTH = 1;

	std::vector<uint32_t> EXTRA_INTS; // Sometimes used as KIDs

	S_G1T_HEADER_EX(BYTE* buffer, uint32_t& offset, int bufferLen, uint32_t i)
	{
		ID = i+1;
		// panic check
		if ((offset + headerSize) > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		TYPE = *(TEX_EX_TYPE*)(buffer + offset); offset += 2; if (bBigEndian) LITTLE_BIG_SWAP(TYPE);
		COUNT = *(uint8_t*)(buffer + offset); offset += 1; 
		/*CONSUMED = *(uint8_t*)(buffer + offset);*/ offset += 1;

		// panic check
		if (((COUNT * 4) + offset) > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		// Read 'COUNT' number of int values to parse later
		if (COUNT != 0)
		{
			EXTRA_INTS.resize(COUNT);
			memcpy(EXTRA_INTS.data(), buffer + offset, 4 * COUNT);
			for (auto& offs : EXTRA_INTS)
			{
				if (bBigEndian)
				{
					LITTLE_BIG_SWAP(offs);
				}
			}
		}

		if (TYPE == TEX_EX_TYPE::NORMALMAP && COUNT == 2)
		{
			xBOX360_w = EXTRA_INTS[0];
			xBOX360_h = EXTRA_INTS[1];
		}
		else if (TYPE == TEX_EX_TYPE::TLUT && COUNT == 1)
		{
			Wii_SWIZZLE = EXTRA_INTS[0]; 
		}
		else if (TYPE == TEX_EX_TYPE::CAFE_SWIZZLE && COUNT == 1)
		{
			WiiU_SWIZZLE = EXTRA_INTS[0];
		}
		else if (TYPE == TEX_EX_TYPE::PS4_PLANE_ARRAY && COUNT == 32)
		{
			// unsure what this is, Vector bases?
			int vOffset = offset;
			ID1 = *(uint32_t*)(buffer + vOffset); vOffset += 4;  if (bBigEndian) LITTLE_BIG_SWAP(ID1);
			memcpy(FLOAT1, buffer + vOffset, 16); vOffset += 16; if (bBigEndian) SwapFloatArray(FLOAT1, 4);
			ID2 = *(uint32_t*)(buffer + vOffset); vOffset += 4;  if (bBigEndian) LITTLE_BIG_SWAP(ID2);
			memcpy(FLOAT2, buffer + vOffset, 16); vOffset += 16; if (bBigEndian) SwapFloatArray(FLOAT2, 4);
			ID3 = *(uint32_t*)(buffer + vOffset); vOffset += 4;  if (bBigEndian) LITTLE_BIG_SWAP(ID3);
			memcpy(FLOAT3, buffer + vOffset, 20); vOffset += 20; if (bBigEndian) SwapFloatArray(FLOAT3, 5);
			ID4 = *(uint32_t*)(buffer + vOffset); vOffset += 4;  if (bBigEndian) LITTLE_BIG_SWAP(ID4);
			memcpy(FLOAT4, buffer + vOffset, 16); vOffset += 16; if (bBigEndian) SwapFloatArray(FLOAT4, 4);
			ID5 = *(uint32_t*)(buffer + vOffset); vOffset += 4;  if (bBigEndian) LITTLE_BIG_SWAP(ID5);
			memcpy(FLOAT5, buffer + vOffset, 16); vOffset += 16; if (bBigEndian) SwapFloatArray(FLOAT5, 4);
			ID6 = *(uint32_t*)(buffer + vOffset); vOffset += 4;  if (bBigEndian) LITTLE_BIG_SWAP(ID6);
			memcpy(FLOAT6, buffer + vOffset, 20); vOffset += 20; if (bBigEndian) SwapFloatArray(FLOAT6, 5);
			// I have no idea what this is
			if (EXTRA_INTS[0] == 922948206)
			{
				ARRAY_DEPTH = 2;
			}
		}
		else if (TYPE == TEX_EX_TYPE::ASTC_TYPE && COUNT == 1)
		{
			KT_ASTC_FORMAT = (EG1TASTCFormat)EXTRA_INTS[0];
		}

		offset += (COUNT * 4);
		headerSize += (COUNT * 4);
	}
};

enum class KTGL_PIXEL_FORMAT : uint8_t
{
	RGBA8             = 0x00,
	BGRA8             = 0x01,
	R32F              = 0x02,
	RGBA16F           = 0x03,
	RGBA32F           = 0x04,
	Z24S8             = 0x05,
	DXT1              = 0x06,
	DXT3              = 0x07,
	DXT5              = 0x08,
	TILE_RGBA8        = 0x09,
	TILE_BGRA8        = 0x0A,
	TILE_R32F         = 0x0B,
	TILE_RGBA16F      = 0x0C,
	TILE_RGBA32F      = 0x0D,
	RGB10FA2F         = 0x0E,
	A8                = 0x0F,
	TILE_DXT1         = 0x10,
	TILE_DXT3         = 0x11,
	TILE_DXT5         = 0x12,
	TILE_Z24S8        = 0x13,
	Z16               = 0x14,
	TILE_Z16          = 0x15,
	R16               = 0x16,
	TILE_R16          = 0x17,
	TILE_A8           = 0x18,
	B5G6R5            = 0x19,
	B5G5R5A1          = 0x1A,
	B4G4R4A4          = 0x1B,
	TILE_B5G6R5       = 0x1C,
	TILE_B5G5R5A1     = 0x1D,
	TILE_B4G4R4A4     = 0x1E,
	Z24FS8            = 0x1F,
	TILE_Z24FS8       = 0x20,
	BGRX8             = 0x21,
	TILE_BGRX8        = 0x22,
	GR16              = 0x23,
	TILE_GR16         = 0x24,
	TILE_B5G5R5A3     = 0x25,
	TILE_BG8          = 0x26,
	TILE_RA8          = 0x27,
	TILE_B8           = 0x28,
	TILE_G8           = 0x29,
	TILE_R8           = 0x2A,
	TILE_L4           = 0x2B,
	TILE_L8           = 0x2C,
	TILE_LA4          = 0x2D,
	TILE_LA8          = 0x2E,
	TILE_Z4           = 0x2F,
	TILE_Z8           = 0x30,
	TILE_INDEX4       = 0x31,
	TILE_INDEX8       = 0x32,
	TILE_INDEX14X2    = 0x33,
	R5G6B5            = 0x34,
	R5G5B5A1          = 0x35,
	R4G4B4A4          = 0x36,
	INDEX4            = 0x37,
	INDEX8            = 0x38,
	INDEX16           = 0x39,
	INDEX32           = 0x3A,
	TILE_R5G6B5       = 0x3B,
	TILE_R5G5B5A1     = 0x3C,
	TILE_R4G4B4A4     = 0x3D,
	TILE_INDEX16      = 0x3E,
	TILE_INDEX32      = 0x3F,
	R10G10B10A2       = 0x40,
	R16G16B16A16      = 0x41,
	TILE_R10G10B10A2  = 0x42,
	TILE_R16G16B16A16 = 0x43,
	TILE_A4           = 0x44,
	TILE_RGB8         = 0x45,
	TILE_RG8          = 0x46,
	TILE_ETC1         = 0x47,
	TILE_ETC1A4       = 0x48,
	RGB8              = 0x49,
	Z24               = 0x4A,
	TILE_Z24          = 0x4B,
	GR32F             = 0x4C,
	TILE_GR32F        = 0x4D,
	Z32F              = 0x4E,
	TILE_Z32F         = 0x4F,
	PVRTII2           = 0x50,
	TILE_PVRTII2      = 0x51,
	PVRTII4           = 0x52,
	TILE_PVRTII4      = 0x53,
	L8                = 0x54,
	LA8               = 0x55,
	ETC1              = 0x56,
	PVRT2             = 0x57,
	PVRT4             = 0x58,
	BC1               = 0x59,
	BC2               = 0x5A,
	BC3               = 0x5B,
	BC4               = 0x5C,
	BC5               = 0x5D,
	BC6               = 0x5E,
	BC7               = 0x5F,
	TILE_BC1          = 0x60,
	TILE_BC2          = 0x61,
	TILE_BC3          = 0x62,
	TILE_BC4          = 0x63,
	TILE_BC5          = 0x64,
	TILE_BC6          = 0x65,
	TILE_BC7          = 0x66,
	RGBA8U            = 0x67,
	RG8U              = 0x68,
	RG16F             = 0x69,
	R16F              = 0x6A,
	R11G11B10F        = 0x6B,
	Z32FS8            = 0x6C,
	TILE_Z32FS8       = 0x6D,
	Z16S8             = 0x6E,
	ETC1RGBETC1A      = 0x6F,
	PVRT4RGBPVRT4A    = 0x70,
	ETC2RGBA8         = 0x71,
	R8                = 0x72,
	RG8               = 0x73,
	TILE_RGBA8U       = 0x74,
	TILE_RG8U         = 0x75,
	TILE_RG16F        = 0x76,
	TILE_R16F         = 0x77,
	TILE_R11G11B10F   = 0x78,
	TILE_Z16S8        = 0x79,
	EACR11            = 0x7A,
	EACRG11           = 0x7B,
	ETC2RGB8A1        = 0x7C,
	ASTC              = 0x7D,
	ASTC_TILE         = 0x7E,
	FORCE_INT32       = 0x7FFFFFFF,
};

template <bool bBigEndian>
struct S_G1T_TEX_HEADER
{
	uint8_t mipSys = 0;
	S_GT1_LOAD_TYPE KTGL_TEXTURE_TYPE = S_GT1_LOAD_TYPE::PLANAR;
	uint8_t MIP_COUNT = 0;

	KTGL_PIXEL_FORMAT PIXEL_FORMAT = KTGL_PIXEL_FORMAT::FORCE_INT32;

	uint8_t dxdy = 0;
	uint8_t PACKED_WIDTH = 0;
	uint8_t PACKED_HEIGHT = 0;
	uint32_t HEIGHT = 0, WIDTH = 0, DEPTH = 0;

	uint8_t depthEx = 0;
	uint8_t PACKED_DEPTH = 0;
	uint8_t READ_G1T_HEADER_EX = 0;
	
	uint8_t address1_2 = 0;
	uint8_t KTGL_GD_TEXADDRESSU = 0;
	uint8_t KTGL_GD_TEXADDRESSV = 0;

	uint8_t address3_texFilter1 = 0;
	uint8_t KTGL_GD_TEXADDRESSW = 0;
	uint8_t KTGL_GD_TEXMAGFILTER = 0;

	uint8_t texFilter2_mipFilter1 = 0;
	uint8_t KTGL_GD_TEXMINFILTER = 0;
	uint8_t KTGL_GD_MIPFILTER = 0;

	uint8_t mipFilter2_hasEx = 0;
	uint8_t KTGL_GD_ANISO_SAMPLE_NUM = 0;
	uint8_t HAS_TEX_EX_HEADER = 0;

	uint32_t headerSize = 0x8;

	// S_G1T_TEX_HEADER_EX values

	uint32_t TEX_HEADER_EX_SIZE = 0;

	float Z_SCALE = 0;

	uint16_t exFacesArrays = 0;
	uint16_t EX_FACES = 0;
	uint16_t EX_ARRAY_COUNT = 0;

	EX_SWIZZLE_TYPE EX_SWIZZLE = EX_SWIZZLE_TYPE::NONE;
	KTGL_GD_COLOR_SPACE_TYPE KTGL_GD_COLOR_SPACE = KTGL_GD_COLOR_SPACE_TYPE::NONE; // mostly used as srgb from linear

	uint32_t EX_WIDTH = 0;
	uint32_t EX_HEIGHT = 0;

	bool bIsPower2 = true;
	bool bPanic = false;

	S_G1T_TEX_HEADER(BYTE* buffer, uint32_t& offset, int bufferLen, int i)
	{
		// panic check
		if ((offset + headerSize) > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		//Read basic info
		mipSys                = *(uint8_t*)(buffer + offset); offset += 1;
		//split mip_count and subsys
		if (bBigEndian) 
		{
			KTGL_TEXTURE_TYPE = (S_GT1_LOAD_TYPE)((mipSys >> 4) & 0xF);
			MIP_COUNT = ((mipSys >> 0) & 0xF);
		}
		else
		{
			KTGL_TEXTURE_TYPE = (S_GT1_LOAD_TYPE)((mipSys >> 0) & 0xF);
			MIP_COUNT = ((mipSys >> 4) & 0xF);
		}

		PIXEL_FORMAT = *(KTGL_PIXEL_FORMAT*)(buffer + offset); offset += 1;

		// panic check
		if (PIXEL_FORMAT > KTGL_PIXEL_FORMAT::FORCE_INT32)
		{
			bPanic = true;
		}

		dxdy                  = *(uint8_t*)(buffer + offset); offset += 1;
		//split width and height
		PACKED_WIDTH =  ((dxdy >> 0) & 0xF);
		PACKED_HEIGHT = ((dxdy >> 4) & 0xF);
		//compute width and height
		WIDTH = (uint32_t)pow(2, PACKED_WIDTH);
		HEIGHT = (uint32_t)pow(2, PACKED_HEIGHT);


		if (bBigEndian) {
			uint32_t temp = WIDTH;
			WIDTH = HEIGHT;
			HEIGHT = temp;
			swapNibbles(&PACKED_WIDTH, &PACKED_HEIGHT);
		}
		
		depthEx              = *(uint8_t*)(buffer + offset); offset += 1;
		//split depth and read ex header
		PACKED_DEPTH         = ((depthEx >> 0) & 0xF);
		READ_G1T_HEADER_EX   = ((depthEx >> 4) & 0xF);
		if (bBigEndian) swapNibbles(&PACKED_DEPTH, &READ_G1T_HEADER_EX);

		DEPTH = (uint32_t)pow(2, PACKED_DEPTH);

		address1_2            = *(uint8_t*)(buffer + offset); offset += 1;
		//split texture address
		KTGL_GD_TEXADDRESSU   = ((address1_2 >> 0) & 0xF);
		KTGL_GD_TEXADDRESSV   = ((address1_2 >> 4) & 0xF);
		if (bBigEndian) swapNibbles(&KTGL_GD_TEXADDRESSU, &KTGL_GD_TEXADDRESSV);

		address3_texFilter1   = *(uint8_t*)(buffer + offset); offset += 1;
		//split texture address and texture filter
		KTGL_GD_TEXADDRESSW   = ((address3_texFilter1 >> 0) & 0xF);
		KTGL_GD_TEXMAGFILTER  = ((address3_texFilter1 >> 4) & 0xF);
		if (bBigEndian) swapNibbles(&KTGL_GD_TEXADDRESSW, &KTGL_GD_TEXMAGFILTER);

		texFilter2_mipFilter1 = *(uint8_t*)(buffer + offset); offset += 1;
		//split texture filter and mip filter
		KTGL_GD_TEXMINFILTER  = ((texFilter2_mipFilter1 >> 0) & 0xF);
		KTGL_GD_MIPFILTER    = ((texFilter2_mipFilter1 >> 4) & 0xF);
		if (bBigEndian) swapNibbles(&KTGL_GD_TEXMINFILTER, &KTGL_GD_MIPFILTER);

		mipFilter2_hasEx      = *(uint8_t*)(buffer + offset); offset += 1;
		//split mip filter and read tex ex header
		KTGL_GD_ANISO_SAMPLE_NUM = ((mipFilter2_hasEx >> 0) & 0xF);
		// Engine name: attrNum, on ktgl::S_G1T_TEX_HEADER. It is a COUNT, not a
		// flag. ktgl::graphics::oes2::consume walks exactly attrNum attribute
		// records, each 4 + 4 * dataWC bytes, and stamps the byte at +3 of every
		// record: 0 on all but the last, 1 on the last. So that byte means "last
		// in the chain", not "already consumed", and the engine writes it during
		// load rather than reading it from the file. Treating attrNum as a
		// boolean is fine while every sample has at most one record, but a file
		// with two would need the chain walked.
		HAS_TEX_EX_HEADER     = ((mipFilter2_hasEx >> 4) & 0xF);
		if (bBigEndian) swapNibbles(&KTGL_GD_ANISO_SAMPLE_NUM, &HAS_TEX_EX_HEADER);

		//Extra header
		if (HAS_TEX_EX_HEADER > 0)
		{
			TEX_HEADER_EX_SIZE = *(uint32_t*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(TEX_HEADER_EX_SIZE);
			headerSize += TEX_HEADER_EX_SIZE;

			// panic check
			if ((offset + TEX_HEADER_EX_SIZE) > uint32_t(bufferLen))
			{
				bPanic = true;
				return;
			}

			Z_SCALE      = *(float*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(Z_SCALE);

			exFacesArrays    = *(uint16_t*)(buffer + offset); offset += 2; if (bBigEndian) LITTLE_BIG_SWAP(exFacesArrays);
			//split faces and array count
			EX_FACES		= ((exFacesArrays >> 0) & 0xF);
			EX_ARRAY_COUNT  = ((exFacesArrays >> 4) & 0xFFF);

			EX_SWIZZLE          = *(EX_SWIZZLE_TYPE*)(buffer + offset); offset += 1;
			KTGL_GD_COLOR_SPACE = *(KTGL_GD_COLOR_SPACE_TYPE*)(buffer + offset); offset += 1;
			
			if (TEX_HEADER_EX_SIZE >= 0x10)
			{
				EX_WIDTH = *(uint32_t*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(EX_WIDTH);
				if (EX_WIDTH != 0) WIDTH = EX_WIDTH;
			}
			if (TEX_HEADER_EX_SIZE >= 0x14)
			{
				EX_HEIGHT = *(uint32_t*)(buffer + offset); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(EX_HEIGHT);
				if (EX_HEIGHT != 0) HEIGHT = EX_HEIGHT;
			}

			if (!isPowerOfTwo(HEIGHT) || !isPowerOfTwo(WIDTH))
			{
				bIsPower2 = false;
			}

			if ( TEX_HEADER_EX_SIZE != 0xC && 
				 TEX_HEADER_EX_SIZE != 0x14)
			{
				PopUpMessage(L"TEX_HEADER_EX size error on #%d\nTEX_HEADER_EX_SIZE: %d", i, TEX_HEADER_EX_SIZE);
			}
		}
	}
};

template <bool bBigEndian>
struct S_G1T_COMPED_HEADER
{
	uint32_t VERSION = 0;
	uint32_t VERSION_NUMBER = 0;
	// 36 + 16*mips*layers + 8*(PAGE_COUNT+1) on every sample
	uint32_t COMPED_BLOCK_SIZE = 0;
	// 128 on all 29 samples. UNDETERMINED
	uint32_t COMPED_UKN1 = 0;
	// 0x10000 on all 29. Uncompressed size of one streamed page
	uint32_t COMPED_WINDOW_SIZE = 0;
	// Mip levels held as compressed pages rather than in the file
	uint32_t STREAMED_LEVELS = 0;
	// Total streamed pages; equals the sum of TILES_X*TILES_Y below
	uint32_t PAGE_COUNT = 0;
	// was COMPED_META2. VERIFIED == mipCount - STREAMED_LEVELS on all 29 samples
	uint32_t RESIDENT_LEVELS = 0;
	// 1 or 2 across the corpus; only ever tested against zero, and there is never more than one uncompressed chunk
	uint32_t COMPED_HAS_UNCOMPED_CHUNK = 0;
	// Size of the resident tail. VERIFIED == the sum of every RESIDENT_LEVEL.BYTE_SIZE
	uint32_t COMPED_UNCOMPED_CHUNK_SIZE = 0;
	uint32_t HEADER_SIZE = 36;
	bool bPanic = false;

	S_G1T_COMPED_HEADER(BYTE* buffer, uint32_t& offset, noeRAPI_t* rapi, int bufferLen)
	{
		// panic check
		if (offset + HEADER_SIZE > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}

		VERSION = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(VERSION);

		// get the version number
		VERSION_NUMBER =
			(((VERSION >> 24)  & 0xFF) - 0x30) * 1000 +
			(((VERSION >> 16)  & 0xFF) - 0x30) * 100 +
			(((VERSION >> 8 )  & 0xFF) - 0x30) * 10 +
			  (VERSION & 0xFF) - 0x30;

		COMPED_BLOCK_SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(COMPED_BLOCK_SIZE);
		COMPED_UKN1 = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(COMPED_UKN1);
		COMPED_WINDOW_SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(COMPED_WINDOW_SIZE);
		STREAMED_LEVELS = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(STREAMED_LEVELS);
		PAGE_COUNT = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(PAGE_COUNT);
		RESIDENT_LEVELS = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(RESIDENT_LEVELS);
		COMPED_HAS_UNCOMPED_CHUNK = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(COMPED_HAS_UNCOMPED_CHUNK);
		COMPED_UNCOMPED_CHUNK_SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(COMPED_UNCOMPED_CHUNK_SIZE);
	}
};

template <bool bBigEndian>
struct S_G1T_COMPED_LEVELS
{
	uint32_t TILES_X = 0;
	uint32_t TILES_Y = 0;
	uint32_t PAGE_ZERO = 0;
	uint32_t FIRST_PAGE = 0;
	uint32_t HEADER_SIZE = 16;
	bool bPanic = false;
	S_G1T_COMPED_LEVELS(BYTE* buffer, uint32_t& offset, int bufferLen)
	{
		// panic check
		if ((offset + HEADER_SIZE) > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}
		TILES_X = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(TILES_X);
		TILES_Y = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(TILES_Y);
		PAGE_ZERO = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(PAGE_ZERO);
		FIRST_PAGE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(FIRST_PAGE);
	}
};

template <bool bBigEndian>
struct S_G1T_COMPED_RESIDENT
{
	uint32_t ROW_PITCH = 0;
	uint32_t BYTE_SIZE = 0;
	uint32_t RUNTIME_PTR_LO = 0;
	uint32_t RUNTIME_PTR_HI = 0;
	uint32_t HEADER_SIZE = 16;
	uint32_t DIM = 0;
	bool bPanic = false;
	S_G1T_COMPED_RESIDENT(BYTE* buffer, uint32_t& offset, int bufferLen)
	{
		// panic check
		if ((offset + HEADER_SIZE) > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}
		ROW_PITCH = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(ROW_PITCH);
		BYTE_SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(BYTE_SIZE);
		RUNTIME_PTR_LO = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(RUNTIME_PTR_LO);
		RUNTIME_PTR_HI = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(RUNTIME_PTR_HI);
		DIM = ROW_PITCH ? BYTE_SIZE / ROW_PITCH : 0;
	}
};

template <bool bBigEndian>
struct S_G1T_COMPED_OFFSETS
{
	// From file start
	uint32_t OFFSET = 0; 
	uint32_t SIZE = 0;
	uint32_t HEADER_SIZE = 8;
	bool bPanic = false;
	S_G1T_COMPED_OFFSETS(BYTE* buffer, uint32_t& offset, int bufferLen)
	{
		// panic check
		if ((offset + HEADER_SIZE) > (uint32_t)bufferLen)
		{
			bPanic = true;
			return;
		}
		OFFSET = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(OFFSET);
		SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(SIZE);
	}
};

///////////////////////////////
//      MAIN FUNCTION        //
///////////////////////////////

template <bool bBigEndian>
struct G1TG_TEXTURE
{

	std::vector<uint32_t> offsetList;
	std::vector<S_G1T_TEX_ATTR_HEADER<bBigEndian>> AttrList;
	std::vector<S_G1T_HEADER_EX<bBigEndian>> ExList;

	G1TG_TEXTURE(BYTE* buffer, int bufferLen, CArrayList<noesisTex_t*>& noeTex, noeRAPI_t* rapi, int selectedTex)
	{

		///////////////////////////////
		//   READ BASIC HEADER INFO  //
		///////////////////////////////

		// Panic check on header size
		if (bufferLen < 20)
		{
			PopUpMessage(L"G1T file too small!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
			assert(0 && "G1T too small to be a file!");
		}

		// Read basic header
		uint32_t offset = 0;
		S_G1T_HEADER<bBigEndian> G1T_HEADER = S_G1T_HEADER<bBigEndian>(buffer, offset, rapi, bufferLen);

		// Panic check on header read
		if (G1T_HEADER.bPanic)
		{
			PopUpMessage(L"G1T file header read error!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
			assert(0 && "G1T file header read error!");
		}

		offset = G1T_HEADER.HEADER_SIZE;

		// weird but the 0 file size ATTR headers are missing
		if (G1T_HEADER.FILE_SIZE != 0)
		{
			// Read ATTR header
			for (uint32_t i = 0; i < G1T_HEADER.TEX_COUNT; i++)
			{
				AttrList.push_back(S_G1T_TEX_ATTR_HEADER<bBigEndian>(buffer, offset, bufferLen));
				// Panic check on header
				if (AttrList[i].bPanic)
				{
					PopUpMessage(L"G1T_TEX_ATTR_HEADER read error!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
					assert(0 && "G1T_TEX_ATTR_HEADER read error!");
					break;
				}
			}
		}
		else
		{
			// creates a fake one
			uint32_t dummyOffset = 0;
			std::vector<uint32_t> dummyBuffer(G1T_HEADER.TEX_COUNT, 0); // Safe dummy data
			// Dummy ATTR header
			for (uint32_t i = 0; i < G1T_HEADER.TEX_COUNT; i++)
			{
				S_G1T_TEX_ATTR_HEADER<bBigEndian> dummy = S_G1T_TEX_ATTR_HEADER<bBigEndian>((BYTE*)dummyBuffer.data(), dummyOffset, (int)dummyBuffer.size());
				AttrList.push_back(dummy);
			}
		}

		if (AttrList.size() != G1T_HEADER.TEX_COUNT)
		{
			PopUpMessage(L"G1T_TEX_ATTR_HEADER missing!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
			assert(0 && "G1T_TEX_ATTR_HEADER missing error! AttrList.size() != header.TEX_COUNT");
		}

		offset = G1T_HEADER.TEX_OFFSET;

		// SKIP_TABLE offsets (data size is now computed and check as well now)
		offsetList.resize(G1T_HEADER.TEX_COUNT);
		memcpy(offsetList.data(), buffer + offset, 4 * G1T_HEADER.TEX_COUNT); offset += (4 * G1T_HEADER.TEX_COUNT);
		for (auto& offs : offsetList)
		{
			if (bBigEndian)
			{
				LITTLE_BIG_SWAP(offs);
			}
			// panic check on size
			if ((offs + G1T_HEADER.TEX_OFFSET) > (uint32_t)bufferLen)
			{
				PopUpMessage(L"G1T_SKIP_TABLE size read error!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
				assert(0 && "G1T_SKIP_TABLE size read error!\n");
			}
		}

		// panic check on size
		if (offset > (uint32_t)bufferLen)
		{
			PopUpMessage(L"G1T_SKIP_TABLE size read error!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
			assert(0 && "G1T_SKIP_TABLE size longer than buffer!");
		}

		///////////////////////////////
		//      PROCESS EX HEADER    //
		///////////////////////////////

		// create meta data for each texture
		std::vector<S_G1T_TEX_META> METAS(G1T_HEADER.TEX_COUNT);

		// for checking ex header amount in file
		uint32_t ex_count = 0;
		// for checking ex header size file
		uint32_t ex_size_read = 0;

		// Read EX headers
		if ( G1T_HEADER.HEADER_EX_SIZE != 0 )
		{
			if (bDebugLog)
			{
				LogDebug("\t\t-- EX HEADERS --\n");
			}
			do
			{
				S_G1T_HEADER_EX<bBigEndian> G1T_HEADER_EX = S_G1T_HEADER_EX<bBigEndian>(buffer, offset, bufferLen, ex_count);
				// check check
				if ( G1T_HEADER_EX.bPanic )
				{
					PopUpMessage(L"G1T_HEADER_EX read error!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
					assert(0 && "G1T_HEADER_EX read error!");
				}

				ex_count += 1;

				ex_size_read += G1T_HEADER_EX.headerSize;

				ExList.push_back(G1T_HEADER_EX);

				if (bDebugLog)
				{
					LogDebug("S_G1T_HEADER_EX #%d\n", ex_count);
					LogDebug("\tTEX_EX_TYPE:\t\t%d (%s)\n", G1T_HEADER_EX.TYPE, EX_TYPE_STR[clamp_index((int)G1T_HEADER_EX.TYPE, sizeof(EX_TYPE_STR))]);
					LogDebug("\tTEX_EX_COUNT:\t\t%d\n", G1T_HEADER_EX.COUNT);
					
					if (G1T_HEADER_EX.TYPE == TEX_EX_TYPE::ASTC_TYPE && G1T_HEADER_EX.COUNT == 1)
					{
						LogDebug("\tKT_ASTC_FORMAT:\t%d (%s)\n", G1T_HEADER_EX.KT_ASTC_FORMAT, ASTC_FORMAT_STR[clamp_index((int)G1T_HEADER_EX.KT_ASTC_FORMAT, sizeof(ASTC_FORMAT_STR))]);
					}

					if (G1T_HEADER_EX.TYPE == TEX_EX_TYPE::PS4_PLANE_ARRAY)
					{
						LogDebug("\tARRAY_DEPTH:\t\t%d\n", G1T_HEADER_EX.ARRAY_DEPTH);
					}
				}
			} while ( ex_size_read < G1T_HEADER.HEADER_EX_SIZE );
		}

		if (ex_size_read != G1T_HEADER.HEADER_EX_SIZE)
		{
			PopUpMessage(L"Extra G1T header size detected. More data might be present.");
		}

		// issue with 0 file size
		if (G1T_HEADER.FILE_SIZE == 0)
		{
			offsetList[0] = G1T_HEADER.HEADER_EX_SIZE + G1T_HEADER.TEX_COUNT * 4;
		}

		///////////////////////////////
		//      SET UP OPERATORS     //
		///////////////////////////////
		
		// PS4 swizzle (made our own)
		//typedef void(*NoesisMisc_Untile1dThin_p)(uint8_t* pDest, const uint32_t destSize, const uint8_t* pSrc, const uint32_t srcSize, const uint32_t w, const uint32_t h, const uint32_t bitsPerTexel, bool isBC, noeRAPI_t* pRapi);
		// uint8_t* pDest, const uint32_t destSize, const uint8_t* pSrc, const uint32_t srcSize, const uint32_t w, const uint32_t h, const uint32_t bitsPerTexel, bool isBC, noeRAPI_t* pRapi
		//NoesisMisc_Untile1dThin_p NoesisMisc_Untile1dThin = NULL;
		//NoesisMisc_Untile1dThin = (NoesisMisc_Untile1dThin_p)g_nfn->NPAPI_GetUserExtProc("NoesisMisc_Untile1dThin");

		///////////////////////////////
		//      PROCESS TEXTURES     //
		///////////////////////////////

		if (bDebugLog)
		{
			LogDebug("\n\t\t--TEXTURE INFO--\n\n");
		}

		for (uint32_t i = 0; i < G1T_HEADER.TEX_COUNT; i++ )
		{
			///////////////////////////////
			//       CHECK OFFSETS       //
			///////////////////////////////

			// This list is updated for alpha atlas textures as this table is incorrect for them
			uint32_t offs = offsetList[i]; 

			offset = G1T_HEADER.TEX_OFFSET + offs;

			// panic check
			if (offset > (uint32_t)bufferLen)
			{
				PopUpMessage(L"G1T_SKIP_TABLE location read error!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
				assert(0 && "G1T_SKIP_TABLE outside of file size!");
			}

			///////////////////////////////
			//      READ TEX HEADERS     //
			///////////////////////////////

			S_G1T_TEX_HEADER<bBigEndian> G1T_TEX_HEADER = S_G1T_TEX_HEADER<bBigEndian>(buffer, offset, bufferLen, i);

			// panic check
			if (G1T_TEX_HEADER.bPanic)
			{
				PopUpMessage(L"G1T_TEX_HEADER read error on texture index #%d!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.", i);
				assert(0 && "G1T_TEX_HEADER read error!\n");
			}

			///////////////////////////////
			//     COMPUTE METADATA      //
			///////////////////////////////

			// When an ex header is present, 
			// it can be more than 1 entry 
			// so we have to cycle them 
			// and parses them how the game would

			S_G1T_TEX_ATTR_HEADER G1T_TEX_ATTR_HEADER = AttrList[i];

			S_G1T_TEX_META META = METAS[i];

			std::vector<uint32_t> read_ex_headers;

			META.bIsPower2 = G1T_TEX_HEADER.bIsPower2;

			META.EXCount = G1T_TEX_HEADER.READ_G1T_HEADER_EX;

			if (META.EXCount > ex_count)
			{
				META.EXCount = ex_count;
			}

			// Not 100% always but it has been so far
			if (G1T_HEADER.SYSTEM == PLATFORM::RVL  || // working
				G1T_HEADER.SYSTEM == PLATFORM::NGP  || // working
				G1T_HEADER.SYSTEM == PLATFORM::CAFE || // working
				G1T_HEADER.SYSTEM == PLATFORM::PS4  ||
				G1T_HEADER.SYSTEM == PLATFORM::NX   || // almost 100%
				G1T_HEADER.SYSTEM == PLATFORM::PS5
				)
			{
				META.bSwizzled = true;
			}

			if (G1T_HEADER.SYSTEM == PLATFORM::NX)
			{
				META.bUseSwitchSize = true;
			}

			if (G1T_HEADER.SYSTEM == PLATFORM::NX &&
				G1T_TEX_HEADER.bIsPower2 == false)
			{
				META.bUseSwitchSize = false;
			}
			if (G1T_HEADER.SYSTEM == PLATFORM::PS5)
			{
				META.bUsePS5Size = true;
			}

			if (G1T_TEX_HEADER.EX_SWIZZLE == EX_SWIZZLE_TYPE::DX12_64kb)
			{
				META.bSwizzled = true;
			}

			if(G1T_TEX_HEADER.EX_SWIZZLE == EX_SWIZZLE_TYPE::ZLIB_COMPRESSED)
			{
				META.bSwizzled = true;
				META.bZlibCompressed = true;
			}

			if (G1T_HEADER.SYSTEM == PLATFORM::PS4)
			{
				META.pAlignment = 1024;
			}
			
			switch (G1T_TEX_ATTR_HEADER.TYPE)
			{
			case TEX_EX_TYPE::IBL_SPECULAR:
				// unsure what this flag is
				// was on a screenshot like image
				// the color channels were also backwards
				// means dont swizzle in other formats than xbox 360
				if (G1T_HEADER.SYSTEM != PLATFORM::XB2)
				{
					META.bSwizzled = false;
					META.bUseSwitchSize = false;
				}
				else
				{
					META.bChannelFlip = true;
				}
				break;
			case TEX_EX_TYPE::IBL_DIFFUSE:
				// unsure what I can do here
				// the programs lets you turn off alpha 
				break;
			case TEX_EX_TYPE::NORMALMAP:
				META.bNormalMap = true;
				break;
			case TEX_EX_TYPE::IBL_SHADOW:
				// need to figure this one out
				// was on test file for Switch seeker_wt_pb2warray2.g1t
				// Lots of full alpha images
				break;
			case TEX_EX_TYPE::SH_IRRADIANCE_VOLUME:
				// unsure what to do here, might just be a flag
				// that the textures can be palettable
				break;
			default:
				break;
			}

			// How ex headers work is that they use the CONSUMED byte when read.
			// I'm not totally sure when to consume them across the whole file
			// so it's not read on others textures. While ASTC headers make sense
			// some of these others do not. 

			// this can also change based on the system
			// but I haven't found a method that works yets based on systems
			if ( G1T_HEADER.HEADER_EX_SIZE != 0 && 
				 META.EXCount != 0
				)
			{
				// this seems to imply the starting header
				size_t ex_start = G1T_TEX_HEADER.READ_G1T_HEADER_EX - 1;

				if (ex_start > ex_count)
				{
					ex_start = ex_count;
				}

				for (size_t ex = ex_start; ex < ex_count; ex++)
				{
					if (ExList[ex].CONSUMED == 0 && META.EXCount != 0)
					{
						switch (ExList[ex].TYPE)
						{
						case TEX_EX_TYPE::IBL_SPECULAR:
							if (G1T_HEADER.SYSTEM != PLATFORM::XB2)
							{
								META.bSwizzled = false;
								META.bUseSwitchSize = false;
							}
							else
							{
								META.bChannelFlip = true;
							}
							META.bUseSwitchSize = false;
							META.bSwizzled = false;
							break;
						case TEX_EX_TYPE::NORMALMAP:
							// only ever seen these with 1, 1 data
							// and it was a normal map
							// I don't know what that means
							META.xBOX360_w = ExList[ex].xBOX360_w;
							META.xBOX360_h = ExList[ex].xBOX360_h;
							META.bSwizzled = true;
							break;
						case TEX_EX_TYPE::TLUT:
							// for sure consumed
							META.Wii_SWIZZLE = ExList[ex].Wii_SWIZZLE;
							META.bSwizzled = true;
							break;
						case TEX_EX_TYPE::CAFE_SWIZZLE:
							// I don't think this one is consumed
							// read every time for all tex
							META.WiiU_SWIZZLE = ExList[ex].WiiU_SWIZZLE;
							META.bSwizzled = true;
							break;
						case TEX_EX_TYPE::PS4_PLANE_ARRAY:
							// I don't think this one is consumed
							// read every time for all tex
							META.ARRAY_ID1 = ExList[ex].ID1;
							META.ARRAY_DEPTH = ExList[ex].ARRAY_DEPTH;
							break;
						case TEX_EX_TYPE::SH_IRRADIANCE_VOLUME:
							META.bSwizzled = true;
							break;
						case TEX_EX_TYPE::ASTC_TYPE:
							META.KT_ASTC_FORMAT = ExList[ex].KT_ASTC_FORMAT;
							break;
						default:
							break;
						}
						META.EXTRA_INTS = ExList[ex].EXTRA_INTS;
						read_ex_headers.push_back(ex);
						ExList[ex].CONSUMED = 1;
						META.EXCount--;
					} 
				} 
			}

			uint32_t WIDTH = max(G1T_TEX_HEADER.WIDTH, 1);

			uint32_t HEIGHT = max(G1T_TEX_HEADER.HEIGHT, 1);

			uint32_t MIPS = max(G1T_TEX_HEADER.MIP_COUNT, 1);

			uint32_t DEPTH = max(G1T_TEX_HEADER.DEPTH, 1);

			if (G1T_TEX_HEADER.KTGL_TEXTURE_TYPE == S_GT1_LOAD_TYPE::CUBE)
			{
				DEPTH = 6;
			}

			// Not sure why this is a thing but I need it for size
			if ( META.ARRAY_DEPTH != 1)
			{
				DEPTH = META.ARRAY_DEPTH;
			}

			// Find faces
			uint32_t FACES = 1;

			if (G1T_TEX_HEADER.HAS_TEX_EX_HEADER > 0 &&
				G1T_TEX_HEADER.EX_FACES != 0)
			{
				FACES = G1T_TEX_HEADER.EX_FACES;
			}

			// Find plane count
			uint32_t PLANE_COUNT = 1;

			if ( G1T_TEX_HEADER.KTGL_TEXTURE_TYPE == S_GT1_LOAD_TYPE::PLANAR )
			{
				PLANE_COUNT = 1;
			}
			else if (G1T_TEX_HEADER.KTGL_TEXTURE_TYPE == S_GT1_LOAD_TYPE::PLANE_ARRAY)
			{
				if (G1T_TEX_HEADER.HAS_TEX_EX_HEADER > 0 &&
					G1T_TEX_HEADER.EX_ARRAY_COUNT != 0)
				{
					PLANE_COUNT = G1T_TEX_HEADER.EX_ARRAY_COUNT;
				}
			}
			else if ( G1T_TEX_HEADER.KTGL_TEXTURE_TYPE == S_GT1_LOAD_TYPE::CUBE_ARRAY )
			{
				if (G1T_TEX_HEADER.HAS_TEX_EX_HEADER > 0 &&
					G1T_TEX_HEADER.EX_ARRAY_COUNT != 0
				)
				{
					PLANE_COUNT = G1T_TEX_HEADER.EX_ARRAY_COUNT; // * depth for total later
				}
			}
			else if (G1T_TEX_HEADER.KTGL_TEXTURE_TYPE == S_GT1_LOAD_TYPE::VOLUME)
			{
				if (G1T_TEX_HEADER.HAS_TEX_EX_HEADER > 0 &&
					G1T_TEX_HEADER.EX_ARRAY_COUNT != 0)
				{
					PLANE_COUNT = (uint32_t)pow(2, G1T_TEX_HEADER.EX_ARRAY_COUNT);
				}
			}

			// Size of the first mip image data
			uint32_t firstMipSize = 0;
			// full size of all the image data including mips
			uint32_t totalTexBufferLen = 0;
			// texture size in bits per pixel
			double bitsPerPixel = 0;
			// smallest byte amount for a single pixel (or block size for compressed)
			uint32_t minBytes = 0;
			// for BC1-7 & DXT
			int fourccFormat = -1;
			// for compressed block sizes dims
			int blockWidth = 1, blockHeight = 1;
			// raw format bit count per channels, uses rgbampc. Normally rgba but has extras: 
			// m is for max value based on bits, 
			// p for padding, 
			// c is for constant 00 or 11 as 0.0 or 1.0/255
			// Speical cases are converted outside of the Noesis API
			// d for depth (covers single value to all rgb channels) 
			// s for stencil (that converts to alpha) 
			// And for lumiance we just use r, 
			std::string rawFormat = "";

			///////////////////////////////
			//    TEXTURE INDEX SWITCH   //
			///////////////////////////////

			switch (G1T_TEX_HEADER.PIXEL_FORMAT)
			{
			case KTGL_PIXEL_FORMAT::RGBA8: // GL_RGBA GL_RGBA GL_UNSIGNED_BYTE
				rawFormat = "r8g8b8a8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = false;
				break;
			case KTGL_PIXEL_FORMAT::BGRA8: // GL_BGRA GL_BGRA GL_UNSIGNED_BYTE
				rawFormat = "b8g8r8a8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = false;
				if (G1T_HEADER.SYSTEM == PLATFORM::XB2)
				{
					META.bChannelFlip = true;
				}
				break;
			case KTGL_PIXEL_FORMAT::R32F: // GL_R32F GL_RED GL_FLOAT
				rawFormat = "r#F32"; // should be working
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bFloat = false;
				break;
			case KTGL_PIXEL_FORMAT::RGBA16F: // GL_RGBA16F GL_RGBA GL_HALF_FLOAT_OES
				rawFormat = "r#F16g#F16b#F16a#F16"; // should be working
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bHalfFloat = false;
				break;
			case KTGL_PIXEL_FORMAT::RGBA32F: // GL_RGBA32F GL_RGBA GL_FLOAT
				rawFormat = "r#F32g#F32b#F32a#F32"; // should be working
				bitsPerPixel = 0x80;
				minBytes = 0x10;
				META.bFloat = false;
				break;
			case KTGL_PIXEL_FORMAT::Z24S8: // GL_DEPTH_STENCIL GL_DEPTH_STENCIL GL_UNSIGNED_INT_24_8
				rawFormat = "d24s8"; // convert to r24g24b24a8
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsDepth = false;
				META.bD24_8Convert = false;
				META.bIsUNorm = false;
				break;
			case KTGL_PIXEL_FORMAT::DXT1: // GL_COMPRESSED_RGBA_S3TC_DXT1_EXT GL_RGBA GL_UNSIGNED_BYTE BC1
				fourccFormat = FOURCC_DXT1; // convert to r8g8b8a8
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				META.bCompressedFormat = true;
				if (G1T_HEADER.SYSTEM == PLATFORM::XB2)
				{
					META.bBigEndianShortSwap = true;
				}
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = false;
				break;
			case KTGL_PIXEL_FORMAT::DXT3: // GL_COMPRESSED_RGBA_S3TC_DXT3_EXT GL_RGBA GL_UNSIGNED_BYTE BC2
				fourccFormat = FOURCC_DXT3; // convert to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				if (G1T_HEADER.SYSTEM == PLATFORM::XB2)
				{
					META.bBigEndianShortSwap = true;
				}
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::DXT5: // GL_COMPRESSED_RGBA_S3TC_DXT5_EXT GL_RGBA GL_UNSIGNED_BYTE BC3
				fourccFormat = FOURCC_DXT5; // convert to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				if (G1T_HEADER.SYSTEM == PLATFORM::XB2)
				{
					META.bBigEndianShortSwap = true;
				}
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RGBA8: // DXGI_FORMAT_R8G8B8A8_TYPELESS DXGI_FORMAT_R8G8B8A8_UNORM DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
				rawFormat = "r8g8b8a8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = true;
				META.bSwizzled = true; // unsure if this 100%
				break;
			case KTGL_PIXEL_FORMAT::TILE_BGRA8: // DXGI_FORMAT_B8G8R8A8_TYPELESS DXGI_FORMAT_B8G8R8A8_UNORM DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
				rawFormat = "b8g8r8a8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = true;
				META.bSwizzled = true; // unsure if this 100%
				break;
			case KTGL_PIXEL_FORMAT::TILE_R32F: // DXGI_FORMAT_R32_TYPELESS DXGI_FORMAT_R32_FLOAT DXGI_FORMAT_R32_FLOAT
				rawFormat = "r#F32"; // should be working
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RGBA16F: // DXGI_FORMAT_R16G16B16A16_TYPELESS DXGI_FORMAT_R16G16B16A16_FLOAT DXGI_FORMAT_R16G16B16A16_FLOAT
				rawFormat = "r#F16g#F16b#F16a#F16"; // should be working
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bHalfFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RGBA32F: // DXGI_FORMAT_R32G32B32A32_TYPELESS DXGI_FORMAT_R32G32B32A32_FLOAT DXGI_FORMAT_R32G32B32A32_FLOAT 
				rawFormat = "r#F32g#F32b#F32a#F32"; // should be working
				bitsPerPixel = 0x80;
				minBytes = 0x10;
				META.bFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::RGB10FA2F: // SCE_GXM_TEXTURE_FORMAT_U2F10F10F10 SWIZZLE4_ABGR
				rawFormat = "a2b#F10g#F10r#F10"; // converts to r#F32g#F32b#F32a#F32
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bConvert10BitFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::A8: // GL_ALPHA GL_ALPHA GL_UNSIGNED_BYTE
				rawFormat = "a8";
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_DXT1: // DXGI_FORMAT_BC1_TYPELESS DXGI_FORMAT_BC1_UNORM DXGI_FORMAT_BC1_UNORM_SRGB DXT1 (PS Vita 4x4 Swizzle & sometimes flip)
				fourccFormat = FOURCC_DXT1; // convert to r8g8b8a8
				bitsPerPixel = 0x04; // mortonWidth
				minBytes = 0x08;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				META.bSwizzled = true; // 4x4
				break;
			case KTGL_PIXEL_FORMAT::TILE_DXT3: // DXGI_FORMAT_BC2_TYPELESS DXGI_FORMAT_BC2_UNORM DXGI_FORMAT_BC2_UNORM_SRGB DXT2 / DXT3 (PS Vita 4x4 Swizzle & sometimes flip)
				fourccFormat = FOURCC_DXT3; // convert to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				META.bSwizzled = true; // 4x4
				break;
			case KTGL_PIXEL_FORMAT::TILE_DXT5: // DXGI_FORMAT_BC3_TYPELESS DXGI_FORMAT_BC3_UNORM DXGI_FORMAT_BC3_UNORM_SRGB DXT4 / DXT5 (PS Vita 4x4 Swizzle & sometimes flip)
				fourccFormat = FOURCC_DXT5; // convert to r8g8b8a8
				bitsPerPixel = 0x08; // mortonWidth
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				META.bSwizzled = true; // 4x4
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z24S8: // DXGI_FORMAT_R24G8_TYPELESS DXGI_FORMAT_R24_UNORM_X8_TYPELESS DXGI_FORMAT_D24_UNORM_S8_UINT
				rawFormat = "d24s8"; // convert to r24g24b24a8
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsDepth = true;
				META.bD24_8Convert = true;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::Z16: // GL_DEPTH_COMPONENT16 GL_DEPTH_COMPONENT GL_UNSIGNED_SHORT
				rawFormat = "d16"; // convert to r16g16b16
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsDepth = true;
				META.bD16Convert = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z16: // DXGI_FORMAT_R16_TYPELESS DXGI_FORMAT_R16_UNORM DXGI_FORMAT_D16_UNORM
				rawFormat = "d16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::R16: // GL_R16 GL_RED GL_UNSIGNED_SHORT
				rawFormat = "r16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R16: // DXGI_FORMAT_R16_TYPELESS DXGI_FORMAT_R16_UNORM DXGI_FORMAT_R16_UNORM
				rawFormat = "r16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_A8: // DXGI_FORMAT_A8_UNORM DXGI_FORMAT_A8_UNORM DXGI_FORMAT_A8_UNORM
				rawFormat = "a8";
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::B5G6R5: // DXGI_FORMAT_B5G6R5_UNORM DXGI_FORMAT_B5G6R5_UNORM DXGI_FORMAT_B5G6R5_UNORM
				rawFormat = "b5g6r5";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::B5G5R5A1: // DXGI_FORMAT_B5G5R5A1_UNORM DXGI_FORMAT_B5G5R5A1_UNORM DXGI_FORMAT_B5G5R5A1_UNORM
				rawFormat = "b5g5r5a1";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::B4G4R4A4: // SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_SWIZZLE4_ARGB
				rawFormat = "a4r4g4b4"; // could also be b4g4r4a4
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_B5G6R5: // DXGI_FORMAT_B5G6R5_UNORM DXGI_FORMAT_B5G6R5_UNORM DXGI_FORMAT_B5G6R5_UNORM
				rawFormat = "b5g6r5";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_B5G5R5A1: // DXGI_FORMAT_B5G5R5A1_UNORM DXGI_FORMAT_B5G5R5A1_UNORM DXGI_FORMAT_B5G5R5A1_UNORM
				rawFormat = "b5g5r5a1";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_B4G4R4A4: // SCE_GXM_TEXTURE_FORMAT_U4U4U4U4_SWIZZLE4_ARGB
				rawFormat = "a4r4g4b4"; // could also be b4g4r4a4
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::Z24FS8: // GL_R32F GL_RED GL_FLOAT
				rawFormat = "r#F32";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z24FS8: // DXGI_FORMAT_R32_TYPELESS DXGI_FORMAT_R32_FLOAT DXGI_FORMAT_R32_FLOAT
				rawFormat = "d24s8"; // could also be R32 float
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsDepth = true;
				META.bD24_8Convert = true;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::BGRX8: // GL_BGRA GL_BGRA GL_UNSIGNED_BYTE
				rawFormat = "b8g8r8a8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BGRX8: // DXGI_FORMAT_B8G8R8X8_TYPELESS DXGI_FORMAT_B8G8R8X8_UNORM DXGI_FORMAT_B8G8R8X8_UNORM_SRGB
				rawFormat = "b8g8r8p8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::GR16: // DXGI_FORMAT_R16G16_TYPELESS DXGI_FORMAT_R16G16_UNORM DXGI_FORMAT_R16G16_UNORM 
				rawFormat = "r16g16";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_GR16: // DXGI_FORMAT_R16G16_TYPELESS DXGI_FORMAT_R16G16_UNORM DXGI_FORMAT_R16G16_UNORM (same as above but had a 0 bpp error)
				rawFormat = "g16r16";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_B5G5R5A3: // GX_TF_RGB5A3 GX_TF_RGB5A3 GX_TF_RGB5A3 (Wii)
				rawFormat = "r5g5b5a3"; // also seens as BC6H
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BG8: // SEC_G8R8 SEC_G8R8 SEC_UNORM
				rawFormat = "g8b8";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RA8: // SCE_GXM_TEXTURE_FORMAT_U8U8_SWIZZLE2_GRRR (g8r8)
				rawFormat = "a8r8";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_B8: // SEC_R8 SEC_R8 SEC_UNORM 
				rawFormat = "b8"; // XXRX = Red -> Blue
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_G8: // SEC_R8 SEC_R8 SEC_UNORM (dupe of 0x28 so might have another thing going on)
				rawFormat = "g8"; // XRXX = Red -> Blue
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R8: // GL_R8 GL_RED GL_UNSIGNED_BYTE
				rawFormat = "r8"; // RXXX
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_L4: // PICA_LUMINANCE PICA_LUMINANCE PICA_UNORM_4_HALF_BYTE
				rawFormat = "r4"; // just making this red as it's a weird format
				                  // channels can be seen as RRRX
				                  // also seen as BC6H
				bitsPerPixel = 0x04;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_L8: // PICA_LUMINANCE PICA_LUMINANCE GL_UNSIGNED_BYTE
				rawFormat = "r8";
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_LA4: // PICA_LUMINANCE_A PICA_LUMINANCE_A PICA_UNORM_44_BYTE
				rawFormat = "r4a4"; // just making this red as it's a weird format
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_LA8: // PICA_LUMINANCE_A PICA_LUMINANCE_A GL_UNSIGNED_BYTE
				rawFormat = "r8a8"; // just making this red as it's a weird format
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z4: // GX_TF_I4 GX_TF_I4 GX_TF_I4
				rawFormat = "r4";
				bitsPerPixel = 0x04;
				minBytes = 0x01;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z8: // SCE_GXM_TEXTURE_FORMAT_U8_SWIZZLE1_111R (r8)
				rawFormat = "r8"; // might be a special case depending on the system
				                  // NWii:   GX_TF_Z8
						          // X360:   R8
				                  // PSVita: SCE_GXM_TEXTURE_FORMAT_U8_SWIZZLE1_111R
				                  // PS4:    R8
								  // PS5:    A8
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_INDEX4: // SCE_GXM_TEXTURE_FORMAT_P4_SWIZZLE4_ABGR (CI4)  (a8b8g8r8)
				rawFormat = "r4"; // Converts to r8g8b8a8
							      // NWii:   GX_TF_CI4
				                  // PSVita: SCE_GXM_TEXTURE_FORMAT_P4_SWIZZLE4_ABGR
				bitsPerPixel = 0x04;
				minBytes = 0x01;
				META.bPalette4 = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_INDEX8: // SCE_GXM_TEXTURE_FORMAT_P8_SWIZZLE4_ABGR (CI8) (a8b8g8r8)
				rawFormat = "r8"; // Converts to r8g8b8a8
								  // NWii:   GX_TF_CI8
							      // PSVita: SCE_GXM_TEXTURE_FORMAT_P8_SWIZZLE4_ABGR
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bPalette8 = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_INDEX14X2: // GX_TF_CI14 GX_TF_CI14 GX_TF_CI14
				rawFormat = "r14"; // Converts to r8g8b8a8
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bPalette14 = true;
				break;
			case KTGL_PIXEL_FORMAT::R5G6B5: // GL_RGB GL_RGB GL_UNSIGNED_SHORT_5_6_5     b5g6r5
				rawFormat = "b5g6r5";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::R5G5B5A1: // GL_RGBA GL_RGBA GL_UNSIGNED_SHORT_5_5_5_1
				rawFormat = "a1b5g5r5"; // might be r5g5b5a1
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::R4G4B4A4: // GL_RGBA GL_RGBA GL_UNSIGNED_SHORT_4_4_4_4 a4b4g4r4
				rawFormat = "a4b4g4r4"; // might be r4g4b4a4
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::INDEX4: // SCE_GXM_TEXTURE_FORMAT_P4_SWIZZLE4_ABGR 
				rawFormat = "r4";
				bitsPerPixel = 0x04;
				minBytes = 0x01;
				META.bPalette4 = true;
				break;
			case KTGL_PIXEL_FORMAT::INDEX8: // GL_R8I GL_RED_INTEGER GL_BYTE
				rawFormat = "r8"; // Converts to r8g8b8a8
				                  // PSVita: SCE_GXM_TEXTURE_FORMAT_P8_SWIZZLE4_ABGR
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bPalette8 = true;
				break;
			case KTGL_PIXEL_FORMAT::INDEX16: // GL_R16I GL_RED_INTEGER GL_SHORT
				rawFormat = "r16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bSigned = true;
				break;
			case KTGL_PIXEL_FORMAT::INDEX32: // GL_R32I GL_RED_INTEGER GL_INT
				rawFormat = "r32";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bSigned = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R5G6B5: // BNTX_R5G6B5_UNORM BNTX_R5G6B5_UNORM, BNTX_R5G6B5_UNORM      (found in switch)
				rawFormat = "r5g6b5";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R5G5B5A1: // BNTX_R5G5B5A1_UNORM BNTX_R5G5B5A1_UNORM BNTX_R5G5B5A1_UNORM (found in switch)
				rawFormat = "r5g5b5a1";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R4G4B4A4: // BNTX_R4G4B4A4_UNORM BNTX_R4G4B4A4_UNORM BNTX_R4G4B4A4_UNORM (found in switch)
				rawFormat = "r4g4b4a4";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_INDEX16: // GL_R16I GL_RED_INTEGER GL_SHORT (likely 0x39 dupe)
				rawFormat = "r16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bSigned = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_INDEX32: // GL_R32I GL_RED_INTEGER GL_INT   (likely 0x3A dupe)
				rawFormat = "r32";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bSigned = true;
				break;
			case KTGL_PIXEL_FORMAT::R10G10B10A2: // GL_RGB10_A2 GL_RGBA GL_UNSIGNED_INT_2_10_10_10_REV
				rawFormat = "r10g10b10a2";
				bitsPerPixel = 0x20;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::R16G16B16A16: // GL_RGBA16_EXT GL_RGBA GL_UNSIGNED_SHORT
				rawFormat = "r16g16b16a16";
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R10G10B10A2: // DXGI_FORMAT_R10G10B10A2_UNORM DXGI_FORMAT_R10G10B10A2_UNORM DXGI_FORMAT_R10G10B10A2_UNORM
				rawFormat = "r10g10b10a2";
				bitsPerPixel = 0x20;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R16G16B16A16: // DXGI_FORMAT_R16G16B16A16_TYPELESS DXGI_FORMAT_R16G16B16A16_UNORM DXGI_FORMAT_R16G16B16A16_UNORM 
				rawFormat = "r16g16b16a16";
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_A4: // PICA_A PICA_A PICA_UNORM_4_HALF_BYTE (3DS)
				rawFormat = "r4";
				bitsPerPixel = 0x04;
				minBytes = 0x01;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RGB8: // GL_RGB GL_RGB GL_UNSIGNED_BYTE       (3DS) (PS Vita swizzle 32x32 tiles)
				rawFormat = "r8g8b8";
				bitsPerPixel = 0x18;
				minBytes = 0x03;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RG8: // DXGI_FORMAT_R8G8_UNORM DXGI_FORMAT_R8G8_UNORM DXGI_FORMAT_R8G8_UNORM (3DS Swizzle)
				rawFormat = "r8g8";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_ETC1: // PICA_ETC1_RGB8 PICA_ETC1_RGB8 GL_RGB 
				META.bPICAETC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsETC1 = true;
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::TILE_ETC1A4: // PICA_ETC1_RGB8A4 PICA_ETC1_RGB8A4 GL_RGBA (has 2 block per to account for the alpha)
				META.bPICAETC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsETC1 = true;
				bitsPerPixel = 0x04;
				minBytes = 0x10;
				META.b3DSAlpha = true;
				break;
			case KTGL_PIXEL_FORMAT::RGB8: // GL_RGB GL_RGB GL_UNSIGNED_BYTE
				rawFormat = "r8g8b8";
				bitsPerPixel = 0x18;
				minBytes = 0x03;
				break;
			case KTGL_PIXEL_FORMAT::Z24: // GL_DEPTH_COMPONENT GL_DEPTH_COMPONENT GL_UNSIGNED_INT (r8g8b8) (also 3DS)
				rawFormat = "r8g8b8";
				bitsPerPixel = 0x18;
				minBytes = 0x03;
				META.bIsDepth = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z24: // GL_DEPTH_COMPONENT GL_DEPTH_COMPONENT GL_UNSIGNED_INT (r8g8b8) (like a dupe of above)
				rawFormat = "r8g8b8";
				bitsPerPixel = 0x18;
				minBytes = 0x03;
				META.bIsDepth = true;
				break;
			case KTGL_PIXEL_FORMAT::GR32F: // DXGI_FORMAT_R32G32_TYPELESS DXGI_FORMAT_R32G32_FLOAT DXGI_FORMAT_R32G32_FLOAT
				rawFormat = "r#F32g#F32";
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_GR32F: // DXGI_FORMAT_R32G32_TYPELESS DXGI_FORMAT_R32G32_FLOAT DXGI_FORMAT_R32G32_FLOAT
				rawFormat = "r#F32g#F32";
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::Z32F: // GL_DEPTH_COMPONENT32F GL_DEPTH_COMPONENT GL_FLOAT (R32F)
				rawFormat = "r#F32";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsDepth = true;
				META.bFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z32F: // DXGI_FORMAT_R32_TYPELESS DXGI_FORMAT_R32_FLOAT DXGI_FORMAT_D32_FLOAT
				rawFormat = "d#F32"; // converts to r#F32g#F32b#F32
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bIsDepth = true;
				META.bD32Convert = true;
				META.bFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::PVRTII2: // SCE_GXM_TEXTURE_FORMAT_PVRTII2BPP_SWIZZLE4_ABGR
				META.bIsPVRTC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				bitsPerPixel = 0x02;
				blockWidth = 8;
				blockHeight = 4;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::TILE_PVRTII2: // SCE_GXM_TEXTURE_FORMAT_PVRTII2BPP_SWIZZLE4_ABGR
				META.bIsPVRTC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				bitsPerPixel = 0x02;
				blockWidth = 8;
				blockHeight = 4;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::PVRTII4: // SCE_GXM_TEXTURE_FORMAT_PVRTII4BPP_SWIZZLE4_ABGR
				META.bIsPVRTC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				bitsPerPixel = 0x04;
				blockWidth = 4;
				blockHeight = 4;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::TILE_PVRTII4: // SCE_GXM_TEXTURE_FORMAT_PVRTII4BPP_SWIZZLE4_ABGR 
				META.bIsPVRTC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				bitsPerPixel = 0x04;
				blockWidth = 4;
				blockHeight = 4;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::L8: // GL_LUMINANCE GL_LUMINANCE GL_UNSIGNED_BYTE
				rawFormat = "r8";
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::LA8: // GL_LUMINANCE_ALPHA GL_LUMINANCE_ALPHA GL_UNSIGNED_BYTE (also seen as G8R8)
				rawFormat = "r8a8";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::ETC1: // GL_ETC1_RGB8_OES GL_RGB GL_UNSIGNED_BYTE
				META.bIsETC1 = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				rawFormat = "RGB";
				if (G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE == KTGL_GD_COLOR_SPACE_TYPE::sRGB_FROM_LINEAR)
				{
					rawFormat = "sRGB";
				}
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::PVRT2: // GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG GL_RGBA GL_UNSIGNED_BYTE
				META.bIsPVRTC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 8;
				blockHeight = 4;
				bitsPerPixel = 0x02;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::PVRT4: // GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG GL_RGBA GL_UNSIGNED_BYTE
				META.bIsPVRTC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::BC1: // GL_COMPRESSED_RGBA_S3TC_DXT1_EXT GL_RGBA GL_UNSIGNED_BYTE (BC1)
				fourccFormat = FOURCC_DXT1; // converts to r8g8b8a8
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::BC2: // GL_COMPRESSED_RGBA_S3TC_DXT3_EXT GL_RGBA GL_UNSIGNED_BYTE (BC2)
				fourccFormat = FOURCC_DXT3; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::BC3: // GL_COMPRESSED_RGBA_S3TC_DXT5_EXT GL_RGBA GL_UNSIGNED_BYTE (BC3)
				fourccFormat = FOURCC_DXT5; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::BC4: // DXGI_FORMAT_BC4_TYPELESS DXGI_FORMAT_BC4_UNORM DXGI_FORMAT_BC4)_UNORM 
				fourccFormat = FOURCC_BC4; // converts to r8g8b8a8
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::BC5: // DXGI_FORMAT_BC5_TYPELESS DXGI_FORMAT_BC5_UNORM DXGI_FORMAT_BC5_UNORM 
				fourccFormat = FOURCC_BC5; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::BC6: // DXGI_FORMAT_BC6H_TYPELESS DXGI_FORMAT_BC6H_UF16 DXGI_FORMAT_BC6H_UF16 // Uses cubemaps
				fourccFormat = FOURCC_BC6H; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::BC7: // DXGI_FORMAT_BC7_TYPELESS DXGI_FORMAT_BC7_UNORM DXGI_FORMAT_BC7_UNORM_SRGB  
				fourccFormat = FOURCC_BC7; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BC1: // DXGI_FORMAT_BC1_TYPELESS DXGI_FORMAT_BC1_UNORM DXGI_FORMAT_BC1_UNORM_SRGB  DXT1 (swizzled)
				fourccFormat = FOURCC_BC1; // converts to r8g8b8a8
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				META.bSwizzled = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BC2: // DXGI_FORMAT_BC2_TYPELESS DXGI_FORMAT_BC2_UNORM DXGI_FORMAT_BC2_UNORM_SRGB  DXT2 / DXT3 (swizzled)
				fourccFormat = FOURCC_BC2; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				META.bSwizzled = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BC3: // DXGI_FORMAT_BC3_TYPELESS DXGI_FORMAT_BC3_UNORM DXGI_FORMAT_BC3_UNORM_SRGB  DXT4 / DXT5 (swizzled)
				fourccFormat = FOURCC_BC3; // converts to r8g8b8a8
				bitsPerPixel = 0x04;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bIsUNorm = true;
				META.bSwizzled = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BC4: // DXGI_FORMAT_BC4_TYPELESS DXGI_FORMAT_BC4_UNORM DXGI_FORMAT_BC4_UNORM ATI1  (swizzled)
				fourccFormat = FOURCC_BC4; // converts to r8g8b8a8
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				META.bSwizzled = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BC5: // DXGI_FORMAT_BC5_TYPELESS DXGI_FORMAT_BC5_UNORM DXGI_FORMAT_BC5_UNORM ATI2  (swizzled)
				fourccFormat = FOURCC_BC5; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				META.bSwizzled = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BC6: // DXGI_FORMAT_BC6H_TYPELESS DXGI_FORMAT_BC6H_UF16 DXGI_FORMAT_BC6H_UF16      (swizzled)
				fourccFormat = FOURCC_BC6H; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				META.bSwizzled = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_BC7: // DXGI_FORMAT_BC7_TYPELESS DXGI_FORMAT_BC7_UNORM DXGI_FORMAT_BC7_UNORM_SRGB  (swizzled)
				fourccFormat = FOURCC_BC7; // converts to r8g8b8a8
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				META.bNormalized = false;
				META.bIsUNorm = true;
				META.bSwizzled = true;
				break;
			case KTGL_PIXEL_FORMAT::RGBA8U: // GL_RGBA8UI GL_RGBA_INTEGER GL_UNSIGNED_BYTE
				rawFormat = "r8g8b8a8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				break;
			case KTGL_PIXEL_FORMAT::RG8U: // GL_RG8UI GL_RG_INTEGER GL_UNSIGNED_BYTE
				rawFormat = "r8g8";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				break;
			case KTGL_PIXEL_FORMAT::RG16F: // GL_RG16F GL_RG GL_HALF_FLOAT_OES
				rawFormat = "r#F16g#F16";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bHalfFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::R16F: // GL_R16F GL_RED GL_HALF_FLOAT_OES
				rawFormat = "r#F16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bHalfFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::R11G11B10F: // GL_R11F_G11F_B10F GL_RGB GL_UNSIGNED_INT_10F_11F_11F_REV
				rawFormat = "r#F11g#F11b#F10"; // very special case
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bConvert11Bit10BitFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::Z32FS8: // GL_DEPTH32F_STENCIL8 GL_DEPTH_COMPONENT GL_FLOAT_32_UNSIGNED_INT_24_8_REV (df32p24s8)
				rawFormat = "d#F32p24s8";
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bIsDepth = true;
				META.bFloat = true;
				META.bD32FloatConvert = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z32FS8: // DXGI_FORMAT_R32G8X24_TYPELESS DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS DXGI_FORMAT_D32_FLOAT_S8X24_UINT 
				rawFormat = "d#F32s8p24";
				bitsPerPixel = 0x40;
				minBytes = 0x08;
				META.bIsDepth = true;
				META.bFloat = true;
				META.bD32FloatConvert = true;
				break;
			case KTGL_PIXEL_FORMAT::Z16S8: // SEC_R16 SEC_R16 SEC_UNORM
				rawFormat = "r16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::ETC1RGBETC1A: // GL_ETC1_RGB8_OES GL_RGB GL_UNSIGNED_BYTE (alpha under, bpp adjusted)
				META.bIsETC1 = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				blockWidth = 4;
				blockHeight = 4;
				rawFormat = "RGB";
				if (G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE == KTGL_GD_COLOR_SPACE_TYPE::sRGB_FROM_LINEAR)
				{
					rawFormat = "sRGB";
				}
				bitsPerPixel = 0x04; // hard codes to 0x08 but we adjust the height
				HEIGHT *= 2;
				minBytes = 0x08;
				META.bHasAlphaAtlas = true;
				break;
			case KTGL_PIXEL_FORMAT::PVRT4RGBPVRT4A: // GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG GL_RGB GL_UNSIGNED_BYTE (likely alpha under, bpp adjusted)
				META.bIsPVRTC = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				bitsPerPixel = 0x04; // hard codes to 0x08 but we adjust the height
				HEIGHT *= 2;
				minBytes = 0x08;
				META.bHasAlphaAtlas = true;
				break;
			case KTGL_PIXEL_FORMAT::ETC2RGBA8: // GL_COMPRESSED_RGBA8_ETC2_EAC GL_RGBA GL_UNSIGNED_BYTE
				META.bIsETC2 = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				rawFormat = "RGBA";
				if (G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE == KTGL_GD_COLOR_SPACE_TYPE::sRGB_FROM_LINEAR)
				{
					rawFormat = "sRGBA";
				}
				bitsPerPixel = 0x04;  // hard codes to 0x08 but we adjust the height
				HEIGHT *= 2;
				minBytes = 0x08;
				META.bHasAlphaAtlas = true;
				// in this case we skip alpha atlas but include the extra image data (this has to be a bug in KTGL?)
				META.bSkipAlphaAtlas = true;
				break;
			case KTGL_PIXEL_FORMAT::R8: // GL_R8 GL_RED GL_UNSIGNED_BYTE
				rawFormat = "r8";
				bitsPerPixel = 0x08;
				minBytes = 0x01;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::RG8: // GL_RG8 GL_RG GL_UNSIGNED_BYTE
				rawFormat = "r8g8";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RGBA8U: // DXGI_FORMAT_R8G8B8A8_TYPELESS DXGI_FORMAT_R8G8B8A8_UINT DXGI_FORMAT_R8G8B8A8_UINT
				rawFormat = "r8g8b8a8";
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RG8U: // DXGI_FORMAT_R8G8_TYPELESS DXGI_FORMAT_R8G8_UINT DXGI_FORMAT_R8G8_UINT
				rawFormat = "r8g8";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				break;
			case KTGL_PIXEL_FORMAT::TILE_RG16F: // DXGI_FORMAT_R16G16_TYPELESS DXGI_FORMAT_R16G16_FLOAT DXGI_FORMAT_R16G16_FLOAT
				rawFormat = "r#F16g#F16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bHalfFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R16F: // DXGI_FORMAT_R16_TYPELESS DXGI_FORMAT_R16_FLOAT DXGI_FORMAT_R16_FLOAT
				rawFormat = "r#F16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bHalfFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_R11G11B10F: // DXGI_FORMAT_R11G11B10_FLOAT GL_HALF_FLOAT_OES GL_HALF_FLOAT_OES
				rawFormat = "r#F11g#F11b#F10"; // very special case
				bitsPerPixel = 0x20;
				minBytes = 0x04;
				META.bConvert11Bit10BitFloat = true;
				break;
			case KTGL_PIXEL_FORMAT::TILE_Z16S8: // SEC_R16 SEC_R16 SEC_UNORM (dupe of 0x6E so something else might be going on)
				rawFormat = "r16";
				bitsPerPixel = 0x10;
				minBytes = 0x02;
				META.bIsUNorm = true;
				break;
			case KTGL_PIXEL_FORMAT::EACR11: // GL_COMPRESSED_R11_EAC GL_RED GL_UNSIGNED_BYTE
				META.bIsETC2 = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				rawFormat = "R";
				if (G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE == KTGL_GD_COLOR_SPACE_TYPE::sRGB_FROM_LINEAR)
				{
					rawFormat = "Rs";
				}
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::EACRG11: // GL_COMPRESSED_RG11_EAC GL_RG GL_UNSIGNED_BYTE
				META.bIsETC2 = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				rawFormat = "RG";
				if (G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE == KTGL_GD_COLOR_SPACE_TYPE::sRGB_FROM_LINEAR)
				{
					rawFormat = "RGs";
				}
				bitsPerPixel = 0x08;
				minBytes = 0x10;
				break;
			case KTGL_PIXEL_FORMAT::ETC2RGB8A1: // GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 GL_RGBA GL_UNSIGNED_BYTE
				META.bIsETC2 = true; // converts to r8g8b8a8
				META.bCompressedFormat = true;
				blockWidth = 4;
				blockHeight = 4;
				rawFormat = "RGBA1";
				if (G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE == KTGL_GD_COLOR_SPACE_TYPE::sRGB_FROM_LINEAR)
				{
					rawFormat = "sRGBA1";
				}
				bitsPerPixel = 0x04;
				minBytes = 0x08;
				break;
			case KTGL_PIXEL_FORMAT::ASTC: //  ASTC uses ASTC_SUB format
			case KTGL_PIXEL_FORMAT::ASTC_TILE: //  likely hardcoded NSwitch swizzled
				rawFormat = "ASTC_5x5"; // converts to r8g8b8a8
				blockWidth = 5;
				blockHeight = 5;
				META.bCompressedFormat = true;
				bitsPerPixel = 5.12;
				minBytes = 0x10;
				META.bIsASTC = true;
				// unsure why this is but found unswizzled if not power 2
				if (G1T_HEADER.HEADER_EX_SIZE)
				{
					switch (META.KT_ASTC_FORMAT)
					{
					case EG1TASTCFormat::ASTC_4x4:
						rawFormat = "ASTC_4x4";
						bitsPerPixel = 8.0;
						blockWidth = 4;
						blockHeight = 4;
						break;
					case EG1TASTCFormat::ASTC_5x4:
						rawFormat = "ASTC_5x4";
						bitsPerPixel = 6.40;
						blockWidth = 5;
						blockHeight = 4;
						break;
					case EG1TASTCFormat::ASTC_5x5:
						rawFormat = "ASTC_5x5";
						bitsPerPixel = 5.12;
						blockWidth = 5;
						blockHeight = 5;
						break;
					case EG1TASTCFormat::ASTC_6x5:
						rawFormat = "ASTC_6x5";
						bitsPerPixel = 4.27;
						blockWidth = 6;
						blockHeight = 5;
						break;
					case EG1TASTCFormat::ASTC_6x6:
						rawFormat = "ASTC_6x6";
						bitsPerPixel = 3.56;
						blockWidth = 6;
						blockHeight = 6;
						break;
					case EG1TASTCFormat::ASTC_8x5:
						rawFormat = "ASTC_8x5";
						bitsPerPixel = 3.20;
						blockWidth = 8;
						blockHeight = 5;
						break;
					case EG1TASTCFormat::ASTC_8x6:
						rawFormat = "ASTC_8x6";
						bitsPerPixel = 2.67;
						blockWidth = 8;
						blockHeight = 6;
						break;
					case EG1TASTCFormat::ASTC_8x8:
						rawFormat = "ASTC_8x8";
						bitsPerPixel = 2.00;
						blockWidth = 8;
						blockHeight = 8;
						break;
					case EG1TASTCFormat::ASTC_10x5:
						rawFormat = "ASTC_10x5";
						bitsPerPixel = 2.56;
						blockWidth = 10;
						blockHeight = 5;
						break;
					case EG1TASTCFormat::ASTC_10x6:
						rawFormat = "ASTC_10x6";
						bitsPerPixel = 2.13;
						blockWidth = 10;
						blockHeight = 6;
						break;
					case EG1TASTCFormat::ASTC_10x8:
						rawFormat = "ASTC_10x8";
						bitsPerPixel = 1.60;
						blockWidth = 10;
						blockHeight = 8;
						break;
					case EG1TASTCFormat::ASTC_10x10:
						rawFormat = "ASTC_10x10";
						bitsPerPixel = 1.28;
						blockWidth = 10;
						blockHeight = 10;
						break;
					case EG1TASTCFormat::ASTC_12x10:
						rawFormat = "ASTC_12x10";
						bitsPerPixel = 1.07;
						blockWidth = 12;
						blockHeight = 10;
						break;
					case EG1TASTCFormat::ASTC_12x12:
						rawFormat = "ASTC_12x12";
						bitsPerPixel = 0.89;
						blockWidth = 12;
						blockHeight = 12;
						break;
					default:
						{
							PopUpMessage(L"Uknown ASTC Texture on %d of %d!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.", i + 1, META.KT_ASTC_FORMAT);
							assert(0 && "Unknown ASTC texture format.");
						}
						break;
					}
				}
				else
				{
					PopUpMessage(L"ASTC Texture on texture %d missing format type!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.", i + 1);
					assert(0 && "ASTC ex header format missing.");
				}
				break;
			default:
				PopUpMessage(L"Uknown Texture format of #%d on texture %d!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.", G1T_TEX_HEADER.PIXEL_FORMAT, i);
				assert(0 && "Unknown texture format.");
				break;
			}

			///////////////////////////////
			//    COMPUTE IMAGE SIZE     //
			///////////////////////////////

			// some Switch stuff isn't swizzled or GOBed
			if ( META.bUseSwitchSize == true)
			{
				firstMipSize = SwitchTextureSizeWithMips(
					WIDTH,
					HEIGHT,
					1 /*DEPTH*/,
					1 /*MIPS*/,
					1 /*PLANE_COUNT*/,
					blockWidth,
					blockHeight,
					minBytes,
					bitsPerPixel,
					META.bCompressedFormat,
					G1T_TEX_HEADER.KTGL_TEXTURE_TYPE,
					1,
					1
				).layerSize;

				totalTexBufferLen = SwitchTextureSizeWithMips(
					WIDTH,
					HEIGHT,
					DEPTH,
					MIPS,
					PLANE_COUNT,
					blockWidth,
					blockHeight,
					minBytes,
					bitsPerPixel,
					META.bCompressedFormat,
					G1T_TEX_HEADER.KTGL_TEXTURE_TYPE,
					PLANE_COUNT,
					DEPTH
				).totalSize;
			}
			else if (META.bUsePS5Size == true)
			{
				// unkown how the PS5 creates padding for its mip sheets with KT systems
				// skipping the sizing just pulling the full size between the headers

				firstMipSize = TextureSizeWithMips(
					WIDTH,
					HEIGHT,
					1 /*DEPTH*/,
					1 /*MIPS*/,
					1 /*PLANE_COUNT*/,
					1 /*FACES*/,
					minBytes,
					blockWidth,
					blockHeight,
					META.bCompressedFormat,
					bitsPerPixel,
					META.pAlignment
				);

				if (G1T_HEADER.TEX_COUNT == 1)
				{
					totalTexBufferLen = bufferLen - offset;
				}
				else
				{
					if (i < (int)(G1T_HEADER.TEX_COUNT - 1))
					{
						totalTexBufferLen = (G1T_HEADER.TEX_OFFSET + offsetList[i + 1]) - offset;
					}
					else
					{
						totalTexBufferLen = bufferLen - offset;
					}
				}
			}
			else
			{
				firstMipSize = TextureSizeWithMips(
					WIDTH,
					HEIGHT,
					1 /*DEPTH*/,
					1 /*MIPS*/,
					1 /*PLANE_COUNT*/,
					1 /*FACES*/,
					minBytes,
					blockWidth,
					blockHeight,
					META.bCompressedFormat,
					bitsPerPixel,
					META.pAlignment
				);

				totalTexBufferLen = TextureSizeWithMips(
					WIDTH,
					HEIGHT,
					DEPTH,
					MIPS,
					PLANE_COUNT,
					FACES,
					minBytes,
					blockWidth,
					blockHeight,
					META.bCompressedFormat,
					bitsPerPixel,
					META.pAlignment
				);
			}

			///////////////////////////////
			//     ZLIB DECOMPRESS       //
			///////////////////////////////

			// for zlib compressed data
			BYTE* unzippedData = nullptr;

			if (META.bZlibCompressed)
			{
				// if compressed, parse new headers
				S_G1T_COMPED_HEADER<bBigEndian> G1T_COMPED_HEADER = S_G1T_COMPED_HEADER<bBigEndian>(buffer, offset, rapi, bufferLen);
				// Panic check on header read
				if (G1T_COMPED_HEADER.bPanic)
				{
					PopUpMessage(L"G1T file compressed header read error!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
					assert(0 && "G1T_COMPED_HEADER Size Issue!");
				}

				uint32_t UNCOMPRESSED_SIZE = (G1T_COMPED_HEADER.PAGE_COUNT * G1T_COMPED_HEADER.COMPED_WINDOW_SIZE) + (G1T_COMPED_HEADER.COMPED_UNCOMPED_CHUNK_SIZE);

				std::vector<S_G1T_COMPED_LEVELS<bBigEndian>> G1T_COMPED_LEVELS;

				for (uint32_t j = 0; j < G1T_COMPED_HEADER.STREAMED_LEVELS * DEPTH * FACES * PLANE_COUNT; j++)
				{
					G1T_COMPED_LEVELS.push_back(S_G1T_COMPED_LEVELS<bBigEndian>(buffer, offset, bufferLen));
					// Panic check on header read
					if (G1T_COMPED_LEVELS[j].bPanic)
					{
						PopUpMessage(L"G1T file header read error on S_G1T_COMPED_LEVELS data!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
						assert(0 && "S_G1T_COMPED_LEVELS Size Issue!");
					}
				}

				std::vector<S_G1T_COMPED_RESIDENT<bBigEndian>> G1T_COMPED_RESIDENT;

				for (uint32_t j = 0; j < G1T_COMPED_HEADER.RESIDENT_LEVELS * DEPTH * FACES * PLANE_COUNT; j++)
				{
					G1T_COMPED_RESIDENT.push_back(S_G1T_COMPED_RESIDENT<bBigEndian>(buffer, offset, bufferLen));
					// Panic check on header read
					if (G1T_COMPED_RESIDENT[j].bPanic)
					{
						PopUpMessage(L"G1T file header read error on S_G1T_COMPED_RESIDENT data!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
						assert(0 && "S_G1T_COMPED_RESIDENT Size Issue!");
					}
				}

				uint32_t COMPRESSED_SIZE = 0;

				std::vector <S_G1T_COMPED_OFFSETS<bBigEndian>> G1T_COMPED_OFFSETS;

				for (uint32_t j = 0; j < G1T_COMPED_HEADER.PAGE_COUNT; j++)
				{
					G1T_COMPED_OFFSETS.push_back(S_G1T_COMPED_OFFSETS<bBigEndian>(buffer, offset, bufferLen));
					// Panic check on header read
					if (G1T_COMPED_OFFSETS[j].bPanic)
					{
						PopUpMessage(L"G1T file header read error on compressed offsets!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
						assert(0 && "G1T_COMPED_OFFSETS Size Issue!");
					}
					COMPRESSED_SIZE += G1T_COMPED_OFFSETS[j].SIZE;
				}

				std::vector <S_G1T_COMPED_OFFSETS<bBigEndian>> G1T_UNCOMPED_OFFSETS;

				if(G1T_COMPED_HEADER.COMPED_HAS_UNCOMPED_CHUNK)
				{
					G1T_UNCOMPED_OFFSETS.push_back(S_G1T_COMPED_OFFSETS<bBigEndian>(buffer, offset, bufferLen));
					// Panic check on header read
					if (G1T_UNCOMPED_OFFSETS[0].bPanic)
					{
						PopUpMessage(L"G1T file header read error on uncompressed offsets!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
						assert(0 && "G1T_UNCOMPED_OFFSETS Size Issue!");
					}
					COMPRESSED_SIZE += G1T_UNCOMPED_OFFSETS[0].SIZE;
				}

				if (UNCOMPRESSED_SIZE != totalTexBufferLen)
				{
					LogDebug("Image size of %d but expecting %d.\n", (uint32_t)UNCOMPRESSED_SIZE, (uint32_t)totalTexBufferLen);
				}

				if (offset + COMPRESSED_SIZE < (uint32_t)bufferLen)
				{
					LogDebug("Data size of %d but only %d left.\n", (uint32_t)COMPRESSED_SIZE, (uint32_t)(bufferLen - offset));
					assert(0 && "COMPRESSED_SIZE Issue!");
				}

				// uncompress code

				unzippedData = (BYTE*)rapi->Noesis_UnpooledAlloc(UNCOMPRESSED_SIZE);

				if (!unzippedData)
				{
					PopUpMessage(L"ERROR!! unzippedData alloc\n");
					return;
				}

				uint32_t unzippedOffset = 0;

				for (uint32_t j = 0; j < G1T_COMPED_HEADER.PAGE_COUNT; j++)
				{
					uint32_t SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(SIZE);

					int retCode = rapi->Decomp_Inflate(buffer + offset, unzippedData + unzippedOffset, SIZE, G1T_COMPED_HEADER.COMPED_WINDOW_SIZE);

					if (retCode < 0)
					{
						assert(0 && "Decomp_Inflate Issue!");
					}

					offset += SIZE;

					unzippedOffset += G1T_COMPED_HEADER.COMPED_WINDOW_SIZE;
				}

				if (G1T_COMPED_HEADER.COMPED_HAS_UNCOMPED_CHUNK)
				{
					// only 1 chunk
					memcpy(unzippedData + unzippedOffset, buffer + offset, G1T_UNCOMPED_OFFSETS[0].SIZE);

					offset += G1T_UNCOMPED_OFFSETS[0].SIZE;

					unzippedOffset += G1T_UNCOMPED_OFFSETS[0].SIZE;
				}
			}
			else
			{

				///////////////////////////////
				//     ADJUST IMAGE SIZE     //
				///////////////////////////////

				// Sanity check on texture data size
				if (G1T_HEADER.TEX_COUNT == 1)
				{
					// If there is only one texture, check if there is extra data and meet it
					if (offset + totalTexBufferLen < (uint32_t)bufferLen)
					{
						LogDebug("Data size of %d but expecting %d.\n", (uint32_t)totalTexBufferLen, (uint32_t)(bufferLen - offset));
						totalTexBufferLen = bufferLen - offset;
					}
					else if (offset + totalTexBufferLen > (uint32_t)bufferLen)
					{
						LogDebug("Data size of %d but only %d left.\n", (uint32_t)totalTexBufferLen, (uint32_t)(bufferLen - offset));
						totalTexBufferLen = bufferLen - offset;
					}
				}
				else if (i < (int)(G1T_HEADER.TEX_COUNT - 1))
				{
					uint32_t expected_offset = G1T_HEADER.TEX_OFFSET + offsetList[i + 1];
					uint32_t dif = expected_offset - (offset + totalTexBufferLen);
					// Check if the size matchs the next texture.
					// if not, adjust based on if over or under.
					if (expected_offset != offset + totalTexBufferLen)
					{
						// If next expected offset is ahead, meet it. But check for max file size.
						if (expected_offset > offset + totalTexBufferLen)
						{
							if (offset + totalTexBufferLen + dif <= (uint32_t)bufferLen)
							{
								LogDebug("Data size on texture %d was %d but less than the expected %d.\n", i + 1, (uint32_t)totalTexBufferLen, (uint32_t)(expected_offset - offset));
								totalTexBufferLen += dif;
							}
							else
							{
								LogDebug("Data size on texture %d was more than remaining file size of %d.\n", i + 1, (uint32_t)(offset + totalTexBufferLen));
							}
						}
						else
						{
							// If expected offset is under, warn and continue. But check for max file size.
							if (!META.bHasAlphaAtlas && offsetList[i + 1] != 0)
							{
								LogDebug("Data size on texture %d was more than expected at %d instead of %d.\n", i + 1, (uint32_t)totalTexBufferLen, (uint32_t)(expected_offset - offset));
								totalTexBufferLen = expected_offset - offset;
							}
						}
					}
				}
				else
				{
					// If this is the last texture, check if there is extra data and meet it
					if (offset + totalTexBufferLen > (uint32_t)bufferLen)
					{
						LogDebug("Data size on texture %d has extra size of %d instead of %d.\n", i + 1, (uint32_t)totalTexBufferLen, (int)(bufferLen - offset));
						totalTexBufferLen = bufferLen - offset;
					}
				}

				// these formats have incorrect offsets in the skip table so we must fix
				if (META.bHasAlphaAtlas || ((i < (int)(G1T_HEADER.TEX_COUNT - 1) && offsetList[i + 1] == 0)))
				{
					if (i < (G1T_HEADER.TEX_COUNT - 1))
					{
						offsetList[i + 1] = offsetList[i] + G1T_TEX_HEADER.headerSize + (uint32_t)totalTexBufferLen;
					}
				}
			}

			///////////////////////////////
			//       DEBUG LOG DATA      //
			///////////////////////////////

			// for when we only need one texture for a load model
			if (selectedTex != -1)
			{
				if (selectedTex != i)
				{
					continue;
				}
			}

			if (bDebugLog)
			{
				LogDebug("G1T_TEXTURE #%d @ %d\n", i + 1, offset);
				// Log any metadata
				if ( G1T_TEX_ATTR_HEADER.TYPE != TEX_EX_TYPE::ENVMAP )
				{
					LogDebug("\tTEX_EX_TYPE:\t\t%d (%s)\n", G1T_TEX_ATTR_HEADER.TYPE, EX_TYPE_STR[clamp_index((int)G1T_TEX_ATTR_HEADER.TYPE, sizeof(EX_TYPE_STR))]);
				}

				// Log main entry
				LogDebug("\tKTGL_TEXTURE_TYPE:\t%d (%s)\n", G1T_TEX_HEADER.KTGL_TEXTURE_TYPE, LOAD_TYPE_STR[clamp_index((int)G1T_TEX_HEADER.KTGL_TEXTURE_TYPE, sizeof(LOAD_TYPE_STR))]);
				LogDebug("\tMIP_COUNT:\t\t%d\n", G1T_TEX_HEADER.MIP_COUNT);
				LogDebug("\tKTGL_PIXEL_FORMAT:\t0x%02X\n", G1T_TEX_HEADER.PIXEL_FORMAT);
				LogDebug("\tSYS_TEX_FORMAT:\t\t%s\n", getFormatStr(G1T_HEADER.SYSTEM, (uint8_t)G1T_TEX_HEADER.PIXEL_FORMAT));
				LogDebug("\tPACKED_WIDTH:\t\t%d (%d)\n", G1T_TEX_HEADER.PACKED_WIDTH, WIDTH);
				LogDebug("\tPACKED_HEIGHT:\t\t%d (%d)\n", G1T_TEX_HEADER.PACKED_HEIGHT, HEIGHT);
				LogDebug("\tPACKED_DEPTH:\t\t%d (%d)\n", G1T_TEX_HEADER.PACKED_DEPTH, DEPTH);
				LogDebug("\tREAD_G1T_HEADER_EX:\t%d\n", G1T_TEX_HEADER.READ_G1T_HEADER_EX);
				LogDebug("\tHAS_TEX_EX_HEADER:\t%d\n", G1T_TEX_HEADER.HAS_TEX_EX_HEADER);
				if (G1T_TEX_HEADER.HAS_TEX_EX_HEADER > 0)
				{
					LogDebug("\tTEX_HEADER_EX_SIZE:\t%d\n", G1T_TEX_HEADER.TEX_HEADER_EX_SIZE);
					LogDebug("\tZ_SCALE:\t\t\t%.4f\n", G1T_TEX_HEADER.Z_SCALE);
					LogDebug("\tEX_FACES:\t\t%d (%d)\n", G1T_TEX_HEADER.EX_FACES, FACES);
					LogDebug("\tEX_ARRAY_COUNT:\t%d (%d)\n", G1T_TEX_HEADER.EX_ARRAY_COUNT, PLANE_COUNT);
					LogDebug("\tEX_SWIZZLE:\t\t%d (%s)\n", G1T_TEX_HEADER.EX_SWIZZLE, EX_SWIZZLE_TYPE_STR[clamp_index((int)G1T_TEX_HEADER.EX_SWIZZLE, sizeof(EX_SWIZZLE_TYPE_STR))]);
					LogDebug("\tKTGL_GD_COLOR_SPACE:\t%d (%s)\n", G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE, COLOR_SPACE_STR[clamp_index((int)G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE, sizeof(COLOR_SPACE_STR))]);
					LogDebug("\tEX_WIDTH:\t\t%d\n", G1T_TEX_HEADER.EX_WIDTH);
					LogDebug("\tEX_HEIGHT:\t\t%d\n", G1T_TEX_HEADER.EX_HEIGHT);
				}
				if (G1T_TEX_HEADER.READ_G1T_HEADER_EX)
				{
					for (size_t q = 0; q < read_ex_headers.size(); q++) 
					{
						S_G1T_HEADER_EX ex_header = ExList[read_ex_headers[q]];
						LogDebug("\t*TEX_HEADER_EX_TYPE %d:\t%d (%s)", ex_header.ID, ex_header.TYPE, EX_TYPE_STR[clamp_index((int)ex_header.TYPE, sizeof(EX_TYPE_STR))]);
						switch (ex_header.TYPE)
						{
						case TEX_EX_TYPE::ASTC_TYPE:
							LogDebug("\n\t*KT_ASTC_FORMAT:\t%d (%s)\n", ex_header.KT_ASTC_FORMAT, ASTC_FORMAT_STR[clamp_index((int)ex_header.KT_ASTC_FORMAT, sizeof(ASTC_FORMAT_STR))]);
							break;
						case TEX_EX_TYPE::PS4_PLANE_ARRAY:
							LogDebug("\n\t*ARRAY_DEPTH:\t\t%d\n", ex_header.ARRAY_DEPTH);
							break;
						default:
							LogDebug("\n");
							break;
						}
					}
				}

				LogDebug("\t\t-TEXTURE %d META-\n", i + 1);
				LogDebug("\tDimensions:\t\t%d x %d (d%d x p%d) mips %d\n", WIDTH, HEIGHT, DEPTH, PLANE_COUNT, MIPS);
				if (META.bCompressedFormat)
				{
					LogDebug("\tbCompressedFormat:\t%s\n", META.bCompressedFormat ? "true" : "false");
					if (fourccFormat != -1)       LogDebug("\tfourccFormat:\t\t%c%c%c%c\n", fourccFormat & 0xFF, (fourccFormat >> 8) & 0xFF, (fourccFormat >> 16) & 0xFF, (fourccFormat >> 24) & 0xFF);
					if (META.bIsETC1)             LogDebug("\tbIsETC1:\t\t\t%s\n", META.bIsETC1 ? "true" : "false");
					if (META.bPICAETC)            LogDebug("\tbPICAETC:\t\t%s\n", META.bPICAETC ? "true" : "false");
					if (META.bIsETC2)             LogDebug("\tbIsETC2:\t\t\t%s\n", META.bIsETC2 ? "true" : "false");
					if (META.bPalette4)           LogDebug("\tbPalette4:\t\t%s\n", META.bPalette4 ? "true" : "false");
					if (META.bPalette8)           LogDebug("\tbPalette8:\t\t%s\n", META.bPalette8 ? "true" : "false");
					if (META.bPalette14)          LogDebug("\tbPalette14:\t\t%s\n", META.bPalette14 ? "true" : "false");
					if (META.bIsPVRTC)            LogDebug("\tbIsPVRTC:\t\t%s\n", META.bIsPVRTC ? "true" : "false");
					if (META.bIsASTC)             LogDebug("\tbIsASTC:\t\t\t%s\n", META.bIsASTC ? "true" : "false");
					if (META.bHasAlphaAtlas)      LogDebug("\tbHasAlphaAtlas:\t\t%s\n", META.bHasAlphaAtlas ? "true" : "false");
					if (META.b3DSAlpha)           LogDebug("\tb3DSAlpha:\t\t%s\n", META.b3DSAlpha ? "true" : "false");
					LogDebug("\tblockDim:\t\t\t%dx%d\n", blockWidth, blockHeight);
					LogDebug("\tblockByteSize:\t\t%d\n", minBytes);
				}
				if (rawFormat != "")			  LogDebug("\trawFormat:\t\t%s\n", rawFormat.c_str());
				LogDebug("\tbitsPerPixel:\t\t%.2f\n", bitsPerPixel);
				LogDebug("\tminBytes:\t\t\t%d\n", minBytes);
				LogDebug("\tfirstMipSize:\t\t%d\n", firstMipSize);
				LogDebug("\ttotalTexBufferLen:\t\t%d\n", totalTexBufferLen);
				if (META.bIsUNorm)                LogDebug("\tbIsUNorm:\t\t%s\n", META.bIsUNorm ? "true" : "false");
				if (META.bNormalMap)              LogDebug("\tbNormalMap:\t\t%s\n", META.bNormalMap ? "true" : "false");
				if (META.bNormalized)             LogDebug("\tbNormalize:\t\t%s\n", META.bNormalized ? "true" : "false");
				if (META.bSwizzled)               LogDebug("\tbSwizzled:\t\t%s\n", META.bSwizzled ? "true" : "false");
				if (META.bBigEndianShortSwap)     LogDebug("\tbBigEndianShortSwap:\t%s\n", META.bBigEndianShortSwap ? "true" : "false");
				if (META.bBigEndianLongSwap)      LogDebug("\tbBigEndianLongSwap:\t%s\n", META.bBigEndianLongSwap ? "true" : "false");
				if (META.bSigned)                 LogDebug("\tbSigned:\t\t\t%s\n", META.bSigned ? "true" : "false");
				if (META.bChannelFlip)            LogDebug("\tbChannelFlip:\t\t%s\n", META.bChannelFlip ? "true" : "false");
				if (META.bFloat)                  LogDebug("\tbFloat:\t\t\t%s\n", META.bFloat ? "true" : "false");
				if (META.bHalfFloat)              LogDebug("\tbHalfFloat:\t\t%s\n", META.bHalfFloat ? "true" : "false");
				if (META.bIsDepth)                LogDebug("\tbIsDepth:\t\t%s\n", META.bIsDepth ? "true" : "false");
				if (META.bD16Convert)             LogDebug("\tbD16Convert:\t\t%s\n", META.bD16Convert ? "true" : "false");
				if (META.bD32Convert)             LogDebug("\tbD32Convert:\t\t%s\n", META.bD32Convert ? "true" : "false");
				if (META.bD32FloatConvert)        LogDebug("\tbD32FloatConvert:\t\t%s\n", META.bD32FloatConvert ? "true" : "false");
				if (META.bZlibCompressed)		  LogDebug("\tbZlibCompressed:\t\t%s\n", META.bZlibCompressed ? "true" : "false");
				if (META.bD24_8Convert)           LogDebug("\tbD24_8Convert:\t\t%s\n", META.bD24_8Convert ? "true" : "false");
				if (META.bConvert10BitFloat)      LogDebug("\tbConvert10BitFloat:\t%s\n", META.bConvert10BitFloat ? "true" : "false");
				if (META.bConvert11Bit10BitFloat) LogDebug("\tbConvert11Bit10BitFloat:\t%s\n", META.bConvert11Bit10BitFloat ? "true" : "false");
			}

			///////////////////////////////
			//        UNTILE DATA        //
			///////////////////////////////

			// We untile, decomp then post convert
			// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData

			BYTE* texStart = buffer + offset; if (META.bZlibCompressed) texStart = unzippedData;
			// for sanity checks between stages
			BYTE* passOffPointer = nullptr;
			uint32_t currentImageSize = firstMipSize;
			// Being that there can be more than one texture in a buffer now,
			// we process by planes and depths
			uint32_t planesToProcess = bLayers ? PLANE_COUNT : 1; if (selectedTex != -1) planesToProcess = 1;
			// Being that there can be more than one texture in a buffer now,
			// we process by planes and depths
			uint32_t depthsToProcess = bLayers ? DEPTH : 1; if (selectedTex != -1) depthsToProcess = 1;
			// for reseting between planes / depths
			float bitsPerPixelMaster = bitsPerPixel;
			// for reseting between planes / depths
			uint32_t minBytesMaster = minBytes;
			// for reseting between planes / depths
			std::string rawFormatMaster = rawFormat;

			// Endian swap if needed
			if (META.bBigEndianShortSwap)
			{	//Swap endian for x360 textures
				if (G1T_HEADER.SYSTEM == PLATFORM::XB2)
				{
					// gets flipped on decomp
					// so we dont do it here
					if (!META.bSwizzled)
					{
						short_swap_byte_order(texStart, currentImageSize);
					}
				} 
				else
				{
					short_swap_byte_order(texStart, currentImageSize);
				}
			}

			for (uint32_t p = 0; p < planesToProcess; p++)
			{
				for (uint32_t d = 0; d < depthsToProcess; d++)
				{
					BYTE* untiledTexData = nullptr;
					bool bUntiledTexShouldFree = false;
					passOffPointer = nullptr;

					// Untile/Unswizzle the data if relevant
					if (META.bSwizzled)
					{
						switch (G1T_HEADER.SYSTEM)
						{
						case PLATFORM::DX9: // never found
							// if not used, passes the pointer on to the next section
							// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
							passOffPointer = texStart;
							break;
						case PLATFORM::PS3: // Didnt find any sizzled data to test
							//untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bUntiledTexShouldFree = true;
							//if (META.bCompressedFormat)
							//{
							//	rapi->Noesis_UntileImageDXTEx(untiledTexData, texStart, currentImageSize, WIDTH, HEIGHT, minBytes, currentImageSize, 0, 0);
							//}
							//else
							//{
							//	rapi->Noesis_UntileImageRAWEx(untiledTexData, texStart, currentImageSize, WIDTH, HEIGHT, minBytes, currentImageSize, 0, 0);
							//}
							// if not used, passes the pointer on to the next section
							// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
							passOffPointer = texStart;
							break;
						case PLATFORM::XB2: // working
						{
							untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bUntiledTexShouldFree = true;
							if (!untiledTexData)
							{
								PopUpMessage(L"ERROR!! untiledTexData alloc\n");
								return;
							}
							unswizzle_x360(untiledTexData, texStart, currentImageSize, WIDTH, HEIGHT, blockWidth, minBytes);
							passOffPointer = untiledTexData;
							break;
						}
						case PLATFORM::RVL: // Working
						{
							tplFormats_e wTextFormat = getNWiiFormat((uint8_t)G1T_TEX_HEADER.PIXEL_FORMAT);
							BYTE* PALETTE = nullptr;
							tplPaletteFormats_e PALETTE_TYPE = tplPaletteFormats_e::NONE;
							// needs palette file
							// may move prompt to other file if needed again
							if ( wTextFormat == tplFormats_e::C14X2 ||
								 wTextFormat == tplFormats_e::C8 ||
								 wTextFormat == tplFormats_e::C4
								)
							{
								PopUpMessage(L"Texure %d needs a palette file, please select a G1TL file with the palette.", i+1);
								char des[256] = { "Select G1TL palette file." };
								char ext[256] = { ".g1tl" };
								int outLen = 0;
								char outPath[MAX_NOESIS_PATH] = {};
								BYTE* g1tlData = rapi->Noesis_LoadPairedFile(des, ext, outLen, outPath);
								bool clearDummy = false;
								while (!g1tlData)
								{
									if (outLen == 0)
									{
										PopUpMessage(L"Error loading file, dummy palette will be used.");
										PALETTE = (BYTE*)rapi->Noesis_UnpooledAlloc(32768); clearDummy = true;
										if (!PALETTE)
										{
											PopUpMessage(L"ERROR!! PALETTE alloc\n");
											return;
										}
										if (META.bPalette14)
										{
											PALETTE_TYPE = tplPaletteFormats_e::PRGB565;
										}
										else if (META.bPalette8)
										{
											PALETTE_TYPE = tplPaletteFormats_e::PRGB5A3;
										}
										else if (META.bPalette4)
										{
											PALETTE_TYPE = tplPaletteFormats_e::PIA8;
										}
										g1tlData = (BYTE*)rapi->Noesis_UnpooledAlloc(1);
										if (!g1tlData)
										{
											PopUpMessage(L"ERROR!! g1tlData alloc\n");
											return;
										}
									}
									else
									{
										PopUpMessage(L"Error loading file, try again.");
										outLen = 0;
										g1tlData = rapi->Noesis_LoadPairedFile(des, ext, outLen, outPath);
									}
								}
								if (!PALETTE)
								{
									uint32_t dummy = 0;
									S_G1TL_HEADER<bBigEndian> g1tlFile = S_G1TL_HEADER<bBigEndian>(g1tlData, dummy, outLen);
									while (g1tlFile.bPanic)
									{
										PopUpMessage(L"The G1TL file didn't pass the type check. \nThe file may have errors, be a malformed or in an unsupported format. \n Please select a different file.");
										rapi->Noesis_UnpooledFree(g1tlData);
										outLen = 0;
										g1tlData = rapi->Noesis_LoadPairedFile(des, ext, outLen, outPath);
										while (!g1tlData)
										{
											if (outLen == 0)
											{
												PopUpMessage(L"Error loading file, dummy palette will be used.");
												g1tlFile.bPanic = false;
												PALETTE = (BYTE*)rapi->Noesis_UnpooledAlloc(32768); clearDummy = true;
												if (!PALETTE)
												{
													PopUpMessage(L"ERROR!! PALETTE alloc\n");
													return;
												}
												PALETTE_TYPE = tplPaletteFormats_e::PIA8;
												g1tlData = (BYTE*)rapi->Noesis_UnpooledAlloc(1);
												if (!PALETTE)
												{
													PopUpMessage(L"ERROR!! g1tlData alloc\n");
													return;
												}
											}
											else
											{
												PopUpMessage(L"Error loading file, try again.");
												outLen = 0;
												g1tlData = rapi->Noesis_LoadPairedFile(des, ext, outLen, outPath);
											}
										}
										if (!PALETTE)
										{
											outLen = 0;
											g1tlFile = S_G1TL_HEADER<bBigEndian>(g1tlData, dummy, outLen);
										}
									}
									if (!PALETTE)
									{
										int paletteIndex = 0;
										if (g1tlFile.ENTRY_COUNT != 1)
										{
											sprintf_s(des, 256, "G1TL file has more than one palette.");
											sprintf_s(ext, 256, "G1TL file has %d palettes. \nWhich palette would you like to use?", g1tlFile.ENTRY_COUNT);
											paletteIndex = PromptBetweenNumbers(des, ext, "1", 1, g1tlFile.ENTRY_COUNT, rapi);
											paletteIndex--;
										}
										PALETTE = g1tlFile.ENTRIES[paletteIndex].PALETTE;
										PALETTE_TYPE = g1tlFile.ENTRIES[paletteIndex].WiiPALETTE_TYPE;
									}
								}
							}
							// Will also decomp
							untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * 4); bUntiledTexShouldFree = true;
							if (!untiledTexData)
							{
								PopUpMessage(L"ERROR!! untiledTexData alloc\n");
								return;
							}
							tplDecodeImage(untiledTexData, texStart, WIDTH, HEIGHT, wTextFormat, PALETTE_TYPE, PALETTE);
							META.bCompressedFormat = false;
							currentImageSize = (uint32_t)(WIDTH * HEIGHT * 4);
							rawFormat = "r8g8b8a8";
							bitsPerPixel = 0x20;
							minBytes = 4;
							passOffPointer = untiledTexData;
							break;
						}	
						case PLATFORM::PSP:  // never found
							// if not used, passes the pointer on to the next section
							// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
							passOffPointer = texStart;
							break;
						case PLATFORM::CTR: // didnt find sizzle
							// if not used, passes the pointer on to the next section
							// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
							passOffPointer = texStart;
							break;
						case PLATFORM::NGP: // working
						{
							untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bUntiledTexShouldFree = true;
							if (!untiledTexData)
							{
								PopUpMessage(L"ERROR!! untiledTexData alloc\n");
								return;
							}
							if (fourccFormat == -1)
							{
								// working on raw 24bpp images 32x32 tile
								DeswizzlePSVitaRaw(texStart, untiledTexData, WIDTH, HEIGHT, bitsPerPixel);
							}
							else
							{
								convert_morton_psvita_dreamcast(texStart, untiledTexData, WIDTH, HEIGHT, (int)bitsPerPixel, blockWidth, blockHeight, minBytes);
							}
							passOffPointer = untiledTexData;
							break;
						}
						case PLATFORM::AND: // No known swizzles
						case PLATFORM::IOS: // No known swizzles
							// if not used, passes the pointer on to the next section
							// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
							passOffPointer = texStart;
							break;
						case PLATFORM::CAFE: // working
						{
							int texDimType = 1; // Texture2D aka dimensions
							switch (G1T_TEX_HEADER.KTGL_TEXTURE_TYPE)
							{
							case S_GT1_LOAD_TYPE::PLANAR:
								texDimType = 1; // Texture2D
								break;
							case S_GT1_LOAD_TYPE::CUBE:
								texDimType = 3; // TextureCube
								break;
							case S_GT1_LOAD_TYPE::VOLUME:
								texDimType = 2; // Texture3D
								break;
							default:
								break;
							}
							int WiiU_Swizzle = 0; // should pretty much always be 0
							if (G1T_HEADER.HEADER_EX_SIZE != 0)
							{
								WiiU_Swizzle = META.WiiU_SWIZZLE;
							}

							// starting here 
							// https://github.com/KillzXGaming/Switch-Toolbox/blob/16de585c7b998826e39ac88915f64554c0025423/File_Format_Library/FileFormats/HyruleWarriors/G1T/G1TFile.cs#L249
							GX2Surface surf = { 0 };
							surf.bpp = (uint32_t)bitsPerPixel;
							surf.height = HEIGHT;
							surf.width = WIDTH;
							surf.aa = 0;
							surf.alignment = 0;
							surf.depth = PLANE_COUNT; // DEPTH;
							surf.dim = texDimType;
							surf.format = getNWiiUFormatValue((uint8_t)G1T_TEX_HEADER.PIXEL_FORMAT, (uint8_t)G1T_TEX_HEADER.KTGL_GD_COLOR_SPACE);
							surf.use = 2; // USE_COLOR_BUFFER
							surf.pitch = 0;
							surf.data = buffer + offset; // this function gets the array textures so we always start at the start of the tex buffer
							if (META.bZlibCompressed) surf.data = unzippedData;
							surf.numMips = MIPS;
							surf.mipOffset.resize(MIPS);
							surf.mipData = buffer + offset; // this function gets the array textures so we always start at the start of the tex buffer
							if (META.bZlibCompressed) surf.mipData = unzippedData;
							surf.tileMode = 0x04; // MODE_2D_TILED_THIN1
							surf.swizzle = WiiU_Swizzle;
							surf.imageSize = totalTexBufferLen;
							surf.numArray = PLANE_COUNT;
							untiledTexData = Gx2Decode(surf, p, 0, rapi); bUntiledTexShouldFree = true;
							passOffPointer = untiledTexData;
							break;
						}
						case PLATFORM::DX11: // none here but the sizzle can be on this system enum
						case PLATFORM::DX12: // only 64kb swizzle at the moment
						{
							if (G1T_TEX_HEADER.EX_SWIZZLE != EX_SWIZZLE_TYPE::NONE)
							{
								switch (G1T_TEX_HEADER.EX_SWIZZLE)
								{
								case EX_SWIZZLE_TYPE::DX12_64kb:        // DeswizzleD3D12_64KB
								case EX_SWIZZLE_TYPE::ZLIB_COMPRESSED:  // Also DeswizzleD3D12_64KB
								{
									if (currentImageSize > 65536) // texture data less than 64kb does not swizzle
									{
										if (META.bCompressedFormat)
										{
											// working on DXT, unsure about other formats
											untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bUntiledTexShouldFree = true;
											if (!untiledTexData)
											{
												PopUpMessage(L"ERROR!! untiledTexData alloc\n");
												return;
											}
											DeswizzleD3D12_64KBDXT(texStart, untiledTexData, (uint32_t)((WIDTH + blockWidth - 1) / blockWidth), (uint32_t)((HEIGHT + blockHeight - 1) / blockHeight), minBytes);
											passOffPointer = untiledTexData;
										}
										else
										{
											// worked on 32 bpp but could use more samples
											untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bUntiledTexShouldFree = true;
											if (!untiledTexData)
											{
												PopUpMessage(L"ERROR!! untiledTexData alloc\n");
												return;
											}
											DeswizzleD3D12_64KBRaw(texStart, untiledTexData, WIDTH, HEIGHT, bitsPerPixel);
											passOffPointer = untiledTexData;
										}
									}
									else
									{
										// if not used, passes the start pointer on to the next section
										// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
										passOffPointer = texStart;
									}
									break;
								}
								default:
									// if not used, passes the start pointer on to the next section
									// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
									passOffPointer = texStart;
									break;
								}
							}
							else
							{
								// if not used, passes the pointer on to the next section
								// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
								passOffPointer = texStart;
							}
							break;
						}
						case PLATFORM::PS4: // Working
						{
							untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bUntiledTexShouldFree = true;
							if (!untiledTexData)
							{
								PopUpMessage(L"ERROR!! untiledTexData alloc\n");
								return;
							}
							if (fourccFormat == -1)
							{
								// working
								DeswizzlePS4Raw(texStart, untiledTexData, WIDTH, HEIGHT, bitsPerPixel);
								//NoesisMisc_Untile1dThin(untiledTexData, (int)currentImageSize, texStart, (int)currentImageSize, WIDTH, HEIGHT, (int)bitsPerPixel, 0, rapi);
							}
							else
							{
								int widthInBlocks = (WIDTH + (blockWidth - 1)) / blockWidth;
								int	heightInBlocks = (HEIGHT + (blockHeight - 1)) / blockHeight;
								DeswizzlePS4Raw(texStart, untiledTexData, widthInBlocks, heightInBlocks, minBytes * 8);
								//NoesisMisc_Untile1dThin(untiledTexData, (int)currentImageSize, texStart, (int)currentImageSize, WIDTH, HEIGHT, minBytes * 8, 1, rapi);
							}
							passOffPointer = untiledTexData;
							break;
						}
						case PLATFORM::NX: // working
						{
							int nOffset = SwitchTextureSizeWithMips(
								WIDTH,
								HEIGHT,
								DEPTH,
								MIPS,
								PLANE_COUNT,
								blockWidth,
								blockHeight,
								minBytes,
								bitsPerPixel,
								META.bCompressedFormat,
								G1T_TEX_HEADER.KTGL_TEXTURE_TYPE,
								p,
								d
							).offset;
							untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(totalTexBufferLen); bUntiledTexShouldFree = true;
							if (!untiledTexData)
							{
								PopUpMessage(L"ERROR!! untiledTexData alloc\n");
								return;
							}
							int widthInBlocks = (WIDTH + (blockWidth - 1)) / blockWidth;
							int	heightInBlocks = (HEIGHT + (blockHeight - 1)) / blockHeight;
							uint32_t blockHeightEnum = block_height_enum(heightInBlocks);
							BYTE* startLoc = buffer + offset + nOffset;
							if (META.bZlibCompressed) startLoc = unzippedData + nOffset;
							rapi->Image_UntileBlockLinearGOBs(untiledTexData, totalTexBufferLen, startLoc, totalTexBufferLen, widthInBlocks, heightInBlocks, blockHeightEnum, minBytes);
							passOffPointer = untiledTexData;
							break;
						}
						case PLATFORM::PS5: // working (with some issues but only known swizzle)
						{
							// to get offset for this to work right, 
							// we need to take the buffer offset it from the end because of mip at start
							// hacky but no known way of finding the PS5 image buffer size 
							// when swizzle and tile type are missing (maybe hardcoded)
							BYTE* mip0Offset = texStart;
							int planeSize = totalTexBufferLen / PLANE_COUNT;
							int depthSize = planeSize / DEPTH;
							int mipOff = 0;
							if (MIPS != 1) // only one mip mean image starts at start
							{
								if (PLANE_COUNT == 1 && DEPTH == 1)
								{
									mipOff = totalTexBufferLen - currentImageSize;
								}
								else
								{
									if (d != 0)
									{
										mipOff += ((depthSize * d) + depthSize) - currentImageSize;
									}
									if (p != 0)
									{
										mipOff += ((planeSize * p) + planeSize) - currentImageSize;
									}
									if (mipOff == 0)
									{
										mipOff = depthSize - currentImageSize;
										if (mipOff == 0)
										{
											mipOff = totalTexBufferLen - currentImageSize;
										}
									}
								}
								mip0Offset = buffer + offset; if (META.bZlibCompressed) mip0Offset = unzippedData;
								mip0Offset += mipOff;
							}
							else
							{
								firstMipSize = depthSize;
							}
							untiledTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bUntiledTexShouldFree = true;
							if (!untiledTexData)
							{
								PopUpMessage(L"ERROR!! untiledTexData alloc\n");
								return;
							}

							// There are tiles happening on top of the swizzle for some of the smaller images and non square images (god help us with non-power 2).
							// The only way to make a proper deswizzle and untile code would be to reverse the Agc Gpu address logic and default tile allocation as there wasn't any header data for what swizzle was used within any samples found
							// maybe if more interest happens within the system this untile function will work with more textures
							
							//if (META.bCompressedFormat && (currentImageSize == 65536 || currentImageSize == 16384))
							//{
							//	int widthInBlocks = (WIDTH + (blockWidth - 1)) / blockWidth;
							//	int	heightInBlocks = (HEIGHT + (blockHeight - 1)) / blockHeight;
							//	DeswizzlePS5DXT(mip0Offset, untiledTexData, widthInBlocks, heightInBlocks, /minBytes /* 8);
							//}
							//else
							//{
								UnswizzlePS5(mip0Offset, untiledTexData, currentImageSize, WIDTH, HEIGHT, blockWidth, blockHeight, bitsPerPixel, minBytes, META.bCompressedFormat, rapi);
							//}
							
							passOffPointer = untiledTexData;
							break;
						}
						default:
							// if not used, passes the pointer on to the next section
							// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
							passOffPointer = texStart;
							break;
						}
					}
					else
					{
						// if not used, passes the pointer on to the next section
						// untiledTexData -> decompTexData -> postConvertTexData -> finalTexData
						passOffPointer = texStart;
					}

					if (!passOffPointer)
					{
						PopUpMessage(L"Pointer pass off missed!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
						assert(0 && "passOffPointer from swizzle null!");
					}

					///////////////////////////////
					//    DECOMPRESS TEXTURES    //
					///////////////////////////////

					BYTE* decompTexData = nullptr;
					bool bDecompTexShouldFree = false;

					if (META.bCompressedFormat)
					{
						// Decompress PVRTC
						// Mode		Block	Bits		Bytes 
						//			Size	per Pixel	Per Block
						// ----------------------------------
						// 4bpp		4x4		4			8
						// 2bpp		8x4		2			8
						if (META.bIsPVRTC)
						{
							decompTexData = rapi->Image_DecodePVRTC(passOffPointer, (int)currentImageSize, WIDTH, HEIGHT, (int)bitsPerPixel); bDecompTexShouldFree = true;
							if (!decompTexData)
							{
								PopUpMessage(L"ERROR!! decompTexData alloc\n");
								return;
							}
							rawFormat = "r8g8b8a8";
							bitsPerPixel = 0x20;
							minBytes = 4;
							currentImageSize = (uint32_t)(WIDTH * HEIGHT * 4);
							passOffPointer = decompTexData;
						}
						// Decompress ASTC
						// Block	Bits		Bytes 
						// Size		per Pixel	Per Block
						// ------------------------------
						// 4x4		8			16
						// 5x4		6.4			16
						// 5x5		5.12		16
						// 6x5		4.27		16
						// 6x6 		3.56		16
						// 8x5		3.20		16
						// 8x6		2.67		16
						// 10x5		2.56		16
						// 10x6		2.13		16
						// 8x8		2			16
						// 10x8		1.60		16
						// 10x10	1.28		16
						// 12x10	1.07		16
						// 12x12	0.89		16
						else if (META.bIsASTC)
						{
							int pBlockDims[3] = { (int)blockWidth, (int)blockHeight, 1 };
							int pImageSize[3] = { (int)WIDTH, (int)HEIGHT, 1 };
							decompTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * 4); bDecompTexShouldFree = true;
							if (!decompTexData)
							{
								PopUpMessage(L"ERROR!! decompTexData alloc\n");
								return;
							}
							rapi->Image_DecodeASTC(decompTexData, passOffPointer, (unsigned int)currentImageSize, pBlockDims, pImageSize);
							rawFormat = "r8g8b8a8";
							bitsPerPixel = 0x20;
							minBytes = 4;
							currentImageSize = (uint32_t)(WIDTH * HEIGHT * 4);
							passOffPointer = decompTexData;
						}
						// Decompress ETC - 4x4
						// Format 		Block		 Bits		Notes
						//				Size (bytes) per Pixel	
						// ------------------------------------------
						// ETC1			4			 8			RGB only, no alpha channel support.
						// ETC2 RGB		4			 8			RGB, higher quality compared to ETC1.
						// ETC2 RGBA	8			 16			RGBA with full alpha channel.
						// ETC2 RGBA1	4			 8			RGBA with 1 - bit "punchthrough" alpha(fully opaque or fully transparent).
						// EAC R11		4			 8			Single - channel(Red or grayscale) data, 11 - bits per channel with 0.5 bytes per channel.
						// EAC RG11		8			 16			Two - channel(Red and Green) data, 11 - bits per channel, 1 byte per pixel total.
						else if (META.bIsETC1 || META.bIsETC2)
						{
							if (META.bPICAETC)
							{
								decompTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * 16); bDecompTexShouldFree = true;
								if (!decompTexData)
								{
									PopUpMessage(L"ERROR!! decompTexData alloc\n");
									return;
								}
								rapi->Image_DecodePICA200ETC(decompTexData, passOffPointer, WIDTH, HEIGHT, META.b3DSAlpha, 0, 0);
								flip_vertically(decompTexData, WIDTH, HEIGHT, 4);
								passOffPointer = decompTexData;
							}
							else
							{
								decompTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * 4); bDecompTexShouldFree = true;
								if (!decompTexData)
								{
									PopUpMessage(L"ERROR!! decompTexData alloc\n");
									return;
								}
								rapi->Image_DecodeETC(decompTexData, passOffPointer, (int)currentImageSize, WIDTH, HEIGHT, &*(const char*)rawFormat.c_str());
								passOffPointer = decompTexData;
							}
							rawFormat = "r8g8b8a8";
							bitsPerPixel = 0x20;
							minBytes = 4;
							currentImageSize = (uint32_t)(WIDTH * HEIGHT * 4);
						}
						// Decompress DXT - 4x4
						// Format 		Block		 Bits		Notes
						//				Size (bytes) per Pixel			
						// ------------------------------------------			 
						// BC1(DXT1)	8			 4			RGB, optional 1 - bit alpha.Good for color maps and normal maps.
						// BC2(DXT3)	16			 8			RGB + 4 - bit explicit alpha. Generally less used.
						// BC3(DXT5)	16			 8			RGBA, common for textures with full alpha.
						// BC4			8			 4			Single - channel grayscale. Height maps, gloss maps, etc.
						// BC5			16			 8			Two - channel, can be used for normal maps.
						// BC6H			16			 8			RGB, High Dynamic Range(HDR) textures.
						// BC7			16			 8			RGB or RGBA, high - quality compression.
						else if (fourccFormat != -1)
						{
							if (META.bNormalized)
							{
								decompTexData = rapi->Noesis_ConvertDXT(WIDTH, HEIGHT, passOffPointer, fourccFormat); bDecompTexShouldFree = true;
								if (!decompTexData)
								{
									PopUpMessage(L"ERROR!! decompTexData alloc\n");
									return;
								}
								passOffPointer = decompTexData;
							}
							else
							{
								convertDxtExParams_t* params = (convertDxtExParams_t*)rapi->Noesis_UnpooledAlloc(sizeof(convertDxtExParams_t));
								if (!params)
								{
									PopUpMessage(L"ERROR!! params alloc\n");
									return;
								}
								params->ati2ZScale = G1T_TEX_HEADER.Z_SCALE;
								params->ati2NoNormalize = true;
								params->decodeAsSigned = META.bSigned;
								params->resvBB = false;
								params->resvBC = false;
								memset(params->resv, 0, 15 * sizeof(TResvInt));
								decompTexData = rapi->Noesis_ConvertDXTEx(WIDTH, HEIGHT, passOffPointer, fourccFormat, currentImageSize, params, 0); bDecompTexShouldFree = true;
								if (!decompTexData)
								{
									PopUpMessage(L"ERROR!! decompTexData alloc\n");
									return;
								}
								rapi->Noesis_UnpooledFree(params);
								passOffPointer = decompTexData;
							}
							rawFormat = "r8g8b8a8";
							bitsPerPixel = 0x20;
							minBytes = 4;
							currentImageSize = (uint32_t)(WIDTH * HEIGHT * 4);
						}
						// Only found on Wii and handled with swizzle code
						//else if (META.bPalette4)
						//{
						//	bitsPerPixel = 0x20;
						//	decompTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8));
						//	ConvertP4_to_RGBA32(passOffPointer, PALETTE, decompTexData, WIDTH, HEIGHT);
						//	rawFormat = "r8g8b8a8";
						//	currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						//  passOffPointer = decompTexData;
						//}
						//else if (META.bPalette8)
						//{
						//	bitsPerPixel = 0x20;
						//	decompTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8));
						//  if (!decompTexData)
						//  {
						//	PopUpMessage(L"ERROR!! decompTexData alloc\n");
						//	return;
						//	}
						//	ConvertP8_to_RGBA32(passOffPointer, PALETTE, decompTexData, WIDTH, HEIGHT);
						//	rawFormat = "r8g8b8a8";
						//	currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						//  passOffPointer = decompTexData;
						//}
						//else if (META.bPalette14)
						//{
						//	bitsPerPixel = 0x20;
						//	decompTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8));
						//  if (!decompTexData)
						//  {
						//	PopUpMessage(L"ERROR!! decompTexData alloc\n");
						//	return;
						//	}
						//	// need to find where the tlut is
						//	ConvertP14_to_RGBA32(passOffPointer, TLUT, decompTexData, WIDTH, HEIGHT);
						//	rawFormat = "r8g8b8a8";
						//	currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						//  passOffPointer = decompTexData;
						//}
					}

					if (!passOffPointer)
					{
						PopUpMessage(L"Pointer pass off missed!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
						assert(0 && "passOffPointer from decomp null!");
					}

					///////////////////////////////
					//       POST CONVERTS       //
					///////////////////////////////

					BYTE* postConvertTexData = nullptr;
					bool bPostConvertTexShouldFree = false;

					// Alpha stored under image
					if (META.bHasAlphaAtlas)
					{
						// data should be r8g8b8a8 by now
						HEIGHT /= 2;
						if (!META.bSkipAlphaAtlas)
						{
							uint32_t alphaOffset = WIDTH * HEIGHT * 4;
							for (uint32_t j = 0; j < WIDTH * HEIGHT; j++)
							{
								passOffPointer[4 * j + 3] = passOffPointer[alphaOffset + 4 * j];
							}
						}
						bitsPerPixel = 0x20;
						minBytes = 4;
						rawFormat = "r8g8b8a8";
						currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
					}

					if (META.bChannelFlip)
					{
						// we leave alpha where it is on dxt files
						if (G1T_HEADER.SYSTEM == PLATFORM::XB2)
						{
							FlipChannelOrder(&rawFormat[0], fourccFormat == -1 ? true : false);
						}
						else
						{
							FlipChannelOrder(&rawFormat[0], true );
						}
					}

					if (META.bBigEndianLongSwap)
					{
						long_swap_byte_order(passOffPointer, currentImageSize);
					}

					// Depth & Stecil convert
					if (META.bD24_8Convert)
					{
						bitsPerPixel = 0x50;
						postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8)); bPostConvertTexShouldFree = true;
						if (!postConvertTexData)
						{
							PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
							return;
						}
						ConvertDepth24Stencil8_to_24_24_24_8(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
						rawFormat = "r24g24b24a8";
						minBytes = 10;
						currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						passOffPointer = postConvertTexData;
					}
					else if (META.bD16Convert)
					{
						bitsPerPixel = 0x30;
						postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8)); bPostConvertTexShouldFree = true;
						if (!postConvertTexData)
						{
							PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
							return;
						}
						ConvertDepth16_to_16_16_16(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
						rawFormat = "r16g16b16";
						minBytes = 6;
						currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						passOffPointer = postConvertTexData;
					}
					else if (META.bD32Convert)
					{
						bitsPerPixel = 0x60;
						postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8)); bPostConvertTexShouldFree = true;
						if (!postConvertTexData)
						{
							PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
							return;
						}
						ConvertDepth32_to_32_32_32(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
						rawFormat = "r#F32g#F32b#F32";
						currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						passOffPointer = postConvertTexData;
					}
					// weird HDR bit float converts
					else if (META.bConvert11Bit10BitFloat)
					{
						bitsPerPixel = 0x60;
						postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8)); bPostConvertTexShouldFree = true;
						if (!postConvertTexData)
						{
							PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
							return;
						}
						ConvertR11FG11FB10F_to_RGB32F(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
						rawFormat = "r#F32g#F32b#F32";
						minBytes = 12;
						currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						passOffPointer = postConvertTexData;
					}
					else if (META.bConvert10BitFloat)
					{
						bitsPerPixel = 0x80;
						postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8)); bPostConvertTexShouldFree = true;
						if (!postConvertTexData)
						{
							PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
							return;
						}
						ConvertU2F10F10F10_to_RGBA32F(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
						rawFormat = "r#F32g#F32b#F32a#F32";
						minBytes = 16;
						currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						passOffPointer = postConvertTexData;
					}
					else if (META.bD32FloatConvert)
					{
						bitsPerPixel = 0x80;
						postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(WIDTH * HEIGHT * (int)(bitsPerPixel / 8)); bPostConvertTexShouldFree = true;
						if (!postConvertTexData)
						{
							PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
							return;
						}
						ConvertDepth32Stencil8_to_RGBA32f(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
						rawFormat = "r#F32g#F32b#F32a#F32";
						minBytes = 16;
						currentImageSize = (uint32_t)((WIDTH * HEIGHT * bitsPerPixel) / 8);
						passOffPointer = postConvertTexData;
					}
					// signed to unsigned
					else if (META.bSigned)
					{
						if (rawFormat == "r16")
						{
							postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bPostConvertTexShouldFree = true;
							if (!postConvertTexData)
							{
								PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
								return;
							}
							ConvertR16I_to_R16(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
							passOffPointer = postConvertTexData;
						}
						else if (rawFormat == "r32")
						{
							postConvertTexData = (BYTE*)rapi->Noesis_UnpooledAlloc(currentImageSize); bPostConvertTexShouldFree = true;
							if (!postConvertTexData)
							{
								PopUpMessage(L"ERROR!! postConvertTexData alloc\n");
								return;
							}
							ConvertR32I_to_R32UI(passOffPointer, postConvertTexData, WIDTH, HEIGHT);
							passOffPointer = postConvertTexData;
						}
					}

					if (!passOffPointer)
					{
						PopUpMessage(L"Pointer pass off missed!\nRestart app with 'Enable debug log' on in 'Tools/Project G1M' menu for info.");
						assert(0 && "passOffPointer from convert null!");
					}

					///////////////////////////////
					//         HAND OFF          //
					///////////////////////////////

					std::string inFile = rapi->Noesis_GetLastCheckedName();
					std::filesystem::path pathObj(inFile);
					std::string fileName = pathObj.stem().string();

					// Decode the data
					BYTE* finalTexData = rapi->Noesis_ImageDecodeRaw(passOffPointer, (int)currentImageSize, WIDTH, HEIGHT, &rawFormat[0]);
					if (!finalTexData)
					{
						PopUpMessage(L"ERROR!! finalTexData alloc\n");
						return;
					}

					// Flip Vertically
					if (bFlipVertically)
					{
						flip_vertically(finalTexData, WIDTH, HEIGHT, (int)(bitsPerPixel / 8));
					}

					// Flip Horizontally
					if (bFlipHorizontally)
					{
						flip_horizontally(finalTexData, WIDTH, HEIGHT, (int)(bitsPerPixel / 8));
					}

					//Create the texture
					char texName[250];
					if (bLayers && (planesToProcess * depthsToProcess) > 1)
					{
						fileName.append("out%d_%d_%d.dds");
						const char* fileNameC = fileName.c_str();
						snprintf(texName, 250, fileNameC, i, d + 1, p +1);
					}
					else
					{
						fileName.append("out%d.dds");
						const char* fileNameC = fileName.c_str();
						snprintf(texName, 250, fileNameC, noeTex.Num());
					}
					noesisTex_t* texture = rapi->Noesis_TextureAlloc(texName, WIDTH, HEIGHT, finalTexData, NOESISTEX_RGBA32);
					texture->shouldFreeData = true;
					texture->globalIdx = noeTex.Num();
					noeTex.Append(texture);

					///////////////////////////////
					//      FREE POINTERS        //
					///////////////////////////////

					if (bPostConvertTexShouldFree)
					{
						// now check if it was used
						if (postConvertTexData)
						{
							rapi->Noesis_UnpooledFree(postConvertTexData);
							postConvertTexData = nullptr;
						}
						bPostConvertTexShouldFree = false;
					}
					if (bDecompTexShouldFree)
					{
						// now check if it was used
						if (decompTexData)
						{
							rapi->Noesis_UnpooledFree(decompTexData);
							decompTexData = nullptr;
						}
						bDecompTexShouldFree = false;
					}
					if (bUntiledTexShouldFree)
					{
						// now check if it was used
						if (untiledTexData)
						{
							rapi->Noesis_UnpooledFree(untiledTexData);
							untiledTexData = nullptr;
						}
						bUntiledTexShouldFree = false;
					}

					///////////////////////////////
					//        RESET LOOP         //
					///////////////////////////////

					texStart += firstMipSize;
					currentImageSize = firstMipSize;
					bitsPerPixel = bitsPerPixelMaster;
					minBytes = minBytesMaster;
					rawFormat = rawFormatMaster;
				}
			}

			if (META.bZlibCompressed)
			{
				rapi->Noesis_UnpooledFree(unzippedData);
				unzippedData = nullptr;
			}
		}
	}
};


#endif // !G1T_H