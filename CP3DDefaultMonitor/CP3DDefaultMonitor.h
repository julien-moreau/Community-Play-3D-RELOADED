#pragma once

#include <ICP3DRenderingEngine.h>
#include <ICP3DHandler.h>

namespace cp3d {
namespace engine {

class CCP3DDefaultMonitor : public ICP3DMonitor {

public:
	//! ctor & dtor
	CCP3DDefaultMonitor(rendering::ICP3DRenderingEngine *rengine);
	~CCP3DDefaultMonitor();

	void render();

private:
	// Datas
	rendering::ICP3DHandler *Handler;

};

} /// End namespace engine
} /// End namespace cp3d
