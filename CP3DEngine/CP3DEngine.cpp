// Il s'agit du fichier DLL principal.

#include "stdafx.h"

#define CP3DR_COMPILE_RENDERING_ENGINE
#include <CP3DCompileConfig.h>

#include "CP3DEngine.h"
#include "Engine/Core/CCP3DEventReceiver.h"

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

CCP3DEngine::CCP3DEngine(irr::IrrlichtDevice *device) : Device(device)
{
	/// Configure rendering
	Rengine = createRenderingEngine(device);
	Handler = Rengine->getHandler();
	
	Driver = Rengine->getVideoDriver();

	/// Configure Events & Update
	EventReceiver = new CCP3DEventReceiver();
	Device->setEventReceiver(EventReceiver);

	Updater = new CCP3DCustomUpdater();

	/// Scene
	SceneNodeCreator = new CCP3DSceneNodeCreator(Rengine);

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

		Handler->update();

		Driver->setViewPort(rect<s32>(0, 0, Driver->getScreenSize().Width, Driver->getScreenSize().Height));
		if (DrawGUI)
			Gui->drawAll();

		Updater->OnPostUpdate();

		Device->getVideoDriver()->endScene();

	}
}

void CCP3DEngine::setDrawGUI(const bool draw) {
	DrawGUI = draw;
}

const bool CCP3DEngine::isDrawingGUI() const {
	return DrawGUI;
}

CCP3DEventReceiver *CCP3DEngine::getEventReceiver() {
	return EventReceiver;
}

CCP3DCustomUpdater *CCP3DEngine::getCustomUpdater() {
	return Updater;
}

CCP3DSceneNodeCreator *CCP3DEngine::getSceneNodeCreator() {
	return SceneNodeCreator;
}

rendering::ICP3DRenderingEngine *CCP3DEngine::getRenderingEngine() {
	return Rengine;
}

} /// End namespace engine
} /// End namespace cp3d
