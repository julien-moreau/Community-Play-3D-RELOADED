#ifndef _H_ICP3D_SCENE_NODE_CREATOR_INCLUDED__
#define _H_ICP3D_SCENE_NODE_CREATOR_INCLUDED__

#include <irrlicht.h>
#include <functional>

namespace cp3d {
namespace engine {

typedef std::function<void(irr::scene::ISceneNode *node)> ICP3DSceneNodeCreatorCallback;
#define ICP3D_SN_CREATOR_DEFAULT_CB [&](irr::scene::ISceneNode *node) { }

class ICP3DSceneNodeCreator {
public:

	//!----------------------------------------------------------------
	//! SCENE NODES
	//!----------------------------------------------------------------

	//! Creates a new cloud scene node
	//! \param translation: cloud's translation
	//! \param texture: the video texture to apply
	//! \param scale: the cloud scale
	virtual irr::scene::ISceneNode *createCloudNode(const irr::core::vector2df &translation, irr::video::ITexture *texture,
													const irr::f32 textureScale, const irr::f32 centerHeight, const irr::f32 innerHeight,
													const irr::f32 outerHeight, ICP3DSceneNodeCreatorCallback callback = ICP3D_SN_CREATOR_DEFAULT_CB) = 0;

	//! Creates a cube scene node
	//! \param size: the cube's size
	//! \param callback: callback called after scene node was created
	virtual irr::scene::ISceneNode *createCubeSceneNode(const irr::f32 size, ICP3DSceneNodeCreatorCallback callback = ICP3D_SN_CREATOR_DEFAULT_CB) = 0;

	//! Configures scene node to default values
	//! \param node: the node to configure
	virtual void configureSceneNode(irr::scene::ISceneNode *node) = 0;

	//!----------------------------------------------------------------
	//! MESHES
	//!----------------------------------------------------------------

	//! Loads and returns a configured and optimized mesh
	//! \param path: the mesh path on file system
	//! \param withTangents: true if the configurator recalculates the mesh with tangents (irr::video::S3DVertexTangent)
	virtual irr::scene::IMesh *getStaticMesh(irr::core::stringc path, bool withTangents = false) = 0;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
