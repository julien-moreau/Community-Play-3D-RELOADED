#ifndef __H_C_ADD_MESH_UI_INCLUDED__
#define __H_C_ADD_MESH_UI_INCLUDED__

#include <irrlicht.h>
#include "../../../Core/CCP3DEditorCore.h"
#include <ICP3DUIRegister.h>

namespace cp3d {
namespace ui {

class AddMeshUI {
public:

	/// Constructor & Destructor
	AddMeshUI(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
	{ }

	~AddMeshUI()
	{ }

	void remove()
	{ }

private:
	// Editor
	CCP3DEditorCore *EditorCore;

};


} // end namespace ui
} // end namespace irr

#endif
