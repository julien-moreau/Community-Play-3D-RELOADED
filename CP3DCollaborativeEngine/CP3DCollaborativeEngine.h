// CP3DRenderingEngine.h

#ifndef __H_CP3D_COLLABORATIVE_ENGINE_INCLUDED__
#define __H_CP3D_COLLABORATIVE_ENGINE_INCLUDED__

#include <irrlicht.h>

#define CP3DR_COMPILE_COLLABORATIVE
#include <CP3DCompileConfig.h>
#include <ICP3DCollaborativeEngine.h>

#include "model/CP3DCollaborativeEngineRoot.h"
#include "ohm/flip/DocumentObserver.h"
#include "ohm/flip/DocumentClientMaster.h"
#include "ohm/flip/DocumentLocal.h"  

namespace cp3d {
namespace data {

class CCP3DRenderingEngine : public ICP3DCollaborativeEngine,
							 public ohm::flip::DocumentObserver<CCP3DRenderingEngineRoot>
{
public:

	CCP3DRenderingEngine(irr::core::stringc documentPath);
	~CCP3DRenderingEngine();

	/// ohm::flip::DocumentObserver
	virtual void document_changed (CCP3DRenderingEngineRoot &root);
	virtual void signal (ohm::flip::Object *obj_ptr, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data);

private:

	/// Datas
	irr::core::stringc DocumentPath;

	/// ohm::flip
	ohm::flip::DocumentClientMaster *DocumentMasterPtr;
	ohm::flip::DocumentClientMasterSlot *SlotPtr;
	ohm::flip::DocumentClient *DocumentPtr;

};

} /// End namespace data
} /// End namespace cp3d

#endif
