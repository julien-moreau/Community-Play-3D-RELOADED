// CP3DRenderingTests.cpp : définit le point d'entrée pour l'application console.

#include "stdafx.h"

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

/// Passes
enum E_SHADER_EXTENSION
{
	ESE_GLSL,
	ESE_HLSL,

	ESE_COUNT
};

const char* SHADOW_PASS_1V[ESE_COUNT] = {"uniform mat4 mWorldViewProj;\n"
"uniform float MaxD;\n"
""
"void main()"
"{"
"	vec4 tPos = mWorldViewProj * gl_Vertex;\n"
"	gl_Position = tPos;\n"
"	gl_TexCoord[0] = vec4(MaxD, tPos.y, tPos.z, tPos.w);\n"
""
"	gl_TexCoord[1].xy = gl_MultiTexCoord0.xy;\n"
"}"
,
"float4x4 mWorldViewProj;\n"
"float MaxD;\n"
""
"struct VS_OUTPUT "
"{"
"	float4 Position: POSITION0;\n"
"	float4 ClipPos: TEXCOORD0;\n"
"	float2 Texcoords: TEXCOORD1;\n"
"	float4 VColor: TEXCOORD2;\n"
"};\n"
""
"VS_OUTPUT vertexMain(float3 Position : POSITION0, float2 Texcoords : TEXCOORD0, float4 vColor : COLOR0)"
"{"
"	VS_OUTPUT  OUT;\n"
"	float4 hpos = mul(float4(Position.x, Position.y, Position.z, 1.0), mWorldViewProj);\n"
"   OUT.ClipPos = hpos;\n"
"	OUT.ClipPos.x = MaxD;\n"
"   OUT.Position = hpos;\n"
"	OUT.Texcoords = Texcoords;\n"
"	OUT.VColor = vColor;\n"
"	return(OUT);\n"
"}"};

const char* SHADOW_PASS_1P[ESE_COUNT] = {"void main() "
"{"
"	vec4 vInfo = gl_TexCoord[0];\n"
"	float depth = vInfo.z / vInfo.x;\n"
"   gl_FragColor = vec4(depth, depth * depth, 0.0, 0.0);\n"
"}"
,
"float4 pixelMain(float4 ClipPos: TEXCOORD0) : COLOR0"
"{"
"	float depth = ClipPos.z / ClipPos.x;\n"
"	return float4(depth, depth * depth, 0.0, 0.0);\n"
"}"};

class CCustomPassDepth : public cp3d::rendering::ICP3DCustomPass, public IShaderConstantSetCallBack {
public:

	CCustomPassDepth(IVideoDriver *driver, stringc name) : cp3d::rendering::ICP3DCustomPass(driver, name)
	{
		E_SHADER_EXTENSION shaderExt = driver->getDriverType() == EDT_OPENGL ? ESE_GLSL : ESE_HLSL;
		IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();
		MaterialType = gpu->addHighLevelShaderMaterial(
			SHADOW_PASS_1V[shaderExt], "vertexMain", video::EVST_VS_2_0,
			SHADOW_PASS_1P[shaderExt], "pixelMain", video::EPST_PS_2_0,
			this, video::EMT_SOLID);
		DepthRTT = driver->addRenderTargetTexture(driver->getScreenSize(), "CustomDepthPassRTT", ECF_G32R32F); /// 32 bits
	}

	~CCustomPassDepth() { }

	void setRenderTarget() {
		Driver->setRenderTarget(DepthRTT, true, true, SColor(0xffffffff));
	}

	void OnSetConstants(IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		if (driver->getDriverType() == EDT_DIRECT3D9) {
			core::matrix4 worldViewProj;
			worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
			worldViewProj *= driver->getTransform(video::ETS_VIEW);
			worldViewProj *= driver->getTransform(video::ETS_WORLD);

			services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		}
		
		f32 FarLink = 200.f;
		services->setVertexShaderConstant("MaxD", &FarLink, 1);
	}

private:
	ITexture *DepthRTT;

};

/// Custom post process
class CCustomPostProcess : public cp3d::rendering::IPostProcessingRenderCallback {
public:
	CCustomPostProcess(cp3d::rendering::ICP3DHandler *handler, IVideoDriver *driver) {
		E_SHADER_EXTENSION shaderExt = driver->getDriverType() == EDT_OPENGL ? ESE_GLSL : ESE_HLSL;
		stringc shader[ESE_COUNT] = {
		"uniform sampler2D UserMapSampler;\n"
		"void main() {\n"
		"	gl_FragColor = texture2D(UserMapSampler, gl_TexCoord[0].xy);\n"
		"}\n"
		,
		"sampler2D UserMapSampler : register(s3);\n"
		"float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR0 {\n"
		"	return tex2D(UserMapSampler, texCoord);\n"
		"}\n"
		};
		matType = handler->addPostProcessingEffectFromString(shader[shaderExt], this);
		tex = driver->getTexture("CustomDepthPassRTT");
	}

	void OnPreRender(cp3d::rendering::ICP3DHandler* handler) {
		handler->setPostProcessingUserTexture(tex);
	}
	void OnPostRender(cp3d::rendering::ICP3DHandler* handler) { }

private:
	s32 matType;
	ITexture *tex;
};

/// Main function
int main(int argc, char* argv[]) {

	/// Create a device
	IrrlichtDevice *device = createDevice(EDT_DIRECT3D9, dimension2du(800, 600), 32, false, false, false, 0);
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
	handler->addShadowToNode(cubeNode, cp3d::rendering::EFT_NONE, cp3d::rendering::ESM_BOTH);

	cp3d::rendering::SShadowLight light1(1024, vector3df(0.f, 100.f, 100.f), vector3df(0.f), SColor(255, 255, 255, 255), 1.f, 400.f, 90.f * f32(irr::core::DEGTORAD64), false);
	handler->addShadowLight(light1);

	/// Add a custom pass
	CCustomPassDepth *customDepthPass = new CCustomPassDepth(driver, "Custom Depth Pass");
	customDepthPass->addNodeToPass(cubeNode);
	handler->addCustomPass(customDepthPass);

	/// Add a custom filter (rendering the custom depth pass result)
	CCustomPostProcess *customPostProcess = new CCustomPostProcess(handler, driver);

	/// Finish
	handler->setAmbientColor(SColor(255, 32, 32, 32));

	/// Update the application
	while (device->run()) {
		driver->beginScene(true, true, SColor(0x0));

		handler->update();

		driver->endScene();
	}

	return 0;
}

