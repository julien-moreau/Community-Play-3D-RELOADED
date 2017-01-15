#ifndef __H_C_SSAO_EFFECT_INCLUDED__
#define __H_C_SSAO_EFFECT_INCLUDED__

#include <irrlicht.h>
#include <ICP3DHandlerPostProcesses.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;

class CSSAOEffect : public IPostProcessingRenderCallback {
public:
	/// ctor & dtor
	CSSAOEffect(CCP3DHandler *handler);
	~CSSAOEffect();

	/// IPostProcessingRenderCallback
	void OnPreRender(ICP3DHandler* handler);

private:

	/// Datas
	CCP3DHandler *Handler;

	/// Materials
	irr::s32 SSAOMaterial;
	irr::s32 BlurHMaterial;
	irr::s32 BlurVMaterial;
	irr::s32 SSAOCombineMaterial;

	/// Textures
	irr::video::ITexture *NormalRTT;
	irr::video::ITexture *DepthRTT;
	irr::video::ITexture *RandomTex;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
