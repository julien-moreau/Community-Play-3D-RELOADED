#ifndef _H_ICP3D_HANDLER_INCLUDED__
#define _H_ICP3D_HANDLER_INCLUDED__

#include <functional>

#include <irrlicht.h>
#include "SShadowLight.h"
#include "CScreenQuad.h"
#include "ICP3DHandlerCallbacks.h"
#include "ICP3DHandlerPostProcesses.h"

namespace cp3d {
namespace rendering {

//! Shadow mode. If the nodes should receive, cast or exclude
enum E_SHADOW_MODE {
	ESM_RECEIVE = 0,
	ESM_CAST,
	ESM_BOTH,
	ESM_EXCLUDE,
	ESM_NO_SHADOW,

	ESM_COUNT
};
const irr::c8* const ShadowModeNames[] = {
	"Receive",
	"Cast",
	"Both (Cast & Receive)",
	"Exclude",
	0
};

//! Shadow map filter type
enum E_FILTER_TYPE {
	EFT_NONE = 0,
	EFT_4PCF,
	EFT_8PCF,
	EFT_12PCF,
	EFT_16PCF,
	EFT_COUNT
};
const irr::c8* const FilterTypeNames[] = {
	"None",
	"4 PCF",
	"8 PCF",
	"12 PCF",
	"16 PCF",
	0
};

class ICP3DCustomPass;
class ICP3DCustomDepthPass;

//! ICP3DHandler class interface
class ICP3DHandler : public ICP3DHandlerPostProcesses {

public:

	//! Draw call. Updates the scene using custom passes, shadows, filters...
	//! \param output: The output texture. 0 if backbuffer
	virtual void update(irr::video::ITexture *output = 0) = 0;

	//! Returns the shadow lights count
	virtual const irr::u32 getShadowLightCount() const = 0;

	//! Returns the shadow map of a light
	//! \param resolution: the resolution of the shadow map
	//! \param secondary: if the returns shadow map was blurred
	virtual irr::video::ITexture* getShadowMapTexture(const irr::u32 resolution, const bool secondary = false) = 0;

	//! Removes shadows from a node.
	//! \param node: the node you want to remove from lighting calculations
	virtual void removeShadowFromNode(irr::scene::ISceneNode* node) = 0;

	//! Adds shadows from a node
	//! \param node: the to compute in lighting calculations
	//! \param filterType: the shadow map filter type
	//! \param shadowMode: the shadow mode for this node. Cast, Receive, Both or Exclude
	//! \param depthMaterial: the custom depth material. -1 for default depth material
	//! \param shadowsMaterial: the custom shadows material. -1 for default shadows material
	//! \param customCallback: the custom callback for custom depth & shadows materials
	virtual void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE, E_SHADOW_MODE shadowMode = ESM_BOTH, 
								 irr::s32 depthMaterial = -1, irr::s32 shadowsMaterial = -1,
								 ICP3DHandlerCustomCallback *customCallback = 0) = 0;

	//! Returns if the node is shadowed or not
	//! \param node: the node to test
	virtual bool isNodeShadowed(irr::scene::ISceneNode *node) = 0;

	//! Sets if the handler renders shadows
	//! \param render: true if the handler should render shadows. False if not
	virtual void setRenderShadows(const bool render) = 0;

	//! Returns if the handler is rendering shadows
	virtual const bool isRenderingShadows() = 0;

	//! Sets the ambiant color of the scene
	//! \param ambiantColour: the ambiant color of the scene
	virtual void setAmbientColor(irr::video::SColor ambientColour) = 0;

	//! Add a custom pass (see cp3d::rendering::ICP3DCustomPass)
	//! Allow you to add custom passes like a depth pass, custom object-based depth-of-field etc.
	//! \param pass: the custom pass derived from cp3d::rendering::ICP3DCustomPass
	virtual void addCustomPass(ICP3DCustomPass *pass) = 0;

	//! Remove a custom pass (see cp3d::rendering::ICP3DCustomPass)
	virtual void removeCustomPass(ICP3DCustomPass *pass) = 0;

	//! Returns custom pass count.
	virtual const irr::u32 getCustomPassCount() const = 0;

	//! Returns a custom pass
	//! \param index: the index of the custom pass
	virtual ICP3DCustomPass *getCustomPass(const irr::u32 index) = 0;

	//! Returns the screen quad used for rendering (especially post-processes)
	virtual CScreenQuad& getScreenQuad() = 0;
	//! Returns the screen quad's pointer
	virtual CScreenQuad *getScreenQuadPtr() = 0;

	//! Returns the post-process id
	//! \param id: can be the post-process material id in MaterialRenderers, or the direct ID [0, x];
	virtual const irr::s32 getPostProcessID(const irr::s32 &id) = 0;

	//! Returns the post-process id by its name
	//! \param name: the name of the post-process (path)
	//! \return: the post-process id
	virtual const irr::s32 getPostProcessIDByName(const irr::core::stringc &name) = 0;

	//! Sets if the the given post-process is activated or not
	//! \param id: the material type id to activate or deactivate
	//! \param activated: activate or deactivate the post-process
	virtual void setPostProcessActivated(const irr::s32 &id, const bool activated) = 0;
	//! Returns if the post-process is activated
	//! \param id: the material type id to check
	//! \return: true if post-process is activated, false if deactivated
	virtual bool isPostProcessActivated(const irr::s32 &id) = 0;

	//! Returns the amount of post-processes
	//! \return: the amount of post-processes
	virtual const irr::u32 getPostProcessingRoutineSize() const = 0;

	//! Returns the post-processing name
	//! \return: the post-process name for the given material type (material type can be the direct ID [0, x])
	virtual const irr::core::stringc getPostProcessingRoutineName(const irr::s32 &materialType) = 0;

	//! Returns the custom depth pass manager
	//! Allows you to add custom depth passes using multiple render targets
	virtual ICP3DCustomDepthPass *getDepthPassManager() = 0;

	//! Returns the general pass manager
	//! It computes the normal pass, light scattering pass
	/*
	For light scattering pass :
		if node is ESNT_BILLBOARD then the node is rendered normal
		else the node is totally black
	For the normal pass :
		the normal texture is the second texture TextureLayer[1]
	*/
	virtual ICP3DCustomPass *getGeneralPassManager() = 0;

	//! Generates a random texutre that can be used for SSAO post-prcess etc
	//! \param dimensions: the texutre's dimensions
	//! \param name: the name of the texture
	virtual irr::video::ITexture* generateRandomVectorTexture(const irr::core::dimension2du& dimensions, const irr::core::stringc& name = "randVec") = 0;

	//! Resets the render targets to the given resolution
	//! \param resolution: the new resolution of the render targets
	virtual void setScreenRenderTargetResolution(const irr::core::dimension2du& resolution) = 0;

	//! Sets the view port where to render the scene
	//! \param viewPort: the new view port
	virtual void setViewPort(const irr::core::rect<irr::s32> viewPort) = 0;

	//! Returns the current viewport of the handler
	virtual irr::core::rect<irr::s32> getViewPort() = 0;

	//! Returns the shadow mode for the given node
	//! \param node: the node to test
	virtual E_SHADOW_MODE getShadowModeForNode(irr::scene::ISceneNode *node) = 0;

	//! Sets the new shadow mode for the node
	//! \param node: the node to modify
	//! \param shadowMode: the shadow mode
	virtual void setShadowModeForNode(irr::scene::ISceneNode *node, E_SHADOW_MODE shadowMode) = 0;

	//! Returns the filter type for the given node
	//! \param node: the node to test
	virtual E_FILTER_TYPE getFilterTypeForNode(irr::scene::ISceneNode *node) = 0;

	//! Sets the new filter type for the scene node
	//! \param: the node to modify
	//! \param filterType: the filter type to set
	virtual void setFilterTypeForNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType) = 0;
};

} /// End rendering namespace
} /// End cp3d namespace

#endif
