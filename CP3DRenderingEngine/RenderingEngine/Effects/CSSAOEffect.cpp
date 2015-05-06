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

	DepthRTT = driver->getTexture("SSAODepthRTT");
	RandomTex = handler->generateRandomVectorTexture(driver->getScreenSize(), "SSAORandomTex");

	// Create post-processes
	SSAOMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAO.fragment.fx", this);
	BlurHMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");
	BlurVMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/BlurHP.fragment.fx");
	SSAOCombineMaterial = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/SSAOCombine.fragment.fx");
}

CSSAOEffect::~CSSAOEffect() {
	/// Set callback to 0 to keep "this" instance in memory
	/// removePostprocessingEffect deletes the callback instance
	Handler->setPostProcessingRenderCallback(SSAOMaterial, 0);

	Handler->removePostProcessingEffect(SSAOMaterial);
	Handler->removePostProcessingEffect(BlurHMaterial);
	Handler->removePostProcessingEffect(BlurVMaterial);
	Handler->removePostProcessingEffect(SSAOCombineMaterial);

	Handler->getDepthPassManager()->removePass("SSAODepthRTT");
	Handler->getVideoDriver()->removeTexture(RandomTex);
}

void CSSAOEffect::OnPreRender(ICP3DHandler* handler) {
	handler->setPostProcessingTextureAtIndex(2, DepthRTT);
	handler->setPostProcessingTextureAtIndex(3, RandomTex);
}

} /// End namespace rendering
} /// End namespace cp3d
