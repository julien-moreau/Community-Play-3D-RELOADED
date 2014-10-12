#ifndef _H_ICP3D_HANDLER_INCLUDED__
#define _H_ICP3D_HANDLER_INCLUDED__

#include <irrlicht.h>
#include <SShadowLight.h>

namespace cp3d {

namespace rendering {

	//! Shadow mode. If the nodes should receive, cast or exclude
	enum E_SHADOW_MODE {
		ESM_RECEIVE,
		ESM_CAST,
		ESM_BOTH,
		ESM_EXCLUDE,
		ESM_COUNT
	};

	//! Shadow map filter type
	enum E_FILTER_TYPE {
		EFT_NONE,
		EFT_4PCF,
		EFT_8PCF,
		EFT_12PCF,
		EFT_16PCF,
		EFT_COUNT
	};

	class ICP3DHandler {

	public:

		//! Draw call. Updates the scene using custom passes, shadows, filters...
		//! \param output: The output texture. 0 if backbuffer
		virtual void update(irr::video::ITexture *output = 0) = 0;

		//! Adds a shadow light to compute.
		//! \param shadowLight: the SShadowLight to add
		virtual void addShadowLight(const SShadowLight& shadowLight) = 0;

		//! Returns the shadow light at index "index"
		//! \param index: the index of the shadow light
		virtual SShadowLight& getShadowLight(irr::u32 index) = 0;

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

		//! Sets the ambiant color of the scene
		//! \param ambiantColour: the ambiant color of the scene
		virtual void setAmbientColor(irr::video::SColor ambientColour) = 0;

	};

} /// End rendering namespace
} /// End cp3d namespace

#endif