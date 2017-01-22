#ifndef __H_I_CP3D_ANIMATED_MESH_MANIPULATOR_INCLUDED__
#define __H_I_CP3D_ANIMATED_MESH_MANIPULATOR_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

class ICP3DAnimatedMeshSceneNodeManipulator {
public:

	//! Sets the new skinned mesh to use
	//! \param node: the scene node to configure
	//! \param mesh: the new mesh to apply
	virtual void setSkinnedMesh(irr::scene::IAnimatedMeshSceneNode *node, irr::scene::ISkinnedMesh *mesh) = 0;

	//! Appends the animations to an existing animated mesh scene node
	//! \param node: the scene node to configure
	//! \param mesh: the new mesh to append
	virtual void appendAnimations(irr::scene::IAnimatedMeshSceneNode *node, irr::scene::ISkinnedMesh *mesh) = 0;

};

} /// End namespace engine
} /// End namespace cp3d
#endif
