#ifndef _H_ICP3D_HDR_MANAGER_INCLUDED__
#define _H_ICP3D_HDR_MANAGER_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class ICP3DHDRManager {
public:

	//! Sets the bloom / gaussian coefficient
	//! \param coeff: coefficient to set
	virtual void setGaussianCoefficient(const irr::f32 coeff) = 0;

	//! Returns the bloom / gaussian coefficient
	virtual irr::f32 getGaussianCoefficient() const = 0;

	//! Sets the bloom / gaussian mean
	//! \param mean: the gaussian mean
	virtual void setGaussianMean(const irr::f32 mean) = 0;

	//! Returns the bloom / gaussian mean
	virtual irr::f32 getGaussianMean() const = 0;

	//! Sets the bloom / gaussian standard derivation
	//! \param standDev: the standard derivation
	virtual void setGaussianStandardDerivation(const irr::f32 standDev) = 0;

	//! Returns the bloom / gaussian standard derivation
	virtual irr::f32 getGaussianStandardDerivation() const = 0;

	//! Sets the brightness threshold
	//! \param threshold: the brightness threshold
	virtual void setBrightnessThreshold(const irr::f32 threshold) = 0;

	//! Returns the brightness threshold
	virtual irr::f32 getBrightnessThreshold() const = 0;

	//! Sets the HDR exposure
	//! \param exposure: the HDR exposure
	virtual void setExposure(const irr::f32 exposure) = 0;

	//! Returns the HDR exposure
	virtual irr::f32 getExposure() const = 0;

	//! Sets the minimum luminance
	//! \param min: the minimum luminance to set
	virtual void setMinimumLuminance(const irr::f32 min) = 0;

	//! Returns the minimum luminance
	virtual irr::f32 getMinimumLuminance() const = 0;

	//! Sets the increase rate
	//! \param rate: the increase rate
	virtual void setIncreaseRate(const irr::f32 rate) = 0;

	//! Returns the increase rate
	virtual irr::f32 getIncreaseRate() const = 0;

	//! Sets the decrease rate
	//! \param rate: the decrease rate
	virtual void setDecreaseRate(const irr::f32 rate) = 0;

	//! Returns the decrease rate
	virtual irr::f32 getDecreaseRate() const = 0;
};

} /// End namespace engine
} /// End namespace cp3d

#endif
