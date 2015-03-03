// Copyright (C) 2012 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_VERTEX_DESCRIPTOR_H_INCLUDED__
#define __C_VERTEX_DESCRIPTOR_H_INCLUDED__

#include "IVertexDescriptor.h"

namespace irr
{
namespace video
{
	class CVertexAttribute : public IVertexAttribute
	{
	public:
		CVertexAttribute(const core::stringc& name, u32 elementCount, E_VERTEX_ATTRIBUTE_SEMANTIC semantic, E_VERTEX_ATTRIBUTE_TYPE type, u32 offset, u32 bufferID);
		virtual ~CVertexAttribute();

		bool operator < (const CVertexAttribute& other) const;

		virtual const core::stringc& getName() const;

		virtual u32 getElementCount() const;

		virtual E_VERTEX_ATTRIBUTE_SEMANTIC getSemantic() const;

		virtual E_VERTEX_ATTRIBUTE_TYPE getType() const;

		virtual u32 getTypeSize() const;

		virtual u32 getOffset() const;

		virtual void setOffset(u32 offset);

		virtual u32 getBufferID() const;

	protected:
		core::stringc Name;

		u32 ElementCount;

		E_VERTEX_ATTRIBUTE_SEMANTIC Semantic;

		E_VERTEX_ATTRIBUTE_TYPE Type;

		u32 TypeSize;

		u32 Offset;

		u32 BufferID;
	};

	class CVertexDescriptor : public IVertexDescriptor
	{
	public:
		CVertexDescriptor(const core::stringc& name, u32 id);
		virtual ~CVertexDescriptor();

		virtual u32 getID() const;

		virtual void setID(u32 id);

		virtual const core::stringc& getName() const;

		virtual u32 getVertexSize(u32 bufferID) const;

		virtual E_INSTANCE_DATA_STEP_RATE getInstanceDataStepRate(u32 bufferID) const;

		virtual void setInstanceDataStepRate(E_INSTANCE_DATA_STEP_RATE rate, u32 bufferID);

		virtual bool addAttribute(const core::stringc& name, u32 elementCount, E_VERTEX_ATTRIBUTE_SEMANTIC semantic, E_VERTEX_ATTRIBUTE_TYPE type, u32 bufferID);

		virtual IVertexAttribute* getAttribute(u32 id) const;

		virtual IVertexAttribute* getAttributeByName(const core::stringc& name) const;

		virtual IVertexAttribute* getAttributeBySemantic(E_VERTEX_ATTRIBUTE_SEMANTIC semantic) const;

		virtual u32 getAttributeCount() const;

		virtual bool removeAttribute(u32 id);

		virtual void removeAllAttribute();

	protected:
		u32 ID;

		core::stringc Name;

		core::array<u32> VertexSize;
		core::array<E_INSTANCE_DATA_STEP_RATE> InstanceDataStepRate;
		core::array<CVertexAttribute*> Attribute;

		s32 AttributePointer[(u32)EVAS_COUNT];
	};

}
}

#endif
