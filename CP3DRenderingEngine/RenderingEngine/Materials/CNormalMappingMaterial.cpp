
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

CNormalMappingMaterial::CNormalMappingMaterial(CCP3DRenderingEngine *renderingEngine) : RenderingEngine(renderingEngine)
{
	CMaterialCreator m(renderingEngine->getVideoDriver());
	m.addDefine("__CP3D__MAX_LIGHTS__", stringc(__CP3D__MAX_LIGHTS__));

	s32 MatType = m.createMaterialFromFiles("Shaders/Materials/NormalMapping.vertex.fx", "Shaders/Materials/NormalMapping.fragment.fx", EMT_SOLID, this);
	renderingEngine->NormalMappingMaterialSolid = (E_MATERIAL_TYPE)MatType;

	MatType = m.createMaterialFromFiles("Shaders/Materials/NormalMapping.vertex.fx", "Shaders/Materials/NormalMapping.fragment.fx", EMT_TRANSPARENT_ADD_COLOR, this);
	renderingEngine->NormalMappingMaterialTransAdd = (E_MATERIAL_TYPE)MatType;

	MatType = m.createMaterialFromFiles("Shaders/Materials/NormalMapping.vertex.fx", "Shaders/Materials/NormalMapping.fragment.fx", EMT_TRANSPARENT_ALPHA_CHANNEL_REF, this);
	renderingEngine->NormalMappingMaterialTransAlphaRef = (E_MATERIAL_TYPE)MatType;
}

CNormalMappingMaterial::~CNormalMappingMaterial() {

}

void CNormalMappingMaterial::computeArrays() {
	LightColorArray.clear();
	LightPositionArray.clear();
	LightStrengthArray.clear();

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

			LightColorArray.push_back(ln->getLightData().DiffuseColor.getRed());
			LightColorArray.push_back(ln->getLightData().DiffuseColor.getGreen());
			LightColorArray.push_back(ln->getLightData().DiffuseColor.getBlue());
			LightColorArray.push_back(ln->getLightData().DiffuseColor.getAlpha());
		}
	}

}

void CNormalMappingMaterial::OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData) {
	computeArrays();

	/// Vertex
	matrix4 matWorldInverse;
    matWorldInverse = services->getVideoDriver()->getTransform(ETS_WORLD);
    matWorldInverse.makeInverse();
	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		services->setVertexShaderConstant("matWorldInverse", matWorldInverse.pointer(), 16);
	else
		services->setPixelShaderConstant("matWorldInverse", matWorldInverse.pointer(), 16);

	if (services->getVideoDriver()->getDriverType() == EDT_DIRECT3D9) {
		matrix4 worldView;
		worldView *= services->getVideoDriver()->getTransform(ETS_VIEW);
		worldView *= services->getVideoDriver()->getTransform(ETS_WORLD);
		services->setPixelShaderConstant("ModelViewMatrix", worldView.pointer(), 16);

		matrix4 worldViewProj;
		worldViewProj *= services->getVideoDriver()->getTransform(ETS_PROJECTION);
		worldViewProj *= services->getVideoDriver()->getTransform(ETS_VIEW);
		worldViewProj *= services->getVideoDriver()->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("ModelViewProjectionMatrix", worldViewProj.pointer(), 16);
	} else {
        matrix4 worldView;
		worldView *= services->getVideoDriver()->getTransform(ETS_VIEW);
		worldView *= services->getVideoDriver()->getTransform(ETS_WORLD);
        services->setVertexShaderConstant("ModelViewMatrix", worldView.pointer(), 16);
	}

	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL) {
		services->setVertexShaderConstant("fvLightPosition", LightPositionArray.pointer(), LightPositionArray.size());
		services->setVertexShaderConstant("fLightStrength", LightStrengthArray.pointer(), LightStrengthArray.size());
	} else {
		services->setPixelShaderConstant("fvLightPosition", LightPositionArray.pointer(), LightPositionArray.size());
		services->setPixelShaderConstant("fLightStrength", LightStrengthArray.pointer(), LightStrengthArray.size());
	}

	s32 lightsCount = RenderingEngine->getLightCount();
	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		services->setVertexShaderConstant("numLights", (s32 *)&lightsCount, 1);

	/// Pixel
	if (services->getVideoDriver()->getDriverType() == EDT_OPENGL) {
		s32 baseMap = 0;
		s32 bumpMap = 1;
		services->setPixelShaderConstant("baseMap", &baseMap, 1);
		services->setPixelShaderConstant("bumpMap", &bumpMap, 1);
	}

	f32 fvAmbiant[4] = { 1.f, 1.f, 1.f, 1.f };
    services->setPixelShaderConstant(("fvAmbient"), fvAmbiant, 4);

	services->setPixelShaderConstant("fvLightColor", LightColorArray.pointer(), LightColorArray.size() * 4);

	f32 fSpecularPower = 20.f;
    f32 fSpecularStrength = 1.9f;
    f32 fBumpStrength = 3.f;
    services->setPixelShaderConstant(("fSpecularPower"), &fSpecularPower, 1);
    services->setPixelShaderConstant(("fSpecularStrength"), &fSpecularStrength, 1);
    services->setPixelShaderConstant(("fBumpStrength"), &fBumpStrength, 1);

	services->setPixelShaderConstant("numLights", &lightsCount, 1);
}

} /// End namespace rendering
} /// End namespace cp3d
