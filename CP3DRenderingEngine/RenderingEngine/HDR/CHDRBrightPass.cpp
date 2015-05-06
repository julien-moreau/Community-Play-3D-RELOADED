#include "Stdafx.h"

#include "CHDRBrightPass.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRBrightPass::CHDRBrightPass(CCP3DHandler *handler) : Handler(handler) {
	Driver = Handler->getVideoDriver();

	CMaterialCreator cmat(handler->getVideoDriver());
	
	Callback = new ScreenQuadCB(Handler, true);
	MaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/BrightPass.fragment.fx", EMT_SOLID, Callback);
}

CHDRBrightPass::~CHDRBrightPass() {

}

void CHDRBrightPass::render(irr::video::ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialType;
	screenQuad.render(Driver);
}

} /// End namespace rendering
} /// End namespace cp3d
