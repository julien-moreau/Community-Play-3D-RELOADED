#ifndef _H_ICP3D_HANDLER_INCLUDED__
#define _H_ICP3D_HANDLER_INCLUDED__

#include <irrlicht.h>
#include "SShadowLight.h"

namespace cp3d {
namespace rendering {

	//! Shadow mode. If the nodes should receive, cast or exclude
	enum E_SHADOW_MODE {
		ESM_RECEIVE = 0,
		ESM_CAST,
		ESM_BOTH,
		ESM_EXCLUDE,
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

	class ICP3DHandler;
	class ICP3DCustomPass;
	class ICP3DCustomDepthPass;

	//! Custom post-processing render callback
	class IPostProcessingRenderCallback {
	public:
		//! on pre-render callback
		//! \param effect: the handler rendering the post-process
		virtual void OnPreRender(ICP3DHandler* handler) = 0;

		//! on post-render callback
		//! \param effect: the handler rendering the post-process
		virtual void OnPostRender(ICP3DHandler* handler) = 0;
	};

	class ICP3DHandler {

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
		virtual void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE, E_SHADOW_MODE shadowMode = ESM_BOTH) = 0;

		//! Returns if the node is shadowed or not
		//! \param node: the node to test
		virtual bool isNodeShadowed(irr::scene::ISceneNode *node) = 0;

		//! Sets the ambiant color of the scene
		//! \param ambiantColour: the ambiant color of the scene
		virtual void setAmbientColor(irr::video::SColor ambientColour) = 0;

		//! Add a custom pass (see cp3d::rendering::ICP3DCustomPass)
		//! Allow you to add custom passes like a depth pass, custom object-based depth-of-field etc.
		//! \param pass: the custom pass derived from cp3d::rendering::ICP3DCustomPass
		virtual void addCustomPass(ICP3DCustomPass *pass) = 0;

		//! Remove a custom pass (see cp3d::rendering::ICP3DCustomPass)
		virtual void removeCustomPass(ICP3DCustomPass *pass) = 0;

		//! Adds a post-process from a file
		//! \param filename: the file containing the shader's code
		//! \param callback: the callback derived from cp3d::rendering::IPostProcessingRenderCallback
		virtual irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc &filename, IPostProcessingRenderCallback *callback = 0) = 0;

		//! Adds a post-process from a string
		//! \param shader: the string containing the shader's code
		//! \param callback: the callback derived from cp3d::rendering::IPostProcessingRenderCallback
		virtual irr::s32 addPostProcessingEffectFromString(const irr::core::stringc &shader, IPostProcessingRenderCallback *callback = 0) = 0;

		//! Sets a post processing effect constant (OnSetConstant)
		//! \param materialType: the material to configure
		//! \param name: the name of the constant
		//! \param data: the constant's data
		//! \param count: the amount of floats in *data
		virtual void setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, const irr::f32* data, const irr::u32 count) = 0;

		//! Adds a custom UserMapSampler texture
		//! \param userTexture: the texture to set at UserMapSampler
		virtual void setPostProcessingUserTexture(irr::video::ITexture* userTexture) = 0;

		//! Sets a custom texture at index "index"
		//! \param index: the index of the texture layer (0 to MATERIAL_MAX_TEXTURES)
		//! \param texture: the texture to set
		virtual void setPostProcessingTextureAtIndex(irr::u32 index, irr::video::ITexture *texture) = 0;

		//! Returns the custom depth pass manager
		//! Allows you to add custom depth passes using multiple render targets
		virtual ICP3DCustomDepthPass *getDepthPassManager() = 0;

		//! Generates a random texutre that can be used for SSAO post-prcess etc
		//! \param dimensions: the texutre's dimensions
		//! \param name: the name of the texture
		virtual irr::video::ITexture* generateRandomVectorTexture(const irr::core::dimension2du& dimensions, const irr::core::stringc& name = "randVec") = 0;

		//! Resets the render targets to the given resolution
		//! \param resolution: the new resolution of the render targets
		virtual void setScreenRenderTargetResolution(const irr::core::dimension2du& resolution) = 0;

		//! Sets the view port where to render the scene
		//! \param viewPort: the new view port
		virtual void setViewPort(irr::core::rect<irr::s32> viewPort) = 0;

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