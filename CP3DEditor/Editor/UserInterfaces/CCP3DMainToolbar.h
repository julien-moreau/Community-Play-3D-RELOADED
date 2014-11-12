#ifndef __H_C_CP3D_MAIN_TOOLBAR_INCLUDED__
#define __H_C_CP3D_MAIN_TOOLBAR_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

class CCP3DEditorCore;

/*
Class that handle the main toolbar
Will route the events to the selected processes
*/
class CCP3DMainToolbar : public irr::IEventReceiver {
public:

	/// Constructor & Destructor
	CCP3DMainToolbar(CCP3DEditorCore *editorCore);
	~CCP3DMainToolbar();

	bool OnEvent(const irr::SEvent &event);

private:
	/// Irrlicht
	irr::gui::IGUIEnvironment *Gui;
	irr::video::IVideoDriver *Driver;

	/// CP3D
	CCP3DEditorCore *EditorCore;

	/// Gui
	irr::gui::IGUIToolBar *Toolbar;

	/// Position, Rotation, Scale
	irr::gui::IGUIButton *PositionButton, *RotationButton, *ScaleButton;

};

} /// End namespace cp3d

#endif