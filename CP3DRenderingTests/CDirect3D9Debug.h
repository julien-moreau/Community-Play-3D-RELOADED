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

	void Direct3D9Debug(irr::IrrlichtDevice *device, IEventReceiver *receiver) {
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
		light->setPosition(vector3df(250.f, 300.f, 0.f));
		light->setLightColor(SColorf(1.f, 0.5f, 0.f, 1.f));
		light->getLightData().SpecularColor = SColorf(1.f, 1.f, 1.f, 1.f);
		light->getShadowLight()->setUseRoundSpotLight(false);
		light->getShadowLight()->setFarValue(1000.f);
		light->setLightStrength(driver->getDriverType() == EDT_DIRECT3D9 ? 2.5f : 1.f);
		light->getShadowLight()->setShadowMapResolution(1024);

		ISceneNodeAnimator *animator = smgr->createFlyStraightAnimator(vector3df(-250.f, 200.f, -100.f), vector3df(250.f, 200.f, 100.f), 10000, true, true);

		ILightSceneNode *lightNode = *light;
		//lightNode->addAnimator(animator);
        
        /// Create another light
        cp3d::rendering::ICP3DLightSceneNode *light2 = cpre->createLightSceneNode(true, true);
        light2->setPosition(vector3df(0.f, 300.f, 250.f));
        light2->setLightColor(SColorf(1.f, 0.f, 0.5f, 1.f));
        light2->getLightData().SpecularColor = SColorf(1.f, 1.f, 1.f, 1.f);
        light2->getShadowLight()->setUseRoundSpotLight(false);
        light2->getShadowLight()->setFarValue(1000.f);
        light2->setLightStrength(driver->getDriverType() == EDT_DIRECT3D9 ? 2.5f : 1.f);
        light2->getShadowLight()->setShadowMapResolution(1024);
        
        /// Create another light
        cp3d::rendering::ICP3DLightSceneNode *light3 = cpre->createLightSceneNode(true, true);
        light3->setPosition(vector3df(-250.f, 300.f, 0.f));
        light3->setLightColor(SColorf(0.f, 0.5f, 1.f, 1.f));
        light3->getLightData().SpecularColor = SColorf(1.f, 1.f, 1.f, 1.f);
        light3->getShadowLight()->setUseRoundSpotLight(false);
        light3->getShadowLight()->setFarValue(1000.f);
        light3->setLightStrength(driver->getDriverType() == EDT_DIRECT3D9 ? 2.5f : 1.f);
        light3->getShadowLight()->setShadowMapResolution(1024);
        
        /// Create another light
        cp3d::rendering::ICP3DLightSceneNode *light4 = cpre->createLightSceneNode(true, true);
        light4->setPosition(vector3df(0.f, 300.f, -250.f));
        light4->setLightColor(SColorf(0.f, 1.f, 0.5f, 1.f));
        light4->getLightData().SpecularColor = SColorf(1.f, 1.f, 1.f, 1.f);
        light4->getShadowLight()->setUseRoundSpotLight(false);
        light4->getShadowLight()->setFarValue(1000.f);
        light4->setLightStrength(driver->getDriverType() == EDT_DIRECT3D9 ? 2.5f : 1.f);
        light4->getShadowLight()->setShadowMapResolution(1024);
        
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

		IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 25.f, 0.f), vector3df(0.f, 45.f, 0.f));
		cubeNode->setMaterialTexture(0, driver->getTexture("Textures/diffuse.tga"));
		cubeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
		cubeNode->setMaterialTexture(2, driver->getTexture("Textures/specular.tga"));
		cubeNode->setMaterialFlag(EMF_LIGHTING, false);
		handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_16PCF, cp3d::rendering::ESM_BOTH);

		array<vector3df> points;
		points.push_back(vector3df(-150.f, 25.f, 0.f));
		points.push_back(vector3df(150.f, 25.f, 0.f));
		animator = smgr->createFollowSplineAnimator(0, points, 1.f, 1.5f, true, true);
		cubeNode->addAnimator(animator);

		animator = smgr->createRotationAnimator(vector3df(0.f, 4.5f, 0.f));
		cubeNode->addAnimator(animator);

		/// Clouds
		#ifndef _IRR_COMPILE_WITH_DIRECT3D_11_
		ISceneNode *cloud1 = engine->getSceneNodeCreator()->createCloudNode(vector2df(0.008f, 0.0f), driver->getTexture("Textures/Clouds/cloud01.png"), 1.f, 0.5f, 0.1f, -0.05f);
		ISceneNode *cloud2 = engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud02.png"), 0.4f, 0.05f, -0.1f, 0.5f);
		ISceneNode *cloud3 = engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud03.png"), 0.035f, 0.f, -0.15f, 0.4f);
		#endif

		/// SSAO and Depth
		cpre->getEffectsManager()->createSSAOEffect(true);

		handler->getDepthPassManager()->addNodeToPass(planeNode);
		handler->getDepthPassManager()->addNodeToPass(cubeNode);

		handler->getGeneralPassManager()->addNodeToPass(cubeNode);

		/// HDR
		handler->getHDRManager()->setEnabled(true);
		handler->getHDRManager()->setBrightnessThreshold(1.f);
		handler->getHDRManager()->setGaussWidth(2.f);
		handler->getHDRManager()->setGaussianCoefficient(0.25f);
		handler->getHDRManager()->setMinimumLuminance(0.5f);
		handler->getHDRManager()->setMaximumLuminance(1e20f);
		handler->getHDRManager()->setDecreaseRate(0.5f);
		handler->getHDRManager()->setIncreaseRate(0.2f);
		handler->getHDRManager()->setLensTexture(driver->getTexture("Textures/lensdirt.png"));

		/// Finish
		handler->setAmbientColor(SColor(255, 32, 32, 32));

		/// Get hdr texture
		handler->update();

		ITexture *hdrTexture = driver->getTexture("ColorMapSampler");
		IGUIImage *img = gui->addImage(rect<s32>(driver->getScreenSize().Width - 512, 0, driver->getScreenSize().Width, 512));
		img->setScaleImage(true);
		img->setImage(hdrTexture);

		/// Update the application
		engine->setDrawGUI(true);
		engine->runEngine();
	}

} /// End namespace test
} /// End namespace cp3d
