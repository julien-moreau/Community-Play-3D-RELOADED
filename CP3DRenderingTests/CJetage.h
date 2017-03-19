#include <irrlicht.h>

/// CP3D
// #define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
#include <CP3DCompileConfig.h>
#include <CP3DRenderingEngine.h>
#include <CP3DEngine.h>

namespace cp3d {
namespace test {

	using namespace irr;
	using namespace video;
	using namespace scene;
	using namespace core;
	using namespace gui;

	void Jetage(irr::IrrlichtDevice *device, IEventReceiver *receiver) {
		IVideoDriver *driver = device->getVideoDriver();
		ISceneManager *smgr = device->getSceneManager();
		IGUIEnvironment *gui = device->getGUIEnvironment();

		/// Create rendering engine
		engine::ICP3DEngine *engine = cp3d::createEngine(device);
		engine->getEventReceiver()->addEventReceiver(receiver);

		rendering::ICP3DRenderingEngine *cpre = engine->getRenderingEngine();
		cpre->createNormalMappingMaterial();

		rendering::ICP3DHandler *handler = cpre->getHandler();

		/// Create a fps camera
		ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
		camera->setPosition(vector3df(0.f, 0.f, 0.f));
		device->getCursorControl()->setVisible(false);

		/// Draw skydome
		ITexture *skyTexture = driver->getTexture("Textures/test2.jpg");
		ISceneNode *skydome = smgr->addSkyDomeSceneNode(skyTexture, 32, 16, 0.89999999f, 2.f, 500.f, 0, -1);
		skydome->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
		skydome->setMaterialFlag(EMF_FRONT_FACE_CULLING, true);
		handler->addShadowToNode(skydome, rendering::EFT_NONE, rendering::ESM_EXCLUDE);

		/// HDR
		handler->getHDRManager()->setEnabled(false);
		handler->getHDRManager()->setBrightnessThreshold(1.f);
		handler->getHDRManager()->setGaussWidth(2.f);
		handler->getHDRManager()->setGaussianCoefficient(0.25f);
		handler->getHDRManager()->setMinimumLuminance(0.5f);
		handler->getHDRManager()->setMaximumLuminance(1e20f);
		handler->getHDRManager()->setDecreaseRate(0.2f);
		handler->getHDRManager()->setIncreaseRate(0.2f);
		handler->getHDRManager()->setLensTexture(driver->getTexture("Textures/lensdirt.png"));

		// VR
		handler->getHDRManager()->enableLuminance(false);
		handler->getHDRManager()->enableLensFlare(false);

		/// Finish
		handler->setAmbientColor(SColor(255, 32, 32, 32));

		/// Get hdr texture
		handler->update();

		ITexture *hdrTexture = driver->getTexture("leftEye");
		IGUIImage *img = gui->addImage(rect<s32>(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height)); // VR
		img->setScaleImage(true);
		img->setImage(hdrTexture);

		/// Update the application
		engine->setDrawGUI(true);
		engine->runEngine();
	}

} /// End namespace test
} /// End namespace cp3d
