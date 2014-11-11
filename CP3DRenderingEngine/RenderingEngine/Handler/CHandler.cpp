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

CCP3DHandler::CCP3DHandler(IrrlichtDevice* dev, const irr::core::dimension2du& screenRTTSize,
	const bool useVSMShadows, const bool useRoundSpotLights, const bool use32BitDepthBuffers)
: device(dev), smgr(dev->getSceneManager()), driver(dev->getVideoDriver()),
ScreenRTTSize(screenRTTSize.getArea() == 0 ? dev->getVideoDriver()->getScreenSize() : screenRTTSize),
ClearColour(0x0), shadowsUnsupported(false), DepthMC(0), ShadowMC(0),
AmbientColour(0x0), use32BitDepth(use32BitDepthBuffers), useVSM(useVSMShadows)
{
	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	ScreenRTT = driver->addRenderTargetTexture(ScreenRTTSize);
	ScreenQuad.rt[0] = driver->addRenderTargetTexture(ScreenRTTSize);
	ScreenQuad.rt[1] = driver->addRenderTargetTexture(ScreenRTTSize);

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	CShaderPreprocessor sPP(driver);

	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ESE_HLSL : ESE_GLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	
	if(gpu && ((driver->getDriverType() == EDT_OPENGL && driver->queryFeature(EVDF_ARB_GLSL)) ||
			   (driver->getDriverType() == EDT_DIRECT3D9 && driver->queryFeature(EVDF_PIXEL_SHADER_2_0))))
	{
		DepthMC = new DepthShaderCB(this);
		ShadowMC = new ShadowShaderCB(this);

		CustomDepthPassMgr = new CCustomDepthPass(driver, "CustomDepthPassManager");
		addCustomPass(CustomDepthPassMgr);

		Depth = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
			sPP.ppShader(SHADOW_PASS_1P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
			DepthMC, video::EMT_SOLID);

		DepthT = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
			sPP.ppShader(SHADOW_PASS_1PT[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
			DepthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

		WhiteWash = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
			sPP.ppShader(WHITE_WASH_P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
			DepthMC, video::EMT_SOLID);

		WhiteWashTRef = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
			sPP.ppShader(WHITE_WASH_P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
			DepthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

		WhiteWashTAdd = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
			sPP.ppShader(WHITE_WASH_P_ADD[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
			DepthMC, video::EMT_TRANSPARENT_ADD_COLOR);

		WhiteWashTAlpha = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SHADOW_PASS_1V[shaderExt]).c_str(), "vertexMain", video::EVST_VS_2_0,
			sPP.ppShader(WHITE_WASH_P[shaderExt]).c_str(), "pixelMain", video::EPST_PS_2_0,
			DepthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

		if(useVSMShadows)
			sPP.addShaderDefine("VSM");

		const u32 sampleCounts[EFT_COUNT] = {1, 4, 8, 12, 16};

		const E_VERTEX_SHADER_TYPE vertexProfile = 
			driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;

		const E_PIXEL_SHADER_TYPE pixelProfile = 
			driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

		for(u32 i = 0;i < EFT_COUNT;i++) {
			sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(sampleCounts[i]));
			Shadow[i] = gpu->addHighLevelShaderMaterial(sPP.ppShader(SHADOW_PASS_2V[shaderExt]).c_str(), "vertexMain", vertexProfile,
														sPP.ppShader(SHADOW_PASS_2P[shaderExt]).c_str(), "pixelMain", pixelProfile,
														ShadowMC, video::EMT_SOLID);

			if (useRoundSpotLights) {
				sPP.addShaderDefine("ROUND_SPOTLIGHTS");
				ShadowRoundedSpot[i] = gpu->addHighLevelShaderMaterial(sPP.ppShader(SHADOW_PASS_2V[shaderExt]).c_str(), "vertexMain", vertexProfile,
																	   sPP.ppShader(SHADOW_PASS_2P[shaderExt]).c_str(), "pixelMain", pixelProfile,
																	   ShadowMC, video::EMT_SOLID);
				sPP.removeShaderDefine("ROUND_SPOTLIGHTS");
			}
		}

		// Set resolution preprocessor defines.
		sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
		sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));	

		// Create screen quad shader callback.
		ScreenQuadCB* SQCB = new ScreenQuadCB(this, true);

		// Light modulate.
		LightModulate = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
			sPP.ppShader(LIGHT_MODULATE_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB);

		// Simple present.
		Simple = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
			sPP.ppShader(SIMPLE_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB,
			video::EMT_TRANSPARENT_ADD_COLOR);

		// VSM blur.
		VSMBlurH = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
			sPP.ppShader(VSM_BLUR_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB);

		sPP.addShaderDefine("VERTICAL_VSM_BLUR");

		VSMBlurV = gpu->addHighLevelShaderMaterial(
			sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", vertexProfile,
			sPP.ppShader(VSM_BLUR_P[shaderExt]).c_str(), "pixelMain", pixelProfile, SQCB);
		
		// Drop the screen quad callback.
		SQCB->drop();
	}
	else {
		Depth = EMT_SOLID;
		DepthT = EMT_SOLID;
		WhiteWash = EMT_SOLID;
		WhiteWashTRef = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		WhiteWashTAdd = EMT_TRANSPARENT_ADD_COLOR;
		WhiteWashTAlpha = EMT_TRANSPARENT_ALPHA_CHANNEL;
		Simple = EMT_SOLID;

		for(u32 i = 0;i < EFT_COUNT;++i)
			Shadow[i] = EMT_SOLID;

		device->getLogger()->log("CP3DHandler: Shader effects not supported on this system.");
		shadowsUnsupported = true;
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

void CCP3DHandler::setScreenRenderTargetResolution(const irr::core::dimension2du& resolution) {
	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	if(ScreenRTT)
		driver->removeTexture(ScreenRTT);

	ScreenRTT = driver->addRenderTargetTexture(resolution);

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	ScreenQuad.rt[0] = driver->addRenderTargetTexture(resolution);

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);

	ScreenQuad.rt[1] = driver->addRenderTargetTexture(resolution);

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	ScreenRTTSize = resolution;
}


void CCP3DHandler::addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback) {
	SPostProcessingPair pPair(MaterialType, 0);
	pPair.renderCallback = callback;
	PostProcessingRoutines.push_back(pPair);
}


void CCP3DHandler::addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode) {
	SShadowNode snode = {node, shadowMode, filterType};
	ShadowNodeArray.push_back(snode);
}

E_SHADOW_MODE CCP3DHandler::getShadowModeForNode(irr::scene::ISceneNode *node) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node)
			return ShadowNodeArray[i].shadowMode;
	}

	return ESM_EXCLUDE;
}
void CCP3DHandler::setShadowModeForNode(irr::scene::ISceneNode *node, E_SHADOW_MODE shadowMode) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			ShadowNodeArray[i].shadowMode = shadowMode;
			break;
		}
	}
}

E_FILTER_TYPE CCP3DHandler::getFilterTypeForNode(irr::scene::ISceneNode *node) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node)
			return ShadowNodeArray[i].filterType;
	}

	return EFT_NONE;
}

void CCP3DHandler::setFilterTypeForNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType) {
	for (u32 i=0; i < ShadowNodeArray.size(); i++) {
		if (ShadowNodeArray[i].node == node) {
			ShadowNodeArray[i].filterType = filterType;
			break;
		}
	}
}

bool CCP3DHandler::isNodeShadowed(irr::scene::ISceneNode *node) {
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

bool CCP3DHandler::removeShadowLight(const irr::u32 index) {
	if (index >= 0 && index < LightList.size())
		LightList.erase(index);
	else
		return false;

	return true;
}

void CCP3DHandler::update(irr::video::ITexture* outputTarget) {
	if(shadowsUnsupported || smgr->getActiveCamera() == 0)
		return;
	
	if(!ShadowNodeArray.empty() && !LightList.empty()) {
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, AmbientColour);

		ICameraSceneNode* activeCam = smgr->getActiveCamera();
		activeCam->OnAnimate(device->getTimer()->getTime());
		activeCam->OnRegisterSceneNode();
		activeCam->render();

		const u32 ShadowNodeArraySize = ShadowNodeArray.size();
		const u32 LightListSize = LightList.size();

		/// Render all dept maps used by shadow mapping materials
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

			DepthMC->FarLink = LightList[l].getFarValue();

			driver->setTransform(ETS_VIEW, LightList[l].getViewMatrix());
			driver->setTransform(ETS_PROJECTION, LightList[l].getProjectionMatrix());
			
			ITexture* currentShadowMapTexture = getShadowMapTexture(LightList[l].getShadowMapResolution());

			/// Recalculate the shadow map if the shadow light must be recalculated or auto recalculated
			if (LightList[l].mustRecalculate() || LightList[l].mustAutoRecalculate()) {
				driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));
				for(u32 i = 0;i < ShadowNodeArraySize;++i) {
					if(ShadowNodeArray[i].shadowMode == ESM_RECEIVE || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
						continue;

					const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
					core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
					BufferMaterialList.set_used(0);

					for(u32 m = 0;m < CurrentMaterialCount;++m) {
						BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)
							(BufferMaterialList[m] == video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF ? DepthT : Depth);
					}

					ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
					ShadowNodeArray[i].node->render();

					const u32 BufferMaterialListSize = BufferMaterialList.size();
					for(u32 m = 0;m < BufferMaterialListSize;++m)
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
				}

				/// Blur the shadow map texture if we're using VSM filtering.
				if(useVSM) {
					ITexture* currentSecondaryShadowMap = getShadowMapTexture(LightList[l].getShadowMapResolution(), true);

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

			driver->setRenderTarget(ScreenQuad.rt[1], true, true, SColor(0xffffffff));
		
			driver->setTransform(ETS_VIEW, activeCam->getViewMatrix());
			driver->setTransform(ETS_PROJECTION, activeCam->getProjectionMatrix());

			ShadowMC->LightColour = LightList[l].DiffuseColor;
			ShadowMC->LightLink = LightList[l].Pos;
			ShadowMC->FarLink = LightList[l].FarPlane;
			ShadowMC->ViewLink = LightList[l].ViewMat;
			ShadowMC->ProjLink = LightList[l].ProjMat;
			ShadowMC->MapRes = (f32)LightList[l].MapRes;

			/// Render all receive nodes
			for(u32 i = 0;i < ShadowNodeArraySize;++i) {
				if(ShadowNodeArray[i].shadowMode == ESM_CAST || ShadowNodeArray[i].shadowMode == ESM_EXCLUDE)
						continue;

				const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
				core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
				core::array<irr::video::ITexture*> BufferTextureList(CurrentMaterialCount);
				
				for(u32 m = 0;m < CurrentMaterialCount;++m) {
					BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
					BufferTextureList.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(0));
				
					ShadowNodeArray[i].node->getMaterial(m).MaterialType = (LightList[l].UseRoundSpotLight)
																		   ? (E_MATERIAL_TYPE)ShadowRoundedSpot[ShadowNodeArray[i].filterType]
																		   : (E_MATERIAL_TYPE)Shadow[ShadowNodeArray[i].filterType];
					ShadowNodeArray[i].node->getMaterial(m).setTexture(0, currentShadowMapTexture);
				}

				ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
				ShadowNodeArray[i].node->render();

				for(u32 m = 0;m < CurrentMaterialCount;++m) {
					ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
					ShadowNodeArray[i].node->getMaterial(m).setTexture(0, BufferTextureList[m]);
				}
			}

			driver->setRenderTarget(ScreenQuad.rt[0], false, false, SColor(0x0));
			ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
			ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;
			auto blendOperation = ScreenQuad.getMaterial().BlendOperation;
			ScreenQuad.getMaterial().BlendOperation = EBO_ADD;
			ScreenQuad.render(driver);
			ScreenQuad.getMaterial().BlendOperation = blendOperation;
		}

		// Render all the excluded and casting-only nodes.
		for(u32 i = 0;i < ShadowNodeArraySize;++i) {
			if(ShadowNodeArray[i].shadowMode != ESM_CAST && ShadowNodeArray[i].shadowMode != ESM_EXCLUDE)
					continue;

			const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
			core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
			BufferMaterialList.set_used(0);
			
			for(u32 m = 0;m < CurrentMaterialCount;++m) {
				BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
			
				switch(BufferMaterialList[m]) {
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

			for(u32 m = 0;m < CurrentMaterialCount;++m)
				ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
		}
	}
	else {
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, SColor(0xffffffff));
	}
	
	driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
	smgr->drawAll();

	const u32 PostProcessingRoutinesSize = PostProcessingRoutines.size();

	driver->setRenderTarget(PostProcessingRoutinesSize  ? ScreenRTT : outputTarget, true, true, SColor(0x0));

	ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
	ScreenQuad.getMaterial().setTexture(1, ScreenQuad.rt[0]);

	ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)LightModulate;
	if (!PostProcessingRoutinesSize)
		driver->setViewPort(ViewPort);
	ScreenQuad.render(driver);

	// Perform custom passes after rendering, to ensure animations stay up to date
	for (u32 i=0; i < CustomPasses.size(); i++) {
		if (CustomPasses[i]->isEnabled()) {
			if (!CustomPasses[i]->setRenderTarget())
				continue;

			for (u32 j=0; j < CustomPasses[i]->getSceneNodes().size(); j++) {
				core::array<irr::s32> BufferMaterialList(CustomPasses[i]->getSceneNodes()[j]->getMaterialCount());
				BufferMaterialList.set_used(0);
				for(u32 g = 0;g < CustomPasses[i]->getSceneNodes()[j]->getMaterialCount(); ++g)
					BufferMaterialList.push_back(CustomPasses[i]->getSceneNodes()[j]->getMaterial(g).MaterialType);

				CustomPasses[i]->onPreRender(CustomPasses[i]->getSceneNodes()[j]);
				CustomPasses[i]->getSceneNodes()[j]->setMaterialType((E_MATERIAL_TYPE)CustomPasses[i]->getMaterialType());
				CustomPasses[i]->getSceneNodes()[j]->OnAnimate(device->getTimer()->getTime());
				CustomPasses[i]->getSceneNodes()[j]->render();
				CustomPasses[i]->onPostRender(CustomPasses[i]->getSceneNodes()[j]);

				for(u32 g = 0;g < CustomPasses[i]->getSceneNodes()[j]->getMaterialCount();++g)
					CustomPasses[i]->getSceneNodes()[j]->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];
			}

		}
	}

	driver->setRenderTarget(0, false, false);
	
	if(PostProcessingRoutinesSize) {
		bool Alter = false;

		for(u32 i = 0;i < PostProcessingRoutinesSize;++i) {
			ScreenQuad.getMaterial().setTexture(1, ScreenRTT);

			ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)PostProcessingRoutines[i].materialType;

			Alter = !Alter;
			ScreenQuad.getMaterial().setTexture(0, i == 0 ? ScreenRTT : ScreenQuad.rt[int(!Alter)]);

			if (i >= PostProcessingRoutinesSize - 1) {
				driver->setViewPort(ViewPort);
				driver->setRenderTarget(outputTarget, true, true, ClearColour);
			}
			else
				driver->setRenderTarget(ScreenQuad.rt[int(Alter)], true, true, ClearColour);

			if(PostProcessingRoutines[i].renderCallback) PostProcessingRoutines[i].renderCallback->OnPreRender(this);
			ScreenQuad.render(driver);
			if(PostProcessingRoutines[i].renderCallback) PostProcessingRoutines[i].renderCallback->OnPostRender(this);
		}

	}
}


irr::video::ITexture* CCP3DHandler::getShadowMapTexture(const irr::u32 resolution, const bool secondary) {
	core::stringc shadowMapName = core::stringc("CP3DHandler_SM_") + core::stringc(resolution);

	if(secondary)
		shadowMapName += "_2";

	ITexture* shadowMapTexture = driver->getTexture(shadowMapName);

	if(shadowMapTexture == 0) {
		device->getLogger()->log("CP3DHandler: Please ignore previous warning, it is harmless.");

		shadowMapTexture = driver->addRenderTargetTexture(dimension2du(resolution, resolution),
			shadowMapName, use32BitDepth ? ECF_G32R32F : ECF_G16R16F);
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


CCP3DHandler::SPostProcessingPair CCP3DHandler::obtainScreenQuadMaterial(const irr::core::stringc& data,  irr::video::E_MATERIAL_TYPE baseMaterial, bool fromFile) {
	CShaderPreprocessor sPP(driver);

	sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
	sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));
	if (driver->getDriverType() == EDT_OPENGL)
		sPP.addShaderDefine("OPENGL_DRIVER", "1");
	else
		sPP.addShaderDefine("DIRECT3D_DRIVER", "1");
	
	video::E_VERTEX_SHADER_TYPE VertexLevel = driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	video::E_PIXEL_SHADER_TYPE PixelLevel = driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	E_SHADER_EXTENSION shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ESE_HLSL : ESE_GLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	stringc shaderString;
	if (fromFile)
		shaderString = sPP.ppShaderFF(data.c_str());
	else
		shaderString = sPP.ppShader(data.c_str());

	ScreenQuadCB* SQCB = new ScreenQuadCB(this, true);

	s32 PostMat = gpu->addHighLevelShaderMaterial(
		sPP.ppShader(SCREEN_QUAD_V[shaderExt]).c_str(), "vertexMain", VertexLevel,
		shaderString.c_str(), "pixelMain", PixelLevel,
		SQCB, baseMaterial);
	
	SPostProcessingPair pPair(PostMat, SQCB);

	SQCB->drop();

	return pPair;
}

void CCP3DHandler::setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, const f32* data, const irr::u32 count) {
	SPostProcessingPair tempPair(materialType, 0);
	s32 matIndex = PostProcessingRoutines.binary_search(tempPair);
	
	if(matIndex != -1)
		PostProcessingRoutines[matIndex].callback->uniformDescriptors[name] = ScreenQuadCB::SUniformDescriptor(data, count);
}

s32 CCP3DHandler::addPostProcessingEffectFromString(const irr::core::stringc &shader, IPostProcessingRenderCallback *callback) {
	SPostProcessingPair pPair = obtainScreenQuadMaterial(shader, EMT_SOLID, false);
	pPair.renderCallback = callback;
	PostProcessingRoutines.push_back(pPair);

	return pPair.materialType;
}

s32 CCP3DHandler::addPostProcessingEffectFromFile(const irr::core::stringc& filename, IPostProcessingRenderCallback* callback) {
	SPostProcessingPair pPair = obtainScreenQuadMaterial(filename);
	pPair.renderCallback = callback;
	PostProcessingRoutines.push_back(pPair);

	return pPair.materialType;
}

} /// End namespace rendering
} /// End namespace cp3d
