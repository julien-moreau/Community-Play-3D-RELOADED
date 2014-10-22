// CP3DEngine.h

#ifndef __H_CP3D_ENGINE_INCLUDED__
#define __H_CP3D_ENGINE_INCLUDED__

#include <CP3DRenderingEngine.h>
#include <CP3DEngine.h>

#include "Engine/Core/CCP3DEventReceiver.h"
#include "Engine/Core/CCP3DCustomUpdater.h"

namespace cp3d {
namespace engine {

class CCP3DEngine : public ICP3DEngine {
public:

	CCP3DEngine(irr::IrrlichtDevice *device);
	~CCP3DEngine();

	void runEngine();

	/// Getters
	CCP3DEventReceiver *getEventReceiver();
	CCP3DCustomUpdater *getCustomUpdater();

	rendering::ICP3DRenderingEngine *getRenderingEngine();

	/// GUI
	const bool isDrawingGUI() const;
	void setDrawGUI(const bool draw);

private:
	/// Irrlicht
	irr::IrrlichtDevice *Device;
	irr::gui::IGUIEnvironment *Gui;

	/// Rendering
	rendering::ICP3DRenderingEngine *Rengine;
	rendering::ICP3DHandler *Handler;

	/// Datas
	CCP3DEventReceiver *EventReceiver;
	CCP3DCustomUpdater *Updater;

	bool DrawGUI;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
