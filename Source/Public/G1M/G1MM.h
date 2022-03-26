#pragma once

#ifndef G1MM_
#define G1MM_

template<bool bBigEndian>
struct G1MM
{
	uint32_t matrixCount;
	std::vector<RichMat44> matrices;
	G1MM(BYTE* buffer, uint32_t startOffset)
	{
		uint32_t offset = startOffset;
		GResourceHeader sectionHeader = reinterpret_cast<GResourceHeader<bBigEndian>*>(buffer + offset);
		offset = startOffset + 12;
		matrixCount = *reinterpret_cast<uint32_t*>(buffer+offset);

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(matrixCount);
		}
		offset += 4;

		matrices.resize(matrixCount);
		memcpy(matrices.data(), buffer+offset, matrixCount* sizeof(RichMat44));
		
		if (bBigEndian)
		{
			for (auto& m : matrices)
			{
				m.ChangeEndian();
			}
		}
	}
};

#endif // !G1MM_

