
#include "Stdafx.h"

#include <CP3DCompileConfig.h>
#include "Editor/Core/CCP3DEditorCore.h"

//#define CP3DR_HANDLE_SRGB
#define CP3DR_HIGH_PRECISION_FPU
#define CP3DR_DOUBLE_BUFFER

namespace cp3d {

	CP3DR_LIB_API ICP3DEditor *createEditor() {

		irr::SIrrlichtCreationParameters params;
		params.AntiAlias = true;
		params.Bits = 32;
		params.Fullscreen = false;
		params.LoggingLevel = irr::ELL_INFORMATION;
		params.WindowSize = irr::core::dimension2du(800, 600);
		params.WithAlphaChannel = false;
		params.DriverType = irr::video::EDT_DIRECT3D9;

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

		irr::IrrlichtDevice *device = irr::createDeviceEx(params);
		if (!device)
			exit(EXIT_FAILURE);

		return new CCP3DEditorCore(device);
	}

}
