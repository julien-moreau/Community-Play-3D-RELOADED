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
	: cp3d::rendering::ICP3DCustomPass(driver, name)
{
	/// Configure shaders pre-processors
	MaterialCreator = new CMaterialCreator(driver);

	MaterialType = MaterialCreator->createMaterialFromFiles("Shaders/Materials/MultiplePass.vertex.fx",
															"Shaders/Materials/MultiplePass.fragment.fx",
															EMT_SOLID, this);

	RenderTargets.push_back(driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DNormalPass", ECF_A32B32G32R32F));
	RenderTargets.push_back(driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DLightScattering", ECF_A8R8G8B8));
}

CCustomGeneralPass::~CCustomGeneralPass() {
	for (u32 i=0; i < RenderTargets.size(); i++) {
		Driver->removeTexture(RenderTargets[i].RenderTexture);
	}
	RenderTargets.clear();
	delete MaterialCreator;
}

bool CCustomGeneralPass::setRenderTarget() {
	return Driver->setRenderTarget(RenderTargets, true, true, SColor(255, 0, 0, 0));
}

void CCustomGeneralPass::OnSetConstants(IMaterialRendererServices* services, s32 userData) {

	core::matrix4 worldViewProj;
	worldViewProj = Driver->getTransform(video::ETS_PROJECTION);			
	worldViewProj *= Driver->getTransform(video::ETS_VIEW);
	worldViewProj *= Driver->getTransform(video::ETS_WORLD);
	services->setVertexShaderConstant("WorldViewProj", worldViewProj.pointer(), 16);

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
	ESCENE_NODE_TYPE type = SceneNodes[CurrentSceneNode]->getType();
	bool renderNormal = false;
	if (type == ESNT_BILLBOARD)
		renderNormal = true;

	s32 renderNormals32 = (s32)renderNormal;
	services->setPixelShaderConstant("LSRenderNormal", &renderNormals32, 1);
}

} /// End namespace rendering
} /// End namespace cp3d
