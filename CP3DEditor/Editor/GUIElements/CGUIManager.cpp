#include "Stdafx.h"
#include "CGUIManager.h"

#include "../Core/CCP3DEditorCore.h"

#include "GUIPanel/CGUIPanel.h"
#include "GUIFileSelector/CGUIFileSelector.h"
#include "GUIColorDialog/CGUIColorDialog.h"
#include "GUIViewPort/CGUIViewPort.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace video;

namespace cp3d {
namespace ui {

CGUIManager::CGUIManager(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	Device = editorCore->getDevice();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
}

CGUIManager::~CGUIManager()
{ }

void CGUIManager::centerWindow(IGUIWindow *window) {
	IVideoDriver *driver = Gui->getVideoDriver();
	rect<s32> &position = window->getRelativePosition();

	window->setRelativePosition(rect<s32>(
		driver->getScreenSize().Width / 2 - position.getWidth() / 2,
		driver->getScreenSize().Height / 2 - position.getHeight() / 2,
		driver->getScreenSize().Width / 2 + position.getWidth() / 2,
		driver->getScreenSize().Height / 2 + position.getHeight() / 2
	));
}

ICP3DViewPort *CGUIManager::createViewPort(rect<s32> rectangle, IGUIElement* parent, s32 id) {
	return new CGUIViewport(Gui, parent, id, rectangle);
}

ui::ICP3DFileSelector *CGUIManager::createFileOpenDialog(irr::core::stringw name, irr::gui::IGUIElement *parent, ui::ICP3DFileSelector::E_FILESELECTOR_TYPE type, bool modal) {
	ui::CGUIFileSelector *selector = new ui::CGUIFileSelector(name.c_str(), Gui, parent == 0 ? Gui->getRootGUIElement() : parent, -1, type);
	stringc WorkingDirectory = EditorCore->getWorkingDirectory();

	selector->setCustomDirectoryIcon(Driver->getTexture(WorkingDirectory + "GUI/open.png"));
	selector->setCustomFileIcon(Driver->getTexture(WorkingDirectory + "GUI/parameters.png"));

	selector->addPlacePaths(L"CP3D Directory", WorkingDirectory.c_str(), Driver->getTexture(WorkingDirectory + "GUI/parameters.png"));
	selector->addPlacePaths(L"Project Directory", EditorCore->getProjectDirectory().c_str(), Driver->getTexture(WorkingDirectory + "GUI/play_game.png"));
	selector->addPlacePaths(L"Opened Place", Device->getFileSystem()->getWorkingDirectory(), Driver->getTexture(WorkingDirectory + "Gui/open.png"));

	if (modal) {
		IGUIElement *modalElement = Gui->addModalScreen(parent);
		modalElement->addChild(selector);
	}

	return selector;
}

IGUIWindow *CGUIManager::createMessageBox(stringw title, stringw text, s32 flags, bool modal, ITexture *texture) {
	return Gui->addMessageBox(title.c_str(), text.c_str(), modal, flags, 0, -1, texture);
}

} // end namespace ui
} // end namespace cp3d


