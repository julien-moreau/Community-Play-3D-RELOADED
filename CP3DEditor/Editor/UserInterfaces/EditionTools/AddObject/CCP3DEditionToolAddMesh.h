#ifndef __H_C_CP3D_EDITION_ADD_MESH_INCLUDED__
#define __H_C_CP3D_EDITION_ADD_MESH_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>
#include <ICP3DEditionTool.h>
#include <ICP3DRenderingEngine.h>
#include <ICP3DHandler.h>
#include <ICP3DGUIManager.h>

namespace cp3d {

class CCP3DEditionTool;
class CCP3DEditorCore;

/*
Class that will manage the post-processes nodes.
*/
class CCP3DEditionToolAddMesh : public irr::IEventReceiver,
								public ICP3DEditionToolController
{
public:

	/// Constructor & Destructor
	CCP3DEditionToolAddMesh(CCP3DEditorCore *editorCore, bool animated = false);
	~CCP3DEditionToolAddMesh();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// ICP3DEditionToolController
	void createInterface();
	void configure();
	void apply();
	void clear();

private:

	/// Irrlicht
	irr::video::IVideoDriver *Driver;
	irr::scene::ISceneManager *Smgr;
	irr::gui::IGUIEnvironment *Gui;

	/// CP3D
	CCP3DEditorCore *EditorCore;
	CCP3DEditionTool *EditionTool;

	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;

	/// Data
	bool Animated;

	/// GUI
	irr::gui::IGUITab *AddMeshTab;

	/// Interface datas
	SCP3DInterfaceData FileOpenDialog;
	SCP3DInterfaceData ViewPort;
	SCP3DInterfaceData AutoAnimate;
	SCP3DInterfaceData AddButton;

	/// Extras
	irr::scene::ISceneManager *ViewPortSmgr;
	irr::scene::ISceneNode *Node;
	irr::scene::IMesh *Mesh;
	irr::core::stringc MeshPath;
};

} /// End namespace cp3d

#endif
