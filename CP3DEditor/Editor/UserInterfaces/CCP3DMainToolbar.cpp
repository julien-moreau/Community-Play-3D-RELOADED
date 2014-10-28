
#include "stdafx.h"
#include "../Core/CCP3DEditorCore.h"

#include "CCP3DMainToolbar.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DMainToolbar::CCP3DMainToolbar(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
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

	Toolbar->addButton(-1, L"", L"Open project...", Driver->getTexture("GUI/open.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Save project...", Driver->getTexture("GUI/save.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Undo...", Driver->getTexture("GUI/undo.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Redo...", Driver->getTexture("GUI/redo.png"), 0, false, true);

	Toolbar->addButton(-1, L"", L"", Driver->getTexture("GUI/separator.png"), false, true)->setVisible(false);

	Toolbar->addButton(-1, L"", L"Set node's position", Driver->getTexture("GUI/position.png"), 0, true, true);
	Toolbar->addButton(-1, L"", L"Set node's rotation", Driver->getTexture("GUI/rotation.png"), 0, true, true);
	Toolbar->addButton(-1, L"", L"Set node's scale", Driver->getTexture("GUI/scale.png"), 0, true, true);
	Toolbar->addButton(-1, L"World", L"Set world transform", Driver->getTexture("GUI/worldtransform.png"), 0, true, true);

	Toolbar->addButton(-1, L"", L"", Driver->getTexture("GUI/separator.png"), false, true)->setVisible(false);

	Toolbar->addButton(-1, L"", L"Manage animators...", Driver->getTexture("GUI/animators.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Manage materials...", Driver->getTexture("GUI/materials.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Manage post-processes", Driver->getTexture("GUI/postprocesses.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Manage scenarios...", Driver->getTexture("GUI/scenarios.png"), 0, false, true);
	Toolbar->addButton(-1, L"", L"Manage particle systems...", Driver->getTexture("GUI/particles.png"), 0, false, true);

}

CCP3DMainToolbar::~CCP3DMainToolbar() {

}

bool CCP3DMainToolbar::OnEvent(const SEvent &event) {

	return false;
}

} /// End namespace cp3d
