#ifndef __H_C_UI_ADD_MESH_INCLUDED__
#define __H_C_UI_ADD_MESH_INCLUDED__

#include <irrlicht.h>
#include "AddMeshUI.h"

namespace cp3d {

class CCP3DEditorCore;

namespace ui {

REGISTER_UI(AddMeshUI, UIInstance)
class CUIAddMesh : public irr::IEventReceiver, public IAddMeshUI {
public:

	/// Constructor & Destructor
	CUIAddMesh(CCP3DEditorCore *editorCore);
	~CUIAddMesh();

	/// Inheritance
	bool OnEvent(const irr::SEvent& event);

private:
	// Editor
	CCP3DEditorCore *EditorCore;

};


} // end namespace ui
} // end namespace irr

#endif
