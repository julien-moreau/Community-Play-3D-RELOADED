
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

	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DEditionToolBillboardSceneNode::~CCP3DEditionToolBillboardSceneNode() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DEditionToolBillboardSceneNode::createInterface() {
	/// Tabs
	BillboardTab = EditionTool->addTab("Billboard");
	
	/// Billboard
	EditionTool->setNewZone(BillboardTab, "Size");
	BillboardSizeW = EditionTool->addField(BillboardTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Size W"));
	BillboardSizeH = EditionTool->addField(BillboardTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Size H"));

	EditionTool->setNewZone(BillboardTab, "Color");
	BillboardColorTop = EditionTool->addField(BillboardTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Color Top"));
	BillboardColorBottom = EditionTool->addField(BillboardTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Color Bottom"));
}

void CCP3DEditionToolBillboardSceneNode::configure() {
	IBillboardSceneNode *node = (IBillboardSceneNode *)SceneNode;

	/// Color
	SColor top, bottom;
	node->getColor(top, bottom);
	BillboardColorTop.ColorData.ColorElement->setColor(top);
	BillboardColorBottom.ColorData.ColorElement->setColor(bottom);

	/// Size
	dimension2d<f32> size = node->getSize();
	BillboardSizeW.TextBox->setText(stringw(size.Width).c_str());
	BillboardSizeH.TextBox->setText(stringw(size.Height).c_str());
}

void CCP3DEditionToolBillboardSceneNode::apply() {
	IBillboardSceneNode *node = (IBillboardSceneNode *)SceneNode;

	/// Color
	SColorf colort = BillboardColorTop.ColorData.ColorElement->getColor().toSColor();
	SColorf colorb = BillboardColorBottom.ColorData.ColorElement->getColor().toSColor();

	node->setColor(colort.toSColor(), colorb.toSColor());

	/// Size
	dimension2d<f32> size;
	size.Width = core::fast_atof(stringc(BillboardSizeW.TextBox->getText()).c_str());
	size.Height = core::fast_atof(stringc(BillboardSizeH.TextBox->getText()).c_str());

	node->setSize(size);
}

bool CCP3DEditionToolBillboardSceneNode::OnEvent(const SEvent &event) {
	return false;
}

} /// End namespace cp3d
