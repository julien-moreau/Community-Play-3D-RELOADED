#ifndef __H_C_CP3D_MONITOR_PLUGIN_INCLUDED__
#define __H_C_CP3D_MONITOR_PLUGIN_INCLUDED__

#include <ICP3DMonitor.h>

#if defined(_IRR_WINDOWS_API_)
#include <Windows.h>
typedef HINSTANCE MonitorLibraryType;
#endif

namespace cp3d {
namespace engine {

struct SCP3DMonitorPlugin {
	MonitorLibraryType Hdll;
	ICP3DMonitor *Monitor;
};

class CCP3DMonitorPlugin {
public:

	//! ctor & dtor
	CCP3DMonitorPlugin(rendering::ICP3DRenderingEngine *rengine);
	~CCP3DMonitorPlugin();

	void render();

	bool addMonitor(irr::core::stringc path);
	bool removeMonitor(irr::core::stringc name);

	irr::u32 getMonitorCount() {
		return Monitors.size();
	}

private:
	// Methods
	int freeLibrary(MonitorLibraryType lib);
	MonitorLibraryType loadLibrary(irr::core::stringw path);
	ICP3DMonitor *invokeCreateMonitor(MonitorLibraryType lib, irr::core::stringc name);

	// Rendering
	rendering::ICP3DRenderingEngine *Rengine;

	// Datas
	irr::core::array<SCP3DMonitorPlugin> Monitors;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
