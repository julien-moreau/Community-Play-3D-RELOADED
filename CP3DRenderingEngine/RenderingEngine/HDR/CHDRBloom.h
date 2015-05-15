#ifndef __H_C_HDR_BLOOM_INCLUDED__
#define __H_C_HDR_BLOOM_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class ScreenQuadCB;

class CHDRBrightPass;
class CHDRDownSample;
class CHDRGaussianBlur;

class CHDRBloom {
	friend class CHDRManager;
public:
	/// ctor & dtor
	CHDRBloom(CCP3DHandler *handler);
	~CHDRBloom();

	void render(irr::video::ITexture *source, CScreenQuad &screenQuad);

private:

	/// Datas
	CCP3DHandler *Handler;
	irr::video::IVideoDriver *Driver;

	/// Rendering
	irr::video::ITexture *BrightPassRT;
	irr::video::ITexture *BlurRT1;
	irr::video::ITexture *BlurRT2;

	CHDRBrightPass *BrightPass;
	CHDRDownSample *DownSample;
	CHDRGaussianBlur *GaussianBlur;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
