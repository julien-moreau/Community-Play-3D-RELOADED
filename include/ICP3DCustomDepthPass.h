#ifndef _H_ICP3D_CUSTOM_DEPTH_PASS_INCLUDED__
#define _H_ICP3D_CUSTOM_DEPTH_PASS_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomPass.h>

namespace cp3d {
namespace rendering {

	class ICP3DCustomDepthPass : public ICP3DCustomPass {
	public:

		//! Constructor
		//! \param driver: video driver (OpenGL or Direct3D9)
		//! \param name: the name of the custom pass
		ICP3DCustomDepthPass(irr::video::IVideoDriver *driver, irr::core::stringc name)
			: ICP3DCustomPass(driver, name) { }

		//! add a pass to the depth pass manager
		//! \param name: the name of the render target to add
		virtual void addPass(irr::core::stringc name) = 0;

		//! Sets depth of the pass named "name"
		//! \param name: the name of the pass
		//! \param farLink: the depth to set
		virtual void setDepth(irr::core::stringc name, irr::f32 farLink) = 0;

	};

} /// End rendering namespace
} /// End cp3d namespace

#endif