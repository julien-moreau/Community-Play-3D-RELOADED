#ifndef __H_C_HDR_BRIGHT_PASS_INCLUDED__
#define __H_C_HDR_BRIGHT_PASS_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class ScreenQuadCB;

class CHDRBrightPass {
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
	ScreenQuadCB *Callback;
	irr::s32 MaterialType;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
