#include "Stdafx.h"

#include "CHDRGaussianBlur.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRGaussianBlur::CHDRGaussianBlur(CCP3DHandler *handler) : Handler(handler) {
	Driver = Handler->getVideoDriver();

	CMaterialCreator cmat(handler->getVideoDriver());
	
	CallbackH = new CScreenQuadCB(Handler, true);
	cmat.addDefine("GAUSSIAN_BLUR_H", "");
	MaterialTypeH = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/GaussianBlurH.fragment.fx", EMT_SOLID, CallbackH);

	CallbackV = new CScreenQuadCB(Handler, true);
	cmat.clearDefines();
	MaterialTypeV = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/GaussianBlurV.fragment.fx", EMT_SOLID, CallbackV);

	GaussCoeff = 0.3f;
	GaussMean = 1.f;
	GaussStandDev = 0.8f;
	calculateWeights();

	for (u32 i = 0; i < 9; i++) {
		BlurOffsetsH[i] = (f32(i) - 4.f) * (1.f / f32(Driver->getScreenSize().Width / 8));
		BlurOffsetsV[i] = (f32(i) - 4.f) * (1.f / f32(Driver->getScreenSize().Height / 8));
	}

	E_DRIVER_TYPE type = Driver->getDriverType();

	CallbackH->uniformDescriptors[type == EDT_OPENGL ? "blurOffsets[0]" : "blurOffsets"] = CScreenQuadCB::SUniformDescriptor(BlurOffsetsH, 9);
	CallbackH->uniformDescriptors[type == EDT_OPENGL ? "blurWeights[0]" : "blurWeights"] = CScreenQuadCB::SUniformDescriptor(BlurWeights, 9);

	CallbackV->uniformDescriptors[type == EDT_OPENGL ? "blurOffsets[0]" : "blurOffsets"] = CScreenQuadCB::SUniformDescriptor(BlurOffsetsV, 9);
	CallbackV->uniformDescriptors[type == EDT_OPENGL ? "blurWeights[0]" : "blurWeights"] = CScreenQuadCB::SUniformDescriptor(BlurWeights, 9);
}

CHDRGaussianBlur::~CHDRGaussianBlur() {

}

void CHDRGaussianBlur::calculateWeights() {
	f32 x;
	for (u32 i = 0; i < 9; i++) {
		x = (f32(i) - 4.f) / 4.f;
		BlurWeights[i] = GaussCoeff * (1.0f / sqrt(2.0f * PI * GaussStandDev * GaussStandDev)) * expf((-((x - GaussMean) * (x - GaussMean))) / (2.f * GaussStandDev * GaussStandDev));
	}
}

void CHDRGaussianBlur::renderH(ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialTypeH;
	screenQuad.render(Driver);
}

void CHDRGaussianBlur::renderV(ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialTypeV;
	screenQuad.render(Driver);
}

} /// End namespace rendering
} /// End namespace cp3d
