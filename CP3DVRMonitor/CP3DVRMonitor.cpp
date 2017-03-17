#include "stdafx.h"
#include "CP3DVRMonitor.h"

#include <CP3DRenderingEngine.h>
#include <ICP3DPluginManager.h>

#include "VR/Utils.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace engine {

REGISTER_MONITOR(CCP3DVRMonitor)
CCP3DVRMonitor::CCP3DVRMonitor(rendering::ICP3DRenderingEngine *rengine)
	: ICP3DMonitor(rengine)
{
	Handler = rengine->getHandler();
	Name = stringc("VR Monitor");

	// Init vr
	vr::HmdError vrError;
	VRSystem = vr::VR_Init(&vrError, vr::VRApplication_Scene);

	if (VRSystem) {
		// Render targets
		VRSystem->GetRecommendedRenderTargetSize(&RenderWidth, &RenderHeight);

		LeftRenderTarget = rengine->getVideoDriver()->addRenderTargetTexture(dimension2du(RenderWidth, RenderHeight), "leftEye", ECF_A8R8G8B8);
		RightRenderTarget = rengine->getVideoDriver()->addRenderTargetTexture(dimension2du(RenderWidth, RenderHeight), "rightEye", ECF_A8R8G8B8);

		HandlerRenderTargets.ScreenRTT = rengine->getVideoDriver()->addRenderTargetTexture(dimension2du(RenderWidth, RenderHeight), "ScreenRTT_VR", ECF_A8R8G8B8);
		HandlerRenderTargets.ColorMapSampler = rengine->getVideoDriver()->addRenderTargetTexture(dimension2du(RenderWidth, RenderHeight), "ColorMapSampler_VR", ECF_A8R8G8B8);
		HandlerRenderTargets.ScreenMapSampler = rengine->getVideoDriver()->addRenderTargetTexture(dimension2du(RenderWidth, RenderHeight), "ScreenMapSampler_VR", ECF_A8R8G8B8);

		// Create event receiver
		Receiver = new CVREventReceiver(rengine, VRSystem);

		// Enable Virtual Reality
		Handler->enableVirtualReality(true);
	}
	else {
		// Disable monitor and log error
		setEnabled(false);
		Rengine->getDevice()->getLogger()->log("Cannot get VR System: VR MOnitor has been disabled\n", ELL_ERROR);
	}
}

CCP3DVRMonitor::~CCP3DVRMonitor() {
	vr::VR_Shutdown();
	
	// Clear render targets
	LeftRenderTarget->drop();
	RightRenderTarget->drop();

	HandlerRenderTargets.drop();

	delete Receiver;
}

void CCP3DVRMonitor::render() {
	if (!VRSystem)
		return;

	/// Update poses
	vr::VRCompositor()->WaitGetPoses(TrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; nDevice++) {
		if (TrackedDevicePose[nDevice].bPoseIsValid) {
			Mat4DevicePose[nDevice] = Utils::getMatrix(TrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);

			ISceneNode *deviceNode = Receiver->getSceneNodes()[nDevice];
			if (!deviceNode)
				continue;

			deviceNode->setPosition(Mat4DevicePose[nDevice].getTranslation());
			deviceNode->setRotation(Mat4DevicePose[nDevice].getRotationDegrees());
			deviceNode->setScale(Mat4DevicePose[nDevice].getScale());
		}
	}

	if (TrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
		SystemPose = Mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		SystemPose.makeInverse();
	}

	vr::IVRCompositor *vrCompositor = vr::VRCompositor();

	ICameraSceneNode *camera = Rengine->getSceneManager()->getActiveCamera();
	camera->setNearValue(0.1f);
	
	/// Left eye
	Handler->getHDRManager()->setAutoUpdate(false);

	matrix4 finalLeftProjection = Utils::getMatrix(VRSystem->GetProjectionMatrix(vr::Eye_Left, camera->getNearValue(), camera->getFarValue()));
	matrix4 finalLeftView = Utils::getMatrix(VRSystem->GetEyeToHeadTransform(vr::Eye_Left));
	finalLeftView.makeInverse();

	camera->setProjectionMatrix(finalLeftProjection);
	camera->setViewMatrixAffector(finalLeftView * SystemPose);

	// Update and submit
	Handler->update(LeftRenderTarget, &HandlerRenderTargets);

	if (vrCompositor) {
		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)(LeftRenderTarget->getOpenGLTextureName()) , vr::TextureType_OpenGL, vr::ColorSpace_Linear };
		vrCompositor->Submit(vr::Eye_Left, &leftEyeTexture);
	}

	/// Right eye
	Handler->getHDRManager()->setAutoUpdate(true);

	matrix4 finalRightProjection = Utils::getMatrix(VRSystem->GetProjectionMatrix(vr::Eye_Right, camera->getNearValue(), camera->getFarValue()));
	matrix4 finalRightView = Utils::getMatrix(VRSystem->GetEyeToHeadTransform(vr::Eye_Right));
	finalRightView.makeInverse();

	camera->setProjectionMatrix(finalRightProjection);
	camera->setViewMatrixAffector(finalRightView * SystemPose);

	// Update and submit
	Handler->update(RightRenderTarget, &HandlerRenderTargets);

	if (vrCompositor) {
		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)(RightRenderTarget->getOpenGLTextureName()) , vr::TextureType_OpenGL, vr::ColorSpace_Linear };
		vrCompositor->Submit(vr::Eye_Right, &rightEyeTexture);
	}

	/// Clear stack and send events
	while (VRSystem->PollNextEvent(&Event, sizeof(Event)))
		Receiver->onEvent(Event);

	/// Finish
	Rengine->getVideoDriver()->setRenderTarget(0, true, true, SColor(0x0));
}

} /// End namespace engine
} /// End namespace cp3d
