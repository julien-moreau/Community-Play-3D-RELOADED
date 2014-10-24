
#ifndef __H_C_CP3D_SCENE_NODE_CREATOR_INCLUDED__
#define __H_C_CP3D_SCENE_NODE_CREATOR_INCLUDED__

#include <irrlicht.h>
#include <ICP3DSceneNodeCreator.h>

namespace cp3d {

	namespace rendering {
		class ICP3DRenderingEngine;
	}

namespace engine {

/*
Class that creates scene nodes
This class works like a factory and make you able to create
scene node quickly. It auto-configures the created scene nodes
and can be called with arguments to facilitate the default parameters
of each node in the scene
*/
class CCP3DSceneNodeCreator : public ICP3DSceneNodeCreator {
public:

	CCP3DSceneNodeCreator(rendering::ICP3DRenderingEngine *renderingEngine);
	~CCP3DSceneNodeCreator();

	irr::scene::ISceneNode *createCloudNode(const irr::core::vector2df &translation, irr::video::ITexture *texture,
											const irr::f32 textureScale, const irr::f32 centerHeight, const irr::f32 innerHeight,
											const irr::f32 outerHeight, ICP3DSceneNodeCreatorCallback callback = ICP3D_SN_CREATOR_DEFAULT_CB);

private:
	/// Irrlicht
	irr::IrrlichtDevice *Device;
	irr::video::IVideoDriver *Driver;
	irr::scene::ISceneManager *Smgr;

	/// Rendering engine
	rendering::ICP3DRenderingEngine *Rengine;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
