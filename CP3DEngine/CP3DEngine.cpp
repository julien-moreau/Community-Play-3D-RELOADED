#include "Stdafx.h"

#include "CP3DEngine.h"
#include "Engine/Core/CCP3DEventReceiver.h"

#if defined(CP3DR_COMPILE_WITH_OPENMP) && defined(_IRR_WINDOWS_API_)
#include <omp.h>
#endif

using namespace irr;
using namespace video;
using namespace core;

namespace cp3d {
	CP3DR_LIB_API engine::ICP3DEngine *createEngine(IrrlichtDevice *device) {
		return new engine::CCP3DEngine(device);
	}
}

namespace cp3d {
namespace engine {

CCP3DEngine::CCP3DEngine(irr::IrrlichtDevice *device) : Device(device), DrawGUI(false)
{
	/// Configure rendering
	Rengine = createRenderingEngine(device);
	Handler = Rengine->getHandler();

	MonitorPlugin = new CCP3DMonitorPlugin(Rengine);
	MonitorPlugin->addMonitor("CP3DDefaultMonitor_d.dll");
	
	Driver = Rengine->getVideoDriver();

	/// Configure Events & Update
	EventReceiver = new CCP3DEventReceiver();
	Device->setEventReceiver(EventReceiver);

	Updater = new CCP3DCustomUpdater();

	/// Scene
	SceneNodeCreator = new CCP3DSceneNodeCreator(Rengine);
	GeometryCreator = new CCP3DGeometryCreator(Rengine->getSceneManager());

	/// Finish
	Gui = device->getGUIEnvironment();
}

CCP3DEngine::~CCP3DEngine() {
	Device->setEventReceiver(0);
	delete EventReceiver;
}

void CCP3DEngine::runEngine() {
	while (Device->run()) {
		if (!Device->isWindowActive())
			continue;

		Driver->beginScene(true, true, SColor(0x0));

		Updater->OnPreUpdate();

		if (MonitorPlugin->getMonitorCount() == 0)
			Handler->update();
		else
			MonitorPlugin->render();

		for (u32 i = 0; i < CustomSceneManagers.size(); i++) {
			#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
			if (Driver->getDriverType() == EDT_DIRECT3D11)
				Driver->clearZBuffer();
			#endif
			CustomSceneManagers[i]->drawAll();
		}

		Driver->setViewPort(rect<s32>(0, 0, Driver->getScreenSize().Width, Driver->getScreenSize().Height));
		if (DrawGUI)
			Gui->drawAll();

		Updater->OnPostUpdate();

		Device->getVideoDriver()->endScene();

	}
}

/// Gui
void CCP3DEngine::setDrawGUI(const bool draw) {
	DrawGUI = draw;
}

const bool CCP3DEngine::isDrawingGUI() const {
	return DrawGUI;
}

/// Core
CCP3DEventReceiver *CCP3DEngine::getEventReceiver() {
	return EventReceiver;
}

CCP3DCustomUpdater *CCP3DEngine::getCustomUpdater() {
	return Updater;
}

/// Scene
CCP3DSceneNodeCreator *CCP3DEngine::getSceneNodeCreator() {
	return SceneNodeCreator;
}
CCP3DGeometryCreator *CCP3DEngine::getGeometryCreator() {
	return GeometryCreator;
}

/// Rendering
rendering::ICP3DRenderingEngine *CCP3DEngine::getRenderingEngine() {
	return Rengine;
}

void CCP3DEngine::addSceneManager(irr::scene::ISceneManager *smgr) {
	if (!smgr)
		return;

	CustomSceneManagers.push_back(smgr);
}

void CCP3DEngine::removeSceneManager(irr::scene::ISceneManager *smgr) {
	s32 index = CustomSceneManagers.binary_search(smgr);
	if (index != -1)
		CustomSceneManagers.erase(index);
}

} /// End namespace engine
} /// End namespace cp3d
