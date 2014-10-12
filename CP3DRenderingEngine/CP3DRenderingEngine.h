// CP3DRenderingEngine.h

#pragma once

#include <CP3DCompileConfig.h>
#include <ICP3DRenderingEngine.h>

namespace cp3d {
namespace rendering {

class CCP3DRenderingEngine : public ICP3DRenderingEngine {

public:

	CCP3DRenderingEngine(irr::IrrlichtDevice *device);
	~CCP3DRenderingEngine();

	/// Getters
	irr::IrrlichtDevice *getDevice();

	irr::video::IVideoDriver *getVideoDriver();

	ICP3DHandler *getHandler() { return Handler; }

private:

	ICP3DHandler *Handler;

};

} /// End namespace rendering
} /// End namespace cp3d