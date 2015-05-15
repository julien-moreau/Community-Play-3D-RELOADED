#ifndef __H_C_HDR_DOWN_SAMPLE_INCLUDED__
#define __H_C_HDR_DOWN_SAMPLE_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CScreenQuadCB;

class CHDRDownSample {
public:
	/// ctor & dtor
	CHDRDownSample(CCP3DHandler *handler);
	~CHDRDownSample();

	void render(irr::video::ITexture *source, CScreenQuad &screenQuad);

private:

	/// Datas
	CCP3DHandler *Handler;
	irr::video::IVideoDriver *Driver;

	/// Material
	CScreenQuadCB *Callback;
	irr::s32 MaterialType;

	irr::f32 DsOffsets[32];

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
