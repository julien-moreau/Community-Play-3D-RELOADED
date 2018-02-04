
#include "Stdafx.h"

#include <CP3DCompileConfig.h>
#include "Editor/Core/CCP3DEditorCore.h"

//#define CP3DR_HANDLE_SRGB
#define CP3DR_HIGH_PRECISION_FPU
#define CP3DR_DOUBLE_BUFFER
#define CP3DR_DRIVER_MULTITHREADED

namespace cp3d {

	CP3DR_LIB_API ICP3DEditor *createEditor() {
		/// Get desktop size
		irr::IrrlichtDevice *device = irr::createDevice(irr::video::EDT_NULL);
		
		if (!device)
			return 0;

		irr::core::dimension2du desktopSize = device->getVideoModeList()->getDesktopResolution();
		device->closeDevice();

		/// Create device
		irr::SIrrlichtCreationParameters params;
		params.AntiAlias = false;
		params.Bits = 32;
		params.Fullscreen = false;
		params.LoggingLevel = irr::ELL_DEBUG;
		params.WindowSize = desktopSize;
		params.WithAlphaChannel = false;
		params.DriverType = irr::video::EDT_DIRECT3D9;
		params.EventReceiver = 0;

		#ifdef CP3DR_EDITOR_DIRECT3D11
		params.DriverType = irr::video::EDT_DIRECT3D11;
		#endif

		#ifdef CP3DR_HANDLE_SRGB
		params.HandleSRGB = true;
		#endif

		#ifdef CP3DR_HIGH_PRECISION_FPU
		params.HighPrecisionFPU = true;
		#endif

		#ifdef CP3DR_DRIVER_MULTITHREADED
		params.DriverMultithreaded = true;
		#endif

		#ifdef CP3DR_DOUBLE_BUFFER
		params.Doublebuffer = true;
		#else
		params.Doublebuffer = false;
		#endif

		device = irr::createDeviceEx(params);
		if (!device)
			exit(EXIT_FAILURE);

		/// Configure scene
		device->getSceneManager()->getParameters()->addBool(irr::scene::IRR_SCENE_MANAGER_IS_EDITOR, true);

		/// Return instance of editor core
		return new CCP3DEditorCore(device);
	}

}
