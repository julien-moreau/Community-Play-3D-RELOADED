#ifndef _H_ICP3D_PLUGIN_INCLUDED__
#define _H_ICP3D_PLUGIN_INCLUDED__

#include <irrlicht.h>
#include "CP3DCompileConfig.h"
#include "ICP3DEngine.h"

namespace cp3d {
namespace engine {

class ICP3DPlugin {
public:

	//! Constructor
	ICP3DPlugin(ICP3DEngine *engine) : Engine(engine), Name("New Plugin")
	{ }

protected:
	// Rendering
	ICP3DEngine *Engine;

	// Datas
	irr::core::stringc Name;
	bool Enabled;

};

extern "C" CP3DR_LIB_API ICP3DPlugin *createPlugin(ICP3DEngine *engine);

} /// End namespace engine
} /// End namespace cp3d

#endif
