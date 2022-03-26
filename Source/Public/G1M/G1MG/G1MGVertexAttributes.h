#pragma once
#ifndef G1M_G_VA
#define G1M_G_VA

enum class EG1MGVADatatype : uint8_t
{
	VADataType_Float_x1 = 0x00,
	VADataType_Float_x2 = 0x01,
	VADataType_Float_x3 = 0x02,
	VADataType_Float_x4 = 0x03,
	VADataType_UByte_x4 = 0x05,
	VADataType_UShort_x4 = 0x07,
	VADataType_UInt_x4 = 0x09, //Need confirmation
	VADataType_HalfFloat_x2 = 0x0A,
	VADataType_HalfFloat_x4 = 0x0B,
	VADataType_NormUByte_x4 = 0x0D,
	VADataType_Dummy = 0xFF
};

enum class EG1MGVASemantic : uint8_t
{
	Position = 0x00,
	JointWeight,
	JointIndex,
	Normal,
	PSize,
	UV,
	Tangent,
	Binormal,
	TessalationFactor,
	PosTransform,
	Color,
	Fog,
	Depth,
	Sample
};

struct G1MGVertexAttribute
{
	uint16_t bufferID;
	uint16_t offset;
	EG1MGVADatatype dataType;
	EG1MGVASemantic semantic;
	uint8_t layer;
};

template <bool bBigEndian>
struct G1MGVertexAttributeSet
{
	uint32_t indexCount;
	std::vector<uint32_t> vBufferIndices;
	uint32_t attributesCount;
	std::vector<G1MGVertexAttribute> attributes;

	G1MGVertexAttributeSet(BYTE* buffer, uint32_t& offset)
	{	
		//Get vBuffer refs count
		indexCount = *reinterpret_cast<uint32_t*>(buffer + offset); 
		if (bBigEndian)
			LITTLE_BIG_SWAP(indexCount);
		offset += 4;
		for (auto i = 0; i < indexCount; i++)
		{
			uint32_t idx = *reinterpret_cast<uint32_t*>(buffer + offset);
			if (bBigEndian)
				LITTLE_BIG_SWAP(idx);
			vBufferIndices.push_back(idx);
			offset += 4;
		}

		//Get all the  attributes parameters
		attributesCount = *reinterpret_cast<uint32_t*>(buffer + offset);
		if (bBigEndian)
			LITTLE_BIG_SWAP(attributesCount);
		offset += 4;
		for (auto i = 0; i < attributesCount; i++)
		{
			G1MGVertexAttribute attribute;
			attribute.bufferID = *reinterpret_cast<uint16_t*>(buffer + offset);
			attribute.offset = *reinterpret_cast<uint16_t*>(buffer + offset + 2);
			offset += 4;
			attribute.dataType = *reinterpret_cast<EG1MGVADatatype*>(buffer + offset);
			attribute.semantic = *reinterpret_cast<EG1MGVASemantic*>(buffer + offset + 2);
			attribute.layer = *reinterpret_cast<uint8_t*>(buffer + offset + 3);
			offset += 4;
			if (bBigEndian)
			{
				//Only need to swap these two since others are 1 byte long
				LITTLE_BIG_SWAP(attribute.bufferID);
				LITTLE_BIG_SWAP(attribute.offset);
			}
			attribute.bufferID = vBufferIndices[attribute.bufferID];
			attributes.push_back(std::move(attribute));
		}
	}
};

#endif // !G1M_G_VA
