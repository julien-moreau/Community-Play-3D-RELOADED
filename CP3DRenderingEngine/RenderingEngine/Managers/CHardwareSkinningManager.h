#ifndef __H_C_HARDWARE_SKINNING_MANAGER_INCLUDED__
#define __H_C_HARDWARE_SKINNING_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DHardwareSkinningManager.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CHardwareSkinningMaterial;

class CHardwareSkinningManager : public ICP3DHardwareSkinningManager {

public:

	// Ctor & dtor
	CHardwareSkinningManager(CCP3DHandler *handler);
	~CHardwareSkinningManager();

	// ICP3DHardwareSkinningManager
	bool addNode(irr::scene::IAnimatedMeshSceneNode *node);
	bool removeNode(irr::scene::IAnimatedMeshSceneNode *node);
	ICP3DHardwareSkinningMaterial *getHWMaterial(irr::scene::IAnimatedMeshSceneNode *node);

private:
	// Rendering
	CCP3DHandler *Handler;

	// Datas
	irr::core::array<CHardwareSkinningMaterial *> Materials;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
