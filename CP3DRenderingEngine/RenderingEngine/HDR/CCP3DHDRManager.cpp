#include "Stdafx.h"

#include "CCP3DHDRManager.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "CHDRBloom.h"
#include "CHDRTextureAdder.h"
#include "CHDRLuminance.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRManager::CHDRManager(CCP3DHandler *handler) : Handler(handler) {
	/// Configure
	Driver = Handler->getVideoDriver();
	Timer = Handler->getIrrlichtDevice()->getTimer();
	TextureAdderRTT = Driver->addRenderTargetTexture(Driver->getScreenSize(), "CP3DHDRTextureAdder", ECF_A32B32G32R32F);

	/// Material
	CMaterialCreator cmat(handler->getVideoDriver());

	Callback = new CScreenQuadCB(Handler, true);
	MaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/HDR.fragment.fx", EMT_SOLID, Callback);

	Callback->uniformDescriptors["exposure"] = CScreenQuadCB::SUniformDescriptor(&Exposure, 1);
	Callback->uniformDescriptors["averageLuminance"] = CScreenQuadCB::SUniformDescriptor(&OutputLuminance, 1);

	Exposure = 1.f;
	LastTime = 0;
	MinLuminance = 1.f;
	MaxLuminance = 1e20f;
	IncreaseRate = 1.f;
	DecreaseRate = 2.f;
	OutputLuminance = -1.f;

	/// Create pipeline
	Bloom = new CHDRBloom(Handler);
	TextureAdder = new CHDRTextureAdder(Handler, TextureAdderRTT);
	Luminance = new CHDRLuminance(Handler);

	/// Configure Pipeline
	TextureAdder->OtherRTT = Bloom->BlurRT1;
}

CHDRManager::~CHDRManager() {

}

void CHDRManager::render(ITexture *source, ITexture *output) {
	/// Render pipeline
	Bloom->render(source, ScreenQuad);

	Driver->setRenderTarget(TextureAdderRTT, true, true, SColor(0x0));
	TextureAdder->OtherRTT = Bloom->BlurRT1;
	TextureAdder->render(source, ScreenQuad);

	Driver->setRenderTarget(Luminance->LuminanceRTTs[CHDRLuminance::LumStepsNum - 1], true, true, SColor(0x0));
	Luminance->renderLuminance(TextureAdderRTT, ScreenQuad);
	Luminance->renderDownSample(ScreenQuad);

	/// Compute HDR
	if (Driver->getDriverType() == EDT_DIRECT3D9)
		CurrentLuminance = *static_cast<f32*>(Luminance->LuminanceRTTs[0]->lock());

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

	Driver->setRenderTarget(output, true, true, SColor(0x0));
	ScreenQuad.getMaterial().setTexture(0, TextureAdderRTT);
	ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialType;
	ScreenQuad.render(Driver);
}

} /// End namespace rendering
} /// End namespace cp3d
