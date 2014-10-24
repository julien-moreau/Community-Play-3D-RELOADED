
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

CCP3DEditionTool::CCP3DEditionTool(CCP3DEditorCore *editorCore) : EditorCore(editorCore), WindowWidth(400), NewZone(true)
{
	/// Configure
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
	editorCore->getEngine()->getCustomUpdater()->addCustomUpdate(this);

	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();
	Driver = Gui->getVideoDriver();
	CursorControl = editorCore->getDevice()->getCursorControl();

	/// Create elements
	Window = Gui->addWindow(rect<s32>(0, 25, WindowWidth, 0), false, L"Edition Tool", 0, -1);
	Window->getCloseButton()->setVisible(false);
	Window->setDraggable(false);

	TabCtrl = Gui->addTabControl(rect<s32>(0, 0, 0, 0), Window, true, true, -1);

	/// Finish
	SControlDescriptor descriptor(EICC_RIGHT);
	descriptor.MinWidth = 200;
	descriptor.MaxWidth = std::numeric_limits<s32>::max();
	editorCore->getInterfaceController()->addElement(this, descriptor);

	OnResize();
}

CCP3DEditionTool::~CCP3DEditionTool() {
	Window->remove();
	TabCtrl->remove();
}

void CCP3DEditionTool::OnPreUpdate() {

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

void CCP3DEditionTool::OnResize() {
	/// Select the new y position of the window
	s32 positionY = 0, count = 0;
	core::list<IGUIElement *>::ConstIterator it = Gui->getRootGUIElement()->getChildren().begin();
	for (; it != Gui->getRootGUIElement()->getChildren().end(); ++it) {
		EGUI_ELEMENT_TYPE type = (*it)->getType();
		if (type == EGUIET_MENU || type == EGUIET_TOOL_BAR) {
			positionY += (*it)->getRelativePosition().getHeight();
			count++;
		}
	}
	positionY -= 2 * count;

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

void CCP3DEditionTool::setNewZone(IGUITab *tab, stringw name) {
	NewZone = true;

	ui::CGUIPanel *panel = Panels[tab->getNumber()];

	s32 width = panel->getRelativePosition().getWidth();
	s32 offset = getElementPositionOffset(tab, panel);

	IGUIStaticText *e2 = Gui->addStaticText(name.c_str(), rect<s32>(5, offset, width - 10, offset + 20), false, false, panel, -1, false);
	e2->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	e2->setBackgroundColor(SColor(255, 128, 128, 128));

	NewZone = true;
}

SCP3DInterfaceData CCP3DEditionTool::addField(IGUITab *tab, EGUI_ELEMENT_TYPE type, ICP3DEditionToolCallback callback) {
	SCP3DInterfaceData e(EGUIET_ELEMENT);
	ui::CGUIPanel *panel = Panels[tab->getNumber()];

	switch (type) {
	case EGUIET_EDIT_BOX: e = createTextBoxField(tab, panel); break;
	case EGUIET_LIST_BOX: e = createListBoxField(tab, panel); break;
	case EGUIET_COMBO_BOX: e = createComboBoxField(tab, panel); break;

	default: break;
	}

	callback(e);

	return e;
}

bool CCP3DEditionTool::OnEvent(const SEvent &event) {

	return false;
}

SCP3DInterfaceData CCP3DEditionTool::createTextBoxField(IGUITab *tab, ui::CGUIPanel *panel) {
	SCP3DInterfaceData e(EGUIET_EDIT_BOX);

	s32 width = panel->getRelativePosition().getWidth();
	s32 offset = getElementPositionOffset(tab, panel);

	e.TextElement = Gui->addStaticText(L"Text", rect<s32>(5, offset, width / 3, offset + 20), true, false, panel, -1, true);
	((IGUIStaticText*)e.TextElement)->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

	e.TextBox = Gui->addEditBox(L"", rect<s32>(width / 3, offset, width - 10, offset + 20), true, panel, -1);

	return e;
}

SCP3DInterfaceData CCP3DEditionTool::createListBoxField(IGUITab *tab, ui::CGUIPanel *panel) {
	SCP3DInterfaceData e(EGUIET_LIST_BOX);

	s32 width = panel->getRelativePosition().getWidth();
	s32 offset = getElementPositionOffset(tab, panel);

	e.TextElement = Gui->addStaticText(L"Text", rect<s32>(5, offset, width - 150, offset + 20), true, false, panel, -1, true);
	((IGUIStaticText*)e.TextElement)->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

	e.ListData.AddButton = Gui->addButton(rect<s32>(width - 30, offset, width - 10, offset + 20), panel, -1, L"+", L"Add...");
	e.ListData.RemoveButton = Gui->addButton(rect<s32>(width - 50, offset, width - 30, offset + 20), panel, -1, L"-", L"Remove...");
	e.ListData.EditButton = Gui->addButton(rect<s32>(width - 150, offset, width - 50, offset + 20), panel, -1, L"Edit...", L"Edit...");
	e.ListData.List = Gui->addListBox(rect<s32>(5, offset + 20, width - 10, offset + 300), panel, -1, true);

	return e;
}

SCP3DInterfaceData CCP3DEditionTool::createComboBoxField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel) {
	SCP3DInterfaceData e(EGUIET_COMBO_BOX);

	s32 width = panel->getRelativePosition().getWidth();
	s32 offset = getElementPositionOffset(tab, panel);

	e.TextElement = Gui->addStaticText(L"Text", rect<s32>(5, offset, width - 150, offset + 20), true, false, panel, -1, true);
	((IGUIStaticText*)e.TextElement)->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

	e.ComboBox = Gui->addComboBox(rect<s32>(width / 3, offset, width - 10, offset + 20), panel, -1);

	return e;
}

s32 CCP3DEditionTool::getElementPositionOffset(IGUITab *tab, ui::CGUIPanel *panel) {

	core::list<IGUIElement *> glist = panel->getChildren();
	core::list<IGUIElement *>::ConstIterator it = glist.begin();

	s32 offset = (NewZone || glist.size() == 0) ? 10 : 0;
	s32 maxOffset = offset;
	
	for (; it != glist.end(); ++it) {
		s32 height = (*it)->getRelativePosition().LowerRightCorner.Y;
		if (height >= maxOffset)
			maxOffset = offset + height;
	}

	offset = (glist.size() == 0) ? maxOffset : offset + maxOffset;

	NewZone = false;

	return offset;
}

} /// End namespace cp3d
