#ifndef _H_ICP3D_VIEW_PORT_INCLUDED__
#define _H_ICP3D_VIEW_PORT_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace ui {

class ICP3DViewPort : public irr::gui::IGUIElement {
public:

	//! Constructor
	ICP3DViewPort(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle)
		: irr::gui::IGUIElement(irr::gui::EGUIET_MESH_VIEWER, environment, parent, id, rectangle)
	{ }

	//! Sets the new scene for the view port to be draw
	//! \param scene: the new scene manager
	virtual void setSceneManager(irr::scene::ISceneManager* scene) = 0;

	//! Returns the current scene drawn by the view port
	//! \return: the current scene
	virtual irr::scene::ISceneManager* getSceneManager() const = 0;

	//! Sets the override color of the view port
	//! \param color: the override color
	virtual void setOverrideColor(const irr::video::SColor &color) = 0;

	//! Returns the override color of the view port
	//! \return the override color
	virtual void enableOverrideColor(const bool override) = 0;

};

} /// End namepsace ui
} /// End namespace cp3d

#endif