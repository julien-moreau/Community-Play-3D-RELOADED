#ifndef __H_C_GUI_MANAGER_INCLUDED__
#define __H_C_GUI_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DGUIManager.h>

namespace cp3d {
	class CCP3DEditorCore;
}

namespace cp3d {
namespace ui {

class ICP3DViewPort;

class CGUIManager : public ICP3DGUIManager {
public:

	/// Constructor & destructor
	CGUIManager(CCP3DEditorCore *editorCore);
	~CGUIManager();

	void centerWindow(irr::gui::IGUIWindow *window);

	ICP3DViewPort *createViewPort(irr::core::rect<irr::s32> rectangle, irr::gui::IGUIElement* parent = 0, irr::s32 id = -1);
	ui::ICP3DFileSelector *createFileOpenDialog(irr::core::stringw name, irr::gui::IGUIElement *parent, ui::ICP3DFileSelector::E_FILESELECTOR_TYPE type, bool modal = true);
	irr::gui::IGUIWindow *createMessageBox(irr::core::stringw title, irr::core::stringw text, irr::s32 flags, bool modal = true, irr::video::ITexture *texture = 0);

private:
	/// Members
	CCP3DEditorCore *EditorCore;

	irr::IrrlichtDevice *Device;
	irr::video::IVideoDriver *Driver;
	irr::gui::IGUIEnvironment *Gui;

};


} /// End namespace ui
} /// End namespace irr

#endif
