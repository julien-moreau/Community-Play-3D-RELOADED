
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../CCP3DEditionTool.h"

#include "CCP3DEditionToolSceneNode.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolSceneNode::CCP3DEditionToolSceneNode(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	/// Configure
	EditionTool = editorCore->getEditionTool();
}

CCP3DEditionToolSceneNode::~CCP3DEditionToolSceneNode() {

}

void CCP3DEditionToolSceneNode::createInterface() {
	/// General
	GeneralTab = EditionTool->addTab("General");

	SceneNodeName = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Name :"));

	EditionTool->setNewZone(GeneralTab, "Transforms");

	SceneNodePositionX = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Position X :"));
	SceneNodePositionY = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Position Y :"));
	SceneNodePositionZ = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Position Z :"));
	EditionTool->addSeparator(GeneralTab);
	SceneNodeRotationX = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Rotation X :"));
	SceneNodeRotationY = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Rotation Y :"));
	SceneNodeRotationZ = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Rotation Z :"));
	EditionTool->addSeparator(GeneralTab);
	SceneNodeScaleX = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Scaling X :"));
	SceneNodeScaleY = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Scaling Y :"));
	SceneNodeScaleZ = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Scaling Z :"));
}

void CCP3DEditionToolSceneNode::configure() {
	/// General
	SceneNodeName.TextBox->setText(stringw(SceneNode->getName()).c_str());

	SceneNodePositionX.TextBox->setText(stringw(SceneNode->getPosition().X).c_str());
	SceneNodePositionY.TextBox->setText(stringw(SceneNode->getPosition().Y).c_str());
	SceneNodePositionZ.TextBox->setText(stringw(SceneNode->getPosition().Z).c_str());

	SceneNodeRotationX.TextBox->setText(stringw(SceneNode->getRotation().X).c_str());
	SceneNodeRotationY.TextBox->setText(stringw(SceneNode->getRotation().Y).c_str());
	SceneNodeRotationZ.TextBox->setText(stringw(SceneNode->getRotation().Z).c_str());

	SceneNodeScaleX.TextBox->setText(stringw(SceneNode->getScale().X).c_str());
	SceneNodeScaleY.TextBox->setText(stringw(SceneNode->getScale().Y).c_str());
	SceneNodeScaleZ.TextBox->setText(stringw(SceneNode->getScale().Z).c_str());
}

void CCP3DEditionToolSceneNode::apply() {
	/// General
	SceneNode->setName(SceneNodeName.TextBox->getText());

	vector3df position(core::fast_atof(stringc(SceneNodePositionX.TextBox->getText()).c_str()), /// Position
					   core::fast_atof(stringc(SceneNodePositionY.TextBox->getText()).c_str()),
					   core::fast_atof(stringc(SceneNodePositionZ.TextBox->getText()).c_str()));
	SceneNode->setPosition(position);

	vector3df rotation(core::fast_atof(stringc(SceneNodeRotationX.TextBox->getText()).c_str()), /// Rotation
					   core::fast_atof(stringc(SceneNodeRotationY.TextBox->getText()).c_str()),
					   core::fast_atof(stringc(SceneNodeRotationZ.TextBox->getText()).c_str()));
	SceneNode->setRotation(rotation);

	vector3df scale(core::fast_atof(stringc(SceneNodeScaleX.TextBox->getText()).c_str()), /// Scaling
					core::fast_atof(stringc(SceneNodeScaleY.TextBox->getText()).c_str()),
					core::fast_atof(stringc(SceneNodeScaleZ.TextBox->getText()).c_str()));
	SceneNode->setScale(scale);
}

bool CCP3DEditionToolSceneNode::OnEvent(const SEvent &event) {

	return false;
}

} /// End namespace cp3d
