// CP3DRenderingEngine.h

#ifndef __H_CP3D_COLLABORATIVE_ENGINE_ROOT_INCLUDED__
#define __H_CP3D_COLLABORATIVE_ENGINE_ROOT_INCLUDED__

#include <irrlicht.h>

#include "ohm/flip/Object.h"
#include "ohm/flip/TxSessionGuard.h"
#include "ohm/flip/Float64.h"

namespace cp3d {
namespace data {

class CCP3DRenderingEngineRoot : public ohm::flip::Object {
public:

	/// Constructor & Destructor
	CCP3DRenderingEngineRoot(ohm::flip::DocumentBase &document);
	~CCP3DRenderingEngineRoot();

	/// ohm::flip::Object
	static void declare();

private:
	/// ohm::flip
	ohm::flip::TxSessionGuard FlipTexSessionGuard;

	ohm::flip::Float64 TestValue;
};

} /// End namespace data
} /// End namespace cp3d

#endif
