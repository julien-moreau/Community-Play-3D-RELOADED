#include "Stdafx.h"

#include "CCustomDepthPass.h"
#include "../Handler/CShaderPre.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CCustomDepthPass::CCustomDepthPass(IVideoDriver *driver, stringc name)
	: cp3d::rendering::ICP3DCustomDepthPass(driver, name), FarLinksChanged(true)
{
	/// Configure shaders pre-processors
	SppV = new CShaderPreprocessor(driver);
	SppP = new CShaderPreprocessor(driver);

	if (driver->getDriverType() == EDT_OPENGL) {
		SppV->addShaderDefine("OPENGL_DRIVER", "1");
		SppP->addShaderDefine("OPENGL_DRIVER", "1");
	}

	VertexShader = SppV->ppShaderFF("Shaders/Materials/CustomDepthPass.vertex.fx");
	PixelShader = SppP->ppShaderFF("Shaders/Materials/CustomDepthPass.fragment.fx");
}

CCustomDepthPass::~CCustomDepthPass() {
	for (u32 i=0; i < RenderTargets.size(); i++) {
		Driver->removeTexture(RenderTargets[i].RenderTexture);
	}

	RenderTargets.clear();
	delete SppV, SppP;
}

bool CCustomDepthPass::setRenderTarget() {
	if (RenderTargets.size() == 0)
		return false;

	if (Driver->getDriverType() == EDT_OPENGL) {
		return (RenderTargets.size() == 1) ? Driver->setRenderTarget(RenderTargets[0].RenderTexture, true, true, SColor(0xffffffff))
										   : Driver->setRenderTarget(RenderTargets, true, true, SColor(0xffffffff));
	} else
		return Driver->setRenderTarget(RenderTargets, true, true, SColor(0xffffffff));
}

void CCustomDepthPass::addPass(irr::core::stringc name) {
	IRenderTarget t(Driver->addRenderTargetTexture(Driver->getScreenSize(), name.c_str(), ECF_G32R32F));
	RenderTargets.push_back(t);
	FarLinks.push_back(200.f);

	/// Drop the old material
	IMaterialRenderer *m = Driver->getMaterialRenderer(MaterialType);
	if (m)
		m->drop();

	/// Create the new material
	createMaterial();
}

void CCustomDepthPass::createMaterial() {
	/// Configure shader
	SppP->addShaderDefine("__CP3D__PASSES_COUNT__", stringc(RenderTargets.size()));
	stringc shaderCode = "", shaderRT = "";
	for (u32 i = 0; i < RenderTargets.size(); i++) {
		if (Driver->getDriverType() != EDT_OPENGL)
			shaderRT += stringc("float4 Color") + stringc(i) + stringc(" : COLOR") + stringc(i) + stringc(";\n");

		if (Driver->getDriverType() != EDT_OPENGL) {
			shaderCode += (i == 0 ? stringc("float") : stringc("")) + stringc(" depth = In.ClipPos.z / MaxD[") + stringc(i) + stringc("];\n");
			shaderCode += stringc("OUT.Color") + stringc(i) + stringc(" = float4(depth, depth * depth, 0.0, 0.0);\n\n");
		}
		else {
			shaderCode += (i == 0 ? stringc("float") : stringc("")) + stringc(" depth = gl_TexCoord[0].z / MaxD[") + stringc(i) + stringc("];\n");
			shaderCode += (RenderTargets.size() == 1 ? stringc("gl_FragColor") : stringc("gl_FragData[") + stringc(i) + stringc("]")) + stringc(" = vec4(depth, depth * depth, 0.0, 0.0);\n");
		}
	}

	if (Driver->getDriverType() != EDT_OPENGL)
		SppP->addShaderDefine("__CP3D__RENDER_TARGETS__", shaderRT);
	SppP->addShaderDefine("__CP3D__PIXEL_MAIN__", shaderCode);

	#ifdef _DEBUG
	stringc vp = SppV->ppShader(VertexShader);
	stringc pp = SppP->ppShader(PixelShader);
	#endif

	/// Create material
	IGPUProgrammingServices *gpu = Driver->getGPUProgrammingServices();
	MaterialType = gpu->addHighLevelShaderMaterial(
		SppV->ppShader(VertexShader).c_str(), "vertexMain", video::EVST_VS_2_0,
		SppP->ppShader(PixelShader).c_str(), "pixelMain", video::EPST_PS_2_0,
		this, video::EMT_SOLID);

	FarLinksChanged = true;
}

void CCustomDepthPass::setDepth(irr::core::stringc name, irr::f32 farLink) {
	for (u32 i=0; i < RenderTargets.size(); i++) {
		if (RenderTargets[i].RenderTexture->getName().getPath() == name) {
			FarLinks[i] = farLink;
			FarLinksChanged = true;
			break;
		}
	}
}

u32 CCustomDepthPass::getPassCount() {
	return RenderTargets.size();
}

bool CCustomDepthPass::removePass(stringc name) {
	/// To implement
	for (u32 i = 0; i < RenderTargets.size(); i++) {
		if (RenderTargets[i].RenderTexture->getName().getPath() == name) {
			/// Remove elements
			Driver->removeTexture(RenderTargets[i].RenderTexture);
			FarLinks.erase(i);
			RenderTargets.erase(i);

			/// Finish
			if (RenderTargets.size() > 0)
				createMaterial();

			return true;
		}
	}

	return false;
}

void CCustomDepthPass::OnSetConstants(IMaterialRendererServices* services, s32 userData) {
	IVideoDriver* driver = services->getVideoDriver();

	core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(video::ETS_VIEW);
	worldViewProj *= driver->getTransform(video::ETS_WORLD);
	services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
	
	if (FarLinksChanged) {
		if (driver->getDriverType() == EDT_OPENGL)
			services->setPixelShaderConstant("MaxD[0]", FarLinks.pointer(), FarLinks.size());
		else
			services->setPixelShaderConstant("MaxD", FarLinks.pointer(), FarLinks.size());
	}
}

} /// End namespace rendering
} /// End namespace cp3d
