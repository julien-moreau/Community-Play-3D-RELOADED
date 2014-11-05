
#include "stdafx.h"
#include "../../../Core/CCP3DEditorCore.h"
#include "../../CCP3DCustomView.h"
#include "../../../GUIElements/GUIPanel/CGUIPanel.h"

#include "CCP3DSceneNodeAnimators.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;
using namespace io;

namespace cp3d {

CCP3DSceneNodeAnimators::CCP3DSceneNodeAnimators(CCP3DEditorCore *editorCore) : EditorCore(editorCore), Panel(0)
{
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DSceneNodeAnimators::~CCP3DSceneNodeAnimators() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DSceneNodeAnimators::setAnimator(irr::scene::ISceneNodeAnimator *animator) {
	/// Set animator and clear custom view
	Animator = animator;
	EditorCore->getCustomView()->clearView();
	EditorCore->getCustomView()->getElementToResize()->setVisible(true);

	IVideoDriver *Driver = EditorCore->getRenderingEngine()->getVideoDriver();
	IGUIEnvironment *Gui = EditorCore->getRenderingEngine()->getGUIEnvironment();
	Panel = ((ui::CGUIPanel*)EditorCore->getCustomView()->getParentGUIElement());

	/// Create GUI interface
	Gui->addStaticText(L" Name :", rect<s32>(5, 5, 150, 25), false, true, Panel, -1, false);
	AnimatorName = Gui->addEditBox(stringw(Animator->getName()).c_str(), rect<s32>(150, 5, 300,25), true, Panel, -1);

	/// Create GUI interface from attributes
	io::IAttributes *attributes = EditorCore->getDevice()->getFileSystem()->createEmptyAttributes(Driver);
	animator->serializeAttributes(attributes);

	s32 offset = 30;
	for (u32 i=0; i < attributes->getAttributeCount(); i++) {

		stringw name = attributes->getAttributeName(i);
		E_ATTRIBUTE_TYPE type = attributes->getAttributeType(i);

		if (type == EAT_FLOAT) {
			Gui->addStaticText(name.c_str(), rect<s32>(5, offset, 150, offset + 20), false, true, Panel, -1, false);
			IGUIEditBox *e = Gui->addEditBox(stringw(attributes->getAttributeAsFloat(i)).c_str(), rect<s32>(150, offset, 300, offset + 20), true, Panel, -1);
			e->setName(name);
		}
		else if (type == EAT_VECTOR3D) {
			Gui->addStaticText(name.c_str(), rect<s32>(5, offset, 150, offset + 20), false, true, Panel, -1, false);

		}
		else {
			continue;
		}

		offset += 20;
	}

	/// Finally calculate the new scrollbar for the custom view
	Panel->recalculateScrollBar();
}

bool CCP3DSceneNodeAnimators::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		
		IGUIElement *caller = event.GUIEvent.Caller;
		if (caller && caller->getParent() == Panel) {



			return true;
		}

	}

	return false;
}

} /// End namespace cp3d
