#include <irrlicht.h>

/// CP3D
#define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
#include <CP3DCompileConfig.h>
#include <CP3DRenderingEngine.h>

namespace cp3d {
namespace test {

/// Custom post process
class CCustomPostProcess : public cp3d::rendering::IPostProcessingRenderCallback {
public:
	CCustomPostProcess(cp3d::rendering::ICP3DHandler *handler, irr::video::IVideoDriver *driver) {
		using namespace irr::core;
		stringc shader =
			"##ifdef OPENGL_DRIVER\n"
			"uniform sampler2D UserMapSampler;\n"
			"uniform sampler2D ColorMapSampler;\n"
			"void main() {\n"
			"	gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0) - texture2D(UserMapSampler, gl_TexCoord[0].xy)\n"
			"	+ texture2D(ColorMapSampler, gl_TexCoord[0].xy);\n"
			"}\n"
			"##else\n"
			"sampler2D UserMapSampler : register(s3);\n"
			"sampler2D ColorMapSampler : register(s0);\n"
			"float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR0 {\n"
			"	return float4(1.0, 1.0, 1.0, 1.0) - tex2D(UserMapSampler, texCoord)\n"
			"	+ tex2D(ColorMapSampler, texCoord);\n"
			"}\n"
			"##endif\n";
		MatType = handler->addPostProcessingEffectFromString(shader, this);
		Tex = driver->getTexture("CustomDepthPassRTT");
	}

	void OnPreRender(cp3d::rendering::ICP3DHandler* handler) {
		handler->setPostProcessingUserTexture(Tex);
	}
	void OnPostRender(cp3d::rendering::ICP3DHandler* handler) { }

private:
	irr::s32 MatType;
	irr::video::ITexture *Tex;
};

/// Custom General Pass
enum E_CUSTOM_GENERAL_PASS_TYPE {
	ECGPT_NORMAL = 0,
	ECGPT_LIGHT_SCATTERING
};

class CCustomGeneralPostProcess : public cp3d::rendering::IPostProcessingRenderCallback {
public:

	CCustomGeneralPostProcess(cp3d::rendering::ICP3DHandler *handler, irr::video::IVideoDriver *driver, E_CUSTOM_GENERAL_PASS_TYPE type) {
		using namespace irr::core;
		stringc shader =
			"##ifdef OPENGL_DRIVER\n"
			"uniform sampler2D UserMapSampler;\n"
			"void main() {\n"
			"	gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0) - texture2D(UserMapSampler, gl_TexCoord[0].xy);\n"
			"}\n"
			"##else\n"
			"sampler2D UserMapSampler : register(s0);\n"
			"float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR0 {\n"
			"	return float4(1.0, 1.0, 1.0, 1.0) - tex2D(UserMapSampler, texCoord);\n"
			"}\n"
			"##endif\n";
		MatType = handler->addPostProcessingEffectFromString(shader, this);

		if (type == ECGPT_NORMAL)
			Tex = driver->getTexture("CP3DNormalPass");

	}

	void OnPreRender(cp3d::rendering::ICP3DHandler *handler) {
		handler->setPostProcessingTextureAtIndex(0, Tex);
	}

private:
	irr::s32 MatType;
	irr::video::ITexture *Tex;
};

/// Custom post process from file
class CCustomPostProcessFile : public cp3d::rendering::IPostProcessingRenderCallback {
public:
	CCustomPostProcessFile(cp3d::rendering::ICP3DHandler *handler, irr::video::IVideoDriver *driver) {
		using namespace irr::core;
		MatType = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAO.fragment.fx", this);
		handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");
		handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurVP.fragment.fx");
		handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAOCombine.fragment.fx");
		MatType2 = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/FXAA.fragment.fx", this);

		Tex = handler->generateRandomVectorTexture(dimension2du(512, 512), "SSAORandomTexture");
		DepthTex = driver->getTexture("CustomDepthPassRTT");
		handler->getDepthPassManager()->setDepth("CustomDepthPassRTT", 200.f);

		BufferHeight = BufferWidth = 2048;

		Driver = driver;
	}

	void OnPreRender(cp3d::rendering::ICP3DHandler* handler) {
		using namespace irr::video;
		mViewProj = Driver->getTransform(ETS_PROJECTION) * Driver->getTransform(ETS_VIEW);
		handler->setPostProcessingEffectConstant(MatType, "mViewProj", mViewProj.pointer(), 16);

		handler->setPostProcessingEffectConstant(MatType2, "BufferWidth", &BufferWidth, 1);
		handler->setPostProcessingEffectConstant(MatType2, "BufferHeight", &BufferHeight, 1);

		handler->setPostProcessingTextureAtIndex(2, DepthTex);
		handler->setPostProcessingUserTexture(Tex);
	}
	void OnPostRender(cp3d::rendering::ICP3DHandler* handler) { }

private:
	irr::s32 MatType, MatType2;

	irr::video::ITexture *Tex, *DepthTex;
	irr::video::IVideoDriver *Driver;

	irr::core::matrix4 mViewProj;
	irr::f32 BufferWidth, BufferHeight;
};

class CCustomDepthCallback : public rendering::ICP3DHandlerCustomCallback
{

public:

	CCustomDepthCallback(irr::IrrlichtDevice *device) : rendering::ICP3DHandlerCustomCallback(true, true)
	{
		timer = device->getTimer();
	}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		irr::video::IVideoDriver *driver = services->getVideoDriver();
		irr::core::matrix4 m = driver->getTransform(irr::video::ETS_PROJECTION);
		m *= driver->getTransform(irr::video::ETS_VIEW);
		m *= driver->getTransform(irr::video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", m.pointer(), 16);
		irr::f32 time = (irr::f32)(timer->getTime()) / 600.f;
		services->setVertexShaderConstant("time", &time, 1);

		ICP3DHandlerCustomCallback::OnSetConstants(services, userData);
	}

private:
	irr::ITimer *timer;

};

void GlobalTest(irr::IrrlichtDevice *device) {
	using namespace irr;
	using namespace video;
	using namespace scene;
	using namespace core;
	using namespace gui;

	IVideoDriver *driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();
	IGUIEnvironment *gui = device->getGUIEnvironment();

	/// Create rendering engine
	cp3d::rendering::ICP3DRenderingEngine *cpre = cp3d::createRenderingEngine(device);
	cp3d::rendering::ICP3DHandler *handler = cpre->getHandler();

	/// Create a fps camera
	ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
	camera->setPosition(vector3df(100.f, 100.f, 0.f));
	device->getCursorControl()->setVisible(false);

	/// Create a custo material (for custom depth test)
	CCustomDepthCallback *customDepthCallback = new CCustomDepthCallback(device);
	rendering::ICP3DMaterialCreator *matCreator = cpre->createMaterialCreator();

	irr::s32 mat1 = matCreator->createMaterialFromFiles("Shaders/Materials/depthExample.vertex.fx", "Shaders/Materials/depthExample.fragment.fx",
														EMT_SOLID, customDepthCallback);

	irr::s32 mat2 = matCreator->createCustomDepthMaterialFromFiles("Shaders/Materials/depthExample.vertex.fx", "Shaders/Materials/depthExample.fragment.fx",
																   EMT_SOLID, customDepthCallback);

	irr::s32 mat3 = matCreator->createCustomShadowsMaterialFromFile("Shaders/Materials/depthExample.vertex.fx", EMT_SOLID, customDepthCallback);

	/// Create a test scene
	IAnimatedMesh *planeMesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(100, 100),
		0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));

	IMeshSceneNode *planeNode = smgr->addMeshSceneNode(planeMesh);
	planeNode->setMaterialTexture(0, driver->getTexture("Textures/diffuse.tga"));
	planeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
	planeNode->setMaterialTexture(2, driver->getTexture("Textures/specular.tga"));
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	planeNode->getMaterial(0).Shininess = 0.f;
	handler->addShadowToNode(planeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH, mat2, mat3, customDepthCallback);

	IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 50.f, 0.f), vector3df(0.f, 45.f, 0.f));
	cubeNode->setMaterialTexture(0, driver->getTexture("Textures/specular.tga"));
	cubeNode->setMaterialTexture(1, driver->getTexture("Textures/normal.tga"));
	cubeNode->setMaterialTexture(2, driver->getTexture("Textures/specular.tga"));
	cubeNode->setMaterialFlag(EMF_LIGHTING, false);
	cubeNode->getMaterial(0).Shininess = 0.f;
	handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH);

	/// Create the normal mapping material
	cpre->createNormalMappingMaterial();
	cubeNode->setMaterialType(cpre->Materials[EMT_SOLID]);
	planeNode->setMaterialType((E_MATERIAL_TYPE)mat1);

	/// Create a light
	cp3d::rendering::ICP3DLightSceneNode *light = cpre->createLightSceneNode(true, true);
	light->setPosition(vector3df(0.f, 100.f, 100.f));
	light->setLightColor(SColorf(1.f, 1.f, 1.f, 1.f));
	light->getLightData().SpecularColor = SColorf(1.f, 0.5f, 0.f, 1.f);
	light->getShadowLight()->setUseRoundSpotLight(false);
	light->getShadowLight()->setFarValue(600.f);
	light->getShadowLight()->setShadowMapResolution(1024);

	// Test hardware skinning
	/*
	IAnimatedMesh *animatedMesh = smgr->getMesh("data/batman.x");

	IAnimatedMeshSceneNode *animatedNode = smgr->addAnimatedMeshSceneNode(animatedMesh);
	animatedNode->setScale(vector3df(1.0f));
	animatedNode->setPosition(vector3df(120.f, 0.f, -150.f));
	animatedNode->setMaterialFlag(video::EMF_LIGHTING, false);
	handler->addShadowToNode(animatedNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH);
	cpre->getHWSkinningManager()->addNode(animatedNode);

	cp3d::rendering::ICP3DHardwareSkinningMaterial *hwmat = cpre->getHWSkinningManager()->getHWMaterial(animatedNode);
	//handler->addShadowToNode(animatedNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH, hwmat->getDepthMaterial(), hwmat->getShadowsMaterial(), (cp3d::rendering::ICP3DHandlerCustomCallback *)hwmat);
	*/

	/// Finish
	handler->setAmbientColor(SColor(255, 32, 32, 32));

	IGUIImage *img = gui->addImage(rect<s32>(driver->getScreenSize().Width - 512, 0, driver->getScreenSize().Width, 512));
	img->setScaleImage(true);

	/// Update the application
	while (device->run()) {
		if (!device->isWindowActive())
			continue;

		driver->beginScene(true, true, SColor(0x0));
		handler->update();

		ITexture *sm = handler->getShadowMapTexture(1024, false);
		img->setImage(sm);

		gui->drawAll();
		driver->endScene();
	}
}

} /// End namespace test
} /// End namespace cp3d
