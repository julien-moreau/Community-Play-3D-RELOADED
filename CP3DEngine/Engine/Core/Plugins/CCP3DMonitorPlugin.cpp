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
	MonitorLibraryType hdll = loadLibrary(stringw(path));
	if (!hdll)
		return false;

	ICP3DMonitor *monitor = invokeCreateMonitor(hdll, "createMonitor");
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
			freeLibrary(Monitors[i].Hdll);
			Monitors.erase(i);
			return true;
		}
	}

	return false;
}

ICP3DMonitor *CCP3DMonitorPlugin::invokeCreateMonitor(MonitorLibraryType lib, stringc name) {
	typedef ICP3DMonitor* (*createMonitorFunc)(rendering::ICP3DRenderingEngine *rengine);
	createMonitorFunc cm;
	
	#if defined(_IRR_WINDOWS_API_)
	cm = reinterpret_cast<createMonitorFunc>(GetProcAddress(lib, name.c_str()));
	#endif

	if (cm) {
		ICP3DMonitor *monitor = static_cast<ICP3DMonitor *>(cm(Rengine));
		return monitor;
	}
	
	return 0;
}

MonitorLibraryType CCP3DMonitorPlugin::loadLibrary(stringw path) {
	#if defined(_IRR_WINDOWS_API_)
	MonitorLibraryType hdll = LoadLibrary(path.c_str());
	#endif

	return hdll;
}

int CCP3DMonitorPlugin::freeLibrary(MonitorLibraryType lib) {
	#if defined(_IRR_WINDOWS_API_)
	return FreeLibrary(lib);
	#endif
}

} /// End namespace engine
} /// End namespace cp3d
