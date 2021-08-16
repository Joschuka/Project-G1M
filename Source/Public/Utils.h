#pragma once

#ifndef UTILS_H
#define UTILS_H

void AlignOffset(uint32_t& offset, int n)
{
	int a = offset % n;
	if (offset % n)
		offset += (n - a);
}

void RevAlignOffset(uint32_t & offset, int n)
{
	int a = offset % n;
	if (offset % n)
		offset -= a;
}

void function1(std::array<uint64_t, 4> & quantizedData, RichVec3 & result, float currentTime, float totalTime)
{
	float time_pc = currentTime / totalTime;
	float time_squared = time_pc * time_pc;
	float time_cubed = time_squared * time_pc;

	uint64_t row1 = quantizedData[0x0];
	uint64_t row2 = quantizedData[0x1];
	uint64_t row3 = quantizedData[0x2];
	uint64_t row4 = quantizedData[0x3];

	float x, y, z, w;
	uint32_t a, b, c, d;
	a = ((row1 >> 0x25) & 0x7800000) + 0x32000000;
	b = ((row2 >> 0x25) & 0x7800000) + 0x32000000;
	c = ((row3 >> 0x25) & 0x7800000) + 0x32000000;
	d = ((row4 >> 0x25) & 0x7800000) + 0x32000000;
	memcpy(&x, &a, 4);
	memcpy(&y, &b, 4);
	memcpy(&z, &c, 4);
	memcpy(&w, &d, 4);

	result.v[0x0] = (float)(int32_t)((row4 >> 28) & 0xFFFFF000) * w * time_cubed +
		(float)(int32_t)((row1 >> 28) & 0xFFFFF000) * x +
		(float)(int32_t)((row2 >> 28) & 0xFFFFF000) * y * time_pc +
		(float)(int32_t)((row3 >> 28) & 0xFFFFF000) * z * time_squared;
	result.v[0x1] = (float)(int32_t)((row4 >> 8) & 0xFFFFF000) * w * time_cubed +
		(float)(int32_t)((row1 >> 8) & 0xFFFFF000) * x +
		(float)(int32_t)((row2 >> 8) & 0xFFFFF000) * y * time_pc +
		(float)(int32_t)((row3 >> 8) & 0xFFFFF000) * z * time_squared;
	result.v[0x2] = (float)(int32_t)(row4 << 12) * w * time_cubed +
		(float)(int32_t)(row1 << 12) * x +
		(float)(int32_t)(row2 << 12) * y * time_pc +
		(float)(int32_t)(row3 << 12) * z * time_squared;
}

void function2(RichVec3 & vec, RichQuat & quat)
{
	float angle = sqrt(vec.v[0] * vec.v[0] + vec.v[1] * vec.v[1] + vec.v[2] * vec.v[2]);
	float s = sin(angle * 0.5);
	float c = cos(angle * 0.5);
	if (angle > 0.000011920929)
	{
		quat.q[0] = vec.v[0] * (s / angle);
		quat.q[1] = vec.v[1] * (s / angle);
		quat.q[2] = vec.v[2] * (s / angle);
	}
	else
	{
		quat.q[0] = vec.v[0] * 0.5;
		quat.q[1] = vec.v[1] * 0.5;
		quat.q[2] = vec.v[2] * 0.5;

	}
	quat.q[3] = c;
}

void function3(std::vector<std::vector<std::array<float, 4>>> & chanValues, std::vector<std::vector<float>> & chanTimes, uint32_t index, uint32_t componentCount,
	std::set<float> & allTimes, std::vector<float> & allValues, uint32_t & stride)
{
	for (auto u = 0; u < componentCount; u++)
	{
		for (auto& time : chanTimes[index + u])
			allTimes.insert(time);
	}
	allTimes.insert(0.0);
	stride = allTimes.size();
	for (auto u = 0; u < componentCount; u++)
	{
		for (auto& t : allTimes)
		{
			uint32_t chanIndex = chanValues[index + u].size() - 1;
			for (auto v = 0; v < chanTimes[index + u].size(); v++)
			{
				float& t1 = chanTimes[index + u][v];
				if (t < t1)
				{
					chanIndex = v;
					break;
				}
			}
			float a = chanValues[index + u][chanIndex][0];
			float b = chanValues[index + u][chanIndex][1];
			float c = chanValues[index + u][chanIndex][2];
			float d = chanValues[index + u][chanIndex][3];
			float t1 = chanTimes[index + u][chanIndex];
			float t0;
			if (chanIndex == 0)
				t0 = 0.0;
			else
				t0 = chanTimes[index + u][chanIndex - 1];
			float tratio = (t - t0) / (t1 - t0);
			float value = a * pow(tratio, 3) + b * pow(tratio, 2) + c * tratio + d;
			allValues.push_back(value);
		}
	}


}

bool has_suffix(const std::string & str, const std::string & suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

template<bool bBigEndian>
void transformPosF(BYTE * posBuffer, int vertexCount, int stride, modelMatrix_t * mat)
{
	for (int i = 0; i < vertexCount; i++)
	{
		float* src = (float*)(posBuffer + stride * i);
		float tmp[3];
		if (bBigEndian) {
			LITTLE_BIG_SWAP(src[0]);
			LITTLE_BIG_SWAP(src[1]);
			LITTLE_BIG_SWAP(src[2]);
		}
		g_mfn->Math_TransformPointByMatrix(mat, src, tmp);
		g_mfn->Math_VecCopy(tmp, src);
		if (bBigEndian) {
			LITTLE_BIG_SWAP(src[0]);
			LITTLE_BIG_SWAP(src[1]);
			LITTLE_BIG_SWAP(src[2]);
		}
	}
}

template<bool bBigEndian>
void transformPosHF(BYTE * hfposBuffer, BYTE * posBuffer, int vertexCount, int stride, modelMatrix_t * mat = nullptr)
{
	float* dst = (float*)posBuffer;

	for (int i = 0; i < vertexCount; i++)
	{
		uint16_t* src = (uint16_t*)(hfposBuffer + stride * i);
		float tmp1[3];
		float tmp2[3];
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(src[0]);
			LITTLE_BIG_SWAP(src[1]);
			LITTLE_BIG_SWAP(src[2]);
		}
		tmp1[0] = g_mfn->Math_GetFloat16(src[0]);
		tmp1[1] = g_mfn->Math_GetFloat16(src[1]);
		tmp1[2] = g_mfn->Math_GetFloat16(src[2]);

		if (mat)
		{
			g_mfn->Math_TransformPointByMatrix(mat, tmp1, tmp2);
			g_mfn->Math_VecCopy(tmp2, dst + 3 * i);
		}
		else
		{
			g_mfn->Math_VecCopy(tmp1, dst + 3 * i);
		}

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(dst[3 * i]);
			LITTLE_BIG_SWAP(dst[3 * i + 1]);
			LITTLE_BIG_SWAP(dst[3 * i + 2]);
		}
	}
}

template<bool bBigEndian>
void skinSMeshW(BYTE * jointWB, EG1MGVADatatype jointWBType, BYTE * jointWB2, EG1MGVADatatype jointWB2Type, BYTE * finaljointWB, int vertexCount, int stride, bool bHas8Weights)
{
	float* dstW = (float*)finaljointWB;
	int dstOff = bHas8Weights ? 8 : 4;
	memset(dstW, 0, dstOff * sizeof(float) * vertexCount);

	//Weight 1st layer
	if (!jointWB) //sometimes only bone indices and no weights, in that case assume a weight of 1 on the first one.
	{
		for (auto i = 0; i < vertexCount; i++)
		{
			dstW[dstOff * i] = 1;
			if (bBigEndian)
				LITTLE_BIG_SWAP(dstW[dstOff * i]);
		}
	}
	else
	{
		int maxWBID;
		float accW;
		switch (jointWBType)
		{
		case EG1MGVADatatype::VADataType_Float_x1:
			maxWBID = 1;
			for (auto i = 0; i < vertexCount; i++)
			{
				accW = 0;
				for (auto j = 0; j < maxWBID; j++)
				{
					dstW[dstOff * i + j] = *(float*)(jointWB + stride * i + 4 * j);
					if (bBigEndian)
					{
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
					}
					accW += dstW[dstOff * i + j];
				}
				dstW[dstOff * i + maxWBID] = 1 - accW;
				if (dstW[dstOff * i + maxWBID] < 0.00001)
					dstW[dstOff * i + maxWBID] = 0;
				if (bBigEndian)
				{
					for (auto j = 0; j < maxWBID + 1; j++)
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
				}
			}
			break;
		case EG1MGVADatatype::VADataType_Float_x2:
			maxWBID = 2;
			for (auto i = 0; i < vertexCount; i++)
			{
				accW = 0;
				for (auto j = 0; j < maxWBID; j++)
				{
					dstW[dstOff * i + j] = *(float*)(jointWB + stride * i + 4 * j);
					if (bBigEndian)
					{
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
					}
					accW += dstW[dstOff * i + j];
				}
				dstW[dstOff * i + maxWBID] = 1 - accW;
				if (dstW[dstOff * i + maxWBID] < 0.00001)
					dstW[dstOff * i + maxWBID] = 0;
				if (bBigEndian)
				{
					for (auto j = 0; j < maxWBID + 1; j++)
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
				}
			}
			break;
		case EG1MGVADatatype::VADataType_Float_x3:
			maxWBID = 3;
			for (auto i = 0; i < vertexCount; i++)
			{
				accW = 0;
				for (auto j = 0; j < maxWBID; j++)
				{
					dstW[dstOff * i + j] = *(float*)(jointWB + stride * i + 4 * j);
					if (bBigEndian)
					{
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
					}
					accW += dstW[dstOff * i + j];
				}
				dstW[dstOff * i + maxWBID] = 1 - accW;
				if (dstW[dstOff * i + maxWBID] < 0.00001)
					dstW[dstOff * i + maxWBID] = 0;
				if (bBigEndian)
				{
					for (auto j = 0; j < maxWBID + 1; j++)
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
				}
			}
			break;
		case EG1MGVADatatype::VADataType_Float_x4:
			maxWBID = 4;
			for (auto i = 0; i < vertexCount; i++)
			{
				for (auto j = 0; j < maxWBID; j++)
				{
					dstW[dstOff * i + j] = *(float*)(jointWB + stride * i + 4 * j);
					if (bBigEndian)
					{
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
					}
				}
				if (bBigEndian)
				{
					for (auto j = 0; j < maxWBID; j++)
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
				}
			}
			break;
		case EG1MGVADatatype::VADataType_HalfFloat_x2:
			maxWBID = 2;
			for (auto i = 0; i < vertexCount; i++)
			{
				accW = 0;
				for (auto j = 0; j < maxWBID; j++)
				{
					uint16_t tmp = *(uint16_t*)(jointWB + stride * i + 2 * j);
					if (bBigEndian)
					{
						LITTLE_BIG_SWAP(tmp);
					}
					float tmp2 = g_mfn->Math_GetFloat16(tmp);
					accW += tmp2;
					dstW[dstOff * i + j] = tmp2;
				}
				dstW[dstOff * i + maxWBID] = 1 - accW;
				if (dstW[dstOff * i + maxWBID] < 0.00001)
					dstW[dstOff * i + maxWBID] = 0;
				if (bBigEndian)
				{
					for (auto j = 0; j < maxWBID + 1; j++)
						LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
				}
			}
			break;
		case EG1MGVADatatype::VADataType_HalfFloat_x4:
			maxWBID = 4;
			for (auto i = 0; i < vertexCount; i++)
			{
				for (auto j = 0; j < maxWBID; j++)
				{
					uint16_t tmp = *(uint16_t*)(jointWB + stride * i + 2 * j);
					if (bBigEndian)
					{
						LITTLE_BIG_SWAP(tmp);
					}
					dstW[dstOff * i + j] = g_mfn->Math_GetFloat16(tmp);
					if (bBigEndian)
					{
						for (auto j = 0; j < maxWBID; j++)
							LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
					}
				}

			}
			break;
		case EG1MGVADatatype::VADataType_NormUByte_x4:
			maxWBID = 4;
			for (auto i = 0; i < vertexCount; i++)
			{
				for (auto j = 0; j < maxWBID; j++)
				{
					dstW[dstOff * i + j] = *(uint8_t*)(jointWB + stride * i + j) / 255.0f;
				}
			}
			break;
		default:
			break;
		}

		if (jointWB2)
		{
			switch (jointWB2Type)
			{
			case EG1MGVADatatype::VADataType_Float_x1:
				maxWBID = 1;
				for (auto i = 0; i < vertexCount; i++)
				{
					accW = 0;
					for (auto j = 0; j < 4; j++)
					{
						accW += dstW[dstOff * i + j];
					}
					for (auto j = 0; j < maxWBID; j++)
					{
						dstW[dstOff * i + j + 4] = *(float*)(jointWB2 + stride * i + 4 * j);
						if (bBigEndian)
						{
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
						}
						accW += dstW[dstOff * i + j + 4];
					}
					dstW[dstOff * i + maxWBID + 4] = 1 - accW;
					if (dstW[dstOff * i + maxWBID + 4] < 0.00001)
						dstW[dstOff * i + maxWBID + 4] = 0;
					if (bBigEndian)
					{
						for (auto j = 0; j < maxWBID + 1; j++)
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
					}
				}
				break;
			case EG1MGVADatatype::VADataType_Float_x2:
				maxWBID = 2;
				for (auto i = 0; i < vertexCount; i++)
				{
					accW = 0;
					for (auto j = 0; j < 4; j++)
					{
						accW += dstW[dstOff * i + j];
					}
					for (auto j = 0; j < maxWBID; j++)
					{
						dstW[dstOff * i + j + 4] = *(float*)(jointWB2 + stride * i + 4 * j);
						if (bBigEndian)
						{
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
						}
						accW += dstW[dstOff * i + j + 4];
					}
					dstW[dstOff * i + maxWBID + 4] = 1 - accW;
					if (dstW[dstOff * i + maxWBID + 4] < 0.00001)
						dstW[dstOff * i + maxWBID + 4] = 0;
					if (bBigEndian)
					{
						for (auto j = 0; j < maxWBID + 1; j++)
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
					}
				}
				break;
			case EG1MGVADatatype::VADataType_Float_x3:
				maxWBID = 3;
				for (auto i = 0; i < vertexCount; i++)
				{
					accW = 0;
					for (auto j = 0; j < 4; j++)
					{
						accW += dstW[dstOff * i + j];
					}
					for (auto j = 0; j < maxWBID; j++)
					{
						dstW[dstOff * i + j + 4] = *(float*)(jointWB2 + stride * i + 4 * j);
						if (bBigEndian)
						{
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
						}
						accW += dstW[dstOff * i + j + 4];
					}
					dstW[dstOff * i + maxWBID + 4] = 1 - accW;
					if (dstW[dstOff * i + maxWBID + 4] < 0.00001)
						dstW[dstOff * i + maxWBID + 4] = 0;
					if (bBigEndian)
					{
						for (auto j = 0; j < maxWBID + 1; j++)
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
					}
				}
				break;
			case EG1MGVADatatype::VADataType_Float_x4:
				maxWBID = 4;
				for (auto i = 0; i < vertexCount; i++)
				{
					for (auto j = 0; j < maxWBID; j++)
					{
						dstW[dstOff * i + j + 4] = *(float*)(jointWB2 + stride * i + 4 * j);
						if (bBigEndian)
						{
							LITTLE_BIG_SWAP(dstW[dstOff * i + j]);
						}
					}
					if (bBigEndian)
					{
						for (auto j = 0; j < maxWBID; j++)
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
					}
				}
				break;
			case EG1MGVADatatype::VADataType_HalfFloat_x2:
				maxWBID = 2;
				for (auto i = 0; i < vertexCount; i++)
				{
					accW = 0;
					for (auto j = 0; j < 4; j++)
					{
						accW += dstW[dstOff * i + j];
					}
					for (auto j = 0; j < maxWBID; j++)
					{
						uint16_t tmp = *(uint16_t*)(jointWB2 + stride * i + 2 * j);
						if (bBigEndian)
						{
							LITTLE_BIG_SWAP(tmp);
						}
						float tmp2 = g_mfn->Math_GetFloat16(tmp);
						accW += tmp2;
						dstW[dstOff * i + j + 4] = tmp2;
					}
					dstW[dstOff * i + maxWBID + 4] = 1 - accW;
					if (dstW[dstOff * i + maxWBID + 4] < 0.00001)
						dstW[dstOff * i + maxWBID + 4] = 0;
					if (bBigEndian)
					{
						for (auto j = 0; j < maxWBID + 1; j++)
							LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
					}
				}
				break;
			case EG1MGVADatatype::VADataType_HalfFloat_x4:
				maxWBID = 4;
				for (auto i = 0; i < vertexCount; i++)
				{
					for (auto j = 0; j < maxWBID; j++)
					{
						uint16_t tmp = *(uint16_t*)(jointWB2 + stride * i + 2 * j);
						if (bBigEndian)
						{
							LITTLE_BIG_SWAP(tmp);
						}
						dstW[dstOff * i + j + 4] = g_mfn->Math_GetFloat16(tmp);
						if (bBigEndian)
						{
							for (auto j = 0; j < maxWBID; j++)
								LITTLE_BIG_SWAP(dstW[dstOff * i + j + 4]);
						}
					}

				}
				break;
			case EG1MGVADatatype::VADataType_NormUByte_x4:
				maxWBID = 4;
				for (auto i = 0; i < vertexCount; i++)
				{
					for (auto j = 0; j < maxWBID; j++)
					{
						dstW[dstOff * i + j + 4] = *(uint8_t*)(jointWB2 + stride * i + j) / 255.0f;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

template<bool bBigEndian>
void genColor3F(BYTE * buffer, BYTE * finalBuffer, int vertexCount, int stride)
{
	float* dst = (float*)finalBuffer;

	for (int i = 0; i < vertexCount; i++)
	{
		float* src = (float*)(buffer + stride * i);
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(src[0]);
			LITTLE_BIG_SWAP(src[1]);
			LITTLE_BIG_SWAP(src[2]);
		}

		dst[4 * i] = src[0];
		dst[4 * i + 1] = src[1];
		dst[4 * i + 2] = src[2];
		dst[4 * i + 3] = 1;

		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(dst[4 * i]);
			LITTLE_BIG_SWAP(dst[4 * i + 1]);
			LITTLE_BIG_SWAP(dst[4 * i + 2]);
			LITTLE_BIG_SWAP(dst[4 * i + 3]);
		}
	}
}

void createDriverVertexBuffers(mesh_t & dMesh, uint32_t cpSize, std::vector<void*> & unpooledBufs, noeRAPI_t * rapi)
{
	dMesh.posBuffer.address = (BYTE*)rapi->Noesis_UnpooledAlloc(sizeof(float) * 3 * cpSize);
	unpooledBufs.push_back(dMesh.posBuffer.address);
	dMesh.posBuffer.dataType = RPGEODATA_FLOAT;
	dMesh.posBuffer.stride = 12;
	float* posB = (float*)dMesh.posBuffer.address;

	dMesh.blendIndicesBuffer.address = (BYTE*)rapi->Noesis_UnpooledAlloc(sizeof(uint16_t) * cpSize);
	unpooledBufs.push_back(dMesh.blendIndicesBuffer.address);
	dMesh.blendIndicesBuffer.dataType = RPGEODATA_USHORT;
	dMesh.blendIndicesBuffer.stride = 2;
	uint16_t* bIB = (uint16_t*)dMesh.blendIndicesBuffer.address;
	dMesh.jointPerVertex = 1;

	dMesh.blendWeightsBuffer.address = (BYTE*)rapi->Noesis_UnpooledAlloc(sizeof(float) * cpSize);
	unpooledBufs.push_back(dMesh.blendWeightsBuffer.address);
	dMesh.blendWeightsBuffer.dataType = RPGEODATA_FLOAT;
	dMesh.blendWeightsBuffer.stride = 4;
	float* bWB = (float*)dMesh.blendWeightsBuffer.address;
}

void createDriverIndexBuffers(mesh_t & dMesh, std::vector<RichVec3> polys, std::vector<void*> & unpooledBufs, noeRAPI_t * rapi)
{
	dMesh.indexBuffer.address = (BYTE*)rapi->Noesis_UnpooledAlloc(sizeof(uint16_t) * 3 * polys.size());
	unpooledBufs.push_back(dMesh.indexBuffer.address);
	dMesh.indexBuffer.dataType = RPGEODATA_USHORT;
	dMesh.indexBuffer.indexCount = polys.size() * 3;
	dMesh.indexBuffer.primType = RPGEO_TRIANGLE;
	uint16_t* tB = (uint16_t*)dMesh.indexBuffer.address;
	for (auto i = 0; i < polys.size(); i++)
	{
		tB[3 * i] = (uint16_t)polys[i][0];
		tB[3 * i + 1] = (uint16_t)polys[i][1];
		tB[3 * i + 2] = (uint16_t)polys[i][2];
	}
}

void flip_vertically(BYTE * pixels, const size_t width, const size_t height, const size_t bytes_per_pixel)
{
	const size_t stride = width * bytes_per_pixel;
	BYTE* row = (BYTE*)malloc(stride);
	BYTE* low = pixels;
	BYTE* high = &pixels[(height - 1) * stride];

	for (; low < high; low += stride, high -= stride) {
		memcpy(row, low, stride);
		memcpy(low, high, stride);
		memcpy(high, row, stride);
	}
	free(row);
}

#endif // !UTILS_H