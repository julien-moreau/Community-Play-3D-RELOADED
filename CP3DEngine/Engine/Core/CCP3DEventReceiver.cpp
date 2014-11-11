
#include "stdafx.h"
#include "CCP3DEventReceiver.h"

using namespace irr;

namespace cp3d {
namespace engine {

CCP3DEventReceiver::CCP3DEventReceiver() {

}

CCP3DEventReceiver::~CCP3DEventReceiver() {

}

bool CCP3DEventReceiver::OnEvent(const irr::SEvent &event) {
	#ifdef CP3DR_COMPILE_WITH_OPENMP
	#pragma omp for schedule(dynamic)
	#endif
	for (u32 i=0; i < Receivers.size(); i++)
		if (Receivers[i]->OnEvent(event))
			return true;

	return false;
}

bool CCP3DEventReceiver::addEventReceiver(irr::IEventReceiver *receiver) {
	s32 index = Receivers.binary_search(receiver);
	if (index != -1)
		return false;

	Receivers.push_back(receiver);
	return true;
}

bool CCP3DEventReceiver::removeEventReceiver(irr::IEventReceiver *receiver) {
	s32 index = Receivers.binary_search(receiver);
	if (index != -1)
		Receivers.erase(index);
	else
		return false;

	return true;
}

} /// End namespace engine
} /// End namespace cp3d
