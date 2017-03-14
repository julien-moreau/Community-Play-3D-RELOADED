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

	// VR
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
	}
}

CCP3DVRMonitor::~CCP3DVRMonitor() {

}

void CCP3DVRMonitor::render() {
	if (!VRSystem)
		return;

	/// Update poses
	matrix4 systemPose;
	vr::VRCompositor()->WaitGetPoses(TrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; nDevice++) {
		if (TrackedDevicePose[nDevice].bPoseIsValid) {
			Mat4DevicePose[nDevice] = Utils::getMatrix(TrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);

			ISceneNode *deviceNode = Receiver->getSceneNodes()[nDevice];
			if (deviceNode) {
				deviceNode->setPosition(Mat4DevicePose[nDevice].getTranslation());
				deviceNode->setRotation(Mat4DevicePose[nDevice].getRotationDegrees());
				deviceNode->setScale(Mat4DevicePose[nDevice].getScale());
			}
		}
	}

	if (TrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
		systemPose = Mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		systemPose.makeInverse();
	}

	vr::IVRCompositor *vrCompositor = vr::VRCompositor();

	ICameraSceneNode *camera = Rengine->getSceneManager()->getActiveCamera();
	camera->setNearValue(0.1f);
	
	/// Left eye
	Handler->getHDRManager()->setAutoUpdate(false);

	vr::HmdMatrix44_t hmdLeftProjection = VRSystem->GetProjectionMatrix(vr::Eye_Left, camera->getNearValue(), camera->getFarValue());
	vr::HmdMatrix34_t hmdLeftView = VRSystem->GetEyeToHeadTransform(vr::Eye_Left);

	matrix4 finalLeftProjection = Utils::getMatrix(hmdLeftProjection);

	matrix4 finalLeftView = Utils::getMatrix(hmdLeftView);
	finalLeftView.makeInverse();

	camera->setProjectionMatrix(finalLeftProjection);
	camera->setViewMatrixAffector(finalLeftView * systemPose);

	// Update and submit
	Handler->update(LeftRenderTarget, &HandlerRenderTargets);

	if (vrCompositor) {
		u32 textureId = LeftRenderTarget->getOpenGLTextureName();

		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)(textureId) , vr::TextureType_OpenGL, vr::ColorSpace_Linear };
		vrCompositor->Submit(vr::Eye_Left, &leftEyeTexture);
	}

	/// Right eye
	Handler->getHDRManager()->setAutoUpdate(true);

	vr::HmdMatrix44_t hmdRightProjection = VRSystem->GetProjectionMatrix(vr::Eye_Right, camera->getNearValue(), camera->getFarValue());
	vr::HmdMatrix34_t hmdRightView = VRSystem->GetEyeToHeadTransform(vr::Eye_Right);

	matrix4 finalRightProjection = Utils::getMatrix(hmdRightProjection);

	matrix4 finalRightView = Utils::getMatrix(hmdRightView);
	finalRightView.makeInverse();

	camera->setProjectionMatrix(finalRightProjection);
	camera->setViewMatrixAffector(finalRightView * systemPose);

	// Update and submit
	Handler->update(RightRenderTarget, &HandlerRenderTargets);

	if (vrCompositor) {
		u32 textureId = RightRenderTarget->getOpenGLTextureName();

		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)(textureId) , vr::TextureType_OpenGL, vr::ColorSpace_Linear };
		vrCompositor->Submit(vr::Eye_Right, &rightEyeTexture);
	}

	// Clear event stack
	vr::VREvent_t event;
	while (VRSystem->PollNextEvent(&event, sizeof(event)))
		Receiver->onEvent(event);

	// Finish
	Rengine->getVideoDriver()->setRenderTarget(0, true, true, SColor(0x0));
}

} /// End namespace engine
} /// End namespace cp3d
