#ifndef _H_ICP3D_PLUGIN_MANAGER_INCLUDED__
#define _H_ICP3D_PLUGIN_MANAGER_INCLUDED__

#include <irrlicht.h>

#if defined(_IRR_WINDOWS_API_)
#include <Windows.h>
typedef HINSTANCE PluginLibraryType;
#else
typedef void* PluginLibraryType;
#endif

#include <utility>

namespace cp3d {
namespace engine {

class ICP3DPluginManager {
public:

	//! Static
	//! Free the already loaded dynamic library
	//! \param lib: the library instance
	//! \return: returns if the library was free
	static int freeLibrary(PluginLibraryType lib) {
		#if defined(_IRR_WINDOWS_API_)
		return FreeLibrary(lib);
		#else
		return dlclose(lib);
		#endif
	}

	//! Static
	//! Loads a dynamic library
	//! \param path: the path of the library on your file system
	//! \return: returns the library instance
	static PluginLibraryType loadLibrary(irr::core::stringw path) {
		#if defined(_IRR_WINDOWS_API_)
		PluginLibraryType hdll = LoadLibrary(path.c_str());
		#else
		PluginLibraryType hdll = dlopen(stringc(path).c_str(), RTLD_LAZY);
		#endif

		return hdll;
	}

	//! Static
	//! Calls a function handled into the given dynamic library
	/*
	Example :
		PluginLibraryType hdll = ICP3DPluginManager::loadLibrary("defaultMonitor.dll");
		ICP3DMonitor *monitor = ICP3DPluginManager::callFunction<ICP3DMonitor *, ICP3DRenderingEngine *>(hdll, "createMonitor", renderingEngine);
	*/
	//! \param lib: the library instance (see loadLibrary)
	//! \param name: the function name
	//! \param params: the parameters used during the call
	//! \param 
	template<typename T, typename... Args>
	static T callFunction(PluginLibraryType lib, irr::core::stringc name, Args... params) {
		typedef T (*functionDef)(Args...);
		functionDef fd;

		#if defined(_IRR_WINDOWS_API_)
		fd = reinterpret_cast<functionDef>(GetProcAddress(lib, name.c_str()));
		#else
		fd = reinterpret_cast <functionDef> (dlsym(hdll, name.c_str()));
		#endif

		if (fd) {
			T value = static_cast<T>(fd(params...));
			return value;
		}

		return 0;
	}

};

} /// End engine namespace
} /// End cp3d namespace

#endif