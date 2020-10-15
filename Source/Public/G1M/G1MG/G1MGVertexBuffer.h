#pragma once
#ifndef G1M_G_VBUF
#define G1M_G_VBUF

template <bool bBigEndian>
struct G1MGVertexBuffer
{
	uint32_t stride;
	uint32_t count;
	BYTE* bufferAdress = nullptr;

	G1MGVertexBuffer(BYTE* buffer, uint32_t& offset, uint32_t version)
	{
		offset += 4; //skipping unk1
		stride = *(uint32_t*)(buffer + offset);
		count = *(uint32_t*)(buffer + offset + 4);
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(stride);
			LITTLE_BIG_SWAP(count);
		}
		if (version > 0x30303430)
			offset += 12; //skip un2
		else
			offset += 8;
		bufferAdress = buffer + offset; //Might store "buffer" here as well too, we'll see
		offset += stride * count;
	}
};

#endif // !G1M_G_VBUF
