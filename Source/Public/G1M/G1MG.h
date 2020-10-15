#pragma once

#ifndef G1MG_
#define G1MG_

#include "G1MG/G1MGMaterial.h"
#include "G1MG/G1MGVertexBuffer.h"
#include "G1MG/G1MGVertexAttributes.h"
#include "G1MG/G1MGIndexBuffer.h"
#include "G1MG/G1MGBonePalette.h"
#include "G1MG/G1MGSubMesh.h"
#include "G1MG/G1MGMesh.h"

#define SECTION1_MAGIC   0x00010001
#define MATERIALS_MAGIC  0x00010002
#define SECTION3_MAGIC  0x00010003
#define VERTEX_BUFFERS_MAGIC  0x00010004
#define VERTEX_ATTRIBUTES_MAGIC  0x00010005
#define JOINT_PALETTES_MAGIC  0x00010006
#define INDEX_BUFFER_MAGIC  0x00010007
#define SUBMESH_MAGIC  0x00010008
#define MESH_MAGIC  0x00010009


template <bool bBigEndian>
struct G1MGSubSectionHeader
{
	uint32_t magic;
	uint32_t size;
	uint32_t count;

	G1MGSubSectionHeader(G1MGSubSectionHeader* ptr) : G1MGSubSectionHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(magic);
			LITTLE_BIG_SWAP(size);
			LITTLE_BIG_SWAP(count);
		}
	}
};

template <bool bBigEndian>
struct G1MGHeader
{
	uint32_t platform;
	uint32_t reserved;
	float min_x;
	float min_y;
	float min_z;
	float max_x;
	float max_y;
	float max_z;
	uint32_t sectionCount;

	G1MGHeader(G1MGHeader* ptr) : G1MGHeader(*ptr)
	{
		if (bBigEndian)
		{
			LITTLE_BIG_SWAP(platform);
			LITTLE_BIG_SWAP(min_x);
			LITTLE_BIG_SWAP(min_y);
			LITTLE_BIG_SWAP(min_z);
			LITTLE_BIG_SWAP(max_x);
			LITTLE_BIG_SWAP(max_y);
			LITTLE_BIG_SWAP(max_z);
			LITTLE_BIG_SWAP(sectionCount);
		}
	}
};

template <bool bBigEndian>
struct G1MG
{

	std::vector<G1MGMaterial<bBigEndian>> materials;
	std::vector<G1MGVertexBuffer<bBigEndian>> vertexBuffers;
	std::vector<G1MGVertexAttributeSet<bBigEndian>> vertexAttributeSets;
	std::vector<G1MGJointPalette<bBigEndian>> jointPalettes;
	std::vector<G1MGIndexBuffer<bBigEndian>> indexBuffers;
	std::vector<G1MGSubmesh<bBigEndian>> submeshes;
	std::vector<G1MGMeshGroup<bBigEndian>> meshGroups;

	G1MG(BYTE* buffer, size_t startOffset, G1MS<bBigEndian>* internalSkel, G1MS<bBigEndian>* externalSkel, std::map<uint32_t, uint32_t>* globalToFinal)
	{
		size_t offset = startOffset;
		//Read headers
		GResourceHeader sectionHeader = reinterpret_cast<GResourceHeader<bBigEndian>*>(buffer + offset);
		offset = startOffset + 12;
		G1MGHeader<bBigEndian> g1mgHeader = reinterpret_cast<G1MGHeader<bBigEndian>*>(buffer + offset);
		offset += sizeof(G1MGHeader<bBigEndian>);
		size_t checkpoint = offset;
		for (auto i = 0; i < g1mgHeader.sectionCount; i++)
		{
			offset = checkpoint;
			G1MGSubSectionHeader<bBigEndian> subSectionHeader = reinterpret_cast<G1MGSubSectionHeader<bBigEndian>*>(buffer + offset);
			offset += sizeof(G1MGSubSectionHeader<bBigEndian>);
			switch (subSectionHeader.magic)
			{
			case SECTION1_MAGIC:
				break;
			case MATERIALS_MAGIC:
				for (auto j = 0; j < subSectionHeader.count; j++)
				{
					materials.push_back(G1MGMaterial<bBigEndian>(buffer, offset));
				}
				break;
			case SECTION3_MAGIC:
				break;
			case VERTEX_BUFFERS_MAGIC:
				for (auto j = 0; j < subSectionHeader.count; j++)
				{
					vertexBuffers.push_back(G1MGVertexBuffer<bBigEndian>(buffer, offset,sectionHeader.chunkVersion));
				}
				break;
			case VERTEX_ATTRIBUTES_MAGIC:
				for (auto j = 0; j < subSectionHeader.count; j++)
				{
					vertexAttributeSets.push_back(G1MGVertexAttributeSet<bBigEndian>(buffer, offset));
				}
				break;
			case JOINT_PALETTES_MAGIC:
				for (auto j = 0; j < subSectionHeader.count; j++)
				{
					jointPalettes.push_back(G1MGJointPalette<bBigEndian>(buffer, offset,internalSkel,externalSkel,globalToFinal));
				}
				break;
			case INDEX_BUFFER_MAGIC:
				for (auto j = 0; j < subSectionHeader.count; j++)
				{
					indexBuffers.push_back(G1MGIndexBuffer<bBigEndian>(buffer, offset, sectionHeader.chunkVersion));
				}
				break;
			case SUBMESH_MAGIC:
				for (auto j = 0; j < subSectionHeader.count; j++)
				{
					G1MGSubmesh<bBigEndian> subM = reinterpret_cast<G1MGSubmesh<bBigEndian>*>(buffer + offset);
					submeshes.push_back(std::move(subM));
					offset += sizeof(G1MGSubmesh<bBigEndian>);
				}
				break;
			case MESH_MAGIC:
				for (auto j = 0; j < subSectionHeader.count; j++)
				{
					meshGroups.push_back(G1MGMeshGroup<bBigEndian>(buffer, offset, sectionHeader.chunkVersion));
				}
				break;
			default:
				break;
			}
			checkpoint += subSectionHeader.size;
		}
	}
};

#endif //!G1MG_