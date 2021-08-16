#pragma once

#ifndef OBJD_H
#define OBJD_H

template<bool bBigEndian>
struct OBJDHeader
{
	char magic[4];
	uint16_t sectionCount;
	uint16_t section0EntryCount;
	uint16_t section1EntryCount;
	uint16_t section2EntryCount;
	uint16_t section3EntryCount;
	uint16_t padding;

	OBJDHeader(OBJDHeader* ptr) : OBJDHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(sectionCount);
			LITTLE_BIG_SWAP(section0EntryCount);
			LITTLE_BIG_SWAP(section1EntryCount);
			LITTLE_BIG_SWAP(section2EntryCount);
			LITTLE_BIG_SWAP(section3EntryCount);
			LITTLE_BIG_SWAP(padding);
		}
	}
};

template<bool bBigEndian>
struct OBJD
{
	std::map<uint16_t, std::pair<uint8_t,uint16_t>> section1IDToBundleG1MID;
	std::vector<std::pair<uint16_t, modelMatrix_t>> entityMatrices;
	OBJD(BYTE* buffer, int bufferLen)
	{
		//header
		uint32_t offset = 0;
		OBJDHeader<bBigEndian> header = reinterpret_cast<OBJDHeader<bBigEndian>*>(buffer+offset);
		offset += sizeof(OBJDHeader<bBigEndian>);
		//We skip section 0, 16 bytes header + entry Count*0x40
		offset += (0x10 + header.section0EntryCount*0x40);
		//Section1
		offset += 0x10;
		for (auto i = 0; i < header.section1EntryCount; i++)
		{
			offset += 0x1D0;
			uint16_t g1mID = *(uint16_t*)(buffer + offset);
			offset += 0x37;
			uint8_t bundleID = *(uint8_t*)(buffer + offset);
			offset += 0x1F9;
			section1IDToBundleG1MID[i] = std::make_pair(bundleID, g1mID);
		}
		//Section2
		offset += 0x10;
		//entityMatrices.resize(header.section2EntryCount);
		for (auto i = 0; i < header.section2EntryCount; i++)
		{
			//Coords
			RichVec3 scale = RichVec3((float*)(buffer+offset));
			offset += 0x10;
			RichQuat rotation = RichAngles((float*)(buffer + offset),false).ToQuat();
			offset += 0x10;
			RichVec3 position = RichVec3((float*)(buffer + offset));
			offset += 0x10;
			//Rest of chunk
			offset += 0x4;
			uint16_t section1ID = *(uint16_t*)(buffer + offset);
			offset += 0xC;

			//Create transform matrix
			float scaleMatValues[12] = { scale.v[0],0,0,0,scale.v[1],0,0,0,scale.v[2],0,0,0 };
			modelMatrix_t mat = (rotation.ToMat43().GetTranspose() * RichMat43(scaleMatValues)).m;
			g_mfn->Math_VecCopy(position.v, mat.o);
			entityMatrices.push_back(std::make_pair(section1ID, mat));
		}
		//Section 3 : we don't have useful info to get from this section
	}
};

void DirtyAlign(BYTE* buffer, uint32_t& offset)
{
	while (!*(uint32_t*)(buffer + offset))
		offset += 4;
}

template<bool bBigEndian>
bool UnpackBundles(BYTE* buffer, int bufferLen, std::map<uint32_t, std::vector<BYTE*>>& bundleIDtoG1mOffsets, std::map<uint32_t, std::vector<uint32_t>>& bundleIDtoG1mSizes)
{
	size_t offset = 0;
	bool bSuccess = true;

	//Check entry count first, should be 2 in most cases
	uint32_t entryCount = *(uint32_t*)(buffer);
	//assert(entryCount == 2);
	uint32_t firstOffset = *(uint32_t*)(buffer + 4);
	uint32_t firstSize = *(uint32_t*)(buffer + 8);
	offset = firstOffset;

	//Parsing bundle sizes
	std::vector<uint32_t> bundleSizes;
	uint32_t bundleSizesOffsets = offset;
	uint32_t bundleCount = *(uint32_t*)(buffer + offset);
	bundleSizes.resize(bundleCount);
	memcpy(bundleSizes.data(), buffer + offset + 4, bundleCount * sizeof(uint32_t));
	offset += (4 + bundleCount * 4);
	DirtyAlign(buffer, offset);

	//Parsing bundles, for each of them, read only first mdlk
	for (auto i = 0; i<bundleSizes.size(); i++)
	{
		uint32_t bundleStartOffset = offset;
		uint32_t mdlkCount = *(uint32_t*)(buffer + offset);
		offset += (4 + mdlkCount * 4);
		DirtyAlign(buffer, offset);
		offset += 8; //8 comes from MLDK magic + header
		uint32_t g1mCount = *(uint32_t*)(buffer + offset);
		offset += 8; //rest of MLDK header
		//Get all the g1m info
		for (auto j = 0; j < g1mCount; j++)
		{
			GResourceHeader<bBigEndian> g1mHeader = reinterpret_cast<GResourceHeader<bBigEndian>*>(buffer + offset);
			//check if correct g1m file, else break
			uint8_t ed = *(uint8_t*)(buffer + offset);
			bSuccess = !(bBigEndian ^ (ed == 0x47 ? true : false));
			if (!bSuccess)
				break;
			bundleIDtoG1mOffsets[i].push_back(buffer + offset);
			bundleIDtoG1mSizes[i].push_back(g1mHeader.chunkSize);
			offset += g1mHeader.chunkSize;
		}
		offset = bundleStartOffset + bundleSizes[i];
	}

	return bSuccess;
}


#endif OBJD_H
