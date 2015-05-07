#ifndef __H_C_EFFECTS_MANAGER_INCLUDED__
#define __H_C_EFFECTS_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DEffectsManager.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;

class CSSAOEffect;
class CVolumetricLightScatteringEffect;

class CCP3DEffectsManager : public ICP3DEffectsManager {
public:
	/// ctor & dtor
	CCP3DEffectsManager(CCP3DHandler *handler);
	~CCP3DEffectsManager();

	/// Creators
	void createSSAOEffect(const bool create);
	const bool isSSAOCreated() { return SSAO != 0; }

	void createVolumetricLightScatteringEffect(const bool create, irr::scene::ISceneNode *node);
	const bool isVolumetricLightScatteringCreated() { return VLS != 0; }

private:

	/// Datas
	CCP3DHandler *Handler;

	/// Effects
	CSSAOEffect *SSAO;
	CVolumetricLightScatteringEffect *VLS;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
