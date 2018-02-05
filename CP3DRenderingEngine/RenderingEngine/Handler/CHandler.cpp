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

CCP3DHandler::CCP3DHandler(IrrlichtDevice* dev, const dimension2du& screenRTTSize,
	const bool useVSMShadows, const bool useRoundSpotLights, const bool use32BitDepthBuffers)
: device(dev), smgr(dev->getSceneManager()), driver(dev->getVideoDriver()),
ScreenRTTSize(screenRTTSize.getArea() == 0 ? dev->getVideoDriver()->getScreenSize() : screenRTTSize),
ClearColour(0x0), ShadowsUnsupported(false), DepthMC(0), ShadowMC(0),
AmbientColour(0x0), Use32BitDepth(use32BitDepthBuffers), UseVSM(useVSMShadows), RenderShadows(true), HDRManager(0),
VREnabled(false), VRMaterial(-1), CurrentFade(1.f)
{
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	ScreenQuad.initializeD3D11(driver);
	#endif

	WorkingPath = device->getFileSystem()->getWorkingDirectory() + "/";

	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	ScreenRTT = BaseScreenRTT = driver->addRenderTargetTexture(ScreenRTTSize, "ScreenRTT");
	ScreenQuad.rt[0] = BaseRT[0] = driver->addRenderTargetTexture(ScreenRTTSize, "ColorMapSampler");
	ScreenQuad.rt[1] = BaseRT[1] = driver->addRenderTargetTexture(ScreenRTTSize, "ScreenMapSampler");

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);
    
    CurrentShadowMaps.set_used(_IRR_MATERIAL_MAX_TEXTURES_);

	CShaderPreprocessor sPP(driver);
	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	if (driver->getDriverType() == EDT_DIRECT3D11)
		sPP.addShaderDefine("DIRECT3D_11");
	else
	#endif
	if (driver->getDriverType() == EDT_DIRECT3D9)
		sPP.addShaderDefine("DIRECT3D_9");
	else
		sPP.addShaderDefine("OPENGL_DRIVER");

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9 || driver->getDriverType() == EDT_DIRECT3D11) ? ESE_HLSL : ESE_GLSL;
	#else
	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ESE_HLSL : ESE_GLSL;
	#endif

	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	
	if (gpu && ((driver->getDriverType() == EDT_OPENGL && driver->queryFeature(EVDF_ARB_GLSL)) || ((driver->getDriverType() == EDT_DIRECT3D9)
		#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
		|| (driver->getDriverType() == EDT_DIRECT3D11)
		#endif
		&& driver->queryFeature(EVDF_PIXEL_SHADER_2_0))))
	{
		DepthMC = new DepthShaderCB(this);
		DepthMC->PassType = ECHPT_DEPTH;

		ShadowMC = new ShadowShaderCB(this);
		
		Depth = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.vertex.fx").c_str()).c_str(), "vertexMain", EVST_VS_2_0,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.fragment.fx").c_str()).c_str(), "pixelMain", EPST_PS_2_0,
			DepthMC, EMT_SOLID);

		sPP.addShaderDefine("ALPHA_ENABLED");

		DepthT = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.vertex.fx").c_str()).c_str(), "vertexMain", EVST_VS_2_0,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.fragment.fx").c_str()).c_str(), "pixelMain", EPST_PS_2_0,
			DepthMC, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
		sPP.removeShaderDefine("ALPHA_ENABLED");

		WhiteWash = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.vertex.fx").c_str()).c_str(), "vertexMain", EVST_VS_2_0,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/WhiteWashP.fragment.fx").c_str()).c_str(), "pixelMain", EPST_PS_2_0,
			DepthMC, EMT_SOLID);

		WhiteWashTRef = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.vertex.fx").c_str()).c_str(), "vertexMain", EVST_VS_2_0,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/WhiteWashP.fragment.fx").c_str()).c_str(), "pixelMain", EPST_PS_2_0,
			DepthMC, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

		WhiteWashTAdd = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.vertex.fx").c_str()).c_str(), "vertexMain", EVST_VS_2_0,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/WhiteWashPADD.fragment.fx").c_str()).c_str(), "pixelMain", EPST_PS_2_0,
			DepthMC, EMT_TRANSPARENT_ADD_COLOR);

		WhiteWashTAlpha = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Depth.vertex.fx").c_str()).c_str(), "vertexMain", EVST_VS_2_0,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/WhiteWashP.fragment.fx").c_str()).c_str(), "pixelMain", EPST_PS_2_0,
			DepthMC, EMT_TRANSPARENT_ALPHA_CHANNEL);

		const u32 sampleCounts[EFT_COUNT] = {1, 4, 8, 12, 16};

		E_VERTEX_SHADER_TYPE vertexProfile = 
			driver->queryFeature(EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;

		E_PIXEL_SHADER_TYPE pixelProfile = 
			driver->queryFeature(EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

		// Set resolution preprocessor defines.
		sPP.addShaderDefine("SCREENX", stringc(ScreenRTTSize.Width));
		sPP.addShaderDefine("SCREENY", stringc(ScreenRTTSize.Height));

		// Create screen quad shader callback.
		CScreenQuadCB* SQCB = new CScreenQuadCB(this, true);

		CScreenQuadCB* SQCBModulate = new CScreenQuadCB(this, true);
		SQCBModulate->uniformDescriptors["fade"] = CScreenQuadCB::SUniformDescriptor(&CurrentFade, 1);

		// Light modulate.
		LightModulate = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/ScreenQuad.vertex.fx").c_str()).c_str(), "vertexMain", vertexProfile,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/LightModulate.fragment.fx").c_str()).c_str(), "pixelMain", pixelProfile, SQCBModulate);

		// Simple present.
		Simple = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/ScreenQuad.vertex.fx").c_str()).c_str(), "vertexMain", vertexProfile,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/Simple.fragment.fx").c_str()).c_str(), "pixelMain", pixelProfile, SQCB,
			EMT_TRANSPARENT_ADD_COLOR);

		// VSM blur.
		VSMBlurH = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/ScreenQuad.vertex.fx").c_str()).c_str(), "vertexMain", vertexProfile,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/VSMBlur.fragment.fx").c_str()).c_str(), "pixelMain", pixelProfile, SQCB);

		sPP.addShaderDefine("VERTICAL_VSM_BLUR");

		VSMBlurV = gpu->addHighLevelShaderMaterial(
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/ScreenQuad.vertex.fx").c_str()).c_str(), "vertexMain", vertexProfile,
			sPP.ppShaderDF(sPP.getFileContent("Shaders/InternalHandler/VSMBlur.fragment.fx").c_str()).c_str(), "pixelMain", pixelProfile, SQCB);
		
		// Drop the screen quad callback.
		SQCB->drop();

		// Create default passes
		CustomDepthPassMgr = new CCustomDepthPass(driver, "CustomDepthPassManager");
		CustomDepthPassMgr->setEnabled(true);
		addCustomPass(CustomDepthPassMgr);

		CustomGeneralPass = new CCustomGeneralPass(driver, "CustomGeneralPass");
		CustomGeneralPass->setEnabled(true);
		addCustomPass(CustomGeneralPass);

		// Create effects
		HDRManager = new CHDRManager(this);
	}
	else {
		Depth = EMT_SOLID;
		DepthT = EMT_SOLID;
		WhiteWash = EMT_SOLID;
		WhiteWashTRef = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		WhiteWashTAdd = EMT_TRANSPARENT_ADD_COLOR;
		WhiteWashTAlpha = EMT_TRANSPARENT_ALPHA_CHANNEL;
		Simple = EMT_SOLID;

		device->getLogger()->log("CP3DHandler: Shader effects not supported on this system.");
		ShadowsUnsupported = true;
	}

	ViewPort = driver->getViewPort();
}

CCP3DHandler::~CCP3DHandler() {
	if(ScreenRTT)
		driver->removeTexture(ScreenRTT);

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);
}

void CCP3DHandler::setScreenRenderTargetResolution(const dimension2du& resolution) {
	driver->setRenderTarget(0, true, true);

	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	if (ScreenRTT)
		driver->removeTexture(ScreenRTT);

	ScreenRTT = BaseScreenRTT = driver->addRenderTargetTexture(resolution);

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	ScreenQuad.rt[0] = BaseRT[0] = driver->addRenderTargetTexture(resolution);

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);

	ScreenQuad.rt[1] = BaseRT[1] = driver->addRenderTargetTexture(resolution);

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	ScreenRTTSize = resolution;
}

void CCP3DHandler::addShadowToNode(ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode,
								   s32 depthMaterial, s32 shadowsMaterial, ICP3DHandlerCustomCallback *customCallback) {
	SShadowNode snode = { node, shadowMode, filterType, depthMaterial, shadowsMaterial, customCallback };
	ShadowNodeArray.push_back(snode);
}

E_SHADOW_MODE CCP3DHandler::getShadowModeForNode(ISceneNode *node) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node)
			return ShadowNodeArray[i].shadowMode;
	}

	return ESM_NO_SHADOW;
}
void CCP3DHandler::setShadowModeForNode(ISceneNode *node, E_SHADOW_MODE shadowMode) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			ShadowNodeArray[i].shadowMode = shadowMode;
			break;
		}
	}
}

E_FILTER_TYPE CCP3DHandler::getFilterTypeForNode(ISceneNode *node) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node)
			return ShadowNodeArray[i].filterType;
	}

	return EFT_NONE;
}

void CCP3DHandler::setFilterTypeForNode(ISceneNode *node, E_FILTER_TYPE filterType) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			ShadowNodeArray[i].filterType = filterType;
			break;
		}
	}
}

bool CCP3DHandler::isNodeShadowed(ISceneNode *node) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node)
			return true;
	}

	return false;
}

u32 CCP3DHandler::addShadowLight(SShadowLight &shadowLight) {
	 LightList.push_back(shadowLight);
	 return LightList.size() - 1;
}

bool CCP3DHandler::removeShadowLight(const u32 index) {
	if (index >= 0 && index < LightList.size())
		LightList.erase(index);
	else
		return false;

	return true;
}

void CCP3DHandler::clear() {
	ShadowNodeArray.clear();
	LightList.clear();

	for (u32 i = 0; i < CustomPasses.size(); i++)
		CustomPasses[i]->SceneNodes.clear();
	CustomGeneralPass->setVolumetricLightScatteringNode(0);
}

s32 CCP3DHandler::getShadowMaterialType(const u32 &lightsCount, const E_FILTER_TYPE &filterType, const bool &useRoundedSpotLight) {
	SShadowMapType *shadowType = 0;

	map<u32, array<SShadowMapType>>::Node *lightsShadowMapNode = ShadowsMap.find(lightsCount);
	if (lightsShadowMapNode) {
		shadowType = &lightsShadowMapNode->getValue()[filterType];

		if (shadowType->ShadowType != -1 && shadowType->ShadowRoundedSpotType != -1)
			return useRoundedSpotLight ? shadowType->ShadowRoundedSpotType : shadowType->ShadowType;
	}

	/// Doesn't exists, create it
	CShaderPreprocessor sPP(driver);

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	if (driver->getDriverType() == EDT_DIRECT3D11)
		sPP.addShaderDefine("DIRECT3D_11");
	else
	#endif

	if (driver->getDriverType() == EDT_DIRECT3D9)
		sPP.addShaderDefine("DIRECT3D_9");
	else
		sPP.addShaderDefine("OPENGL_DRIVER");

	if (UseVSM)
		sPP.addShaderDefine("VSM");

	E_VERTEX_SHADER_TYPE vertexProfile = driver->queryFeature(EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	E_PIXEL_SHADER_TYPE pixelProfile = driver->queryFeature(EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	sPP.addShaderDefine("SAMPLE_AMOUNT", stringc(filterType + 1));
	sPP.addShaderDefine("LIGHTS_COUNT", stringc(lightsCount));

	#ifdef _DEBUG
	stringc v = sPP.ppShaderDF(sPP.getFileContent(WorkingPath + "Shaders/InternalHandler/ShadowPass.vertex.fx").c_str()).c_str();
	stringc p = sPP.ppShaderDF(sPP.getFileContent(WorkingPath + "Shaders/InternalHandler/ShadowPass.fragment.fx").c_str()).c_str();
	#endif

	IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();

	s32 shadowMaterialType = gpu->addHighLevelShaderMaterial(
		sPP.ppShaderDF(sPP.getFileContent(WorkingPath + "Shaders/InternalHandler/ShadowPass.vertex.fx").c_str()).c_str(), "vertexMain", vertexProfile,
		sPP.ppShaderDF(sPP.getFileContent(WorkingPath + "Shaders/InternalHandler/ShadowPass.fragment.fx").c_str()).c_str(), "pixelMain", pixelProfile,
		ShadowMC, EMT_SOLID);

	sPP.addShaderDefine("ROUND_SPOTLIGHTS");

	s32 shadowRoundedSpotMaterialType = gpu->addHighLevelShaderMaterial(
		sPP.ppShaderDF(sPP.getFileContent(WorkingPath + "Shaders/InternalHandler/ShadowPass.vertex.fx").c_str()).c_str(), "vertexMain", vertexProfile,
		sPP.ppShaderDF(sPP.getFileContent(WorkingPath + "Shaders/InternalHandler/ShadowPass.fragment.fx").c_str()).c_str(), "pixelMain", pixelProfile,
		ShadowMC, EMT_SOLID);
	sPP.removeShaderDefine("ROUND_SPOTLIGHTS");

	/// Save it for cache purpose
	if (lightsShadowMapNode)
		lightsShadowMapNode->getValue()[filterType] = SShadowMapType(shadowMaterialType, shadowRoundedSpotMaterialType);
	else {
		array<SShadowMapType> newArray;
		for (u32 i = 0; i < EFT_COUNT; i++)
			newArray.push_back(SShadowMapType(-1, -1));

		newArray[filterType] = SShadowMapType(shadowMaterialType, shadowRoundedSpotMaterialType);
		ShadowsMap.insert(lightsCount, newArray);
	}

	return useRoundedSpotLight ? shadowRoundedSpotMaterialType : shadowMaterialType;
}

void CCP3DHandler::update(ITexture *outputTarget, SHandlerRenderTargets *textures) {
	if(ShadowsUnsupported || smgr->getActiveCamera() == 0)
		return;

	if (textures) {
		ScreenRTT = textures->ScreenRTT;
		ScreenQuad.rt[0] = textures->ColorMapSampler;
		ScreenQuad.rt[1] = textures->ScreenMapSampler;
	}
	else {
		ScreenRTT = BaseScreenRTT;
		ScreenQuad.rt[0] = BaseRT[0];
		ScreenQuad.rt[1] = BaseRT[1];
	}
	
	if (!ShadowNodeArray.empty() && !LightList.empty() && RenderShadows) {
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, AmbientColour);

		ICameraSceneNode* activeCam = smgr->getActiveCamera();
		activeCam->OnAnimate(device->getTimer()->getTime());
		activeCam->OnRegisterSceneNode();
		activeCam->render();

		const u32 ShadowNodeArraySize = ShadowNodeArray.size();
		const u32 LightListSize = LightList.size();

		// Render all dept maps used by shadow mapping materials
		for(u32 l = 0;l < LightListSize;++l) {

			if (LightList[l].LightScenenode->getParent() != smgr->getRootSceneNode()) {
				LightList[l].LightScenenode->updateAbsolutePosition();
				if (LightList[l].Pos != LightList[l].LightScenenode->getAbsolutePosition())
					LightList[l].setPosition(LightList[l].LightScenenode->getAbsolutePosition());
			}
			else {
				if (LightList[l].Pos != LightList[l].LightScenenode->getPosition())
					LightList[l].setPosition(LightList[l].LightScenenode->getPosition());
			}

			// Configure default depth material
			DepthMC->DepthCallback->FarLink = LightList[l].FarPlane;

			driver->setTransform(ETS_VIEW, LightList[l].getViewMatrix());
			driver->setTransform(ETS_PROJECTION, LightList[l].getProjectionMatrix());
			
			ITexture* currentShadowMapTexture = getShadowMapTexture(LightList[l].getShadowMapResolution(), false, l);

			// Recalculate the shadow map if the shadow light must be recalculated or auto recalculated
			if (LightList[l].mustRecalculate() || LightList[l].mustAutoRecalculate()) {
				driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));
				for(u32 i = 0;i < ShadowNodeArraySize;++i) {
					if(ShadowNodeArray[i].shadowMode == ESM_RECEIVE || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
						continue;

					/// Configure custom depth material if exists
					if (ShadowNodeArray[i].customCallback && ShadowNodeArray[i].customCallback->DepthCallback) {
						ShadowNodeArray[i].customCallback->DepthCallback->FarLink = LightList[l].FarPlane;
						ShadowNodeArray[i].customCallback->PassType = ECHPT_DEPTH;
					}

					const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
					array<s32> BufferMaterialList(CurrentMaterialCount);
					BufferMaterialList.set_used(0);

					for(u32 m = 0;m < CurrentMaterialCount;++m) {
						BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);

						if (ShadowNodeArray[i].depthMaterial >= 0)
							ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)ShadowNodeArray[i].depthMaterial;
						else
							ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)(BufferMaterialList[m] == EMT_TRANSPARENT_ALPHA_CHANNEL_REF ? DepthT : Depth);
					}

					ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
					ShadowNodeArray[i].node->render();

					const u32 BufferMaterialListSize = BufferMaterialList.size();
					for(u32 m = 0;m < BufferMaterialListSize;++m)
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
				}

				// Blur the shadow map texture if we're using VSM filtering.
				if(UseVSM) {
					ITexture* currentSecondaryShadowMap = getShadowMapTexture(LightList[l].getShadowMapResolution(), true, l);

					driver->setRenderTarget(currentSecondaryShadowMap, true, true, SColor(0xffffffff));
					ScreenQuad.getMaterial().setTexture(0, currentShadowMapTexture);
					ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurH;
				
					ScreenQuad.render(driver);

					driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));
					ScreenQuad.getMaterial().setTexture(0, currentSecondaryShadowMap);
					ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)VSMBlurV;
				
					ScreenQuad.render(driver);
				}

				LightList[l].setMustRecalculate(false);
			}
            
            CurrentShadowMaps[l] = currentShadowMapTexture;
            
			// configure default shadows callback
            ShadowMC->ShadowsCallback->LightColour[l] = LightList[l].DiffuseColor;
			ShadowMC->ShadowsCallback->LightLink[l].set(LightList[l].Pos);
			ShadowMC->ShadowsCallback->FarLink[l] = LightList[l].FarPlane;
            ShadowMC->ShadowsCallback->ViewLink[l].setM(LightList[l].ViewMat.pointer());
            ShadowMC->ShadowsCallback->ProjLink[l].setM(LightList[l].ProjMat.pointer());
			ShadowMC->ShadowsCallback->MapRes[l] = (f32)LightList[l].MapRes;
            ShadowMC->ShadowsCallback->LightsCount = LightListSize;
			ShadowMC->PassType = ECHPT_SHADOWS;
            
            for(u32 i = 0;i < ShadowNodeArraySize;++i) {
                if(ShadowNodeArray[i].shadowMode == ESM_CAST || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
                    continue;
                
                // Configure custom shadows callback & set pass type
                if (ShadowNodeArray[i].customCallback && ShadowNodeArray[i].customCallback->ShadowsCallback) {
                    ShadowNodeArray[i].customCallback->ShadowsCallback->LightColour[l] = LightList[l].DiffuseColor;
                    ShadowNodeArray[i].customCallback->ShadowsCallback->LightLink[l] = LightList[l].Pos;
                    ShadowNodeArray[i].customCallback->ShadowsCallback->FarLink[l] = LightList[l].FarPlane;
                    ShadowNodeArray[i].customCallback->ShadowsCallback->ViewLink[l] = LightList[l].ViewMat;
                    ShadowNodeArray[i].customCallback->ShadowsCallback->ProjLink[l] = LightList[l].ProjMat;
                    ShadowNodeArray[i].customCallback->ShadowsCallback->MapRes[l] = (f32)LightList[l].MapRes;
                    ShadowNodeArray[i].customCallback->PassType = ECHPT_SHADOWS;
                }
            }
        }
        
        driver->setRenderTarget(ScreenQuad.rt[1], true, true, SColor(0xffffffff));
        
        driver->setTransform(ETS_VIEW, activeCam->getViewMatrix());
        driver->setTransform(ETS_PROJECTION, activeCam->getProjectionMatrix());
        
        // Render all receive nodes
        for(u32 i = 0;i < ShadowNodeArraySize; ++i) {
            if(ShadowNodeArray[i].shadowMode == ESM_CAST || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
                continue;
            
            const u32 currentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
            array<s32> bufferMaterialList(currentMaterialCount);
            array<array<ITexture*>> bufferTextureList(currentMaterialCount);
            
            const s32 shadowMaterialType = getShadowMaterialType(LightList.size(), ShadowNodeArray[i].filterType, /*LightList[l].UseRoundSpotLight*/ false);
            
            for(u32 m = 0;m < currentMaterialCount;++m) {
				bufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);

				array<ITexture*> textures;
				for (u32 t = 0; t < _IRR_MATERIAL_MAX_TEXTURES_; t++)
					textures.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(t));
				bufferTextureList.push_back(textures);

                ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)shadowMaterialType;
                
                for (u32 t = 0; t < LightListSize; t++)
                    ShadowNodeArray[i].node->getMaterial(m).setTexture(t, CurrentShadowMaps[t]);
            }
            
            ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
            ShadowNodeArray[i].node->render();
            
            for(u32 m = 0;m < currentMaterialCount;++m) {
                ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)bufferMaterialList[m];

				const array<ITexture*> textures = bufferTextureList[m];
				const u32 texturesCount = textures.size();

				for (u32 t = 0; t < texturesCount; t++)
					ShadowNodeArray[i].node->getMaterial(m).setTexture(t, textures[t]);
            }
            
            if (ShadowNodeArray[i].customCallback)
                ShadowNodeArray[i].customCallback->PassType = ECHPT_NONE;
        }
        
        driver->setRenderTarget(ScreenQuad.rt[0], false, false, SColor(0x0));
        ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
        ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;
        auto blendOperation = ScreenQuad.getMaterial().BlendOperation;
        ScreenQuad.getMaterial().BlendOperation = EBO_ADD;
        ScreenQuad.render(driver);
        ScreenQuad.getMaterial().BlendOperation = blendOperation;

		// Render all the excluded and casting-only nodes.
		for(u32 i = 0; i < ShadowNodeArraySize;++i) {
			if(ShadowNodeArray[i].shadowMode != ESM_CAST && ShadowNodeArray[i].shadowMode != ESM_EXCLUDE)
					continue;

			const u32 currentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
			array<s32> bufferMaterialList(currentMaterialCount);
			bufferMaterialList.set_used(0);
			
			for(u32 m = 0;m < currentMaterialCount;++m) {
				bufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
			
				switch(bufferMaterialList[m]) {
					case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWashTRef;
						break;
					case EMT_TRANSPARENT_ADD_COLOR:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWashTAdd;
						break;
					case EMT_TRANSPARENT_ALPHA_CHANNEL:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWashTAlpha;
						break;
					default:
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)WhiteWash;
						break;
				}
			}

			ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
			ShadowNodeArray[i].node->render();

			for(u32 m = 0; m < currentMaterialCount;++m)
				ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)bufferMaterialList[m];
		}
	}
	else {
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, SColor(0xffffffff));
	}
	
	driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
	smgr->drawAll();

	const u32 PostProcessingRoutinesSize = PostProcessingRoutines.size();
	const u32 CustomPassesSize = CustomPasses.size();

	driver->setRenderTarget(PostProcessingRoutinesSize || HDRManager->isEnabled() ? ScreenRTT : outputTarget, true, true, SColor(0x0));

	ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
	ScreenQuad.getMaterial().setTexture(1, ScreenQuad.rt[0]);

	ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)LightModulate;
	if (!PostProcessingRoutinesSize && !HDRManager->isEnabled())
		driver->setViewPort(ViewPort);
	ScreenQuad.render(driver);

	// Perform custom passes after rendering, to ensure animations stay up to date
	for (u32 i = 0; i < CustomPassesSize; i++) {
		if (CustomPasses[i]->isEnabled()) {
			if (!CustomPasses[i]->setRenderTarget())
				continue;

			for (u32 j=0; j < CustomPasses[i]->getSceneNodes().size(); j++) {
				array<s32> BufferMaterialList(CustomPasses[i]->getSceneNodes()[j]->getMaterialCount());
				BufferMaterialList.set_used(0);
				for(u32 g = 0;g < CustomPasses[i]->getSceneNodes()[j]->getMaterialCount(); ++g)
					BufferMaterialList.push_back(CustomPasses[i]->getSceneNodes()[j]->getMaterial(g).MaterialType);

				CustomPasses[i]->CurrentSceneNode = j;

				CustomPasses[i]->onPreRender(CustomPasses[i]->getSceneNodes()[j]);
				CustomPasses[i]->getSceneNodes()[j]->setMaterialType((E_MATERIAL_TYPE)CustomPasses[i]->getMaterialType());
				CustomPasses[i]->getSceneNodes()[j]->OnAnimate(device->getTimer()->getTime());
				CustomPasses[i]->getSceneNodes()[j]->render();
				CustomPasses[i]->onPostRender(CustomPasses[i]->getSceneNodes()[j]);

				for(u32 g = 0;g < CustomPasses[i]->getSceneNodes()[j]->getMaterialCount();++g)
					CustomPasses[i]->getSceneNodes()[j]->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];
			}

			CustomPasses[i]->CurrentSceneNode = 0;
		}
	}

	driver->setRenderTarget(0, false, false);

	if (!PostProcessingRoutinesSize && !HDRManager->isEnabled())
		driver->setViewPort(ViewPort);
	
	bool alter = false;
	ITexture *lastRtt = 0;

	if(PostProcessingRoutinesSize) {

		for(u32 i = 0; i < PostProcessingRoutinesSize; i++) {
			const SPostProcessingPair &pair = PostProcessingRoutines[i];

			ScreenQuad.getMaterial().setTexture(1, ScreenRTT);
			ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)pair.materialType;

			if (i == 0 || lastRtt == 0) {
				alter = !alter;
				ScreenQuad.getMaterial().setTexture(0, i == 0 ? ScreenRTT : ScreenQuad.rt[int(!alter)]);
			}
			else if (lastRtt != 0) {
				ScreenQuad.getMaterial().setTexture(0, lastRtt);
			}

			lastRtt = 0;

			if (i == PostProcessingRoutinesSize - 1 && !HDRManager->isEnabled()) {
				driver->setRenderTarget(outputTarget);
				driver->setViewPort(ViewPort);
			}
			else {
				ITexture *rt = ScreenQuad.rt[int(alter)];
				if (pair.CustomRTT) {
					rt = pair.CustomRTT;
					lastRtt = rt;
				}

				driver->setRenderTarget(rt, true, true, ClearColour);
			}

			if (pair.renderCallback) pair.renderCallback->OnPreRender(this);
			ScreenQuad.render(driver);
			if (pair.renderCallback) pair.renderCallback->OnPostRender(this);
		}
	}

	HDRManager->render(PostProcessingRoutinesSize == 0 ? ScreenRTT : ScreenQuad.rt[int(alter)], outputTarget, ViewPort);
}

irr::video::ITexture* CCP3DHandler::getShadowMapTexture(const irr::u32 &resolution, const bool &secondary, const u32 &index) {
    core::stringc shadowMapName = core::stringc("CP3DHandler_SM_") + core::stringc(resolution);
    
    if (index != -1)
        shadowMapName += core::stringc("_") + core::stringc(index);

	if(secondary)
		shadowMapName += "_2";

	ITexture* shadowMapTexture = driver->getTexture(shadowMapName);

	if(shadowMapTexture == 0) {
		device->getLogger()->log("CP3DHandler: Please ignore previous warning, it is harmless.");

		shadowMapTexture = driver->addRenderTargetTexture(dimension2du(resolution, resolution),
			shadowMapName, Use32BitDepth ? ECF_G32R32F : ECF_G16R16F);
	}

	return shadowMapTexture;
}


irr::video::ITexture* CCP3DHandler::generateRandomVectorTexture(const irr::core::dimension2du& dimensions, const irr::core::stringc& name) {
	IImage* tmpImage = driver->createImage(irr::video::ECF_A8R8G8B8, dimensions);

	srand(device->getTimer()->getRealTime());
	
	for(u32 x = 0;x < dimensions.Width;++x) {
		for(u32 y = 0;y < dimensions.Height;++y) {
			vector3df randVec; 
			
			do {randVec = vector3df((f32)rand() / (f32)RAND_MAX, (f32)rand() / (f32)RAND_MAX, (f32)rand() / (f32)RAND_MAX);}
			while(randVec.getLengthSQ() > 1.0f);

			const SColorf randCol(randVec.X, randVec.Y, randVec.Z);
			tmpImage->setPixel(x, y, randCol.toSColor());
		}
	}

	ITexture* randTexture = driver->addTexture(name, tmpImage);
	
	tmpImage->drop();

	return randTexture;
}

} /// End namespace rendering
} /// End namespace cp3d
