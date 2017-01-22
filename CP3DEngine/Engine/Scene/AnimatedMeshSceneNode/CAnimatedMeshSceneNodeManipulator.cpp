
#include "stdafx.h"
#include "CAnimatedMeshSceneNodeManipulator.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;


namespace cp3d {
namespace engine {

	CCP3DAnimatedMeshSceneNodeManipulator::CCP3DAnimatedMeshSceneNodeManipulator(ISceneManager *smgr) : Smgr(smgr)
{

}

CCP3DAnimatedMeshSceneNodeManipulator::~CCP3DAnimatedMeshSceneNodeManipulator() {

}

void CCP3DAnimatedMeshSceneNodeManipulator::setSkinnedMesh(IAnimatedMeshSceneNode *node, ISkinnedMesh *mesh) {
	((ISkinnedMesh *)node->getMesh())->useAnimationFrom(mesh);
}

void CCP3DAnimatedMeshSceneNodeManipulator::appendAnimations(IAnimatedMeshSceneNode *node, ISkinnedMesh *mesh) {
	ISkinnedMesh *currentMesh = ((ISkinnedMesh *)node->getMesh());
	const irr::u32 framesCount = currentMesh->getFrameCount();

	array<ISkinnedMesh::SJoint *> currentJoints = currentMesh->getAllJoints();
	array<ISkinnedMesh::SJoint *> newJoints = mesh->getAllJoints();

	const u32 count = (u32)core::s32_min((s32)currentJoints.size(), (s32)newJoints.size());

	for (u32 i = 0; i < count; i++) {
		ISkinnedMesh::SJoint *currentJoint = currentJoints[i];
		ISkinnedMesh::SJoint *newJoint = newJoints[i];

		/// Append positions
		for (u32 j = 0; j < newJoint->PositionKeys.size(); j++) {
			ISkinnedMesh::SPositionKey key;
			key.frame = newJoint->PositionKeys[j].frame + framesCount;
			key.position = newJoint->PositionKeys[j].position;

			currentJoint->PositionKeys.push_back(key);
		}

		/// Append rotations
		for (u32 j = 0; j < newJoint->RotationKeys.size(); j++) {
			ISkinnedMesh::SRotationKey key;
			key.frame = newJoint->RotationKeys[j].frame + framesCount;
			key.rotation = newJoint->RotationKeys[j].rotation;

			currentJoint->RotationKeys.push_back(key);
		}

		/// Append scales
		for (u32 j = 0; j < newJoint->ScaleKeys.size(); j++) {
			ISkinnedMesh::SScaleKey key;
			key.frame = newJoint->ScaleKeys[j].frame + framesCount;
			key.scale = newJoint->ScaleKeys[j].scale;

			currentJoint->ScaleKeys.push_back(key);
		}
	}

	currentMesh->finalize();
	node->render();
}

} /// End namespace engine
} /// End namespace cp3d
