
#include "stdafx.h"
#include "CCP3DSceneNodeCreator.h"

#include <ICP3DRenderingEngine.h>

#include "Clouds/CCloudSceneNode.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

namespace cp3d {
namespace engine {

CCP3DSceneNodeCreator::CCP3DSceneNodeCreator(rendering::ICP3DRenderingEngine *renderingEngine) : Rengine(renderingEngine)
{
	Device = Rengine->getDevice();
	Driver = Rengine->getVideoDriver();
	Smgr = Rengine->getSceneManager();
}

CCP3DSceneNodeCreator::~CCP3DSceneNodeCreator() {

}

irr::scene::ISceneNode *CCP3DSceneNodeCreator::createCloudNode(const vector2df &translation, ITexture *texture,
															   const f32 textureScale, const f32 centerHeight, const f32 innerHeight,
															   const f32 outerHeight, ICP3DSceneNodeCreatorCallback callback)
{
	CCloudSceneNode *cloud = new CCloudSceneNode(Smgr->getRootSceneNode(), Smgr);
	cloud->setTranslation(translation);
	cloud->getMaterial(0).setTexture(0, texture);
	cloud->setCloudHeight(centerHeight, innerHeight, outerHeight);
	cloud->setTextureScale(textureScale);
	cloud->setName("Cloud Layer 1");

	callback(cloud);

	return cloud;
}

} /// End namespace engine
} /// End namespace cp3d
