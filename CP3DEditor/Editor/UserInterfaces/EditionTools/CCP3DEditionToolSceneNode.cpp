
#include "stdafx.h"
#include "../../Core/CCP3DEditorCore.h"
#include "../../GUIElements/CGUIManager.h"
#include "../CCP3DCustomView.h"
#include "../CCP3DEditionTool.h"
#include "SceneNodeEditionTools/CCP3DSceneNodeAnimators.h"

#include "CCP3DEditionToolSceneNode.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolSceneNode::CCP3DEditionToolSceneNode(CCP3DEditorCore *editorCore) : EditorCore(editorCore), CurrentMaterialID(0)
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

CCP3DEditionToolSceneNode::~CCP3DEditionToolSceneNode() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DEditionToolSceneNode::createInterface() {
	/// Tabs
	GeneralTab = EditionTool->addTab("General");
	if (SceneNode->getType() != ESNT_LIGHT)
		MaterialTab = EditionTool->addTab("Materials");
	AnimatorsTab = EditionTool->addTab("Animators");

	/// General
	EditionTool->setNewZone(GeneralTab, "General");
	SceneNodeName = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Name :"));
	SceneNodeID = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("ID :"));
	SceneNodeVisible = EditionTool->addField(GeneralTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Visible"));

	EditionTool->addSeparator(GeneralTab);
	SceneNodeMaterialGeneral = EditionTool->addField(GeneralTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("General Material Type"));

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

	EditionTool->setNewZone(GeneralTab, "Rendering");
	SceneNodeSetShadowed = EditionTool->addField(GeneralTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Shadowed"));
	SceneNodeShadowType = EditionTool->addField(GeneralTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("Shadowing :"));
	SceneNodeFilterType = EditionTool->addField(GeneralTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("Filter type :"));
	{
		if (SceneNode->getType() == ESNT_LIGHT)
			SceneNodeSetShadowed.CheckBox->setEnabled(false);
		for (u32 i=0; i < rendering::ESM_COUNT; i++)
			SceneNodeShadowType.ComboBox->addItem(stringw(rendering::ShadowModeNames[i]).c_str());
		for (u32 i=0; i < rendering::EFT_COUNT; i++)
			SceneNodeFilterType.ComboBox->addItem(stringw(rendering::FilterTypeNames[i]).c_str());
	}

	if (SceneNode->getType() == ESNT_ANIMATED_MESH) {
		SceneNodeAnimationSpeed = EditionTool->addField(GeneralTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Animation Speed :"));
	}

	/// Materials
	if (SceneNode->getType() != ESNT_LIGHT) {
		/// General
		EditionTool->setNewZone(MaterialTab, "General");
		MaterialName = EditionTool->addField(MaterialTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Name :"));
		MaterialSelector = EditionTool->addField(MaterialTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("Material :"));

		/// Colors
		EditionTool->setNewZone(MaterialTab, "Colors");
		MaterialDiffuseColor = EditionTool->addField(MaterialTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Diffuse Color :"));
		MaterialEmissiveColor = EditionTool->addField(MaterialTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Emissive Color :"));
		MaterialSpecularColor = EditionTool->addField(MaterialTab, EGUIET_COLOR_SELECT_DIALOG, DefaultEditionToolCallback("Specular Color :"));

		/// Parameters
		EditionTool->setNewZone(MaterialTab, "Parameters");
		MaterialMatType = EditionTool->addField(MaterialTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("Material Type :"));
		{
			u32 i = 0;
			while (sBuiltInMaterialTypeNames[i] != 0) {
				MaterialMatType.ComboBox->addItem(stringw(sBuiltInMaterialTypeNames[i]).make_upper().c_str());
				SceneNodeMaterialGeneral.ComboBox->addItem(stringw(sBuiltInMaterialTypeNames[i]).make_upper().c_str());
				i++;
			}
		}
		MaterialShininess = EditionTool->addField(MaterialTab, EGUIET_EDIT_BOX, DefaultEditionToolCallback("Shininess :"));

		/// Textures
		EditionTool->setNewZone(MaterialTab, "Textures");
		for (u32 i=0; i < irr::video::MATERIAL_MAX_TEXTURES; i++) {
			stringw txt = stringw("Texture layer ") + stringw(i) + stringw(" :");
			MaterialTextures[i] = EditionTool->addField(MaterialTab, EGUIET_IMAGE, DefaultEditionToolCallback(txt));
			if (i != irr::video::MATERIAL_MAX_TEXTURES - 1)
				EditionTool->addSeparator(MaterialTab);
		}

		/// Flags
		EditionTool->setNewZone(MaterialTab, "Flags");
		MaterialNormalizeNormals = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Normalize Normals"));
		MaterialFrontFaceCulling = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Front Face Culling"));
		MaterialBackFaceCulling = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Back Face Culling"));
		MaterialLighting = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Lighting"));
		MaterialAntialiasing = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Antialiasing"));
		MaterialBilinearFilter = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Bilinear Filter"));
		MaterialTrilinearFilter = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Trilinear Filter"));
		MaterialAnisotropicFilter = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Anisotropic Filter"));
		MaterialUseMipMaps = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Use MipMaps"));
		MaterialZWrite = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("ZWrite enable"));
		MaterialZBuffer = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("ZBuffer"));
		MaterialColorMask = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Color Mask"));
		MaterialColorMaterial = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Color Material"));
		MaterialPolygonOffset = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Polygon Offset"));

		/// Rendering
		EditionTool->setNewZone(MaterialTab, "Polygon Offset");
		MaterialPolygonOffsetDirectionCombo = EditionTool->addField(MaterialTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("Polygon Offset Direction :"));
		{
			u32 i = 0;
			while (PolygonOffsetDirectionNames[i] != 0) {
				MaterialPolygonOffsetDirectionCombo.ComboBox->addItem(stringw(PolygonOffsetDirectionNames[i]).c_str());
				i++;
			}
		}
		MaterialPolygonOffsetFactorCombo = EditionTool->addField(MaterialTab, EGUIET_COMBO_BOX, DefaultEditionToolCallback("Polygon Offset Factor :"));
		{
			for (u32 i=0; i < 8; i++)
				MaterialPolygonOffsetFactorCombo.ComboBox->addItem(stringw(i).c_str());
		}

		EditionTool->setNewZone(MaterialTab, "Antialiasing mode");
		u32 i = 0;
		while (AntiAliasingModeNames[i] != 0) {
			MaterialAntiAliasMode[i] = EditionTool->addField(MaterialTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback(AntiAliasingModeNames[i]));
			i++;
		}
	}

	/// Animators
	EditionTool->setNewZone(AnimatorsTab, "Animators");
	SceneNodeAnimators = EditionTool->addField(AnimatorsTab, EGUIET_LIST_BOX, DefaultEditionToolCallback("Animators :"));
	
}

void CCP3DEditionToolSceneNode::configure() {
	/// General
	SceneNodeName.TextBox->setText(stringw(SceneNode->getName()).c_str());
	SceneNodeID.TextBox->setText(stringw(SceneNode->getID()).c_str());
	SceneNodeVisible.CheckBox->setChecked(SceneNode->isVisible());

	SceneNodePositionX.TextBox->setText(stringw(SceneNode->getPosition().X).c_str());
	SceneNodePositionY.TextBox->setText(stringw(SceneNode->getPosition().Y).c_str());
	SceneNodePositionZ.TextBox->setText(stringw(SceneNode->getPosition().Z).c_str());

	SceneNodeRotationX.TextBox->setText(stringw(SceneNode->getRotation().X).c_str());
	SceneNodeRotationY.TextBox->setText(stringw(SceneNode->getRotation().Y).c_str());
	SceneNodeRotationZ.TextBox->setText(stringw(SceneNode->getRotation().Z).c_str());

	SceneNodeScaleX.TextBox->setText(stringw(SceneNode->getScale().X).c_str());
	SceneNodeScaleY.TextBox->setText(stringw(SceneNode->getScale().Y).c_str());
	SceneNodeScaleZ.TextBox->setText(stringw(SceneNode->getScale().Z).c_str());

	SceneNodeSetShadowed.CheckBox->setChecked(Handler->isNodeShadowed(SceneNode));
	{
		SceneNodeShadowType.ComboBox->setSelected(Handler->getShadowModeForNode(SceneNode));
		SceneNodeShadowType.ComboBox->setEnabled(SceneNodeSetShadowed.CheckBox->isChecked());
		SceneNodeFilterType.ComboBox->setSelected(Handler->getFilterTypeForNode(SceneNode));
		SceneNodeFilterType.ComboBox->setEnabled(SceneNodeSetShadowed.CheckBox->isChecked());
	}

	if (SceneNode->getType() == ESNT_ANIMATED_MESH) {
		SceneNodeAnimationSpeed.TextBox->setText(stringw(((IAnimatedMeshSceneNode *)SceneNode)->getAnimationSpeed()).c_str());
	}

	/// Materials
	if (SceneNode->getType() != ESNT_LIGHT) {
		/// General
		MaterialName.TextBox->setText(stringw(SceneNode->getMaterial(CurrentMaterialID).Name).c_str());
		MaterialSelector.ComboBox->clear();
		for (u32 i=0; i < SceneNode->getMaterialCount(); i++) {
			MaterialSelector.ComboBox->addItem(stringw(SceneNode->getMaterial(i).Name).c_str());
		}
		MaterialSelector.ComboBox->setSelected(CurrentMaterialID);

		/// Colors
		MaterialDiffuseColor.ColorData.ColorElement->setColor(SceneNode->getMaterial(CurrentMaterialID).DiffuseColor);
		MaterialEmissiveColor.ColorData.ColorElement->setColor(SceneNode->getMaterial(CurrentMaterialID).EmissiveColor);
		MaterialSpecularColor.ColorData.ColorElement->setColor(SceneNode->getMaterial(CurrentMaterialID).SpecularColor);

		/// Textures
		for (u32 i=0; i < irr::video::MATERIAL_MAX_TEXTURES; i++) {
			MaterialTextures[i].TextureData.Image->setImage(SceneNode->getMaterial(CurrentMaterialID).TextureLayer[i].Texture);
			if (MaterialTextures[i].TextureData.Image->getImage())
				MaterialTextures[i].TextureData.EditBoxPath->setText(stringw(MaterialTextures[i].TextureData.Image->getImage()->getName().getPath()).c_str());
		}

		/// Paramters
		rendering::ICP3DRenderingEngine *renderingEngine = EditorCore->getRenderingEngine();
		core::map<E_MATERIAL_TYPE, E_MATERIAL_TYPE>::Iterator it = renderingEngine->Materials.getIterator();
		for (; !it.atEnd(); it++) {
			if (it.getNode()->getValue() == SceneNode->getMaterial(CurrentMaterialID).MaterialType) {
				MaterialMatType.ComboBox->setSelected(it.getNode()->getKey());
				break;
			}
		}
		MaterialShininess.TextBox->setText(stringw(SceneNode->getMaterial(CurrentMaterialID).Shininess).c_str());

		/// Flags
		MaterialNormalizeNormals.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).NormalizeNormals);
		MaterialFrontFaceCulling.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).FrontfaceCulling);
		MaterialBackFaceCulling.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).BackfaceCulling);
		MaterialLighting.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).Lighting);
		MaterialAntialiasing.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_ANTI_ALIASING));
		MaterialBilinearFilter.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_BILINEAR_FILTER));
		MaterialTrilinearFilter.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_TRILINEAR_FILTER));
		MaterialAnisotropicFilter.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_ANISOTROPIC_FILTER));
		MaterialUseMipMaps.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).UseMipMaps);
		MaterialZWrite.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).ZWriteEnable);
		MaterialZBuffer.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_ZBUFFER));
		MaterialColorMask.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_COLOR_MASK));
		MaterialColorMaterial.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_COLOR_MATERIAL));
		MaterialPolygonOffset.CheckBox->setChecked(SceneNode->getMaterial(CurrentMaterialID).getFlag(EMF_POLYGON_OFFSET));

		MaterialPolygonOffsetDirectionCombo.ComboBox->setSelected(SceneNode->getMaterial(CurrentMaterialID).PolygonOffsetDirection);
		MaterialPolygonOffsetFactorCombo.ComboBox->setSelected(SceneNode->getMaterial(CurrentMaterialID).PolygonOffsetFactor);

		for (u32 i=0; i < 7; i++)
			MaterialAntiAliasMode[i].CheckBox->setChecked(false);
		if (SceneNode->getMaterial(CurrentMaterialID).AntiAliasing & EAAM_OFF) MaterialAntiAliasMode[0].CheckBox->setChecked(true);
		if (SceneNode->getMaterial(CurrentMaterialID).AntiAliasing & EAAM_SIMPLE) MaterialAntiAliasMode[1].CheckBox->setChecked(true);
		if (SceneNode->getMaterial(CurrentMaterialID).AntiAliasing & EAAM_QUALITY) MaterialAntiAliasMode[2].CheckBox->setChecked(true);
		if (SceneNode->getMaterial(CurrentMaterialID).AntiAliasing & EAAM_LINE_SMOOTH) MaterialAntiAliasMode[3].CheckBox->setChecked(true);
		if (SceneNode->getMaterial(CurrentMaterialID).AntiAliasing & EAAM_POINT_SMOOTH) MaterialAntiAliasMode[4].CheckBox->setChecked(true);
		if (SceneNode->getMaterial(CurrentMaterialID).AntiAliasing & EAAM_FULL_BASIC) MaterialAntiAliasMode[5].CheckBox->setChecked(true);
		if (SceneNode->getMaterial(CurrentMaterialID).AntiAliasing & EAAM_ALPHA_TO_COVERAGE) MaterialAntiAliasMode[6].CheckBox->setChecked(true);
	}

	/// Animators
	core::list<ISceneNodeAnimator *>::ConstIterator animator = SceneNode->getAnimators().begin();
	for (; animator != SceneNode->getAnimators().end(); ++animator) {
		ISceneNodeAnimator *anim = (*animator);
		SceneNodeAnimators.ListData.List->addItem(stringw(anim->getName()).c_str());
	}
}

void CCP3DEditionToolSceneNode::apply() {
	/// General
	SceneNode->setName(SceneNodeName.TextBox->getText());
	SceneNode->setID((s32)core::fast_atof(stringc(SceneNodeID.TextBox->getText()).c_str()));
	SceneNode->setVisible(SceneNodeVisible.CheckBox->isChecked());

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

	rendering::E_FILTER_TYPE filterType = (rendering::E_FILTER_TYPE)SceneNodeFilterType.ComboBox->getSelected();
	rendering::E_SHADOW_MODE shadowMode = (rendering::E_SHADOW_MODE)SceneNodeShadowType.ComboBox->getSelected();
	Handler->removeShadowFromNode(SceneNode);
	if (SceneNodeSetShadowed.CheckBox->isChecked())
		Handler->addShadowToNode(SceneNode, filterType, shadowMode);

	if (SceneNode->getType() == ESNT_ANIMATED_MESH) {
		((IAnimatedMeshSceneNode *)SceneNode)->setAnimationSpeed(core::fast_atof(stringc(SceneNodeAnimationSpeed.TextBox->getText()).c_str()));
	}

	/// Materials
	if (SceneNode->getType() != ESNT_LIGHT) {
		SceneNode->getMaterial(CurrentMaterialID).Name = MaterialName.TextBox->getText();
		SceneNode->getMaterial(CurrentMaterialID).MaterialType = EditorCore->getRenderingEngine()->Materials[(E_MATERIAL_TYPE)MaterialMatType.ComboBox->getSelected()];
		SceneNode->getMaterial(CurrentMaterialID).Shininess = core::fast_atof(stringc(MaterialShininess.TextBox->getText()).c_str());

		/// Colors
		SceneNode->getMaterial(CurrentMaterialID).DiffuseColor = MaterialDiffuseColor.ColorData.ColorElement->getColor().toSColor();
		SceneNode->getMaterial(CurrentMaterialID).EmissiveColor = MaterialEmissiveColor.ColorData.ColorElement->getColor().toSColor();
		SceneNode->getMaterial(CurrentMaterialID).SpecularColor = MaterialSpecularColor.ColorData.ColorElement->getColor().toSColor();

		{
			/// Flags
			SceneNode->getMaterial(CurrentMaterialID).NormalizeNormals = MaterialNormalizeNormals.CheckBox->isChecked();
			SceneNode->getMaterial(CurrentMaterialID).FrontfaceCulling = MaterialFrontFaceCulling.CheckBox->isChecked();
			SceneNode->getMaterial(CurrentMaterialID).BackfaceCulling = MaterialBackFaceCulling.CheckBox->isChecked();
			SceneNode->getMaterial(CurrentMaterialID).Lighting = MaterialLighting.CheckBox->isChecked();
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_ANTI_ALIASING, MaterialAntialiasing.CheckBox->isChecked());
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_BILINEAR_FILTER, MaterialBilinearFilter.CheckBox->isChecked());
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_TRILINEAR_FILTER, MaterialTrilinearFilter.CheckBox->isChecked());
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_ANISOTROPIC_FILTER, MaterialAnisotropicFilter.CheckBox->isChecked());
			SceneNode->getMaterial(CurrentMaterialID).UseMipMaps = MaterialUseMipMaps.CheckBox->isChecked();
			SceneNode->getMaterial(CurrentMaterialID).ZWriteEnable = MaterialZWrite.CheckBox->isChecked();
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_ZBUFFER, MaterialZBuffer.CheckBox->isChecked());
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_COLOR_MASK, MaterialColorMask.CheckBox->isChecked());
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_COLOR_MATERIAL, MaterialColorMaterial.CheckBox->isChecked());
			SceneNode->getMaterial(CurrentMaterialID).setFlag(EMF_POLYGON_OFFSET, MaterialPolygonOffset.CheckBox->isChecked());

			SceneNode->getMaterial(CurrentMaterialID).PolygonOffsetDirection = (E_POLYGON_OFFSET)MaterialPolygonOffsetDirectionCombo.ComboBox->getSelected();
			SceneNode->getMaterial(CurrentMaterialID).PolygonOffsetFactor = (u8)MaterialPolygonOffsetFactorCombo.ComboBox->getSelected();

			u8 antiAliasMode = EAAM_SIMPLE;
			if (MaterialAntiAliasMode[0].CheckBox->isChecked()) {
				antiAliasMode = EAAM_OFF;
				for (u32 i=1; i < 7; i++)
					MaterialAntiAliasMode[i].CheckBox->setChecked(false);
			} else {
				if (MaterialAntiAliasMode[1].CheckBox->isChecked()) antiAliasMode = antiAliasMode | EAAM_SIMPLE;
				if (MaterialAntiAliasMode[2].CheckBox->isChecked()) antiAliasMode = antiAliasMode | EAAM_QUALITY;
				if (MaterialAntiAliasMode[3].CheckBox->isChecked()) antiAliasMode = antiAliasMode | EAAM_LINE_SMOOTH;
				if (MaterialAntiAliasMode[4].CheckBox->isChecked()) antiAliasMode = antiAliasMode | EAAM_POINT_SMOOTH;
				if (MaterialAntiAliasMode[5].CheckBox->isChecked()) antiAliasMode = antiAliasMode | EAAM_FULL_BASIC;
				if (MaterialAntiAliasMode[6].CheckBox->isChecked()) antiAliasMode = antiAliasMode | EAAM_ALPHA_TO_COVERAGE;
				SceneNode->getMaterial(CurrentMaterialID).AntiAliasing = antiAliasMode;
			}

		}
	}

}

bool CCP3DEditionToolSceneNode::OnEvent(const SEvent &event) {
	
	/// Textures (Materials)
	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

			/// Textures (Materials)
			for (u32 i=0; i < video::MATERIAL_MAX_TEXTURES; i++) {
				/// Browse
				if (event.GUIEvent.Caller == MaterialTextures[i].TextureData.BrowseButton) {
					MaterialTextures[i].TextureData.BrowseDialog = EditorCore->getGUIManager()->createFileOpenDialog("Select Texture...", 0, ui::ICP3DFileSelector::EFST_OPEN_DIALOG);
					return true;
				}
				else if (event.GUIEvent.Caller == MaterialTextures[i].TextureData.RemoveButton) {
					MaterialTextures[i].TextureData.Image->setImage(0);
					SceneNode->getMaterial(CurrentMaterialID).TextureLayer[i].Texture = 0;
					return true;
				}
			}

			if (event.GUIEvent.Caller == SceneNodeAnimators.ListData.AddButton) {
				ICursorControl *cursorControl = EditorCore->getDevice()->getCursorControl();

				rect<s32> rectangle = rect<s32>(cursorControl->getPosition().X, cursorControl->getPosition().Y, 
											    cursorControl->getPosition().X+300, cursorControl->getPosition().Y+200);
				AnimatorsCtxMenu = Gui->addContextMenu(rectangle, 0, -1);

				for (u32 i=0; i < ESNAT_COUNT; i++) {
					stringw name = Smgr->getDefaultSceneNodeAnimatorFactory()->getCreateableSceneNodeAnimatorTypeName(i);
					AnimatorsCtxMenu->addItem(name.c_str(), i, true, false, false, false);
				}

				return true;
			}

			else if (event.GUIEvent.Caller == SceneNodeAnimators.ListData.RemoveButton) {
				s32 index = SceneNodeAnimators.ListData.List->getSelected();
				if (index == -1)
					return true;

				core::list<ISceneNodeAnimator *>::ConstIterator it = SceneNode->getAnimators().begin();
				u32 i = 0;
				for (; it != SceneNode->getAnimators().end(); ++it) {
					if (i == index) {
						SceneNode->removeAnimator(*it);
						break;
					}
					i++;
				}

				SceneNodeAnimators.ListData.List->removeItem(index);
				AnimatorsController->setAnimator(0);

				return true;
			}

		}

		else if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {

			if (event.GUIEvent.Caller == AnimatorsCtxMenu) {
				ISceneNodeAnimatorFactory *factory = Smgr->getDefaultSceneNodeAnimatorFactory();
				ISceneNodeAnimator *anim = factory->createSceneNodeAnimator((ESCENE_NODE_ANIMATOR_TYPE)AnimatorsCtxMenu->getSelectedItem(), SceneNode);

				SceneNodeAnimators.ListData.List->addItem(stringw(AnimatorsCtxMenu->getItemText(AnimatorsCtxMenu->getSelectedItem())).c_str());

				return true;
			}
		}

		else if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED || event.GUIEvent.EventType == EGET_LISTBOX_SELECTED_AGAIN) {

			if (event.GUIEvent.Caller == SceneNodeAnimators.ListData.List) {
				s32 index = SceneNodeAnimators.ListData.List->getSelected();
				if (index == -1)
					return true;

				u32 i = 0;
				core::list<ISceneNodeAnimator *>::ConstIterator it = SceneNode->getAnimators().begin();
				for (; it != SceneNode->getAnimators().end(); ++it) {
					if (i == index)
						break;
					i++;
				}

				AnimatorsController->setAnimator(*it);

				return true;
			}
		}

		else if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {

			/// Textures(Materials)
			for (u32 i=0; i < video::MATERIAL_MAX_TEXTURES; i++) {
				/// Browse
				if (event.GUIEvent.Caller == MaterialTextures[i].TextureData.BrowseDialog) {
					ITexture *tex = Driver->getTexture(MaterialTextures[i].TextureData.BrowseDialog->getFileName());
					if (!tex)
						return true;

					MaterialTextures[i].TextureData.Image->setImage(tex);
					SceneNode->getMaterial(CurrentMaterialID).TextureLayer[i].Texture = tex;
					return true;
				}
			}
		}

		else if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == MaterialSelector.ComboBox) {
				CurrentMaterialID = MaterialSelector.ComboBox->getSelected();
				if (CurrentMaterialID < 0)
					CurrentMaterialID = 0;
				configure();
				return true;
			}
			else if (event.GUIEvent.Caller == SceneNodeMaterialGeneral.ComboBox) {
				SceneNode->setMaterialType(EditorCore->getRenderingEngine()->Materials[(E_MATERIAL_TYPE)SceneNodeMaterialGeneral.ComboBox->getSelected()]);
				return true;
			}
		}

		else if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (event.GUIEvent.Caller == MaterialName.TextBox) {
				const u32 selected = MaterialSelector.ComboBox->getSelected();
				MaterialSelector.ComboBox->clear();
				for (u32 i=0; i < SceneNode->getMaterialCount(); i++)
					MaterialSelector.ComboBox->addItem(stringw(SceneNode->getMaterial(i).Name).c_str());
				MaterialSelector.ComboBox->setSelected(selected);
				return true;
			}
		}

	}

	return false;
}

} /// End namespace cp3d
