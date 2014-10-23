// Il s'agit du fichier DLL principal.

#include "stdafx.h"
#include "CP3DRenderingEngine.h"

#include "RenderingEngine/Handler/CP3DHandler.h"
#include "RenderingEngine/Materials/CMaterialCreator.h"
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
	Handler = new CCP3DHandler(device, device->getVideoDriver()->getScreenSize(), false, true, true);
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

void CCP3DRenderingEngine::createNormalMappingMaterial() {
	if (!NormalMapMaterialType)
		NormalMapMaterialType = new CNormalMappingMaterial(this);
}

void CCP3DRenderingEngine::destroyNormalMappingMaterial() {
	if (NormalMapMaterialType)
		delete NormalMapMaterialType;
	NormalMapMaterialType = 0;

	getVideoDriver()->getMaterialRenderer(NormalMappingMaterialSolid)->drop();
	getVideoDriver()->getMaterialRenderer(NormalMappingMaterialTransAdd)->drop();
	getVideoDriver()->getMaterialRenderer(NormalMappingMaterialTransAlphaRef)->drop();

	NormalMappingMaterialSolid = (E_MATERIAL_TYPE)-1;
	NormalMappingMaterialTransAdd = (E_MATERIAL_TYPE)-1;
	NormalMappingMaterialTransAlphaRef = (E_MATERIAL_TYPE)-1;
}

} /// End namespace rendering
} /// End namespace cp3d
