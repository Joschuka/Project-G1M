#pragma once

#ifndef NUNO_
#define NUNO_

#define NUNO1_MAGIC 0x00030001
#define NUNO2_MAGIC 0x00030002
#define NUNO3_MAGIC 0x00030003
#define NUNO4_MAGIC 0x00030004
#define NUNO5_MAGIC 0x00030005

#define NUNO5FLAG_SKIP0	(1<<0)
#define NUNO5FLAG_SKIP1	(1<<1)
#define NUNO5FLAG_SKIP2	(1<<2)

struct NunInfluence
{
	int P1;
	int P2;
	int P3;
	int P4;
	float P5;
	float P6;
	NunInfluence(){}
	NunInfluence(NunInfluence* ptr) : NunInfluence(*ptr)
	{
	}
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
	uint32_t entrySize;
	std::vector<RichVec4> controlPoints;
	std::vector<NunInfluence> influences;
	NUNO1(BYTE* buffer, uint32_t startOffset, uint32_t version)
	{
		uint32_t offset = startOffset;
		parentID = *reinterpret_cast<uint32_t*>(buffer + offset);
		uint32_t controlPointCount = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
		uint32_t unknownSectionCount = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
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
	uint32_t entrySize;
	uint32_t entryID;
	int parentSetID;
	std::vector<RichVec4> controlPoints;
	std::vector<NunInfluence> influences;
	NUNO3(BYTE* buffer, uint32_t startOffset, uint32_t version, bool bIsNuno5, std::map<uint32_t, uint32_t>* entryIDToNunoID)
	{
		uint32_t offset = startOffset;
		parentID = *reinterpret_cast<uint32_t*>(buffer + offset);
		parentSetID = -1;
		if (!bIsNuno5) //"Classic" NUNO3
		{
			uint32_t controlPointCount = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
			uint32_t unknownSectionCount = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
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
		}
		else //NUNO5
		{
			//See the BT for more info, I'll just take the necessary values here.
			uint32_t lodCount = *reinterpret_cast<uint32_t*>(buffer + offset + 8); //Not sure if this is the actual count or if the value is hardcoded
			entryID = *reinterpret_cast<uint16_t*>(buffer + offset + 20);
			if (*reinterpret_cast<uint16_t*>(buffer + offset + 22) & 0x7FF)
				parentSetID = (*entryIDToNunoID)[entryID];

			offset += 0x24;
			for (auto l = 0; l < lodCount; l++)
			{
				uint32_t skip10Size = 0;
				uint32_t skip10Count = 0;

				uint32_t controlPointCount = *reinterpret_cast<uint32_t*>(buffer + offset);
				uint32_t flags = *reinterpret_cast<uint32_t*>(buffer + offset + 4);
				uint32_t skip1 = *reinterpret_cast<uint32_t*>(buffer + offset + 8);
				uint32_t skip2 = *reinterpret_cast<uint32_t*>(buffer + offset + 12);
				uint32_t skip3 = *reinterpret_cast<uint32_t*>(buffer + offset + 16);
				uint32_t skip4 = *reinterpret_cast<uint32_t*>(buffer + offset + 20);
				uint32_t skip5 = *reinterpret_cast<uint32_t*>(buffer + offset + 24);
				uint32_t skip6 = *reinterpret_cast<uint32_t*>(buffer + offset + 28);
				uint32_t skip7 = *reinterpret_cast<uint32_t*>(buffer + offset + 32);
				uint32_t skip8 = *reinterpret_cast<uint32_t*>(buffer + offset + 36);
				uint32_t skip9 = *reinterpret_cast<uint32_t*>(buffer + offset + 40);
				uint32_t bUseSkip10 = *reinterpret_cast<uint32_t*>(buffer + offset + 44);
				if (bUseSkip10)
				{
					skip10Size = *reinterpret_cast<uint32_t*>(buffer + offset + 48);
					skip10Count = *reinterpret_cast<uint32_t*>(buffer + offset + 52);
				}

				if (bBigEndian)
				{
					LITTLE_BIG_SWAP(lodCount);
					LITTLE_BIG_SWAP(controlPointCount);
					LITTLE_BIG_SWAP(flags);
					LITTLE_BIG_SWAP(skip1);
					LITTLE_BIG_SWAP(skip2);
					LITTLE_BIG_SWAP(skip3);
					LITTLE_BIG_SWAP(skip4);
					LITTLE_BIG_SWAP(skip5);
					LITTLE_BIG_SWAP(skip6);
					LITTLE_BIG_SWAP(skip7);
					LITTLE_BIG_SWAP(skip8);
					LITTLE_BIG_SWAP(skip9);
					LITTLE_BIG_SWAP(skip10Count);
					LITTLE_BIG_SWAP(skip10Size);
				}
				offset += 0x30;
				if (bUseSkip10)
					offset += 8;
				uint32_t cpOffset = *reinterpret_cast<uint32_t*>(buffer + offset);
				if (bBigEndian)
					LITTLE_BIG_SWAP(cpOffset);
				offset += cpOffset;
				if (!l) //First LOD, we grab its info. Otherwise, just skip.
				{
					for (auto i = 0; i < controlPointCount; i++)
					{
						controlPoints.push_back(RichVec3((float*)(buffer + offset)).ToVec4());
						offset += 0x18;
						influences.push_back(reinterpret_cast<NunInfluence*>(buffer + offset)); //P5/P6 will get a wrong value but we don't use them anywhere anyways so that's fine
						offset += 0x14;
					}
					if (bBigEndian)
					{
						for (auto& cp : controlPoints)
						{
							cp.ChangeEndian();
						}
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
				}
				else
					offset += controlPointCount * 0x2C;				

				//Skipping the other parts, that define physics parameters and other info that we don't need
				if (flags & NUNO5FLAG_SKIP0)
					offset += 0x20 * controlPointCount;
				if (flags & NUNO5FLAG_SKIP1)
					offset += 0x18 * controlPointCount;
				offset += (skip1 * 4 + skip2 * 12 + skip3 * 16 + skip4 * 12 + skip5 * 8 + skip6 * 0x30 + skip7 * 0x48 + skip8*0x20);
				if (flags & NUNO5FLAG_SKIP2)
					offset += 0x4 * controlPointCount;
				for (auto u = 0; u < skip9; u++)
				{
					uint32_t tempCount = *reinterpret_cast<uint32_t*>(buffer + offset);
					offset += (tempCount * 4 + 0x10);
				}
				offset += skip10Count * skip10Size;
				
			}
		}
		entrySize = offset - startOffset;
	}
};

template<bool bBigEndian>
struct NUNO
{
	std::vector<NUNO1<bBigEndian>> Nuno1s;
	std::vector<NUNO3<bBigEndian>> Nuno3s;
	NUNO(BYTE* buffer, uint32_t startOffset)
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
					Nuno3s.push_back(std::move(NUNO3<bBigEndian>(buffer, offset + checkpoint, header.chunkVersion,false, nullptr)));
					checkpoint += Nuno3s.back().entrySize;
				}
				offset += checkpoint;
				break;
			case NUNO5_MAGIC:
			{
				if (header.chunkVersion >= 0x30303335)
					offset += 4;
				checkpoint = 0;
				bIsNUNO5Global = true; //Set the NUNO5 boolean to true for cloth processing. If I ever see NUNO3 and NUNO5 in the same model I'll change the logic.
				std::map<uint32_t, uint32_t> entryIDToNunoID;
				std::map<uint32_t, std::map<const float, int>> nunoIDToSubsetMap;
				for (auto i = 0; i < subHeader.entryCount; i++)
				{
					//This is not a mistake. Seems like these are registered to NUNO3, contrary to the externalIDs distinction between NUN01,NUNV and NUNO3.
					//So for now we'll consider these as NUNO3, with a different parsing logic
					Nuno3s.push_back(std::move(NUNO3<bBigEndian>(buffer, offset + checkpoint, header.chunkVersion, true, &entryIDToNunoID)));
					if (entryIDToNunoID.count(Nuno3s.back().entryID) ==0)
						entryIDToNunoID[Nuno3s.back().entryID] = i;
					checkpoint += Nuno3s.back().entrySize;
				}
				//We parsed all the chunks, time to check which ones have subsets and create them (is there any other way than manually matching the coords ?)
				//A previous attempt was made to use P1 and P2 but it breaks (see Queen from SoP for an example)
				for (auto j= 0; j<Nuno3s.size(); j++)
				{
					NUNO3<bBigEndian>& nun3 = Nuno3s[j];
					if (nun3.parentSetID >= 0 && nunoIDToSubsetMap.count(nun3.parentSetID) == 0) //This entry has a parent entry and its map isn't there yet
					{
						//Create the map
						std::map<const float, int> tempMap;
						NUNO3<bBigEndian>& parentNunoEntry = Nuno3s[nun3.parentSetID];
						for (i = 0; i < parentNunoEntry.controlPoints.size(); i++)
							tempMap[parentNunoEntry.controlPoints[i].v[0] + parentNunoEntry.controlPoints[i].v[1] + parentNunoEntry.controlPoints[i].v[2] + parentNunoEntry.controlPoints[i].v[0]] = i;
						nunoIDToSubsetMap[nun3.parentSetID] = tempMap;
					}

					if (nun3.parentSetID >= 0 && nunoIDToSubsetMap.count(nun3.parentSetID)) //This entry has a parent entry and its map has been created
					{
						auto& tempMap = nunoIDToSubsetMap[nun3.parentSetID];
						//Since we don't use the influences for the subsets to build drivers, we can safely override them to store the index info
						for (i = 0; i < nun3.controlPoints.size(); i++)
						{
							const RichVec4& cp = nun3.controlPoints[i];
							if (tempMap.count(cp.v[0] + cp.v[1] + cp.v[2] + cp.v[0]))
								nun3.influences[i].P1 = tempMap[cp.v[0] + cp.v[1] + cp.v[2] + cp.v[0]];
							else
								assert(0);
						}
					}
				}


				offset += checkpoint;
				break;
			}
			default:
				offset += subHeader.chunkSize - 12;
				break;
			}
		}
	}
};


#endif // !NUNO_


