// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_MESH_BUFFER_H_INCLUDED__
#define __I_MESH_BUFFER_H_INCLUDED__

#include "IReferenceCounted.h"
#include "SMaterial.h"
#include "aabbox3d.h"
#include "S3DVertex.h"
#include "IVertexDescriptor.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "EHardwareBufferFlags.h"
#include "EPrimitiveTypes.h"

namespace irr
{
namespace scene
{
	//! Struct for holding a mesh with a single material.
	/** A part of an IMesh which has the same material on each face of that
	group. Logical groups of an IMesh need not be put into separate mesh
	buffers, but can be. Separately animated parts of the mesh must be put
	into separate mesh buffers.
	Some mesh buffer implementations have limitations on the number of
	vertices the buffer can hold. In that case, logical grouping can help.
	Moreover, the number of vertices should be optimized for the GPU upload,
	which often depends on the type of gfx card. Typial figures are
	1000-10000 vertices per buffer.
	SMeshBuffer is a simple implementation of a MeshBuffer, which supports
	up to 65535 vertices.

	Since meshbuffers are used for drawing, and hence will be exposed
	to the driver, chances are high that they are grab()'ed from somewhere.
	It's therefore required to dynamically allocate meshbuffers which are
	passed to a video driver and only drop the buffer once it's not used in
	the current code block anymore.
	*/
	class IMeshBuffer : public virtual IReferenceCounted
	{
	public:
		IMeshBuffer() : VertexBufferCompatible(true)
		{
		}

		virtual ~IMeshBuffer() {}

		virtual bool addVertexBuffer(IVertexBuffer* vertexBuffer) = 0;

		virtual IVertexBuffer* getVertexBuffer(u32 id = 0) const = 0;

		virtual u32 getVertexBufferCount() const = 0;

		virtual void removeVertexBuffer(u32 id) = 0;

		virtual bool setVertexBuffer(IVertexBuffer* vertexBuffer, u32 id = 0) = 0;

		virtual IIndexBuffer* getIndexBuffer() const = 0;

		virtual bool setIndexBuffer(IIndexBuffer* indexBuffer) = 0;

		//! Inform if stored vertex buffers have the same vertex descriptors.
		bool isVertexBufferCompatible() const
		{
			return VertexBufferCompatible;
		}

		//! Get the material of this meshbuffer
		/** \return Material of this buffer. */
		virtual video::SMaterial& getMaterial() = 0;

		//! Get the material of this meshbuffer
		/** \return Material of this buffer. */
		virtual const video::SMaterial& getMaterial() const = 0;

		//! Get the axis aligned bounding box of this meshbuffer.
		/** \return Axis aligned bounding box of this buffer. */
		virtual const core::aabbox3df& getBoundingBox() const = 0;

		//! Set axis aligned bounding box
		/** \param box User defined axis aligned bounding box to use
		for this buffer. */
		virtual void setBoundingBox(const core::aabbox3df& pBox) = 0;

		//! Call this after changing the positions of any vertex.
		virtual void boundingBoxNeedsRecalculated() = 0;

		//! Recalculates the bounding box. Should be called if the mesh changed.
		virtual void recalculateBoundingBox() = 0;

		virtual void append(IVertexBuffer* vertexBuffer, IIndexBuffer* indexBuffer, u32 id = 0) = 0;

		//! Append the meshbuffer to the current buffer
		/** Only works for compatible vertex types
		\param meshBuffer Buffer to append to this one. */
		virtual void append(IMeshBuffer* meshBuffer) = 0;

		//! get the current hardware mapping hint
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Vertex(u32 id = 0) const = 0;

		//! get the current hardware mapping hint
		virtual E_HARDWARE_MAPPING getHardwareMappingHint_Index() const = 0;

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING pMappingHint, E_BUFFER_TYPE type = EBT_VERTEX_AND_INDEX, u32 id = 0) = 0;

		//! flags the meshbuffer as changed, reloads hardware buffers
		virtual void setDirty(E_BUFFER_TYPE type = EBT_VERTEX_AND_INDEX, u32 id = 0) = 0;

		//! Get the currently used ID for identification of changes.
		/** This shouldn't be used for anything outside the VideoDriver. */
		virtual u32 getChangedID_Vertex(u32 id = 0) const = 0;

		//! Get the currently used ID for identification of changes.
		/** This shouldn't be used for anything outside the VideoDriver. */
		virtual u32 getChangedID_Index() const = 0;

		virtual core::matrix4& getTransformation() = 0;

	protected:
		bool VertexBufferCompatible;
	};

} // end namespace scene
} // end namespace irr

#endif


