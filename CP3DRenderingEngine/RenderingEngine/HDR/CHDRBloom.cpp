#include "Stdafx.h"

#include "CHDRBloom.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "CHDRBrightPass.h"
#include "CHDRDownSample.h"
#include "CHDRGaussianBlur.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRBloom::CHDRBloom(CCP3DHandler *handler) : Handler(handler) {
	Driver = Handler->getVideoDriver();

	/// Rendering
	BrightPassRT = Driver->addRenderTargetTexture(Driver->getScreenSize() / 2, "CP3DHDRBrightPassRT", ECF_A32B32G32R32F);
	BlurRT1 = Driver->addRenderTargetTexture(Driver->getScreenSize() / 4, "CP3DHDRBlurRT1", ECF_A32B32G32R32F);
	BlurRT2 = Driver->addRenderTargetTexture(Driver->getScreenSize() / 4, "CP3DHDRBlurRT2", ECF_A32B32G32R32F);

	BrightPass = new CHDRBrightPass(handler);
	DownSample = new CHDRDownSample(handler);
	GaussianBlur = new CHDRGaussianBlur(handler);
}

CHDRBloom::~CHDRBloom() {

}

void CHDRBloom::render(ITexture *source, CScreenQuad &screenQuad) {
	/// Bright pass
	Driver->setRenderTarget(BrightPassRT, true, true, SColor(0x0));
	BrightPass->render(source, screenQuad);

	/// Down Sample
	Driver->setRenderTarget(BlurRT1, true, true, SColor(0x0));
	DownSample->render(BrightPassRT, screenQuad);

	/// Gaussian Blur
	Driver->setRenderTarget(BlurRT2, true, true, SColor(0x0));
	GaussianBlur->renderH(BlurRT1, screenQuad);

	Driver->setRenderTarget(BlurRT1, true, true, SColor(0x0));
	GaussianBlur->renderV(BlurRT2, screenQuad);
}

} /// End namespace rendering
} /// End namespace cp3d
