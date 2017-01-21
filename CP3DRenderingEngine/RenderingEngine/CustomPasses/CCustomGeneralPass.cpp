#include "Stdafx.h"

#include "CCustomGeneralPass.h"

#include "../Materials/CMaterialCreator.h"
#include "../Handler/CShaderPre.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CCustomGeneralPass::CCustomGeneralPass(IVideoDriver *driver, stringc name)
	: cp3d::rendering::ICP3DCustomPass(driver, name), VolumetricLightScatteringNode(0)
{
	/// Configure shaders pre-processors
	MaterialCreator = new CMaterialCreator(driver);

	MaterialType = MaterialCreator->createMaterialFromFiles("Shaders/Materials/MultiplePass.vertex.fx",
															"Shaders/Materials/MultiplePass.fragment.fx",
															EMT_SOLID, this);

	RenderTargets.push_back(driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DNormalPass", ECF_A32B32G32R32F));
	RenderTargets.push_back(driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DLightScattering", ECF_A8R8G8B8));
	RenderTargets.push_back(driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DVelocity", ECF_A32B32G32R32F));
}

CCustomGeneralPass::~CCustomGeneralPass() {
	for (u32 i=0; i < RenderTargets.size(); i++) {
		Driver->removeTexture(RenderTargets[i].RenderTexture);
	}
	RenderTargets.clear();
	delete MaterialCreator;
}

bool CCustomGeneralPass::setRenderTarget() {
	return Driver->setRenderTarget(RenderTargets, true, true, SColor(255, 32, 32, 0));
}

void CCustomGeneralPass::addNodeToPass(ISceneNode *node) {
	ICP3DCustomPass::addNodeToPass(node);

	matrix4 worldViewProjection;
	PrevWorldViewProjections.push_back(worldViewProjection);
	PrevWorldViewProjectionPointers.push_back(0);
}

void CCustomGeneralPass::OnSetConstants(IMaterialRendererServices* services, s32 userData) {

	E_DRIVER_TYPE type = Driver->getDriverType();

	core::matrix4 worldViewProj;
	worldViewProj = Driver->getTransform(video::ETS_PROJECTION);
	worldViewProj *= Driver->getTransform(video::ETS_VIEW);
	worldViewProj *= Driver->getTransform(video::ETS_WORLD);

	if (type != EDT_OPENGL) {
		services->setVertexShaderConstant("WorldViewProj", worldViewProj.pointer(), 16);
	}
	else {
		s32 texVar0 = 0;
		s32 texVar1 = 1;
		services->setPixelShaderConstant("BaseSampler", &texVar0, 1);
		services->setPixelShaderConstant("NormalSampler", &texVar1, 1);
	}

	/// Normal pass
	core::matrix4 worldView;
	worldView = Driver->getTransform(video::ETS_WORLD);
	worldView *= Driver->getTransform(video::ETS_VIEW);
	services->setVertexShaderConstant("WorldView", worldView.pointer(), 16);

	if (CurrentSceneNode < 0 || CurrentSceneNode >= SceneNodes.size())
		return;

	f32 farDistance = SceneNodes[CurrentSceneNode]->getSceneManager()->getActiveCamera()->getFarValue();
	services->setPixelShaderConstant("FarDistance", &farDistance, 1);
	
	/// Light Scattering
	bool renderNormal = false;
	if (SceneNodes[CurrentSceneNode] == VolumetricLightScatteringNode)
		renderNormal = true;

	s32 renderNormals32 = (s32)renderNormal;
	services->setPixelShaderConstant("LSRenderNormal", &renderNormals32, 1);

	/// Set veolicty
	const f32 *prevWorldViewProjectionPointer = PrevWorldViewProjectionPointers[CurrentSceneNode];

	if (prevWorldViewProjectionPointer != 0)
		services->setVertexShaderConstant("PrevWorldViewProj", prevWorldViewProjectionPointer, 16);

	/// Update velocity world view projections
	PrevWorldViewProjections[CurrentSceneNode] = worldViewProj;
	PrevWorldViewProjectionPointers[CurrentSceneNode] = PrevWorldViewProjections[CurrentSceneNode].pointer();
}

} /// End namespace rendering
} /// End namespace cp3d
