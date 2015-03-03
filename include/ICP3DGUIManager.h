#ifndef _H_ICP3D_GUI_MANAGER_INCLUDED__
#define _H_ICP3D_GUI_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DViewPort.h>
#include <ICP3DFileSelector.h>

namespace cp3d {
namespace ui {

class ICP3DGUIManager {
public:

	//! Centers the window on the screen
	//! \param window: the window to center
	virtual void centerWindow(irr::gui::IGUIWindow *window) = 0;

	//! Creates a view port to draw a scene
	//! \param rectangle: the element's relative position
	//! \param parent: the element's parent
	//! \param id: the element's id
	virtual ICP3DViewPort *createViewPort(irr::core::rect<irr::s32> rectangle, irr::gui::IGUIElement* parent = 0, irr::s32 id = -1) = 0;

	//! Creates a file picker dialog
	//! \param name: the element's name
	//! \param parent: the element's parent
	//! \param type: the file picker type (open, save, etc.)
	//! \param modal: true if the window should be modal
	virtual ICP3DFileSelector *createFileOpenDialog(irr::core::stringw name, irr::gui::IGUIElement *parent, ICP3DFileSelector::E_FILESELECTOR_TYPE type, bool modal = true) = 0;

	//! Creates a messagebox
	//! \param title: the messagebox title
	//! \param text: the messagebox text body
	//! \param flags: the messagebox flags (OK and CANCEL, OK, etc.)
	//! \param modal: if the window is modal or not
	//! \parma texture: the messagebox texture to show
	virtual irr::gui::IGUIWindow *createMessageBox(irr::core::stringw title, irr::core::stringw text, irr::s32 flags, bool modal = true, irr::video::ITexture *texture = 0) = 0;

};

} /// End namespace ui
} /// End namespace cp3d

#endif
