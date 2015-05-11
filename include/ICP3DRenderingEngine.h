#ifndef _H_ICP3D_RENDERING_ENGINE_INCLUDED__
#define _H_ICP3D_RENDERING_ENGINE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class ICP3DHandler;
class ICP3DMaterialCreator;
class ICP3DHardwareSkinningManager;
class ICP3DEffectsManager;
struct ICP3DLightSceneNode;

/*
Rendering engine interface
This interface let you get all the components of
the rendering engine. Including main rendering handler, video driver,
gui environment, material creators etc.

simply include :
#include <CP3DRenderingEngine.h>

*/
class ICP3DRenderingEngine {
public:

	//! Contains the default materials created by Irrlicht and CP3D
	//! EMT_SOLID:					   CP3D's Default Material
	//! EMT_NORMAL_MAP:				   CP3D's Normal Map Material
	//! EMT_NORMAL_MAP_TRANS_ADD:	   CP3D's Normal Map Material with transparence
	//! EMT_NORMAL_MAP_TRANS_ALPHAREF: CP3D's Normal Map Material with alpha ref
	irr::core::map<irr::video::E_MATERIAL_TYPE, irr::video::E_MATERIAL_TYPE> Materials;

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
	virtual ICP3DLightSceneNode *getLightSceneNode(const irr::u32 &index) = 0;

	//! Returns the light containing the given scene node
	//! \param: node: the irrlicht light scene node
	virtual ICP3DLightSceneNode *getLightSceneNode(irr::scene::ILightSceneNode *node) = 0;

	//! Removes a light scene node from its pointer
	//! \param node: the light scene node to remove
	virtual void removeLightSceneNode(ICP3DLightSceneNode *node) = 0;

	//! Removes a light scene node from its ILightSceneNode pointer
	//! \param node: the light node to remove
	virtual void removeLightSceneNode(irr::scene::ILightSceneNode *node) = 0;

	//! Sets if the light scene node computes shadows or not
	//! \param node: the node to compute
	//! \param compute: true if the light computes shadows
	//! returns the shadow light index to use with ICP3DHandler (-1 if shadow light removed)
	virtual irr::s32 setLightSceneNodeComputeShadows(ICP3DLightSceneNode *node, const bool compute) = 0;
	//! Sets if the light scene node computes shadows or not
	//! \param index: the index of the light scene node
	//! \param compute: true if the light computes shadows
	//! returns the shadow light index to use with ICP3DHandler (-1 if shadow light removed)
	virtual irr::s32 setLightSceneNodeComputeShadows(const irr::u32 index, const bool compute) = 0;

	//! Creates the normal mapping material
	//! Sets the cp3d::rendering::NormalMappingMaterial etc.
	virtual void createNormalMappingMaterial() = 0;

	//! Removes the normal mapping material
	//! Sets the cp3d::rendering::NormalMappingMaterial etc. to default material
	virtual void destroyNormalMappingMaterial() = 0;

	//! Returns the hardware skinning manager
	virtual ICP3DHardwareSkinningManager *getHWSkinningManager() = 0;

	//! Returns the effects manager
	virtual ICP3DEffectsManager *getEffectsManager() = 0;

	//! Clears the scene (handler + lights + scene)
	virtual void clear() = 0;

};

} /// End rendering namespace
} /// End cp3d namespace

#endif
