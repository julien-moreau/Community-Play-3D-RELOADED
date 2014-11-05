
#include "stdafx.h"
#include "CCP3DEditionTool.h"
#include "CCP3DSceneGraph.h"
#include "../Core/CCP3DInterfaceController.h"
#include "../Core/CCP3DEditorCore.h"
#include "../GUIElements/GUIPanel/CGUIPanel.h"

#include "CCP3DCustomView.h"

#define CCP3DCustomViewMinHeight 150

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DCustomView::CCP3DCustomView(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	/// Configure
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
	SControlDescriptor descriptor(EICC_TOP);
	descriptor.MinHeight = CCP3DCustomViewMinHeight;
	descriptor.MaxHeight = std::numeric_limits<irr::s32>::max();
	editorCore->getInterfaceController()->addElement(this, descriptor);

	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	EditionTool = editorCore->getEditionTool();
	SceneGraph = editorCore->getSceneGraph();

	/// GUI
	Window = Gui->addWindow(rect<s32>(0, Driver->getScreenSize().Height - 50, 0, Driver->getScreenSize().Height), false, L"", 0, -1);
	Window->setDraggable(false);
	Window->setDrawTitlebar(false);
	Window->getCloseButton()->setVisible(false);
	Window->getMinimizeButton()->setVisible(true);

	Panel = new ui::CGUIPanel(Gui, Window, -1, rect<s32>(0, 0, 0, 0));

	OnResize();
}

CCP3DCustomView::~CCP3DCustomView() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DCustomView::clearView() {
	Panel->remove();
	Panel = new ui::CGUIPanel(Gui, Window, -1, rect<s32>(0, 0, 0, 0));
	OnResize();
}

irr::gui::IGUIElement *CCP3DCustomView::getParentGUIElement() {
	return Panel;
}

void CCP3DCustomView::OnResize() {
	rect<s32> position = Window->getRelativePosition();
	position.UpperLeftCorner.X = EditionTool->getElementToResize()->getRelativePosition().getWidth();
	position.LowerRightCorner.Y = Driver->getScreenSize().Height;
	position.LowerRightCorner.X = Driver->getScreenSize().Width - SceneGraph->getElementToResize()->getRelativePosition().getWidth();

	if (position.getHeight() <= CCP3DCustomViewMinHeight)
		position.UpperLeftCorner.Y = Driver->getScreenSize().Height - CCP3DCustomViewMinHeight + 5;

	Window->setRelativePosition(position);

	Panel->setRelativePosition(rect<s32>(5, 20, position.getWidth() - 5, position.getHeight() - 5));
}

bool CCP3DCustomView::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

			if (event.GUIEvent.Caller == Window->getMinimizeButton()) {
				Window->setVisible(false);
				return true;
			}

		}
	}

	return false;
}

} /// End namespace cp3d
