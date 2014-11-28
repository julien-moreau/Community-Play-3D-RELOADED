
#include "CP3DCollaborativeEngineRoot.h"

#include "ohm/flip/ClassDescription.h"

/// Rendering engine class
namespace cp3d {
namespace data {

void CCP3DRenderingEngineRoot::declare() {
	using namespace ohm::flip;
   
	ClassDescription <CCP3DRenderingEngineRoot>::use ().set_name ("CP3DCollaborativeEngine.Root");
	ClassDescription <CCP3DRenderingEngineRoot>::use ().push_var_desc (&CCP3DRenderingEngineRoot::TestValue, "TestValue");
   
	ClassDescription <CCP3DRenderingEngineRoot>::use ().enable_root ("CP3DCollaborativeEngine.Rev0");
   
	ClassDescManager::declare(ClassDescription <CCP3DRenderingEngineRoot>::use ());
}

CCP3DRenderingEngineRoot::CCP3DRenderingEngineRoot(ohm::flip::DocumentBase &document)
	: ohm::flip::Object(document), FlipTexSessionGuard(document), TestValue(document)
{

}

CCP3DRenderingEngineRoot::~CCP3DRenderingEngineRoot() {

}

} /// End namespace data
} /// End namespace cp3d
