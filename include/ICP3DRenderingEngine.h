#ifndef _H_ICP3D_RENDERING_ENGINE_INCLUDED__
#define _H_ICP3D_RENDERING_ENGINE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class ICP3DHandler;
class ICP3DMaterialCreator;
struct ICP3DLightSceneNode;

class ICP3DRenderingEngine {
public:

	irr::video::E_MATERIAL_TYPE NormalMappingMaterialSolid;
	irr::video::E_MATERIAL_TYPE NormalMappingMaterialTransAdd;
	irr::video::E_MATERIAL_TYPE NormalMappingMaterialTransAlphaRef;

	//! Returns the Irrlicht device
	virtual irr::IrrlichtDevice *getDevice() = 0;

	//! Returns the video driver
	virtual irr::video::IVideoDriver *getVideoDriver() = 0;

	//! Returns the scene manager
	virtual irr::scene::ISceneManager *getSceneManager() = 0;

	//! Returns the gui environment
	virtual irr::gui::IGUIEnvironment *getGUIEnvironment() = 0;

	//! Returns the main handler that computes shadows, post-process,
	//! custom passes, etc.
	virtual ICP3DHandler *getHandler() = 0;

	//! Creates a new material creator
	virtual ICP3DMaterialCreator *createMaterialCreator() = 0;

	//! Creates a new light scene node and returns its pointer
	virtual ICP3DLightSceneNode *createLightSceneNode(const bool computeNormalMapping = false, const bool computeShadows = false) = 0;

	//! Returns the amount of lights
	virtual const irr::u32 getLightSceneNodeCount() const = 0;

	//! Returns light scene node at index "index"
	//! \param index: the index of the light
	virtual ICP3DLightSceneNode *getLightSceneNode(const irr::u32 index) = 0;

	//! Creates the normal mapping material
	//! Sets the cp3d::rendering::NormalMappingMaterial etc.
	virtual void createNormalMappingMaterial() = 0;

	//! Removes the normal mapping material
	//! Sets the cp3d::rendering::NormalMappingMaterial etc. to default material
	virtual void destroyNormalMappingMaterial() = 0;

};

} /// End rendering namespace
} /// End cp3d namespace

#endif
