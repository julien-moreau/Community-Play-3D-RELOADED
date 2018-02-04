// CP3DRenderingTests.cpp�: d�finit le point d'entr�e pour l'application console.

#include "stdafx.h"

#include <iostream>
#include <irrlicht.h>

#include "CGlobalTests.h"
#include "CDirect3D11Tests.h"
#include "CDirect3D9Debug.h"
#include "CHDRTests.h"
#include "CSkinnedMeshTest.h"
#include "CJetage.h"

/// Irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

/// Create an event receiver to exit the application when escape is pressed
class CEventReceiver : public irr::IEventReceiver {
public:

	CEventReceiver(irr::IrrlichtDevice *device) : device(device) { }

	bool OnEvent(const irr::SEvent &event) {
		if (event.EventType == EET_KEY_INPUT_EVENT) {
			if (event.KeyInput.Key == KEY_ESCAPE)
				device->closeDevice();

		}

		return false;
	}

private:
	irr::IrrlichtDevice *device;

};

/// Main function
int main(int argc, char* argv[]) {

	/// Create a device
	irr::video::E_DRIVER_TYPE driverType = EDT_OPENGL;
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	// Automatically test the D3D11 driver
	driverType = EDT_DIRECT3D11;
	#endif

	SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.WindowSize = dimension2du(1280, 800);
	params.Bits = 32;
	params.Fullscreen = false;
	params.Vsync = false;
	params.DriverMultithreaded = true;
	params.Doublebuffer = true;
	params.Stencilbuffer = false;
	params.AntiAlias = 32;
	params.ZBufferBits = 32;
	params.HighPrecisionFPU = true;
	IrrlichtDevice *device = createDeviceEx(params);
	device->setResizable(true);
	device->getLogger()->setLogLevel(ELL_INFORMATION);
    
    #ifdef _IRR_OSX_PLATFORM_
    device->getFileSystem()->changeWorkingDirectoryTo("/Users/julienmoreau-mathis/Desktop/Babylon/CP3D/CP3DR/");
    #endif

    printf(device->getFileSystem()->getWorkingDirectory().c_str());
    
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	if (driverType == EDT_DIRECT3D11)
		device->setWindowCaption(L"Rendering Test - DIRECT3D 11");
	else
	#endif
	if (driverType == EDT_DIRECT3D9)
		device->setWindowCaption(L"Rendering Test - DIRECT3D 9");
	else
		device->setWindowCaption(L"Rendering Test - OPENGL");

	device->setEventReceiver(new CEventReceiver(device));

	//cp3d::test::GlobalTest(device);
	//cp3d::test::Direct3D11Test(device);
	cp3d::test::Direct3D9Debug(device, new CEventReceiver(device));
	//cp3d::test::HDRTest(device, new CEventReceiver(device));
	//cp3d::test::SkinnedMeshDebug(device, new CEventReceiver(device));
	//cp3d::test::Jetage(device, new CEventReceiver(device));

	return EXIT_SUCCESS;
}
