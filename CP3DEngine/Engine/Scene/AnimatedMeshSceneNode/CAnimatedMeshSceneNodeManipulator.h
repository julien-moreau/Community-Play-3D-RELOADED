#ifndef __H_C_CP3D_ANIMATED_MESH_MANIPULATOR_INCLUDED__
#define __H_C_CP3D_ANIMATED_MESH_MANIPULATOR_INCLUDED__

#include <irrlicht.h>
#include <ICP3DAnimatedMeshSceneNodeManipulator.h>

namespace cp3d {
namespace engine {

class CCP3DAnimatedMeshSceneNodeManipulator : public ICP3DAnimatedMeshSceneNodeManipulator {
public:
	
	CCP3DAnimatedMeshSceneNodeManipulator(irr::scene::ISceneManager *Smgr);
	~CCP3DAnimatedMeshSceneNodeManipulator();

	//! Sets the new mesh of the animated mesh scene node
	void setSkinnedMesh(irr::scene::IAnimatedMeshSceneNode *node, irr::scene::ISkinnedMesh *mesh);
	void appendAnimations(irr::scene::IAnimatedMeshSceneNode *node, irr::scene::ISkinnedMesh *mesh);

private:
	/// Irrlicht
	irr::scene::ISceneManager *Smgr;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
