#ifndef _H_ICP3D_ENGINE_INCLUDED__
#define _H_ICP3D_ENGINE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

	namespace rendering {
		class ICP3DRenderingEngine;
	}

namespace engine {

class ICP3DEventReceiver;
class ICP3DCustomUpdater;
class ICP3DSceneNodeCreator;

class ICP3DEngine {
public:

	//! Returns the event receiver
	//! Then you can add / remove event receivers using
	//! addEventReceiver(receiver) or removeEventReceiver(receiver)
	virtual ICP3DEventReceiver *getEventReceiver() = 0;
	
	//! Returns the custom updater
	//! Then you can add / remove custom updates using
	//! addCustomUpdate(update) or removeCustomUpdate(update)
	virtual ICP3DCustomUpdater *getCustomUpdater() = 0;

	//! Returns the scene node creator
	//! Let you able to create scene nodes using
	//! Engine->getSceneNodeCreator()->create[...]();
	virtual ICP3DSceneNodeCreator *getSceneNodeCreator() = 0;

	//! Runs the engine.
	//! Should be called each frame in your programs
	//! runEngine() will run your custom updates and
	//! will run other processes you called
	virtual void runEngine() = 0;

	//! Returns the rendering engine
	virtual rendering::ICP3DRenderingEngine *getRenderingEngine() = 0;

};

} /// End engine namespace
} /// End cp3d namespace

#endif