#ifndef __H_C_HDR_BRIGHT_PASS_INCLUDED__
#define __H_C_HDR_BRIGHT_PASS_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CScreenQuadCB;

class CHDRBrightPass {
	friend class CHDRManager;
public:
	/// ctor & dtor
	CHDRBrightPass(CCP3DHandler *handler);
	~CHDRBrightPass();

	void render(irr::video::ITexture *source, CScreenQuad &screenQuad);

private:

	/// Datas
	CCP3DHandler *Handler;
	irr::video::IVideoDriver *Driver;

	/// Material
	CScreenQuadCB *Callback;
	irr::s32 MaterialType;

	irr::f32 DsOffsets[8];
	irr::f32 BrightThreshold;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
