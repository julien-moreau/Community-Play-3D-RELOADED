#ifndef __H_C_CP3D_EDITION_HDR_INCLUDED__
#define __H_C_CP3D_EDITION_HDR_INCLUDED__

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
class CCP3DEditionToolHDR : public irr::IEventReceiver,
							public ICP3DEditionToolController
{
public:

	/// Constructor & Destructor
	CCP3DEditionToolHDR(CCP3DEditorCore *editorCore);
	~CCP3DEditionToolHDR();

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

	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;

	/// GUI
	irr::gui::IGUITab *HDRTab;

	/// Post-Processes list
	SCP3DInterfaceData HDRExposure;
	SCP3DInterfaceData HDRIncrease;
	SCP3DInterfaceData HDRDecrease;
	SCP3DInterfaceData HDRMinLuminance;

	SCP3DInterfaceData BloomThreshold;
	SCP3DInterfaceData BloomCoeff;
	SCP3DInterfaceData BloomMean;
	SCP3DInterfaceData BloomStandDev;
};

} /// End namespace cp3d

#endif
