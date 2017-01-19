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
	cmat.addDefine("POST_PROCESS", "");
	
	Callback = new CScreenQuadCB(Handler, true);
	MaterialType = cmat.createMaterialFromFiles("Shaders/InternalHandler/ScreenQuad.vertex.fx", "Shaders/HDR/BrightPass.fragment.fx", EMT_SOLID, Callback);

	f32 sU = (1.f / f32(Driver->getScreenSize().Width / 2));
	f32 sV = (1.f / f32(Driver->getScreenSize().Height / 2));
	DsOffsets[0] = -0.5f * sU;
	DsOffsets[1] = 0.5f  * sV;
	DsOffsets[2] = 0.5f  * sU;
	DsOffsets[3] = 0.5f  * sV;
	DsOffsets[4] = -0.5f * sU;
	DsOffsets[5] = -0.5f * sV;
	DsOffsets[6] = 0.5f  * sU;
	DsOffsets[7] = -0.5f * sV;
	BrightThreshold = 0.8f;

	E_DRIVER_TYPE type = Driver->getDriverType();
	Callback->uniformDescriptors[type == EDT_OPENGL ? "dsOffsets[0]" : "dsOffsets"] = CScreenQuadCB::SUniformDescriptor(DsOffsets, 8);
	Callback->uniformDescriptors["brightThreshold"] = CScreenQuadCB::SUniformDescriptor(&BrightThreshold, 1);
}

CHDRBrightPass::~CHDRBrightPass() {

}

void CHDRBrightPass::render(ITexture *source, CScreenQuad &screenQuad) {
	screenQuad.getMaterial().setTexture(0, source);
	screenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)MaterialType;
	screenQuad.render(Driver);
}

} /// End namespace rendering
} /// End namespace cp3d
