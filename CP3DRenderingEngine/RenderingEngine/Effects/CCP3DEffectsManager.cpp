#include "Stdafx.h"

#include "CCP3DEffectsManager.h"
#include "../Handler/CP3DHandler.h"

#include "CSSAOEffect.h"
#include "CVolumetricLightScattering.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CCP3DEffectsManager::CCP3DEffectsManager(CCP3DHandler *handler)
	: Handler(handler), SSAO(0), VLS(0)
{ }

CCP3DEffectsManager::~CCP3DEffectsManager() {
	createSSAOEffect(false);
}

void CCP3DEffectsManager::createSSAOEffect(const bool create) {
	if (create) {
		if (!SSAO)
			SSAO = new CSSAOEffect(Handler);
	}
	else {
		if (SSAO)
			delete SSAO;
		SSAO = 0;
	}
}

void CCP3DEffectsManager::createVolumetricLightScatteringEffect(const bool create, irr::scene::ISceneNode *node) {
	if (create) {
		if (!VLS)
			VLS = new CVolumetricLightScatteringEffect(Handler, node);
	}
	else {
		if (VLS)
			delete VLS;
		VLS = 0;
	}
}

} /// End namespace rendering
} /// End namespace cp3d
