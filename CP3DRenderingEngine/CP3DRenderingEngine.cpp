// Il s'agit du fichier DLL principal.

#include "stdafx.h"
#include "CP3DRenderingEngine.h"

#include "RenderingEngine/Handler/CP3DHandler.h"
#include "RenderingEngine/Materials/CMaterialCreator.h"

#include "RenderingEngine/Materials/CSolidMaterial.h"
#include "RenderingEngine/Materials/CNormalMappingMaterial.h"

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
	Handler = new CCP3DHandler(device, device->getVideoDriver()->getScreenSize(), false, true, true);
	((CCP3DHandler *)Handler)->setUseVSM(false);
	
	/// Configure materials
	u32 i = 0;
	while (sBuiltInMaterialTypeNames[i] != 0) {
		Materials[(E_MATERIAL_TYPE)i] = (E_MATERIAL_TYPE)i;
		i++;
	}

	//SolidMaterialType = new CSolidMaterial(this);
}

CCP3DRenderingEngine::~CCP3DRenderingEngine() {
	delete Handler;
}

/// Irrlicht
IrrlichtDevice *CCP3DRenderingEngine::getDevice() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice();
}

IVideoDriver *CCP3DRenderingEngine::getVideoDriver() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice()->getVideoDriver();
}

ISceneManager *CCP3DRenderingEngine::getSceneManager() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice()->getSceneManager();
}

IGUIEnvironment *CCP3DRenderingEngine::getGUIEnvironment() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice()->getGUIEnvironment();
}

/// Creators
ICP3DMaterialCreator *CCP3DRenderingEngine::createMaterialCreator() {
	return new CMaterialCreator(((CCP3DHandler*)Handler)->getIrrlichtDevice()->getVideoDriver());
}

/// Lights
ICP3DLightSceneNode *CCP3DRenderingEngine::createLightSceneNode(const bool computeNormalMapping, const bool computeShadows) {
	CCP3DHandler *Chandler = (CCP3DHandler *)Handler;

	SShadowLight shadowLight;
	s32 shadowLightIndex = -1;

	if (computeShadows)
		shadowLightIndex = Chandler->addShadowLight(shadowLight);

	ILightSceneNode *lightSceneNode = getDevice()->getSceneManager()->addLightSceneNode();
	ICP3DLightSceneNode *light = new ICP3DLightSceneNode(lightSceneNode, computeNormalMapping, shadowLightIndex);

	light->ShadowLight = (shadowLightIndex == -1) ? 0 : Chandler->getShadowLightPtr(shadowLightIndex);
	light->ShadowLight->LightScenenode = lightSceneNode;

	Lights.push_back(light);
	return light;
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
		((CCP3DHandler*)Handler)->removeShadowLight(node->ShadowLightIndex);
		node->ShadowLightIndex = -1;
		node->ShadowLight = 0;
		return -1;
	}

	SShadowLight shadowLight;
	shadowLight.DiffuseColor = node->getLightData().DiffuseColor;
	shadowLight.LightScenenode = *node;

	s32 shadowLightIndex = (s32)((CCP3DHandler*)Handler)->addShadowLight(shadowLight);
	node->ShadowLightIndex = shadowLightIndex;
	node->ShadowLight = ((CCP3DHandler*)Handler)->getShadowLightPtr(shadowLightIndex);

	return shadowLightIndex;
}

/// Materials
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

} /// End namespace rendering
} /// End namespace cp3d
