
#include "stdafx.h"
#include "../Core/CCP3DEditorCore.h"

#include "CCP3DToolsToolbar.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DToolsToolbar::CCP3DToolsToolbar(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	/// Configure
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);

	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	/// Gui
	Toolbar = Gui->addToolBar(0, -1);
	rect<s32> position = Toolbar->getRelativePosition();
	position.UpperLeftCorner.Y -= 2;
	Toolbar->setRelativePosition(position);

	Toolbar->addButton(-1, L"", L"Play game...", Driver->getTexture("GUI/play_game.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Create a render...", Driver->getTexture("GUI/render.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Show console", Driver->getTexture("GUI/console.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Show parameters", Driver->getTexture("GUI/parameters.png"), 0, false, true);

	Toolbar->addButton(-1, L"", L"", Driver->getTexture("GUI/separator.png"), false, true)->setVisible(false);

	Toolbar->addButton(-1, L"", L"Show wireframe", Driver->getTexture("GUI/wireframe.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Show pointcloud", Driver->getTexture("GUI/pointcloud.png"), 0, false, true);

	Toolbar->addButton(-1, L"", L"", Driver->getTexture("GUI/separator.png"), false, true)->setVisible(false);

	/// Others
}

CCP3DToolsToolbar::~CCP3DToolsToolbar() {

}

bool CCP3DToolsToolbar::OnEvent(const SEvent &event) {

	return false;
}

} /// End namespace cp3d
