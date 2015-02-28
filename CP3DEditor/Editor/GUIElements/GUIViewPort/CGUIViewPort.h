#ifndef __C_GUI_VIEWPORT_H_INCLUDED__
#define __C_GUI_VIEWPORT_H_INCLUDED__

#include <irrlicht.h>
#include <ICP3DViewPort.h>

namespace cp3d {
namespace ui {

class CGUIViewport : public ICP3DViewPort {

public:

	CGUIViewport(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle);
	~CGUIViewport();

	bool OnEvent(const irr::SEvent &event);

	void draw();

	void setSceneManager(irr::scene::ISceneManager* scene);
	irr::scene::ISceneManager* getSceneManager() const;

	void setOverrideColor(const irr::video::SColor &color);
	void enableOverrideColor(const bool override);

private:

	// Components
	irr::scene::ISceneManager* Smgr;

	// Drawing
	bool OverrideColorEnabled;
	irr::video::SColor OverrideColor;
};


}
}

#endif
