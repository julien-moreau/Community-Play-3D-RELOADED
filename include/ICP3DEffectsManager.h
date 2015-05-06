#ifndef _H_ICP3D_EFFECTS_MANAGER_INCLUDED__
#define _H_ICP3D_EFFECTS_MANAGER_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class ICP3DEffectsManager {
public:

	//! Creates the SSAO pipeline effect
	//! \param create: true to create effect, false to destroy effect
	virtual void createSSAOEffect(bool create) = 0;

	//! Returns if the SSAO pipeline effect was created
	//! \return if the SSAO pipeline effect was created
	virtual bool isSSAOCreated() = 0;

};

} /// End rendering namespace
} /// End cp3d namespace

#endif