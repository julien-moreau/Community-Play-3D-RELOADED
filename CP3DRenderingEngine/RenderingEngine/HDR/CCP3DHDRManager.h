#ifndef __H_C_HDR_MANAGER_INCLUDED__
#define __H_C_HDR_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;

class CHDRBrightPass;

class CHDRManager {
public:
	/// ctor & dtor
	CHDRManager(CCP3DHandler *handler);
	~CHDRManager();

	void render(irr::video::ITexture *source, irr::video::ITexture *output);

private:

	/// Datas
	irr::video::IVideoDriver *Driver;
	irr::video::ITexture *TempTexture;

	CCP3DHandler *Handler;
	CScreenQuad ScreenQuad;

	/// Pipeline
	CHDRBrightPass *BrightPass;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
