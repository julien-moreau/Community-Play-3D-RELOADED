// CP3DEngine.h

#ifndef __H_CP3D_ENGINE_INCLUDED__
#define __H_CP3D_ENGINE_INCLUDED__

#include <CP3DRenderingEngine.h>
#include <CP3DEngine.h>

#include "Engine/Core/Plugins/CCP3DMonitorPlugin.h"

#include "Engine/Core/CCP3DEventReceiver.h"
#include "Engine/Core/CCP3DCustomUpdater.h"

#include "Engine/Scene/CCP3DSceneNodeCreator.h"
#include "Engine/Scene/CCP3DGeometryCreator.h"

#include "Engine/Scene/AnimatedMeshSceneNode/CAnimatedMeshSceneNodeManipulator.h"

#include "Engine/IO/CCP3DExporter.h"

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
	CCP3DGeometryCreator *getGeometryCreator();

	ICP3DAnimatedMeshSceneNodeManipulator *getAnimatedMeshSceneNodeManipulator();

	/// Rendering
	rendering::ICP3DRenderingEngine *getRenderingEngine();

	/// GUI
	const bool isDrawingGUI() const;
	void setDrawGUI(const bool draw);

	/// Scene manager
	void addSceneManager(irr::scene::ISceneManager *smgr);
	void removeSceneManager(irr::scene::ISceneManager *smgr);
	irr::scene::ISceneManager *getSceneManager(irr::u32 indice);
	irr::u32 getSceneManagerCount();

	CCP3DExporter *createExporter();

private:
	/// Irrlicht
	irr::IrrlichtDevice *Device;
	irr::video::IVideoDriver *Driver;
	irr::gui::IGUIEnvironment *Gui;

	/// Events & Update
	CCP3DEventReceiver *EventReceiver;
	CCP3DCustomUpdater *Updater;
	irr::core::array<irr::scene::ISceneManager *> CustomSceneManagers;

	/// Scene
	CCP3DSceneNodeCreator *SceneNodeCreator;
	CCP3DGeometryCreator *GeometryCreator;

	CCP3DAnimatedMeshSceneNodeManipulator *AnimatedMeshSceneNodeManipulator;

	/// Rendering
	CCP3DMonitorPlugin *MonitorPlugin;
	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;

	bool DrawGUI;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
