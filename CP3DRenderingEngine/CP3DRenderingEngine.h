#ifndef __H_CP3D_RENDERING_ENGINE_INCLUDED__
#define __H_CP3D_RENDERING_ENGINE_INCLUDED__

#include <CP3DCompileConfig.h>
#include <ICP3DRenderingEngine.h>
#include <ICP3DLightSceneNode.h>
#include <ICP3DHardwareSkinningManager.h>
#include <ICP3DEffectsManager.h>

#include "RenderingEngine/Handler/CP3DHandler.h"
#include "RenderingEngine/Managers/CHardwareSkinningManager.h"
#include "RenderingEngine/Effects/CCP3DEffectsManager.h"

namespace cp3d {
namespace rendering {

class CNormalMappingMaterial;
class CSolidMaterial;

class CCP3DRenderingEngine : public ICP3DRenderingEngine {
public:

	/// Constructor & destructor
	CCP3DRenderingEngine(irr::IrrlichtDevice *device);
	~CCP3DRenderingEngine();

	/// Irrlicht
	irr::IrrlichtDevice *getDevice();
	irr::video::IVideoDriver *getVideoDriver();
	irr::scene::ISceneManager *getSceneManager();
	irr::gui::IGUIEnvironment *getGUIEnvironment();

	/// Handlers
	ICP3DHandler *getHandler() {
		return Handler;
	}

	/// Materials
	ICP3DMaterialCreator *createMaterialCreator();

	void createNormalMappingMaterial();
	void destroyNormalMappingMaterial();

	CHardwareSkinningManager *getHWSkinningManager();

	/// Effects
	CCP3DEffectsManager *getEffectsManager();

	/// Lights
	ICP3DLightSceneNode *createLightSceneNode(const bool computeNormalMapping = false, const bool computeShadows = false);
	irr::u32 getLightCount() {
		return Lights.size();
	}
	ICP3DLightSceneNode *getLightSceneNode(const irr::u32 &index) {
		return Lights[index];
	}
	ICP3DLightSceneNode *getLightSceneNode(irr::scene::ILightSceneNode *node);
	const irr::u32 getLightSceneNodeCount() const {
		return Lights.size();
	}
	void removeLightSceneNode(ICP3DLightSceneNode *node);
	void removeLightSceneNode(irr::scene::ILightSceneNode *node);
	irr::s32 setLightSceneNodeComputeShadows(ICP3DLightSceneNode *node, const bool compute);
	irr::s32 setLightSceneNodeComputeShadows(const irr::u32 index, const bool compute);

	/// Scene
	void clear();

private:
	/// Rendering
	CCP3DHandler *Handler;

	/// Materials
	CSolidMaterial *SolidMaterialType;
	CNormalMappingMaterial *NormalMapMaterialType;
	CHardwareSkinningManager *HWSkinningManager;

	/// Effects
	CCP3DEffectsManager *EffectsManager;

	/// Datas
	irr::core::array<ICP3DLightSceneNode *> Lights;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
