#include "Stdafx.h"

#include "CUIAddMesh.h"
#include "../../../Core/CCP3DEditorCore.h"

using namespace irr;
using namespace gui;

namespace cp3d {
namespace ui {

CUIAddMesh::CUIAddMesh(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	CreateUI(editorCore);
}

CUIAddMesh::~CUIAddMesh() {

}

bool CUIAddMesh::OnEvent(const SEvent& event) {

	return false;
}


} // end namespace ui
} // end namespace cp3d


