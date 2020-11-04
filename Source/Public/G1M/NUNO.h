#pragma once

#ifndef NUNO_
#define NUNO_

#define NUNO1_MAGIC 0x00030001
#define NUNO2_MAGIC 0x00030002
#define NUNO3_MAGIC 0x00030003
#define NUNO4_MAGIC 0x00030004

struct NunInfluence
{
	int P1;
	int P2;
	int P3;
	int P4;
	float P5;
	float P6;
};

template<bool bBigEndian>
struct NunHeader
{
	uint32_t magic;
	uint32_t chunkSize;
	uint32_t entryCount;
	NunHeader(NunHeader* ptr) : NunHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(magic);
			LITTLE_BIG_SWAP(chunkSize);
			LITTLE_BIG_SWAP(entryCount);
		}
	}
};

template<bool bBigEndian>
struct NUNO1
{
	uint32_t parentID;
	size_t entrySize;
	std::vector<RichVec4> controlPoints;
	std::vector<NunInfluence> influences;
	NUNO1(BYTE* buffer, size_t startOffset, uint32_t version)
	{
		size_t offset = startOffset;
		parentID = *reinterpret_cast<uint32_t*>(buffer + offset);
		size_t controlPointCount = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
		size_t unknownSectionCount = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
		uint32_t skip1 = *reinterpret_cast<uint32_t*>(buffer + offset + 12);
		uint32_t skip2 = *reinterpret_cast<uint32_t*>(buffer + offset + 16);
		uint32_t skip3 = *reinterpret_cast<uint32_t*>(buffer + offset + 20);

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(parentID);
			LITTLE_BIG_SWAP(controlPointCount);
			LITTLE_BIG_SWAP(unknownSectionCount);
			LITTLE_BIG_SWAP(skip1);
			LITTLE_BIG_SWAP(skip2);
			LITTLE_BIG_SWAP(skip3);
		}

		offset += 24;
		offset += 0x3C;
		if (version > 0x30303233)
			offset += 0x10;
		if (version >= 0x30303235)
			offset += 0x10;
		controlPoints.resize(controlPointCount);
		influences.resize(controlPointCount);

		//ControlPoints
		memcpy(controlPoints.data(), buffer + offset, controlPointCount * sizeof(RichVec4));
		if (bBigEndian)
		{
			for (auto& cp : controlPoints)
			{
				cp.ChangeEndian();
			}
		}
		offset += controlPointCount * sizeof(RichVec4);
		//Influences
		memcpy(influences.data(), buffer + offset, controlPointCount * sizeof(NunInfluence));
		if (bBigEndian)
		{
			for (auto& infl : influences)
			{
				LITTLE_BIG_SWAP(infl.P1);
				LITTLE_BIG_SWAP(infl.P2);
				LITTLE_BIG_SWAP(infl.P3);
				LITTLE_BIG_SWAP(infl.P4);
				LITTLE_BIG_SWAP(infl.P5);
				LITTLE_BIG_SWAP(infl.P6);
			}
		}

		offset += controlPointCount * sizeof(NunInfluence);
		offset += 48 * unknownSectionCount;
		offset += 4 * (skip1 + skip2 + skip3);
		entrySize = offset - startOffset;
	}
};

template<bool bBigEndian>
struct NUNO3
{
	uint32_t parentID;
	size_t entrySize;
	std::vector<RichVec4> controlPoints;
	std::vector<NunInfluence> influences;
	NUNO3(BYTE* buffer, size_t startOffset, uint32_t version)
	{
		size_t offset = startOffset;
		parentID = *reinterpret_cast<uint32_t*>(buffer + offset);
		size_t controlPointCount = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
		size_t unknownSectionCount = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
		uint32_t skip1 = *reinterpret_cast<uint32_t*>(buffer + offset + 12);
		uint32_t skip2 = *reinterpret_cast<uint32_t*>(buffer + offset + 20);
		uint32_t skip3 = *reinterpret_cast<uint32_t*>(buffer + offset + 24);
		uint32_t skip4 = *reinterpret_cast<uint32_t*>(buffer + offset + 28);

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(parentID);
			LITTLE_BIG_SWAP(controlPointCount);
			LITTLE_BIG_SWAP(unknownSectionCount);
			LITTLE_BIG_SWAP(skip1);
			LITTLE_BIG_SWAP(skip2);
			LITTLE_BIG_SWAP(skip3);
			LITTLE_BIG_SWAP(skip4);
		}

		offset += 32;
		if (version < 0x30303330)
		{
			offset += 0xA8;
			if (version >= 0x30303235)
				offset += 0x10;
		}

		else
		{
			offset += 8;
			uint32_t temp = *reinterpret_cast<uint32_t*>(buffer + offset);
			if (bBigEndian)
				LITTLE_BIG_SWAP(temp);
			offset += temp;
		}

		controlPoints.resize(controlPointCount);
		influences.resize(controlPointCount);

		//ControlPoints
		memcpy(controlPoints.data(), buffer + offset, controlPointCount * sizeof(RichVec4));
		if (bBigEndian)
		{
			for (auto& cp : controlPoints)
			{
				cp.ChangeEndian();
			}
		}
		offset += controlPointCount * sizeof(RichVec4);
		//Influences
		memcpy(influences.data(), buffer + offset, controlPointCount * sizeof(NunInfluence));
		if (bBigEndian)
		{
			for (auto& infl : influences)
			{
				LITTLE_BIG_SWAP(infl.P1);
				LITTLE_BIG_SWAP(infl.P2);
				LITTLE_BIG_SWAP(infl.P3);
				LITTLE_BIG_SWAP(infl.P4);
				LITTLE_BIG_SWAP(infl.P5);
				LITTLE_BIG_SWAP(infl.P6);
			}
		}

		offset += controlPointCount * sizeof(NunInfluence);
		offset += 48 * unknownSectionCount;
		offset += (4 * skip1 + 8 * skip2 + 12 * skip3 + 8 * skip4);
		entrySize = offset - startOffset;
	}
};

template<bool bBigEndian>
struct NUNO
{
	std::vector<NUNO1<bBigEndian>> Nuno1s;
	std::vector<NUNO3<bBigEndian>> Nuno3s;
	NUNO(BYTE* buffer, size_t startOffset)
	{
		size_t offset = startOffset;
		//Reading the header, we want the version
		GResourceHeader<bBigEndian> header = reinterpret_cast<GResourceHeader<bBigEndian>*>(buffer + startOffset);
		offset += sizeof(GResourceHeader<bBigEndian>);

		uint32_t sectionCount = *reinterpret_cast<uint32_t*> (buffer + offset);
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(sectionCount);
		}
		offset += 4;
		for (auto i = 0; i < sectionCount; i++)
		{
			NunHeader<bBigEndian> subHeader = reinterpret_cast<NunHeader<bBigEndian>*>(buffer + offset);
			offset += 12;
			size_t checkpoint;
			switch (subHeader.magic)
			{
			case NUNO1_MAGIC:
				checkpoint = 0;
				for (auto i = 0; i < subHeader.entryCount; i++)
				{
					Nuno1s.push_back(std::move(NUNO1<bBigEndian>(buffer,offset + checkpoint,header.chunkVersion)));
					checkpoint += Nuno1s.back().entrySize;
				}
				offset += checkpoint;
				break;
			case NUNO3_MAGIC:
				checkpoint = 0;
				for (auto i = 0; i < subHeader.entryCount; i++)
				{
					Nuno3s.push_back(std::move(NUNO3<bBigEndian>(buffer, offset + checkpoint, header.chunkVersion)));
					checkpoint += Nuno3s.back().entrySize;
				}
				offset += checkpoint;
				break;
			default:
				offset += subHeader.chunkSize - 12;
				break;
			}
		}
	}
};


#endif // !NUNO_


