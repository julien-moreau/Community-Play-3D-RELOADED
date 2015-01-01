
#include "stdafx.h"
#include "../Core/CCP3DEditorCore.h"
#include "CCP3DContextMenu.h"

#include "CCP3DEditionTool.h"
#include <ICP3DEditionTool.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DContextMenu::CCP3DContextMenu(CCP3DEditorCore *editorCore) : EditorCore(editorCore), AddStaticMeshOpenDialog(0)
{
	/// Configure
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	/// Create menus
	ContextMenu = Gui->addMenu(0, -1);
	ContextMenu->addItem(L"CP3D Editor", -1, false);
	FileContextMenu = ContextMenu->getSubMenu(ContextMenu->addItem(L"File", -1, true, true));
	EditContextMenu = ContextMenu->getSubMenu(ContextMenu->addItem(L"Edit", -1, true, true));
	ViewContextMenu = ContextMenu->getSubMenu(ContextMenu->addItem(L"View", -1, true, true));
	SceneContextMenu = ContextMenu->getSubMenu(ContextMenu->addItem(L"Scene", -1, true, true));

	/// --------------------------------------------------
	/// Fill "file"
	FileContextMenuNew = FileContextMenu->getSubMenu(FileContextMenu->addItem(L"New", -1, true, true));
	FileContextMenuOpen = FileContextMenu->getSubMenu(FileContextMenu->addItem(L"Open", -1, true, true));
	FileContextMenu->addSeparator();
	FileContextMenuAdd = FileContextMenu->getSubMenu(FileContextMenu->addItem(L"Add", -1, true, true));
	FileContextMenu->addSeparator();
	FileContextMenuSaveID = FileContextMenu->addItem(L"", -1);
	FileContextMenuSaveAsID = FileContextMenu->addItem(L"", -1);
	FileContextMenu->addSeparator();
	FileContextMenu->addItem(L"Exit... (TO DO)", -1);

	/// Fill "file->new"
	FileContextMenuNew->addItem(L"Project... (TO DO)", -1);
	FileContextMenuNew->addItem(L"Script file... (TO DO)", -1);

	/// Fille "file->open"
	FileContextMenuOpen->addItem(L"Existing project... (TO DO)", -1);
	FileContextMenuOpen->addItem(L"Existing file... (TO DO)", -1);

	/// Fill "file->add"
	FileContextMenuAdd->addItem(L"New scene... (TO DO)", -1);
	FileContextMenuAdd->addItem(L"Existing scene... (TO DO)", -1);
	/// --------------------------------------------------

	/// --------------------------------------------------
	/// Fill "edit"
	EditContextMenu->addItem(L"Undo (TO DO)", -1);
	EditContextMenu->addItem(L"Redo (TO DO)", -1);
	EditContextMenu->addSeparator();
	EditContextMenu->addItem(L"Cut (TO DO)", -1);
	EditContextMenu->addItem(L"Copy (TO DO)", -1);
	EditContextMenu->addItem(L"Paste (TO DO)", -1);
	EditContextMenu->addItem(L"Delete (TO DO)", -1);
	EditContextMenu->addSeparator();
	EditContextMenu->addItem(L"Edit current scene... (TO DO)", -1);
	EditContextMenu->addItem(L"Restore selected object... (TO DO)", -1);
	EditContextMenu->addItem(L"Apply Physics simulation (TO DO)", -1);
	EditContextMenu->addSeparator();
	EditContextMenu->addItem(L"Edit preferences... (TO DO)", -1);
	/// --------------------------------------------------

	/// --------------------------------------------------
	/// Fill "view"
	ViewContextMenu->addItem(L"Draw post-processes (TO DO)", EVCM_DRAW_POST_PROCESSES, true, false, true, true);
	ViewContextMenu->addItem(L"Draw shadows (TO DO)", EVCM_DRAW_SHADOWS, true, false, true, true);
	ViewContextMenu->addItem(L"FPS counter (TO DO)", EVCM_FPS_COUNTER, true, false, true, true);
	ViewContextMenu->addItem(L"Scene graph", EVCM_SCENE_GRAPH, true, false, true, true);
	ViewContextMenu->addItem(L"Edition tool", EVCM_EDITION_TOOL, true, false, true, true);
	ViewContextMenu->addSeparator();
	ViewContextMenu->addItem(L"Textures manager... (TO DO)", EVCM_TEXTURES_MANAGER);
	ViewContextMenu->addItem(L"Materials manager... (TO DO)", EVCM_MATERIALS_MANAGER);
	ViewContextMenu->addItem(L"Post-processes manager... (TO DO)", EVCM_POST_PROCESSES_MANAGER);
	ViewContextMenu->addItem(L"Animators manager... (TO DO)", EVCM_ANIMATORS_MANAGER);
	ViewContextMenu->addItem(L"Scenarios manager... (TO DO)", EVCM_SCENARIOS_MANAGER);
	/// --------------------------------------------------

	/// --------------------------------------------------
	/// Fill "scene"
	SceneContextMenu->addItem(L"Add new terrain... (TO DO)", ESCM_ADD_NEW_TERRAIN);
	SceneContextMenu->addItem(L"Add static mesh... (TO DO)", ESCM_ADD_STATIC_MESH);
	SceneContextMenu->addItem(L"Add animated mesh... (TO DO)", -1);
	SceneContextMenuLight = SceneContextMenu->getSubMenu(SceneContextMenu->addItem(L"Add light", -1, true, true));
	SceneContextMenu->addSeparator();
	SceneContextMenu->addItem(L"Add cube... (TO DO)", -1);
	SceneContextMenu->addItem(L"Add sphere... (TO DO)", -1);
	SceneContextMenu->addItem(L"Add plane... (TO DO)", -1);
	SceneContextMenu->addItem(L"Add billboard (TO DO)", -1);
	SceneContextMenu->addSeparator();
	SceneContextMenu->addItem(L"Add clouds... (TO DO)", -1);
	SceneContextMenu->addItem(L"Add skybox... (TO DO)", -1);
	SceneContextMenu->addItem(L"Add skydome... (TO DO)", -1);
	SceneContextMenu->addSeparator();
	SceneContextMenu->addItem(L"Paint terrain... (TO DO)", -1);

	/// Fille "scene->add light"
	SceneContextMenuLight->addItem(L"Shadow light... (TO DO)", -1);
	SceneContextMenuLight->addItem(L"Standard light... (TO DO)", -1);
	/// --------------------------------------------------

}

CCP3DContextMenu::~CCP3DContextMenu() {

}

void CCP3DContextMenu::setProjectName(irr::core::stringc name) {
	FileContextMenu->setItemText(FileContextMenuSaveID, stringw(stringc("Save ") + name + stringc(" CTRL+S")).c_str());
	FileContextMenu->setItemText(FileContextMenuSaveAsID, stringw(stringc("Save ") + name + stringc(" as...")).c_str());
}

bool CCP3DContextMenu::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {

			/// Scene context menu
			if (event.GUIEvent.Caller == SceneContextMenu) {
				if (SceneContextMenu->getSelectedItem() == ESCM_ADD_STATIC_MESH) {
					AddStaticMeshOpenDialog = EditorCore->createFileOpenDialog(L"Select Static mesh...", 0, ui::ICP3DFileSelector::EFST_OPEN_DIALOG);
				}

				return true;
			}
			/// View context menu
			else if (event.GUIEvent.Caller == ViewContextMenu) {
				checkViewContextMenu(ViewContextMenu->getItemCommandId(ViewContextMenu->getSelectedItem()));
				return true;
			}

		}

		else if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == AddStaticMeshOpenDialog) {
				IMesh *m = EditorCore->getEngine()->getSceneNodeCreator()->getStaticMesh(AddStaticMeshOpenDialog->getFileName(), false);
				ISceneNode *n = EditorCore->getRenderingEngine()->getSceneManager()->addMeshSceneNode(m);

				SEvent ev;
				ev.EventType = EET_USER_EVENT;
				ev.UserEvent.UserData1 = EIE_NODE_ADDED;
				ev.UserEvent.UserData2 = (s32)n;
				EditorCore->getEngine()->getEventReceiver()->OnEvent(ev);

				AddStaticMeshOpenDialog = 0;
				return true;
			}
		}
	}

	return false;
}

void CCP3DContextMenu::checkViewContextMenu(irr::s32 id) {

	CCP3DEditionTool *editionTool = EditorCore->getEditionTool();

	switch (id) {

	case EVCM_POST_PROCESSES_MANAGER:
		editionTool->applyForControllerType((ESCENE_NODE_TYPE)ESNT2_POST_PROCESS, 0);
		break;

	default:
		break;
	}
}

} /// End namespace cp3d
