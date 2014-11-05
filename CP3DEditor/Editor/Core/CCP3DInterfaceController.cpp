
#include "stdafx.h"

#include <ICP3DInterface.h>

#include "CCP3DEditorCore.h"
#include "CCP3DInterfaceController.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DInterfaceController::CCP3DInterfaceController(CCP3DEditorCore *editorCore) : MouseHeld(false), SelectedNode(0)
{
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
	editorCore->getEngine()->getCustomUpdater()->addCustomUpdate(this);

	Gui = editorCore->getDevice()->getGUIEnvironment();
	CursorControl = editorCore->getDevice()->getCursorControl();

	ScreenSize = Gui->getVideoDriver()->getScreenSize();
}

CCP3DInterfaceController::~CCP3DInterfaceController() {

}

bool CCP3DInterfaceController::addElement(ICP3DInterface *element, const SControlDescriptor &descriptor) {
	if (Parameters.find(element))
		return false;

	Parameters[element] = descriptor;
	return true;
}

bool CCP3DInterfaceController::addElement(ICP3DInterface *element, irr::s32 flags) {
	return addElement(element, SControlDescriptor(flags, 200, 400));
}

bool CCP3DInterfaceController::removeElement(ICP3DInterface *element) {
	if (!Parameters.find(element))
		return false;

	Parameters.remove(element);
	return true;
}

void CCP3DInterfaceController::OnPreUpdate() {
	bool resize = false;
	if (Gui->getVideoDriver()->getScreenSize() != ScreenSize) {
		resize = true;
		ScreenSize = Gui->getVideoDriver()->getScreenSize();
	}

	vector2di cpos = CursorControl->getPosition();
	CursorControl->setActiveIcon(ECI_NORMAL);

	/// Check for icon
	core::map<ICP3DInterface *, SControlDescriptor>::ConstIterator it = Parameters.getIterator();
	for (; !it.atEnd(); it++) {
		if (resize)
			it.getNode()->getKey()->OnResize();

		IGUIElement *element = it.getNode()->getKey()->getElementToResize();
		const s32 flags = it.getNode()->getValue().Flags;

		/// Check right side
		if ((flags & EICC_RIGHT) != 0) {
			const s32 x = element->getRelativePosition().LowerRightCorner.X;
			checkForIcon(cpos.X, x, ECI_SIZEWE);
		}
		/// Check for left side
		if ((flags & EICC_LEFT) != 0) {
			const s32 x = element->getRelativePosition().UpperLeftCorner.X;
			checkForIcon(cpos.X, x, ECI_SIZEWE);
		}
		/// Check for top side
		if ((flags & EICC_TOP) != 0) {
			const s32 y = element->getRelativePosition().UpperLeftCorner.Y;
			checkForIcon(cpos.Y, y, ECI_SIZENS);
		}
		/// Check for bottom side
		if ((flags & EICC_BOTTOM) != 0) {
			const s32 y = element->getRelativePosition().LowerRightCorner.Y;
			checkForIcon(cpos.Y, y, ECI_SIZENS);
		}
	}

	/// Resize window
	if (MouseHeld) {
		rect<s32> position = SelectedNode->getKey()->getElementToResize()->getRelativePosition();
		IGUIElement *element = SelectedNode->getKey()->getElementToResize();
		const SControlDescriptor descriptor = SelectedNode->getValue();

		if (SelectedCheck == EICC_RIGHT) {
			if (position.getWidth() > descriptor.MinWidth && position.getWidth() < descriptor.MaxWidth)
				position.LowerRightCorner.X = cpos.X;
		}
		else if (SelectedCheck == EICC_LEFT) {
			if (position.getWidth() > descriptor.MinWidth && position.getWidth() < descriptor.MaxWidth)
				position.UpperLeftCorner.X = cpos.X;
		}
		else if (SelectedCheck == EICC_TOP) {
			if (position.getHeight() > descriptor.MinHeight && position.getHeight() < descriptor.MaxHeight)
				position.UpperLeftCorner.Y = cpos.Y;
		}
		else if (SelectedCheck == EICC_BOTTOM) {
			if (position.getHeight() > descriptor.MinHeight && position.getHeight() < descriptor.MaxHeight)
				position.LowerRightCorner.Y = cpos.Y;
		}

		/// Check for size
		if (descriptor.MinWidth > position.getWidth() && SelectedCheck == EICC_RIGHT)
			position.LowerRightCorner.X = position.UpperLeftCorner.X + descriptor.MinWidth + 30;
		else if (descriptor.MinWidth > position.getWidth() && SelectedCheck == EICC_LEFT)
			position.UpperLeftCorner.X = position.LowerRightCorner.X - descriptor.MinWidth - 30;
		else if (descriptor.MinHeight > position.getHeight() && SelectedCheck == EICC_TOP)
			position.UpperLeftCorner.Y = position.LowerRightCorner.Y - descriptor.MinHeight + 30;
		else if (descriptor.MinHeight > position.getHeight() && SelectedCheck == EICC_BOTTOM)
			position.LowerRightCorner.Y = position.UpperLeftCorner.Y + descriptor.MinHeight + 30;
		
		SelectedNode->getKey()->getElementToResize()->setRelativePosition(position);

		/// Resize the selected element
		SelectedNode->getKey()->OnResize();

		/// And resize all other elements
		core::map<ICP3DInterface *, SControlDescriptor>::ConstIterator it = Parameters.getIterator();
		for (; !it.atEnd(); it++)
			it.getNode()->getKey()->OnResize();
	}

}

bool CCP3DInterfaceController::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {

		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
			core::map<ICP3DInterface *, SControlDescriptor>::ConstIterator it = Parameters.getIterator();
			for (; !it.atEnd(); it++) {
				IGUIElement *element = it.getNode()->getKey()->getElementToResize();
				IGUIElement *focus = Gui->getFocus();
				ECURSOR_ICON icon = CursorControl->getActiveIcon();

				if (focus && checkForChild(element, focus) && (icon == ECI_SIZENS || icon == ECI_SIZEWE)) {
					MouseHeld = true;
					SelectedNode = it;

					/// Test selected check
					vector2di cpos = CursorControl->getPosition();
					const rect<s32> position = element->getRelativePosition();
					if (icon == ECI_SIZEWE) {
						if (cpos.X > position.LowerRightCorner.X - 10 && cpos.X < position.LowerRightCorner.X + 10)
							SelectedCheck = EICC_RIGHT;
						else
							SelectedCheck = EICC_LEFT;
					}
					else {
						if (cpos.Y > position.UpperLeftCorner.Y - 10 && cpos.Y < position.UpperLeftCorner.Y + 10)
							SelectedCheck = EICC_TOP;
						else
							SelectedCheck = EICC_BOTTOM;
					}

					break;
				}
			}
		}
		else if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			MouseHeld = false;
		}

	}

	return false;
}

/// Private methods
void CCP3DInterfaceController::checkForIcon(const s32 cursorValue, const s32 elementValue, const irr::gui::ECURSOR_ICON icon) {
	if (cursorValue > elementValue - 10 && cursorValue < elementValue + 10)
		CursorControl->setActiveIcon(icon);
}

bool CCP3DInterfaceController::checkForChild(IGUIElement *element, IGUIElement *child) {
	if (element == 0)
		return false;

	if (element == child)
		return true;

	IGUIElement *parent = child->getParent();
	while (parent) {
		if (parent == element)
			return true;
		else
			parent = parent->getParent();
	}

	return false;
}

} /// End namespace cp3d
