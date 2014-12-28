
#include "stdafx.h"
#include "CMaterialCreator.h"

#include "../Handler/CShaderPre.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace irr;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

CMaterialCreator::CMaterialCreator(IVideoDriver *driver) : Driver(driver)
{
	Spp = new CShaderPreprocessor(driver);
	clearDefines();
}

CMaterialCreator::~CMaterialCreator() {
	delete Spp;
}

irr::s32 CMaterialCreator::createMaterialFromFiles(const stringc &vertexFilename, const stringc &pixelFilename, E_MATERIAL_TYPE baseMaterial,
												   IShaderConstantSetCallBack *callback)
{
	return createMaterialFromStrings(Spp->getFileContent(vertexFilename.c_str()).c_str(), Spp->getFileContent(pixelFilename.c_str()).c_str(), baseMaterial, callback);
}

irr::s32 CMaterialCreator::createMaterialFromStrings(const stringc &vertexShader, const stringc &pixelShader, E_MATERIAL_TYPE baseMaterial,
													 IShaderConstantSetCallBack *callback)
{
	IGPUProgrammingServices *gpu = Driver->getGPUProgrammingServices();

	#if defined(_DEBUG)
	stringc vs = Spp->ppShaderDF(vertexShader).c_str();
	stringc ps = Spp->ppShaderDF(pixelShader).c_str();
	#endif

	return gpu->addHighLevelShaderMaterial(Spp->ppShaderDF(vertexShader).c_str(), "vertexMain", EVST_VS_3_0,
										   Spp->ppShaderDF(pixelShader).c_str(), "pixelMain", EPST_PS_3_0,
										   callback, baseMaterial);
}

void CMaterialCreator::addDefine(const stringc define, const stringc value) {
	Spp->addShaderDefine(define, value);
}

void CMaterialCreator::removeDefine(const irr::core::stringc define) {
	Spp->removeShaderDefine(define);
}

void CMaterialCreator::clearDefines() {
	Spp->clearDefines();
	if (Driver->getDriverType() == EDT_OPENGL)
		Spp->addShaderDefine("OPENGL_DRIVER", "1");
	else
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	if (Driver->getDriverType() == EDT_DIRECT3D11)
		Spp->addShaderDefine("DIRECT3D_11", "1");
	else
	#endif
		if (Driver->getDriverType() == EDT_DIRECT3D9)
		Spp->addShaderDefine("DIRECT3D_9", "1");
}

} /// End namespace rendering
} /// End namespace cp3d
