
#include "stdafx.h"
#include "CCP3DCustomUpdater.h"

using namespace irr;

namespace cp3d {
namespace engine {

CCP3DCustomUpdater::CCP3DCustomUpdater() {

}

CCP3DCustomUpdater::~CCP3DCustomUpdater() {

}

void CCP3DCustomUpdater::OnPreUpdate() {
	#ifdef CP3DR_COMPILE_WITH_OPENMP
	#pragma omp for schedule(dynamic)
	#endif
	for (u32 i=0; i < Updates.size(); i++)
		Updates[i]->OnPreUpdate();
}

void CCP3DCustomUpdater::OnPostUpdate() {
	#ifdef CP3DR_COMPILE_WITH_OPENMP
	#pragma omp for schedule(dynamic)
	#endif
	for (u32 i=0; i < Updates.size(); i++)
		Updates[i]->OnPostUpdate();
}

bool CCP3DCustomUpdater::addCustomUpdate(ICP3DUpdate *update) {
	s32 index = Updates.binary_search(update);
	if (index != -1)
		return false;

	Updates.push_back(update);
	return true;
}

bool CCP3DCustomUpdater::addCustomUpdateToFront(ICP3DUpdate *update) {
	s32 index = Updates.binary_search(update);
	if (index != -1)
		return false;

	Updates.push_front(update);
	return true;
}

bool CCP3DCustomUpdater::removeCustomUpdate(ICP3DUpdate *update) {
	s32 index = Updates.binary_search(update);
	if (index != -1)
		Updates.erase(index);
	else
		return false;

	return true;
}

} /// End namespace engine
} /// End namespace cp3d
