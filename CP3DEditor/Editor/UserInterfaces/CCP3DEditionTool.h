#ifndef __H_C_CP3D_EDITION_TOOL_INCLUDED__
#define __H_C_CP3D_EDITION_TOOL_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>
#include <ICP3DInterface.h>
#include <ICP3DEditionTool.h>

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
class CCP3DEditionTool : public irr::IEventReceiver, public engine::ICP3DUpdate,
						 public ICP3DInterface, public ICP3DEditionTool
{
public:

	/// Constructor & Destructor
	CCP3DEditionTool(CCP3DEditorCore *editorCore);
	~CCP3DEditionTool();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);
	/// ICP3DUpdate
	void OnPostUpdate();
	/// ICP3DInterface
	void OnResize();
	irr::gui::IGUIElement *getElementToResize() { return Window; }

	/// ICP3DEditionTool
	irr::gui::IGUITab *addTab(const irr::core::stringc name);
	
	void clearTabs();
	void clear();

	void addSeparator(irr::gui::IGUITab *tab);

	void setNewZone(irr::gui::IGUITab *tab, irr::core::stringw name);
	
	SCP3DInterfaceData addField(irr::gui::IGUITab *tab, irr::gui::EGUI_ELEMENT_TYPE type, ICP3DEditionToolCallback callback = ICP3D_EDITION_TOOL_DEFAULT_CB);

	/// Utils
	void createDefaultControllers();
	bool addController(irr::scene::ESCENE_NODE_TYPE type, ICP3DEditionToolController *controller);
	const EditionToolControllerNode::Node *getControllersForType(irr::scene::ESCENE_NODE_TYPE type);
	void applyForControllerType(irr::scene::ESCENE_NODE_TYPE type, void *data);

private:
	/// Irrlicht
	irr::gui::IGUIEnvironment *Gui;
	irr::video::IVideoDriver *Driver;
	irr::gui::ICursorControl *CursorControl;

	/// CP3D
	CCP3DEditorCore *EditorCore;

	/// Datas
	irr::s32 WindowWidth;
	bool NewZone;

	/// GUI
	irr::gui::IGUIWindow *Window;
	irr::gui::IGUITabControl *TabCtrl;

	irr::core::array<ui::CGUIPanel *> Panels;

	irr::core::map<irr::scene::ESCENE_NODE_TYPE, irr::core::array<ICP3DEditionToolController *>> EditionTools;
	irr::scene::ESCENE_NODE_TYPE LastSceneNodeType;
	irr::s32 LastSelectedTab;

	/// Methods
	irr::s32 getElementPositionOffset(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);
	irr::s32 getPanelWidth(irr::gui::IGUIElement *panel);

	/// Creators
	SCP3DInterfaceData createTextBoxField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);
	SCP3DInterfaceData createListBoxField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);
	SCP3DInterfaceData createComboBoxField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);
	SCP3DInterfaceData createTextureField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);
	SCP3DInterfaceData createCheckBoxField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);
	SCP3DInterfaceData createColorField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);
	SCP3DInterfaceData createViewportField(irr::gui::IGUITab *tab, ui::CGUIPanel *panel);

	irr::core::list<SCP3DInterfaceData> InterfaceDatas;

};

} /// End namespace cp3d

#endif