
#include "stdafx.h"

#define CP3DR_PROJECT_NAME "Community Play 3D Editor"
#define CP3DR_COMPILE_ENGINE
#include <CP3DCompileConfig.h>

#include "CCP3DEditorCore.h"
#include "CCP3DInterfaceController.h"
#include "../UserInterfaces/CCP3DContextMenu.h"
#include "../UserInterfaces/CCP3DMainToolbar.h"
#include "../UserInterfaces/CCP3DToolsToolbar.h"
#include "../UserInterfaces/CCP3DEditionTool.h"
#include "../UserInterfaces/CCP3DSceneGraph.h"

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
	MainToolbar = new CCP3DMainToolbar(this);
	ToolsToolbar = new CCP3DToolsToolbar(this);
	EditionTool = new CCP3DEditionTool(this);
	SceneGraph = new CCP3DSceneGraph(this);

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
	SceneGraph->fillGraph();
}

CCP3DEditorCore::~CCP3DEditorCore() {
	delete Rengine;
	delete Engine;
}

/// Creates components like camera etc.
void CCP3DEditorCore::createComponents() {
	ISceneManager *smgr = Device->getSceneManager();
	ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya();
	camera->setName("Editor camera");
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
	planeNode->setName("Plane");
	planeNode->setMaterialTexture(0, driver->getTexture("Textures/diffuse.tga"));
	planeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	Handler->addShadowToNode(planeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_RECEIVE);

	IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 25.f, 0.f), vector3df(0.f, 45.f, 0.f));
	cubeNode->setName("Cube");
	cubeNode->setMaterialTexture(0, driver->getTexture("Textures/Ciment1.png"));
	cubeNode->setMaterialTexture(1, driver->getTexture("Textures/Ciment1NM.png"));
	cubeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	smgr->getMeshManipulator()->recalculateNormals(cubeNode->getMesh(), true, true);
	cubeNode->setMaterialFlag(EMF_LIGHTING, false);
	Handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH);

	cp3d::rendering::ICP3DLightSceneNode *light = Rengine->createLightSceneNode(false, true);
	light->setName("Light");
	light->setPosition(vector3df(0.f, 0.f, 0.f));
	light->getLightData().DiffuseColor = SColorf(1.f, 0.f, 0.f, 1.f);
	light->getShadowLight()->setMustAutoRecalculate(true);
	light->setLightStrength(1.f);

	ISceneNode *emptySceneNode = smgr->addBillboardTextSceneNode(Gui->getSkin()->getFont(), L"Light :)", 0, dimension2df(30.f, 30.f), vector3df(0.f), -1, SColor(255, 255, 0, 0));
	emptySceneNode->setName("Text Node");
	emptySceneNode->setMaterialFlag(EMF_LIGHTING, false);
	emptySceneNode->setPosition(vector3df(0.f, 100.f, 100.f));
	Handler->addShadowToNode(emptySceneNode, rendering::EFT_NONE, rendering::ESM_EXCLUDE);
	light->setParent(emptySceneNode);
	emptySceneNode->setDebugDataVisible(EDS_BBOX);

	Rengine->createNormalMappingMaterial();
	planeNode->setMaterialType(Rengine->NormalMappingMaterialSolid);
	cubeNode->setMaterialType(Rengine->NormalMappingMaterialSolid);

	ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("Textures/Skybox/glacier_up.png"),
		driver->getTexture("Textures/Skybox/glacier_dn.png"),
		driver->getTexture("Textures/Skybox/glacier_lf.png"),
		driver->getTexture("Textures/Skybox/glacier_rt.png"),
		driver->getTexture("Textures/Skybox/glacier_ft.png"),
		driver->getTexture("Textures/Skybox/glacier_bk.png"));
	skyboxNode->setName("Skybox");

	u32 count = 0;
	auto callback = [&](ISceneNode *node) {
		count++;
		node->setName(stringc(stringc("Cloud Node ") + stringc(count)).c_str());
	};

	Engine->getSceneNodeCreator()->createCloudNode(vector2df(0.008f, 0.0f), driver->getTexture("Textures/Clouds/cloud01.png"), 1.f, 0.5f, 0.1f, -0.05f, callback);
	Engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud02.png"), 0.4f, 0.05f, -0.1f, 0.5f, callback);
	Engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud03.png"), 0.035f, 0.f, -0.15f, 0.4f, callback);

	Handler->setAmbientColor(SColor(255, 32, 32, 32));

	std::function<ICP3DEditionToolCallback(stringw text)> guiCallback = [&](stringw text) -> ICP3DEditionToolCallback {

		ICP3DEditionToolCallback c = [=](SCP3DInterfaceData data) {
			data.TextElement->setText(text.c_str());

			if (data.Type == EGUIET_EDIT_BOX)
				data.TextBox->setText(L"No Name");
			else if (data.Type == EGUIET_COMBO_BOX) {
				data.ComboBox->addItem(L"Item 1");
				data.ComboBox->addItem(L"Item 2");
			} else if (data.Type == EGUIET_LIST_BOX) {
				data.ListData.List->addItem(L"Item 1");
				data.ListData.List->addItem(L"Item 2");
			}

		};

		return c;
	};

	IGUITab *tab = EditionTool->addTab("General");
	EditionTool->setNewZone(tab, "Zone 1");
	EditionTool->addField(tab, EGUIET_EDIT_BOX, guiCallback("Name :"));
	EditionTool->addField(tab, EGUIET_EDIT_BOX, guiCallback("Name 2 Test"));
	EditionTool->setNewZone(tab, "Zone 2");
	EditionTool->addField(tab, EGUIET_LIST_BOX, guiCallback("List"));
	EditionTool->addField(tab, EGUIET_COMBO_BOX, guiCallback("ComboBox 1 :"));
	EditionTool->setNewZone(tab, "Zone 3");
	EditionTool->addField(tab, EGUIET_COMBO_BOX, guiCallback("ComboBox 2 :"));

	IGUITab *tab2 = EditionTool->addTab("Advanced");
	EditionTool->addField(tab2, EGUIET_LIST_BOX, guiCallback("List advanced"));
}

#endif

} /// End namespace cp3d
