#include "stdafx.h"
#include "CP3DDefaultMonitor.h"

#include <ICP3DPluginManager.h>

using namespace irr;
using namespace core;

namespace cp3d {
namespace engine {

REGISTER_MONITOR(CCP3DDefaultMonitor)
CCP3DDefaultMonitor::CCP3DDefaultMonitor(rendering::ICP3DRenderingEngine *rengine)
	: ICP3DMonitor(rengine)
{
	Handler = rengine->getHandler();
	Name = stringc("Default Monitor");
}

CCP3DDefaultMonitor::~CCP3DDefaultMonitor() {

}

void CCP3DDefaultMonitor::render() {
	Handler->update();
}

} /// End namespace engine
} /// End namespace cp3d
