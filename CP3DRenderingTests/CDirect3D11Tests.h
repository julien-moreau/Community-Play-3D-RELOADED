#include <irrlicht.h>

/// CP3D
#define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
#include <CP3DRenderingEngine.h>

namespace cp3d {
namespace test {

class CPureDepthSSAO : public rendering::IPostProcessingRenderCallback {

public:

	CPureDepthSSAO(irr::IrrlichtDevice *device, rendering::ICP3DHandler *handler) {
		RandomTexture = handler->generateRandomVectorTexture(irr::core::dimension2du(512, 512), "SSAORandomTexture");
		DepthRTT = device->getVideoDriver()->getTexture("DepthRTT");
	}

	void OnPreRender(rendering::ICP3DHandler *handler) {
		handler->setPostProcessingTextureAtIndex(2, DepthRTT);
		handler->setPostProcessingTextureAtIndex(3, RandomTexture);
	}

private:
	irr::video::ITexture *RandomTexture, *DepthRTT;

};

void Direct3D11Test(irr::IrrlichtDevice *device) {
	using namespace irr;
	using namespace video;
	using namespace scene;
	using namespace core;

	IVideoDriver *driver = device->getVideoDriver();
	IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();

	ISceneManager *smgr = device->getSceneManager();

	cp3d::rendering::ICP3DRenderingEngine *rengine = cp3d::createRenderingEngine(device);
	cp3d::rendering::ICP3DHandler *handler = rengine->getHandler();

	/// Create a test scene
	ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
	camera->setPosition(vector3df(30.f, 30.f, 0.f));
	device->getCursorControl()->setVisible(false);

	ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("Textures/Skybox/glacier_up.png"),
		driver->getTexture("Textures/Skybox/glacier_dn.png"),
		driver->getTexture("Textures/Skybox/glacier_lf.png"),
		driver->getTexture("Textures/Skybox/glacier_rt.png"),
		driver->getTexture("Textures/Skybox/glacier_ft.png"),
		driver->getTexture("Textures/Skybox/glacier_bk.png"));

	handler->getDepthPassManager()->addPass("DepthRTT");

	rengine->createNormalMappingMaterial();
	for (s32 i = 0; i < 6; ++i)
	{
		for (s32 j = 0; j < 6; ++j)
		{
			for (s32 k = 0; k < 6; ++k)
			{
				ISceneNode* cube = smgr->addCubeSceneNode(4.0f);
				cube->setPosition(vector3df(i * 4.0f + 2.0f, j * 5.0f + 1.0f, k * 6.0f + 3.0f));
				cube->setRotation(vector3df((f32)(rand() % 360), (f32)(rand() % 360), (f32)(rand() % 360)));
				cube->getMaterial(0).setTexture(0, driver->getTexture("Textures/diffuse.tga"));
				cube->getMaterial(0).setTexture(1, driver->getTexture("Textures/normal.tga"));
				cube->getMaterial(0).setFlag(EMF_LIGHTING, false);
				cube->getMaterial(0).MaterialType = rengine->Materials[EMT_NORMAL_MAP_SOLID];
				//cube->getMaterial(0).MaterialType = rengine->Materials[EMT_SOLID];

				handler->getDepthPassManager()->addNodeToPass(cube);
				handler->getGeneralPassManager()->addNodeToPass(cube);
			}
		}
	}

	rendering::ICP3DLightSceneNode *light = rengine->createLightSceneNode(true, false);
	light->setPosition(vector3df(100.f, 100.f, 0.f));
	light->setLightColor(SColorf(1.f, 1.f, 1.f, 1.f));

	handler->addPostProcessingEffectFromFile("Tests/ssao.fragment.fx", new CPureDepthSSAO(device, handler));
	handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");
	handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurVP.fragment.fx");
	handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAOCombine.fragment.fx");

	while (device->run()) {
		if (!device->isWindowActive())
			continue;

		driver->beginScene(true, true, SColor(0x0));
		handler->update();
		//driver->draw2DImage(driver->getTexture("DepthRTT"), vector2di(0, 0));
		driver->endScene();
	}
}

} /// End namespace test
} /// End namespace cp3d
