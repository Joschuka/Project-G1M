#pragma once

#ifndef ARCHIVES_H
#define ARCHIVES_H

typedef struct HEADER_DZ_S
{
	uint32_t CHUNKS;
	uint32_t UNZIPPED_SIZE;
} HEADER_DZ_T;

typedef struct HEADER_GZ_S
{
	uint32_t WINDOW_SIZE;
	uint32_t CHUNKS;
	uint32_t UNZIPPED_SIZE;
} HEADER_GZ_T;

__int64 Align(__int64 offset, uint32_t alignNum) {
	__int64 value = offset % alignNum;

	if (value) {
		return offset + (alignNum - value);
	}
	else
	{
		return offset;
	}
}

template<bool bBigEndian>
bool HandleArchiveDZ(FILE* f, __int64 fileLen, bool justChecking, noeRAPI_t* rapi)
{

	if (fileLen < 128) return false;

	__int64 offset = 0;

	HEADER_DZ_T header;
	fread(&header, sizeof(header), 1, f); offset += sizeof(HEADER_DZ_T);
	
	if (bBigEndian)
	{
		LITTLE_BIG_SWAP(header.CHUNKS);

		LITTLE_BIG_SWAP(header.UNZIPPED_SIZE);
	}

	// check there is another data to read offsets
	if ((header.CHUNKS * 4) + 8 > fileLen) return false;

	std::vector<uint32_t> chunkSizes; chunkSizes.resize(header.CHUNKS);

	fread(&chunkSizes.front(), sizeof(uint32_t) * header.CHUNKS, 1, f); offset += sizeof(uint32_t) * header.CHUNKS;

	if (justChecking) return true;

	BYTE* rawData = (BYTE*)rapi->Noesis_UnpooledAlloc(header.UNZIPPED_SIZE);

	uint32_t unzippedOffset = 0;

	for (uint32_t chunkNum = 0; chunkNum < header.CHUNKS; ++chunkNum)
	{
		
		uint32_t chunkSize = chunkSizes[chunkNum];

		offset = Align(offset, 128);
		_fseeki64(f, offset, SEEK_SET);

		uint32_t zipSize = 0;

		fread(&zipSize, sizeof(uint32_t), 1, f); offset += 4;

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(zipSize);

			LITTLE_BIG_SWAP(chunkSize);
		}

		if (zipSize + 4 == chunkSize)
		{
			// is compressed
			BYTE* zipChunk = (BYTE*)rapi->Noesis_UnpooledAlloc(zipSize);

			fread(zipChunk, 1, zipSize, f); offset += zipSize;

			int unzipSize = rapi->Noesis_GetInflatedSize(zipChunk, zipSize);

			int retCode = rapi->Decomp_Inflate(zipChunk, rawData + unzippedOffset, zipSize, header.UNZIPPED_SIZE - unzippedOffset);

			if (retCode < 0)
			{
				return false;
			}

			unzippedOffset += unzipSize;

			rapi->Noesis_UnpooledFree(zipChunk);
		}
		else
		{
			// not compressed, just copy full chunk
			offset -= 4; _fseeki64(f, offset, SEEK_SET);

			BYTE* zipChunk = (BYTE*)rapi->Noesis_UnpooledAlloc(chunkSize);

			fread(zipChunk, 1, chunkSize, f); offset += chunkSize;

			memcpy(rawData + unzippedOffset, zipChunk, chunkSize);

			unzippedOffset += chunkSize;

			rapi->Noesis_UnpooledFree(zipChunk);
		}

	}

	std::string inFile = rapi->Noesis_GetLastCheckedName();

	std::filesystem::path pathObj(inFile);

	std::string fileName = pathObj.stem().string(); // should drop the .dz

	//write it out
	rapi->LogOutput("Writing '%s'.\n", fileName.c_str());

	rapi->Noesis_ExportArchiveFile((char *)fileName.c_str(), rawData, header.UNZIPPED_SIZE);

	rapi->Noesis_UnpooledFree(rawData);

	return true;
}

template<bool bBigEndian>
bool HandleArchiveGZ(FILE* f, __int64 fileLen, bool justChecking, noeRAPI_t* rapi)
{
	if (fileLen < 128) return false;

	__int64 offset = 0;

	HEADER_GZ_T header;
	fread(&header, sizeof(header), 1, f); offset += sizeof(HEADER_GZ_T);

	if (bBigEndian)
	{
		LITTLE_BIG_SWAP(header.CHUNKS);

		LITTLE_BIG_SWAP(header.UNZIPPED_SIZE);
	}

	// check there is another data to read offsets
	if ((header.CHUNKS * 4) + 8 > fileLen) return false;

	std::vector<uint32_t> chunkSizes; chunkSizes.resize(header.CHUNKS);

	fread(&chunkSizes.front(), sizeof(uint32_t) * header.CHUNKS, 1, f); offset += sizeof(uint32_t) * header.CHUNKS;

	if (justChecking) return true;

	BYTE* rawData = (BYTE*)rapi->Noesis_UnpooledAlloc(header.UNZIPPED_SIZE);

	uint32_t unzippedOffset = 0;

	for (uint32_t chunkNum = 0; chunkNum < header.CHUNKS; ++chunkNum)
	{

		uint32_t chunkSize = chunkSizes[chunkNum];

		offset = Align(offset, 128);
		_fseeki64(f, offset, SEEK_SET);

		uint32_t zipSize = 0;

		fread(&zipSize, sizeof(uint32_t), 1, f); offset += 4;

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(zipSize);

			LITTLE_BIG_SWAP(chunkSize);
		}

		if (zipSize + 4 == chunkSize)
		{
			// is compressed
			BYTE* zipChunk = (BYTE*)rapi->Noesis_UnpooledAlloc(zipSize);

			fread(zipChunk, 1, zipSize, f); offset += zipSize;

			int unzipSize = rapi->Noesis_GetInflatedSize(zipChunk, zipSize);

			int retCode = rapi->Decomp_Inflate(zipChunk, rawData + unzippedOffset, zipSize, header.UNZIPPED_SIZE - unzippedOffset);

			if (retCode < 0)
			{
				return false;
			}

			unzippedOffset += unzipSize;

			rapi->Noesis_UnpooledFree(zipChunk);
		}
		else
		{
			// not compressed, just copy full chunk
			offset -= 4; _fseeki64(f, offset, SEEK_SET);

			BYTE* zipChunk = (BYTE*)rapi->Noesis_UnpooledAlloc(chunkSize);

			fread(zipChunk, 1, chunkSize, f); offset += chunkSize;

			memcpy(rawData + unzippedOffset, zipChunk, chunkSize);

			unzippedOffset += chunkSize;

			rapi->Noesis_UnpooledFree(zipChunk);
		}

	}

	std::string inFile = rapi->Noesis_GetLastCheckedName();

	std::filesystem::path pathObj(inFile);

	std::string fileName = pathObj.stem().string(); // should drop the .gz

	//write it out
	rapi->LogOutput("Writing '%s'.\n", fileName.c_str());

	rapi->Noesis_ExportArchiveFile((char*)fileName.c_str(), rawData, header.UNZIPPED_SIZE);

	rapi->Noesis_UnpooledFree(rawData);

	return true;
}

#endif // !ARCHIVES_H