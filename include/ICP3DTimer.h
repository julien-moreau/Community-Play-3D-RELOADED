#ifndef _H_ICP3D_TIMER_INCLUDED__
#define _H_ICP3D_TIMER_INCLUDED__

#include <irrlicht.h>
#include <functional>

#include "ICP3DEngine.h"
#include "ICP3DRenderingEngine.h"
#include "ICP3DCustomUpdate.h"

namespace cp3d {
namespace engine {

class ICP3DTimer : public ICP3DUpdate {
public:
	//! Creates a new timer
	static ICP3DTimer *create(ICP3DEngine *engine) {
		return new ICP3DTimer(engine);
	}

	//! Runs the timer until the given time (ms)
	//! \param end: the end time
	void runUntil(const irr::u32 &end, const std::function<void()> &callback) {
		StartTime = Timer->getTime();
		End = end;
		Callback = callback;
	}

	//! On pre update (ICP3DUpdate)
	void OnPreUpdate() {
		if (End == -1)
			return;

		const irr::u32 currentTime = Timer->getTime() - StartTime;
		Updater(currentTime);

		if (currentTime >= End) {
			Callback();
		}
	}

	//! Registers the update
	void registerUpdater(const std::function<void(const irr::u32 &time)> &updater) {
		Updater = updater;
	}

	//! Unregisters the updater
	void unregisterUpdater() {
		Updater = [](const irr::u32 &time) { };
	}

	void stop() {
		End = -1;
	}

	//! Desctructor
	~ICP3DTimer() {
		Engine->getCustomUpdater()->removeCustomUpdate(this);
	}

private:
	//! Constructor
	ICP3DTimer(ICP3DEngine *engine) : Engine(engine), StartTime(-1), End(-1) {
		// Add updater
		engine->getCustomUpdater()->addCustomUpdate(this);

		// Get timer
		Timer = engine->getRenderingEngine()->getDevice()->getTimer();

		// Configure
		Callback = []() { };
		unregisterUpdater();
	}

protected:
	// Irrlicht & engine
	ICP3DEngine *Engine;
	irr::ITimer *Timer;

	// Timer
	irr::u32 StartTime, End;

	// Callback
	std::function<void()> Callback;
	std::function<void(const irr::u32 &time)> Updater;
};

}
}

#endif
