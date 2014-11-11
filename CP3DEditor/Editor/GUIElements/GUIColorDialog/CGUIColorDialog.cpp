
#include "Stdafx.h"
#include <ECP3DGUIEvents.h>

#include "CGUIColorDialog.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;

namespace cp3d {
namespace ui {

CGUIColorDialog::CGUIColorDialog(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
	: ICP3DColorDialog(EGUIET_COLOR_SELECT_DIALOG, environment, parent, id, rectangle),
	  SelectingColor(false), Color(1.0, 1.0, 1.0, 1.0)
{
	ColorPicker = new CGUIColorPicker(environment, environment->getRootGUIElement(), this);
	ColorPicker->setVisible(false);
}

CGUIColorDialog::~CGUIColorDialog() {

}

bool CGUIColorDialog::OnEvent(const SEvent& event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {

			IGUIElement *focus = Environment->getFocus();
			if (focus == this) {
				SelectingColor = !SelectingColor;
				return true;
			}

		}
	}

	return IGUIElement::OnEvent(event);
}

void CGUIColorDialog::draw() {
	/// Draw the element
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	video::IVideoDriver* driver = Environment->getVideoDriver();
	core::rect<s32> viewPort = AbsoluteRect;
	viewPort.LowerRightCorner.X -= 1;
	viewPort.LowerRightCorner.Y -= 1;
	viewPort.UpperLeftCorner.X += 1;
	viewPort.UpperLeftCorner.Y += 1;

	viewPort.clipAgainst(AbsoluteClippingRect);

	core::rect<s32> frameRect(AbsoluteRect);

	/// Draw color rectangle
	skin->draw2DRectangle(this, Color.toSColor(), frameRect, &AbsoluteClippingRect);

	/// Finish drawing
	frameRect.LowerRightCorner.Y = frameRect.UpperLeftCorner.Y + 1;
	skin->draw2DRectangle(this, skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);

	frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	frameRect.LowerRightCorner.X = frameRect.UpperLeftCorner.X + 1;
	skin->draw2DRectangle(this, skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);

	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.X = frameRect.LowerRightCorner.X - 1;
	skin->draw2DRectangle(this, skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);

	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - 1;
	skin->draw2DRectangle(this, skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);

	/// Draw color picking
	if (SelectingColor) {
		updateAbsolutePosition();
		frameRect = rect<s32>(AbsoluteRect);
		ColorPicker->setVisible(true);

		Environment->getRootGUIElement()->bringToFront(ColorPicker);

		vector2di r(frameRect.LowerRightCorner.X, frameRect.UpperLeftCorner.Y);
		const u32 height = frameRect.LowerRightCorner.Y + 256;
		const u32 width = frameRect.LowerRightCorner.X + 512;

		if (height > driver->getScreenSize().Height) {
			r.Y -= height - driver->getScreenSize().Height;
		}
		if (width > driver->getScreenSize().Width) {
			r.X -= width - driver->getScreenSize().Width;
		}

		ColorPicker->setRelativePosition(rect<s32>(r, vector2di(r.X + 512, r.Y + 256)));
	}
	else {
		ColorPicker->setVisible(false);
	}

	IGUIElement::draw();
}

void CGUIColorDialog::setColor(const irr::video::SColorf &color) {
	Color = color;
}

const irr::video::SColorf &CGUIColorDialog::getColor() const {
	return Color;
}

} // end namespace ui
} // end namespace cp3d

namespace cp3d {
namespace ui {

CGUIColorPicker::CGUIColorPicker(IGUIEnvironment *gui, IGUIElement *parent, CGUIColorDialog *dialog)
	: ICP3DColorPicker(gui, parent)
{
	ColorDialog = dialog;

	IVideoDriver *driver = Environment->getVideoDriver();

	ColorTexture = driver->getTexture("CP3D:ColorTexture");
	if (!ColorTexture) {

		IImage *i = driver->createImage(ECF_R8G8B8, dimension2du(512, 256));
		for (u32 y=0; y < 256; y++) {
			for (u32 x=0; x < 512; x++) {
			
				SColor c(255, y, x < 255 ? x : 255, x < 255 ? 0 : x - 255);
				i->setPixel(x, y, c);
			}
		}

		ColorTexture = driver->addTexture("CP3D:ColorTexture", i);
	}
}

CGUIColorPicker::~CGUIColorPicker() {

}

void CGUIColorPicker::draw() {
	if (!IsVisible)
		return;

	/*core::rect<s32> viewPort = AbsoluteRect;
	viewPort.LowerRightCorner.X -= 1;
	viewPort.LowerRightCorner.Y -= 1;
	viewPort.UpperLeftCorner.X += 1;
	viewPort.UpperLeftCorner.Y += 1;
	viewPort.clipAgainst(AbsoluteClippingRect);*/
	core::rect<s32> frameRect(AbsoluteRect);

	Environment->getVideoDriver()->draw2DImage(ColorTexture, frameRect.UpperLeftCorner);
}

bool CGUIColorPicker::OnEvent(const SEvent &event) {
	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {

			updateAbsolutePosition();
			
			const f32 inv = 1.f / 255.f;
			s32 y = event.MouseInput.Y - AbsoluteRect.UpperLeftCorner.Y;
			s32 x = event.MouseInput.X - AbsoluteClippingRect.UpperLeftCorner.X;
			SColor c(255, y, x < 255 ? x : 255, x < 255 ? 0 : x - 255);

			ColorDialog->Color.r = c.getRed() * inv;
			ColorDialog->Color.g = c.getGreen() * inv;
			ColorDialog->Color.b = c.getBlue() * inv;
			Color = c;

			ColorDialog->SelectingColor = false;

			SEvent ev;
			ev.EventType = EET_GUI_EVENT;
			ev.GUIEvent.Caller = ColorDialog;
			ev.GUIEvent.EventType = (EGUI_EVENT_TYPE)ECGET_COLOR_SELECTED;
			ev.GUIEvent.Element = 0;
			return Parent->OnEvent(ev);
		}
	}

	return IGUIElement::OnEvent(event);
}

const irr::video::SColor &CGUIColorPicker::getColor() const {
	return Color;
}

} /// End namespace ui
} /// End namespace cp3d
