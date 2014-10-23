
#ifndef __H_C_CP3D_EVENT_RECEIVER_INCLUDED__
#define __H_C_CP3D_EVENT_RECEIVER_INCLUDED__

#include <ICP3DEventReceiver.h>

namespace cp3d {
namespace engine {

/*
Class that handle events in your application.
Once an event happen, the job of the CCP3DEventReceiver is to dispatch
the event to all the event receivers

To receive events, just create a class that heritates from irr::IEventReceiver
and call Engine->getEventReceiver()->addEventReceiver(this);

Example :

class CMyClass : public irr::IEventReceiver {
public:

	CMyClass(ICP3DEngine *engine) : Engine(engine) {
		Engine->getEventReceiver()->addEventReceiver(this);
	}

	~CMyClass() {
		Engine->getEventReceiver()->removeEventReceiver(this);
	}

	bool OnEvent(const SEvent &event) {
		///
			Manage events here
		///
		return false;
	}

private:
	ICP3DEngine *Engine;

};

*/
class CCP3DEventReceiver : public ICP3DEventReceiver {
public:

	CCP3DEventReceiver();
	~CCP3DEventReceiver();

	bool OnEvent(const irr::SEvent &event);

	bool addEventReceiver(irr::IEventReceiver *receiver);
	bool removeEventReceiver(irr::IEventReceiver *receiver);

private:
	irr::core::array<irr::IEventReceiver *> Receivers;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
