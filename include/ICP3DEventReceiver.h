#ifndef _H_ICP3D_EVENT_RECEIVER_INCLUDED__
#define _H_ICP3D_EVENT_RECEIVER_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

class ICP3DEventReceiver : public irr::IEventReceiver {
public:

	//! Adds an event receiver
	//! \param receiver: the instance of the event receiver
	/* Example :
	
	class MyClass : public irr::IEventReceiver {
	public:

		MyClass() { }
		~MyClass() { }

		bool OnEvent(const SEvent &event) {
			/// manage events here
		}

	};

	*/
	virtual bool addEventReceiver(irr::IEventReceiver *receiver) = 0;

	//! Removes an event receiver
	//! \param receiver: the receiver to remove
	virtual bool removeEventReceiver(irr::IEventReceiver *receiver) = 0;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
