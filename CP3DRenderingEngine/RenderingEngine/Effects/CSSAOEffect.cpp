#include "Stdafx.h"

#include "CSSAOEffect.h"
#include "../Handler/CP3DHandler.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CSSAOEffect::CSSAOEffect(CCP3DHandler *handler) : Handler(handler) {
	IVideoDriver *driver = handler->getVideoDriver();

	if (handler->getDepthPassManager()->getPassCount() == 0) {
		handler->getDepthPassManager()->addPass("SSAODepthRTT");
		handler->getDepthPassManager()->setDepth("SSAODepthRTT", 1000.f);
	}

	NormalRTT = driver->getTexture("CP3DNormalPass");
	DepthRTT = driver->getTexture("SSAODepthRTT");
	RandomTex = handler->generateRandomVectorTexture(dimension2du(512, 512), "SSAORandomTex");

	// Create post-processes
	SSAOMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAO.fragment.fx", this);
	BlurHMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");
	BlurVMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurVP.fragment.fx");
	SSAOCombineMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAOCombine.fragment.fx");

	// Set custom RTT
	dimension2du size = driver->getScreenSize() / 2;
	handler->setPostProcessCustomRTT(SSAOMaterial, size, "SSAOMaterial");
	handler->setPostProcessCustomRTT(BlurHMaterial, size, "BlurHMaterial");
	handler->setPostProcessCustomRTT(BlurVMaterial, size, "BlurVMaterial");
}

CSSAOEffect::~CSSAOEffect() {
	/// Set callback to 0 to keep "this" instance in memory
	/// removePostprocessingEffect deletes the callback instance
	Handler->setPostProcessingRenderCallback(SSAOMaterial, 0);

	Handler->removePostProcessingEffect(SSAOMaterial);
	Handler->removePostProcessingEffect(BlurHMaterial - 1);
	Handler->removePostProcessingEffect(BlurVMaterial - 2);
	Handler->removePostProcessingEffect(SSAOCombineMaterial - 3);

	Handler->getDepthPassManager()->removePass("SSAODepthRTT");
	Handler->getVideoDriver()->removeTexture(RandomTex);

	Handler->setPostProcessingTextureAtIndex(2, 0);
	Handler->setPostProcessingTextureAtIndex(3, 0);
}

void CSSAOEffect::OnPreRender(ICP3DHandler* handler) {
	handler->setPostProcessingTextureAtIndex(1, NormalRTT);
	handler->setPostProcessingTextureAtIndex(2, DepthRTT);
	handler->setPostProcessingTextureAtIndex(3, RandomTex);
}

} /// End namespace rendering
} /// End namespace cp3d
