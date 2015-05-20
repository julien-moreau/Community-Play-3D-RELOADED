#ifndef __H_C_CP3D_SAVER_OPENER_INCLUDED__
#define __H_C_CP3D_SAVER_OPENER_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

class CCP3DEditorCore;
class ICP3DSpy;

class CCP3DSaverOpener : public irr::IEventReceiver {
public:

	/// Constructor & Destructor
	CCP3DSaverOpener(CCP3DEditorCore *editorCore);
	~CCP3DSaverOpener();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// Methods
	void save();
	void open();

private:
	/// Methods
	void saveFile(irr::core::stringc filename);
	void openFile(irr::core::stringc filename);

	/// Editor
	CCP3DEditorCore *EditorCore;
	irr::gui::IGUIEnvironment *Gui;

	/// Gui
	irr::gui::IGUIFileOpenDialog *SaveDialog;
	irr::gui::IGUIFileOpenDialog *OpenDialog;
	irr::gui::IGUIWindow *AskSaveWindow;

	/// Saving
	bool Saved;
};

} /// End namespace cp3d

#endif