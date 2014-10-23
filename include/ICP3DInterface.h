#ifndef _H_ICP3D_INTERFACE_INCLUDED__
#define _H_ICP3D_INTERFACE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

enum E_INTERFACE_EVENT {
	EIE_NODE_ADDED = 0,
	EIE_NODE_REMOVED,

	EIE_NODE_SELECTED,
	EIE_NODE_CHANGED,

	EIE_TEXTURE_SELECTED,
	EIE_MATERIAL_SELECTED
};

/// Interface controller interface
class ICP3DInterface {
public:

	//! Called by ICP3DInterfaceController when the element
	//! was resized
	virtual void OnResize() = 0;

	//! Returns the element to resize.
	//! Can be a window, panel, etc.
	virtual irr::gui::IGUIElement *getElementToResize() = 0;

};

} /// End cp3d namespace

#endif