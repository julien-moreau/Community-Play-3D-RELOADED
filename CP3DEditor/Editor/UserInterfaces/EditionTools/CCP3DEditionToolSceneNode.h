#ifndef __H_C_CP3D_EDITION_TOOL_MESH_INCLUDED__
#define __H_C_CP3D_EDITION_TOOL_MESH_INCLUDED__

#include <irrlicht.h>
#include <ICP3DEditionTool.h>
#include <ICP3DHandler.h>

namespace cp3d {

class CCP3DEditionTool;
class CCP3DEditorCore;
class CCP3DSceneNodeAnimators;

/*
Class that will manage the editon for ESNT_MESH type scene
nodes.
*/
class CCP3DEditionToolSceneNode : public irr::IEventReceiver, public ICP3DEditionToolController {
public:

	/// Constructor & Destructor
	CCP3DEditionToolSceneNode(CCP3DEditorCore *editorCore);
	~CCP3DEditionToolSceneNode();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// ICP3DEditionToolController
	void createInterface();
	void configure();
	void apply();

private:
	/// Irrlicht
	irr::video::IVideoDriver *Driver;
	irr::scene::ISceneManager *Smgr;
	irr::gui::IGUIEnvironment *Gui;

	/// CP3D
	CCP3DEditorCore *EditorCore;
	CCP3DEditionTool *EditionTool;
	CCP3DSceneNodeAnimators *AnimatorsController;

	/// Rendering
	rendering::ICP3DHandler *Handler;

	/// GUI
	irr::gui::IGUITab *GeneralTab;
	irr::gui::IGUITab *MaterialTab;
	irr::gui::IGUITab *AnimatorsTab;

	irr::gui::IGUIContextMenu *AnimatorsCtxMenu;

	/// General
	SCP3DInterfaceData SceneNodeName;
	SCP3DInterfaceData SceneNodeID;
	SCP3DInterfaceData SceneNodeVisible;
	SCP3DInterfaceData SceneNodeMaterialGeneral;
	SCP3DInterfaceData SceneNodePositionX, SceneNodePositionY, SceneNodePositionZ;
	SCP3DInterfaceData SceneNodeRotationX, SceneNodeRotationY, SceneNodeRotationZ;
	SCP3DInterfaceData SceneNodeScaleX, SceneNodeScaleY, SceneNodeScaleZ;
	SCP3DInterfaceData SceneNodeSetShadowed, SceneNodeShadowType, SceneNodeFilterType;

	SCP3DInterfaceData SceneNodeAnimationSpeed;

	/// Materials
	SCP3DInterfaceData MaterialName;
	SCP3DInterfaceData MaterialSelector;

	SCP3DInterfaceData MaterialDiffuseColor; /// Colors
	SCP3DInterfaceData MaterialEmissiveColor;
	SCP3DInterfaceData MaterialSpecularColor;

	SCP3DInterfaceData MaterialTextures[irr::video::MATERIAL_MAX_TEXTURES];
	SCP3DInterfaceData MaterialMatType;
	SCP3DInterfaceData MaterialShininess;

	SCP3DInterfaceData MaterialNormalizeNormals; /// Flags
	SCP3DInterfaceData MaterialBackFaceCulling, MaterialFrontFaceCulling;
	SCP3DInterfaceData MaterialLighting;
	SCP3DInterfaceData MaterialAntialiasing, MaterialBilinearFilter, MaterialTrilinearFilter, MaterialAnisotropicFilter;
	SCP3DInterfaceData MaterialUseMipMaps;
	SCP3DInterfaceData MaterialZWrite, MaterialZBuffer;
	SCP3DInterfaceData MaterialTextureWrap;
	SCP3DInterfaceData MaterialColorMask, MaterialColorMaterial;
	SCP3DInterfaceData MaterialPolygonOffset;

	SCP3DInterfaceData MaterialPolygonOffsetDirectionCombo, MaterialPolygonOffsetFactorCombo;
	SCP3DInterfaceData MaterialAntiAliasMode[7];

	/// Animators
	SCP3DInterfaceData SceneNodeAnimators;

	/// Datas
	irr::s32 CurrentMaterialID;

};

} /// End namespace cp3d

#endif