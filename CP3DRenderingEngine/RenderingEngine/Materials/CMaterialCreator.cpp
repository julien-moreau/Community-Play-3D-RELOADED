
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

s32 CMaterialCreator::createMaterialFromFiles(const stringc &vertexFilename, const stringc &pixelFilename, E_MATERIAL_TYPE baseMaterial,
											  IShaderConstantSetCallBack *callback)
{
	return createMaterialFromStrings(Spp->getFileContent(vertexFilename.c_str()).c_str(), Spp->getFileContent(pixelFilename.c_str()).c_str(),
														 baseMaterial, callback);
}

s32 CMaterialCreator::createMaterialFromStrings(const stringc &vertexShader, const stringc &pixelShader, E_MATERIAL_TYPE baseMaterial,
												IShaderConstantSetCallBack *callback)
{
	IGPUProgrammingServices *gpu = Driver->getGPUProgrammingServices();

	#if defined(_DEBUG)
	stringc vs = Spp->ppShaderDF(vertexShader).c_str();
	stringc ps = Spp->ppShaderDF(pixelShader).c_str();
	#endif

	return gpu->addHighLevelShaderMaterial(Spp->ppShaderDF(vertexShader, true).c_str(), "vertexMain", EVST_VS_3_0,
										   pixelShader == "" ? NULL : Spp->ppShaderDF(pixelShader).c_str(), pixelShader == "" ? NULL : "pixelMain", EPST_PS_3_0,
										   callback, baseMaterial);
}

s32 CMaterialCreator::createCustomDepthMaterialFromFiles(const stringc &vertexFilename, const stringc &pixelFilename,
														 E_MATERIAL_TYPE baseMaterial, IShaderConstantSetCallBack *callback)
{
	return createCustomDepthMaterialFromStrings(Spp->getFileContent(vertexFilename.c_str()).c_str(), Spp->getFileContent(pixelFilename.c_str()).c_str(),
												baseMaterial, callback);
}
s32 CMaterialCreator::createCustomDepthMaterialFromStrings(const stringc &vertexShader, const stringc &pixelShader,
														   E_MATERIAL_TYPE baseMaterial, IShaderConstantSetCallBack *callback)
{
	Spp->addShaderDefine("CP3D_COMPUTE_DEPTH_MATERIAL");
	s32 mat = createMaterialFromStrings(vertexShader, pixelShader, baseMaterial, callback);
	Spp->removeShaderDefine("CP3D_COMPUTE_DEPTH_MATERIAL");
	return mat;
}

s32 CMaterialCreator::createCustomShadowsMaterialFromFile(const stringc &vertexFilename, E_MATERIAL_TYPE baseMaterial,
														  IShaderConstantSetCallBack *callback)
{
	return createCustomShadowsMaterialFromString(Spp->getFileContent(vertexFilename.c_str()).c_str(), baseMaterial, callback);
}
s32 CMaterialCreator::createCustomShadowsMaterialFromString(const stringc &vertexShader, E_MATERIAL_TYPE baseMaterial,
															IShaderConstantSetCallBack *callback)
{
	// To fix
	Spp->addShaderDefine("SAMPLE_AMOUNT", "1");
	Spp->addShaderDefine("VSM");

	Spp->addShaderDefine("CP3D_COMPUTE_SHADOWS_MATERIAL");
	s32 mat = createMaterialFromStrings(vertexShader, Spp->getFileContent("Shaders/InternalHandler/ShadowPass.fragment.fx").c_str(), baseMaterial, callback);
	Spp->removeShaderDefine("CP3D_COMPUTE_SHADOWS_MATERIAL");
	return mat;
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
		Spp->addShaderDefine("OPENGL_DRIVER");
	else
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	if (Driver->getDriverType() == EDT_DIRECT3D11)
		Spp->addShaderDefine("DIRECT3D_11");
	else
	#endif
	if (Driver->getDriverType() == EDT_DIRECT3D9)
		Spp->addShaderDefine("DIRECT3D_9");

	Spp->addShaderDefine("CP3D_MATERIAL");
}

} /// End namespace rendering
} /// End namespace cp3d
