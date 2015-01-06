
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"
#include "CCP3DEditionToolPostProcess.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

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
	EditorCore->getEngine()->getCustomUpdater()->removeCustomUpdate(this);
}

void CCP3DEditionToolPostProcess::createInterface() {
	/// Tabs
	PostProcessTab = EditionTool->addTab("Post-Processes");
	
	/// Parameters for IBillBoardTextSceneNode
	EditionTool->setNewZone(PostProcessTab, "Post-Processes");
	PostProcessesList = EditionTool->addField(PostProcessTab, EGUIET_LIST_BOX, DefaultEditionToolCallback("Post-Processes pipeline"));

	EditionTool->setNewZone(PostProcessTab, "Options");
	PostProcessAutomaticReload = EditionTool->addField(PostProcessTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Automatic Reload"));
}

void CCP3DEditionToolPostProcess::configure() {
	EditorCore->getEngine()->getCustomUpdater()->addCustomUpdate(this);

	IGUIListBox *list = PostProcessesList.ListData.List;
	// Enable or disable UI
	bool enable = enableUI();

	// Apply

	const u32 size = Handler->getPostProcessingRoutineSize();
	for (u32 i = 0; i < size; i++) {
		list->addItem(stringw(Handler->getPostProcessingRoutineName(i)).c_str());
		Changes[i] = getChangedTime(Handler->getPostProcessingRoutineName(i));
	}
}

void CCP3DEditionToolPostProcess::apply() {
	IGUIListBox *list = PostProcessesList.ListData.List;
	bool enable = enableUI();
}

void CCP3DEditionToolPostProcess::clear() {
	EditorCore->getEngine()->getCustomUpdater()->removeCustomUpdate(this);
	Changes.clear();
}

bool CCP3DEditionToolPostProcess::enableUI() {
	IGUIListBox *list = PostProcessesList.ListData.List;

	bool enable = list->getSelected() != -1;

	return enable;
}

void CCP3DEditionToolPostProcess::OnPreUpdate() {
	io::IFileSystem *fs = EditorCore->getDevice()->getFileSystem();

	for (u32 i = 0; i < Handler->getPostProcessingRoutineSize(); i++) {
		stringc path = Handler->getPostProcessingRoutineName(Handler->getPostProcessID(i));
		if (path == "")
			continue;

		time_t time;
		if (fs->existFile(path.c_str()))
			time = getChangedTime(path);
		else
			fs->existFile(EditorCore->getWorkingDirectory() + path.c_str());

		if (time < 0 || time == Changes[i])
			continue;

		rendering::IPostProcessingRenderCallback *callback = Handler->getPostProcessingCallback(i);
		s32 pp;
		if (fs->existFile(path.c_str()))
			pp = Handler->replacePostProcessAtIndex(i, path, callback);
		else
			pp = Handler->replacePostProcessAtIndex(i, EditorCore->getWorkingDirectory() + path, callback);

		if (pp)
			Changes[i] = time;
	}
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
					//PostProcessActivated.CheckBox->setChecked(Handler->isPostProcessActivated(list->getSelected()));
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
					Changes[Handler->getPostProcessingRoutineSize() - 1] = getChangedTime(path);
				}

				return true;
			}

		}
	}

	return false;
}

time_t CCP3DEditionToolPostProcess::getChangedTime(stringc filename) {
	struct stat buf;
	int result = stat(stringc(EditorCore->getWorkingDirectory() + filename).c_str(), &buf);
	if (result >= 0)
		return buf.st_mtime;
	return 0;
}

} /// End namespace cp3d
