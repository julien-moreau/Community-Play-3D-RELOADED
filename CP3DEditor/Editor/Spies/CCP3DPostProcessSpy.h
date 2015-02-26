#ifndef __H_C_CP3D_POST_PROCESS_SPY_INCLUDED__
#define __H_C_CP3D_POST_PROCESS_SPY_INCLUDED__

#include <irrlicht.h>
#include <ICP3DSpy.h>

namespace cp3d {

class CCP3DEditorCore;

namespace rendering {
	class ICP3DHandler;
}

class CCP3DPostProcessSpy : public ICP3DSpy {
public:

	/// Constructor & Destructor
	CCP3DPostProcessSpy(CCP3DEditorCore *editorCore);
	~CCP3DPostProcessSpy();

	/// ICP3DSpy
	void prepare();
	void check();

private:
	// Editor
	CCP3DEditorCore *EditorCore;

	// Rendering Engine
	rendering::ICP3DHandler *Handler;

};

} /// End namespace cp3d

#endif