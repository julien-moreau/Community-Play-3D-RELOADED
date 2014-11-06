#ifndef _H_ICP3D_CUSTOM_VIEW_INCLUDED__
#define _H_ICP3D_CUSTOM_VIEW_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

/*
Class that handles the custom view.
The custom view is able to draw scenes, gui, etc. This is the
external tool other tools will call to get more gui space.

Imagine a FPS creation tool.
The FPS parameters will be handled by the ICP3DEditionTool but the the scene view
for real-time testing will be handled by the ICP3DCustomView where you'll draw your custom
scene gui elemnts (gui view port), gui elements, etc. to specified positions etc.

The ICP3DCustom view automatically handles the horizontal and vertical scroll bars.
*/
class ICP3DCustomView : public irr::IEventReceiver {
public:

	//! Clears the view
	virtual void clearView() = 0;

	//! Returns the parent gui element for drawing your gui elements
	/*
	Example :
	ICP3DCustomView *myView = ...

	Gui->addComboBox(myView->getParentGUIElement);

	Then the combo box will have the ParentGUIElement as parent.
	*/
	virtual irr::gui::IGUIElement *getParentGUIElement() = 0;

	//! Splits the screen. Height = ScreenSize / 2
	virtual void splitScreen() = 0;

};

} /// End cp3d namespace

#endif