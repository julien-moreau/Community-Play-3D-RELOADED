
#include "Stdafx.h"
#include "../Core/CCP3DEditorCore.h"
#include "../Core/CCP3DInterfaceController.h"
#include "../GUIElements/GUIPanel/CGUIPanel.h"

#include "EditionTools/CCP3DEditionToolSceneNode.h"

#include "CCP3DEditionTool.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionTool::CCP3DEditionTool(CCP3DEditorCore *editorCore) : EditorCore(editorCore), WindowWidth(400), NewZone(true),
	LastSceneNodeType(ESNT_UNKNOWN), LastSelectedTab(-1)
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

	/// Configure UI
	SControlDescriptor descriptor(EICC_RIGHT);
	descriptor.MinWidth = 200;
	descriptor.MaxWidth = std::numeric_limits<s32>::max();
	editorCore->getInterfaceController()->addElement(this, descriptor);

	/// Finish
	OnResize();
}

CCP3DEditionTool::~CCP3DEditionTool() {
	Window->remove();
	TabCtrl->remove();
}

void CCP3DEditionTool::OnPostUpdate() {
	const u32 index = TabCtrl->getActiveTab();
	if (index == -1)
		return;

	IGUITab *tab = TabCtrl->getTab(index);
	ui::CGUIPanel *panel = Panels[index];

	core::list<SCP3DInterfaceData>::ConstIterator it = InterfaceDatas.begin();
	for (; it != InterfaceDatas.end(); ++it) {

		/// If image then zoom on the image
		if ((*it).Type == EGUIET_IMAGE) {
			IGUIImage *img = (*it).TextureData.Image;

			if (img->getParent() == panel) {
				vector2di cpos = CursorControl->getPosition();
				img->updateAbsolutePosition();
				if (img->getImage() && img->getAbsolutePosition().isPointInside(cpos)) {

					rect<s32> destRect(cpos.X, cpos.Y, cpos.X + 250, cpos.Y + 250);
					rect<s32> sourceRect(0, 0, img->getImage()->getOriginalSize().Width, img->getImage()->getOriginalSize().Height);
					Driver->draw2DImage(img->getImage(), destRect, sourceRect);

				}
			}
		}

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
	Panels.clear();

	/// Tabs
	LastSelectedTab = TabCtrl->getActiveTab();
	TabCtrl->remove();
	TabCtrl = Gui->addTabControl(rect<s32>(0, 0, 0, 0), Window, true, true, -1);
	OnResize();

	/// Interface Datas
	InterfaceDatas.clear();

	/// Finish
	NewZone = true;
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

		/// Resize children elements to original position
		Panels[i]->getScrollBar()->setPos(0);
		SEvent ev;
		ev.EventType = EET_GUI_EVENT;
		ev.GUIEvent.Caller = Panels[i]->getScrollBar();
		ev.GUIEvent.EventType = EGET_SCROLL_BAR_CHANGED;
		Panels[i]->OnEvent(ev);
	}

	core::list<SCP3DInterfaceData>::ConstIterator itd = InterfaceDatas.begin();
	for (; itd != InterfaceDatas.end(); ++itd) {
		core::array<IGUIElement *> elements;
		if ((*itd).Type == EGUIET_EDIT_BOX)
			elements.push_back((*itd).TextBox);
		else if ((*itd).Type == EGUIET_COMBO_BOX)
			elements.push_back((*itd).ComboBox);
		else if ((*itd).Type == EGUIET_STATIC_TEXT)
			elements.push_back((*itd).TextElement);
		else if ((*itd).Type == EGUIET_IMAGE) {
			elements.push_back((*itd).TextureData.BrowseButton);
			elements.push_back((*itd).TextureData.EditBoxPath);
			elements.push_back((*itd).TextureData.RemoveButton);
			elements.push_back((*itd).TextureData.Zone);
		}
		else if ((*itd).Type == EGUIET_LIST_BOX)
			elements.push_back((*itd).ListData.List);
		else if ((*itd).Type == EGUIET_CHECK_BOX)
			elements.push_back((*itd).CheckBox);

		for (u32 i=0; i < elements.size(); i++) {
			rect<s32> position = elements[i]->getRelativePosition();
			position.LowerRightCorner.X = getPanelWidth(elements[i]->getParent()) - 5;
			//elements[i]->getParent()->getRelativePosition().getWidth() - 5;
			elements[i]->setRelativePosition(rect<s32>(position));
		}
	}
}

void CCP3DEditionTool::createDefaultControllers() {
	CCP3DEditionToolSceneNode *EditionToolSceneNode = new CCP3DEditionToolSceneNode(EditorCore);

	addController(ESNT_MESH, EditionToolSceneNode);
	addController(ESNT_ANIMATED_MESH, EditionToolSceneNode);
	addController(ESNT_LIGHT, EditionToolSceneNode);
	addController(ESNT_BILLBOARD, EditionToolSceneNode);
	addController(ESNT_CUBE, EditionToolSceneNode);
	addController(ESNT_EMPTY, EditionToolSceneNode);
	addController(ESNT_OCTREE, EditionToolSceneNode);
	addController(ESNT_SKY_BOX, EditionToolSceneNode);
	addController(ESNT_SKY_DOME, EditionToolSceneNode);
	addController(ESNT_SPHERE, EditionToolSceneNode);
	addController(ESNT_TERRAIN, EditionToolSceneNode);
	addController(ESNT_TEXT, EditionToolSceneNode);
	addController(ESNT_VOLUME_LIGHT, EditionToolSceneNode);
	addController(ESNT_WATER_SURFACE, EditionToolSceneNode);
}

bool CCP3DEditionTool::addController(ESCENE_NODE_TYPE type, ICP3DEditionToolController *controller) {
	if (!EditionTools.find(type))
		EditionTools.insert(type, 0);

	core::map<ESCENE_NODE_TYPE, core::array<ICP3DEditionToolController *>>::Node *it = EditionTools.find(type);
	for (u32 i=0; i < it->getValue().size(); i++) {
		if (it->getValue()[i] == controller)
			return false;
	}

	it->getValue().push_back(controller);

	return true;
}

void CCP3DEditionTool::addSeparator(irr::gui::IGUITab *tab) {
	ui::CGUIPanel *panel = Panels[tab->getNumber()];

	s32 width = getPanelWidth(panel);
	s32 offset = getElementPositionOffset(tab, panel);

	IGUIStaticText *e = Gui->addStaticText(L"", rect<s32>(5, offset, width - 10, offset + 20), false, false, panel, -1, false);
	e->setVisible(false);
}

void CCP3DEditionTool::setNewZone(IGUITab *tab, stringw name) {
	NewZone = true;

	ui::CGUIPanel *panel = Panels[tab->getNumber()];

	s32 width = getPanelWidth(panel);
	s32 offset = getElementPositionOffset(tab, panel);

	IGUIStaticText *e = Gui->addStaticText(name.c_str(), rect<s32>(2, offset, width - 2, offset + 25), false, false, panel, -1, false);
	e->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	e->setBackgroundColor(SColor(255, 0, 0, 0));

	SCP3DInterfaceData ed(EGUIET_STATIC_TEXT);
	ed.TextElement = e;
	InterfaceDatas.push_back(ed);

	NewZone = true;
}

SCP3DInterfaceData CCP3DEditionTool::addField(IGUITab *tab, EGUI_ELEMENT_TYPE type, ICP3DEditionToolCallback callback) {
	SCP3DInterfaceData e(EGUIET_ELEMENT);
	ui::CGUIPanel *panel = Panels[tab->getNumber()];

	switch (type) {
	case EGUIET_EDIT_BOX: e = createTextBoxField(tab, panel); break;
	case EGUIET_LIST_BOX: e = createListBoxField(tab, panel); break;
	case EGUIET_COMBO_BOX: e = createComboBoxField(tab, panel); break;
	case EGUIET_IMAGE: e = createTextureField(tab, panel); break;
	case EGUIET_CHECK_BOX: e = createCheckBoxField(tab, panel); break;

	default: break;
	}

	callback(e);
	InterfaceDatas.push_back(e);

	panel->recalculateScrollBar();

	return e;
}

bool CCP3DEditionTool::OnEvent(const SEvent &event) {

	if (event.EventType == EET_USER_EVENT) {

		/// update all edition tools with slected scene node
		if (event.UserEvent.UserData1 == EIE_NODE_SELECTED) {
			
			ISceneNode *node = (ISceneNode *)event.UserEvent.UserData2;
			node = dynamic_cast<ISceneNode *>(node);

			if (!node)
				clearTabs();
			else {
				ESCENE_NODE_TYPE type = node->getType();
				core::map<ESCENE_NODE_TYPE, array<ICP3DEditionToolController *>>::Node *it = EditionTools.find(type);

				if (type != LastSceneNodeType)
					clearTabs();

				if (!it)
					return false;

				for (u32 i=0; i < it->getValue().size(); i++) {
					it->getValue()[i]->setSceneNode(node);
					if (type != LastSceneNodeType)
						it->getValue()[i]->createInterface();
					it->getValue()[i]->configure();
				}

				LastSceneNodeType = type;
				if (TabCtrl->getTabCount() >= LastSelectedTab)
					TabCtrl->setActiveTab(LastSelectedTab);

				return false;
			}
		}

	}

	else if (event.EventType == EET_GUI_EVENT) {

		/// Update the current edition tool with the active tab
		if (event.GUIEvent.Caller && event.GUIEvent.EventType != EGET_ELEMENT_FOCUSED &&
			event.GUIEvent.EventType != EGET_ELEMENT_FOCUS_LOST && event.GUIEvent.EventType != EGET_ELEMENT_HOVERED &&
			event.GUIEvent.EventType != EGET_ELEMENT_LEFT)
		{
			IGUIElement *parent = event.GUIEvent.Caller;
			bool isChildOfTabCtrl = false;

			while (parent) {
				if (parent->getParent() == TabCtrl) {
					isChildOfTabCtrl = true;
					break;
				}
				parent = parent->getParent();
			}

			core::map<ESCENE_NODE_TYPE, core::array<ICP3DEditionToolController *>>::Node *it = EditionTools.find(LastSceneNodeType);

			if (isChildOfTabCtrl && it && it->getValue().size() > 0) {
				/// Update edition tools of LastSceneNodeType
				if (it) {
					for (u32 i=0; i < it->getValue().size(); i++) {
						it->getValue()[i]->apply();
					}

					SEvent ev;
					ev.EventType = EET_USER_EVENT;
					ev.UserEvent.UserData1 = EIE_NODE_CHANGED;
					ev.UserEvent.UserData2 = (s32)it->getValue()[0]->getSceneNode();
					EditorCore->getEngine()->getEventReceiver()->OnEvent(ev);
				}
			}
		}

	}

	else if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {

			const s32 tabIndex = TabCtrl->getActiveTab();
			if (tabIndex == -1)
				return false;
			ui::CGUIPanel *panel = Panels[tabIndex];
			IGUIElement *focus = Gui->getFocus();

			if (focus == panel || focus == TabCtrl || focus == TabCtrl->getTab(tabIndex) ||
				(focus && focus->getParent() == panel))
			{
				panel->getScrollBar()->setPos(panel->getScrollBar()->getPos() - s32(event.MouseInput.Wheel * 2.0));

				SEvent ev;
				ev.EventType = EET_GUI_EVENT;
				ev.GUIEvent.Caller = panel->getScrollBar();
				ev.GUIEvent.EventType = EGET_SCROLL_BAR_CHANGED;
				panel->OnEvent(ev);
			}

		}
	}

	return false;
}

SCP3DInterfaceData CCP3DEditionTool::createTextBoxField(IGUITab *tab, ui::CGUIPanel *panel) {
	SCP3DInterfaceData e(EGUIET_EDIT_BOX);

	s32 width = getPanelWidth(panel);
	s32 offset = getElementPositionOffset(tab, panel);

	e.TextElement = Gui->addStaticText(L"Text", rect<s32>(5, offset, width / 3, offset + 20), true, false, panel, -1, true);
	((IGUIStaticText*)e.TextElement)->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

	e.TextBox = Gui->addEditBox(L"", rect<s32>(width / 3, offset, width - 10, offset + 20), false, panel, -1);

	return e;
}

SCP3DInterfaceData CCP3DEditionTool::createListBoxField(IGUITab *tab, ui::CGUIPanel *panel) {
	SCP3DInterfaceData e(EGUIET_LIST_BOX);

	s32 width = getPanelWidth(panel);
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

	s32 width = getPanelWidth(panel);
	s32 offset = getElementPositionOffset(tab, panel);

	e.TextElement = Gui->addStaticText(L"Text", rect<s32>(5, offset, width - 150, offset + 20), true, false, panel, -1, true);
	((IGUIStaticText*)e.TextElement)->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

	e.ComboBox = Gui->addComboBox(rect<s32>(width / 3, offset, width - 10, offset + 20), panel, -1);
	e.ComboBox->setMaxSelectionRows(20);

	return e;
}

SCP3DInterfaceData CCP3DEditionTool::createTextureField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel) {
	SCP3DInterfaceData e(EGUIET_IMAGE);

	s32 width = getPanelWidth(panel);
	s32 offset = getElementPositionOffset(tab, panel);

	e.TextureData.Zone = Gui->addStaticText(L"", rect<s32>(2, offset - 2 - 20, width - 2, offset + 92), false, false, panel, -1, true);
	((IGUIStaticText*)e.TextureData.Zone)->setBackgroundColor(SColor(255, 48, 48, 48));
	e.TextElement = Gui->addStaticText(L"", rect<s32>(5, offset, width - 5, offset + 20), false, true, panel, -1, false);
	e.TextureData.Image = Gui->addImage(rect<s32>(5, offset + 20, 75, offset + 20 + 70), panel, -1, L"Texture", false);
	e.TextureData.Image->setScaleImage(true);
	e.TextureData.EditBoxPath = Gui->addEditBox(L"", rect<s32>(81, offset + 20, width - 15, offset + 40), false, panel, -1);
	e.TextureData.EditBoxPath->setEnabled(false);
	e.TextureData.BrowseButton = Gui->addButton(rect<s32>(80, offset + 40, width - 15, offset + 60), panel, -1, L"Browse...", L"Browse texture...");
	e.TextureData.RemoveButton = Gui->addButton(rect<s32>(80, offset + 60, width - 15, offset + 80), panel, -1, L"Unset", L"Unsets the texture");
	e.TextureData.BrowseDialog = 0;

	return e;
}

SCP3DInterfaceData CCP3DEditionTool::createCheckBoxField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel) {
	SCP3DInterfaceData e(EGUIET_CHECK_BOX);

	s32 width = panel->getRelativePosition().getWidth();
	s32 offset = getElementPositionOffset(tab, panel);

	e.CheckBox = Gui->addCheckBox(false, rect<s32>(5, offset, width - 5, offset + 20), panel, -1, L"");

	return e;
}

s32 CCP3DEditionTool::getPanelWidth(IGUIElement *panel) {
	const s32 size = Gui->getSkin()->getSize(EGDS_SCROLLBAR_SIZE);

	return panel->getRelativePosition().getWidth() - size;
}

s32 CCP3DEditionTool::getElementPositionOffset(IGUITab *tab, ui::CGUIPanel *panel) {

	core::list<IGUIElement *> glist = panel->getChildren();
	core::list<IGUIElement *>::ConstIterator it = glist.begin();

	s32 offset = (NewZone || glist.size() == 0) ? 10 : 0;
	s32 maxOffset = offset;
	
	for (; it != glist.end(); ++it) {
		if ((*it) == panel->getScrollBar())
			continue;

		s32 height = (*it)->getRelativePosition().LowerRightCorner.Y;
		if (height >= maxOffset)
			maxOffset = offset + height;
	}

	offset = (glist.size() == 0) ? maxOffset : offset + maxOffset;

	NewZone = false;

	return offset;
}

} /// End namespace cp3d
