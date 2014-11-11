
#ifndef __H_C_GUI_COLOR_DIALOG_INCLUDED__
#define __H_C_GUI_COLOR_DIALOG_INCLUDED__

#include <irrlicht.h>
#include <ICP3DColorDialog.h>

namespace cp3d {
namespace ui {

class CGUIColorPicker;

class CGUIColorDialog : public ICP3DColorDialog {
	friend class CGUIColorPicker;
public:

	/// Constructor & Destructor
	CGUIColorDialog(irr::gui::IGUIEnvironment* gui, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle);
	~CGUIColorDialog();

	/// Inheritance
	bool OnEvent(const irr::SEvent& event);
	//! Draws the element and its children
	void draw();

	/// ICP3DColorDialog
	void setColor(const irr::video::SColorf &color);
	const irr::video::SColorf &getColor() const;

private:
	/// Datas
	irr::video::SColorf Color;
	bool SelectingColor;

	CGUIColorPicker *ColorPicker;
};


class CGUIColorPicker : public ICP3DColorPicker {
public:

	/// Constructor & Destructor
	CGUIColorPicker(irr::gui::IGUIEnvironment *gui, irr::gui::IGUIElement *parent, CGUIColorDialog *dialog);
	~CGUIColorPicker();

	/// Inheritance
	bool OnEvent(const irr::SEvent &event);
	/// Draws the element and its children
	void draw();

	const irr::video::SColor &getColor() const;

private:
	/// Datas
	irr::video::SColor Color;
	irr::video::ITexture *ColorTexture;

	CGUIColorDialog *ColorDialog;

};


} // end namespace ui
} // end namespace irr

#endif
