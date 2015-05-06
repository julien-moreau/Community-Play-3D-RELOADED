#include "Stdafx.h"

#include "CEffectsManager.h"

#include "../Handler/CP3DHandler.h"
#include "CSSAOEffect.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CEffectsManager::CEffectsManager(CCP3DHandler *handler)
	: Handler(handler), SSAO(0)
{ }

CEffectsManager::~CEffectsManager() {
	createSSAOEffect(false);
}

void CEffectsManager::createSSAOEffect(bool create) {
	if (create) {
		if (!SSAO)
			SSAO = new CSSAOEffect(Handler);
	}
	else {
		if (SSAO)
			delete SSAO;
	}
}

} /// End namespace rendering
} /// End namespace cp3d
