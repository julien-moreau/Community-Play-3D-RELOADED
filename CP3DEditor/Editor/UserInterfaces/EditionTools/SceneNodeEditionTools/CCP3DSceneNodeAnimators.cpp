
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

CCP3DSceneNodeAnimators::CCP3DSceneNodeAnimators(CCP3DEditorCore *editorCore) : EditorCore(editorCore), Panel(0), Attributes(0)
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

	if (!Animator)
		return;

	EditorCore->getCustomView()->getElementToResize()->setVisible(true);

	IVideoDriver *Driver = EditorCore->getRenderingEngine()->getVideoDriver();
	IGUIEnvironment *Gui = EditorCore->getRenderingEngine()->getGUIEnvironment();
	Panel = ((ui::CGUIPanel*)EditorCore->getCustomView()->getParentGUIElement());

	/// Create GUI interface
	Gui->addStaticText(L"Name :", rect<s32>(10, 5, 150, 25), true, true, Panel, -1, false);
	AnimatorName = Gui->addEditBox(stringw(Animator->getName()).c_str(), rect<s32>(150, 5, 300,25), true, Panel, -1);

	/// Create GUI interface from attributes
	if (Attributes)
		Attributes->drop();
	Attributes = EditorCore->getDevice()->getFileSystem()->createEmptyAttributes(Driver);
	animator->serializeAttributes(Attributes);

	AnimatorType = Animator->getType();

	s32 middle = Panel->getRelativePosition().getWidth() / 2;
	s32 loffset = createCommonInterface(middle);
	s32 roffset = 0;

	if (AnimatorType == ESNAT_FOLLOW_SPLINE || AnimatorType == ESNAT_TEXTURE)
		roffset = createVariableInterface(middle);

	/// Calculate separators
	s32 maxValue = core::s32_max(loffset, roffset);

	IGUIStaticText *separator = Gui->addStaticText(L"", rect<s32>(middle, 5, middle + 6, maxValue), false, false, Panel, -1, false);
	separator->setBackgroundColor(SColor(255, 64, 64, 64));
	separator = Gui->addStaticText(L"", rect<s32>(5, maxValue, Panel->getRelativePosition().getWidth() - 25, maxValue + 6), false, false, Panel, -1, false);
	separator->setBackgroundColor(SColor(255, 64, 64, 64));

	/// Finally calculate the new scrollbar for the custom view
	Panel->recalculateScrollBar();
}

s32 CCP3DSceneNodeAnimators::createCommonInterface(s32 middle) {
	IGUIEnvironment *Gui = EditorCore->getRenderingEngine()->getGUIEnvironment();

	s32 loffset = 30, roffset = 5;
	for (u32 i=0; i < Attributes->getAttributeCount(); i++) {

		stringw name = Attributes->getAttributeName(i);
		E_ATTRIBUTE_TYPE type = Attributes->getAttributeType(i);

		/// Giving the type, create user interface
		/// And give the element the attributes name
		/// If vector3d or 2d then add X, Y or Z at the end of the name.
		if (type == EAT_FLOAT || type == EAT_INT || type == EAT_STRING) {
			Gui->addStaticText(name.c_str(), rect<s32>(10, loffset, 150, loffset + 20), true, true, Panel, -1, false);
			if (type == EAT_INT)
				Gui->addEditBox(stringw(Attributes->getAttributeAsInt(i)).c_str(), rect<s32>(150, loffset, 300, loffset + 20), true, Panel, -1)->setName(name);
			else if (type == EAT_STRING)
				Gui->addEditBox(stringw(Attributes->getAttributeAsString(i)).c_str(), rect<s32>(150, loffset, 300, loffset + 20), true, Panel, -1)->setName(name);
			else
				Gui->addEditBox(stringw(Attributes->getAttributeAsFloat(i)).c_str(), rect<s32>(150, loffset, 300, loffset + 20), true, Panel, -1)->setName(name);
		}
		else if (type == EAT_VECTOR3D) { /// On the right side
			Gui->addStaticText(name.c_str(), rect<s32>(middle + 10, roffset, middle + 140, roffset + 20), true, true, Panel, -1, false);

			Gui->addEditBox(stringw(Attributes->getAttributeAsVector3d(i).X).c_str(), rect<s32>(middle + 140, roffset, middle + 290, roffset + 20), true, Panel, -1)->setName(name + "X");
			Gui->addEditBox(stringw(Attributes->getAttributeAsVector3d(i).Y).c_str(), rect<s32>(middle + 140, roffset + 20, middle + 290, roffset + 40), true, Panel, -1)->setName(name + "Y");
			Gui->addEditBox(stringw(Attributes->getAttributeAsVector3d(i).Z).c_str(), rect<s32>(middle + 140, roffset + 40, middle + 290, roffset + 60), true, Panel, -1)->setName(name + "Z");

			Gui->addStaticText(stringw("X").c_str(), rect<s32>(middle + 290, roffset, middle + 310, roffset + 20), true, true, Panel, -1, false);
			Gui->addStaticText(stringw("Y").c_str(), rect<s32>(middle + 290, roffset + 20, middle + 310, roffset + 40), true, true, Panel, -1, false);
			Gui->addStaticText(stringw("Z").c_str(), rect<s32>(middle + 290, roffset + 40, middle + 310, roffset + 60), true, true, Panel, -1, false);
			roffset += 70;
			continue;
		}
		else if (type == EAT_BOOL) {
			Gui->addCheckBox(Attributes->getAttributeAsBool(i), rect<s32>(10, loffset, 150, loffset + 20), Panel, -1, name.c_str())->setName(name);
		}
		/*
		Adding a new type :
		else if (type == OTHER_TYPE) {
			Gui->addStaticText(blablabla);
			Gui->addEditBox(value, rect, etc.)->setName(name); /// For example and sets the name of the edit box to be used in events manager
		}
		*/
		else {
			continue; /// The type wasn't used
		}

		loffset += 30;
	}

	return core::s32_max(loffset, roffset);
}

s32 CCP3DSceneNodeAnimators::createVariableInterface(s32 middle) {
	IGUIEnvironment *Gui = EditorCore->getRenderingEngine()->getGUIEnvironment();
	s32 offset = 0;

	core::list<IGUIElement *>::ConstIterator it = Panel->getChildren().begin();
	for (; it != Panel->getChildren().end(); ++it) {
		IGUIElement *e = *it;
		if (e->getRelativePosition().UpperLeftCorner.X < middle)
			e->move(vector2di(0, 30));
		if (e->getRelativePosition().LowerRightCorner.Y > offset && e != Panel->getScrollBar())
			offset = e->getRelativePosition().LowerRightCorner.Y + 5;
	}

	stringw text = "Add Point";
	if (AnimatorType == ESNAT_TEXTURE)
		text = "Add Texture";

	Gui->addButton(rect<s32>(10, 5, 300, 25), Panel, -1, text.c_str())->setID((s32)AnimatorType);

	return offset;
}

bool CCP3DSceneNodeAnimators::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		
		IGUIElement *caller = event.GUIEvent.Caller;
		EGUI_EVENT_TYPE type = event.GUIEvent.EventType;
		if (caller && caller->getParent() == Panel && Animator) {

			if (type == EGET_CHECKBOX_CHANGED || type == EGET_EDITBOX_CHANGED)
			{
				stringc name = caller->getName();
				stringc text = caller->getText();
				E_ATTRIBUTE_TYPE attributeType = Attributes->getAttributeType(name.c_str());
				const c8 lastChar = name[name.size() -1];

				if (lastChar == 'X' || lastChar == 'Y' || lastChar == 'Z') { /// it is a vector2D or vector3D
					name = name.subString(0, name.size() - 1);
					attributeType = Attributes->getAttributeType(name.c_str());
					f32 value = core::fast_atof(stringc(caller->getText()).c_str());

					if (attributeType == EAT_VECTOR3D) {
						vector3df v = Attributes->getAttributeAsVector3d(name.c_str());
						if (lastChar == 'X')
							v.X = value;
						else if (lastChar == 'Y')
							v.Y = value;
						else
							v.Z = value;

						Attributes->setAttribute(name.c_str(), v);
					}
					else if (attributeType == EAT_VECTOR2D) {
						vector2df v = Attributes->getAttributeAsVector2d(name.c_str());
						if (lastChar == 'X')
							v.X = value;
						else
							v.Y = value;

						Attributes->setAttribute(name.c_str(), v);
					}
				}
				else {

					if (caller->getType() == EGUIET_CHECK_BOX) {
						Attributes->setAttribute(name.c_str(), ((IGUICheckBox*)caller)->isChecked());
					}

					else if (caller->getType() == EGUIET_EDIT_BOX) {

						f32 value = core::fast_atof(stringc(caller->getText()).c_str());
						if (attributeType == EAT_FLOAT)
							Attributes->setAttribute(name.c_str(), value);
						else if (attributeType == EAT_STRING)
							Attributes->setAttribute(name.c_str(), text.c_str());
						else if (attributeType == EAT_INT)
							Attributes->setAttribute(name.c_str(), (s32)value);

					}

				}
				
				Animator->deserializeAttributes(Attributes);

				return true;
			}
			else if (type == EGET_BUTTON_CLICKED) {
				ESCENE_NODE_ANIMATOR_TYPE type = (ESCENE_NODE_ANIMATOR_TYPE)caller->getID();
				s32 elementCount = 1;
				stringc name = "Point"; /// Default is Follow Spline

				if (type == ESNAT_TEXTURE)
					name = "Texture";

				for (u32 i=0; i < Attributes->getAttributeCount(); i++) {
					stringc n = Attributes->getAttributeName(i);
					if (n.find(name.c_str()) != -1)
						elementCount++;
				}

				name += elementCount;

				if (type == ESNAT_FOLLOW_SPLINE) {
					vector3df v(0.f);
					Attributes->addVector3d(name.c_str(), v);
				}
				else if (type == ESNAT_TEXTURE) {
					ITexture *t = 0;
					Attributes->addTexture(name.c_str(), t);
				}

				Animator->deserializeAttributes(Attributes);
				setAnimator(Animator);

				return true;
			}
			else
				return false;
		}

	}

	return false;
}

} /// End namespace cp3d
