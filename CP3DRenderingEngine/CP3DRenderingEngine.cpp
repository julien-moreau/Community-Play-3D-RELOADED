// Il s'agit du fichier DLL principal.

#include "stdafx.h"
#include "CP3DRenderingEngine.h"

#include "RenderingEngine/Handler/CP3DHandler.h"

using namespace irr;
using namespace video;

namespace cp3d {
namespace rendering {

CCP3DRenderingEngine::CCP3DRenderingEngine(IrrlichtDevice *device) {
	Handler = new CCP3DHandler(device, device->getVideoDriver()->getScreenSize(), false, true, true);
}

CCP3DRenderingEngine::~CCP3DRenderingEngine() {
	delete Handler;
}

IrrlichtDevice *CCP3DRenderingEngine::getDevice() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice();
}

IVideoDriver *CCP3DRenderingEngine::getVideoDriver() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice()->getVideoDriver();
}

} /// End namespace rendering

CP3DR_LIB_API rendering::ICP3DRenderingEngine *createRenderingEngine(IrrlichtDevice *device) {
	return new rendering::CCP3DRenderingEngine(device);
}

} /// End namespace cp3d
