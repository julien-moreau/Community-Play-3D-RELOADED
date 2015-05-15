#include "Stdafx.h"

#include "CHDRTextureAdder.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRTextureAdder::CHDRTextureAdder(CCP3DHandler *handler, ITexture *other) : Handler(handler), OtherRTT(other) {
	Driver = Handler->getVideoDriver();

	CMaterialCreator cmat(handler->getVideoDriver());
	
	Callback = new CScreenQuadCB(Handler, true);
	MaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/TextureAdder.fragment.fx", EMT_SOLID, Callback);
}

CHDRTextureAdder::~CHDRTextureAdder() {

}

void CHDRTextureAdder::render(ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().setTexture(3, OtherRTT);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialType;
	screenQuad.render(Driver);
}

} /// End namespace rendering
} /// End namespace cp3d
