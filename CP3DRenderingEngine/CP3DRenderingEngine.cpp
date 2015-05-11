#include "stdafx.h"
#include "CP3DRenderingEngine.h"

/// Rendering
#include "RenderingEngine/Handler/CP3DHandler.h"
#include "RenderingEngine/Materials/CMaterialCreator.h"

/// Materials
#include "RenderingEngine/Materials/CSolidMaterial.h"
#include "RenderingEngine/Materials/CNormalMappingMaterial.h"

/// Effects


using namespace irr;
using namespace video;
using namespace scene;
using namespace gui;

/// Create rendering engine
namespace cp3d {
	CP3DR_LIB_API rendering::ICP3DRenderingEngine *createRenderingEngine(IrrlichtDevice *device) {
		return new rendering::CCP3DRenderingEngine(device);
	}
}

/// Rendering engine class
namespace cp3d {
namespace rendering {

CCP3DRenderingEngine::CCP3DRenderingEngine(IrrlichtDevice *device) : NormalMapMaterialType(0)
{
	/// Configure
	Handler = new CCP3DHandler(device, device->getVideoDriver()->getScreenSize(), true, true, true);
	
	/// Configure materials
	u32 i = 0;
	while (sBuiltInMaterialTypeNames[i] != 0) {
		Materials[(E_MATERIAL_TYPE)i] = (E_MATERIAL_TYPE)i;
		i++;
	}

	/// Materials
	SolidMaterialType = new CSolidMaterial(this);
	HWSkinningManager = new CHardwareSkinningManager(Handler);
	EffectsManager = new CCP3DEffectsManager(Handler);
}

CCP3DRenderingEngine::~CCP3DRenderingEngine() {
	delete Handler;
}

/// Irrlicht
IrrlichtDevice *CCP3DRenderingEngine::getDevice() {
	return Handler->getIrrlichtDevice();
}

IVideoDriver *CCP3DRenderingEngine::getVideoDriver() {
	return Handler->getIrrlichtDevice()->getVideoDriver();
}

ISceneManager *CCP3DRenderingEngine::getSceneManager() {
	return Handler->getIrrlichtDevice()->getSceneManager();
}

IGUIEnvironment *CCP3DRenderingEngine::getGUIEnvironment() {
	return Handler->getIrrlichtDevice()->getGUIEnvironment();
}

/// Lights
ICP3DLightSceneNode *CCP3DRenderingEngine::createLightSceneNode(const bool computeNormalMapping, const bool computeShadows) {
	SShadowLight shadowLight;
	s32 shadowLightIndex = -1;

	if (computeShadows)
		shadowLightIndex = Handler->addShadowLight(shadowLight);

	ILightSceneNode *lightSceneNode = getDevice()->getSceneManager()->addLightSceneNode();
	ICP3DLightSceneNode *light = new ICP3DLightSceneNode(lightSceneNode, computeNormalMapping, shadowLightIndex);

	light->ShadowLight = (shadowLightIndex == -1) ? 0 : Handler->getShadowLightPtr(shadowLightIndex);
	if (light->ShadowLight)
		light->ShadowLight->LightScenenode = lightSceneNode;

	Lights.push_back(light);
	return light;
}

void CCP3DRenderingEngine::removeLightSceneNode(ICP3DLightSceneNode *node) {
	const u32 index = Lights.binary_search(node);
	if (index == -1)
		return;

	if (node->ShadowLight)
		Handler->removeShadowLight(node->ShadowLightIndex);
	
	delete node;
	Lights.erase(index);
}

void CCP3DRenderingEngine::removeLightSceneNode(ILightSceneNode *node) {
	for (u32 i = 0; i < Lights.size(); i++) {
		if (Lights[i]->Node == node) {
			removeLightSceneNode(Lights[i]);
			return;
		}
	}
}

ICP3DLightSceneNode *CCP3DRenderingEngine::getLightSceneNode(ILightSceneNode *node) {
	for (u32 i = 0; i < Lights.size(); i++) {
		if (*Lights[i] == node) {
			return Lights[i];
		}
	}

	return 0;
}

irr::s32 CCP3DRenderingEngine::setLightSceneNodeComputeShadows(const irr::u32 index, const bool compute) {
	if (index < 0 || index >= Lights.size())
		return -1;

	return setLightSceneNodeComputeShadows(Lights[index], compute);
}
s32 CCP3DRenderingEngine::setLightSceneNodeComputeShadows(ICP3DLightSceneNode *node, const bool compute) {
	if (!node)
		return -1;

	if (node->ShadowLight && compute)
		return node->ShadowLightIndex;

	if (!compute) {
		Handler->removeShadowLight(node->ShadowLightIndex);
		node->ShadowLightIndex = -1;
		node->ShadowLight = 0;
		return -1;
	}

	SShadowLight shadowLight;
	shadowLight.DiffuseColor = node->getLightData().DiffuseColor;
	shadowLight.LightScenenode = *node;

	s32 shadowLightIndex = (s32)Handler->addShadowLight(shadowLight);
	node->ShadowLightIndex = shadowLightIndex;
	node->ShadowLight = Handler->getShadowLightPtr(shadowLightIndex);

	return shadowLightIndex;
}

/// Materials
ICP3DMaterialCreator *CCP3DRenderingEngine::createMaterialCreator() {
	return new CMaterialCreator(Handler->getIrrlichtDevice()->getVideoDriver());
}

void CCP3DRenderingEngine::createNormalMappingMaterial() {
	if (!NormalMapMaterialType)
		NormalMapMaterialType = new CNormalMappingMaterial(this);
}

void CCP3DRenderingEngine::destroyNormalMappingMaterial() {
	if (NormalMapMaterialType)
		delete NormalMapMaterialType;
	NormalMapMaterialType = 0;

	getVideoDriver()->getMaterialRenderer(Materials[EMT_NORMAL_MAP_SOLID])->drop();
	getVideoDriver()->getMaterialRenderer(Materials[EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR])->drop();
	getVideoDriver()->getMaterialRenderer(Materials[EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA])->drop();

	Materials[EMT_NORMAL_MAP_SOLID] = (E_MATERIAL_TYPE)-1;
	Materials[EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR] = (E_MATERIAL_TYPE)-1;
	Materials[EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA] = (E_MATERIAL_TYPE)-1;
}

CHardwareSkinningManager *CCP3DRenderingEngine::getHWSkinningManager() {
	return HWSkinningManager;
}

/// Effects
CCP3DEffectsManager *CCP3DRenderingEngine::getEffectsManager() {
	return EffectsManager;
}

/// Scene
void CCP3DRenderingEngine::clear() {
	for (u32 i = 0; i < Lights.size(); i++)
		delete Lights[i];

	Handler->getIrrlichtDevice()->getSceneManager()->clear();
	Handler->clear();

	EffectsManager->createVolumetricLightScatteringEffect(false, 0);
	EffectsManager->createSSAOEffect(false);
}

} /// End namespace rendering
} /// End namespace cp3d
