
#include "stdafx.h"
#include "CNormalMappingMaterial.h"

#include "../../CP3DRenderingEngine.h"
#include "CMaterialCreator.h"

#define __CP3D__MAX_LIGHTS__ 8

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

namespace cp3d {
namespace rendering {

CNormalMappingMaterial::CNormalMappingMaterial(CCP3DRenderingEngine *renderingEngine) : RenderingEngine(renderingEngine), LightCount(0)
{
	Driver = renderingEngine->getVideoDriver();

	CMaterialCreator m(renderingEngine->getVideoDriver());
	m.addDefine("__CP3D__MAX_LIGHTS__", stringc(__CP3D__MAX_LIGHTS__));

	s32 MatType = m.createMaterialFromFiles("Shaders/Materials/NormalMapping.vertex.fx", "Shaders/Materials/NormalMapping.fragment.fx", EMT_SOLID, this);
	RenderingEngine->Materials[EMT_NORMAL_MAP_SOLID] = (E_MATERIAL_TYPE)MatType;

	MatType = m.createMaterialFromFiles("Shaders/Materials/NormalMapping.vertex.fx", "Shaders/Materials/NormalMapping.fragment.fx", EMT_TRANSPARENT_ADD_COLOR, this);
	renderingEngine->Materials[EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR] = (E_MATERIAL_TYPE)MatType;

	MatType = m.createMaterialFromFiles("Shaders/Materials/NormalMapping.vertex.fx", "Shaders/Materials/NormalMapping.fragment.fx", EMT_TRANSPARENT_ALPHA_CHANNEL_REF, this);
	renderingEngine->Materials[EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA] = (E_MATERIAL_TYPE)MatType;

}

CNormalMappingMaterial::~CNormalMappingMaterial() {
	Driver->getMaterialRenderer(RenderingEngine->Materials[EMT_NORMAL_MAP_SOLID])->drop();
	Driver->getMaterialRenderer(RenderingEngine->Materials[EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR])->drop();
	Driver->getMaterialRenderer(RenderingEngine->Materials[EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA])->drop();
}

void CNormalMappingMaterial::computeArrays() {
	LightColorArray.clear();
	LightPositionArray.clear();
	LightStrengthArray.clear();
	LightCount = 0;

	for (u32 i=0;  i < RenderingEngine->getLightCount(); i++) {
		ICP3DLightSceneNode *l = RenderingEngine->getLightSceneNode(i);
		if (l->isComputingNormalMapping()) {
			LightStrengthArray.push_back(l->getLightStrength());

			ILightSceneNode *ln = *l;
			vector3df position = ln->getPosition();
			if (ln->getParent() != ln->getSceneManager()->getRootSceneNode()) {
				ln->updateAbsolutePosition();
				position = ln->getAbsolutePosition();
			}
			
			LightPositionArray.push_back(position.X);
			LightPositionArray.push_back(position.Y);
			LightPositionArray.push_back(position.Z);

			LightColorArray.push_back(ln->getLightData().SpecularColor.getRed());
			LightColorArray.push_back(ln->getLightData().SpecularColor.getGreen());
			LightColorArray.push_back(ln->getLightData().SpecularColor.getBlue());
			LightColorArray.push_back(ln->getLightData().SpecularColor.getAlpha());

			LightCount++;
		}
	}

}

void CNormalMappingMaterial::OnSetMaterial(const SMaterial& material) {
	Material = &material;
}

void CNormalMappingMaterial::OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData) {
	computeArrays();

	bool d3dDriver = Driver->getDriverType() == EDT_DIRECT3D9;
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	d3dDriver = d3dDriver || Driver->getDriverType() == EDT_DIRECT3D11;
	#endif

	/// Vertex
	matrix4 matWorldInverse;
    matWorldInverse = Driver->getTransform(ETS_WORLD);
    matWorldInverse.makeInverse();
	if (Driver->getDriverType() == EDT_OPENGL)
		services->setVertexShaderConstant("matWorldInverse", matWorldInverse.pointer(), 16);
	else
		services->setPixelShaderConstant("matWorldInverse", matWorldInverse.pointer(), 16);

	if (d3dDriver) {
		matrix4 worldView;
		worldView *= Driver->getTransform(ETS_VIEW);
		worldView *= Driver->getTransform(ETS_WORLD);
		services->setPixelShaderConstant("ModelViewMatrix", worldView.pointer(), 16);

		matrix4 worldViewProj;
		worldViewProj *= Driver->getTransform(ETS_PROJECTION);
		worldViewProj *= Driver->getTransform(ETS_VIEW);
		worldViewProj *= Driver->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("ModelViewProjectionMatrix", worldViewProj.pointer(), 16);
	} else {
        matrix4 worldView;
		worldView *= Driver->getTransform(ETS_VIEW);
		worldView *= Driver->getTransform(ETS_WORLD);
        services->setVertexShaderConstant("ModelViewMatrix", worldView.pointer(), 16);
	}

	if (Driver->getDriverType() == EDT_OPENGL) {
		services->setVertexShaderConstant("fvLightPosition[0]", LightPositionArray.pointer(), LightPositionArray.size());
		services->setVertexShaderConstant("fLightStrength[0]", LightStrengthArray.pointer(), LightStrengthArray.size());
	} else {
		services->setPixelShaderConstant("fvLightPosition", LightPositionArray.pointer(), LightPositionArray.size());
		services->setPixelShaderConstant("fLightStrength", LightStrengthArray.pointer(), LightStrengthArray.size());
	}

	if (Driver->getDriverType() == EDT_OPENGL)
		services->setVertexShaderConstant("numLights", &LightCount, 1);

	/// Pixel
	if (Driver->getDriverType() == EDT_OPENGL) {
		s32 baseMap = 0;
		s32 bumpMap = 1;
		s32 specularMap = 2;
		services->setPixelShaderConstant("baseMap", &baseMap, 1);
		services->setPixelShaderConstant("bumpMap", &bumpMap, 1);
		services->setPixelShaderConstant("specularMap", &specularMap, 1);
	}

	f32 fvAmbiant[4] = { 1.f, 1.f, 1.f, 1.f };
    services->setPixelShaderConstant(("fvAmbient"), fvAmbiant, 4);

	if (d3dDriver)
		services->setPixelShaderConstant("fvLightColor", LightColorArray.pointer(), LightColorArray.size() * 4);
	else
		services->setPixelShaderConstant("fvLightColor[0]", LightColorArray.pointer(), LightColorArray.size() * 4);

	f32 fSpecularPower = 20.f;
    f32 fSpecularStrength = 1.9f;
    f32 fBumpStrength = 5.f;
    services->setPixelShaderConstant(("fSpecularPower"), &fSpecularPower, 1);
    services->setPixelShaderConstant(("fSpecularStrength"), &fSpecularStrength, 1);
    services->setPixelShaderConstant(("fBumpStrength"), &fBumpStrength, 1);
	services->setPixelShaderConstant(("shininess"), &Material->Shininess, 1);

	services->setPixelShaderConstant("numLights", &LightCount, 1);
}

} /// End namespace rendering
} /// End namespace cp3d
