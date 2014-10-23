
#include "Stdafx.h"
#include "../Core/CCP3DEditorCore.h"
#include "../Core/CCP3DInterfaceController.h"
#include "../GUIElements/GUIPanel/CGUIPanel.h"

#include "CCP3DEditionTool.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionTool::CCP3DEditionTool(CCP3DEditorCore *editorCore) : EditorCore(editorCore), WindowWidth(400)
{
	/// Configure
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
	editorCore->getEngine()->getCustomUpdater()->addCustomUpdate(this);

	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();
	Driver = Gui->getVideoDriver();
	CursorControl = editorCore->getDevice()->getCursorControl();

	ScreenSize = Driver->getScreenSize();

	/// Create elements
	Window = Gui->addWindow(rect<s32>(0, 25, WindowWidth, 0), false, L"Edition Tool", 0, -1);
	Window->getCloseButton()->setVisible(false);
	Window->setDraggable(false);

	TabCtrl = Gui->addTabControl(rect<s32>(0, 0, 0, 0), Window, true, true, -1);

	/// Finish
	SControlDescriptor descriptor(EICC_RIGHT);
	descriptor.MinWidth = 200;
	descriptor.MaxWidth = std::numeric_limits<s32>::max();
	editorCore->getInterfaceController()->addElement(Window, descriptor);

	resize();
}

CCP3DEditionTool::~CCP3DEditionTool() {
	Window->remove();
	TabCtrl->remove();
}

void CCP3DEditionTool::OnPreUpdate() {
	if (Driver->getScreenSize() != ScreenSize || Window->getRelativePosition().getWidth() != WindowWidth) {
		resize();
		ScreenSize = Driver->getScreenSize();
		WindowWidth = Window->getRelativePosition().getWidth();
	}
}

irr::gui::IGUITab *CCP3DEditionTool::addTab(const irr::core::stringc name) {
	IGUITab *tab = TabCtrl->addTab(stringw(name).c_str(), -1);
	ui::CGUIPanel *panel = new ui::CGUIPanel(Gui, tab, -1,
											 rect<s32>(3, 2, tab->getRelativePosition().getWidth() - 2,
													   tab->getRelativePosition().getHeight() - 2));
	Panels.push_back(panel);

	return tab;
}

void CCP3DEditionTool::clearTabs() {
	/// Panels
	for (u32 i=0; i < Panels.size(); i++)
		Panels[i]->remove();
	Panels.clear();

	/// Tabs
	TabCtrl->clear();
}

void CCP3DEditionTool::resize() {
	/// Select the new y position of the window
	s32 positionY = 0;
	core::list<IGUIElement *>::ConstIterator it = Gui->getRootGUIElement()->getChildren().begin();
	for (; it != Gui->getRootGUIElement()->getChildren().end(); ++it) {
		EGUI_ELEMENT_TYPE type = (*it)->getType();
		if (type == EGUIET_MENU || type == EGUIET_TOOL_BAR)
			positionY += (*it)->getRelativePosition().getHeight();
	}

	/// Resize elements
	Window->setRelativePosition(rect<s32>(0, positionY, Window->getRelativePosition().LowerRightCorner.X,
										  Driver->getScreenSize().Height));
	TabCtrl->setRelativePosition(rect<s32>(5, 25, Window->getRelativePosition().getWidth() - 5, Window->getRelativePosition().getHeight() - 5));

	for (u32 i=0; i < Panels.size(); i++) {
		IGUIElement *parent = Panels[i]->getParent();
		Panels[i]->setRelativePosition(rect<s32>(3, 2, parent->getRelativePosition().getWidth() - 2,
												 parent->getRelativePosition().getHeight() - 2));
	}
}

bool CCP3DEditionTool::OnEvent(const SEvent &event) {

	return false;
}

} /// End namespace cp3d
