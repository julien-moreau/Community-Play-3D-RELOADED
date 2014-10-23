#ifndef _H_ICP3D_SCENE_NODE_CREATOR_INCLUDED__
#define _H_ICP3D_SCENE_NODE_CREATOR_INCLUDED__

#include <irrlicht.h>
#include <functional>

namespace cp3d {
namespace engine {

typedef std::function<void(irr::scene::ISceneNode *node)> ICP3DSceneNodeCreatorCallback;
#define DEFAULT_CALLBACK [&](irr::scene::ISceneNode *node) { }

class ICP3DSceneNodeCreator {
public:

	//! Creates a new cloud scene node
	//! \param translation: 
	//! \param texture: the video texture to apply
	//! \param scale: the cloud scale
	virtual irr::scene::ISceneNode *createCloudNode(const irr::core::vector2df &translation, irr::video::ITexture *texture,
													const irr::f32 textureScale, const irr::f32 centerHeight, const irr::f32 innerHeight,
													const irr::f32 outerHeight, ICP3DSceneNodeCreatorCallback callback = DEFAULT_CALLBACK) = 0;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif