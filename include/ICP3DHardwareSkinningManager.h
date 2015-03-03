#ifndef _H_ICP3D_HARDWARE_SKINNING_MANAGER_INCLUDED__
#define _H_ICP3D_HARDWARE_SKINNING_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DHandlerCallbacks.h>

namespace cp3d {
namespace rendering {

class ICP3DHardwareSkinningMaterial : public ICP3DHandlerCustomCallback {
public:

	//! Constructor
	ICP3DHardwareSkinningMaterial() : ICP3DHandlerCustomCallback(true, true)
	{ }

	//! Returns the node associated to the material
	virtual irr::scene::IAnimatedMeshSceneNode *getNode() = 0;

	//! Returns the node's mesh has ISkinnedMesh
	virtual irr::scene::ISkinnedMesh *getMesh() = 0;

	//! Returns the Hardware Skinning material
	virtual irr::s32 getMaterial() = 0;

	//! Returns the Hardware skinning material width depth support
	virtual irr::s32 getDepthMaterial() = 0;

	//! Returns the hardware Skinning material with shadows support
	virtual irr::s32 getShadowsMaterial() = 0;
};

class ICP3DHardwareSkinningManager {
public:

	//! Adds a hardware skinning instance to the rendering engine
	//! for the given node
	//! \param node: the node to apply hardware skinning
	virtual bool addNode(irr::scene::IAnimatedMeshSceneNode *node) = 0;

	//! Removes a hardware skinning instance to the rendering engine
	//! for the given node
	//! \param node: the node to remove from instance and delete instance
	virtual bool removeNode(irr::scene::IAnimatedMeshSceneNode *node) = 0;

	//! Returns the Hardware Skinning material for the given node
	//! \param node: the node associated to the returned HW material
	virtual ICP3DHardwareSkinningMaterial *getHWMaterial(irr::scene::IAnimatedMeshSceneNode *node) = 0;

};

} /// End rendering namespace
} /// End cp3d namespace

#endif