#ifndef __H_C_CP3D_EDITION_POST_PROCESS_INCLUDED__
#define __H_C_CP3D_EDITION_POST_PROCESS_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>
#include <ICP3DEditionTool.h>
#include <ICP3DRenderingEngine.h>
#include <ICP3DHandler.h>

namespace cp3d {

class CCP3DEditionTool;
class CCP3DEditorCore;

/*
Class that will manage the post-processes nodes.
*/
class CCP3DEditionToolPostProcess : public irr::IEventReceiver,
									public ICP3DEditionToolController,
									public engine::ICP3DUpdate
{
public:

	/// Constructor & Destructor
	CCP3DEditionToolPostProcess(CCP3DEditorCore *editorCore);
	~CCP3DEditionToolPostProcess();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// ICP3DUpdate
	void OnPreUpdate();

	/// ICP3DEditionToolController
	void createInterface();
	void configure();
	void apply();
	void clear();

private:
	/// Methods
	bool enableUI();
	time_t getChangedTime(irr::core::stringc filename);

	/// Irrlicht
	irr::video::IVideoDriver *Driver;
	irr::scene::ISceneManager *Smgr;
	irr::gui::IGUIEnvironment *Gui;

	/// CP3D
	CCP3DEditorCore *EditorCore;
	CCP3DEditionTool *EditionTool;

	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;

	/// GUI
	irr::gui::IGUITab *PostProcessTab;

	/// Post-Processes list
	SCP3DInterfaceData PostProcessesList;
	
	SCP3DInterfaceData PostProcessAutomaticReload;

	/// Extra GUI Elements
	ui::ICP3DFileSelector *OpenPostProcessDialog;

	/// Extra values
	irr::core::map<irr::s32, time_t> Changes;

};

} /// End namespace cp3d

#endif
