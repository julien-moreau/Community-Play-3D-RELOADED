#include "Stdafx.h"
#include "CHardwareSkinningManager.h"

#include "../Handler/CP3DHandler.h"
#include "../Materials/CHardwareSkinningMaterial.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace cp3d {
namespace rendering {

CHardwareSkinningManager::CHardwareSkinningManager(CCP3DHandler *handler) : Handler(handler)
{ }

CHardwareSkinningManager::~CHardwareSkinningManager() {
	for (u32 i = 0; i < Materials.size(); i++) {
		delete Materials[i];
	}
	Materials.clear();
}

bool CHardwareSkinningManager::addNode(IAnimatedMeshSceneNode *node) {
	if (node->getMesh()->getMeshType() != EAMT_SKINNED)
		return false;

	for (u32 i = 0; i < Materials.size(); i++) {
		if (Materials[i]->getNode() == node)
			return false;
	}

	CHardwareSkinningMaterial *hwmat = new CHardwareSkinningMaterial(Handler, node, 17);
	Materials.push_back(hwmat);

	return true;
}

bool CHardwareSkinningManager::removeNode(IAnimatedMeshSceneNode *node) {
	for (u32 i = 0; i < Materials.size(); i++) {
		if (Materials[i]->getNode() == node) {
			delete Materials[i];
			Materials.erase(i);
			return true;
		}
	}

	return false;
}

ICP3DHardwareSkinningMaterial *CHardwareSkinningManager::getHWMaterial(IAnimatedMeshSceneNode *node) {
	for (u32 i = 0; i < Materials.size(); i++) {
		if (Materials[i]->getNode() == node)
			return Materials[i];
	}

	return 0;
}

} /// End namespace rendering
} /// End namespace cp3d
