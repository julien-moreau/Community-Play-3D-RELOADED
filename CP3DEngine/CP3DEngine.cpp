#include "Stdafx.h"

#include "CP3DEngine.h"
#include "Engine/Core/CCP3DEventReceiver.h"

#include "Engine/IO/Loaders/CBabylonFileLoader.h"

#if defined(CP3DR_COMPILE_WITH_OPENMP) && defined(_IRR_WINDOWS_API_)
#include <omp.h>
#endif

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;

namespace cp3d {
	extern "C" CP3DR_LIB_API engine::ICP3DEngine *createEngine(IrrlichtDevice *device) {
		return new engine::CCP3DEngine(device);
	}

	extern "C" CP3DR_LIB_API irr::IrrlichtDevice *createHwndIrrlichtDeviceEx(const HWND hWnd) {
		/// Create device
		SIrrlichtCreationParameters params;
		params.DriverType = EDT_OPENGL;
		params.Bits = 32;
		params.Fullscreen = false;
		params.Vsync = false;
		params.DriverMultithreaded = true;
		params.Doublebuffer = true;
		params.Stencilbuffer = false;
		params.AntiAlias = 32;
		params.ZBufferBits = 32;
		params.HighPrecisionFPU = true;
		params.WindowId = reinterpret_cast<void*>(hWnd);

		IrrlichtDevice *device = createDeviceEx(params);

		/// Configure OpenGL renderer for win32
		SExposedVideoData videoData = SExposedVideoData(0);

		HDC HDc = GetDC(hWnd);
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		int pf = GetPixelFormat(HDc); 
		DescribePixelFormat(HDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		pfd.dwFlags |= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.cDepthBits = 16;
		pf = ChoosePixelFormat(HDc, &pfd);
		SetPixelFormat(HDc, pf, &pfd);
		videoData.OpenGLWin32.HDc = HDc;
		videoData.OpenGLWin32.HRc = wglCreateContext(HDc);
		wglShareLists((HGLRC)device->getVideoDriver()->getExposedVideoData().OpenGLWin32.HRc, (HGLRC)videoData.OpenGLWin32.HRc);

		/// Return device
		return device;
	}
}

namespace cp3d {
namespace engine {

CCP3DEngine::CCP3DEngine(IrrlichtDevice *device) : Device(device), DrawGUI(false)
{
	// Configure rendering
	Rengine = createRenderingEngine(device);
	Handler = Rengine->getHandler();

	MonitorPlugin = new CCP3DMonitorPlugin(Rengine);
	MonitorPlugin->addMonitor("CP3DDefaultMonitor");
	//MonitorPlugin->addMonitor("CP3DVRMonitor");
    
	Driver = Rengine->getVideoDriver();

	// Configure Events & Update
	EventReceiver = new CCP3DEventReceiver();
	Device->setEventReceiver(EventReceiver);
    
	Updater = new CCP3DCustomUpdater();
    
	// Scene
	SceneNodeCreator = new CCP3DSceneNodeCreator(Rengine);
	GeometryCreator = new CCP3DGeometryCreator(Rengine->getSceneManager());

	AnimatedMeshSceneNodeManipulator = new CCP3DAnimatedMeshSceneNodeManipulator(device->getSceneManager());

	// Loaders
	Rengine->getSceneManager()->addExternalSceneLoader(new CBabylonSceneFileLoader(Rengine->getSceneManager(), Device->getFileSystem()));

	// Finish
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

		drawAll();
	}
}

inline void CCP3DEngine::drawAll() {
	Driver->beginScene(true, true, SColor(0x0));

	Updater->OnPreUpdate();

	// Setup active camera
	ICameraSceneNode* camera = Rengine->getSceneManager()->getActiveCamera();
	camera->setAspectRatio((f32) Handler->getViewPort().getWidth() / (f32) Handler->getViewPort().getHeight());

	// Render
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

ICP3DAnimatedMeshSceneNodeManipulator *CCP3DEngine::getAnimatedMeshSceneNodeManipulator() {
	return AnimatedMeshSceneNodeManipulator;
}

CCP3DExporter *CCP3DEngine::createExporter() {
	return new CCP3DExporter(this);
}

/// Rendering
rendering::ICP3DRenderingEngine *CCP3DEngine::getRenderingEngine() {
	return Rengine;
}

void CCP3DEngine::addSceneManager(ISceneManager *smgr) {
	if (!smgr)
		return;

	CustomSceneManagers.push_back(smgr);
}
void CCP3DEngine::removeSceneManager(ISceneManager *smgr) {
	s32 index = CustomSceneManagers.binary_search(smgr);
	if (index != -1)
		CustomSceneManagers.erase(index);
}
ISceneManager *CCP3DEngine::getSceneManager(u32 indice) {
	return CustomSceneManagers[indice];
}
irr::u32 CCP3DEngine::getSceneManagerCount() {
	return CustomSceneManagers.size();
}

} /// End namespace engine
} /// End namespace cp3d
