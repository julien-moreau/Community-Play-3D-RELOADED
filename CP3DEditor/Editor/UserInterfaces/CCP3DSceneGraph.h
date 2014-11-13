#ifndef __H_C_CP3D_SCENE_GRAPH_INCLUDED__
#define __H_C_CP3D_SCENE_GRAPH_INCLUDED__

#include <irrlicht.h>
#include <ICP3DInterface.h>
#include <ICP3DCustomUpdate.h>
#include <CP3DRenderingEngine.h>

namespace cp3d {

class CCP3DEditorCore;

/*
Class creating the main context menu (the top menu of the application's window)
This class plays like a router and receives all the context menu events.
Then, once the event is received, this class will call the appropriate user
interfaces
Below this class, the enums are the command ids of all the sub menus
Then, if the event caller is FileContextMenu, you have to check if the
selected item is ECMF_OPEN_PROJECT, etc. and then run the appropriate command
*/
class CCP3DSceneGraph : public irr::IEventReceiver, public ICP3DInterface, public engine::ICP3DUpdate {
public:

	/// Constructor & Destructor
	CCP3DSceneGraph(CCP3DEditorCore *editorCore);
	~CCP3DSceneGraph();
	
	/// Inheritance
	bool OnEvent(const irr::SEvent &event);
	void OnPreRender();
	void OnResize();
	irr::gui::IGUIElement *getElementToResize() { return Window; }

	/// Methods
	void fillGraph(irr::scene::ISceneNode *start = 0);

private:
	/// Irrlicht
	irr::video::IVideoDriver *Driver;
	irr::scene::ISceneManager *Smgr;
	irr::gui::IGUIEnvironment *Gui;

	/// CP3D
	CCP3DEditorCore *EditorCore;
	rendering::ICP3DRenderingEngine *Rengine;

	/// GUI
	irr::gui::IGUIWindow *Window;
	irr::gui::IGUITreeView *Graph;

	/// Datas
	irr::gui::IGUITreeViewNode *RootNode;
	irr::gui::IGUITreeViewNode *SceneNode;
	irr::scene::ISceneNode *SelectedSceneNode;

	irr::s32 WindowWidth;

	/// Methods
	irr::gui::IGUITreeViewNode *getTreeNodeFromParentSceneNode(irr::gui::IGUITreeViewNode *startNode, irr::scene::ISceneNode *node);
	irr::s32 getIconFromType(irr::scene::ESCENE_NODE_TYPE type);
	void fillGraphRecursively(irr::scene::ISceneNode *start, irr::gui::IGUITreeViewNode *treeNode);
	irr::u32 getLightSceneNodeIndex(irr::scene::ILightSceneNode *node);

};

} /// End namespace cp3d

#endif