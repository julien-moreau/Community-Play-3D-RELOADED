#ifndef __H_C_VOLUMETRIC_LIGHT_SCATTERING_EFFECT_INCLUDED__
#define __H_C_VOLUMETRIC_LIGHT_SCATTERING_EFFECT_INCLUDED__

#include <irrlicht.h>
#include <ICP3DHandlerPostProcesses.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;

class CVolumetricLightScatteringEffect : public IPostProcessingRenderCallback {
public:
	/// ctor & dtor
	CVolumetricLightScatteringEffect(CCP3DHandler *handler, irr::scene::ISceneNode *node);
	~CVolumetricLightScatteringEffect();

	/// IPostProcessingRenderCallback
	void OnPreRender(ICP3DHandler* handler);

private:

	/// Datas
	CCP3DHandler *Handler;
	irr::scene::ISceneManager *Smgr;
	irr::scene::ISceneCollisionManager *CollisionManager;
	irr::scene::ISceneNode *Node;
	irr::core::rect<irr::s32> ViewPort;

	/// Materials
	irr::s32 Material;
	irr::s32 MaterialID;

	/// Textures
	irr::video::ITexture *VolumetricLightScatteringRTT;

	/// Constants
	irr::f32 Decay;
	irr::f32 Exposure;
	irr::f32 Density;
	irr::f32 Weight;
	irr::core::vector2df LightPositionOnScreen;
	irr::video::SColorf LightColor;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
