#ifndef __H_C_CP3D_INTERFACE_CONTROLLER_INCLUDED__
#define __H_C_CP3D_INTERFACE_CONTROLLER_INCLUDED__

#include <limits>
#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>

namespace cp3d {

/// Resize types to check
enum E_INTERFACE_CONTROLLER_CHECK
{
	EICC_RIGHT = 0x1,
	EICC_LEFT = 0x2,
	EICC_TOP = 0x4,
	EICC_BOTTOM = 0x8,
	EMBF_FORCE_32BIT = 0x7fffffff //! This value is not used. It only forces this enumeration to compile in 32 bit.
};

/// Control descriptor
struct SControlDescriptor {
public:
	SControlDescriptor(irr::s32 flags, irr::s32 minSize = 0, irr::s32 maxSize = 0)
		: Flags(flags), MinWidth(minSize), MinHeight(minSize)
	{
		if (maxSize == 0)
			MaxHeight = MaxWidth = minSize + 200;
		else if (maxSize == -1)
			MaxHeight = MaxWidth = std::numeric_limits<int>::max();
	}

	irr::s32 Flags;

	irr::s32 MinWidth, MaxWidth;
	irr::s32 MinHeight, MaxHeight;
};

class CCP3DEditorCore;
class ICP3DInterface;

class CCP3DInterfaceController : public irr::IEventReceiver, public engine::ICP3DUpdate {
public:

	/// Constructor & Destructor
	CCP3DInterfaceController(CCP3DEditorCore *editorCore);
	~CCP3DInterfaceController();

	bool OnEvent(const irr::SEvent &event);
	void OnPreUpdate();

	bool addElement(ICP3DInterface *element, const irr::s32 flags);
	bool addElement(ICP3DInterface *element, const SControlDescriptor &descriptor);
	bool removeElement(ICP3DInterface *element);

private:
	/// Irrlicht
	irr::gui::IGUIEnvironment *Gui;
	irr::gui::ICursorControl *CursorControl;

	/// Datas
	bool MouseHeld;
	irr::core::map<ICP3DInterface *, SControlDescriptor> Parameters;
	irr::core::map<ICP3DInterface *, SControlDescriptor>::ConstIterator SelectedNode;
	E_INTERFACE_CONTROLLER_CHECK SelectedCheck;

	irr::core::dimension2du ScreenSize;

	/// Methods
	bool checkForChild(irr::gui::IGUIElement *element, irr::gui::IGUIElement *child);
	void checkForIcon(const irr::s32 cursorValue, const irr::s32 elementValue, const irr::gui::ECURSOR_ICON icon);
};

} /// End namespace cp3d

#endif