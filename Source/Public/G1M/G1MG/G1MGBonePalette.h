#pragma once
#ifndef G1M_G_BONEP
#define G1M_G_BONEP

struct G1MGJointPaletteEntry
{
	uint32_t G1MMIndex;
	uint32_t physicsIndex;
	uint32_t jointIndex;
};

template <bool bBigEndian>
struct G1MGJointPalette
{
	uint32_t entryCount;
	std::vector<G1MGJointPaletteEntry> entries;
	std::vector<uint32_t> jointMapBuilder;

	G1MGJointPalette(BYTE* buffer, uint32_t& offset, G1MS<bBigEndian>* internalSkel, G1MS<bBigEndian>* externalSkel, std::map<uint32_t, uint32_t>* globalToFinal)
	{
		entryCount = *reinterpret_cast<uint32_t*>(buffer + offset);
		offset += 4;
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(entryCount);
		}
		entries.resize(entryCount);
		memcpy(entries.data(), buffer + offset, entryCount * sizeof(G1MGJointPaletteEntry));
		offset += entryCount * sizeof(G1MGJointPaletteEntry);
		if (bBigEndian)
		{
			for (auto& entry : entries)
			{
				LITTLE_BIG_SWAP(entry.G1MMIndex);
				LITTLE_BIG_SWAP(entry.physicsIndex);
				LITTLE_BIG_SWAP(entry.jointIndex);
			}
		}
		if (internalSkel)
		{
			for (auto i = 0; i < entries.size(); i++)
			{
				G1MGJointPaletteEntry& entry = entries[i];
				if (externalSkel)
				{
					if (entry.jointIndex >> 31) //0x80000000 flag, means that internal needs to be used
					{
						uint32_t globalID = internalSkel->localIDToGlobalID[entry.jointIndex ^ 0x80000000];
						jointMapBuilder.push_back((*globalToFinal)[globalID]);
					}
					else
					{
						uint32_t globalID = externalSkel->localIDToGlobalID[entry.jointIndex];
						jointMapBuilder.push_back((*globalToFinal)[globalID]);
						uint32_t dump;
					}
				}
				else
				{
					uint32_t globalID = internalSkel->localIDToGlobalID[entry.jointIndex];
					jointMapBuilder.push_back((*globalToFinal)[globalID]);
				}
			}
		}
	}
};

#endif // !G1M_G_BONEP
