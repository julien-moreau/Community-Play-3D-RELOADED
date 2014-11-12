#ifndef __H_C_CP3D_EDITOR_TRANSFORMER_INCLUDED__
#define __H_C_CP3D_EDITOR_TRANSFORMER_INCLUDED__

#include <irrlicht.h>
#include <ICP3DCustomUpdate.h>
#include <ICP3DTransformer.h>

namespace cp3d {

class CCP3DEditorCore;

class CCP3DEditorTransformer : public ICP3DTransformer, public irr::IEventReceiver, public engine::ICP3DUpdate {
public:

	/// Constructor & Destructor
	CCP3DEditorTransformer(CCP3DEditorCore *editorCore);
	~CCP3DEditorTransformer();

	/// IEventReceiver
	bool OnEvent(const irr::SEvent &event);

	/// ICP3DUpdate
	void OnPostUpdate();
	void OnPreUpdate();

	/// ICP3DTransformer
	void setTransformerType(E_TRANSFORMER_TYPE type);

	void setViewPort(const irr::core::rect<irr::s32> &viewPort);

private:
	/// Methods
	bool findAndSetMousePositionInPlane();
	void setTransformerColor(irr::scene::ISceneNode *node, irr::video::SColor color);

	/// CP3D
	CCP3DEditorCore *EditorCore;

	/// Irrlicht
	irr::scene::ISceneManager *Smgr, *OriginalSmgr;
	irr::scene::ICameraSceneNode *Camera;

	irr::scene::IAnimatedMeshSceneNode *ArrowX, *ArrowY, *ArrowZ;
	irr::scene::ISceneNode *RingX, *RingY, *RingZ;
	irr::scene::ISceneNode *MassX, *MassY, *MassZ;
	irr::scene::ISceneNode *SelectedTransformer;
	irr::core::array<irr::scene::ISceneNode *> NodesToTransform;

	irr::scene::ISceneCollisionManager *CollisionMgr;

	irr::gui::ICursorControl *CursorControl;

	/// Datas
	irr::core::rect<irr::s32> ViewPort;
	irr::core::plane3df Plane;
	irr::core::vector3df MousePositionInPlane, MousePosition;
	irr::core::vector3df AverageTransformation, AveragePosition;
	irr::core::vector2di MousePositionInViewPort;
	bool CtrlActive;

	E_TRANSFORMER_TYPE TransformerType;

};

} /// End namespace cp3d

#endif