
#include "stdafx.h"
#include "CCP3DSaverOpener.h"

#include "../Core/CCP3DEditorCore.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

/// Ctor
CCP3DSaverOpener::CCP3DSaverOpener(CCP3DEditorCore *editorCore) : EditorCore(editorCore), SaveDialog(0),
	OpenDialog(0), Saved(false), AskSaveWindow(0)
{
	EditorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

/// Dtor
CCP3DSaverOpener::~CCP3DSaverOpener() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DSaverOpener::save() {
	stringc pd = EditorCore->getProjectDirectory();
	if (pd == "")
		SaveDialog = EditorCore->getGUIManager()->createFileOpenDialog("Save the current project as...", 0, ui::ICP3DFileSelector::EFST_SAVE_DIALOG, true);
	else
		saveFile(pd);
}

void CCP3DSaverOpener::open() {
	/// Implement "save before"
	if (!Saved) {
		IVideoDriver *driver = EditorCore->getDevice()->getVideoDriver();
		AskSaveWindow = EditorCore->getGUIManager()->createMessageBox("Save", "Save Before ?", EMBF_YES | EMBF_NO, true,
			driver->getTexture(EditorCore->getWorkingDirectory() + "GUI/info.png"));
	}
	else {
		OpenDialog = EditorCore->getGUIManager()->createFileOpenDialog("Open project...", 0, ui::ICP3DFileSelector::EFST_OPEN_DIALOG, true);
	}
}

void CCP3DSaverOpener::saveFile(stringc filename) {
	engine::ICP3DExporter *exporter = EditorCore->getEngine()->createExporter();
	
	/// Add external attributes

	/// Export all :)
	exporter->exportProject(filename);

	/// Finish
	Saved = true;
	EditorCore->setProjectDirectory(filename);
	delete exporter;
}

void CCP3DSaverOpener::openFile(stringc filename) {
	engine::ICP3DExporter *exporter = EditorCore->getEngine()->createExporter();
	exporter->importProject(filename);

	/// Finish
	Saved = true;
	EditorCore->setProjectDirectory(filename);
	delete exporter;
}

bool CCP3DSaverOpener::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT || event.EventType == EET_KEY_INPUT_EVENT || event.EventType == EET_MOUSE_INPUT_EVENT
		|| event.EventType == EET_USER_EVENT)
	{
		Saved = false;
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_MESSAGEBOX_YES) {
			if (event.GUIEvent.Caller == AskSaveWindow) {
				save();
				return true;
			}
		}
		else if (event.GUIEvent.EventType == EGET_MESSAGEBOX_NO) {
			if (event.GUIEvent.Caller == AskSaveWindow) {
				open();
				return true;
			}
		}

		else if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {

			if (event.GUIEvent.Caller == SaveDialog) {
				saveFile(SaveDialog->getFileName());
				return true;
			}
			else if (event.GUIEvent.Caller == OpenDialog) {
				openFile(OpenDialog->getFileName());
				return true;
			}

		}
	}

	return false;
}

} /// End namespace cp3d
