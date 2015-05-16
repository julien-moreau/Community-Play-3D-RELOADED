#ifndef __H_C_HDR_LUMINANCE_INCLUDED__
#define __H_C_HDR_LUMINANCE_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CScreenQuadCB;

class CHDRLuminance {
	friend class CHDRManager;
public:
	static const irr::u32 LumStepsNum = 6;

	/// ctor & dtor
	CHDRLuminance(CCP3DHandler *handler);
	~CHDRLuminance();

	void renderLuminance(irr::video::ITexture *source, CScreenQuad &screenQuad);
	void renderDownSample(CScreenQuad &screenQuad);

private:
	/// Methods
	void calculateDownSampleOffsets(const irr::core::dimension2du &dim);

	/// Datas
	CCP3DHandler *Handler;
	irr::video::IVideoDriver *Driver;

	/// Material
	irr::video::ITexture *LuminanceRTTs[LumStepsNum];

	CScreenQuadCB *CallbackLuminance;
	irr::s32 MaterialTypeLuminance;
	irr::f32 LuminanceOffsets[8];

	CScreenQuadCB *CallbackDownSample;
	irr::s32 MaterialTypeDownSample;
	irr::s32 MaterialTypeDownSampleFinal;
	irr::f32 DownSampleOffsets[18];
	irr::f32 HalfDestPixelSize;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
