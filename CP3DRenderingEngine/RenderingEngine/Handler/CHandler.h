#ifndef __H_HANDLER_INCLUDED__
#define __H_HANDLER_INCLUDED__

#include <irrlicht.h>
#include "CShaderPre.h"
#include <CScreenQuad.h>

#include "../CustomPasses/CCustomDepthPass.h"
#include "../CustomPasses/CCustomGeneralPass.h"

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
	irr::u32 addShadowLight(SShadowLight& shadowLight);
	bool removeShadowLight(const irr::u32 index);
	SShadowLight &getShadowLight(const irr::u32 index) { return LightList[index]; }
	SShadowLight *getShadowLightPtr(const irr::u32 index) { return &LightList[index]; }
	const irr::u32 getShadowLightCount() const {
		return LightList.size();
	}

	irr::video::ITexture* getShadowMapTexture(const irr::u32 resolution, const bool secondary = false);
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
	void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE,
						 E_SHADOW_MODE shadowMode = ESM_BOTH, irr::s32 depthMaterial = -1);
	bool isNodeShadowed(irr::scene::ISceneNode *node);

	E_SHADOW_MODE getShadowModeForNode(irr::scene::ISceneNode *node);
	E_FILTER_TYPE getFilterTypeForNode(irr::scene::ISceneNode *node);

	void setShadowModeForNode(irr::scene::ISceneNode *node, E_SHADOW_MODE shadowMode);
	void setFilterTypeForNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType);

	void setUseVSM(bool use) { useVSM = use; }

	/// Depth pass
	CCustomDepthPass *getDepthPassManager() { return CustomDepthPassMgr; }
	
	/// Post processes
	void addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0);
	void setPostProcessingRenderCallback(irr::s32 materialType, IPostProcessingRenderCallback* callback) {
		irr::s32 i = getPostProcessID(materialType);

		if(i != -1) {
			if(PostProcessingRoutines[i].renderCallback)
				delete PostProcessingRoutines[i].renderCallback;

			PostProcessingRoutines[i].renderCallback = callback;
		}
	}
	void setPostProcessingRenderCallback(const irr::s32 &materialType,
		std::function<void(ICP3DHandler *handler)> OnPreRender = [&](ICP3DHandler *handler) {},
		std::function<void(ICP3DHandler *handler)> OnPostRender = [&](ICP3DHandler *handler) {});
	bool removePostProcessingEffect(irr::s32 materialType) {
		irr::s32 i = getPostProcessID(materialType);

		if(i != -1) {
			if(PostProcessingRoutines[i].renderCallback)
				delete PostProcessingRoutines[i].renderCallback;

			PostProcessingRoutines.erase(i);
			return true;
		}
		return false;
	}
	irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc &filename, IPostProcessingRenderCallback *callback = 0);
	irr::s32 addPostProcessingEffectFromString(const irr::core::stringc &shader, IPostProcessingRenderCallback *callback = 0);
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
	const irr::u32 getPostProcessID(const irr::s32 &id);
	void setPostProcessActivated(const irr::s32 &id, const bool activated);
	bool isPostProcessActivated(const irr::s32 &id);
	const irr::u32 getPostProcessingRoutineSize() const {
		return PostProcessingRoutines.size();
	}
	const irr::core::stringc getPostProcessingRoutineName(const irr::s32 &materialType);

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

	ICP3DCustomPass *getGeneralPassManager() {
		return CustomGeneralPass;
	}

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
	};

	struct SPostProcessingPair {
		SPostProcessingPair(const irr::s32 materialTypeIn, ScreenQuadCB* callbackIn, IPostProcessingRenderCallback* renderCallbackIn = 0)
			: materialType(materialTypeIn), callback(callbackIn), renderCallback(renderCallbackIn), activated(true)
		{ }

		bool operator < (const SPostProcessingPair& other) const {
			return materialType < other.materialType;
		}

		ScreenQuadCB* callback;
		IPostProcessingRenderCallback* renderCallback;
		irr::s32 materialType;
		bool activated;
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
	
	/// Depth Pass
	CCustomDepthPass *CustomDepthPassMgr;
	irr::s32 Depth;
	irr::s32 DepthT;

	irr::s32 Shadow[EFT_COUNT];
	irr::s32 ShadowRoundedSpot[EFT_COUNT];
	irr::s32 LightModulate;
	irr::s32 Simple;
	irr::s32 WhiteWash;
	irr::s32 WhiteWashTRef;
	irr::s32 WhiteWashTAdd;
	irr::s32 WhiteWashTAlpha;
	irr::s32 VSMBlurH;
	irr::s32 VSMBlurV;
	irr::core::array<ICP3DCustomPass *> CustomPasses;
	
	DepthShaderCB* DepthMC;
	ShadowShaderCB* ShadowMC;

	irr::video::ITexture* ScreenRTT;

	irr::core::array<SPostProcessingPair> PostProcessingRoutines;
	irr::core::array<SShadowLight> LightList;
	irr::core::array<SShadowNode> ShadowNodeArray;
	irr::core::array<irr::scene::ISceneNode*> DepthPassArray;

	irr::core::dimension2du ScreenRTTSize;
	irr::video::SColor ClearColour;
	irr::video::SColor AmbientColour;
	CScreenQuad ScreenQuad;

	irr::core::rect<irr::s32> ViewPort;

	bool shadowsUnsupported;
	bool use32BitDepth;
	bool useVSM;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
