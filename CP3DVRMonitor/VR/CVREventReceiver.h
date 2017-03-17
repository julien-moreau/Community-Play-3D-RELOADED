#ifndef __H_VR_EVENT_RECEIVER_INCLUDED__
#define __H_VR_EVENT_RECEIVER_INCLUDED__

#include <CP3DRenderingEngine.h>
#include <openvr.h>

#include <stdio.h>
#include <cstdlib>

namespace cp3d {
namespace engine {

class CVREventReceiver {
public:
	CVREventReceiver(rendering::ICP3DRenderingEngine *rengine, vr::IVRSystem *vrSystem);
	~CVREventReceiver();

	void onEvent(const vr::VREvent_t &event);

	irr::scene::ISceneNode **getSceneNodes() { return TrackedDeviceNodes; }

private:
	/// Engine
	rendering::ICP3DRenderingEngine *Rengine;
	vr::IVRSystem *VRSystem;

	/// Scene
	irr::scene::ISceneNode *TrackedDeviceNodes[vr::k_unMaxTrackedDeviceCount];
	irr::video::ITexture *TrackedDeviceTextures[vr::k_unMaxTrackedDeviceCount];

	/// Methods
	void setupDeviceModel(const vr::TrackedDeviceIndex_t &index);
};

}
}

#endif
