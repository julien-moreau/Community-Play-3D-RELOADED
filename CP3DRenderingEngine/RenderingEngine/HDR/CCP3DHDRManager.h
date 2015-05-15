#ifndef __H_C_HDR_MANAGER_INCLUDED__
#define __H_C_HDR_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CScreenQuadCB;

class CHDRBloom;
class CHDRTextureAdder;
class CHDRLuminance;

class CHDRManager {
public:
	/// ctor & dtor
	CHDRManager(CCP3DHandler *handler);
	~CHDRManager();

	void render(irr::video::ITexture *source, irr::video::ITexture *output);

private:

	/// Datas
	irr::video::IVideoDriver *Driver;
	irr::ITimer *Timer;

	CCP3DHandler *Handler;
	CScreenQuad ScreenQuad;

	irr::video::ITexture *TextureAdderRTT;

	/// Pipeline
	CHDRBloom *Bloom;
	CHDRTextureAdder *TextureAdder;
	CHDRLuminance *Luminance;

	/// Material
	CScreenQuadCB *Callback;
	irr::s32 MaterialType;

	/// HDR
	irr::f32 Exposure;

	irr::u32 LastTime;
	irr::f32 CurrentLuminance;
	irr::f32 MinLuminance;
	irr::f32 MaxLuminance;
	irr::f32 OutputLuminance;
	irr::f32 IncreaseRate;
	irr::f32 DecreaseRate;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
