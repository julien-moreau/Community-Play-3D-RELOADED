
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"

#include "CCP3DEditionToolBillboardSceneNode.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolBillboardSceneNode::CCP3DEditionToolBillboardSceneNode(CCP3DEditorCore *editorCore) : EditorCore(editorCore), LightSceneNode(0)
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

CCP3DEditionToolBillboardSceneNode::~CCP3DEditionToolBillboardSceneNode() {

}

void CCP3DEditionToolBillboardSceneNode::createInterface() {
	/// Tabs
	BillboardTab = EditionTool->addTab("Billboard");
	
	/// Shadow light
	EditionTool->setNewZone(BillboardTab, "Size");
	BillboardSizeW = EditionTool->addField(BillboardTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Size W"));
	BillboardSizeH = EditionTool->addField(BillboardTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Size H"));
	EditionTool->addSeparator(BillboardTab);
	EditionTool->setNewZone(BillboardTab, "Color");
	BillboardColorTop = EditionTool->addField(BillboardTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Color Top"));
	BillboardColorBottom = EditionTool->addField(BillboardTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Color Bottom"));
}

void CCP3DEditionToolBillboardSceneNode::configure() {
	IBillboardSceneNode *node = (IBillboardSceneNode *)SceneNode;
	SColor top, bottom;

	node->getColor(top, bottom);
	BillboardColorTop.ColorData.ColorElement->setColor(top);
	BillboardColorBottom.ColorData.ColorElement->setColor(bottom);
}

void CCP3DEditionToolBillboardSceneNode::apply() {
	IBillboardSceneNode *node = (IBillboardSceneNode *)SceneNode;

	node->setColor(BillboardColorTop.ColorData.ColorElement->getColor().toSColor(), BillboardColorBottom.ColorData.ColorElement->getColor().toSColor());

}

bool CCP3DEditionToolBillboardSceneNode::OnEvent(const SEvent &event) {
	return false;
}

} /// End namespace cp3d
