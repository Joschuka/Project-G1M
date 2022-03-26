#pragma once

#ifndef NUNS_
#define NUNS_

#define NUNS1_MAGIC 0x00060001


struct NunsInfluence
{
	int P1;
	int P2;
	int P3;
	int P4;
	float P5;
	float P6;
	float P7;
	float P8;
};

template<bool bBigEndian>
struct NUNS1
{
	uint32_t parentID;
	uint32_t entrySize;
	std::vector<RichVec4> controlPoints;
	std::vector<NunsInfluence> influences;
	NUNS1(BYTE* buffer, uint32_t startOffset, uint32_t version)
	{
		uint32_t offset = startOffset;
		parentID = *reinterpret_cast<uint32_t*>(buffer + offset);
		uint32_t controlPointCount = *reinterpret_cast<uint32_t*>(buffer + offset + 4);

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(parentID);
			LITTLE_BIG_SWAP(controlPointCount);
		}

		offset += 8;
		offset += 0xB8;

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
		memcpy(influences.data(), buffer + offset, controlPointCount * sizeof(NunsInfluence));
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

		offset += controlPointCount * sizeof(NunsInfluence);
		int previous = -1;
		while (previous != 0x424C5730)
		{
			previous = *reinterpret_cast<int*>(buffer + offset);
			offset += 4;
		}
		offset += 4;
		uint32_t blwoSize = *reinterpret_cast<uint32_t*>(buffer + offset);
		offset += 16 + blwoSize;
		entrySize = offset - startOffset;
	}
};

template<bool bBigEndian>
struct NUNS
{
	std::vector<NUNS1<bBigEndian>> Nuns1s;
	NUNS(BYTE* buffer, uint32_t startOffset)
	{
		uint32_t offset = startOffset;
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
			uint32_t checkpoint;
			switch (subHeader.magic)
			{
			case NUNS1_MAGIC:
				checkpoint = 0;
				for (auto i = 0; i < subHeader.entryCount; i++)
				{
					Nuns1s.push_back(std::move(NUNS1<bBigEndian>(buffer, offset + checkpoint, header.chunkVersion)));
					checkpoint += Nuns1s.back().entrySize;
				}
				offset += checkpoint;
				break;
			default:
				break;
			}
		}
	}
};


#endif // !NUNS_


