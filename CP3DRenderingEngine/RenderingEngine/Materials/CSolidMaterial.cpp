
#include "stdafx.h"
#include "CSolidMaterial.h"

#include "../../CP3DRenderingEngine.h"
#include "CMaterialCreator.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

namespace cp3d {
namespace rendering {

CSolidMaterial::CSolidMaterial(CCP3DRenderingEngine *renderingEngine) : RenderingEngine(renderingEngine)
{
	Driver = renderingEngine->getVideoDriver();

	CMaterialCreator m(renderingEngine->getVideoDriver());

	s32 MatType = m.createMaterialFromFiles("Shaders/Materials/Solid.vertex.fx", "Shaders/Materials/Solid.fragment.fx", EMT_SOLID, this);
	RenderingEngine->Materials[EMT_SOLID] = (E_MATERIAL_TYPE)MatType;

	m.addDefine("_CP3D_SOLID_2_LAYER_", "1");
	MatType = m.createMaterialFromFiles("Shaders/Materials/Solid.vertex.fx", "Shaders/Materials/Solid.fragment.fx", EMT_SOLID_2_LAYER, this);
	RenderingEngine->Materials[EMT_SOLID_2_LAYER] = (E_MATERIAL_TYPE)MatType;
}

CSolidMaterial::~CSolidMaterial() {
	Driver->getMaterialRenderer(RenderingEngine->Materials[EMT_SOLID])->drop();
	Driver->getMaterialRenderer(RenderingEngine->Materials[EMT_SOLID_2_LAYER])->drop();
}

void CSolidMaterial::OnSetMaterial(const SMaterial& material) {
	Material = &material;
}

void CSolidMaterial::OnSetConstants(IMaterialRendererServices *services, s32 userData) {

	/// Vertex
	if (Driver->getDriverType() != EDT_OPENGL) {
		matrix4 mWorldViewProj;
		mWorldViewProj *= Driver->getTransform(ETS_PROJECTION);
		mWorldViewProj *= Driver->getTransform(ETS_VIEW);
		mWorldViewProj *= Driver->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", mWorldViewProj.pointer(), 16);
	}
	/// Pixel
	else {
		s32 texVar = 0;
		services->setPixelShaderConstant("diffuseSampler", &texVar, 1);

		if (Material->MaterialType == RenderingEngine->Materials[EMT_SOLID_2_LAYER]) {
			s32 texVar1 = 1;
			services->setPixelShaderConstant("blendSampler", &texVar1, 1);
		}
	}
}

} /// End namespace rendering
} /// End namespace cp3d
