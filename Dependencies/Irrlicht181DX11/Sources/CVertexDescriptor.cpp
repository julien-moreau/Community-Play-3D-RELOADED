// Copyright (C) 2012 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CVertexDescriptor.h"

namespace irr
{
namespace video
{

	CVertexAttribute::CVertexAttribute(const core::stringc& name, u32 elementCount, E_VERTEX_ATTRIBUTE_SEMANTIC semantic, E_VERTEX_ATTRIBUTE_TYPE type, u32 offset, u32 bufferID) :
		Name(name), ElementCount(elementCount), Semantic(semantic), Type(type), Offset(offset), BufferID(bufferID)
	{
		switch(Type)
		{
		case EVAT_BYTE:
		case EVAT_UBYTE:
			TypeSize = sizeof(u8);
			break;
		case EVAT_SHORT:
		case EVAT_USHORT:
			TypeSize = sizeof(u16);
			break;
		case EVAT_INT:
		case EVAT_UINT:
			TypeSize = sizeof(u32);
			break;
		case EVAT_FLOAT:
			TypeSize = sizeof(f32);
			break;
		case EVAT_DOUBLE:
			TypeSize = sizeof(f64);
			break;
		}
	}

	CVertexAttribute::~CVertexAttribute()
	{
	}

	bool CVertexAttribute::operator < (const CVertexAttribute& other) const
	{
		return BufferID > other.BufferID;
	}

	const core::stringc& CVertexAttribute::getName() const
	{
		return Name;
	}

	u32 CVertexAttribute::getElementCount() const
	{
		return ElementCount;
	}

	E_VERTEX_ATTRIBUTE_SEMANTIC CVertexAttribute::getSemantic() const
	{
		return Semantic;
	}

	E_VERTEX_ATTRIBUTE_TYPE CVertexAttribute::getType() const
	{
		return Type;
	}

	u32 CVertexAttribute::getTypeSize() const
	{
		return TypeSize;
	}

	u32 CVertexAttribute::getOffset() const
	{
		return Offset;
	}

	void CVertexAttribute::setOffset(u32 offset)
	{
		Offset = offset;
	}

	u32 CVertexAttribute::getBufferID() const
	{
		return BufferID;
	}

	CVertexDescriptor::CVertexDescriptor(const core::stringc& name, u32 id) : ID(id), Name(name)
	{
#ifdef _DEBUG
		setDebugName("CVertexDescriptor");
#endif

		for (u32 i = 0; i < EVAS_COUNT; ++i)
			AttributePointer[i] = -1;
	}

	CVertexDescriptor::~CVertexDescriptor()
	{
		for (u32 i = 0; i < Attribute.size(); ++i)
			delete Attribute[i];
	}

	u32 CVertexDescriptor::getID() const
	{
		return ID;
	}

	void CVertexDescriptor::setID(u32 id)
	{
		ID = id;
	}

	const core::stringc& CVertexDescriptor::getName() const
	{
		return Name;
	}

	u32 CVertexDescriptor::getVertexSize(u32 bufferID) const
	{
		u32 size = 0;

		if (bufferID < VertexSize.size())
			size = VertexSize[bufferID];

		return size;
	}

	E_INSTANCE_DATA_STEP_RATE CVertexDescriptor::getInstanceDataStepRate(u32 bufferID) const
	{
		E_INSTANCE_DATA_STEP_RATE Rate = EIDSR_PER_VERTEX;

		if (bufferID < InstanceDataStepRate.size())
			Rate = InstanceDataStepRate[bufferID];

		return Rate;
	}

	void CVertexDescriptor::setInstanceDataStepRate(E_INSTANCE_DATA_STEP_RATE rate, u32 bufferID)
	{
		if (bufferID < InstanceDataStepRate.size())
			InstanceDataStepRate[bufferID] = rate;
	}

	bool CVertexDescriptor::addAttribute(const core::stringc& name, u32 elementCount, E_VERTEX_ATTRIBUTE_SEMANTIC semantic, E_VERTEX_ATTRIBUTE_TYPE type, u32 bufferID)
	{
		for (u32 i = 0; i < Attribute.size(); ++i)
			if (name == Attribute[i]->getName() || (semantic != EVAS_CUSTOM && semantic == Attribute[i]->getSemantic()))
				return false;

		if (elementCount < 1)
			elementCount = 1;

		if (elementCount > 4)
			elementCount = 4;

		for (u32 i = VertexSize.size(); i <= bufferID; ++i)
			VertexSize.push_back(0);

		for (u32 i = InstanceDataStepRate.size(); i <= bufferID; ++i)
			InstanceDataStepRate.push_back(EIDSR_PER_VERTEX);

		CVertexAttribute* attribute = new CVertexAttribute(name, elementCount, semantic, type, VertexSize[bufferID], bufferID);
		Attribute.push_back(attribute);

		AttributePointer[(u32)attribute->getSemantic()] = Attribute.size()-1;

		VertexSize[bufferID] += (attribute->getTypeSize() * attribute->getElementCount());

		return true;
	}

	IVertexAttribute* CVertexDescriptor::getAttribute(u32 id) const
	{
		if (id < Attribute.size())
			return Attribute[id];

		return 0;
	}

	IVertexAttribute* CVertexDescriptor::getAttributeByName(const core::stringc& name) const
	{
		for (u32 i = 0; i < Attribute.size(); ++i)
			if (name == Attribute[i]->getName())
				return Attribute[i];

		return 0;
	}

	IVertexAttribute* CVertexDescriptor::getAttributeBySemantic(E_VERTEX_ATTRIBUTE_SEMANTIC semantic) const
	{
		s32 ID = AttributePointer[(u32)semantic];

		if (ID >= 0)
			return Attribute[ID];

		return 0;
	}

	u32 CVertexDescriptor::getAttributeCount() const
	{
		return Attribute.size();
	}

	bool CVertexDescriptor::removeAttribute(u32 id)
	{
		if (id < Attribute.size())
		{
			const u32 bufferID = Attribute[id]->getBufferID();

			AttributePointer[(u32)Attribute[id]->getSemantic()] = -1;
			Attribute.erase(id);

			// Recalculate vertex size and attribute offsets.

			VertexSize[bufferID] = 0;

			for (u32 i = 0; i < Attribute.size(); ++i)
			{
				if (Attribute[i]->getBufferID() == bufferID)
				{
					Attribute[i]->setOffset(VertexSize[bufferID]);
					VertexSize[bufferID] += (Attribute[id]->getTypeSize() * Attribute[id]->getElementCount());
				}
			}

			return true;
		}

		return false;		
	}

	void CVertexDescriptor::removeAllAttribute()
	{
		for (u32 i = 0; i < Attribute.size(); ++i)
			delete Attribute[i];

		Attribute.clear();
		VertexSize.clear();

		for(u32 i = 0; i < EVAS_COUNT; ++i)
			AttributePointer[i] = -1;
	}
}
}
