#ifndef __H_HANDLER_INCLUDED__
#define __H_HANDLER_INCLUDED__

#include <irrlicht.h>
#include "CShaderPre.h"
#include <CScreenQuad.h>

#include "../CustomPasses/CCustomDepthPass.h"
#include "../CustomPasses/CCustomGeneralPass.h"

#include "../HDR/CCP3DHDRManager.h"

#include <ICP3DHandler.h>
#include <ICP3DHandlerCallbacks.h>

namespace cp3d {
namespace rendering {

class DepthShaderCB;
class ShadowShaderCB;
class CScreenQuadCB;

class CCP3DHandler : public ICP3DHandler {
public:

	/// Constructor, destructor & update
	CCP3DHandler(irr::IrrlichtDevice* irrlichtDevice, 
		const irr::core::dimension2du& screenRTTSize = irr::core::dimension2du(0, 0),
		const bool useVSMShadows = false, const bool useRoundSpotLights = false,
		const bool use32BitDepthBuffers = false);
	
	~CCP3DHandler();

	void update(irr::video::ITexture* outputTarget = 0, SHandlerRenderTargets *textures = 0);

	/// Shadows
	irr::u32 addShadowLight(SShadowLight& shadowLight);
	bool removeShadowLight(const irr::u32 index);
	SShadowLight &getShadowLight(const irr::u32 index) { return LightList[index]; }
	SShadowLight *getShadowLightPtr(const irr::u32 index) { return &LightList[index]; }
	const irr::u32 getShadowLightCount() const {
		return LightList.size();
	}

	irr::video::ITexture* getShadowMapTexture(const irr::u32 &resolution, const bool &secondary = false, const irr::u32 &index = -1);
	void removeShadowFromNode(irr::scene::ISceneNode* node) {
		for (irr::u32 i=0; i < ShadowNodeArray.size(); i++) {
			if (ShadowNodeArray[i].node == node) {
				ShadowNodeArray.erase(i);
				break;
			}
		}
	}
	void excludeNodeFromLightingCalculations(irr::scene::ISceneNode* node) {
		SShadowNode tmpShadowNode = {node, ESM_EXCLUDE, EFT_NONE};
		ShadowNodeArray.push_back(tmpShadowNode);
	}
	void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE, E_SHADOW_MODE shadowMode = ESM_BOTH,
						 irr::s32 depthMaterial = -1, irr::s32 shadowsMaterial = -1,
						 ICP3DHandlerCustomCallback *customCallback = 0);
	bool isNodeShadowed(irr::scene::ISceneNode *node);

	E_SHADOW_MODE getShadowModeForNode(irr::scene::ISceneNode *node);
	E_FILTER_TYPE getFilterTypeForNode(irr::scene::ISceneNode *node);

	void setShadowModeForNode(irr::scene::ISceneNode *node, E_SHADOW_MODE shadowMode);
	void setFilterTypeForNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType);

	void setUseVSM(bool use) { UseVSM = use; }

	void setRenderShadows(const bool render) { RenderShadows = render; }
	const bool isRenderingShadows() { return RenderShadows; }
	
	/// Post processes
	void addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0);
	void setPostProcessingRenderCallback(irr::s32 materialType, IPostProcessingRenderCallback* callback);
	void setPostProcessingRenderCallback(const irr::s32 &materialType,
		std::function<void(ICP3DHandler *handler)> OnPreRender = [](ICP3DHandler *handler) {},
		std::function<void(ICP3DHandler *handler)> OnPostRender = [](ICP3DHandler *handler) {});
	bool removePostProcessingEffect(irr::s32 materialType, const bool deleteCallback = true);
	irr::u32 replacePostProcessAtIndex(irr::s32 index, const irr::core::stringc &filename, IPostProcessingRenderCallback *callback = 0, irr::video::ITexture *customRTT = 0);
	irr::u32 addPostProcessingEffectFromFile(const irr::core::stringc &filename, IPostProcessingRenderCallback *callback = 0);
	irr::u32 addPostProcessingEffectFromString(const irr::core::stringc &shader, IPostProcessingRenderCallback *callback = 0);
	void setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, const irr::f32* data, const irr::u32 count);

	CScreenQuad& getScreenQuad() {
		return ScreenQuad;
	}
	CScreenQuad *getScreenQuadPtr() {
		return &ScreenQuad;
	}
	void setPostProcessingUserTexture(irr::video::ITexture* userTexture) {
		ScreenQuad.getMaterial().setTexture(3, userTexture);
	}
	void setPostProcessingTextureAtIndex(const irr::u32 &index, irr::video::ITexture *texture) {
		ScreenQuad.getMaterial().setTexture(index, texture);
	}
	const irr::s32 getPostProcessID(const irr::s32 &id);
	const irr::s32 getPostProcessIDByName(const irr::core::stringc &name);
	void setPostProcessActivated(const irr::s32 &id, const bool activated);
	bool isPostProcessActivated(const irr::s32 &id);
	const irr::u32 getPostProcessingRoutineSize() const {
		return PostProcessingRoutines.size();
	}
	const irr::core::stringc getPostProcessingRoutineName(const irr::s32 &materialType);
	IPostProcessingRenderCallback *getPostProcessingCallback(irr::s32 materialType);

	void setPostProcessCustomRTT(const irr::s32 &materialType, const irr::core::dimension2du &size, const irr::core::stringc &name, const irr::video::ECOLOR_FORMAT format = irr::video::ECF_A8R8G8B8);
	void removePostProcessCustomRTT(const irr::s32 &materialType);
	irr::video::ITexture *getPostProcessCustomRTT(const irr::s32 &materialType);

	void enableVirtualReality(const bool &enable);
	bool isVirtualRealityEnabled() { return VREnabled; }

	/// Rendering
	CHDRManager *getHDRManager() {
		return HDRManager;
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
	void setViewPort(const irr::core::rect<irr::s32> viewPort) { ViewPort = viewPort; }
	irr::core::rect<irr::s32> getViewPort() { return ViewPort; }
	irr::video::IVideoDriver *getVideoDriver() { return driver; }
	irr::core::dimension2du getScreenRTTSize() { return ScreenRTTSize; }
	void clear();

	/// Custom Passes
	void addCustomPass(ICP3DCustomPass *pass) {
		CustomPasses.push_back(pass);
	}

	void removeCustomPass(ICP3DCustomPass *pass) {
		irr::s32 index = CustomPasses.binary_search(pass);
		if (index != -1)
			CustomPasses.erase(index);
	}

	const irr::u32 getCustomPassCount() const {
		return CustomPasses.size();
	}

	ICP3DCustomPass *getCustomPass(const irr::u32 index) {
		return CustomPasses[index];
	}

	CCustomDepthPass *getDepthPassManager() {
		return CustomDepthPassMgr;
	}

	CCustomGeneralPass *getGeneralPassManager() {
		return CustomGeneralPass;
	}

	void setCurrentFade(const irr::f32 &fade) { CurrentFade = fade; }

private:

	struct SShadowNode {
		bool operator < (const SShadowNode& other) const {
			return node < other.node;
		}

		irr::scene::ISceneNode* node;
		E_SHADOW_MODE shadowMode;
		E_FILTER_TYPE filterType;

		irr::s32 depthMaterial;
		irr::s32 shadowsMaterial;
		ICP3DHandlerCustomCallback *customCallback;
	};

	struct SPostProcessingPair {
		SPostProcessingPair(const irr::s32 materialTypeIn, CScreenQuadCB* callbackIn, IPostProcessingRenderCallback* renderCallbackIn = 0)
			: materialType(materialTypeIn), callback(callbackIn), renderCallback(renderCallbackIn), CustomRTT(0)
		{ }

		bool operator < (const SPostProcessingPair& other) const {
			return materialType < other.materialType;
		}

		irr::s32 materialType;
		IPostProcessingRenderCallback* renderCallback;
		CScreenQuadCB* callback;
		irr::video::ITexture *CustomRTT;
		irr::core::stringc path;
	};

	SPostProcessingPair obtainScreenQuadMaterial(const irr::core::stringc& data, 
		irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
		bool fromFile = true);

	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::core::dimension2du mapRes;
	irr::core::stringc WorkingPath; // To ensure shaders will load correctly

	/// Default custom passes
	CCustomGeneralPass *CustomGeneralPass;

	/// Effects
	CHDRManager *HDRManager;
	
	/// Depth Pass
	CCustomDepthPass *CustomDepthPassMgr;
	irr::s32 Depth;
	irr::s32 DepthT;

	/// Shadows v2
	struct SShadowMapType {
	public:
		SShadowMapType(irr::s32 shadowType = -1, irr::s32 shadowRoundedSpot = -1)
			: ShadowType(shadowType), ShadowRoundedSpotType(shadowRoundedSpot)
		{ }

		irr::s32 ShadowType;
		irr::s32 ShadowRoundedSpotType;
	};

	irr::core::array<irr::video::ITexture *> CurrentShadowMaps;
	irr::core::map<irr::u32, irr::core::array<SShadowMapType>> ShadowsMap;
	irr::s32 GetShadowMaterialType(const irr::u32 &lightsCount, const E_FILTER_TYPE &filterType, const bool &useRoundedSpotLight);

	/// Post-processes
	irr::s32 LightModulate;
	irr::s32 Simple;
	irr::s32 WhiteWash;
	irr::s32 WhiteWashTRef;
	irr::s32 WhiteWashTAdd;
	irr::s32 WhiteWashTAlpha;
	irr::s32 VSMBlurH;
	irr::s32 VSMBlurV;

	irr::s32 VRMaterial;

	irr::core::array<ICP3DCustomPass *> CustomPasses;
	
	DepthShaderCB* DepthMC;
	ShadowShaderCB* ShadowMC;

	irr::video::ITexture *ScreenRTT, *BaseScreenRTT;
	irr::video::ITexture *BaseRT[2];

	irr::core::array<SPostProcessingPair> PostProcessingRoutines;
	irr::core::array<SShadowLight> LightList;
	irr::core::array<SShadowNode> ShadowNodeArray;

	irr::core::dimension2du ScreenRTTSize;
	irr::video::SColor ClearColour;
	irr::video::SColor AmbientColour;
	CScreenQuad ScreenQuad;

	irr::core::rect<irr::s32> ViewPort;

	irr::f32 CurrentFade;

	bool ShadowsUnsupported;
	bool Use32BitDepth;
	bool UseVSM;
	bool RenderShadows;
	bool VREnabled;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
