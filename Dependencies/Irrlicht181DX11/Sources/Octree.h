// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OCTREE_H_INCLUDED__
#define __C_OCTREE_H_INCLUDED__

#include "SViewFrustum.h"
#include "S3DVertex.h"
#include "aabbox3d.h"
#include "irrArray.h"
#include "CMeshBuffer.h"

/**
	Flags for Octree
*/
//! use meshbuffer for drawing, enables VBO usage
#define OCTREE_USE_HARDWARE	false
//! use visibility information together with VBOs
#define OCTREE_USE_VISIBILITY true
//! use bounding box or frustum for calculate polys
#define OCTREE_BOX_BASED true
//! bypass full invisible/visible test
#define OCTREE_PARENTTEST

namespace irr
{

//! template octree.
/** T must be a vertex type which has a member
called .Pos, which is a core::vertex3df position. */
class Octree
{
public:

	struct SIndexChunk
	{
		scene::CIndexBuffer IndexBuffer;
		s32 MaterialId;
	};

	struct SIndexData
	{
		scene::IIndexBuffer* IndexBuffer;
		s32 CurrentSize;
		s32 MaxSize;
	};


	//! Constructor
	Octree(const core::array<scene::IMeshBuffer*>& meshes, const core::array<s32>& meshesMatID, s32 minimalPolysPerNode=128) :
		IndexData(0), IndexDataCount(meshes.size()), NodeCount(0)
	{
		IndexData = new SIndexData[IndexDataCount];

		// construct array of all indices

		core::array<SIndexChunk>* indexChunks = new core::array<SIndexChunk>;

		indexChunks->reallocate(meshes.size());

		for (u32 i=0; i!=meshes.size(); ++i)
		{
			scene::IIndexBuffer* indexBuffer = meshes[i]->getIndexBuffer();
			u32 indexCount = indexBuffer->getIndexCount();

			IndexData[i].CurrentSize = 0;
			IndexData[i].MaxSize = indexCount;
			IndexData[i].IndexBuffer = new scene::CIndexBuffer(meshes[i]->getIndexBuffer()->getType());
			IndexData[i].IndexBuffer->reallocate(IndexData[i].MaxSize);

			indexChunks->push_back(SIndexChunk());

			SIndexChunk& tic = indexChunks->getLast();
			tic.MaterialId = meshesMatID[i];

			for (u32 j = 0; j < indexCount; ++j)
				tic.IndexBuffer.addIndex(indexBuffer->getIndex(j));
		}

		// create tree
		Root = new OctreeNode(NodeCount, 0, meshes, meshesMatID, indexChunks, minimalPolysPerNode);
	}

	//! returns all ids of polygons partially or fully enclosed
	//! by this bounding box.
	void calculatePolys(const core::aabbox3d<f32>& box)
	{
		for (u32 i=0; i!=IndexDataCount; ++i)
		{
			IndexData[i].IndexBuffer->set_used(0);
			IndexData[i].CurrentSize = 0;
		}

		Root->getPolys(box, IndexData, 0);
	}

	//! returns all ids of polygons partially or fully enclosed
	//! by a view frustum.
	void calculatePolys(const scene::SViewFrustum& frustum)
	{
		for (u32 i=0; i!=IndexDataCount; ++i)
		{
			IndexData[i].IndexBuffer->set_used(0);
			IndexData[i].CurrentSize = 0;
		}

		Root->getPolys(frustum, IndexData, 0);
	}

	const SIndexData* getIndexData() const
	{
		return IndexData;
	}

	u32 getIndexDataCount() const
	{
		return IndexDataCount;
	}

	u32 getNodeCount() const
	{
		return NodeCount;
	}

	//! for debug purposes only, collects the bounding boxes of the tree
	void getBoundingBoxes(const core::aabbox3d<f32>& box,
		core::array< const core::aabbox3d<f32>* >&outBoxes) const
	{
		Root->getBoundingBoxes(box, outBoxes);
	}

	//! destructor
	~Octree()
	{
		for (u32 i=0; i<IndexDataCount; ++i)
			delete IndexData[i].IndexBuffer;

		delete [] IndexData;
		delete Root;
	}

private:
	// private inner class
	class OctreeNode
	{
	public:

		// constructor
		OctreeNode(u32& nodeCount, u32 currentdepth,
			const core::array<scene::IMeshBuffer*>& meshes, const core::array<s32>& meshesMatID,
			core::array<SIndexChunk>* indices,
			s32 minimalPolysPerNode) : IndexData(0),
			Depth(currentdepth+1)
		{
			++nodeCount;

			u32 i; // new ISO for scoping problem with different compilers

			for (i=0; i!=8; ++i)
				Children[i] = 0;

			if (indices->empty())
			{
				delete indices;
				return;
			}

			bool found = false;

			// find first point for bounding box

			for (i=0; i<indices->size(); ++i)
			{
				const video::IVertexDescriptor* descriptor = (meshes[i] && meshes[i]->isVertexBufferCompatible()) ? meshes[i]->getVertexDescriptor() : 0;
				const video::IVertexAttribute* attribute = (descriptor) ? descriptor->getAttributeBySemantic(video::EVAS_POSITION) : 0;
				scene::IVertexBuffer* vertexBuffer = (attribute) ? meshes[i]->getVertexBuffer(attribute->getBufferID()) : 0;

				if (!vertexBuffer || vertexBuffer->getVertexCount() == 0)
					continue;

				u8* offset = static_cast<u8*>(vertexBuffer->getVertices());
				offset += attribute->getOffset();

				const u32 vertexSize = vertexBuffer->getVertexSize();

				if((*indices)[i].IndexBuffer.getIndexCount() > 0)
				{
					core::vector3df* position = (core::vector3df*)(offset + vertexSize * (*indices)[i].IndexBuffer.getIndex(0));

					Box.reset(*position);
					found = true;
					break;
				}
			}

			if (!found)
			{
				delete indices;
				return;
			}

			s32 totalPrimitives = 0;

			// now lets calculate our bounding box
			for (i=0; i<indices->size(); ++i)
			{
				const video::IVertexDescriptor* descriptor = (meshes[i] && meshes[i]->isVertexBufferCompatible()) ? meshes[i]->getVertexDescriptor() : 0;
				const video::IVertexAttribute* attribute = (descriptor) ? descriptor->getAttributeBySemantic(video::EVAS_POSITION) : 0;
				scene::IVertexBuffer* vertexBuffer = (attribute) ? meshes[i]->getVertexBuffer(attribute->getBufferID()) : 0;

				if (!vertexBuffer || vertexBuffer->getVertexCount() == 0)
					continue;

				u8* offset = static_cast<u8*>(meshes[i]->getVertexBuffer()->getVertices());
				offset += attribute->getOffset();

				const u32 vertexSize = vertexBuffer->getVertexSize();

				totalPrimitives += (*indices)[i].IndexBuffer.getIndexCount();

				for (u32 j=0; j<(*indices)[i].IndexBuffer.getIndexCount(); ++j)
				{
					core::vector3df* position = (core::vector3df*)(offset + vertexSize * (*indices)[i].IndexBuffer.getIndex(j));
					Box.addInternalPoint(*position);
				}
			}

			core::vector3df middle = Box.getCenter();
			core::vector3df edges[8];
			Box.getEdges(edges);

			// calculate all children
			core::aabbox3d<f32> box;
			core::array<u32> keepIndices;

			if (totalPrimitives > minimalPolysPerNode && !Box.isEmpty())
			for (u32 ch=0; ch!=8; ++ch)
			{
				box.reset(middle);
				box.addInternalPoint(edges[ch]);

				// create indices for child
				bool added = false;
				core::array<SIndexChunk>* cindexChunks = new core::array<SIndexChunk>;
				cindexChunks->reallocate(meshes.size());
				for (i=0; i<meshes.size(); ++i)
				{
					const video::IVertexDescriptor* descriptor = (meshes[i] && meshes[i]->isVertexBufferCompatible()) ? meshes[i]->getVertexDescriptor() : 0;
					const video::IVertexAttribute* attribute = (descriptor) ? descriptor->getAttributeBySemantic(video::EVAS_POSITION) : 0;
					scene::IVertexBuffer* vertexBuffer = (attribute) ? meshes[i]->getVertexBuffer(attribute->getBufferID()) : 0;

					if (!vertexBuffer || vertexBuffer->getVertexCount() == 0)
						continue;

					u8* offset = static_cast<u8*>(meshes[i]->getVertexBuffer()->getVertices());
					offset += attribute->getOffset();

					const u32 vertexSize = vertexBuffer->getVertexSize();

					cindexChunks->push_back(SIndexChunk());
					SIndexChunk& tic = cindexChunks->getLast();
					tic.MaterialId = meshesMatID[i];

					for (u32 t=0; t<(*indices)[i].IndexBuffer.getIndexCount(); t+=3)
					{
						core::vector3df* position0 = (core::vector3df*)(offset + vertexSize * (*indices)[i].IndexBuffer.getIndex(t));
						core::vector3df* position1 = (core::vector3df*)(offset + vertexSize * (*indices)[i].IndexBuffer.getIndex(t+1));
						core::vector3df* position2 = (core::vector3df*)(offset + vertexSize * (*indices)[i].IndexBuffer.getIndex(t+2));

						if (box.isPointInside(*position0) && box.isPointInside(*position1) && box.isPointInside(*position2))
						{
							tic.IndexBuffer.addIndex((*indices)[i].IndexBuffer.getIndex(t));
							tic.IndexBuffer.addIndex((*indices)[i].IndexBuffer.getIndex(t+1));
							tic.IndexBuffer.addIndex((*indices)[i].IndexBuffer.getIndex(t+2));

							added = true;
						}
						else
						{
							keepIndices.push_back((*indices)[i].IndexBuffer.getIndex(t));
							keepIndices.push_back((*indices)[i].IndexBuffer.getIndex(t+1));
							keepIndices.push_back((*indices)[i].IndexBuffer.getIndex(t+2));
						}
					}

					(*indices)[i].IndexBuffer.set_used(keepIndices.size());

					for (u32 t=0; t<keepIndices.size(); ++t)
						(*indices)[i].IndexBuffer.setIndex(t, keepIndices[t]);

					keepIndices.set_used(0);
				}

				if (added)
					Children[ch] = new OctreeNode(nodeCount, Depth,
						meshes, meshesMatID, cindexChunks, minimalPolysPerNode);
				else
					delete cindexChunks;

			} // end for all possible children

			IndexData = indices;
		}

		// destructor
		~OctreeNode()
		{
			delete IndexData;

			for (u32 i=0; i<8; ++i)
				delete Children[i];
		}

		// returns all ids of polygons partially or full enclosed
		// by this bounding box.
		void getPolys(const core::aabbox3d<f32>& box, SIndexData* idxdata, u32 parentTest ) const
		{
#if defined (OCTREE_PARENTTEST )
			// if not full inside
			if ( parentTest != 2 )
			{
				// partially inside ?
				if (!Box.intersectsWithBox(box))
					return;

				// fully inside ?
				parentTest = Box.isFullInside(box)?2:1;
			}
#else
			if (Box.intersectsWithBox(box))
#endif
			{
				const u32 cnt = IndexData->size();
				u32 i; // new ISO for scoping problem in some compilers

				for (i=0; i!=cnt; ++i)
				{
					s32 idxcnt = (*IndexData)[i].IndexBuffer.getIndexCount();

					if (idxcnt)
					{
						if(idxdata[i].IndexBuffer->getType() != (*IndexData)[i].IndexBuffer.getType())
							continue;

						u32 IndexType = sizeof(s16);
						
						if(idxdata[i].IndexBuffer->getType() == video::EIT_32BIT)
							IndexType = sizeof(s32);

						void* src = (*IndexData)[i].IndexBuffer.getIndices();
						void* dst = idxdata[i].IndexBuffer->getIndices();

						if(idxdata[i].IndexBuffer->getType() == video::EIT_32BIT)
						{
							s32* dst2 = (s32*)dst;
							dst2 += idxdata[i].CurrentSize;
							dst = dst2;
						}
						else
						{
							s16* dst2 = (s16*)dst;
							dst2 += idxdata[i].CurrentSize;
							dst = dst2;
						}

						idxdata[i].CurrentSize += idxcnt;
						idxdata[i].IndexBuffer->set_used(idxdata[i].CurrentSize);

						memcpy(dst, src, idxcnt * IndexType);
					}
				}

				for (i=0; i!=8; ++i)
					if (Children[i])
						Children[i]->getPolys(box, idxdata,parentTest);
			}
		}

		// returns all ids of polygons partially or full enclosed
		// by the view frustum.
		void getPolys(const scene::SViewFrustum& frustum, SIndexData* idxdata,u32 parentTest) const
		{
			u32 i; // new ISO for scoping problem in some compilers

			// if parent is fully inside, no further check for the children is needed
#if defined (OCTREE_PARENTTEST )
			if ( parentTest != 2 )
#endif
			{
#if defined (OCTREE_PARENTTEST )
				parentTest = 2;
#endif
				for (i=0; i!=scene::SViewFrustum::VF_PLANE_COUNT; ++i)
				{
					core::EIntersectionRelation3D r = Box.classifyPlaneRelation(frustum.planes[i]);
					if ( r == core::ISREL3D_FRONT )
						return;
#if defined (OCTREE_PARENTTEST )
					if ( r == core::ISREL3D_CLIPPED )
						parentTest = 1;	// must still check children
#endif
				}
			}

			const u32 cnt = IndexData->size();

			for (i=0; i!=cnt; ++i)
			{
				s32 idxcnt = (*IndexData)[i].IndexBuffer.getIndexCount();

				if (idxcnt)
				{
					if(idxdata[i].IndexBuffer->getType() != (*IndexData)[i].IndexBuffer.getType())
						continue;

					u32 IndexType = sizeof(s16);
						
					if(idxdata[i].IndexBuffer->getType() == video::EIT_32BIT)
						IndexType = sizeof(s32);

					void* src = (*IndexData)[i].IndexBuffer.getIndices();
					void* dst = idxdata[i].IndexBuffer->getIndices();

					if(idxdata[i].IndexBuffer->getType() == video::EIT_32BIT)
					{
						s32* dst2 = (s32*)dst;
						dst2 += idxdata[i].CurrentSize;
						dst = dst2;
					}
					else
					{
						s16* dst2 = (s16*)dst;
						dst2 += idxdata[i].CurrentSize;
						dst = dst2;
					}

					idxdata[i].CurrentSize += idxcnt;
					idxdata[i].IndexBuffer->set_used(idxdata[i].CurrentSize);

					memcpy(dst, src, idxcnt * IndexType);
				}
			}

			for (i=0; i!=8; ++i)
				if (Children[i])
					Children[i]->getPolys(frustum, idxdata,parentTest);
		}

		//! for debug purposes only, collects the bounding boxes of the node
		void getBoundingBoxes(const core::aabbox3d<f32>& box,
			core::array< const core::aabbox3d<f32>* >&outBoxes) const
		{
			if (Box.intersectsWithBox(box))
			{
				outBoxes.push_back(&Box);

				for (u32 i=0; i!=8; ++i)
					if (Children[i])
						Children[i]->getBoundingBoxes(box, outBoxes);
			}
		}

	private:

		core::aabbox3df Box;
		core::array<SIndexChunk>* IndexData;
		OctreeNode* Children[8];
		u32 Depth;
	};

	OctreeNode* Root;
	SIndexData* IndexData;
	u32 IndexDataCount;
	u32 NodeCount;
};

} // end namespace

#endif

