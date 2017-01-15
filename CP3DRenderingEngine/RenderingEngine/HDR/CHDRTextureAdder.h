#ifndef __H_C_HDR_TEXTURE_ADDER_INCLUDED__
#define __H_C_HDR_TEXTURE_ADDER_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CScreenQuadCB;

class CHDRTextureAdder {
	friend class CHDRManager;
public:
	/// ctor & dtor
	CHDRTextureAdder(CCP3DHandler *handler, irr::video::ITexture *other);
	~CHDRTextureAdder();

	void render(irr::video::ITexture *source, CScreenQuad &screenQuad);

private:

	/// Textures
	irr::video::ITexture *LensTexture;

	/// Datas
	CCP3DHandler *Handler;
	irr::video::IVideoDriver *Driver;

	/// Material
	CScreenQuadCB *Callback;
	irr::s32 MaterialType;
	irr::video::ITexture *OtherRTT;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
