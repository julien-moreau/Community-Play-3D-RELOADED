/// CP3D Compile Config file
/// Extern methods + call methods

#ifndef __H_CP3D_COMPILE_CONFIG__
#define __H_CP3D_COMPILE_CONFIG__

namespace irr {
	class IrrlichtDevice;
}

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

	class ICP3DEditor;

	namespace rendering {
		class ICP3DRenderingEngine;
	}

	namespace engine {
		class ICP3DEngine;
	}

	#if defined(CP3DR_COMPILE_RENDERING_ENGINE)
	//! Creates a new rendering engine
	//! \param device: the irrlicht device used by the rendering engine
	extern "C" CP3DR_LIB_API rendering::ICP3DRenderingEngine *createRenderingEngine(irr::IrrlichtDevice *device);
	#endif

	#if defined(CP3DR_COMPILE_ENGINE)
	//! Creates a new CP3D engine
	//! \param device: the device used by the engine
	extern "C" CP3DR_LIB_API engine::ICP3DEngine *createEngine(irr::IrrlichtDevice *device);
	#endif

	#if defined(CP3DR_COMPILE_EDITOR)
	//! Creates a new editor. Can be used by external softwares
	extern "C" CP3DR_LIB_API ICP3DEditor *createEditor();
	#endif
} /// End namespace cp3d

#endif
