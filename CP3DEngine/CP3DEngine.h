// CP3DEngine.h

#ifndef __H_CP3D_ENGINE_INCLUDED__
#define __H_CP3D_ENGINE_INCLUDED__

#include <CP3DRenderingEngine.h>
#include <CP3DEngine.h>

#include "Engine/Core/CCP3DEventReceiver.h"
#include "Engine/Core/CCP3DCustomUpdater.h"
#include "Engine/Scene/CCP3DSceneNodeCreator.h"

namespace cp3d {
namespace engine {

class CCP3DEngine : public ICP3DEngine {
public:

	CCP3DEngine(irr::IrrlichtDevice *device);
	~CCP3DEngine();

	/// ICP3DEngine
	void runEngine();

	/// Events & Updates
	CCP3DEventReceiver *getEventReceiver();
	CCP3DCustomUpdater *getCustomUpdater();

	/// Scene
	CCP3DSceneNodeCreator *getSceneNodeCreator();

	/// Rendering
	rendering::ICP3DRenderingEngine *getRenderingEngine();
	void setSceneRenderingViewPort(irr::core::rect<irr::s32> viewPort);

	/// GUI
	const bool isDrawingGUI() const;
	void setDrawGUI(const bool draw);

private:
	/// Irrlicht
	irr::IrrlichtDevice *Device;
	irr::video::IVideoDriver *Driver;
	irr::gui::IGUIEnvironment *Gui;

	/// Events & Update
	CCP3DEventReceiver *EventReceiver;
	CCP3DCustomUpdater *Updater;

	/// Scene
	CCP3DSceneNodeCreator *SceneNodeCreator;

	/// Rendering
	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;
	irr::core::rect<irr::s32> ViewPort;

	bool DrawGUI;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
