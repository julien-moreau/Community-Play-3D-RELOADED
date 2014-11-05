#ifndef _H_ICP3D_FILE_SELECTOR_INCLUDED__
#define _H_ICP3D_FILE_SELECTOR_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace ui {

class ICP3DFileSelector : public irr::gui::IGUIFileOpenDialog {
public:

	//! Constructor
	//! \param environment: the GUI environment
	//! \param parent: the element's parent
	//! \param id: the element's id
	//! \param rectangle: the rect<s32> retangle of the element
	ICP3DFileSelector(irr::gui::IGUIEnvironment *environment, irr::gui::IGUIElement *parent,
					  irr::s32 id, irr::core::rect<irr::s32> rectangle)
					  : IGUIFileOpenDialog(environment, parent, id, rectangle)
	{ }

	//! Adds a filter to the file selected
	//! \param name: the filter's name
	//! \param ext: the files extension
	//! \param texture: the texture to draw
	virtual void addFileFilter(wchar_t* name, wchar_t* ext, irr::video::ITexture* texture) = 0;

	//! Adds a prefered place path (works like a shortcut)
	//! \param name: the shortcut name
	//! \param path: the path to the place
	//! \param texture: the texture to draw
	virtual void addPlacePaths(wchar_t* name, irr::core::stringw path, irr::video::ITexture* texture) = 0;

	//! Enum for file selector mode
	//! EFST_OPEN_DIALOG for an open dialog
	//! EFST_SAVE_DIALOG for a save dialog
	enum E_FILESELECTOR_TYPE {
		EFST_OPEN_DIALOG = 0,
		EFST_SAVE_DIALOG,
		EFST_NUM_TYPES
	};

};

} /// End namepsace ui
} /// End namespace cp3d

#endif