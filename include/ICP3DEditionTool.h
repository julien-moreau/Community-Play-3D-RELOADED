#ifndef _H_ICP3D_EDITION_TOOL_INCLUDED__
#define _H_ICP3D_EDITION_TOOL_INCLUDED__

#include <irrlicht.h>
#include <functional>

namespace cp3d {

//! Custom GUI events for CP3D
enum E_INTERFACE_EVENT {
	EIE_NODE_ADDED = 0,
	EIE_NODE_REMOVED,
	EIE_NODE_SELECTED,
	EIE_NODE_CHANGED,

	EIE_TEXTURE_SELECTED,
	EIE_MATERIAL_SELECTED,

	EIE_COUNT,

	EIE_FORCE_32BIT = 0x7fffffff
};

//! Custom GUI elements types for CP3D
enum E_CP3DGUI_ELEMENT_TYPE {
	ECET_VIEW_PORT = irr::gui::EGUIET_COUNT,

	ECET_COUNT,

	ECET_FORCE_32BIT = 0x7fffffff
};

//! Structure that contains generated elements
struct SCP3DInterfaceData {

	//!Constructor
	SCP3DInterfaceData(irr::gui::EGUI_ELEMENT_TYPE type = irr::gui::EGUIET_ELEMENT) : Type(type)
	{ }

	//! Text element
	irr::gui::IGUIStaticText *TextElement;

	//! Element type
	irr::gui::EGUI_ELEMENT_TYPE Type;

	//! Structure that contains list elements
	struct SListData {
		irr::gui::IGUIButton *EditButton;
		irr::gui::IGUIButton *RemoveButton;
		irr::gui::IGUIButton *AddButton;
		irr::gui::IGUIListBox *List;
	};

	union {
		SListData ListData;
		irr::gui::IGUIEditBox *TextBox;
		irr::gui::IGUIComboBox *ComboBox;
	};

};

//! Callback typedef for ICP3DEditionTool::createField
typedef std::function<void(const SCP3DInterfaceData &data)> ICP3DEditionToolCallback;
#define ICP3D_EDITION_TOOL_DEFAULT_CB [&](const SCP3DInterfaceData &data) { }

//! Default ICP3DEditionTool::addField callback
//! \param text: the text to displayed by the element
static auto DefaultEditionToolCallback = [](irr::core::stringw text) -> ICP3DEditionToolCallback {
	ICP3DEditionToolCallback c = [=](SCP3DInterfaceData data) {
		data.TextElement->setText(text.c_str());
	};

	return c;
};

//! Main interface that Edition Tools will implement
/*
Example :

class CMyClass : public ICP3DEditionToolController {
public:
	CMyClass(ICP3DEditor *editor) : Editor(editor) {
		EditionTool = editor->getEditionTool();
	}
	~CMyClass() { }

	void createInterface() {
		Tab = EditionTool->addTab(L"General");
		Element = EditionTool->addField(Tab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Name :"));
	}

	void configure() {
		Element.TextBox->setText(SceneNode->getName());
	}

	void apply() {
		SceneNode->setName(stringc(Element.TextBox->getText()).c_str());
	}

private:
	ICP3DEditor *Editor;
	ICP3DEditionTool *EditionTool;
	IGUITab *Tab;
	SCP3DInterfaceData Element;
};
*/
class ICP3DEditionToolController {
public:

	//! Creates the interface using the ICP3DEditionTool
	virtual void createInterface() = 0;

	//! Fills the fields if edited node changed
	virtual void configure() = 0;

	//! Applies the parameters to the scene node
	virtual void apply() = 0;

	//! Sets the scene node to edit
	//! Called before ICP3DEditionToolController::createInterface()
	virtual void setSceneNode(irr::scene::ISceneNode *node) {
		SceneNode = node;
	}

	//! Returns the scene node of the edition tool
	virtual irr::scene::ISceneNode *getSceneNode() {
		return SceneNode;
	}

protected:

	irr::scene::ISceneNode *SceneNode;

};

//! Main CP3D Edition Tool interface
class ICP3DEditionTool {
public:

	//! Adds a new tab and returns its pointer
	//! \param name: the name of the tab
	virtual irr::gui::IGUITab *addTab(const irr::core::stringc name) = 0;

	//! Clears the tabs
	virtual void clearTabs() = 0;

	//! Sets a new zone by adding a text between zones
	//! \param tab: the tab containing the created zone
	//! \param name: the zone's name
	/*
	Example :

	ICP3DEditionTool *tool;
	IGUITab *tab = tool->addTab("Tab Name");

	tool->setNewZone(tab, "Zone with edit boxes");
	SCP3DInterfaceData editBox1 = tool->addField(tab, EGUIET_EDIT_BOX);
	SCP3DInterfaceData editBox2 = tool->addField(tab, EGUIET_EDIT_BOX);

	tool->setNewZone(tab, "Zone with combo boxes");
	SCP3DInterfaceData comboBox1 = tool->addField(tab, EGUIET_COMBO_BOX);
	SCP3DInterfaceData comboBox2 = tool->addField(tab, EGUIET_COMBO_BOX);

	Etc.
	*/
	virtual void setNewZone(irr::gui::IGUITab *tab, irr::core::stringw name) = 0;

	//! Adds a space between elements
	virtual void addSeparator(irr::gui::IGUITab *tab) = 0;

	//! Adds a field to the tab
	//! \param tab: the tab containing the created zone
	//! \param type: the field's type (irr::gui::EGUIET_COMBO_BOX, etc.)
	//! \param callback: callback called after creating the field ([](SCP3DInterfaceData data) { })
	SCP3DInterfaceData addField(irr::gui::IGUITab *tab, irr::gui::EGUI_ELEMENT_TYPE type, ICP3DEditionToolCallback callback = ICP3D_EDITION_TOOL_DEFAULT_CB);

	//! Adds a new controller to the edition tool and returns
	//! Returns false if the controller already exists
	//! \param type: the scene node type handled by the controller
	//! \param controller: pointer to the controller
	/*
	The CP3D edition tool can handle multiple controllers per type
	*/
	virtual bool addController(irr::scene::ESCENE_NODE_TYPE type, ICP3DEditionToolController *controller) = 0;

};

} /// End cp3d namespace

#endif