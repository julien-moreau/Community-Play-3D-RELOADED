// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_MESH_MANIPULATOR_H_INCLUDED__
#define __C_MESH_MANIPULATOR_H_INCLUDED__

#include "IMeshManipulator.h"

namespace irr
{
namespace scene
{

class CMeshManipulator : public IMeshManipulator
{
public:
	virtual void flipSurfaces(IMesh* mesh) const;

	virtual void setVertexColor(IMeshBuffer* meshBuffer, video::SColor color, bool onlyAlpha) const;

	virtual void setVertexColorAlpha(IMeshBuffer* meshBuffer, s32 alpha) const;

	virtual void setVertexColors(IMeshBuffer* meshBuffer, video::SColor color) const;

	virtual void scale(IMeshBuffer* meshBuffer, const core::vector3df& factor) const;

	virtual void scaleTCoords(IMeshBuffer* meshBuffer, const core::vector2df& factor, u32 level = 0) const;

	virtual void transform(IMeshBuffer* meshBuffer, const core::matrix4& mat) const;

	virtual void recalculateNormals(IMeshBuffer* meshBuffer, bool smooth, bool angleWeighted) const;

	virtual void recalculateTangents(IMeshBuffer* meshBuffer, bool recalculateNormals, bool smooth, bool angleWeighted) const;

	virtual void makePlanarTextureMapping(IMeshBuffer* meshBuffer, f32 resolution = 0.001f) const;

	virtual void makePlanarTextureMapping(IMeshBuffer* meshBuffer, f32 resolutionS, f32 resolutionT, u8 axis, const core::vector3df& offset) const;

	virtual bool copyIndices(IIndexBuffer* srcBuffer, IIndexBuffer* dstBuffer) const;

	virtual bool copyVertices(IVertexBuffer* srcBuffer, IVertexBuffer* dstBuffer, u32 srcDescriptionBufferID, u32 dstDescriptionBufferID, bool copyCustomAttribute) const;

	virtual bool createTangents(IMeshBuffer* srcBuffer, IMeshBuffer* dstBuffer, bool copyCustomAttribute);

	virtual bool createUniquePrimitives(IMeshBuffer* srcBuffer, IMeshBuffer* dstBuffer) const;

	virtual bool createWelded(IMeshBuffer* srcBuffer, IMeshBuffer* dstBuffer, f32 tolerance = core::ROUNDING_ERROR_f32,
		bool check4Component = true, bool check3Component = true, bool check2Component = true, bool check1Component = true) const;

	virtual s32 getPolyCount(IMesh* mesh) const;

	virtual s32 getPolyCount(IAnimatedMesh* mesh) const;

	virtual IAnimatedMesh* createAnimatedMesh(IMesh* mesh, E_ANIMATED_MESH_TYPE type) const;

	virtual IMesh* createForsythOptimizedMesh(const IMesh* mesh) const;
};

} // end namespace scene
} // end namespace irr


#endif
