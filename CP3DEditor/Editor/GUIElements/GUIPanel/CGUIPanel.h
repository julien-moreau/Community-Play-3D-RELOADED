
#ifndef __H_C_GUI_PANEL_INCLUDED__
#define __H_C_GUI_PANEL_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>

namespace cp3d {
namespace ui {

	class CGUIPanel : public irr::gui::IGUIElement, public engine::ICP3DUpdate {
	public:

		/// Constructor & Destructor
		CGUIPanel(irr::gui::IGUIEnvironment* gui, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle);
		~CGUIPanel();

		/// Inheritance
		bool OnEvent(const irr::SEvent& event);
		void OnPreUpdate();
		//! Draws the element and its children
		void draw();

	};


} // end namespace ui
} // end namespace irr

#endif
