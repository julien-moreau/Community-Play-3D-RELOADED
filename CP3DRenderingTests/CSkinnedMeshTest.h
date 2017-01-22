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

		void SkinnedMeshDebug(irr::IrrlichtDevice *device, IEventReceiver *receiver) {
			IVideoDriver *driver = device->getVideoDriver();
			ISceneManager *smgr = device->getSceneManager();
			IGUIEnvironment *gui = device->getGUIEnvironment();

			/// Create rendering engine
			cp3d::engine::ICP3DEngine *engine = cp3d::createEngine(device);
			engine->getEventReceiver()->addEventReceiver(receiver);

			cp3d::rendering::ICP3DRenderingEngine *cpre = engine->getRenderingEngine();
			cpre->createNormalMappingMaterial();

			cp3d::rendering::ICP3DHandler *handler = cpre->getHandler();

			/// Create a fps camera
			ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
			camera->setPosition(vector3df(50.f, 50.f, 0.f));
			device->getCursorControl()->setVisible(false);

			/// Create a light
			cp3d::rendering::ICP3DLightSceneNode *light = cpre->createLightSceneNode(true, true);
			light->setPosition(vector3df(150.f, 300.f, 0.f));
			light->setLightColor(SColorf(1.f, 1.f, 1.f, 1.f));
			light->getLightData().SpecularColor = SColorf(1.f, 1.f, 1.f, 1.f);
			light->getShadowLight()->setUseRoundSpotLight(false);
			light->getShadowLight()->setFarValue(1000.f);
			light->setLightStrength(driver->getDriverType() == EDT_DIRECT3D9 ? 2.5f : 1.f);
			light->getShadowLight()->setShadowMapResolution(4096);

			ISceneNodeAnimator *animator = smgr->createFlyStraightAnimator(vector3df(-250.f, 200.f, -100.f), vector3df(250.f, 200.f, 100.f), 10000, true, true);

			ILightSceneNode *lightNode = *light;
			lightNode->addAnimator(animator);

			/// Skybox
			ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
				driver->getTexture("Textures/Skybox/glacier_up.png"),
				driver->getTexture("Textures/Skybox/glacier_dn.png"),
				driver->getTexture("Textures/Skybox/glacier_lf.png"),
				driver->getTexture("Textures/Skybox/glacier_rt.png"),
				driver->getTexture("Textures/Skybox/glacier_ft.png"),
				driver->getTexture("Textures/Skybox/glacier_bk.png"));

			/// Create a test scene
			IAnimatedMesh *planeMesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(100, 100),
				0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));

			IMeshSceneNode *planeNode = smgr->addMeshSceneNode(planeMesh);
			planeNode->setMaterialTexture(0, driver->getTexture("Textures/specular.tga"));
			planeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
			planeNode->setMaterialTexture(2, driver->getTexture("Textures/specular.tga"));
			planeNode->setMaterialFlag(EMF_LIGHTING, false);
			handler->addShadowToNode(planeNode, cp3d::rendering::EFT_16PCF, cp3d::rendering::ESM_BOTH);

			animator = smgr->createRotationAnimator(vector3df(0.f, 4.5f, 0.f));

			/// Clouds
			#ifndef _IRR_COMPILE_WITH_DIRECT3D_11_
			ISceneNode *cloud1 = engine->getSceneNodeCreator()->createCloudNode(vector2df(0.008f, 0.0f), driver->getTexture("Textures/Clouds/cloud01.png"), 1.f, 0.5f, 0.1f, -0.05f);
			ISceneNode *cloud2 = engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud02.png"), 0.4f, 0.05f, -0.1f, 0.5f);
			ISceneNode *cloud3 = engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud03.png"), 0.035f, 0.f, -0.15f, 0.4f);
			#endif

			/// Murphy
			IAnimatedMeshSceneNode *murphy = smgr->addAnimatedMeshSceneNode(smgr->getMesh("data/murphy/walk.x"), 0, -1, vector3df(0.f, -0.f, 0.f), vector3df(0.f), vector3df(1.f));
			murphy->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
			murphy->setAnimationSpeed(5000.f);
			murphy->setMaterialType(cpre->Materials[EMT_NORMAL_MAP_SOLID]);
			handler->addShadowToNode(murphy, rendering::EFT_16PCF, rendering::ESM_BOTH);

			u32 framesCount1 = murphy->getEndFrame();

			/// Test manipulator
			ISkinnedMesh *skeletonMesh = (ISkinnedMesh *)smgr->getMesh("data/murphy/run.x");
			engine::ICP3DAnimatedMeshSceneNodeManipulator *manipulator = engine->getAnimatedMeshSceneNodeManipulator();

			//manipulator->setSkinnedMesh(murphy, skeletonMesh);
			//manipulator->appendAnimations(murphy, skeletonMesh);

			u32 framesCount2 = murphy->getEndFrame();

			/// SSAO and Depth
			cpre->getEffectsManager()->createSSAOEffect(true);
			handler->getDepthPassManager()->addNodeToPass(planeNode);

			/// HDR
			handler->getHDRManager()->setEnabled(true);
			handler->getHDRManager()->setBrightnessThreshold(1.f);
			handler->getHDRManager()->setGaussWidth(2.f);
			handler->getHDRManager()->setGaussianCoefficient(0.25f);
			handler->getHDRManager()->setMinimumLuminance(0.5f);
			handler->getHDRManager()->setMaximumLuminance(1e20f);
			handler->getHDRManager()->setDecreaseRate(0.5f);
			handler->getHDRManager()->setIncreaseRate(0.2f);
			handler->getHDRManager()->setLensTexture(driver->getTexture("Textures/lensdirt.jpg"));

			/// Finish
			handler->setAmbientColor(SColor(255, 32, 32, 32));

			/// Update the application
			engine->runEngine();
		}

	} /// End namespace test
} /// End namespace cp3d
