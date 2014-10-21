// Il s'agit du fichier DLL principal.

#include "stdafx.h"
#include "CP3DRenderingEngine.h"

#include "RenderingEngine/Handler/CP3DHandler.h"
#include "RenderingEngine/Materials/CMaterialCreator.h"
#include "RenderingEngine/Materials/CNormalMappingMaterial.h"

using namespace irr;
using namespace video;

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

IrrlichtDevice *CCP3DRenderingEngine::getDevice() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice();
}

IVideoDriver *CCP3DRenderingEngine::getVideoDriver() {
	return ((CCP3DHandler*)Handler)->getIrrlichtDevice()->getVideoDriver();
}

/// Creators
ICP3DMaterialCreator *CCP3DRenderingEngine::createMaterialCreator() {
	return new CMaterialCreator(((CCP3DHandler*)Handler)->getIrrlichtDevice()->getVideoDriver());
}

ICP3DLightSceneNode *CCP3DRenderingEngine::createLightSceneNode() {
	ICP3DLightSceneNode *l = new ICP3DLightSceneNode(((CCP3DHandler*)Handler)->getIrrlichtDevice()->getSceneManager()->addLightSceneNode());
	Lights.push_back(l);
	return l;
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
