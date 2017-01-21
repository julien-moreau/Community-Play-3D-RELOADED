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

CHDRLensFlare::CHDRLensFlare(CCP3DHandler *handler) : Handler(handler), LensTexture(0), FirstUpdate(true) {
	Driver = Handler->getVideoDriver();

	LensFlareRTT = Driver->addRenderTargetTexture(Driver->getScreenSize() / 8, "CP3DHDRLensFlare", ECF_A8R8G8B8);

	CMaterialCreator cmat(handler->getVideoDriver());
	cmat.addDefine("SAMPLES", "8");
	cmat.addDefine("POST_PROCESS", "");

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
	CallbackFinal->uniformDescriptors["lensStarMatrix"] = CScreenQuadCB::SUniformDescriptor(LensStarMatrix.pointer(), 16);

	cmat.clearDefines();
	cmat.addDefine("MAX_MOTION_SAMPLES", "64.0");
	cmat.addDefine("POST_PROCESS", "");
	FinalMaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/LensFlareFinal.fragment.fx", EMT_SOLID, CallbackFinal);

	Dispersal = 0.1f;
	HaloWidth = 0.4f;
	Distortion = 16.f;

	MotionScale = 1.f;

	DepthTexture = Driver->getTexture("HDRLensFlarePass");
	VelocityTexture = Driver->getTexture("CP3DVelocity");
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
	/// Motion blur
	ICameraSceneNode *camera = Handler->getActiveSceneManager()->getActiveCamera();
	InverseViewProjection = camera->getViewMatrix() * camera->getProjectionMatrix();
	InverseViewProjection.makeInverse();

	if (FirstUpdate) {
		PrevViewProjection = camera->getViewMatrix() * camera->getProjectionMatrix();
		FirstUpdate = false;
	}

	MotionScale = f32(Driver->getFPS()) / 60.f;

	/// Lens Flare
	const matrix4 viewMatrix = camera->getViewMatrix();
	const vector3df camx = vector3df(viewMatrix(0, 0), viewMatrix(0, 1), viewMatrix(0, 2));
	const vector3df camz = vector3df(viewMatrix(2, 0), viewMatrix(2, 1), viewMatrix(2, 2));

	float cameraRotation = camx.dotProduct(vector3df(0.f, 0.f, 1.f)) + camz.dotProduct(vector3df(0.f, 1.f, 0.f));
	cameraRotation *= 4.f;

	LensStarMatrix.makeIdentity();
	LensStarMatrix(0, 0) = cosf(cameraRotation) * 0.5f;
	LensStarMatrix(1, 0) = -sinf(cameraRotation);
	LensStarMatrix(2, 0) = 0.f;
	LensStarMatrix(3, 0) = 1.f;

	LensStarMatrix(0, 1) = sinf(cameraRotation);
	LensStarMatrix(1, 1) = cosf(cameraRotation) * 0.5f;
	LensStarMatrix(2, 1) = 0.f;
	LensStarMatrix(3, 1) = 1.f;

	LensStarMatrix(0, 2) = 0.f;
	LensStarMatrix(1, 2) = 0.f;
	LensStarMatrix(2, 2) = 1.f;
	LensStarMatrix(3, 2) = 1.f;

	LensStarMatrix(0, 3) = 1.f;
	LensStarMatrix(1, 3) = 1.f;
	LensStarMatrix(2, 3) = 1.f;
	LensStarMatrix(3, 3) = 1.f;

	matrix4 sb1;
	sb1(0, 0) = 2.f;
	sb1(1, 0) = 0.f;
	sb1(2, 0) = -1.f;
	sb1(3, 0) = 1.f;

	sb1(0, 1) = 0.f;
	sb1(1, 1) = 2.f;
	sb1(2, 1) = -1.f;
	sb1(3, 1) = 1.f;

	sb1(0, 2) = 0.f;
	sb1(1, 2) = 0.f;
	sb1(2, 2) = 0.f;
	sb1(3, 2) = 1.f;

	sb1(0, 3) = 1.f;
	sb1(1, 3) = 1.f;
	sb1(2, 3) = 1.f;
	sb1(3, 3) = 1.f;

	matrix4 sb2;
	sb2(0, 0) = 0.5f;
	sb2(1, 0) = 0.f;
	sb2(2, 0) = 0.5f;
	sb2(3, 0) = 1.f;

	sb2(0, 1) = 0.f;
	sb2(1, 1) = 0.5f;
	sb2(2, 1) = 0.5f;
	sb2(3, 1) = 1.f;

	sb2(0, 2) = 0.f;
	sb2(1, 2) = 0.f;
	sb2(2, 2) = 1.f;
	sb2(3, 2) = 1.f;

	sb2(0, 3) = 1.f;
	sb2(1, 3) = 1.f;
	sb2(2, 3) = 1.f;
	sb2(3, 3) = 1.f;
	
	LensStarMatrix = sb2 * LensStarMatrix * sb1;

	/// Render
	screenQuad.getMaterial().setTexture(0, LensFlareRTT);
	screenQuad.getMaterial().setTexture(1, other);
	screenQuad.getMaterial().setTexture(2, DepthTexture);
	screenQuad.getMaterial().setTexture(3, LensStarTexture);
	screenQuad.getMaterial().setTexture(4, LensDirtTexture);
	screenQuad.getMaterial().setTexture(5, VelocityTexture);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)FinalMaterialType;
	screenQuad.render(Driver);

	PrevViewProjection = camera->getViewMatrix() * camera->getProjectionMatrix();
}

} /// End namespace rendering
} /// End namespace cp3d
