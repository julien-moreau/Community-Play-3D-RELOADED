#ifndef __H_HANDLER_INCLUDED__
#define __H_HANDLER_INCLUDED__

#include <irrlicht.h>
#include "CShaderPre.h"
#include "CScreenQuad.h"

#include "../CustomPasses/CCustomDepthPass.h"

#include <ICP3DHandler.h>

namespace cp3d {
namespace rendering {

class DepthShaderCB;
class ShadowShaderCB;
class ScreenQuadCB;

class CCP3DHandler : public ICP3DHandler {
public:

	/// Constructor, destructor & update
	CCP3DHandler(irr::IrrlichtDevice* irrlichtDevice, 
		const irr::core::dimension2du& screenRTTSize = irr::core::dimension2du(0, 0),
		const bool useVSMShadows = false, const bool useRoundSpotLights = false,
		const bool use32BitDepthBuffers = false);
	
	~CCP3DHandler();

	void update(irr::video::ITexture* outputTarget = 0);

	/// Shadows
	void addShadowLight(const SShadowLight& shadowLight) { LightList.push_back(shadowLight); }
	SShadowLight& getShadowLight(irr::u32 index) { return LightList[index]; }
	const irr::u32 getShadowLightCount() const {
		return LightList.size();
	}

	irr::video::ITexture* getShadowMapTexture(const irr::u32 resolution, const bool secondary = false);
	void removeShadowFromNode(irr::scene::ISceneNode* node) {
		SShadowNode tmpShadowNode = {node, ESM_RECEIVE, EFT_NONE};
		irr::s32 i = ShadowNodeArray.binary_search(tmpShadowNode);

		if(i != -1)
			ShadowNodeArray.erase(i);
	}
	void excludeNodeFromLightingCalculations(irr::scene::ISceneNode* node) {
		SShadowNode tmpShadowNode = {node, ESM_EXCLUDE, EFT_NONE};
		ShadowNodeArray.push_back(tmpShadowNode);
	}
	void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE, E_SHADOW_MODE shadowMode = ESM_BOTH);

	/// Depth pass
	CCustomDepthPass *getDepthPassManager() { return CustomDepthPassMgr; }
	
	/// Post processes
	void addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0);
	void setPostProcessingRenderCallback(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0) {
		SPostProcessingPair tempPair(MaterialType, 0);
		irr::s32 i = PostProcessingRoutines.binary_search(tempPair);

		if(i != -1) {
			if(PostProcessingRoutines[i].renderCallback)
				delete PostProcessingRoutines[i].renderCallback;

			PostProcessingRoutines[i].renderCallback = callback;
		}
	}
	void removePostProcessingEffect(irr::s32 MaterialType) {
		SPostProcessingPair tempPair(MaterialType, 0);
		irr::s32 i = PostProcessingRoutines.binary_search(tempPair);

		if(i != -1) {
			if(PostProcessingRoutines[i].renderCallback)
				delete PostProcessingRoutines[i].renderCallback;

			PostProcessingRoutines.erase(i);
		}
	}
	irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc &filename, IPostProcessingRenderCallback *callback = 0);
	irr::s32 addPostProcessingEffectFromString(const irr::core::stringc &shader, IPostProcessingRenderCallback *callback = 0);
	void setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, const irr::f32* data, const irr::u32 count);
	const CScreenQuad& getScreenQuad() { return ScreenQuad; }
	void setPostProcessingUserTexture(irr::video::ITexture* userTexture) {
		ScreenQuad.getMaterial().setTexture(3, userTexture);
	}
	void setPostProcessingTextureAtIndex(irr::u32 index, irr::video::ITexture *texture) {
		ScreenQuad.getMaterial().setTexture(index, texture);
	}

	/// Utils
	void setAmbientColor(irr::video::SColor ambientColour) { AmbientColour = ambientColour; }
	irr::video::SColor getAmbientColor() const { return AmbientColour; }
	irr::video::ITexture* generateRandomVectorTexture(const irr::core::dimension2du& dimensions, const irr::core::stringc& name = "randVec");
	void setScreenRenderTargetResolution(const irr::core::dimension2du& resolution);
	irr::IrrlichtDevice* getIrrlichtDevice()  {return device; }
	void setActiveSceneManager(irr::scene::ISceneManager* smgrIn) { smgr = smgrIn; }
	irr::scene::ISceneManager* getActiveSceneManager() { return smgr; }
	irr::f32 getTime() { return device->getTimer()->getTime() / 100.0f; }
	void setClearColour(irr::video::SColor ClearCol) { ClearColour = ClearCol; }

	/// Custom Passes
	void addCustomPass(ICP3DCustomPass *pass) {
		CustomPasses.push_back(pass);
	}

	void removeCustomPass(ICP3DCustomPass *pass) {
		irr::s32 index = CustomPasses.binary_search(pass);
		if (index != -1)
			CustomPasses.erase(index);
	}

private:

	struct SShadowNode {
		bool operator < (const SShadowNode& other) const {
			return node < other.node;
		}

		irr::scene::ISceneNode* node;

		E_SHADOW_MODE shadowMode;
		E_FILTER_TYPE filterType;
	};

	struct SPostProcessingPair {
		SPostProcessingPair(const irr::s32 materialTypeIn, ScreenQuadCB* callbackIn,
			IPostProcessingRenderCallback* renderCallbackIn = 0)
			: materialType(materialTypeIn), callback(callbackIn), renderCallback(renderCallbackIn) {}

		bool operator < (const SPostProcessingPair& other) const {
			return materialType < other.materialType;
		}

		ScreenQuadCB* callback;
		IPostProcessingRenderCallback* renderCallback;
		irr::s32 materialType;
	};

	SPostProcessingPair obtainScreenQuadMaterial(const irr::core::stringc& data, 
		irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
		bool fromFile = true);

	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::core::dimension2du mapRes;
	
	/// Depth Pass
	CCustomDepthPass *CustomDepthPassMgr;
	irr::s32 Depth;
	irr::s32 DepthT;

	irr::s32 Shadow[EFT_COUNT];
	irr::s32 LightModulate;
	irr::s32 Simple;
	irr::s32 WhiteWash;
	irr::s32 WhiteWashTRef;
	irr::s32 WhiteWashTAdd;
	irr::s32 WhiteWashTAlpha;
	irr::s32 VSMBlurH;
	irr::s32 VSMBlurV;
	irr::core::array<ICP3DCustomPass *> CustomPasses;
	
	DepthShaderCB* depthMC;
	ShadowShaderCB* shadowMC;

	irr::video::ITexture* ScreenRTT;

	irr::core::array<SPostProcessingPair> PostProcessingRoutines;
	irr::core::array<SShadowLight> LightList;
	irr::core::array<SShadowNode> ShadowNodeArray;
	irr::core::array<irr::scene::ISceneNode*> DepthPassArray;

	irr::core::dimension2du ScreenRTTSize;
	irr::video::SColor ClearColour;
	irr::video::SColor AmbientColour;
	CScreenQuad ScreenQuad;

	bool shadowsUnsupported;
	bool use32BitDepth;
	bool useVSM;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
