#ifndef __H_C_EFFECTS_MANAGER_INCLUDED__
#define __H_C_EFFECTS_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DEffectsManager.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;

class CSSAOEffect;

class CEffectsManager : public ICP3DEffectsManager {
public:
	/// ctor & dtor
	CEffectsManager(CCP3DHandler *handler);
	~CEffectsManager();

	/// Creators
	void createSSAOEffect(bool create);
	bool isSSAOCreated() { return SSAO != 0; }

private:

	/// Datas
	CCP3DHandler *Handler;

	/// Effects
	CSSAOEffect * SSAO;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
