#pragma once

//All credits goes to DarkstarSword for this one https://github.com/DarkStarSword/3d-fixes/blob/master/decode_doa6_soft.py .Also thanks to Raytwo for the binary template based on his research https://github.com/three-houses-research-team/010-binary-templates/blob/master/Model%20related%20formats/G1M/SOFT.bt
//The goal here is just to parse the physics nodes so the parsing is not exhaustive. Refer to the above resources for more details if needed.
#ifndef SOFT_
#define SOFT_

#define SOFT1_MAGIC 0x00080001
#define SOFT2_MAGIC 0x00080002

template<bool bBigEndian>
struct SoftNodeEntryHeader {
	uint32_t id;
	uint32_t nodeCount;
	uint32_t z2;
	uint32_t z3;
	uint32_t u4;
	uint32_t len2;
	uint32_t len3;
	uint32_t parentID;
	uint32_t u6;
	uint32_t u7;
	uint32_t z8;
	uint32_t o9;
	uint32_t len4;

	SoftNodeEntryHeader(SoftNodeEntryHeader* ptr) : SoftNodeEntryHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(id);
			LITTLE_BIG_SWAP(nodeCount);
			LITTLE_BIG_SWAP(z2);
			LITTLE_BIG_SWAP(z3);
			LITTLE_BIG_SWAP(u4);
			LITTLE_BIG_SWAP(len2);
			LITTLE_BIG_SWAP(len3);
			LITTLE_BIG_SWAP(parentID);
			LITTLE_BIG_SWAP(u6);
			LITTLE_BIG_SWAP(u7);
			LITTLE_BIG_SWAP(z8);
			LITTLE_BIG_SWAP(o9);
			LITTLE_BIG_SWAP(len4);
		}
	}
};

struct SoftNodeEntryUnk1 {
	float unk[24];
};

struct {
	uint32_t id;
	float data;//"Influence weight?"
} SoftNodeEntryNodeInfluence;

struct SoftNodeEntryNodeData {
	uint32_t unk1[3];
	float unk2[3];
};

template<bool bBigEndian>
struct Soft1EntryNode {
	uint32_t id;
	RichVec3 pos;
	RichAngles rot;
	uint32_t unk; // Always 0x43?
	uint8_t b[4];
	uint32_t influenceCount;
	//Some extra stuff after that but we don't care (see BT for more info)

	Soft1EntryNode(Soft1EntryNode* ptr) : Soft1EntryNode(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(id);
			pos->ChangeEndian();
			rot->ChangeEndian()
			LITTLE_BIG_SWAP(unk);
			LITTLE_BIG_SWAP(influenceCount);
		}
	}
};

template<bool bBigEndian>
struct SOFT1
{
	uint32_t parentID;
	size_t entrySize;
	std::vector<Soft1EntryNode<bBigEndian>> softNodes;
	SOFT1(BYTE* buffer, size_t startOffset, uint32_t version)
	{
		size_t offset = startOffset;
		SoftNodeEntryHeader<bBigEndian> entryHeader = *reinterpret_cast<SoftNodeEntryHeader<bBigEndian>*>(buffer + offset);
		offset += sizeof(SoftNodeEntryHeader<bBigEndian>) + sizeof(SoftNodeEntryUnk1);

		parentID = entryHeader.parentID;

		for (auto i = 0; i < entryHeader.nodeCount; i++)
		{
			Soft1EntryNode<bBigEndian> entryNode = *reinterpret_cast<Soft1EntryNode<bBigEndian>*>(buffer + offset);
			softNodes.push_back(entryNode);
			offset += sizeof(Soft1EntryNode<bBigEndian>) + (entryNode.influenceCount + 1)  * 8 + 0x18;
		}

		//Info needed for the plugin retrieved, now we just need to determine the entry size to jump to the next SOFT1 chunk if there's one
		offset += 4 * (entryHeader.u4 + entryHeader.nodeCount + entryHeader.u6 + 3 * entryHeader.len3 + 1);
		uint32_t len5 = *(uint32_t*)(buffer + offset);
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(len5);
		}
		offset += 4 * (len5 / 4 - 1); //-2 + reading the len5 field
		entrySize = offset - startOffset;
	}
};



template<bool bBigEndian>
struct SOFT
{
	std::vector<SOFT1<bBigEndian>> Soft1s;
	SOFT(BYTE* buffer, size_t startOffset)
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
			NunHeader<bBigEndian> subHeader = reinterpret_cast<NunHeader<bBigEndian>*>(buffer + offset); //Not a typo, exact same struct
			offset += 12;
			size_t checkpoint;
			switch (subHeader.magic)
			{
			case SOFT1_MAGIC:
				checkpoint = 0;
				for (auto i = 0; i < subHeader.entryCount; i++)
				{
					Soft1s.push_back(std::move(SOFT1<bBigEndian>(buffer, offset + checkpoint, header.chunkVersion)));
					checkpoint += Soft1s.back().entrySize;
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


#endif // !SOFT_