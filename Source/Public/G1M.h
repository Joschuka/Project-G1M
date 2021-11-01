#pragma once

#ifndef G1M_
#define G1M_

#include <inttypes.h>
#include "G1M/G1MM.h"
#include "G1M/G1MS.h"
#include "G1M/G1MG.h"
#include "G1M/NUNO.h"
#include "G1M/NUNV.h"
#include "G1M/NUNS.h"
#include "G1M/SOFT.h"

#define G1M_MAGIC   0x47314D5F
#define G1MF_MAGIC  0x47314D46
#define G1MS_MAGIC  0x47314D53
#define G1MM_MAGIC  0x47314D4D
#define G1MG_MAGIC  0x47314D47
#define COLL_MAGIC  0x434F4C4C
#define NUNO_MAGIC  0x4E554E4F
#define NUNV_MAGIC  0x4E554E56
#define NUNS_MAGIC  0x4E554E53
#define EXTR_MAGIC  0x45585452
#define HAIR_MAGIC  0x48414952
#define SOFT_MAGIC  0x534F4654
#define G2A_MAGIC	0x4732415F
#define G1A_MAGIC	0x4731415F

struct buffer_t
{
	BYTE* address = nullptr;
	uint32_t stride = 12;
	uint32_t offset = 0;
	rpgeoDataType_e dataType = RPGEODATA_FLOAT;
};

struct indexBuffer_t
{
	BYTE* address = nullptr;
	uint32_t indexCount;
	rpgeoDataType_e dataType;
	rpgeoPrimType_e primType;
};

struct mesh_t
{
	buffer_t posBuffer;
	buffer_t normBuffer;
	buffer_t uvBuffer;
	buffer_t blendIndicesBuffer;
	buffer_t blendWeightsBuffer;
	indexBuffer_t indexBuffer;
	uint8_t jointPerVertex;
};

template<bool bBigEndian>
struct GResourceHeader
{
	uint32_t magic;
	uint32_t chunkVersion;
	uint32_t chunkSize;

	GResourceHeader(GResourceHeader* ptr) : GResourceHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(magic);
			LITTLE_BIG_SWAP(chunkVersion);
			LITTLE_BIG_SWAP(chunkSize);
		}
	}
};

template<bool bBigEndian>
struct G1MHeader
{
	uint32_t firstChunkOffset;
	uint32_t reserved1;
	uint32_t chunkCount;

	G1MHeader();

	G1MHeader(G1MHeader*ptr) : G1MHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(firstChunkOffset);
			LITTLE_BIG_SWAP(reserved1);
			LITTLE_BIG_SWAP(chunkCount);
		}
	}
};



#endif //G1M_