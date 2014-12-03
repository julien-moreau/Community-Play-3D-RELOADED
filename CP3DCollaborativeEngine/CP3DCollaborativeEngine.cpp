
#include "CP3DCollaborativeEngine.h"

/// Create rendering engine
namespace cp3d {
	CP3DR_LIB_API data::ICP3DCollaborativeEngine *createCollaborativeEngine(const char *documentPath) {
		return new data::CCP3DRenderingEngine(documentPath);
	}
}

/// Rendering engine class
namespace cp3d {
namespace data {

CCP3DRenderingEngine::CCP3DRenderingEngine(irr::core::stringc documentPath) : DocumentPath(documentPath),
	DocumentMasterPtr(0), DocumentPtr(0), SlotPtr(0)
{

}

CCP3DRenderingEngine::~CCP3DRenderingEngine() {

}

void CCP3DRenderingEngine::document_changed(CCP3DRenderingEngineRoot &root) {

}

void CCP3DRenderingEngine::signal(ohm::flip::Object *obj_ptr, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data) {

}

} /// End namespace data
} /// End namespace cp3d
