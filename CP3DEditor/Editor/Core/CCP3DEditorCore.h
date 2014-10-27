#ifndef __H_C_CP3D_EDITOR_CORE_INCLUDED__
#define __H_C_CP3D_EDITOR_CORE_INCLUDED__

#include <irrlicht.h>
#include <ICP3DEditor.h>
#include <CP3DRenderingEngine.h>
#include <CP3DEngine.h>

namespace cp3d {

class CCP3DContextMenu;
class CCP3DMainToolbar;
class CCP3DToolsToolbar;
class CCP3DEditionTool;
class CCP3DSceneGraph;
class CCP3DInterfaceController;

class CCP3DEditorCore : public ICP3DEditor
						#if defined (_DEBUG)
						, public irr::IEventReceiver
						#endif
{
public:

	/// Constructor & Destructor
	CCP3DEditorCore(irr::IrrlichtDevice *device);
	~CCP3DEditorCore();

	/// Methods
	void runEditor();

	/// Getters
	rendering::ICP3DRenderingEngine *getRenderingEngine() { return Rengine; }
	engine::ICP3DEngine *getEngine() { return Engine; }

	irr::IrrlichtDevice *getDevice() { return Device; }

	irr::core::stringc getProjectName() { return ProjectName; }
	void setProjectName(irr::core::stringc name);
	irr::core::stringc getProjectDirectory() { return ProjectDirectory; }
	void setProjectDirectory(irr::core::stringc directory) { ProjectDirectory = directory; }

	irr::core::stringc getWorkingDirectory() { return WorkingDirectory; }

	/// Interfaces
	CCP3DInterfaceController *getInterfaceController() { return InterfaceController; }

	CCP3DEditionTool *getEditionTool() { return EditionTool; }

	#if defined(_DEBUG)
	bool OnEvent(const irr::SEvent &event);
	#endif

private:
	/// Device
	irr::IrrlichtDevice *Device;
	irr::gui::IGUIEnvironment *Gui;

	/// Rendering
	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;

	/// Engine
	engine::ICP3DEngine *Engine;

	/// Datas
	CCP3DInterfaceController *InterfaceController;

	CCP3DContextMenu *ContextMenu;
	CCP3DMainToolbar *MainToolbar;
	CCP3DToolsToolbar *ToolsToolbar;
	CCP3DEditionTool *EditionTool;
	CCP3DSceneGraph *SceneGraph;

	irr::core::stringc ProjectName, ProjectDirectory;
	irr::core::stringc WorkingDirectory;

	/// Methods
	void createComponents();
	#if defined(_DEBUG)
	void createTestScene();
	#endif
};

} /// End namespace cp3d

#endif