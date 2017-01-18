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
	BrightPassRT = Driver->addRenderTargetTexture(Driver->getScreenSize() / 4, "CP3DHDRBrightPassRT", ECF_A32B32G32R32F);
	DownSamplerRT = Driver->addRenderTargetTexture(Driver->getScreenSize() / 4, "CP3DHDRDownSampleRT", ECF_A32B32G32R32F);

	BrightPass = new CHDRBrightPass(handler);
	DownSample = new CHDRDownSample(handler);
	GaussianBlur = new CHDRGaussianBlur(handler);
}

CHDRBloom::~CHDRBloom() {

}

void CHDRBloom::render(ITexture *source, CScreenQuad &screenQuad) {
	/// Down sample
	Driver->setRenderTarget(DownSamplerRT, true, true, SColor(0x0));
	DownSample->render(source, screenQuad);

	/// Brightpass
	Driver->setRenderTarget(BrightPassRT, true, true, SColor(0x0));
	BrightPass->render(DownSamplerRT, screenQuad);

	/// Gaussian Blur
	const u32 blurSize = BlurConfigurations.size();

	for (u32 i = 0; i < blurSize; i++) {
		const SBlurConfiguration &config = BlurConfigurations[i];
		GaussianBlur->updateOffsets(config.Size);

		Driver->setRenderTarget(config.BlurRT1, true, true, SColor(255, 0, 0, 0));
		GaussianBlur->renderH(i == 0 ? BrightPassRT : LastRT, screenQuad);

		Driver->setRenderTarget(config.BlurRT2, true, true, SColor(255, 0, 0, 0));
		GaussianBlur->renderV(config.BlurRT1, screenQuad);

		LastRT = config.BlurRT2;
	}
}

void CHDRBloom::addBlurConfiguration(const irr::core::dimension2du &size) {
	SBlurConfiguration config;

	stringc blurName1 = stringc("CP3DHDRBlurRT1_") + stringc(BlurConfigurations.size());
	stringc blurName2 = stringc("CP3DHDRBlurRT2_") + stringc(BlurConfigurations.size());

	config.Size = size;
	config.BlurRT1 = Driver->addRenderTargetTexture(size, blurName1, ECF_A32B32G32R32F);
	config.BlurRT2 = Driver->addRenderTargetTexture(size, blurName2, ECF_A32B32G32R32F);

	BlurConfigurations.push_back(config);
}

} /// End namespace rendering
} /// End namespace cp3d
