
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"

#include "CCP3DEditionToolSceneManager.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolSceneManager::CCP3DEditionToolSceneManager(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	/// Configure
	EditionTool = editorCore->getEditionTool();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	Smgr = editorCore->getRenderingEngine()->getSceneManager();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	Rengine = editorCore->getRenderingEngine();

	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DEditionToolSceneManager::~CCP3DEditionToolSceneManager() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DEditionToolSceneManager::createInterface() {
	/// Tabs
	SceneTab = EditionTool->addTab("Scene");

	/// Scene
	EditionTool->setNewZone(SceneTab, "General");
	AmbiantColor = EditionTool->addField(SceneTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Ambiant Color"));
}

void CCP3DEditionToolSceneManager::configure() {
	/// Scene
	AmbiantColor.ColorData.ColorElement->setColor(Smgr->getAmbientLight());
}

void CCP3DEditionToolSceneManager::apply() {
	/// Scene
	{
		Smgr->setAmbientLight(AmbiantColor.ColorData.ColorElement->getColor());
		Rengine->getHandler()->setAmbientColor(AmbiantColor.ColorData.ColorElement->getColor().toSColor());
	}
}

bool CCP3DEditionToolSceneManager::OnEvent(const SEvent &event) {

	return false;
}

} /// End namespace cp3d
