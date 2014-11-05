#ifndef __H_C_CP3D_EDITION_TOOL_NODE_ANIMATORS_INCLUDED__
#define __H_C_CP3D_EDITION_TOOL_NODE_ANIMATORS_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

namespace ui {
	class CGUIPanel;
}
class CCP3DEditorCore;

/*
Class that will use the custom view to edit animators (from the 
CCP3DEditionToolSceneNode class).
*/
class CCP3DSceneNodeAnimators : public irr::IEventReceiver {
public:

	/// Constructor & Destructor
	CCP3DSceneNodeAnimators(CCP3DEditorCore *editorCore);
	~CCP3DSceneNodeAnimators();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// CCP3DSceneNodeAnimators
	void setAnimator(irr::scene::ISceneNodeAnimator *animator);
	irr::scene::ISceneNodeAnimator *getAnimator() { return Animator; }

private:
	/// Irrlicht
	irr::scene::ISceneNodeAnimator *Animator;

	/// CP3D
	CCP3DEditorCore *EditorCore;

	/// GUI
	ui::CGUIPanel *Panel;

	irr::gui::IGUIEditBox *AnimatorName;

};

} /// End namespace cp3d

#endif