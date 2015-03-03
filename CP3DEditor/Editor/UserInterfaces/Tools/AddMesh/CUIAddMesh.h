#ifndef __H_C_UI_ADD_MESH_INCLUDED__
#define __H_C_UI_ADD_MESH_INCLUDED__

#include <irrlicht.h>
#include "AddMeshUI.h"

#include <ICP3DEditionTool.h>

namespace cp3d {

class CCP3DEditorCore;

namespace ui {

REGISTER_UI(AddMeshUI, UIInstance)
class CUIAddMesh : public irr::IEventReceiver, public IAddMeshUI {
public:

	// Constructor & Destructor
	CUIAddMesh(CCP3DEditorCore *editorCore, const bool animated);
	~CUIAddMesh();

	// Inheritance
	bool OnEvent(const irr::SEvent& event);

private:
	// Editor
	CCP3DEditorCore *EditorCore;
	bool Animated;

	// Window
	irr::core::stringc MeshPath;

	// Viewport
	irr::scene::ISceneManager *ViewPortMgr;
	irr::scene::IMesh *Mesh;
	irr::scene::ISceneNode *Node;

	// GUI
	irr::gui::IGUIFileOpenDialog *BrowseDialog;
};


} /// End namespace ui
} /// End namespace irr

#endif
