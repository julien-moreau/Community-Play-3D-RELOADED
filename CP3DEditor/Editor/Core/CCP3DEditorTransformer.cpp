
#include "stdafx.h"

#include <ICP3DEditionTool.h>

#include "CCP3DEditorCore.h"
#include "../UserInterfaces/CCP3DSceneGraph.h"
#include "../UserInterfaces/CCP3DEditionTool.h"
#include "../UserInterfaces/CCP3DCustomView.h"
#include "CCP3DEditorTransformer.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditorTransformer::CCP3DEditorTransformer(CCP3DEditorCore *editorCore) : EditorCore(editorCore), SelectedTransformer(0), CtrlActive(false)
{
	/// Configure
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
	editorCore->getEngine()->getCustomUpdater()->addCustomUpdate(this);
	OriginalSmgr = editorCore->getDevice()->getSceneManager();
	Smgr = OriginalSmgr->createNewSceneManager(false);
	CollisionMgr = Smgr->getSceneCollisionManager();
	CursorControl = editorCore->getDevice()->getCursorControl();

	/// Create
	auto configureNode = [&](ISceneNode *node) {
		node->setMaterialType(EMT_SOLID);
		node->setMaterialFlag(EMF_LIGHTING, false);
		//node->setMaterialFlag(EMF_ZBUFFER, false);
		ITriangleSelector *s = Smgr->createTriangleSelectorFromBoundingBox(node);
		node->setTriangleSelector(s);
	};

	/// Arrows (position)
	IAnimatedMesh *arrowMesh = Smgr->addArrowMesh("CP3D:TransformerPositionX", 0xffff0000, 0xffff0000);
	ArrowX = Smgr->addAnimatedMeshSceneNode(arrowMesh);
	configureNode(ArrowX);
	ArrowX->setRotation(vector3df(0.f, 0.f, -90.f));

	arrowMesh = Smgr->addArrowMesh("CP3D:TransformerPositionY", 0x66BB00, 0x66BB00);
	ArrowY = Smgr->addAnimatedMeshSceneNode(arrowMesh);
	configureNode(ArrowY);

	arrowMesh = Smgr->addArrowMesh("CP3D:TransformerPositionZ", 0x3300DD, 0x3300DD);
	ArrowZ = Smgr->addAnimatedMeshSceneNode(arrowMesh);
	configureNode(ArrowZ);
	ArrowZ->setRotation(vector3df(90.f, 0.f, 0.f));

	/// Rings (rotation)
	/*IMesh *ringMesh = EditorCore->getEngine()->getGeometryCreator()->createRingMesh(30.f, 4.f, 25, SColor(255, 255, 0, 0));
	RingX = Smgr->addMeshSceneNode(ringMesh);
	configureNode(RingX);
	RingX->setRotation(vector3df(0.f, 0.f, -90.f));

	ringMesh = EditorCore->getEngine()->getGeometryCreator()->createRingMesh(30.f, 4.f, 25, SColor(255, 0, 255, 0));
	RingY = Smgr->addMeshSceneNode(ringMesh);
	configureNode(RingY);
	
	ringMesh = EditorCore->getEngine()->getGeometryCreator()->createRingMesh(30.f, 4.f, 25, SColor(255, 0, 0, 255));
	RingZ = Smgr->addMeshSceneNode(ringMesh);
	configureNode(RingZ);
	RingZ->setRotation(vector3df(90.f, 0.f, 0.f));*/

	/// Finish
	Camera = Smgr->addCameraSceneNode();
	editorCore->getEngine()->addSceneManager(Smgr);
}

CCP3DEditorTransformer::~CCP3DEditorTransformer() {

}

void CCP3DEditorTransformer::setViewPort(const irr::core::rect<s32> &viewPort) {
	ViewPort = viewPort;
}

void CCP3DEditorTransformer::OnPreUpdate() {
	/// Calculate mouse position in viewPort
	IVideoDriver *driver = EditorCore->getDevice()->getVideoDriver();

	vector2di cpos = CursorControl->getPosition() - ViewPort.UpperLeftCorner;
	MousePositionInViewPort.X = (cpos.X * (driver->getScreenSize().Width  / 2)) / (ViewPort.getWidth() / 2);
	MousePositionInViewPort.Y = (cpos.Y * (driver->getScreenSize().Height / 2)) / (ViewPort.getHeight() / 2);

	/// Resize transformers
	ArrowX->updateAbsolutePosition();
	const f32 farValue = OriginalSmgr->getActiveCamera()->getPosition().getDistanceFrom(ArrowX->getAbsolutePosition());
	const vector3df scale = (vector3df(30.f, 80.f, 30.f) * farValue) / 300.f;

	ArrowX->setScale(scale);
	ArrowY->setScale(scale);
	ArrowZ->setScale(scale);

	/// Update camera
	Camera->setPosition(OriginalSmgr->getActiveCamera()->getPosition());
	Camera->setTarget(OriginalSmgr->getActiveCamera()->getTarget());

	/// Update average transformation if multiple selected nodes
	AverageTransformation = vector3df(0.f);
	bool visible = false;

	if (NodesToTransform.size()) {
		for (u32 i=0; i < NodesToTransform.size(); i++) {
			NodesToTransform[i]->updateAbsolutePosition();
			AverageTransformation += NodesToTransform[i]->getAbsolutePosition();
		}

		if (NodesToTransform.size())
			AverageTransformation /= (f32)NodesToTransform.size();

		visible = true;
	}

	ArrowX->setPosition(AverageTransformation);
	ArrowY->setPosition(AverageTransformation);
	ArrowZ->setPosition(AverageTransformation);

	ArrowX->setVisible(visible);
	ArrowY->setVisible(visible);
	ArrowZ->setVisible(visible);
}

void CCP3DEditorTransformer::OnPostUpdate() {

}

bool CCP3DEditorTransformer::findAndSetMousePositionInPlane() {
	core::line3df ray = CollisionMgr->getRayFromScreenCoordinates(MousePositionInViewPort);

	if(Plane.getIntersectionWithLine(ray.start, ray.getVector(), MousePositionInPlane))
		return true;
	else
		return false;
}

bool CCP3DEditorTransformer::OnEvent(const SEvent &event) {
	if (event.EventType == EET_KEY_INPUT_EVENT) {
		CtrlActive = event.KeyInput.Control;
		return false;
	}

	else if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == EIE_NODE_SELECTED) {

			ISceneNode *node = (ISceneNode *)event.UserEvent.UserData2;
			node = dynamic_cast<ISceneNode *>(node);

			if (!node)
				return false;

			if (!CtrlActive)
				NodesToTransform.clear();
			
			NodesToTransform.push_back(node);

			return false;
		}
	}

	else if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {

			core::line3d<f32> ray = CollisionMgr->getRayFromScreenCoordinates(MousePositionInViewPort, Camera);
			vector3df intersection;
			triangle3df hitTriangle;
			ISceneNode *selectedNode = CollisionMgr->getSceneNodeAndCollisionPointFromRay(ray, intersection, hitTriangle, 0, 0);
			OriginalSmgr->getActiveCamera()->setInputReceiverEnabled(false);

			SelectedTransformer = selectedNode;

			if (selectedNode == ArrowX)
				Plane.setPlane(AverageTransformation, core::vector3df(0.f, 0.f, -1.f));
			else if (selectedNode == ArrowY)
				Plane.setPlane(AverageTransformation, core::vector3df(-1.f, 0.f, 0.f));
			else if (selectedNode == ArrowZ)
				Plane.setPlane(AverageTransformation, core::vector3df(0.f, -1.f, 0.f));
			else {
				SelectedTransformer = 0;
				OriginalSmgr->getActiveCamera()->setInputReceiverEnabled(true);
			}

			if (findAndSetMousePositionInPlane()) {
				MousePosition = MousePositionInPlane - AverageTransformation;
			}
			
			return false;
		}
		else if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			SelectedTransformer = 0;
			OriginalSmgr->getActiveCamera()->setInputReceiverEnabled(true);
		}

		else if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {

			if (SelectedTransformer && findAndSetMousePositionInPlane()) {
				vector3df newTransformation = AverageTransformation;

				if (SelectedTransformer == ArrowX)
					AverageTransformation.X = MousePositionInPlane.X - MousePosition.X;
				else if (SelectedTransformer == ArrowY)
					AverageTransformation.Y = MousePositionInPlane.Y - MousePosition.Y;
				else if (SelectedTransformer == ArrowZ)
					AverageTransformation.Z = MousePositionInPlane.Z - MousePosition.Z;

				for (u32 i=0; i < NodesToTransform.size(); i++) {
					ISceneNode *n = NodesToTransform[i];
					n->setPosition(AverageTransformation);
				}

			}

			return false;
		}
	}

	return false;
}


} /// End namespace cp3d
