#include "Stdafx.h"

#include "CHDRLensFlare.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRLensFlare::CHDRLensFlare(CCP3DHandler *handler) : Handler(handler), LensTexture(0) {
	Driver = Handler->getVideoDriver();

	LensFlareRTT = Driver->addRenderTargetTexture(Driver->getScreenSize() / 4, "CP3DHDRLensFlare", ECF_A8R8G8B8);

	CMaterialCreator cmat(handler->getVideoDriver());
	cmat.addDefine("SAMPLES", "8");

	/// Lens Flare
	Callback = new CScreenQuadCB(Handler, true);
	Callback->uniformDescriptors["dispersal"] = CScreenQuadCB::SUniformDescriptor(&Dispersal, 1);
	Callback->uniformDescriptors["haloWidth"] = CScreenQuadCB::SUniformDescriptor(&HaloWidth, 1);
	Callback->uniformDescriptors["distortion"] = CScreenQuadCB::SUniformDescriptor(&Distortion, 1);

	MaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/LensFlare.fragment.fx", EMT_SOLID, Callback);

	/// Lens Flare final
	CallbackFinal = new CScreenQuadCB(Handler, true);
	CallbackFinal->uniformDescriptors["motionScale"] = CScreenQuadCB::SUniformDescriptor(&MotionScale, 1);
	CallbackFinal->uniformDescriptors["inverseViewProjection"] = CScreenQuadCB::SUniformDescriptor(InverseViewProjection.pointer(), 16);
	CallbackFinal->uniformDescriptors["prevViewProjection"] = CScreenQuadCB::SUniformDescriptor(PrevViewProjection.pointer(), 16);

	cmat.clearDefines();
	cmat.addDefine("MAX_MOTION_SAMPLES", "64.0");
	FinalMaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/LensFlareFinal.fragment.fx", EMT_SOLID, CallbackFinal);

	Dispersal = 0.1f;
	HaloWidth = 0.4f;
	Distortion = 16.f;

	MotionScale = 0.1f;
	PrevViewProjection = Driver->getTransform(ETS_PROJECTION) * Driver->getTransform(ETS_VIEW);

	DepthTexture = Driver->getTexture("HDRLensFlarePass");
}

CHDRLensFlare::~CHDRLensFlare() {

}

void CHDRLensFlare::render(ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().setTexture(1, LensTexture);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialType;
	screenQuad.render(Driver);
}

void CHDRLensFlare::renderFinal(ITexture *other, CScreenQuad &screenQuad) {
	InverseViewProjection = Driver->getTransform(ETS_PROJECTION) * Driver->getTransform(ETS_VIEW);
	InverseViewProjection.makeInverse();

	screenQuad.getMaterial().setTexture(0, LensFlareRTT);
	screenQuad.getMaterial().setTexture(1, other);
	screenQuad.getMaterial().setTexture(2, LensStarTexture);
	screenQuad.getMaterial().setTexture(3, LensDirtTexture);
	screenQuad.getMaterial().setTexture(4, DepthTexture);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)FinalMaterialType;
	screenQuad.render(Driver);

	PrevViewProjection = Driver->getTransform(ETS_PROJECTION) * Driver->getTransform(ETS_VIEW);
}

} /// End namespace rendering
} /// End namespace cp3d
