#pragma once

#ifndef NUNV_
#define NUNV_

#define NUNV1_MAGIC 0x00050001

template<bool bBigEndian>
struct NUNV1
{
	uint32_t parentID;
	size_t entrySize;
	std::vector<RichVec4> controlPoints;
	std::vector<NunInfluence> influences;
	NUNV1(BYTE* buffer, size_t startOffset, uint32_t version)
	{
		size_t offset = startOffset;
		parentID = *reinterpret_cast<uint32_t*>(buffer + offset);
		size_t controlPointCount = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
		size_t unknownSectionCount = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
		uint32_t skip1 = *reinterpret_cast<uint32_t*>(buffer + offset + 12);

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(parentID);
			LITTLE_BIG_SWAP(controlPointCount);
			LITTLE_BIG_SWAP(unknownSectionCount);
			LITTLE_BIG_SWAP(skip1);
		}

		offset += 16;
		offset += 0x54;
		if (version >= 0x30303131)
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
		offset += 4 * skip1;
		entrySize = offset - startOffset;
	}
};

template<bool bBigEndian>
struct NUNV
{
	std::vector<NUNV1<bBigEndian>> Nunv1s;
	NUNV(BYTE* buffer, size_t startOffset)
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
			case NUNV1_MAGIC:
				checkpoint = 0;
				for (auto i = 0; i < subHeader.entryCount; i++)
				{
					Nunv1s.push_back(std::move(NUNV1<bBigEndian>(buffer, offset + checkpoint, header.chunkVersion)));
					checkpoint += Nunv1s.back().entrySize;
				}
				offset += checkpoint;
				break;
			default:
				break;
			}
		}
	}
};


#endif // !NUNV_


