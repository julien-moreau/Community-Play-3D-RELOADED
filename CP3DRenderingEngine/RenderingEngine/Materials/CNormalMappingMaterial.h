#ifndef __C_NORMAL_MAPPING_RENDERER_H_INCLUDED__
#define __C_NORMAL_MAPPING_RENDERER_H_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class CCP3DRenderingEngine;

class CNormalMappingMaterial : public irr::video::IShaderConstantSetCallBack {

public:
    
    CNormalMappingMaterial(CCP3DRenderingEngine *renderingEngine);
    ~CNormalMappingMaterial();

	void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData);

private:
	void computeArrays();

	CCP3DRenderingEngine *RenderingEngine;

	irr::core::array<irr::f32> LightStrengthArray;
    irr::core::array<irr::f32> LightPositionArray;
    irr::core::array<irr::f32> LightColorArray;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
