#include "Stdafx.h"

#include "CCP3DHDRManager.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "CHDRBloom.h"
#include "CHDRTextureAdder.h"
#include "CHDRLuminance.h"
#include "CHDRBrightPass.h"
#include "CHDRGaussianBlur.h"
#include "CHDRLensFlare.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRManager::CHDRManager(CCP3DHandler *handler) : Handler(handler), Enabled(true), LuminanceEnabled(true), LensFlareEnabled(true) {
	/// Configure
	Driver = Handler->getVideoDriver();
	Timer = Handler->getIrrlichtDevice()->getTimer();
	TextureAdderRTT = Driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DHDRTextureAdder", ECF_A32B32G32R32F);
	HdrRTT = Driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DHDR", ECF_A8R8G8B8);

	/// Build screen quad
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	ScreenQuad.initializeD3D11(Driver);
	#endif

	/// Material
	CMaterialCreator cmat(handler->getVideoDriver());
	cmat.addDefine("POST_PROCESS", "");

	Callback = new CScreenQuadCB(Handler, true);
	MaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/HDR.fragment.fx", EMT_SOLID, Callback);

	Callback->uniformDescriptors["exposure"] = CScreenQuadCB::SUniformDescriptor(&Exposure, 1);
	Callback->uniformDescriptors["averageLuminance"] = CScreenQuadCB::SUniformDescriptor(&OutputLuminance, 1);

	Exposure = 1.f;
	LastTime = 0;
	MinLuminance = 1.f;
	MaxLuminance = 1e20f;
	IncreaseRate = 0.2f;
	DecreaseRate = 0.5f;
	OutputLuminance = -1.f;

	/// Create pipeline
	Bloom = new CHDRBloom(Handler);
	Bloom->addBlurConfiguration(Driver->getScreenSize() / 4);
	Bloom->addBlurConfiguration(Driver->getScreenSize() / 8);
	Bloom->addBlurConfiguration(Driver->getScreenSize() / 16);

	TextureAdder = new CHDRTextureAdder(Handler, TextureAdderRTT);
	Luminance = new CHDRLuminance(Handler);
	
	// Lens Flare and Depth sampler
	if (handler->getDepthPassManager()->getPassCount() == 0) {
		handler->getDepthPassManager()->addPass("HDRLensFlarePass");
		handler->getDepthPassManager()->setDepth("HDRLensFlarePass", 1000.f);
	}

	LensFlare = new CHDRLensFlare(Handler);
	LensFlare->LensTexture = Driver->getTexture("Textures/lenscolor.png");
	LensFlare->LensStarTexture = Driver->getTexture("Textures/lensstar.png");
}

CHDRManager::~CHDRManager() {

}

void CHDRManager::render(ITexture *source, ITexture *output, const rect<s32> &viewport) {
	if (!Enabled)
		return;

	/// Render pipeline
	Bloom->render(source, ScreenQuad);

	if (LuminanceEnabled)
		Driver->setRenderTarget(TextureAdderRTT, true, true, SColor(0x0));
	else {
		Driver->setRenderTarget(output, true, true, SColor(0x0));
		Driver->setViewPort(viewport);
	}

	TextureAdder->OtherRTT = Bloom->LastRT;
	TextureAdder->render(source, ScreenQuad);

	if (!LuminanceEnabled)
		return;

	Driver->setRenderTarget(Luminance->LuminanceRTTs[CHDRLuminance::LumStepsNum - 1], true, true, SColor(0x0));
	Luminance->renderLuminance(TextureAdderRTT, ScreenQuad);
	Luminance->renderDownSample(ScreenQuad);

	/// Compute HDR
	if (Driver->getDriverType() == EDT_DIRECT3D9)
		CurrentLuminance = *static_cast<f32*>(Luminance->LuminanceRTTs[0]->lock());
	else {
		CurrentLuminance = (static_cast<u8*>(Luminance->LuminanceRTTs[0]->lock()))[1];
		CurrentLuminance = CurrentLuminance / 255.f;
	}

	Luminance->LuminanceRTTs[0]->unlock();

	if (OutputLuminance < 0.f)
		OutputLuminance = CurrentLuminance;
	else {
		f32 dt = (f32(LastTime) - f32(Timer->getTime())) / 1000.f;

		if (CurrentLuminance < OutputLuminance + DecreaseRate * dt) {
			OutputLuminance += DecreaseRate * dt;
		}
		else if (CurrentLuminance > OutputLuminance - IncreaseRate * dt) {
			OutputLuminance -= IncreaseRate * dt;
		}
		else {
			OutputLuminance = CurrentLuminance;
		}
	}

	OutputLuminance = clamp(OutputLuminance, MinLuminance, MaxLuminance);
	LastTime = Timer->getTime();

	if (LensFlareEnabled)
		Driver->setRenderTarget(HdrRTT, true, true, SColor(0x0));
	else {
		Driver->setRenderTarget(output, true, true, SColor(0x0));
		Driver->setViewPort(viewport);
	}

	ScreenQuad.getMaterial().setTexture(0, TextureAdderRTT);
	ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialType;
	ScreenQuad.render(Driver);

	// Lens flare
	if (LensFlareEnabled) {
		Driver->setRenderTarget(LensFlare->LensFlareRTT, true, true, SColor(0x0));
		LensFlare->render(Bloom->BrightPassRT, ScreenQuad);

		Driver->setRenderTarget(output, true, true, SColor(0x0));
		Driver->setViewPort(viewport);
		LensFlare->renderFinal(HdrRTT, ScreenQuad);
	}
}

void CHDRManager::setGaussianCoefficient(const irr::f32 coeff) {
	Bloom->GaussianBlur->GaussCoeff = coeff;
	Bloom->GaussianBlur->calculateWeights();
}

f32 CHDRManager::getGaussianCoefficient() const {
	return Bloom->GaussianBlur->GaussCoeff;
}

void CHDRManager::setGaussianMean(const f32 mean) {
	Bloom->GaussianBlur->GaussMean = mean;
	Bloom->GaussianBlur->calculateWeights();
}

f32 CHDRManager::getGaussianMean() const {
	return Bloom->GaussianBlur->GaussMean;
}

void CHDRManager::setGaussianStandardDerivation(const f32 standDev) {
	Bloom->GaussianBlur->GaussStandDev = standDev;
	Bloom->GaussianBlur->calculateWeights();
}

f32 CHDRManager::getGaussianStandardDerivation() const {
	return Bloom->GaussianBlur->GaussStandDev;
}

void CHDRManager::setGaussWidth(const f32 width) {
	Bloom->GaussianBlur->GaussWidth = width;
}

f32 CHDRManager::getGaussianWidth() const {
	return Bloom->GaussianBlur->GaussWidth;
}

void CHDRManager::setBrightnessThreshold(const f32 threshold) {
	Bloom->BrightPass->BrightThreshold = threshold;
}

f32 CHDRManager::getBrightnessThreshold() const {
	return Bloom->BrightPass->BrightThreshold;
}

void CHDRManager::setLensTexture(irr::video::ITexture *texture) {
	LensFlare->LensDirtTexture = texture;
}

const irr::video::ITexture *CHDRManager::getLensTexture() const {
	return LensFlare->LensDirtTexture;
}

void CHDRManager::setAutoUpdate(const bool autoUpdate) {
	LensFlare->AutoUpdate = autoUpdate;
}

} /// End namespace rendering
} /// End namespace cp3d
