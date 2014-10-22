
#ifndef __H_C_CP3D_EVENT_RECEIVER_INCLUDED__
#define __H_C_CP3D_EVENT_RECEIVER_INCLUDED__

#include <ICP3DEventReceiver.h>

namespace cp3d {
namespace engine {

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
