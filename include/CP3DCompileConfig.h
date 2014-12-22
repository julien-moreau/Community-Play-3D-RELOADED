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

	namespace data {
		class ICP3DCollaborativeEngine;
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

	#if defined(CP3DR_COMPILE_COLLABORATIVE)
	extern "C" CP3DR_LIB_API data::ICP3DCollaborativeEngine *createCollaborativeEngine(const char *documentPath);
	#endif

	//! Compiles the CP3D Engine with OpenMP support
	//! Uncomment this line to enable OpenMP support
	#define CP3DR_COMPILE_WITH_OPENMP

	//! If uncommented, enable proper sRGB and linear color handling
	//! Uncomment this line to enable
	//#define CP3DR_HANDLE_SRGB

	//! If uncommented, specifies if the device should use high precision FPU setting
	//! This is only relevant for DirectX Devices, which switch to low FPU precision by default for performance reasons
	//#define CP3DR_HIGH_PRECISION_FPU

	//! If uncommented, CP3D will used the multithreaded driver
	//! So far only on DirectX
	#define CP3DR_DRIVER_MULTITHREADED

	//! This should be usually enabled, in order to avoid render
	//! artifacts on the visible framebuffer. However, it might be
	//! useful to use only one buffer on very small devices. If no
	//! doublebuffering is available, the drivers will fall back to
	//! single buffers
	//#define CP3DR_DOUBLE_BUFFER

	//! Runs the editor using the experimental Direct3D 11 driver
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	#define CP3DR_EDITOR_DIRECT3D11
	#endif

	//! Compiles the CP3DR Engine with the Microsoft Kinect V2 support
	//! Allow you to access the Microsoft Kinect V2 related tools to develop
	//! your Microsoft Kinect V2 gameplays, or direct use in CP3DR Editor.
	//! Comment the following line if you want to disable the Microsoft Kinect V2 supprot
	#define CP3DR_KINECT_SUPPORT
	#if defined(CP3DR_KINECT_SUPPORT) && defined(CP3DR_COMPILE_ENGINE) && !defined(CP3DR_COMPILE_EDITOR)
	#pragma comment(lib, "Kinect20.lib")
	#endif

} /// End namespace cp3d

#endif
