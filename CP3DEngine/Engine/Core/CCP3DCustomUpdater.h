
#ifndef __H_C_CP3D_CUSTOM_UPDATER_INCLUDED__
#define __H_C_CP3D_CUSTOM_UPDATER_INCLUDED__

#include <ICP3DCustomUpdate.h>

namespace cp3d {
namespace engine {

class CCP3DCustomUpdater : public ICP3DCustomUpdater {
public:

	CCP3DCustomUpdater();
	~CCP3DCustomUpdater();

	void OnPreUpdate();
	void OnPostUpdate();

	bool addCustomUpdate(ICP3DUpdate *update);
	bool addCustomUpdateToFront(ICP3DUpdate *update);
	bool removeCustomUpdate(ICP3DUpdate *update);

private:
	irr::core::array<ICP3DUpdate *> Updates;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
