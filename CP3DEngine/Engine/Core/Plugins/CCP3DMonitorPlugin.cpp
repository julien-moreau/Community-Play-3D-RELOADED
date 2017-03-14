#include "stdafx.h"
#include "CCP3DMonitorPlugin.h"

using namespace irr;
using namespace core;

namespace cp3d {
namespace engine {

CCP3DMonitorPlugin::CCP3DMonitorPlugin(rendering::ICP3DRenderingEngine *rengine) : Rengine(rengine)
{

}

CCP3DMonitorPlugin::~CCP3DMonitorPlugin() {
	while (Monitors.size() != 0)
		removeMonitor(Monitors[0].Monitor->getName());
}

void CCP3DMonitorPlugin::render() {
	for (u32 i = 0; i < Monitors.size(); i++) {
		if (Monitors[i].Monitor->isEnabled())
			Monitors[i].Monitor->render();
	}
}

bool CCP3DMonitorPlugin::addMonitor(stringc path) {
    #ifdef _IRR_OSX_PLATFORM_
    path += ".dylib";
    #else
    path += ".dll";
    #endif
    
	PluginLibraryType hdll = ICP3DPluginManager::loadLibrary(stringw(path));
	if (!hdll)
		return false;

	ICP3DMonitor *monitor = ICP3DPluginManager::callFunction<ICP3DMonitor *, rendering::ICP3DRenderingEngine *>(hdll, "createMonitor", Rengine);

	if (monitor) {
		Monitors.push_back({ hdll, monitor });
		return true;
	}

	return false;
}

bool CCP3DMonitorPlugin::removeMonitor(stringc name) {
	for (u32 i = 0; i < Monitors.size(); i++) {
		if (Monitors[i].Monitor->getName() == name) {
			delete Monitors[i].Monitor;
			ICP3DPluginManager::freeLibrary(Monitors[i].Hdll);
			Monitors.erase(i);
			return true;
		}
	}

	return false;
}

} /// End namespace engine
} /// End namespace cp3d
