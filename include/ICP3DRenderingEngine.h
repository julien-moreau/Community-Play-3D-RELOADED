#ifndef _H_ICP3D_RENDERING_ENGINE_INCLUDED__
#define _H_ICP3D_RENDERING_ENGINE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

	class ICP3DHandler;

	class ICP3DRenderingEngine {
	public:

		//! Returns the Irrlicht device
		virtual irr::IrrlichtDevice *getDevice() = 0;

		//! Returns the video driver
		virtual irr::video::IVideoDriver *getVideoDriver() = 0;

		//! Returns the main handler
		virtual ICP3DHandler *getHandler() = 0;

	};

} /// End rendering namespace
} /// End cp3d namespace

#endif