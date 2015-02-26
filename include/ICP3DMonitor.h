#ifndef _H_ICP3D_MONITOR_INCLUDED__
#define _H_ICP3D_MONITOR_INCLUDED__

#include <irrlicht.h>
#include "CP3DCompileConfig.h"
#include "ICP3DPluginManager.h"
#include "ICP3DRenderingEngine.h"

namespace cp3d {
namespace engine {

class ICP3DMonitor {
public:

	//! Constructor
	ICP3DMonitor(rendering::ICP3DRenderingEngine *rengine) : Rengine(rengine), Enabled(true), Name("New Monitor")
	{ }

	//! Returns if the monitor is enabled
	bool isEnabled() {
		return Enabled;
	}

	//! Sets if the monitor is enabled or not
	//! \param enabled: true to enable, false do disable
	void setEnabled(bool enabled) {
		Enabled = enabled;
	}

	//! Returns the monitor's name
	virtual irr::core::stringc getName() {
		return Name;
	}

	//! Renders the scene
	virtual void render() = 0;

protected:
	// Rendering
	rendering::ICP3DRenderingEngine *Rengine;

	// Datas
	irr::core::stringc Name;
	bool Enabled;

};

extern "C" CP3DR_LIB_API ICP3DMonitor *createMonitor(rendering::ICP3DRenderingEngine *rengine);

#define REGISTER_MONITOR(ClassName) \
	ICP3DMonitor *createMonitor(rendering::ICP3DRenderingEngine *rengine) { \
		return new ClassName(rengine); \
	} \

} /// End namespace engine
} /// End namespace cp3d

#endif
