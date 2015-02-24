#ifndef _H_ICP3D_PLUGIN_MANAGER_INCLUDED__
#define _H_ICP3D_PLUGIN_MANAGER_INCLUDED__

#include <irrlicht.h>

#if defined(_IRR_WINDOWS_API_)
#include <Windows.h>
typedef HINSTANCE PluginLibraryType;
#else
typedef void* PluginLibraryType;
#endif

//! Structure that handle the process informations
//! returns by the startProcess static method
struct PluginProcessType {
	bool started;

	#if defined(_IRR_WINDOWS_API_)
	PROCESS_INFORMATION processInformation;
	#endif
};

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
	template<typename T, typename... Args>
	static T callFunction(PluginLibraryType lib, irr::core::stringc name, Args... params) {
		typedef T (*functionDef)(Args...);
		functionDef fd;

		#if defined(_IRR_WINDOWS_API_)
		fd = reinterpret_cast<functionDef>(GetProcAddress(lib, name.c_str()));
		#else
		fd = reinterpret_cast<functionDef>(dlsym(hdll, name.c_str()));
		#endif

		if (fd) {
			T value = static_cast<T>(fd(params...));
			return value;
		}

		return 0;
	}

	//! Closes a given process
	//! \param process: the process structure returned by startProcess()
	static int closeProcess(PluginProcessType process) {
		if (!process.started)
			return 0;

		#if defined(_IRR_WINDOWS_API_)
		int hProcessResult = CloseHandle(process.processInformation.hProcess);
		int hThreadresult = CloseHandle(process.processInformation.hThread);

		return !(hProcessResult == 0 || hThreadresult == 0);
		#endif
	}

	//! Starts a new process
	//! \param path: the path to the process (.exe, etc.)
	//! \param wait: if the current application should wait until the new process has finished
	//! \param arguments: the arguments to send to the new process
	/*
	Example:
		var process = ICP3DPluginManager::startProcess("app.exe", true, "game.map 0 text.txt" );
	*/
	static PluginProcessType startProcess(const irr::core::stringw path, const bool wait, irr::core::stringw arguments) {
		// Begin process...
		PluginProcessType process;

		#if defined(_IRR_WINDOWS_API_)

		STARTUPINFO startUpInfo;
		ZeroMemory(&startUpInfo, sizeof(startUpInfo));
		startUpInfo.cb = sizeof(startUpInfo);
		ZeroMemory(&process.processInformation, sizeof(process.processInformation));

		DWORD flags = NULL;
		#ifdef _DEBUG
		flags = DEBUG_PROCESS;
		#endif

		int result = CreateProcess(
			path.c_str(), // Module name: path to the .exe
			(wchar_t *)arguments.c_str(), // Arguments
			NULL, // Process handle inheritable
			NULL, // Thread handle inheritable
			NULL, // Handle inheritance
			flags, // Creation flags
			NULL, // Parent's starting block 
			NULL, // Parent's starting directory 
			&startUpInfo,
			&process.processInformation
		);

		if (!result)
			process.started = false;
		else {
			process.started = true;
			if (wait) {
				WaitForSingleObject(process.processInformation.hProcess, INFINITE);
				closeProcess(process);
			}
		}

		#endif

		return process;
	}

};

} /// End engine namespace
} /// End cp3d namespace

#endif