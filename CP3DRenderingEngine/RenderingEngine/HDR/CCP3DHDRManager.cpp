#include "Stdafx.h"

#include "CCP3DHDRManager.h"
#include "../Handler/CP3DHandler.h"

#include "CHDRBrightPass.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRManager::CHDRManager(CCP3DHandler *handler) : Handler(handler) {
	/// Configure
	Driver = Handler->getVideoDriver();
	TempTexture = Driver->addRenderTargetTexture(Handler->getScreenRTTSize(), "HDRTempTexture", ECF_R8G8B8);

	/// Create pipeline
	BrightPass = new CHDRBrightPass(Handler);

}

CHDRManager::~CHDRManager() {

}

void CHDRManager::render(ITexture *source, ITexture *output) {
	/// Render pipeline
	Driver->setRenderTarget(TempTexture, true, true);
	BrightPass->render(source, ScreenQuad);

	Driver->setViewPort(Handler->getViewPort());
	Driver->setRenderTarget(output);

}

} /// End namespace rendering
} /// End namespace cp3d
