// CP3DRenderingTests.cpp : définit le point d'entrée pour l'application console.

#include "stdafx.h"

#include <iostream>
#include <irrlicht.h>

#include "CGlobalTests.h"
#include "CDirect3D11Tests.h"
#include "CDirect3D9Debug.h"
#include "CHDRTests.h"

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
	irr::video::E_DRIVER_TYPE driverType = EDT_DIRECT3D9;
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	// Automatically test the D3D11 driver
	driverType = EDT_DIRECT3D11;
	#endif

	//IrrlichtDevice *device = createDevice(driverType, dimension2du(1280, 800), 32, false, false, false, 0);
	SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.WindowSize = dimension2du(1280, 800);
	params.Bits = 32;
	params.Fullscreen = false;
	params.DriverMultithreaded = true;
	params.Doublebuffer = true;
	IrrlichtDevice *device = createDeviceEx(params);
	device->getLogger()->setLogLevel(ELL_INFORMATION);

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
	//cp3d::test::Direct3D9Debug(device);
	cp3d::test::HDRTest(device);

	/*
	IVideoDriver *driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();

	ISceneNode *cube = smgr->addCubeSceneNode(1, 0, -1, vector3df(0.f), vector3df(0.f), vector3df(1.f));
	cube->setMaterialFlag(EMF_LIGHTING, false);

	ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(10.f, 10.f, 10.f), vector3df(0.f), -1, true);

	while (device->run()) {
		driver->beginScene(true, true, SColor(0x0));
		smgr->drawAll();
		driver->endScene();
	}
	*/

	return EXIT_SUCCESS;
}
