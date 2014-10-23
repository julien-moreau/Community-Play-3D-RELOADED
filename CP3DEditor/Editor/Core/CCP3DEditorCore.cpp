
#include "stdafx.h"

#define CP3DR_PROJECT_NAME "Community Play 3D Editor"
#define CP3DR_COMPILE_ENGINE
#include <CP3DCompileConfig.h>

#include "CCP3DEditorCore.h"
#include "CCP3DInterfaceController.h"
#include "../UserInterfaces/CCP3DContextMenu.h"
#include "../UserInterfaces/CCP3DEditionTool.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditorCore::CCP3DEditorCore(irr::IrrlichtDevice *device) : Device(device), ProjectName("Untitled")
{
	/// Configure device
	device->setResizable(true);
	device->maximizeWindow();
	device->getVideoDriver()->OnResize(device->getVideoDriver()->getScreenSize());

	/// Configure engine
	Engine = cp3d::createEngine(device);

	/// Configure rendering engine
	Rengine = Engine->getRenderingEngine();
	Handler = Rengine->getHandler();
	Handler->setScreenRenderTargetResolution(device->getVideoModeList()->getDesktopResolution());

	/// Configure GUI
	Gui = Rengine->getGUIEnvironment();
	IGUISkin *skin = Gui->createSkin(EGST_WINDOWS_CLASSIC);
	IGUIFont *font = Gui->getFont("GUI/fontlucida.png");
    skin->setFont(font);
    Gui->setSkin(skin);
    skin->setColor(EGDC_BUTTON_TEXT, SColor(255,0xAA,0xAA,0xAA));
	skin->setColor(EGDC_3D_HIGH_LIGHT, SColor(255,0x22,0x22,0x22));
	skin->setColor(EGDC_3D_FACE, SColor(255,0x44,0x44,0x44));
	skin->setColor(EGDC_WINDOW, SColor(255,0x66,0x66,0x66));
	skin->setColor(EGDC_EDITABLE, SColor(255,0x66,0x66,0x66));
	skin->setColor(EGDC_INACTIVE_CAPTION, SColor(255,0,0,0));
	skin->setColor(EGDC_3D_LIGHT, SColor(255, 32, 32, 32));

	/// Create User Interface
	InterfaceController = new CCP3DInterfaceController(this);
	ContextMenu = new CCP3DContextMenu(this);
	EditionTool = new CCP3DEditionTool(this);

	/// Finish
	WorkingDirectory = ProjectDirectory = device->getFileSystem()->getWorkingDirectory();
	setProjectName(ProjectName);

	/// Tests
	/// Configure scene
	createComponents();
	#if defined(_DEBUG)
	Engine->getEventReceiver()->addEventReceiver(this);
	createTestScene();
	#endif
}

CCP3DEditorCore::~CCP3DEditorCore() {
	delete Rengine;
	delete Engine;
}

/// Creates components like camera etc.
void CCP3DEditorCore::createComponents() {
	ISceneManager *smgr = Device->getSceneManager();
	ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya();
	camera->setPosition(vector3df(100.f, 100.f, 100.f));
}

void CCP3DEditorCore::setProjectName(irr::core::stringc name) {
	ProjectName = name;
	Device->setWindowCaption(stringw(stringc(CP3DR_PROJECT_NAME) + stringc(" - ") + stringc(ProjectName)).c_str());
	ContextMenu->setProjectName(name);
}

/// Runs the editor
void CCP3DEditorCore::runEditor() {
	Engine->runEngine();
}

} /// End namespace cp3d

namespace cp3d {
#if defined(_DEBUG)

bool CCP3DEditorCore::OnEvent(const SEvent &event) {
	if (event.EventType == EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Key == KEY_ESCAPE) {
			Device->closeDevice();
		}
	}

	return false;
}

void CCP3DEditorCore::createTestScene() {
	ISceneManager *smgr = Device->getSceneManager();
	IVideoDriver *driver = Device->getVideoDriver();
	/// Create a test scene
	IAnimatedMesh *planeMesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(50, 50),
													  0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));
	IAnimatedMeshSceneNode *planeNode = smgr->addAnimatedMeshSceneNode(planeMesh);
	planeNode->setMaterialTexture(0, driver->getTexture("Textures/diffuse.tga"));
	planeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	Handler->addShadowToNode(planeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_RECEIVE);

	IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 25.f, 0.f), vector3df(0.f, 45.f, 0.f));
	cubeNode->setMaterialTexture(0, driver->getTexture("Textures/Ciment1.png"));
	cubeNode->setMaterialTexture(1, driver->getTexture("Textures/Ciment1NM.png"));
	cubeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	smgr->getMeshManipulator()->recalculateNormals(cubeNode->getMesh(), true, true);
	cubeNode->setMaterialFlag(EMF_LIGHTING, false);
	Handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH);

	cp3d::rendering::SShadowLight light1(1024, vector3df(0.f, 100.f, 100.f), vector3df(0.f), SColor(255, 255, 255, 255), 1.f, 400.f, 90.f * f32(irr::core::DEGTORAD64), false);
	light1.setMustAutoRecalculate(false);
	Handler->addShadowLight(light1);

	Handler->setAmbientColor(SColor(255, 32, 32, 32));

	EditionTool->addTab("General");
	EditionTool->addTab("Materials");
}

#endif

} /// End namespace cp3d
