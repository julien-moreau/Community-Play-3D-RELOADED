#include "Stdafx.h"

#include "CHDRLuminance.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRLuminance::CHDRLuminance(CCP3DHandler *handler) : Handler(handler) {
	Driver = Handler->getVideoDriver();

	CMaterialCreator cmat(handler->getVideoDriver());
	
	CallbackLuminance = new CScreenQuadCB(Handler, true);
	cmat.addDefine("MAGNITUDE", "");
	MaterialTypeLuminance = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/Luminance.fragment.fx", EMT_SOLID, CallbackLuminance);

	CallbackDownSample = new CScreenQuadCB(Handler, true);
	cmat.clearDefines();
	MaterialTypeDownSample = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/LuminanceDownSample.fragment.fx", EMT_SOLID, CallbackDownSample);

	MaterialTypeDownSampleFinal = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/LuminanceDownSample.fragment.fx", EMT_SOLID, CallbackDownSample);

	for (u32 i = 0; i < LumStepsNum; i++) {
		u32 len = u32(pow(3.f, i));
		LuminanceRTTs[i] = Driver->addRenderTargetTexture(dimension2d<u32>(len, len), stringc("CP3DHDRLuminance") + stringc(i), ECF_G32R32F);
	}

	/// Luminance
	f32 sU = (1.f / f32(Driver->getScreenSize().Width));
	f32 sV = (1.f / f32(Driver->getScreenSize().Height));

	LuminanceOffsets[0] = -0.5f * sU;
	LuminanceOffsets[1] = 0.5f  * sV;
	LuminanceOffsets[2] = 0.5f  * sU;
	LuminanceOffsets[3] = 0.5f  * sV;
	LuminanceOffsets[4] = -0.5f * sU;
	LuminanceOffsets[5] = -0.5f * sV;
	LuminanceOffsets[6] = 0.5f  * sU;
	LuminanceOffsets[7] = -0.5f * sV;

	E_DRIVER_TYPE type = Driver->getDriverType();
	CallbackLuminance->uniformDescriptors[type == EDT_OPENGL ? "lumOffsets[0]" : "lumOffsets"] = CScreenQuadCB::SUniformDescriptor(LuminanceOffsets, 8);

	/// Down Sample
	CallbackDownSample->uniformDescriptors[type == EDT_OPENGL ? "dsOffsets[0]" : "dsOffsets"] = CScreenQuadCB::SUniformDescriptor(DownSampleOffsets, 18);
	CallbackDownSample->uniformDescriptors["halfDestPixelSize"] = CScreenQuadCB::SUniformDescriptor(&HalfDestPixelSize, 1);
}

CHDRLuminance::~CHDRLuminance() {

}

void CHDRLuminance::calculateDownSampleOffsets(const dimension2du &dim) {
	u32 id = 0;
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			DownSampleOffsets[id] = f32(x) / f32(dim.Width);
			DownSampleOffsets[id + 1] = f32(y) / f32(dim.Height);
			id += 2;
		}
	}
}

void CHDRLuminance::renderLuminance(ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialTypeLuminance;
	screenQuad.render(Driver);
}

void CHDRLuminance::renderDownSample(CScreenQuad &screenQuad) {
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialTypeDownSample;

	for (u32 i = LumStepsNum - 1; i > 0; i--) {
		calculateDownSampleOffsets(LuminanceRTTs[i]->getSize());
		HalfDestPixelSize = 0.5f / f32(LuminanceRTTs[i - 1]->getSize().Width);

		Driver->setRenderTarget(LuminanceRTTs[i - 1], true, true, SColor(0x0));
		screenQuad.getMaterial().setTexture(0, LuminanceRTTs[i]);

		if (i == 1)
			screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialTypeDownSampleFinal;

		screenQuad.render(Driver);
	}
}

} /// End namespace rendering
} /// End namespace cp3d
