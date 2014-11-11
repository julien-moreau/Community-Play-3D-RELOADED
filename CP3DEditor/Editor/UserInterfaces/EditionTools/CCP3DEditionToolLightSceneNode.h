#ifndef __H_C_CP3D_EDITION_TOOL_LIGHT_INCLUDED__
#define __H_C_CP3D_EDITION_TOOL_LIGHT_INCLUDED__

#include <irrlicht.h>
#include <ICP3DEditionTool.h>
#include <ICP3DRenderingEngine.h>
#include <ICP3DHandler.h>

namespace cp3d {

class CCP3DEditionTool;
class CCP3DEditorCore;
class CCP3DSceneNodeAnimators;

/*
Class that will manage the editon for ESNT_MESH type scene
nodes.
*/
class CCP3DEditionToolLightSceneNode : public irr::IEventReceiver, public ICP3DEditionToolController {
public:

	/// Constructor & Destructor
	CCP3DEditionToolLightSceneNode(CCP3DEditorCore *editorCore);
	~CCP3DEditionToolLightSceneNode();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// ICP3DEditionToolController
	void createInterface();
	void configure();
	void apply();

private:
	/// Methods

	/// Irrlicht
	irr::video::IVideoDriver *Driver;
	irr::scene::ISceneManager *Smgr;
	irr::gui::IGUIEnvironment *Gui;

	/// CP3D
	CCP3DEditorCore *EditorCore;
	CCP3DEditionTool *EditionTool;
	CCP3DSceneNodeAnimators *AnimatorsController;

	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DLightSceneNode *LightSceneNode;

	/// GUI
	irr::gui::IGUITab *LightTab;

	/// Shadow light
	SCP3DInterfaceData ComputeShadows;
	SCP3DInterfaceData ComputeRoundedSpot;
	SCP3DInterfaceData ShadowMapFarValue, ShadowMapNearValue, ShadowMapFov;
	SCP3DInterfaceData ShadowMapDirectional;
	SCP3DInterfaceData ShadowMapResolution;

	SCP3DInterfaceData ShadowMapTexture;

	/// Normal Mapping
	SCP3DInterfaceData ComputeNormalMapping;
	SCP3DInterfaceData NormalMappingStrength;

	/// Color
	SCP3DInterfaceData DiffuseColor, SpecularColor, AmbiantColor;

};

} /// End namespace cp3d

#endif