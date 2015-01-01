#ifndef __H_C_CP3D_EDITION_TOOL_SCENE_MANAGER_INCLUDED__
#define __H_C_CP3D_EDITION_TOOL_SCENE_MANAGER_INCLUDED__

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
class CCP3DEditionToolSceneManager : public irr::IEventReceiver, public ICP3DEditionToolController {
public:

	/// Constructor & Destructor
	CCP3DEditionToolSceneManager(CCP3DEditorCore *editorCore);
	~CCP3DEditionToolSceneManager();

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

	rendering::ICP3DRenderingEngine *Rengine;

	/// GUI
	irr::gui::IGUITab *SceneTab;

	/// Scene
	SCP3DInterfaceData AmbiantColor;

};

} /// End namespace cp3d

#endif