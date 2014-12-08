#ifndef __C_SOLID_RENDERER_H_INCLUDED__
#define __C_SOLID_RENDERER_H_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class CCP3DRenderingEngine;

class CSolidMaterial : public irr::video::IShaderConstantSetCallBack {

public:
    
	/// Constructor & Destructor
	CSolidMaterial(CCP3DRenderingEngine *renderingEngine);
    ~CSolidMaterial();

	/// IShaderConstantSetCallback
	void OnSetMaterial(const irr::video::SMaterial& material);
	void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData);

private:
	/// Irrlicht
	irr::video::IVideoDriver *Driver;

	/// Datas
	const irr::video::SMaterial *Material;

	/// Rendering
	CCP3DRenderingEngine *RenderingEngine;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
