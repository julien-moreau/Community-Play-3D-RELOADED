#pragma once

#include <ICP3DRenderingEngine.h>
#include <ICP3DHandler.h>

#include <openvr.h>

#include "VR/CVREventReceiver.h"

namespace cp3d {
namespace engine {

class CCP3DVRMonitor : public ICP3DMonitor {

public:
	//! ctor & dtor
	CCP3DVRMonitor(rendering::ICP3DRenderingEngine *rengine);
	~CCP3DVRMonitor();

	void render();

private:
	// Datas
	rendering::ICP3DHandler *Handler;
	CVREventReceiver *Receiver;

	// VR
	vr::IVRSystem *VRSystem;
	vr::TrackedDevicePose_t TrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	irr::core::matrix4 Mat4DevicePose[vr::k_unMaxTrackedDeviceCount];

	irr::u32 RenderWidth;
	irr::u32 RenderHeight;

	irr::video::ITexture *LeftRenderTarget;
	irr::video::ITexture *RightRenderTarget;

	rendering::SHandlerRenderTargets HandlerRenderTargets;
};

} /// End namespace engine
} /// End namespace cp3d
