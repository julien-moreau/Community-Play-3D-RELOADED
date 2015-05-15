#include "Stdafx.h"

#include "CHDRDownSample.h"
#include "../Handler/CP3DHandler.h"
#include "../Handler/CHandlerCB.h"

#include "../Materials/CMaterialCreator.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CHDRDownSample::CHDRDownSample(CCP3DHandler *handler) : Handler(handler) {
	Driver = Handler->getVideoDriver();

	CMaterialCreator cmat(handler->getVideoDriver());
	
	Callback = new CScreenQuadCB(Handler, true);
	MaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/DownSample.fragment.fx", EMT_SOLID, Callback);

	u32 id = 0;
	for (s32 i = -2; i < 2; i++) {
		for (s32 j = -2; j < 2; j++) {
			DsOffsets[id] = (f32(i) + 0.5f) * (1.0f / f32(Driver->getScreenSize().Width));
			DsOffsets[id + 1] = (f32(j) + 0.5f) * (1.0f / f32(Driver->getScreenSize().Height));
			id += 2;
		}
	}

	Callback->uniformDescriptors["dsOffsets"] = CScreenQuadCB::SUniformDescriptor(DsOffsets, 32);
}

CHDRDownSample::~CHDRDownSample() {

}

void CHDRDownSample::render(irr::video::ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialType;
	screenQuad.render(Driver);
}

} /// End namespace rendering
} /// End namespace cp3d
