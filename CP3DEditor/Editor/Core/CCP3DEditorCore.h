#ifndef __H_C_CP3D_EDITOR_CORE_INCLUDED__
#define __H_C_CP3D_EDITOR_CORE_INCLUDED__

#include <irrlicht.h>
#include <ICP3DEditor.h>
#include <ICP3DFileSelector.h>
#include <CP3DRenderingEngine.h>
#include <CP3DEngine.h>

namespace cp3d {

class CCP3DContextMenu;
class CCP3DMainToolbar;
class CCP3DToolsToolbar;
class CCP3DEditionTool;
class CCP3DSceneGraph;
class CCP3DInterfaceController;
class CCP3DCustomView;
class CCP3DEditorTransformer;

namespace collaborative {
	class ICP3DCollaborativeEngine;
}

class CCP3DEditorCore : public ICP3DEditor, public engine::ICP3DUpdate, public irr::IEventReceiver
{
public:

	/// Constructor & Destructor
	CCP3DEditorCore(irr::IrrlichtDevice *device);
	~CCP3DEditorCore();

	/// ICP3DEditor
	void runEditor();

	ui::ICP3DFileSelector *createFileOpenDialog(irr::core::stringw name, irr::gui::IGUIElement *parent, ui::ICP3DFileSelector::E_FILESELECTOR_TYPE type);
	irr::gui::IGUIWindow *createMessageBox(irr::core::stringw title, irr::core::stringw text, irr::s32 flags, bool modal = true, irr::video::ITexture *texture = 0);

	/// ICP3DUpdate
	void OnPreUpdate();

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
	CCP3DEditorTransformer *getEditorTransformer() { return EditorTransformer; }

	CCP3DEditionTool *getEditionTool() { return EditionTool; }
	CCP3DSceneGraph *getSceneGraph() { return SceneGraph; }
	CCP3DCustomView *getCustomView() { return CustomView; }

	bool OnEvent(const irr::SEvent &event);

private:
	/// Device
	irr::IrrlichtDevice *Device;
	irr::video::IVideoDriver *Driver;
	irr::gui::IGUIEnvironment *Gui;

	/// Rendering
	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;

	/// Engine
	engine::ICP3DEngine *Engine;

	/// Datas
	CCP3DInterfaceController *InterfaceController;
	CCP3DEditorTransformer *EditorTransformer;

	CCP3DContextMenu *ContextMenu;
	CCP3DMainToolbar *MainToolbar;
	CCP3DToolsToolbar *ToolsToolbar;
	CCP3DEditionTool *EditionTool;
	CCP3DSceneGraph *SceneGraph;
	CCP3DCustomView *CustomView;

	irr::core::stringc ProjectName, ProjectDirectory;
	irr::core::stringc WorkingDirectory;

	irr::core::dimension2du ScreenSize;

	/// Methods
	void createComponents();
	void createTestScene();
};

} /// End namespace cp3d

#endif