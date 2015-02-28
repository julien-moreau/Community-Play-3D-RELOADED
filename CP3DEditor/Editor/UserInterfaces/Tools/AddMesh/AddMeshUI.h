#ifndef __H_C_ADD_MESH_UI_INCLUDED__
#define __H_C_ADD_MESH_UI_INCLUDED__

#include <irrlicht.h>
#include <ICP3DGUIManager.h>
#include <ICP3DEditor.h>
#include <ICP3DUIRegister.h>

namespace cp3d {
namespace ui {

class AddMeshUI {
public:

	// Constructor & Destructor
	AddMeshUI(ICP3DEditor *editorCore) : EditorCore(editorCore)
	{
		irr::gui::IGUIEnvironment *gui = EditorCore->getRenderingEngine()->getGUIEnvironment();
		ICP3DGUIManager *guiManager = EditorCore->getGUIManager();

		Window = gui->addWindow(irr::core::rect<irr::s32>(360, 90, 1035, 450), false, L"Add Static Mesh", 0, -1);
		viewport = guiManager->createViewPort(irr::core::rect<irr::s32>(10, 30, 370, 350), Window, -1);

		PathEditBox = gui->addEditBox(L"", irr::core::rect<irr::s32>(380, 30, 570, 50), false, Window, -1);
		BrowseButton = gui->addButton(irr::core::rect<irr::s32>(570, 30, 670, 50), Window, -1, L"Browse...", L"Browse for static mesh");

		OkButton = gui->addButton(irr::core::rect<irr::s32>(460, 320, 560, 350), Window, -1, L"OK", L"Add the selected static mesh to the scene");
		CancelButton = gui->addButton(irr::core::rect<irr::s32>(570, 320, 670, 350), Window, -1, L"Cancel", L"Cancel process");
	}

	~AddMeshUI()
	{
		// Remove only top parent
		Window->remove();
	}

private:
	// Editor
	ICP3DEditor *EditorCore;

	// GUI
	irr::gui::IGUIWindow *Window;
	ICP3DViewPort *viewport;

	irr::gui::IGUIEditBox *PathEditBox;
	irr::gui::IGUIButton *BrowseButton;

	irr::gui::IGUITreeView *TreeView;

	irr::gui::IGUIButton *OkButton;
	irr::gui::IGUIButton *CancelButton;

};


} // end namespace ui
} // end namespace irr

#endif
