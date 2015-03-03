// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CWaterSurfaceSceneNode.h"
#include "ISceneManager.h"
#include "IMeshManipulator.h"
#include "IMeshCache.h"
#include "S3DVertex.h"
#include "SMesh.h"
#include "os.h"
#include "IVideoDriver.h"

namespace irr
{
namespace scene
{

//! constructor
CWaterSurfaceSceneNode::CWaterSurfaceSceneNode(f32 waveHeight, f32 waveSpeed, f32 waveLength,
		IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position, const core::vector3df& rotation,
		const core::vector3df& scale)
	: CMeshSceneNode(mesh, parent, mgr, id, position, rotation, scale),
	WaveLength(waveLength), WaveSpeed(waveSpeed), WaveHeight(waveHeight),
	OriginalMesh(0)
{
	#ifdef _DEBUG
	setDebugName("CWaterSurfaceSceneNode");
	#endif

	setMesh(mesh);
}


//! destructor
CWaterSurfaceSceneNode::~CWaterSurfaceSceneNode()
{
	// Mesh is dropped in CMeshSceneNode destructor
	if (OriginalMesh)
		OriginalMesh->drop();
}


//! frame
void CWaterSurfaceSceneNode::OnRegisterSceneNode()
{
	CMeshSceneNode::OnRegisterSceneNode();
}


void CWaterSurfaceSceneNode::OnAnimate(u32 timeMs)
{
	if (Mesh && IsVisible)
	{
		const u32 meshBufferCount = Mesh->getMeshBufferCount();
		const f32 time = timeMs / WaveSpeed;

		for (u32 b=0; b<meshBufferCount; ++b)
		{
			video::IVertexAttribute* attribute = Mesh->getMeshBuffer(b)->getVertexDescriptor()->getAttributeBySemantic(video::EVAS_POSITION);
			video::IVertexAttribute* attributeOriginal = OriginalMesh->getMeshBuffer(b)->getVertexDescriptor()->getAttributeBySemantic(video::EVAS_POSITION);

			if(!attribute || !attributeOriginal)
				continue;

			u8* offset = static_cast<u8*>(Mesh->getMeshBuffer(b)->getVertexBuffer()->getVertices());
			offset += attribute->getOffset();

			u8* offsetOriginal = static_cast<u8*>(OriginalMesh->getMeshBuffer(b)->getVertexBuffer()->getVertices());
			offsetOriginal += attributeOriginal->getOffset();

			u32 vtxCnt = Mesh->getMeshBuffer(b)->getVertexBuffer()->getVertexCount();

			if(OriginalMesh->getMeshBuffer(b)->getVertexBuffer()->getVertexCount() < vtxCnt)
				vtxCnt = OriginalMesh->getMeshBuffer(b)->getVertexBuffer()->getVertexCount();

			for (u32 i=0; i<vtxCnt; ++i)
			{
				core::vector3df* position = (core::vector3df*)offset;
				core::vector3df* positionOriginal = (core::vector3df*)offsetOriginal;

				(*position).Y = addWave(*positionOriginal, time);

				offset += Mesh->getMeshBuffer(b)->getVertexBuffer()->getVertexSize();
				offsetOriginal += OriginalMesh->getMeshBuffer(b)->getVertexBuffer()->getVertexSize();
			}
		}// end for all mesh buffers
		Mesh->setDirty(scene::EBT_VERTEX);

		SceneManager->getMeshManipulator()->recalculateNormals(Mesh);
	}
	CMeshSceneNode::OnAnimate(timeMs);
}


void CWaterSurfaceSceneNode::setMesh(IMesh* mesh)
{
	CMeshSceneNode::setMesh(mesh);
	if (!mesh)
		return;
	if (OriginalMesh)
		OriginalMesh->drop();
	IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy<video::S3DVertex>(mesh,
		SceneManager->getVideoDriver()->getVertexDescriptor(0));
	OriginalMesh = mesh;
	Mesh = clone;
	Mesh->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_INDEX);
//	Mesh->setHardwareMappingHint(scene::EHM_STREAM, scene::EBT_VERTEX);
}


//! Writes attributes of the scene node.
void CWaterSurfaceSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addFloat("WaveLength", WaveLength);
	out->addFloat("WaveSpeed",  WaveSpeed);
	out->addFloat("WaveHeight", WaveHeight);

	CMeshSceneNode::serializeAttributes(out, options);
	// serialize original mesh
	out->setAttribute("Mesh", SceneManager->getMeshCache()->getMeshName(OriginalMesh).getPath().c_str());
}


//! Reads attributes of the scene node.
void CWaterSurfaceSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	WaveLength = in->getAttributeAsFloat("WaveLength");
	WaveSpeed  = in->getAttributeAsFloat("WaveSpeed");
	WaveHeight = in->getAttributeAsFloat("WaveHeight");

	if (Mesh)
	{
		Mesh->drop();
		Mesh = OriginalMesh;
		OriginalMesh = 0;
	}
	// deserialize original mesh
	CMeshSceneNode::deserializeAttributes(in, options);

	if (Mesh)
	{
		IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy<video::S3DVertex>(Mesh,
			SceneManager->getVideoDriver()->getVertexDescriptor(0));
		OriginalMesh = Mesh;
		Mesh = clone;
	}
}


f32 CWaterSurfaceSceneNode::addWave(const core::vector3df &source, f32 time) const
{
	return source.Y +
		(sinf(((source.X/WaveLength) + time)) * WaveHeight) +
		(cosf(((source.Z/WaveLength) + time)) * WaveHeight);
}

} // end namespace scene
} // end namespace irr

