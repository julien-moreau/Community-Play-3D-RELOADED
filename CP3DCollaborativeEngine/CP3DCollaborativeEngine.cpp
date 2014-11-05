// Il s'agit du fichier DLL principal.

#include "CP3DCollaborativeEngine.h"

/// Create rendering engine
namespace cp3d {
	CP3DR_LIB_API data::ICP3DCollaborativeEngine *createCollaborativeEngine(const char *documentPath) {
		return new data::CCP3DRenderingEngine();
	}
}

/// Rendering engine class
namespace cp3d {
namespace data {

CCP3DRenderingEngine::CCP3DRenderingEngine() {

}

CCP3DRenderingEngine::~CCP3DRenderingEngine() {

}

} /// End namespace data
} /// End namespace cp3d
