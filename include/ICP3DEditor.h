#ifndef _H_ICP3D_EDITOR_INCLUDED__
#define _H_ICP3D_EDITOR_INCLUDED__

namespace cp3d {

namespace ui {
	class ICP3DGUIManager;
}

namespace rendering {
	class ICP3DRenderingEngine;
}

class ICP3DEditor {
public:

	//! Runs the editor. Should be called only one time to run the editor
	//! after cp3d::createEditor()
	virtual void runEditor() = 0;

	//! Returns the GUI manager
	//! \return: the gui manager instance
	virtual ui::ICP3DGUIManager *getGUIManager() = 0;

	//! Returns the rendering engine
	//! \return: the CP3D rendering engine
	virtual rendering::ICP3DRenderingEngine *getRenderingEngine() = 0;

};

} /// End cp3d namespace

#endif
