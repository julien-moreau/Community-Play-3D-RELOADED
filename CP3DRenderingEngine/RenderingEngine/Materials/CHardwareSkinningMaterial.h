#ifndef __H_C_HARDWARE_SKINNING_MATERIAL_INCLUDED__
#define __H_C_HARDWARE_SKINNING_MATERIAL_INCLUDED__

#include <irrlicht.h>
#include <ICP3DHardwareSkinningManager.h>
#include <ICP3DHandlerCallbacks.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;

class CHardwareSkinningMaterial : public ICP3DHardwareSkinningMaterial
{
public:

	// Ctor & dtor
	CHardwareSkinningMaterial(CCP3DHandler *handler, irr::scene::IAnimatedMeshSceneNode* node, irr::u32 refresh);
	~CHardwareSkinningMaterial();

	// IShaderConstantSetCallback
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	// Materials
	irr::s32 getMaterial() { return Material; }
	irr::s32 getDepthMaterial() { return DepthMaterial; }
	irr::s32 getShadowsMaterial() { return ShadowsMaterial; }

	// Utils
	irr::scene::IAnimatedMeshSceneNode *getNode() { return Node; }
	irr::scene::ISkinnedMesh *getMesh() { return Mesh; }

private:
	// Rendering
	CCP3DHandler *Handler;

	// Datas
	irr::scene::IAnimatedMeshSceneNode *Node;
	irr::scene::ISkinnedMesh* Mesh;

	irr::s32 Material, DepthMaterial, ShadowsMaterial;
	irr::u32 Oldtimer;
	irr::u32 Delay;

	// Methods
	void copyMat(irr::f32* pointer, irr::core::matrix4 &mat);
	void setupNode();

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
