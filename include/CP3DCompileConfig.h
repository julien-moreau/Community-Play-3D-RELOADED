/// CP3D Compile Config file
/// Extern methods + call methods

#ifndef __H_CP3D_COMPILE_CONFIG__
#define __H_CP3D_COMPILE_CONFIG__

#include <irrlicht.h>

#define CP3DR_LIB_EXPORTS
#ifndef _IRR_LINUX_PLATFORM_
    #ifdef CP3DR_LIB_EXPORTS
        #define CP3DR_LIB_API __declspec(dllexport)
    #else
        #define CP3DR_LIB_API __declspec(dllimport)
    #endif
#else
    #define CP3DR_LIB_API
#endif

namespace cp3d {

	namespace rendering {
		class ICP3DRenderingEngine;
	}

	#if defined(CP3DR_COMPILE_RENDERING_ENGINE)
	extern "C" CP3DR_LIB_API rendering::ICP3DRenderingEngine *createRenderingEngine(irr::IrrlichtDevice *device);
	#endif

	#if defined(CP3DR_COMPILE_ENGINE)
	extern "C" CP3DR_LIB_API void *createEngine();
	#endif

	#if defined(CP3DR_COMPILE_EDITOR)
	extern "C" CP3DR_LIB_API void *createEditor();
	#endif
} /// End namespace cp3d

#endif
