#ifndef _H_ICP3D_INTERFACE_INCLUDED__
#define _H_ICP3D_INTERFACE_INCLUDED__

#include <irrlicht.h>
#include <functional>

namespace cp3d {

struct SCP3DInterfaceData;
//! Callback typedef for ICP3DEditionTool::createField
typedef std::function<void(const SCP3DInterfaceData &data)> ICP3DEditionToolCallback;
#define ICP3D_EDITION_TOOL_DEFAULT_CB [&](const SCP3DInterfaceData &data) { }

enum E_INTERFACE_EVENT {
	EIE_NODE_ADDED = 0,
	EIE_NODE_REMOVED,

	EIE_NODE_SELECTED,
	EIE_NODE_CHANGED,

	EIE_TEXTURE_SELECTED,
	EIE_MATERIAL_SELECTED
};

enum E_CP3DGUI_ELEMENT_TYPE {
	ECET_VIEW_PORT = irr::gui::EGUIET_COUNT
};

//! Structure that contains generated elements
struct SCP3DInterfaceData {
	//!Constructor
	SCP3DInterfaceData(irr::gui::EGUI_ELEMENT_TYPE type) : Type(type) { }

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

/// Interface controller interface
class ICP3DInterface {
public:

	//! Called by ICP3DInterfaceController when the element
	//! was resized
	virtual void OnResize() = 0;

	//! Returns the element to resize.
	//! Can be a window, panel, etc.
	virtual irr::gui::IGUIElement *getElementToResize() = 0;

};

} /// End cp3d namespace

#endif