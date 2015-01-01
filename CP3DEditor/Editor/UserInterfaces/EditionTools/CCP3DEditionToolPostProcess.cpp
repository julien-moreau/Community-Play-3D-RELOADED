
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"

#include "CCP3DEditionToolPostProcess.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolPostProcess::CCP3DEditionToolPostProcess(CCP3DEditorCore *editorCore) : EditorCore(editorCore),
OpenPostProcessDialog(0)
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

CCP3DEditionToolPostProcess::~CCP3DEditionToolPostProcess() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DEditionToolPostProcess::createInterface() {
	/// Tabs
	PostProcessTab = EditionTool->addTab("Post-Processes");
	
	/// Parameters for IBillBoardTextSceneNode
	EditionTool->setNewZone(PostProcessTab, "Post-Processes");
	PostProcessesList = EditionTool->addField(PostProcessTab, EGUIET_LIST_BOX, DefaultEditionToolCallback("Post-Processes pipeline"));

	EditionTool->setNewZone(PostProcessTab, "Options");
	PostProcessActivated = EditionTool->addField(PostProcessTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Activated"));
}

void CCP3DEditionToolPostProcess::configure() {
	IGUIListBox *list = PostProcessesList.ListData.List;

	// Enable or disable UI
	bool enable = enableUI();

	// Apply
	if (enable) {
		PostProcessActivated.CheckBox->setChecked(Handler->isPostProcessActivated(list->getSelected()));
	}

	const u32 size = Handler->getPostProcessingRoutineSize();
	for (u32 i = 0; i < size; i++) {
		list->addItem(stringw(Handler->getPostProcessingRoutineName(i)).c_str());
	}
}

void CCP3DEditionToolPostProcess::apply() {
	IGUIListBox *list = PostProcessesList.ListData.List;
	bool enable = enableUI();

	if (enable) {
		Handler->setPostProcessActivated(list->getSelected(), PostProcessActivated.CheckBox->isChecked());
	}
}

void CCP3DEditionToolPostProcess::clear() {

}

bool CCP3DEditionToolPostProcess::enableUI() {
	IGUIListBox *list = PostProcessesList.ListData.List;

	bool enable = list->getSelected() != -1;
	PostProcessActivated.CheckBox->setEnabled(enable);

	return enable;
}

bool CCP3DEditionToolPostProcess::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

			/// Post-Process list
			if (event.GUIEvent.Caller == PostProcessesList.ListData.AddButton) {
				OpenPostProcessDialog = EditorCore->createFileOpenDialog("Choose post-process file", 0, ui::ICP3DFileSelector::EFST_OPEN_DIALOG);
				OpenPostProcessDialog->addFileFilter(L"FX files", L"fx", Driver->getTexture("postprocesses.png"));
				return true;
			}
			else if (event.GUIEvent.Caller == PostProcessesList.ListData.RemoveButton) {
				IGUIListBox *list = PostProcessesList.ListData.List;
				rendering::ICP3DHandler *handler = Rengine->getHandler();

				handler->removePostProcessingEffect(list->getSelected());
				list->removeItem(list->getSelected());
				return true;
			}

		}

		// Perform UI configuration before applying
		else if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if (event.GUIEvent.Caller == PostProcessesList.ListData.List) {
				bool enable = enableUI();
				if (enable) {
					IGUIListBox *list = PostProcessesList.ListData.List;
					PostProcessActivated.CheckBox->setChecked(Handler->isPostProcessActivated(list->getSelected()));
				}
			}
		}

		else if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {

			if (event.GUIEvent.Caller == OpenPostProcessDialog) {
				stringc path = OpenPostProcessDialog->getFileName();
				irr::s32 pp = EditorCore->getRenderingEngine()->getHandler()->addPostProcessingEffectFromFile(path, 0);
				if (!pp)
					EditorCore->createMessageBox("", "", EMBF_OK, true, Driver->getTexture("error.png"));
				else {
					PostProcessesList.ListData.List->addItem(stringw(path).remove(EditorCore->getWorkingDirectory()).c_str());
				}

				return true;
			}

		}
	}

	return false;
}

} /// End namespace cp3d
