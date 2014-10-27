#ifndef __H_C_CP3D_EDITION_TOOL_MESH_INCLUDED__
#define __H_C_CP3D_EDITION_TOOL_MESH_INCLUDED__

#include <irrlicht.h>
#include <ICP3DEditionTool.h>

namespace cp3d {

class CCP3DEditionTool;
class CCP3DEditorCore;

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
	irr::gui::IGUIEnvironment *Gui;

	/// CP3D
	CCP3DEditorCore *EditorCore;
	CCP3DEditionTool *EditionTool;

	/// GUI
	irr::gui::IGUITab *GeneralTab;
	irr::gui::IGUITab *MaterialTab;


	SCP3DInterfaceData SceneNodeName;
	SCP3DInterfaceData SceneNodePositionX, SceneNodePositionY, SceneNodePositionZ;
	SCP3DInterfaceData SceneNodeRotationX, SceneNodeRotationY, SceneNodeRotationZ;
	SCP3DInterfaceData SceneNodeScaleX, SceneNodeScaleY, SceneNodeScaleZ;

};

} /// End namespace cp3d

#endif