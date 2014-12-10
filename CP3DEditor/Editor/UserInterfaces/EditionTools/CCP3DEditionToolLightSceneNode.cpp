
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

CCP3DEditionToolLightSceneNode::CCP3DEditionToolLightSceneNode(CCP3DEditorCore *editorCore) : EditorCore(editorCore), LightSceneNode(0)
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

CCP3DEditionToolLightSceneNode::~CCP3DEditionToolLightSceneNode() {

}

void CCP3DEditionToolLightSceneNode::createInterface() {
	for (u32 i=0; i < Rengine->getLightSceneNodeCount(); i++) {
		if (*Rengine->getLightSceneNode(i) == SceneNode) {
			LightSceneNode = Rengine->getLightSceneNode(i);
			break;
		}
	}

	/// Tabs
	LightTab = EditionTool->addTab("Light");
	
	/// Shadow light
	EditionTool->setNewZone(LightTab, "Shadow Light");
	ComputeShadows = EditionTool->addField(LightTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Compute Shadows"));
	ComputeRoundedSpot = EditionTool->addField(LightTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Rounded Spot"));
	EditionTool->addSeparator(LightTab);
	ShadowMapFarValue = EditionTool->addField(LightTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Far Value"));
	ShadowMapNearValue = EditionTool->addField(LightTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Near Value"));
	ShadowMapFov = EditionTool->addField(LightTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Front Of View"));
	ShadowMapDirectional = EditionTool->addField(LightTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Directional"));
	ShadowMapResolution = EditionTool->addField(LightTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("Resolution"));
	{
		for (u32 i=1; i < 13; i++) {
			ShadowMapResolution.ComboBox->addItem(stringw((s32)pow(2, i)).c_str());
		}
	}

	EditionTool->setNewZone(LightTab, "Shadow Map Textures");
	ShadowMapTexture = EditionTool->addField(LightTab, EGUIET_IMAGE, DefaultEditionToolCallback("Shadow Map Texture"));

	/// Normal Mapping
	EditionTool->setNewZone(LightTab, "Normal Mapping");
	ComputeNormalMapping = EditionTool->addField(LightTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Compute Normal Mapping"));
	NormalMappingStrength = EditionTool->addField(LightTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Light Strength"));

	/// Color
	EditionTool->setNewZone(LightTab, "Colors");
	DiffuseColor = EditionTool->addField(LightTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Diffuse Color"));
	SpecularColor = EditionTool->addField(LightTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Specular Color"));
	AmbiantColor = EditionTool->addField(LightTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Ambiant Color"));
}

void CCP3DEditionToolLightSceneNode::configure() {
	/// Shadow light
	ShadowMapTexture.TextureData.BrowseButton->setText(L"View higher");
	ShadowMapTexture.TextureData.RemoveButton->setVisible(false);

	ComputeShadows.CheckBox->setChecked(LightSceneNode->getShadowLight() != 0);
	if (LightSceneNode->isComputingShadows()) {
		ComputeRoundedSpot.CheckBox->setEnabled(true);
		ShadowMapFarValue.TextBox->setEnabled(true);
		ShadowMapNearValue.TextBox->setEnabled(true);
		ShadowMapFov.TextBox->setEnabled(true);
		ShadowMapDirectional.CheckBox->setEnabled(true);
		ShadowMapResolution.ComboBox->setEnabled(true);

		ComputeRoundedSpot.CheckBox->setChecked(LightSceneNode->getShadowLight()->usingRoundSpotLight());
		ShadowMapFarValue.TextBox->setText(stringw(LightSceneNode->getShadowLight()->getFarValue()).c_str());
		ShadowMapNearValue.TextBox->setText(stringw(LightSceneNode->getShadowLight()->getNearValue()).c_str());
		ShadowMapFov.TextBox->setText(stringw(LightSceneNode->getShadowLight()->getFrontOfView() * (f32)core::RADTODEG64).c_str());
		ShadowMapDirectional.CheckBox->setChecked(LightSceneNode->getShadowLight()->isDirectional());
		{ /// ShadowMapResolution
			const u32 resolution = LightSceneNode->getShadowLight()->getShadowMapResolution();
			u32 i = 1;
			while (pow(2, i) != resolution)
				i++;
			ShadowMapResolution.ComboBox->setSelected(i - 1);
		}

		ShadowMapTexture.TextureData.Image->setImage(Rengine->getHandler()->getShadowMapTexture(LightSceneNode->getShadowLight()->getShadowMapResolution(), false));
	}
	else {
		ComputeRoundedSpot.CheckBox->setEnabled(false);
		ShadowMapFarValue.TextBox->setEnabled(false);
		ShadowMapNearValue.TextBox->setEnabled(false);
		ShadowMapFov.TextBox->setEnabled(false);
		ShadowMapDirectional.CheckBox->setEnabled(false);
		ShadowMapResolution.ComboBox->setEnabled(false);
	}

	rendering::ICP3DLightSceneNode *light;
	ILightSceneNode *lightIrrlicht = *light;

	/// Normal Mapping
	ComputeNormalMapping.CheckBox->setChecked(LightSceneNode->isComputingNormalMapping());
	NormalMappingStrength.TextBox->setText(stringw(LightSceneNode->getLightStrength()).c_str());

	/// Colors
	DiffuseColor.ColorData.ColorElement->setColor(LightSceneNode->getLightData().DiffuseColor);
	SpecularColor.ColorData.ColorElement->setColor(LightSceneNode->getLightData().SpecularColor);
	AmbiantColor.ColorData.ColorElement->setColor(LightSceneNode->getLightData().AmbientColor);
}

void CCP3DEditionToolLightSceneNode::apply() {
	/// Shadow light
	Rengine->setLightSceneNodeComputeShadows(LightSceneNode, ComputeShadows.CheckBox->isChecked());
	if (LightSceneNode->isComputingShadows()) {
		LightSceneNode->getShadowLight()->setUseRoundSpotLight(ComputeRoundedSpot.CheckBox->isChecked());

		LightSceneNode->getShadowLight()->setFarValue(core::fast_atof(stringc(ShadowMapFarValue.TextBox->getText()).c_str()));
		LightSceneNode->getShadowLight()->setNearValue(core::fast_atof(stringc(ShadowMapNearValue.TextBox->getText()).c_str()));
		LightSceneNode->getShadowLight()->setFOV(core::fast_atof(stringc(ShadowMapFov.TextBox->getText()).c_str()) * (f32)core::DEGTORAD64);
		LightSceneNode->getShadowLight()->setDirectionalLight(ShadowMapDirectional.CheckBox->isChecked());
		{
			u32 resol = (u32)core::fast_atof(stringc(ShadowMapResolution.ComboBox->getItem(ShadowMapResolution.ComboBox->getSelected())).c_str());
			LightSceneNode->getShadowLight()->setShadowMapResolution(resol);
		}
	}

	/// Normal Mapping
	LightSceneNode->setComputeNormalMapping(ComputeNormalMapping.CheckBox->isChecked());
	LightSceneNode->setLightStrength(core::fast_atof(stringc(NormalMappingStrength.TextBox->getText()).c_str()));

	/// Colors
	LightSceneNode->setLightColor(DiffuseColor.ColorData.ColorElement->getColor());
	LightSceneNode->getLightData().SpecularColor = SpecularColor.ColorData.ColorElement->getColor();
	LightSceneNode->getLightData().AmbientColor = AmbiantColor.ColorData.ColorElement->getColor();
}

bool CCP3DEditionToolLightSceneNode::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

			if (event.GUIEvent.Caller == ShadowMapTexture.TextureData.BrowseButton && LightSceneNode && LightSceneNode->isComputingShadows()) {
				CCP3DCustomView *customView = EditorCore->getCustomView();
				customView->clearView();
				customView->getElementToResize()->setVisible(true);
				
				rect<s32> r = customView->getParentGUIElement()->getRelativePosition();
				IGUIImage *img = Gui->addImage(rect<s32>(5, 5, r.getWidth() - 20, r.getHeight() - 5), customView->getParentGUIElement(), -1, 0, false);
				img->setScaleImage(true);
				img->setImage(Rengine->getHandler()->getShadowMapTexture(LightSceneNode->getShadowLight()->getShadowMapResolution(), false));

				return true;
			}

		}
	}

	return false;
}

} /// End namespace cp3d
