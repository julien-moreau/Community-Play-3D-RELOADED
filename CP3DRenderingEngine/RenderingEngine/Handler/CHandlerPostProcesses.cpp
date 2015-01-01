#include "Stdafx.h"

#include "CHandler.h"
#include "CHandlerCB.h"
#include "HandlerShaders.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

void CCP3DHandler::setPostProcessingRenderCallback(const irr::s32 &materialType,
	std::function<void(ICP3DHandler *handler)> OnPreRender,
	std::function<void(ICP3DHandler *handler)> OnPostRender)
{
	irr::s32 i = getPostProcessID(materialType);

	if (i == -1)
		return;

	CCustomPostProcessCB *cb = new CCustomPostProcessCB(OnPreRender, OnPostRender);
	setPostProcessingRenderCallback(materialType, cb);
}

void CCP3DHandler::addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback) {
	SPostProcessingPair pPair(MaterialType, 0);
	pPair.renderCallback = callback;
	PostProcessingRoutines.push_back(pPair);
}

CCP3DHandler::SPostProcessingPair CCP3DHandler::obtainScreenQuadMaterial(const irr::core::stringc& data,  irr::video::E_MATERIAL_TYPE baseMaterial, bool fromFile) {
	CShaderPreprocessor sPP(driver);

	sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
	sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));
	if (driver->getDriverType() == EDT_OPENGL)
		sPP.addShaderDefine("OPENGL_DRIVER");
	else
		sPP.addShaderDefine("DIRECT3D_9");

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	if (driver->getDriverType() == EDT_DIRECT3D11)
		sPP.addShaderDefine("DIRECT3D_11", "1");
	#endif
	
	video::E_VERTEX_SHADER_TYPE VertexLevel = driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	video::E_PIXEL_SHADER_TYPE PixelLevel = driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ESE_HLSL : ESE_GLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	stringc shaderString;
	if (fromFile)
		//shaderString = sPP.ppShaderFF(data.c_str());
		shaderString = sPP.ppShaderDF(sPP.getFileContent(data.c_str()).c_str());
	else
		shaderString = sPP.ppShaderDF(data.c_str());

	ScreenQuadCB* SQCB = new ScreenQuadCB(this, true);
	const stringc path = WorkingPath + "Shaders/InternalHandler/ScreenQuad.vertex.fx";

	// To ensure includes, set the directory to origin
	const stringc currentDirectory = device->getFileSystem()->getWorkingDirectory();
	device->getFileSystem()->changeWorkingDirectoryTo(WorkingPath);

	s32 PostMat = gpu->addHighLevelShaderMaterial(
		sPP.ppShaderDF(sPP.getFileContent(path.c_str()).c_str()).c_str(), "vertexMain", VertexLevel,
		shaderString.c_str(), "pixelMain", PixelLevel,
		SQCB, baseMaterial);
	
	SPostProcessingPair pPair(PostMat, SQCB);
	SQCB->drop();

	// Reset directory
	device->getFileSystem()->changeWorkingDirectoryTo(currentDirectory);

	return pPair;
}

void CCP3DHandler::setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, const f32* data, const irr::u32 count) {
	s32 matIndex = getPostProcessID(materialType);
	
	if(matIndex != -1)
		PostProcessingRoutines[matIndex].callback->uniformDescriptors[name] = ScreenQuadCB::SUniformDescriptor(data, count);
}

const u32 CCP3DHandler::getPostProcessID(const irr::s32 &id) {
	s32 matIndex = -1;
	if (id >= 0 && id < PostProcessingRoutines.size())
		matIndex = id;

	if (matIndex == -1) {
		SPostProcessingPair tempPair(id, 0);
		s32 matIndex = PostProcessingRoutines.binary_search(tempPair);
	}

	return matIndex;
}
void CCP3DHandler::setPostProcessActivated(const s32 &id, const bool activated) {
	s32 matIndex = getPostProcessID(id);

	if (matIndex != -1)
		PostProcessingRoutines[matIndex].activated = activated;
}
bool CCP3DHandler::isPostProcessActivated(const irr::s32 &id) {
	s32 matIndex = getPostProcessID(id);

	if (matIndex != -1)
		return PostProcessingRoutines[matIndex].activated;

	return false;
}

const stringc CCP3DHandler::getPostProcessingRoutineName(const s32 &materialType) {
	u32 matIndex = getPostProcessID(materialType);

	if (matIndex != -1)
		return PostProcessingRoutines[matIndex].path;

	return "";
}

s32 CCP3DHandler::addPostProcessingEffectFromString(const irr::core::stringc &shader, IPostProcessingRenderCallback *callback) {
	SPostProcessingPair pPair = obtainScreenQuadMaterial(shader, EMT_SOLID, false);
	pPair.renderCallback = callback;

	if (callback)
		callback->MaterialType = pPair.materialType;

	PostProcessingRoutines.push_back(pPair);

	return pPair.materialType;
}

s32 CCP3DHandler::addPostProcessingEffectFromFile(const irr::core::stringc& filename, IPostProcessingRenderCallback* callback) {
	SPostProcessingPair pPair = obtainScreenQuadMaterial(filename, EMT_SOLID, true);
	pPair.renderCallback = callback;
	pPair.path = filename;

	if (callback)
		callback->MaterialType = pPair.materialType;

	PostProcessingRoutines.push_back(pPair);

	return pPair.materialType;
}

} /// End namespace rendering
} /// End namespace cp3d
