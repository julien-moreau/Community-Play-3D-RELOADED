
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"

#include "CCP3DEditionToolLightSceneNode.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolLightSceneNode::CCP3DEditionToolLightSceneNode(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	/// Configure
	EditionTool = editorCore->getEditionTool();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	Smgr = editorCore->getRenderingEngine()->getSceneManager();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	Handler = editorCore->getRenderingEngine()->getHandler();

	AnimatorsController = new CCP3DSceneNodeAnimators(editorCore);

	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DEditionToolLightSceneNode::~CCP3DEditionToolLightSceneNode() {

}

void CCP3DEditionToolLightSceneNode::createInterface() {
	rendering::ICP3DRenderingEngine *rengine = EditorCore->getRenderingEngine();
	for (u32 i=0; i < rengine->getLightSceneNodeCount(); i++) {
		if (*rengine->getLightSceneNode(i) == SceneNode) {
			LightSceneNode = rengine->getLightSceneNode(i);
			break;
		}
	}

	/// Tabs
	LightTab = EditionTool->addTab("Light");
	
	/// Shadow light
	EditionTool->setNewZone(LightTab, "Shadow Light");
	ComputeShadows = EditionTool->addField(LightTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Compute Shadows"));

	/// Color
	EditionTool->setNewZone(LightTab, "Colors");
	DiffuseColor = EditionTool->addField(LightTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Diffuse Color"));
	SpecularColor = EditionTool->addField(LightTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Specular Color"));
	AmbiantColor = EditionTool->addField(LightTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Ambiant Color"));
}

void CCP3DEditionToolLightSceneNode::configure() {
	/// Shadow light
	ComputeShadows.CheckBox->setChecked(LightSceneNode->getShadowLight() != 0);

	/// Colros
	DiffuseColor.ColorData.ColorElement->setColor(LightSceneNode->getLightData().DiffuseColor);
	SpecularColor.ColorData.ColorElement->setColor(LightSceneNode->getLightData().SpecularColor);
	AmbiantColor.ColorData.ColorElement->setColor(LightSceneNode->getLightData().AmbientColor);
}

void CCP3DEditionToolLightSceneNode::apply() {

}

bool CCP3DEditionToolLightSceneNode::OnEvent(const SEvent &event) {

	return false;
}

} /// End namespace cp3d
