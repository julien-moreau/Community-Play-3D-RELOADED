
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"

#include "CCP3DEditionToolTextSceneNode.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolTextSceneNode::CCP3DEditionToolTextSceneNode(CCP3DEditorCore *editorCore) : EditorCore(editorCore), LightSceneNode(0)
{
	/// Configure
	EditionTool = editorCore->getEditionTool();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	Smgr = editorCore->getRenderingEngine()->getSceneManager();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	Rengine = editorCore->getRenderingEngine();

	AnimatorsController = new CCP3DSceneNodeAnimators(editorCore);

	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DEditionToolTextSceneNode::~CCP3DEditionToolTextSceneNode() {

}

void CCP3DEditionToolTextSceneNode::createInterface() {
	/// Tabs
	TextTab = EditionTool->addTab("Text");
	
	/// Parameters for IBillBoardTextSceneNode
	EditionTool->setNewZone(TextTab, "Text");
	TextColor = EditionTool->addField(TextTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Text color"));
	Text = EditionTool->addField(TextTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Text"));
}

void CCP3DEditionToolTextSceneNode::configure() {
	IBillboardTextSceneNode *node = (IBillboardTextSceneNode *)SceneNode;

	/// Draw text
	Text.TextBox->setText(node->getText());

	/// Draw color
	TextColor.ColorData.ColorElement->setColor(node->getTextColor());
}

void CCP3DEditionToolTextSceneNode::apply() {
	IBillboardTextSceneNode *node = (IBillboardTextSceneNode *)SceneNode;

	/// Set color
	node->setTextColor(TextColor.ColorData.ColorElement->getColor().toSColor());

	/// Set text
	node->setText(Text.TextBox->getText());

}

bool CCP3DEditionToolTextSceneNode::OnEvent(const SEvent &event) {
	return false;
}

} /// End namespace cp3d
