#ifndef __H_C_CP3D_SPIES_MANAGER_INCLUDED__
#define __H_C_CP3D_SPIES_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>

namespace cp3d {

class CCP3DEditorCore;
class ICP3DSpy;

class CCP3DSpiesManager : public engine::ICP3DUpdate {
public:

	/// Constructor & Destructor
	CCP3DSpiesManager(CCP3DEditorCore *editorCore);
	~CCP3DSpiesManager();

	/// ICP3DCustomUpdater
	void OnPreUpdate();

	/// Manager
	bool addSpy(ICP3DSpy *spy);
	bool removeSpy(ICP3DSpy *spy);

private:
	/// Editor
	CCP3DEditorCore *EditorCore;
	irr::ITimer *Timer;

	/// Spies structure
	struct SSpy {
		ICP3DSpy *Spy;
		irr::u32 LastTime;
	};

	/// Spies
	irr::core::list<SSpy> Spies;

};

} /// End namespace cp3d

#endif