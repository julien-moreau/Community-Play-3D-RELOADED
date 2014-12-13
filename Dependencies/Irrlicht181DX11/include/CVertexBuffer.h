// Copyright (C) 2012 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_VERTEX_BUFFER_H_INCLUDED__
#define __C_VERTEX_BUFFER_H_INCLUDED__

#include "S3DVertex.h"
#include "IVertexBuffer.h"

namespace irr
{
namespace scene
{
	template <class T>
	class CVertexBuffer : public IVertexBuffer
	{
	public:
		CVertexBuffer(video::IVertexDescriptor* vertexDescriptor) : Vertices(0), vertexDescriptor(vertexDescriptor), HardwareMappingHint(EHM_NEVER), ChangedID(1)
		{
#ifdef _DEBUG
			setDebugName("CVertexBuffer");
#endif
			if (vertexDescriptor)
				vertexDescriptor->grab();
		}

		CVertexBuffer(const CVertexBuffer& vertexBuffer) : Vertices(0), ChangedID(1)
		{
			vertexDescriptor = vertexBuffer.getVertexDescriptor();

			if (vertexDescriptor)
				vertexDescriptor->grab();

			HardwareMappingHint = vertexBuffer.getHardwareMappingHint();

			Vertices.reallocate(vertexBuffer.getVertexCount());

			for (u32 i = 0; i < vertexBuffer.getVertexCount(); ++i)
				Vertices.push_back(vertexBuffer.getVertex(i));
		}

		virtual ~CVertexBuffer()
		{
			Vertices.clear();

			if (vertexDescriptor)
				vertexDescriptor->drop();
		}

		virtual void clear()
		{
			Vertices.clear();
		}

		virtual T& getLast()
		{
			return Vertices.getLast();
		}

		virtual void set_used(u32 used)
		{
			Vertices.set_used(used);
		}

		virtual void reallocate(u32 size)
		{
			Vertices.reallocate(size);
		}

		virtual u32 allocated_size() const
		{
			return Vertices.allocated_size();
		}

		virtual s32 linear_reverse_search(const T& element) const
		{
			return Vertices.linear_reverse_search(element);
		}

		virtual s32 linear_reverse_search(const void* element) const
		{
			T* Element = (T*)element;
			return Vertices.linear_reverse_search(*Element);
		}

		virtual void fill(u32 used)
		{
			Vertices.reallocate(used);

			while (Vertices.size() < used)
			{
				T element;
				Vertices.push_back(element);
			}
		}

		virtual video::IVertexDescriptor* getVertexDescriptor() const
		{
			return vertexDescriptor;
		}

		virtual bool setVertexDescriptor(video::IVertexDescriptor* vtxDescriptor)
		{
			if (vtxDescriptor && vtxDescriptor != vertexDescriptor)
			{
				if (vertexDescriptor)
					vertexDescriptor->drop();

				vertexDescriptor = vtxDescriptor;
				vertexDescriptor->grab();

				return true;
			}

			return false;
		}

		virtual E_HARDWARE_MAPPING getHardwareMappingHint() const
		{
			return HardwareMappingHint;
		}

		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING hardwareMappingHint)
		{
			if (HardwareMappingHint != hardwareMappingHint)
				setDirty();

			HardwareMappingHint = hardwareMappingHint;
		}

		virtual void addVertex(const T& vertex)
		{
			Vertices.push_back(vertex);
		}

		virtual void addVertex(const void* vertex)
		{
			T* vtx = (T*)vertex;
			Vertices.push_back(*vtx);
		}

		virtual const void* getVertex(u32 id) const
		{
			if (id < Vertices.size())
				return &Vertices[id];

			return 0;
		}

		virtual void* getVertices()
		{
			return Vertices.pointer();
		}

		virtual u32 getVertexCount() const
		{
			return Vertices.size();
		}

		virtual video::E_VERTEX_TYPE getVertexType() const
		{
			return vertexDescriptor ? (video::E_VERTEX_TYPE)vertexDescriptor->getID() : (video::E_VERTEX_TYPE) - 1;
		}

		virtual u32 getVertexSize() const
		{
			return sizeof(T);
		}

		virtual void setVertex(u32 id, const void* vertex)
		{
			if (id < Vertices.size())
			{
				T* vtx = (T*)vertex;
				Vertices[id] = *vtx;
			}
		}

		virtual void setDirty()
		{
			++ChangedID;
		}

		virtual u32 getChangedID() const
		{
			return ChangedID;
		}

	protected:
		core::array<T> Vertices;

		video::IVertexDescriptor* vertexDescriptor;

		E_HARDWARE_MAPPING HardwareMappingHint;

		u32 ChangedID;
	};

	typedef CVertexBuffer<video::S3DVertex> SVertexBuffer;
	typedef CVertexBuffer<video::S3DVertex2TCoords> SVertexBufferLightMap;
	typedef CVertexBuffer<video::S3DVertexTangents> SVertexBufferTangents;
}
}

#endif
