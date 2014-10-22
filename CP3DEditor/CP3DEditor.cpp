// Il s'agit du fichier DLL principal.

#include "stdafx.h"

#include <CP3DCompileConfig.h>
#include "Editor/Core/CCP3DEditorCore.h"

namespace cp3d {

	CP3DR_LIB_API ICP3DEditor *createEditor() {

		irr::SIrrlichtCreationParameters params;
		params.AntiAlias = true;
		params.Bits = 32;
		params.DriverMultithreaded = true;
		params.Fullscreen = false;
		params.LoggingLevel = irr::ELL_INFORMATION;
		params.WindowSize = irr::core::dimension2du(800, 600);
		params.WithAlphaChannel = true;
		params.DriverType = irr::video::EDT_DIRECT3D9;

		irr::IrrlichtDevice *device = irr::createDeviceEx(params);
		if (!device)
			exit(0);

		return new CCP3DEditorCore(device);
	}

}
