// CP3DRenderingTests.cpp : définit le point d'entrée pour l'application console.

#include "stdafx.h"

#include <iostream>
#include <irrlicht.h>

/// Irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

/// CP3D
#define CP3DR_COMPILE_RENDERING_ENGINE // tell the compiler that we use the rendering engine
#include <CP3DRenderingEngine.h>

/// Create an event receiver to exit the application when escape is pressed
class CEventReceiver : public IEventReceiver {
public:

	CEventReceiver(IrrlichtDevice *device) : device(device) { }

	bool OnEvent(const SEvent &event) {
		if (event.EventType == EET_KEY_INPUT_EVENT) {
			if (event.KeyInput.Key == KEY_ESCAPE)
				device->closeDevice();

		}

		return false;
	}

private:
	IrrlichtDevice *device;

};

/// Custom post process
class CCustomPostProcess : public cp3d::rendering::IPostProcessingRenderCallback {
public:
	CCustomPostProcess(cp3d::rendering::ICP3DHandler *handler, IVideoDriver *driver) {
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
	s32 MatType;
	ITexture *Tex;
};

/// Custom post process from file
class CCustomPostProcessFile : public cp3d::rendering::IPostProcessingRenderCallback {
public:
	CCustomPostProcessFile(cp3d::rendering::ICP3DHandler *handler, IVideoDriver *driver) {
		MatType = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAO.fragment.fx", this);
		handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");
		handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurVP.fragment.fx");
		handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAOCombine.fragment.fx");

		Tex = handler->generateRandomVectorTexture(dimension2du(512, 512), "SSAORandomTexture");
		DepthTex = driver->getTexture("CustomDepthPassRTT");
		handler->getDepthPassManager()->setDepth("CustomDepthPassRTT", 500.f);

		Driver = driver;
	}

	void OnPreRender(cp3d::rendering::ICP3DHandler* handler) {
		mViewProj = Driver->getTransform(ETS_PROJECTION) * Driver->getTransform(ETS_VIEW);
		handler->setPostProcessingEffectConstant(MatType, "mViewProj", mViewProj.pointer(), 16);

		handler->setPostProcessingTextureAtIndex(2, DepthTex);
		handler->setPostProcessingUserTexture(Tex);
	}
	void OnPostRender(cp3d::rendering::ICP3DHandler* handler) { }

private:
	s32 MatType;
	ITexture *Tex, *DepthTex;
	IVideoDriver *Driver;
	matrix4 mViewProj;

};

/// Main function
int main(int argc, char* argv[]) {

	/// Create a device
	IrrlichtDevice *device = createDevice(EDT_DIRECT3D9, dimension2du(1280, 800), 32, false, false, false, 0);
	device->setEventReceiver(new CEventReceiver(device));
	IVideoDriver *driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();

	/// Create rendering engine
	cp3d::rendering::ICP3DRenderingEngine *cpre = cp3d::createRenderingEngine(device);
	cp3d::rendering::ICP3DHandler *handler = cpre->getHandler();

	/// Create a fps camera
	ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 200.f, 0.09f);
	camera->setPosition(vector3df(100.f, 100.f, 0.f));
	device->getCursorControl()->setVisible(false);

	/// Create a test scene
	IAnimatedMesh *planeMesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(50, 50),
													  0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));
	IAnimatedMeshSceneNode *planeNode = smgr->addAnimatedMeshSceneNode(planeMesh);
	planeNode->setMaterialTexture(0, driver->getTexture("Textures/diffuse.tga"));
	planeNode->setMaterialFlag(EMF_LIGHTING, false);
	handler->addShadowToNode(planeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_RECEIVE);

	IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 25.f, 0.f));
	cubeNode->setMaterialFlag(EMF_LIGHTING, false);
	handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_CAST);
	cubeNode->addAnimator(smgr->createFlyCircleAnimator(vector3df(0.f, 25.f, 0.f), 100.f));

	cp3d::rendering::SShadowLight light1(1024, vector3df(0.f, 100.f, 100.f), vector3df(0.f), SColor(255, 255, 255, 255), 1.f, 400.f, 90.f * f32(irr::core::DEGTORAD64), false);
	light1.setMustAutoRecalculate(false);
	handler->addShadowLight(light1);

	/// Add a custom depth pass
	cp3d::rendering::ICP3DCustomDepthPass *customDepthPassMgr = handler->getDepthPassManager();
	customDepthPassMgr->addNodeToPass(cubeNode);
	customDepthPassMgr->addPass("CustomDepthPassRTT");

	/// Add a custom filter (rendering the custom depth pass result)
	CCustomPostProcessFile *customPostProcess = new CCustomPostProcessFile(handler, driver);

	/// Finish
	handler->setAmbientColor(SColor(255, 32, 32, 32));

	/// Update the application
	u32 lastTime = device->getTimer()->getTime();
	while (device->run()) {
		if (device->getTimer()->getTime() - lastTime > 1000) {
			lastTime = device->getTimer()->getTime();
			handler->getShadowLight(0).setMustRecalculate(true);
		}

		driver->beginScene(true, true, SColor(0x0));
		handler->update();
		driver->endScene();
	}

	return 0;
}
