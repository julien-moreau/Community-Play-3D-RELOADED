#ifndef __H_C_CP3D_EDITION_TOOL_INCLUDED__
#define __H_C_CP3D_EDITION_TOOL_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>

namespace cp3d {

class CCP3DEditorCore;
	namespace ui {
		class CGUIPanel;
	}

/*
Class creating the main edition tool (left side of the window).
This class must let users (and us =D) able to create easily gui elements
called fields (text, float, color, etc.).
It contains an array of fields
*/
class CCP3DEditionTool : public irr::IEventReceiver, public engine::ICP3DUpdate {
public:

	/// Constructor & Destructor
	CCP3DEditionTool(CCP3DEditorCore *editorCore);
	~CCP3DEditionTool();

	/// Inheritance
	bool OnEvent(const irr::SEvent &event);
	void OnPreUpdate();

	/// Utils
	irr::gui::IGUITab *addTab(const irr::core::stringc name);
	void clearTabs();

private:
	/// Irrlicht
	irr::gui::IGUIEnvironment *Gui;
	irr::video::IVideoDriver *Driver;
	irr::gui::ICursorControl *CursorControl;

	/// CP3D
	CCP3DEditorCore *EditorCore;

	/// Datas
	irr::core::dimension2du ScreenSize;
	irr::s32 WindowWidth;

	/// GUI
	irr::gui::IGUIWindow *Window;
	irr::gui::IGUITabControl *TabCtrl;

	irr::core::array<ui::CGUIPanel *> Panels;

	/// Methods
	void resize();

};

} /// End namespace cp3d

#endif