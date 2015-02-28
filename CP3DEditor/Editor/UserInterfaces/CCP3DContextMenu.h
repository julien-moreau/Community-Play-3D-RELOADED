#ifndef __H_C_CP3D_CONTEXT_MENU_INCLUDED__
#define __H_C_CP3D_CONTEXT_MENU_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

enum ESCENE_CONTEXT_MENU {
	ESCM_ADD_NEW_TERRAIN = 0,
	ESCM_ADD_STATIC_MESH
};

class CCP3DEditorCore;

/*
Class creating the main context menu (the top menu of the application's window)
This class plays like a router and receives all the context menu events.
Then, once the event is received, this class will call the appropriate user
interfaces
Below this class, the enums are the command ids of all the sub menus
Then, if the event caller is FileContextMenu, you have to check if the
selected item is ECMF_OPEN_PROJECT, etc. and then run the appropriate command
*/
class CCP3DContextMenu : public irr::IEventReceiver {
public:

	/// Constructor & Destructor
	CCP3DContextMenu(CCP3DEditorCore *editorCore);
	~CCP3DContextMenu();

	void setProjectName(irr::core::stringc name);

	bool OnEvent(const irr::SEvent &event);

private:
	/// Irrlicht
	irr::gui::IGUIEnvironment *Gui;

	/// CP3D
	CCP3DEditorCore *EditorCore;

	/// --------------------------------------------------
	/// GUI
	/// General
	irr::gui::IGUIContextMenu *ContextMenu;
	irr::gui::IGUIContextMenu *FileContextMenu;
	irr::gui::IGUIContextMenu *EditContextMenu;
	irr::gui::IGUIContextMenu *ViewContextMenu;
	irr::gui::IGUIContextMenu *SceneContextMenu;
	irr::gui::IGUIContextMenu *SpiesContextMenu;

	/// File
	irr::gui::IGUIContextMenu *FileContextMenuNew;
	irr::gui::IGUIContextMenu *FileContextMenuOpen;
	irr::gui::IGUIContextMenu *FileContextMenuAdd;
	irr::u32 FileContextMenuSaveID;
	irr::u32 FileContextMenuSaveAsID;

	/// Scene
	irr::gui::IGUIContextMenu *SceneContextMenuLight;
	/// --------------------------------------------------

	/// Methods
	void checkViewContextMenu(irr::s32 id);
	void checkSceneContextMenu(irr::s32 id);

	/// --------------------------------------------------
	/// ENUMS
	/// View
	enum EVIEW_CONTEXT_MENU {
		EVCM_DRAW_POST_PROCESSES = 0,
		EVCM_DRAW_SHADOWS,
		EVCM_FPS_COUNTER,
		EVCM_SCENE_GRAPH,
		EVCM_EDITION_TOOL,

		EVCM_TEXTURES_MANAGER,
		EVCM_MATERIALS_MANAGER,
		EVCM_POST_PROCESSES_MANAGER,
		EVCM_ANIMATORS_MANAGER,
		EVCM_SCENARIOS_MANAGER
	};

	/// --------------------------------------------------
};

} /// End namespace cp3d

#endif