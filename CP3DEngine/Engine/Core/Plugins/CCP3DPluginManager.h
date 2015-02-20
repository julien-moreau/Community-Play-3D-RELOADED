#ifndef __H_C_CP3D_PLUGIN_MANAGER_INCLUDED__
#define __H_C_CP3D_PLUGIN_MANAGER_INCLUDED__

#include <irrlicht.h>

#if defined(_IRR_WINDOWS_API_)
#include <Windows.h>
typedef HINSTANCE MonitorLibraryType;
#endif

namespace cp3d {
namespace engine {

class CCP3DPluginManager {
public:

	//! ctor & dtor
	CCP3DPluginManager();
	~CCP3DPluginManager();

	//! Statics
	static int freeLibrary(MonitorLibraryType lib);
	static MonitorLibraryType loadLibrary(irr::core::stringw path);

	template<class T>
	static T *invokeConstructor(MonitorLibraryType lib, irr::core::stringc name);

private:

};

} /// End namespace engine
} /// End namespace cp3d

#endif
