#ifndef _H_ICP3D_CUSTOM_PASS_INCLUDED__
#define _H_ICP3D_CUSTOM_PASS_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class ICP3DCustomPass {
	friend class CCP3DHandler;
public:

	//! Constructor
	ICP3DCustomPass(irr::video::IVideoDriver *driver, irr::core::stringc name) : Driver(driver), MaterialType(-1),
		Enabled(true), Name(name), CurrentSceneNode(-1)
	{ }

	//! Sets the appropriate render target
	/* Usage :
	driver->setRenderTarget(myRenderTarget, true, true, SColor(0x0)); /// for a simple render target

	or

	irr::core::array<irr::video::IRenderTarget> rtargets;
	rtargets.push_back(myRenderTarget1);
	rtargets.push_back(myRenderTarget2);
	driver->setRenderTarget(rtargets, true, true, SColor(0x0)); /// for a multiple render targets
	*/
	virtual bool setRenderTarget() = 0;

	//! Pre-render callback
	//! \param node: the node going to be rendered
	virtual void onPreRender(irr::scene::ISceneNode *node) { }

	//! Post-render callback
	//! \param node: the node going to be rendered
	virtual void onPostRender(irr::scene::ISceneNode *node) { }

	//! Returns the custom pass material type
	virtual const irr::s32 getMaterialType() const {
		return MaterialType;
	}

	//! Returns if the pass is enabled
	virtual const bool isEnabled() const {
		return Enabled;
	}

	//! Set the pass enabled or not
	//! \param enabled: enable or disable the pass. True if enabled
	virtual void setEnabled(const bool enabled) {
		Enabled = enabled;
	}

	//! Adds a node to the pass
	//! \param node: the node to add to the pass
	virtual void addNodeToPass(irr::scene::ISceneNode *node) {
		SceneNodes.push_back(node);
	}

	//! Removes a node from the pass
	//! \param node: the node to remove from the pass
	virtual void removeNodeFromPass(irr::scene::ISceneNode *node) {
		irr::s32 index = SceneNodes.binary_search(node);
		if (index != -1)
			SceneNodes.erase(index);
	}

	//! Returns the array of scene nodes rendering by the pass
	inline const irr::core::array<irr::scene::ISceneNode *> &getSceneNodes() const {
		return SceneNodes;
	}

protected:
	irr::video::IVideoDriver *Driver;

	bool Enabled;
	irr::core::stringc Name;
	irr::s32 MaterialType;
	irr::core::array<irr::scene::ISceneNode *> SceneNodes;

	irr::u32 CurrentSceneNode;
};

} /// End rendering namespace
} /// End cp3d namespace

#endif