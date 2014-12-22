#include <irrlicht.h>

/// CP3D
#define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
#include <CP3DRenderingEngine.h>

namespace cp3d {
namespace test {

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
	camera->setPosition(vector3df(100.f, 100.f, 0.f));
	device->getCursorControl()->setVisible(false);

	IAnimatedMesh *planeMesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(50, 50),
		0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));

	IMeshSceneNode *planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialTexture(0, driver->getTexture("Textures/diffuse.tga"));
	planeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
	planeNode->setMaterialTexture(2, driver->getTexture("Textures/specular.tga"));
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	planeNode->getMaterial(0).Shininess = 0.f;
	handler->addShadowToNode(planeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_EXCLUDE);

	while (device->run()) {
		if (!device->isWindowActive())
			continue;

		driver->beginScene(true, true, SColor(0x0));
		handler->update();
		driver->endScene();
	}
}

} /// End namespace test
} /// End namespace cp3d
