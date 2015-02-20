#include "stdafx.h"
#include "CCP3DPluginManager.h"

using namespace irr;
using namespace core;

namespace cp3d {
namespace engine {

CCP3DPluginManager::CCP3DPluginManager()
{

}

CCP3DPluginManager::~CCP3DPluginManager() {

}

// Statics
int CCP3DPluginManager::freeLibrary(MonitorLibraryType lib) {
	#if defined(_IRR_WINDOWS_API_)
	return FreeLibrary(lib);
	#endif
}

MonitorLibraryType CCP3DPluginManager::loadLibrary(irr::core::stringw path) {
	#if defined(_IRR_WINDOWS_API_)
		MonitorLibraryType hdll = LoadLibrary(path.c_str());
	#endif

	return hdll;
}

template<class T>
T *invokeConstructor(MonitorLibraryType lib, stringc name) {
	return NULL;
}

// Methods

} /// End namespace engine
} /// End namespace cp3d
