#pragma once
#ifndef G1M_G_IBUF
#define G1M_G_IBUF

template <bool bBigEndian>
struct G1MGIndexBuffer
{
	uint32_t count;
	rpgeoDataType_e dataType;
	uint32_t bitwidth;
	BYTE* bufferAdress = nullptr;
	uint32_t debugOffset;

	G1MGIndexBuffer(BYTE* buffer, uint32_t& offset, uint32_t version)
	{
		count = *(uint32_t*)(buffer + offset);
		uint32_t dType = *(uint32_t*)(buffer + offset +4);
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(count);
			LITTLE_BIG_SWAP(dType);
		}
		if (version > 0x30303430)
			offset += 12;
		else
			offset += 8;
		switch (dType)
		{
		case 0x8:
			dataType = RPGEODATA_UBYTE;
			bitwidth = 1;
			break;
		case 0x10:
			dataType = RPGEODATA_USHORT;
			bitwidth = 2;
			break;
		case 0x20:
			dataType = RPGEODATA_UINT;
			bitwidth = 4;
			break;
		default:
			break;
		}
		bufferAdress = buffer + offset;
		debugOffset = offset;
		offset += count * bitwidth;
		AlignOffset(offset, 4);
	}
};

#endif // !G1M_I_VBUF
