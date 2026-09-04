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

// ---------------------------------------------------------------------------
// These eleven are exactly the chunks ktgl::CModelData::CreateModelData handles;
// the list was read off the chunk switch on two architectures of the same build
// and they agree. Everything else, including the whole G1MH..G1MR run, falls to
// the skip path and is stepped over by its own size.
//
// Two things worth knowing before adding to this list:
//   * NUNR (0x4E554E52) turns up among the constants in some builds but has NO
//     case body. It is a binary search pivot the compiler emitted between NUNO
//     and NUNS. It is not a chunk. Do not add it on that evidence.
//   * The magic to reader binding is still not formally proved. The engine has
//     exactly four readers with the right signature, named ReadClothInfoSection,
//     ReadVerletClothInfoSection, ReadSkinnedClothInfoSection and
//     ReadSoftBodyInfoSection, for exactly these four chunks, but each case
//     calls through a stub that cannot be resolved in the builds on hand.
//     SOFT to SoftBody is as safe as an unproved pairing gets.
//
// Version ranges the engine accepts, for reference:
//   G1M  0033..0037,  G1MF 0020..0027 (its size indexes an 8 entry table)
// ---------------------------------------------------------------------------
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

// Not parsed by this plugin, listed so a file starting with one is recognised
// rather than dismissed as corrupt.
//
// G1H is the shape (morph target) container: a header, a table of ABSOLUTE part
// offsets, then one G1HP part per entry. It is where facial animation blend
// shapes live, and a G1A object with opcode 401 or a G2A with motion type SHAPE
// is what drives their weights.
//
// IT ALSO TURNS UP BUNDLED AHEAD OF A G1M IN A FILE NAMED .g1m. In that case the
// file begins with G1H_ rather than G1M_, and the G1H header's fileSize field at
// +8 is the offset where the G1M actually starts. A loader that only checks the
// first four bytes will reject such a file even though the model inside it is
// perfectly ordinary. See samples/G1H.bt.
#define G1H_MAGIC   0x4731485F
#define G1HP_MAGIC  0x47314850

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