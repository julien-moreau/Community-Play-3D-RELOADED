
#include "stdafx.h"

#define CP3DR_PROJECT_NAME "Community Play 3D Editor"
#define CP3DR_COMPILE_ENGINE
#define CP3DR_COMPILE_COLLABORATIVE
#include <CP3DCompileConfig.h>

#include "CCP3DEditorCore.h"
#include "CCP3DInterfaceController.h"
#include "CCP3DEditorTransformer.h"
#include "../UserInterfaces/CCP3DContextMenu.h"
#include "../UserInterfaces/CCP3DMainToolbar.h"
#include "../UserInterfaces/CCP3DToolsToolbar.h"
#include "../UserInterfaces/CCP3DEditionTool.h"
#include "../UserInterfaces/CCP3DSceneGraph.h"
#include "../UserInterfaces/CCP3DCustomView.h"
#include "../GUIElements/GUIFileSelector/CGUIFileSelector.h"

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
	device->getSceneManager()->getRootSceneNode()->setName("Smgr:RootSceneNode");

	Driver = device->getVideoDriver();
	ScreenSize = Driver->getScreenSize();

	/// Configure engine
	Engine = cp3d::createEngine(device);
	Engine->getCustomUpdater()->addCustomUpdate(this);

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
	(EditionTool = new CCP3DEditionTool(this))->createDefaultControllers();
	SceneGraph = new CCP3DSceneGraph(this);
	CustomView = new CCP3DCustomView(this);

	/// Create transformers
	EditorTransformer = new CCP3DEditorTransformer(this);

	/// Finish
	WorkingDirectory = ProjectDirectory = device->getFileSystem()->getWorkingDirectory() + "/";
	Device->getFileSystem()->addFileArchive("GUI/", false, true, io::EFAT_FOLDER);
	Device->getFileSystem()->addFileArchive(WorkingDirectory, false, true, io::EFAT_FOLDER);
	setProjectName(ProjectName);

	/// Tests
	/// Configure scene
	createComponents();
	Engine->getEventReceiver()->addEventReceiver(this);
	createTestScene();
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

void CCP3DEditorCore::OnPreUpdate() {
	/// Screen resized
	if (Driver->getScreenSize() != ScreenSize) {
		ScreenSize = Driver->getScreenSize();
	}

	/// Viewport
	rect<s32> viewPort;

	s32 positionY = 0, count = 0;
	core::list<IGUIElement *>::ConstIterator it = Gui->getRootGUIElement()->getChildren().begin();
	for (; it != Gui->getRootGUIElement()->getChildren().end(); ++it) {
		EGUI_ELEMENT_TYPE type = (*it)->getType();
		if (type == EGUIET_MENU || type == EGUIET_TOOL_BAR) {
			positionY += (*it)->getRelativePosition().getHeight();
			count++;
		}
	}
	positionY -= 2 * count;

	viewPort.UpperLeftCorner.X = EditionTool->getElementToResize()->getRelativePosition().getWidth();
	viewPort.UpperLeftCorner.Y = positionY;
	viewPort.LowerRightCorner.X = Driver->getScreenSize().Width - SceneGraph->getElementToResize()->getRelativePosition().getWidth();
	if (CustomView->getElementToResize()->isVisible())
		viewPort.LowerRightCorner.Y = Driver->getScreenSize().Height - CustomView->getElementToResize()->getRelativePosition().getHeight();
	else
		viewPort.LowerRightCorner.Y = Driver->getScreenSize().Height;

	Rengine->getHandler()->setViewPort(viewPort);
	EditorTransformer->setViewPort(viewPort);
}

/// Runs the editor
void CCP3DEditorCore::runEditor() {
	Engine->runEngine();
}

ui::ICP3DFileSelector *CCP3DEditorCore::createFileOpenDialog(irr::core::stringw name, irr::gui::IGUIElement *parent, ui::ICP3DFileSelector::E_FILESELECTOR_TYPE type) {
	ui::CGUIFileSelector *selector = new ui::CGUIFileSelector(name.c_str(), Gui, parent == 0 ? Gui->getRootGUIElement() : parent, -1, type);

	selector->setCustomDirectoryIcon(Driver->getTexture(WorkingDirectory + "GUI/open.png"));
	selector->setCustomFileIcon(Driver->getTexture(WorkingDirectory + "GUI/parameters.png"));
	
	selector->addPlacePaths(L"CP3D Directory", WorkingDirectory.c_str(), Driver->getTexture(WorkingDirectory + "GUI/parameters.png"));
	selector->addPlacePaths(L"Project Directory", ProjectDirectory.c_str(), Driver->getTexture(WorkingDirectory + "GUI/play_game.png"));
	selector->addPlacePaths(L"Opened Place", Device->getFileSystem()->getWorkingDirectory(), Driver->getTexture(WorkingDirectory + "Gui/open.png"));

	return selector;
}

IGUIWindow *CCP3DEditorCore::createMessageBox(stringw title, stringw text, s32 flags, bool modal, ITexture *texture) {
	return Gui->addMessageBox(title.c_str(), text.c_str(), modal, flags, 0, -1, texture);
}

} /// End namespace cp3d

/*
Manages events and creates the default scene
*/
namespace cp3d {

bool CCP3DEditorCore::OnEvent(const SEvent &event) {
	if (event.EventType == EET_KEY_INPUT_EVENT) {
		#ifdef _DEBUG
		if (event.KeyInput.Key == KEY_ESCAPE) {
			Device->closeDevice();
		}
		#endif
	}

	return false;
}

#include <ICP3DHandlerPostProcesses.h>
class CSSAOCallback : public rendering::IPostProcessingRenderCallback
{

public:

	CSSAOCallback(rendering::ICP3DRenderingEngine *renderingEngine) {
		IVideoDriver *driver = renderingEngine->getVideoDriver();
		renderingEngine->getHandler()->getDepthPassManager()->addPass("DepthRTT");

		DepthRTT = driver->getTexture("DepthRTT");
		RandomTex = renderingEngine->getHandler()->generateRandomVectorTexture(dimension2du(512, 512), "random");
	}

	void OnPreRender(rendering::ICP3DHandler* handler) {
		handler->setPostProcessingTextureAtIndex(2, DepthRTT);
		handler->setPostProcessingTextureAtIndex(3, RandomTex);
	}

private:

	ITexture *DepthRTT, *RandomTex;

};

void CCP3DEditorCore::createTestScene() {
	ISceneManager *smgr = Device->getSceneManager();
	IVideoDriver *driver = Device->getVideoDriver();
	/// Create a test scene
	IAnimatedMesh *planeMesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(50, 50),
													  0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));
	IAnimatedMeshSceneNode *planeNode = smgr->addAnimatedMeshSceneNode(planeMesh);
	planeNode->setName("Plane");
	planeNode->setMaterialTexture(0, driver->getTexture("Textures/empty.jpg"));
	planeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	Handler->addShadowToNode(planeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_RECEIVE);
	Engine->getSceneNodeCreator()->configureSceneNode(planeNode);

	IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 25.f, 0.f), vector3df(0.f, 45.f, 0.f));
	cubeNode->setName("Cube");
	cubeNode->setMaterialTexture(0, driver->getTexture("Textures/empty.jpg"));
	cubeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
	cubeNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	smgr->getMeshManipulator()->recalculateNormals(cubeNode->getMesh(), true, false);
	cubeNode->setMaterialFlag(EMF_LIGHTING, false);
	Handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH);
	Engine->getSceneNodeCreator()->configureSceneNode(cubeNode);

	cp3d::rendering::ICP3DLightSceneNode *light = Rengine->createLightSceneNode(true, true);
	light->setName("Light");
	light->setPosition(vector3df(0.f, 0.f, 0.f));
	light->setLightColor(SColorf(1.f, 1.f, 1.f, 1.f));
	light->getLightData().SpecularColor = SColorf(1.f, 0.5f, 0.f, 1.f);
	light->getShadowLight()->setMustAutoRecalculate(true);
	light->setLightStrength(1.f);

	IBillboardTextSceneNode *emptySceneNode = smgr->addBillboardTextSceneNode(Gui->getSkin()->getFont(), L"Light :)", 0, dimension2df(30.f, 30.f), vector3df(0.f), -1, SColor(255, 255, 0, 0));
	emptySceneNode->setName("Text Node");
	emptySceneNode->setMaterialFlag(EMF_LIGHTING, false);
	emptySceneNode->setPosition(vector3df(-100.f, 100.f, 0.f));
	Handler->addShadowToNode(emptySceneNode, rendering::EFT_NONE, rendering::ESM_EXCLUDE);
	light->setParent(emptySceneNode);
	emptySceneNode->setDebugDataVisible(EDS_BBOX);
	Engine->getSceneNodeCreator()->configureSceneNode(emptySceneNode);

	Rengine->createNormalMappingMaterial();
	planeNode->setMaterialType(Rengine->Materials[EMT_SOLID]);
	cubeNode->setMaterialType(Rengine->Materials[EMT_SOLID]);

	ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("Textures/Skybox/glacier_up.png"),
		driver->getTexture("Textures/Skybox/glacier_dn.png"),
		driver->getTexture("Textures/Skybox/glacier_lf.png"),
		driver->getTexture("Textures/Skybox/glacier_rt.png"),
		driver->getTexture("Textures/Skybox/glacier_ft.png"),
		driver->getTexture("Textures/Skybox/glacier_bk.png"));
	skyboxNode->setName("Skybox");
	skyboxNode->getMaterial(0).Name = "Up";
	skyboxNode->getMaterial(1).Name = "Down";
	skyboxNode->getMaterial(2).Name = "Left";
	skyboxNode->getMaterial(3).Name = "Right";
	skyboxNode->getMaterial(4).Name = "Front";
	skyboxNode->getMaterial(5).Name = "Back";
	skyboxNode->setMaterialType((E_MATERIAL_TYPE)Rengine->Materials[EMT_SOLID]);
	Engine->getSceneNodeCreator()->configureSceneNode(skyboxNode);

	IBillboardSceneNode * bill = smgr->addBillboardSceneNode(0, dimension2df(2.f, 2.f), vector3df(0.f, 100.f, 0.f));
	Engine->getSceneNodeCreator()->configureSceneNode(bill);
	Handler->addShadowToNode(bill, rendering::EFT_NONE, rendering::ESM_EXCLUDE);
	bill->setMaterialType(EMT_SOLID);
	bill->setName("BillBoard");
	bill->setColor(SColor(255, 255, 0, 0), SColor(255, 0, 255, 0));

	u32 count = 0;
	auto callback = [&](ISceneNode *node) {
		count++;
		node->setName(stringc(stringc("Cloud Node ") + stringc(count)).c_str());
	};

	Engine->getSceneNodeCreator()->createCloudNode(vector2df(0.008f, 0.0f), driver->getTexture("Textures/Clouds/cloud01.png"), 1.f, 0.5f, 0.1f, -0.05f, callback);
	Engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud02.png"), 0.4f, 0.05f, -0.1f, 0.5f, callback);
	Engine->getSceneNodeCreator()->createCloudNode(vector2df(0.006f, 0.003f), driver->getTexture("Textures/Clouds/cloud03.png"), 0.035f, 0.f, -0.15f, 0.4f, callback);

	Handler->setAmbientColor(SColor(255, 32, 32, 32));

	CSSAOCallback *c = new CSSAOCallback(Rengine);
	Handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAO.fragment.fx", c);
	Handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");
	Handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");

	Handler->getDepthPassManager()->addNodeToPass(planeNode);
	Handler->getDepthPassManager()->addNodeToPass(cubeNode);
}

} /// End namespace cp3d
