#ifndef __H_C_CUSTOM_DEPTH_PASS_INCLUDED__
#define __H_C_CUSTOM_DEPTH_PASS_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomDepthPass.h>

namespace cp3d {
namespace rendering {

class CShaderPreprocessor;

class CCustomDepthPass : public ICP3DCustomDepthPass, 
						 public irr::video::IShaderConstantSetCallBack
{
public:

	CCustomDepthPass(irr::video::IVideoDriver *driver, irr::core::stringc name);
	~CCustomDepthPass();

	bool setRenderTarget();
	void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	void addPass(irr::core::stringc name);
	void setDepth(irr::core::stringc name, irr::f32 farLink);

private:
	irr::core::array<irr::video::IRenderTarget> RenderTargets;
	irr::core::array<irr::f32> FarLinks;

	/// Shader pre-processor Vertex & Pixel
	CShaderPreprocessor *sppV, *sppP;
	irr::core::stringc vertexShader, pixelShader;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
