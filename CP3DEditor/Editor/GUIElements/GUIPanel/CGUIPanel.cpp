
#include "Stdafx.h"

#include "CGUIPanel.h"

using namespace irr;
using namespace gui;

namespace cp3d {
namespace ui {

CGUIPanel::CGUIPanel(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
	: IGUIElement(EGUIET_ELEMENT, environment, parent, id, rectangle)
{
	ScrollBar = Environment->addScrollBar(false, rectangle, this, -1);
	ScrollBar->setSmallStep(1);
	ScrollBar->setLargeStep(1);
	ScrollBar->setPos(0);
	ScrollBar->setMax(0);
	setRelativePosition(rectangle);

	LastScrollBarPosition = 0;
}

CGUIPanel::~CGUIPanel()
{

}

bool CGUIPanel::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED && event.GUIEvent.Caller == ScrollBar) {

			const s32 newPos = ScrollBar->getPos();
			const s32 offset = core::abs_<s32>(LastScrollBarPosition - newPos);

			core::list<IGUIElement *>::ConstIterator it = Children.begin();
			for (; it != Children.end(); ++it) {
				if (*it == ScrollBar)
					continue;

				core::vector2di pos;
				pos.X = (*it)->getRelativePosition().UpperLeftCorner.X;

				if (newPos > LastScrollBarPosition) {
					pos.Y = (*it)->getRelativePosition().UpperLeftCorner.Y - offset;
				}
				else {
					pos.Y = (*it)->getRelativePosition().UpperLeftCorner.Y + offset;
				}

				(*it)->setRelativePosition(pos);

			}

			LastScrollBarPosition = newPos;
		}
	}

	return IGUIElement::OnEvent(event);
}

void CGUIPanel::setRelativePosition(const irr::core::rect<irr::s32> &r) {
	/// Resize scrollbar
	const s32 size = Environment->getSkin()->getSize(EGDS_SCROLLBAR_SIZE);

	core::rect<s32> position;
	position.UpperLeftCorner.X = r.getWidth() - size;
	position.UpperLeftCorner.Y = r.UpperLeftCorner.Y;
	position.LowerRightCorner.X = r.getWidth();
	position.LowerRightCorner.Y = r.getHeight();

	ScrollBar->setRelativePosition(position);

	IGUIElement::setRelativePosition(r);

	/// Recalculate scrollbar
	recalculateScrollBar();
}

void CGUIPanel::recalculateScrollBar() {
	core::list<IGUIElement *>::ConstIterator it = Children.begin();
	s32 maxOffset = (*it)->getRelativePosition().LowerRightCorner.Y;

	for (; it != Children.end(); ++it) {
		s32 pos = (*it)->getRelativePosition().LowerRightCorner.Y;
		if (pos > maxOffset)
			maxOffset = pos;
	}

	if (maxOffset > RelativeRect.getHeight())
		ScrollBar->setMax(maxOffset - RelativeRect.getHeight());
	else
		ScrollBar->setMax(0);
}

void CGUIPanel::draw()
{
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

	IGUIElement::draw();
}

} // end namespace ui
} // end namespace cp3d


