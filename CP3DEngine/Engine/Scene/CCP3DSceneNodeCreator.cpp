
#include "stdafx.h"
#include "CCP3DSceneNodeCreator.h"

#include <ICP3DRenderingEngine.h>
#include <ICP3DHandler.h>
#include <ICP3DCustomPass.h>

#include "Clouds/CCloudSceneNode.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

namespace cp3d {
namespace engine {

CCP3DSceneNodeCreator::CCP3DSceneNodeCreator(rendering::ICP3DRenderingEngine *renderingEngine) : Rengine(renderingEngine)
{
	Device = Rengine->getDevice();
	Driver = Rengine->getVideoDriver();
	Smgr = Rengine->getSceneManager();

	EmptyTexture = Driver->getTexture("Textures/empty.jpg");
}

CCP3DSceneNodeCreator::~CCP3DSceneNodeCreator() {

}

//!--------------------------------------------------------------------------------------------------------------------------------------------
//! SCENE NODES
//!--------------------------------------------------------------------------------------------------------------------------------------------

/// Creates a cloud scene node
irr::scene::ISceneNode *CCP3DSceneNodeCreator::createCloudNode(const vector2df &translation, ITexture *texture,
															   const f32 textureScale, const f32 centerHeight, const f32 innerHeight,
															   const f32 outerHeight, ICP3DSceneNodeCreatorCallback callback)
{
	CCloudSceneNode *cloud = new CCloudSceneNode(Smgr->getRootSceneNode(), Smgr);
	cloud->setTranslation(translation);
	cloud->getMaterial(0).setTexture(0, texture);
	cloud->setCloudHeight(centerHeight, innerHeight, outerHeight);
	cloud->setTextureScale(textureScale);
	cloud->setName("Cloud Layer 1");

	configureSceneNode(cloud);
	callback(cloud);

	return cloud;
}

/// Creates a cube scene node
irr::scene::ISceneNode *CCP3DSceneNodeCreator::createCubeSceneNode(const f32 size, ICP3DSceneNodeCreatorCallback callback) {
	ISceneNode *cube = Smgr->addCubeSceneNode(size);

	configureSceneNode(cube);
	callback(cube);

	return cube;
}

/// Configures scene node to default values
void CCP3DSceneNodeCreator::configureSceneNode(irr::scene::ISceneNode *node) {
	if (!node)
		return;

	node->setMaterialFlag(EMF_LIGHTING, false);

	ESCENE_NODE_TYPE type = node->getType();
	IMeshManipulator *manipulator = Smgr->getMeshManipulator();
	IMeshCache *cache = Smgr->getMeshCache();
	IMesh *m = 0;
	
	if ((type == ESNT_MESH || type == ESNT_OCTREE || type == ESNT_CUBE ||
		type == ESNT_SPHERE) && !Smgr->getParameters()->getAttributeAsBool(scene::IRR_SCENE_MANAGER_IS_EDITOR))
	{
		IMeshSceneNode *n = (IMeshSceneNode *)node;
		m = manipulator->createForsythOptimizedMesh(n->getMesh());

		cache->removeMesh(n->getMesh());
		n->setMesh(m);
	}
	else if (type == ESNT_TERRAIN) {
		ITerrainSceneNode *n = (ITerrainSceneNode *)node;
		m = n->getMesh();
	}
	else if (type == ESNT_ANIMATED_MESH) {
		IAnimatedMeshSceneNode *n = (IAnimatedMeshSceneNode *)node;
		m = n->getMesh();
	}

	if (m) {
		manipulator->recalculateNormals(m, true, true);
	}

	for (u32 i=0; i < node->getMaterialCount(); i++) {
		node->getMaterial(i).PolygonOffsetDirection = EPO_FRONT;
		node->getMaterial(i).PolygonOffsetFactor = 0;

		for (u32 j=0; j < video::MATERIAL_MAX_TEXTURES; j++) {
			if (node->getMaterial(i).TextureLayer[j].Texture == 0) {
				node->getMaterial(i).TextureLayer[j].Texture = EmptyTexture;
			}
		}
	}
}

void CCP3DSceneNodeCreator::removeSceneNode(irr::scene::ISceneNode *node, rendering::ICP3DRenderingEngine *renderingEngine) {
	rendering::ICP3DHandler *handler = renderingEngine->getHandler();

	if (node->getType() != ESNT_LIGHT) {
		for (u32 i = 0; i < handler->getCustomPassCount(); i++)
			handler->getCustomPass(i)->removeNodeFromPass(node);

		handler->removeShadowFromNode(node);
		node->remove();
	}
	else {
		renderingEngine->removeLightSceneNode((ILightSceneNode *)node);
	}
}

//!--------------------------------------------------------------------------------------------------------------------------------------------
//! MESHES
//!--------------------------------------------------------------------------------------------------------------------------------------------

IMesh *CCP3DSceneNodeCreator::getStaticMesh(stringc path, bool withTangents) {
	/// Load the mesh
	IMesh *mesh = Smgr->getMesh(path.c_str());
	if (!mesh)
		return 0;

	/// Recalculate with tangents if needed
	if (withTangents) {
		#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
		//IMesh *meshT = Smgr->getMeshManipulator()->createMeshWithTangents(mesh, true, true, true, true);
		IMesh *meshT = mesh;
		#else
		IMesh *meshT = Smgr->getMeshManipulator()->createMeshWithTangents(mesh, true, true, true, true);
		#endif

		mesh->drop();
		mesh = meshT;
	}

	/// Configure mesh
	mesh->setHardwareMappingHint(EHM_STATIC, EBT_VERTEX_AND_INDEX);

	return mesh;
}

} /// End namespace engine
} /// End namespace cp3d
