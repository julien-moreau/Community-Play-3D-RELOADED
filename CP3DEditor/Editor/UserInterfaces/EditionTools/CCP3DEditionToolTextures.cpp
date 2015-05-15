
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../../GUIElements/CGUIManager.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"
#include "CCP3DEditionToolTextures.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolTextures::CCP3DEditionToolTextures(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	/// Configure
	EditionTool = editorCore->getEditionTool();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	Smgr = editorCore->getRenderingEngine()->getSceneManager();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	Rengine = editorCore->getRenderingEngine();
	Handler = Rengine->getHandler();

	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DEditionToolTextures::~CCP3DEditionToolTextures() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DEditionToolTextures::createInterface() {
	/// Tabs
	TexturesTab = EditionTool->addTab("Post-Processes");
	
	/// Parameters for IBillBoardTextSceneNode
	EditionTool->setNewZone(TexturesTab, "Textures");
	TexturesList = EditionTool->addField(TexturesTab, EGUIET_LIST_BOX, DefaultEditionToolCallback("Textures List"));

	EditionTool->setNewZone(TexturesTab, "Preview");
	Texture = EditionTool->addField(TexturesTab, EGUIET_IMAGE, DefaultEditionToolCallback("Texture"));
}

void CCP3DEditionToolTextures::configure() {
	IGUIListBox *list = TexturesList.ListData.List;

	// Apply
	const u32 size = Driver->getTextureCount();
	for (u32 i = 0; i < size; i++) {
		list->addItem(stringw(Driver->getTextureByIndex(i)->getName()).c_str());
	}
}

void CCP3DEditionToolTextures::apply() {

}

bool CCP3DEditionToolTextures::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if (event.GUIEvent.Caller == TexturesList.ListData.List) {
				s32 selected = TexturesList.ListData.List->getSelected();
				if (selected < 0 || selected > TexturesList.ListData.List->getItemCount())
					return true;

				Texture.TextureData.Image->setImage(Driver->getTextureByIndex(selected));
			}
		}
	}

	return false;
}


} /// End namespace cp3d
