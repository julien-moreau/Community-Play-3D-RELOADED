#ifndef __H_C_HDR_MANAGER_INCLUDED__
#define __H_C_HDR_MANAGER_INCLUDED__

#include <irrlicht.h>
#include <CScreenQuad.h>
#include <ICP3DHDRManager.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class CScreenQuadCB;

class CHDRBloom;
class CHDRTextureAdder;
class CHDRLuminance;

class CHDRLensFlare;

class CHDRManager : public ICP3DHDRManager {
public:
	/// ctor & dtor
	CHDRManager(CCP3DHandler *handler);
	~CHDRManager();

	void render(irr::video::ITexture *source, irr::video::ITexture *output, const irr::core::rect<irr::s32> &viewport);

	void setEnabled(const bool enabled) { Enabled = enabled;  }
	bool isEnabled() const { return Enabled; }

	void setGaussianCoefficient(const irr::f32 coeff);
	irr::f32 getGaussianCoefficient() const;
	void setGaussianMean(const irr::f32 mean);
	irr::f32 getGaussianMean() const;
	void setGaussianStandardDerivation(const irr::f32 standDev);
	irr::f32 getGaussianStandardDerivation() const;
	void setGaussWidth(const irr::f32 width);
	irr::f32 getGaussianWidth() const;
	void setBrightnessThreshold(const irr::f32 threshold);
	irr::f32 getBrightnessThreshold() const;

	void setLensTexture(irr::video::ITexture *texture);
	const irr::video::ITexture *getLensTexture() const;

	void setExposure(const irr::f32 exposure) { Exposure = exposure; }
	irr::f32 getExposure() const { return Exposure; }
	void setMinimumLuminance(const irr::f32 min) { MinLuminance = min; }
	irr::f32 getMinimumLuminance() const { return MinLuminance; }
	void setMaximumLuminance(const irr::f32 max) { MaxLuminance = max; }
	irr::f32 getMaximumLuminance() const { return MaxLuminance; }
	void setIncreaseRate(const irr::f32 rate) { IncreaseRate = rate; }
	irr::f32 getIncreaseRate() const { return IncreaseRate; }
	void setDecreaseRate(const irr::f32 rate) { DecreaseRate = rate; }
	irr::f32 getDecreaseRate() const { return DecreaseRate; }

	void enableLuminance(const bool enable) { LuminanceEnabled = enable; }

	void enableLensFlare(const bool enable) { LensFlareEnabled = enable; }
	bool lensFlareEnabled() { return LensFlareEnabled; }

	void setAutoUpdate(const bool autoUpdate);

private:

	/// Datas
	irr::video::IVideoDriver *Driver;
	irr::ITimer *Timer;

	CCP3DHandler *Handler;
	CScreenQuad ScreenQuad;

	irr::video::ITexture *TextureAdderRTT;
	irr::video::ITexture *HdrRTT;

	bool Enabled;

	/// Pipeline
	CHDRBloom *Bloom;
	CHDRTextureAdder *TextureAdder;
	CHDRLuminance *Luminance;
	CHDRLensFlare *LensFlare;

	/// Material
	CScreenQuadCB *Callback;
	irr::s32 MaterialType;

	/// HDR
	irr::f32 Exposure;

	irr::u32 LastTime;
	irr::f32 CurrentLuminance;
	irr::f32 MinLuminance;
	irr::f32 MaxLuminance;
	irr::f32 OutputLuminance;
	irr::f32 IncreaseRate;
	irr::f32 DecreaseRate;

	/// Enables
	bool LuminanceEnabled;
	bool LensFlareEnabled;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
