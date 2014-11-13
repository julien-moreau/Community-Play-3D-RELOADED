#ifndef __H_C_CUSTOM_GENERAL_PASS_INCLUDED__
#define __H_C_CUSTOM_GENERAL_PASS_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomDepthPass.h>

namespace cp3d {
namespace rendering {

class CShaderPreprocessor;
class CMaterialCreator;

class CCustomGeneralPass : public ICP3DCustomPass, 
						   public irr::video::IShaderConstantSetCallBack
{
public:

	CCustomGeneralPass(irr::video::IVideoDriver *driver, irr::core::stringc name);
	~CCustomGeneralPass();

	bool setRenderTarget();
	void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

private:
	irr::core::array<irr::video::IRenderTarget> RenderTargets;

	/// Shader pre-processor Vertex & Pixel
	CMaterialCreator *MaterialCreator;
	irr::core::stringc VertexShader, PixelShader;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
