#ifndef _H_CP3D_ENGINE_INCLUDED__
#define _H_CP3D_ENGINE_INCLUDED__

#include "ICP3DEngine.h"
#include "ICP3DEventReceiver.h"
#include "ICP3DCustomUpdate.h"
#include "ICP3DSceneNodeCreator.h"
#include "ICP3DGeometryCreator.h"

#include "ICP3DPluginManager.h"
#include "ICP3DMonitor.h"
#include "ICP3DPlugin.h"

// Default monitor
#if defined(_DEBUG)
#define DEFAULT_MONITOR_PATH "CP3DDefaultMonitor_d.dll"
#else
#define DEFAULT_MONITOR_PATH "CP3DDefaultMonitor.dll"
#endif

#endif
