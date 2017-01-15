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
	MaterialTypeV = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/GaussianBlurH.fragment.fx", EMT_SOLID, CallbackV);

	CallbackH->uniformDescriptors["blurWidth"] = CScreenQuadCB::SUniformDescriptor(&GaussWidth, 1);
	CallbackV->uniformDescriptors["blurWidth"] = CScreenQuadCB::SUniformDescriptor(&GaussWidth, 1);

	GaussCoeff = 0.25f;
	GaussMean = 1.f;
	GaussStandDev = 1.f;
	GaussWidth = 2.f;
	calculateWeights();
}

CHDRGaussianBlur::~CHDRGaussianBlur() {

}

void CHDRGaussianBlur::calculateWeights() {
	f32 x;
	for (u32 i = 0; i < 9; i++) {
		x = (f32(i) - 4.f) / 4.f;
		BlurWeights[i] = GaussCoeff * (1.0f / sqrt(2.0f * PI * GaussStandDev)) * expf((-((x - GaussMean) * (x - GaussMean))) / (2.f * GaussStandDev * GaussStandDev));
	}
}

void CHDRGaussianBlur::updateOffsets(const irr::core::dimension2du &size) {
	for (u32 i = 0; i < 9; i++) {
		BlurOffsetsH[i] = (f32(i) - 4.f) * (1.f / f32(size.Width / 2));
		BlurOffsetsV[i] = (f32(i) - 4.f) * (1.f / f32(size.Height / 2));
	}

	E_DRIVER_TYPE type = Driver->getDriverType();

	CallbackH->uniformDescriptors[type == EDT_OPENGL ? "blurOffsets[0]" : "blurOffsets"] = CScreenQuadCB::SUniformDescriptor(BlurOffsetsH, 9);
	CallbackH->uniformDescriptors[type == EDT_OPENGL ? "blurWeights[0]" : "blurWeights"] = CScreenQuadCB::SUniformDescriptor(BlurWeights, 9);

	CallbackV->uniformDescriptors[type == EDT_OPENGL ? "blurOffsets[0]" : "blurOffsets"] = CScreenQuadCB::SUniformDescriptor(BlurOffsetsV, 9);
	CallbackV->uniformDescriptors[type == EDT_OPENGL ? "blurWeights[0]" : "blurWeights"] = CScreenQuadCB::SUniformDescriptor(BlurWeights, 9);
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
