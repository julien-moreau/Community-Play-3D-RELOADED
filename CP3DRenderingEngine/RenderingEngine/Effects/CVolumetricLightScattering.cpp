#include "Stdafx.h"

#include "CVolumetricLightScattering.h"
#include "../Handler/CP3DHandler.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CVolumetricLightScatteringEffect::CVolumetricLightScatteringEffect(CCP3DHandler *handler, ISceneNode *node)
	: Handler(handler), Node(node)
{
	IVideoDriver *driver = handler->getVideoDriver();
	Smgr = Handler->getActiveSceneManager();
	CollisionManager = Smgr->getSceneCollisionManager();

	VolumetricLightScatteringRTT = driver->getTexture("CP3DLightScattering");
	Material = handler->addPostProcessingEffectFromFile("Shaders/PostProcesses/VolumetricLightScattering.fragment.fx", this);
	MaterialID = handler->getPostProcessID(Material);

	Decay = 0.96815f;
	Exposure = 0.3f;
	Density = 0.926;
	Weight = 0.58767f;
	LightColor = SColorf(1.f, 0.5f, 0.f, 1.f);

	Handler->getGeneralPassManager()->setVolumetricLightScatteringNode(node);
}

CVolumetricLightScatteringEffect::~CVolumetricLightScatteringEffect() {
	/// Set callback to 0 to keep "this" instance in memory
	/// removePostprocessingEffect deletes the callback instance
	Handler->setPostProcessingRenderCallback(Material, 0);
	Handler->removePostProcessingEffect(Material);

	Handler->getGeneralPassManager()->setVolumetricLightScatteringNode(0);
}

void CVolumetricLightScatteringEffect::OnPreRender(ICP3DHandler* handler) {
	handler->setPostProcessingTextureAtIndex(3, VolumetricLightScatteringRTT);

	handler->setPostProcessingEffectConstant(MaterialID, "decay", &Decay, 1);
	handler->setPostProcessingEffectConstant(MaterialID, "exposure", &Exposure, 1);
	handler->setPostProcessingEffectConstant(MaterialID, "density", &Density, 1);
	handler->setPostProcessingEffectConstant(MaterialID, "weight", &Weight, 1);

	ViewPort = handler->getViewPort();
	vector2di pos = CollisionManager->getScreenCoordinatesFrom3DPosition(Node->getPosition(), Smgr->getActiveCamera(), true);
	LightPositionOnScreen.Y = (f32(pos.Y) / ViewPort.getHeight());
	LightPositionOnScreen.X = f32(pos.X) / ViewPort.getWidth();

	handler->setPostProcessingEffectConstant(MaterialID, "lightPositionOnScreen", &LightPositionOnScreen.X, 2);
	handler->setPostProcessingEffectConstant(MaterialID, "lightColor", &LightColor.r, 4);
}

} /// End namespace rendering
} /// End namespace cp3d
