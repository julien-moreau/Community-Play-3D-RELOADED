#ifndef __H_C_HDR_GAUSSIAN_BLUR_INCLUDED__
#define __H_C_HDR_GAUSSIAN_BLUR_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CScreenQuadCB;

class CHDRGaussianBlur {
public:
	/// ctor & dtor
	CHDRGaussianBlur(CCP3DHandler *handler);
	~CHDRGaussianBlur();

	void renderH(irr::video::ITexture *source, CScreenQuad &screenQuad);
	void renderV(irr::video::ITexture *source, CScreenQuad &screenQuad);

private:
	/// Methods
	void calculateWeights();

	/// Datas
	CCP3DHandler *Handler;
	irr::video::IVideoDriver *Driver;

	/// Material
	CScreenQuadCB *CallbackH;
	CScreenQuadCB *CallbackV;

	irr::s32 MaterialTypeH;
	irr::s32 MaterialTypeV;

	irr::f32 BlurOffsetsH[9];
	irr::f32 BlurOffsetsV[9];

	irr::f32 GaussCoeff;
	irr::f32 GaussMean;
	irr::f32 GaussStandDev;
	irr::f32 BlurWeights[9];

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
