#ifndef __H_C_CP3D_CUSTOM_VIEW_INCLUDED__
#define __H_C_CP3D_CUSTOM_VIEW_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomView.h>
#include <ICP3DInterface.h>
#include <ICP3DCustomUpdate.h>

namespace cp3d {

namespace ui {
	class CGUIPanel;
}
class CCP3DEditorCore;
class CCP3DEditionTool;
class CCP3DSceneGraph;

class CCP3DCustomView : public ICP3DCustomView, public ICP3DInterface {
public:

	/// Constructor & Destructor
	CCP3DCustomView(CCP3DEditorCore *editorCore);
	~CCP3DCustomView();

	/// ICP3DInterface
	void OnResize();
	irr::gui::IGUIElement *getElementToResize() { return Window; };

	/// irr::IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// ICP3DCustomView
	void clearView();
	irr::gui::IGUIElement *getParentGUIElement();
	void splitScreen();

private:
	/// Irrlicht
	irr::gui::IGUIEnvironment *Gui;
	irr::video::IVideoDriver *Driver;

	/// CP3D
	CCP3DEditorCore *EditorCore;
	CCP3DEditionTool *EditionTool;
	CCP3DSceneGraph *SceneGraph;

	/// GUI
	irr::gui::IGUIWindow *Window;
	ui::CGUIPanel *Panel;

};

} /// End namespace cp3d

#endif