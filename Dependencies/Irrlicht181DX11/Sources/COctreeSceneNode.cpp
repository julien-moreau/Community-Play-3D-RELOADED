// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "COctreeSceneNode.h"
#include "Octree.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "ICameraSceneNode.h"
#include "IMeshCache.h"
#include "IAnimatedMesh.h"
#include "IMaterialRenderer.h"
#include "IMeshManipulator.h"
#include "os.h"
#include "CShadowVolumeSceneNode.h"
#include "EProfileIDs.h"
#include "IProfiler.h"

namespace irr
{
namespace scene
{


//! constructor
COctreeSceneNode::COctreeSceneNode(const core::array<scene::IMeshBuffer*>& meshes, ISceneNode* parent,
					ISceneManager* mgr, s32 id, s32 minimalPolysPerNode)
	: IMeshSceneNode(parent, mgr, id), StdOctree(0), MinimalPolysPerNode(minimalPolysPerNode), Mesh(0), Shadow(0),
	UseVBOs(OCTREE_USE_HARDWARE), UseVisibilityAndVBOs(OCTREE_USE_VISIBILITY),
	BoxBased(OCTREE_BOX_BASED)
{
#ifdef _DEBUG
	setDebugName("COctreeSceneNode");
#endif

	IRR_PROFILE(
		static bool initProfile = false;
		if (!initProfile )
		{
			initProfile = true;
			getProfiler().add(EPID_OC_RENDER, L"render octnode", L"Irrlicht scene");
			getProfiler().add(EPID_OC_CALCPOLYS, L"calc octnode", L"Irrlicht scene");
		}
 	)
	
	StdMeshes = meshes;
}


//! destructor
COctreeSceneNode::~COctreeSceneNode()
{
	if (Shadow)
		Shadow->drop();
	deleteTree();

	for(u32 i = 0; i < StdMeshes.size(); ++i)
		StdMeshes[i]->drop();

	StdMeshes.clear();
}


void COctreeSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		// because this node supports rendering of mixed mode meshes consisting of
		// transparent and solid material at the same time, we need to go through all
		// materials, check of what type they are and register this node for the right
		// render pass according to that.

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		PassCount = 0;
		u32 transparentCount = 0;
		u32 solidCount = 0;

		// count transparent and solid materials in this scene node
		for (u32 i=0; i<Materials.size(); ++i)
		{
			const video::IMaterialRenderer* const rnd =
				driver->getMaterialRenderer(Materials[i].MaterialType);

			if ((rnd && rnd->isTransparent()) || Materials[i].isTransparent())
				++transparentCount;
			else
				++solidCount;

			if (solidCount && transparentCount)
				break;
		}

		// register according to material types counted

		if (solidCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);

		if (transparentCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

		ISceneNode::OnRegisterSceneNode();
	}
}


//! renders the node.
void COctreeSceneNode::render()
{
	IRR_PROFILE(CProfileScope psRender(EPID_OC_RENDER);)
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!driver)
		return;

	ICameraSceneNode* camera = SceneManager->getActiveCamera();
	if (!camera)
		return;

	bool isTransparentPass =
		SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;
	++PassCount;

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	if (Shadow)
		Shadow->updateShadowVolumes();

	SViewFrustum frust = *camera->getViewFrustum();

	//transform the frustum to the current absolute transformation
	if ( !AbsoluteTransformation.isIdentity() )
	{
		core::matrix4 invTrans(AbsoluteTransformation, core::matrix4::EM4CONST_INVERSE);
		frust.transform(invTrans);
	}

	const core::aabbox3d<float> &box = frust.getBoundingBox();

	IRR_PROFILE(getProfiler().start(EPID_OC_CALCPOLYS));
	
	if (BoxBased)
		StdOctree->calculatePolys(box);
	else
		StdOctree->calculatePolys(frust);
		
	IRR_PROFILE(getProfiler().stop(EPID_OC_CALCPOLYS));

	const Octree::SIndexData* d = StdOctree->getIndexData();

	for (u32 i=0; i<Materials.size(); ++i)
	{
		if ( 0 == d[i].CurrentSize )
			continue;

		const video::IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
		const bool transparent = (rnd && rnd->isTransparent());

		// only render transparent buffer if this is the transparent render pass
		// and solid only in solid pass
		if (transparent == isTransparentPass)
		{
			driver->setMaterial(Materials[i]);

			if (!UseVBOs || (UseVBOs && UseVisibilityAndVBOs))
			{
				scene::IIndexBuffer* oldBuffer = StdMeshes[i]->getIndexBuffer();
				oldBuffer->grab();

				StdMeshes[i]->setIndexBuffer(d[i].IndexBuffer);
				StdMeshes[i]->setDirty(scene::EBT_INDEX);

				driver->drawMeshBuffer ( StdMeshes[i] );

				StdMeshes[i]->setIndexBuffer(oldBuffer);
				oldBuffer->drop();

				StdMeshes[i]->setDirty(scene::EBT_INDEX);
			}
			else
			{
				driver->drawMeshBuffer ( StdMeshes[i] );
			}
		}
	}

	// for debug purposes only
	if (DebugDataVisible && !Materials.empty() && PassCount==1)
	{
		const core::aabbox3d<float> &box = frust.getBoundingBox();
		core::array< const core::aabbox3d<f32>* > boxes;
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		if ( DebugDataVisible & scene::EDS_BBOX_BUFFERS )
		{
			StdOctree->getBoundingBoxes(box, boxes);
			for (u32 b=0; b<boxes.size(); ++b)
				driver->draw3DBox(*boxes[b]);
		}

		if ( DebugDataVisible & scene::EDS_BBOX )
			driver->draw3DBox(Box,video::SColor(0,255,0,0));
	}
}


//! Removes a child from this scene node.
//! Implemented here, to be able to remove the shadow properly, if there is one,
//! or to remove attached childs.
bool COctreeSceneNode::removeChild(ISceneNode* child)
{
	if (child && Shadow == child)
	{
		Shadow->drop();
		Shadow = 0;
	}

	return ISceneNode::removeChild(child);
}


//! Creates shadow volume scene node as child of this node
//! and returns a pointer to it.
IShadowVolumeSceneNode* COctreeSceneNode::addShadowVolumeSceneNode(
		const IMesh* shadowMesh, s32 id, bool zfailmethod, f32 infinity)
{
	if (!SceneManager->getVideoDriver()->queryFeature(video::EVDF_STENCIL_BUFFER))
		return 0;

	if (!shadowMesh)
		shadowMesh = Mesh; // if null is given, use the mesh of node

	if (Shadow)
		Shadow->drop();

	Shadow = new CShadowVolumeSceneNode(shadowMesh, this, SceneManager, id,  zfailmethod, infinity);
	return Shadow;
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& COctreeSceneNode::getBoundingBox() const
{
	return Box;
}


//! creates the tree
/* This method has a lot of duplication and overhead. Moreover, the tangents mesh conversion does not really work. I think we need a a proper mesh implementation for octrees, which handle all vertex types internally. Converting all structures to just one vertex type is always problematic.
Thanks to Auria for fixing major parts of this method. */
bool COctreeSceneNode::createTree(IMesh* mesh)
{
	if (!mesh)
		return false;

	MeshName = SceneManager->getMeshCache()->getMeshName(mesh);

    mesh->grab();
	deleteTree();

	Mesh = mesh;

	const u32 beginTime = os::Timer::getRealTime();

	u32 nodeCount = 0;
	u32 polyCount = 0;
	u32 i;

	Box = mesh->getBoundingBox();

	if (mesh->getMeshBufferCount())
	{
		// check for "larger" buffer types
		u32 meshReserve = StdMeshes.size();

		Materials.reallocate(Materials.size()+meshReserve);
		StdMeshesMatID.reallocate(meshReserve);

		for ( i=0; i < mesh->getMeshBufferCount(); ++i)
		{
			const scene::IMeshManipulator* meshManipulator = SceneManager->getMeshManipulator();

			IMeshBuffer* meshBuffer = mesh->getMeshBuffer(i);
			IMeshBuffer* nchunk = StdMeshes[i];

			// copy vertices

			video::IVertexDescriptor* srcDescriptor = meshBuffer->getVertexDescriptor();
			video::IVertexDescriptor* dstDescriptor = nchunk->getVertexDescriptor();
			const u32 vbCount = meshBuffer->getVertexBufferCount();

			for (u32 j = 0; j < vbCount; ++j)
				meshManipulator->copyVertices(meshBuffer->getVertexBuffer(j), j, srcDescriptor, nchunk->getVertexBuffer(j), j, dstDescriptor, true);

			// copy indices

			scene::IIndexBuffer* srcIndexBuffer = meshBuffer->getIndexBuffer();
			scene::IIndexBuffer* dstIndexBuffer = nchunk->getIndexBuffer();
			meshManipulator->copyIndices(srcIndexBuffer, dstIndexBuffer);

			// copy material

			Materials.push_back(meshBuffer->getMaterial());
			StdMeshesMatID.push_back(Materials.size() - 1);

			// others

			polyCount += dstIndexBuffer->getIndexCount();

			if (UseVBOs)
			{
				if (UseVisibilityAndVBOs)
				{
					nchunk->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX);
					nchunk->setHardwareMappingHint(scene::EHM_DYNAMIC, scene::EBT_INDEX);
				}
				else
					nchunk->setHardwareMappingHint(scene::EHM_STATIC);
			}
			else
			{
				nchunk->setHardwareMappingHint(scene::EHM_NEVER);
			}
		}

		StdOctree = new Octree(StdMeshes, StdMeshesMatID, MinimalPolysPerNode);
		nodeCount = StdOctree->getNodeCount();
	}

	const u32 endTime = os::Timer::getRealTime();
	c8 tmp[255];
	sprintf(tmp, "Needed %ums to create Octree SceneNode.(%u nodes, %u polys)",
		endTime - beginTime, nodeCount, polyCount/3);
	os::Printer::log(tmp, ELL_INFORMATION);

	return true;
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& COctreeSceneNode::getMaterial(u32 i)
{
	if ( i >= Materials.size() )
		return ISceneNode::getMaterial(i);

	return Materials[i];
}


//! returns amount of materials used by this scene node.
u32 COctreeSceneNode::getMaterialCount() const
{
	return Materials.size();
}


//! Writes attributes of the scene node.
void COctreeSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addInt("MinimalPolysPerNode", MinimalPolysPerNode);
	out->addString("Mesh", MeshName.c_str());
}


//! Reads attributes of the scene node.
void COctreeSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	const s32 oldMinimal = MinimalPolysPerNode;

	MinimalPolysPerNode = in->getAttributeAsInt("MinimalPolysPerNode");
	io::path newMeshStr = in->getAttributeAsString("Mesh");

	IMesh* newMesh = 0;

	if (newMeshStr == "")
		newMeshStr = MeshName;

	IAnimatedMesh* newAnimatedMesh = SceneManager->getMesh(newMeshStr.c_str());

	if (newAnimatedMesh)
		newMesh = newAnimatedMesh->getMesh(0);

	if (newMesh && ((MeshName != newMeshStr) || (MinimalPolysPerNode != oldMinimal)))
	{
		// recalculate tree
		createTree(newMesh);
	}

	ISceneNode::deserializeAttributes(in, options);
}


void COctreeSceneNode::deleteTree()
{
	if(StdOctree)
		delete StdOctree;

	StdOctree = 0;
	StdMeshesMatID.clear();

	Materials.clear();

	if(Mesh)
		Mesh->drop();
}

void COctreeSceneNode::setMesh(IMesh* mesh)
{
	createTree(mesh);
}

IMesh* COctreeSceneNode::getMesh(void)
{
	return Mesh;
}

void COctreeSceneNode::setReadOnlyMaterials(bool readonly)
{
	// Do nothing
}

bool COctreeSceneNode::isReadOnlyMaterials() const
{
	return false;
}


} // end namespace scene
} // end namespace irr

