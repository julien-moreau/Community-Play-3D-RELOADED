#ifndef _H_ICP3D_COLOR_DIALOG_INCLUDED__
#define _H_ICP3D_COLOR_DIALOG_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace ui {

class ICP3DColorDialog : public irr::gui::IGUIElement {
public:

	//! Constructor
	ICP3DColorDialog(irr::gui::EGUI_ELEMENT_TYPE type, irr::gui::IGUIEnvironment* gui,
					 irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle)
		: irr::gui::IGUIElement(type, gui, parent, id, rectangle)
	{ }

	//! Destructor
	virtual ~ICP3DColorDialog()
	{ }

	//! Sets the new color of the color dialog
	//! \param color: the color to set
	virtual void setColor(const irr::video::SColorf &color) = 0;

	//! Returns the selected color
	virtual const irr::video::SColorf &getColor() const = 0;

};

class ICP3DColorPicker : public irr::gui::IGUIElement {
public:

	//! Constructor
	ICP3DColorPicker(irr::gui::IGUIEnvironment *gui, irr::gui::IGUIElement *parent)
		: irr::gui::IGUIElement(irr::gui::EGUIET_COLOR_SELECT_DIALOG, gui, parent, -1, irr::core::rect<irr::s32>())
	{ }

	//! Destructor
	virtual ~ICP3DColorPicker()
	{ }

	//! Returns the selected color
	virtual const irr::video::SColorf &getColor() const = 0;

};

} /// End ui namespace
} /// End cp3d namespace

#endif
