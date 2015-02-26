
#include "stdafx.h"
#include <ICP3DSpy.h>

#include "../Core/CCP3DEditorCore.h"
#include "CCP3DSpiesManager.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DSpiesManager::CCP3DSpiesManager(CCP3DEditorCore *editorCore) : EditorCore(editorCore)
{
	Timer = editorCore->getDevice()->getTimer();
	editorCore->getEngine()->getCustomUpdater()->addCustomUpdate(this);
}

CCP3DSpiesManager::~CCP3DSpiesManager() {

}

void CCP3DSpiesManager::OnPreUpdate() {
	core::list<SSpy>::Iterator it = Spies.begin();
	u32 time = Timer->getTime();

	for (; it != Spies.end(); ++it) {
		u32 lastTime = (*it).LastTime;
		if (time - (*it).LastTime >= (*it).Spy->getInterval()) {
			(*it).Spy->prepare();
			(*it).Spy->check();
			(*it).LastTime = time;
		}
	}

}

bool CCP3DSpiesManager::addSpy(ICP3DSpy *spy) {
	core::list<SSpy>::ConstIterator it = Spies.begin();

	for (; it != Spies.end(); ++it) {
		stringc name = (*it).Spy->getName();
		if (name == spy->getName())
			return false;
	}

	Spies.push_back({ spy, 0});
	return true;
}

bool CCP3DSpiesManager::removeSpy(ICP3DSpy *spy) {
	core::list<SSpy>::Iterator it = Spies.begin();

	for (; it != Spies.end(); ++it) {
		stringc name = it->Spy->getName();
		if (name == spy->getName()) {
			Spies.erase(it);
			return true;
		}
	}

	return false;
}

} /// End namespace cp3d
