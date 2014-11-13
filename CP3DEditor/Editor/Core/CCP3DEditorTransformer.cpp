
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

#define CP3D_TRANSFORMER_X_COLOR SColor(255, 255, 0, 0)
#define CP3D_TRANSFORMER_Y_COLOR SColor(255, 0, 255, 0)
#define CP3D_TRANSFORMER_Z_COLOR SColor(255, 0, 0, 255)

#define CP3D_TRANSFORMER_X_COLOR_SELECTED SColor(255, 128, 0, 0)
#define CP3D_TRANSFORMER_Y_COLOR_SELECTED SColor(255, 0, 128, 0)
#define CP3D_TRANSFORMER_Z_COLOR_SELECTED SColor(255, 9, 0, 128)

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
	auto configureNode = [&](ISceneNode *node, IMesh *mesh) {
		node->setMaterialType(EMT_SOLID);
		node->setMaterialFlag(EMF_LIGHTING, false);
		ITriangleSelector *s = Smgr->createTriangleSelectorFromBoundingBox(node);
		node->setTriangleSelector(s);
	};

	/// Arrows (position)
	IAnimatedMesh *arrowMesh = Smgr->addArrowMesh("CP3D:TransformerPositionX", 0xffff0000, 0xffff0000);
	ArrowX = Smgr->addAnimatedMeshSceneNode(arrowMesh);
	configureNode(ArrowX, arrowMesh);
	ArrowX->setRotation(vector3df(0.f, 0.f, -90.f));

	arrowMesh = Smgr->addArrowMesh("CP3D:TransformerPositionY", 0x66BB00, 0x66BB00);
	ArrowY = Smgr->addAnimatedMeshSceneNode(arrowMesh);
	configureNode(ArrowY, arrowMesh);

	arrowMesh = Smgr->addArrowMesh("CP3D:TransformerPositionZ", 0x3300DD, 0x3300DD);
	ArrowZ = Smgr->addAnimatedMeshSceneNode(arrowMesh);
	configureNode(ArrowZ, arrowMesh);
	ArrowZ->setRotation(vector3df(90.f, 0.f, 0.f));

	/// Rings (rotation)
	IMesh *ringMesh = EditorCore->getEngine()->getGeometryCreator()->createRingMesh(2.f, 0.4f, 25, SColor(255, 255, 0, 0));
	RingX = Smgr->addMeshSceneNode(ringMesh);
	configureNode(RingX, arrowMesh);
	RingX->setRotation(vector3df(0.f, 0.f, -90.f));
	RingX->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

	ringMesh = EditorCore->getEngine()->getGeometryCreator()->createRingMesh(2.f, 0.4f, 25, SColor(255, 0, 255, 0));
	RingY = Smgr->addMeshSceneNode(ringMesh);
	configureNode(RingY, arrowMesh);
	RingY->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
	
	ringMesh = EditorCore->getEngine()->getGeometryCreator()->createRingMesh(2.f, 0.4f, 25, SColor(255, 0, 0, 255));
	RingZ = Smgr->addMeshSceneNode(ringMesh);
	configureNode(RingZ, arrowMesh);
	RingZ->setRotation(vector3df(90.f, 0.f, 0.f));
	RingZ->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

	/// Masses (scale)
	IMesh *massMesh = EditorCore->getEngine()->getGeometryCreator()->createMassMesh(0.4f, 1.f, SColor(255, 255, 0, 0));
	MassX = Smgr->addMeshSceneNode(massMesh);
	configureNode(MassX, arrowMesh);
	MassX->setRotation(vector3df(0.f, 0.f, -90.f));
	massMesh->drop();

	massMesh = EditorCore->getEngine()->getGeometryCreator()->createMassMesh(0.4f, 1.f, SColor(255, 0, 255, 0));
	MassY = Smgr->addMeshSceneNode(massMesh);
	configureNode(MassY, arrowMesh);
	massMesh->drop();
	
	massMesh = EditorCore->getEngine()->getGeometryCreator()->createMassMesh(0.4f, 1.f, SColor(255, 0, 0, 255));
	MassZ = Smgr->addMeshSceneNode(massMesh);
	configureNode(MassZ, arrowMesh);
	MassZ->setRotation(vector3df(90.f, 0.f, 0.f));
	massMesh->drop();

	/// Finish
	Camera = Smgr->addCameraSceneNode(0);
	editorCore->getEngine()->addSceneManager(Smgr);

	setTransformerType(ETT_NONE);
}

CCP3DEditorTransformer::~CCP3DEditorTransformer() {
	ArrowX->remove();
	ArrowY->remove();
	ArrowZ->remove();
	RingX->remove();
	RingY->remove();
	RingZ->remove();
	MassX->remove();
	MassY->remove();
	MassZ->remove();
}

void CCP3DEditorTransformer::setViewPort(const irr::core::rect<s32> &viewPort) {
	ViewPort = viewPort;
}

void CCP3DEditorTransformer::setTransformerType(E_TRANSFORMER_TYPE type) {
	ArrowX->setVisible(false);
	ArrowY->setVisible(false);
	ArrowZ->setVisible(false);

	RingX->setVisible(false);
	RingY->setVisible(false);
	RingZ->setVisible(false);

	MassX->setVisible(false);
	MassY->setVisible(false);
	MassZ->setVisible(false);

	if (type == ETT_POSITION) {
		ArrowX->setVisible(true);
		ArrowY->setVisible(true);
		ArrowZ->setVisible(true);
	}
	else if (type == ETT_ROTATION) {
		RingX->setVisible(true);
		RingY->setVisible(true);
		RingZ->setVisible(true);
	}
	else if (type == ETT_SCALE) {
		MassX->setVisible(true);
		MassY->setVisible(true);
		MassZ->setVisible(true);
	}
	
	TransformerType = type;
}

void CCP3DEditorTransformer::setTransformerColor(ISceneNode *node, SColor color) {
	IMesh *mesh = 0;

	if (node->getType() == ESNT_ANIMATED_MESH)
		mesh = ((IAnimatedMeshSceneNode *)node)->getMesh();
	else if (node->getType() == ESNT_MESH)
		mesh = ((IMeshSceneNode *)node)->getMesh();

	if (!mesh)
		return;

	for (u32 i=0; i < mesh->getMeshBufferCount(); i++) {
		IMeshBuffer *mb = mesh->getMeshBuffer(i);

		S3DVertex *vertices = (S3DVertex *)mb->getVertices();
		for (u32 j=0; j < mb->getVertexCount(); j++) {
			vertices[j].Color = color;
		}
	}
}

void CCP3DEditorTransformer::OnPreUpdate() {
	/// Calculate mouse position in viewPort
	IVideoDriver *driver = EditorCore->getDevice()->getVideoDriver();

	if (ViewPort.getWidth() == 0 || ViewPort.getHeight() == 0) {
		ViewPort.UpperLeftCorner = vector2di(0, 0);
		ViewPort.LowerRightCorner = vector2di(1, 1);
	}

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

	RingX->setScale(scale);
	RingY->setScale(scale);
	RingZ->setScale(scale);

	MassX->setScale(scale);
	MassY->setScale(scale);
	MassZ->setScale(scale);

	/// Update camera
	ICameraSceneNode *originalCamera = OriginalSmgr->getActiveCamera();
	originalCamera->updateAbsolutePosition();
	Camera->setPosition(originalCamera->getAbsolutePosition());
	Camera->setTarget(originalCamera->getTarget());
	Camera->setUpVector(originalCamera->getUpVector());

	/// Update average transformation if multiple selected nodes
	AveragePosition = vector3df(0.f);
	AverageTransformation = vector3df(0.f);
	bool visible = false;

	if (NodesToTransform.size()) {
		for (u32 i=0; i < NodesToTransform.size(); i++) {
			NodesToTransform[i]->updateAbsolutePosition();
			AveragePosition += NodesToTransform[i]->getAbsolutePosition();

			if (TransformerType == ETT_ROTATION) {
				AverageTransformation += NodesToTransform[i]->getRotation();
			}
			else if (TransformerType == ETT_SCALE) {
				AverageTransformation += NodesToTransform[i]->getScale();
			}
		}

		AveragePosition /= (f32)NodesToTransform.size();

		if (TransformerType == ETT_POSITION)
			AverageTransformation = AveragePosition;

		visible = true;
	}

	ArrowX->setPosition(AveragePosition);
	ArrowY->setPosition(AveragePosition);
	ArrowZ->setPosition(AveragePosition);

	RingX->setPosition(AveragePosition);
	RingY->setPosition(AveragePosition);
	RingZ->setPosition(AveragePosition);

	MassX->setPosition(AveragePosition);
	MassY->setPosition(AveragePosition);
	MassZ->setPosition(AveragePosition);

	if (visible)
		setTransformerType(TransformerType);
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
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			f32 wheel = event.MouseInput.Wheel;

			if (CtrlActive)
				wheel /= 10.f;

			for (u32 i=0; i < NodesToTransform.size(); i++) {
				if (TransformerType == ETT_ROTATION)
					NodesToTransform[i]->setRotation(NodesToTransform[i]->getRotation() + wheel);

				else if (TransformerType == ETT_SCALE)
					NodesToTransform[i]->setScale(NodesToTransform[i]->getScale() + wheel);
			}

			return false;
		}

		else if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {

			core::line3d<f32> ray = CollisionMgr->getRayFromScreenCoordinates(MousePositionInViewPort, Camera);
			vector3df intersection;
			triangle3df hitTriangle;
			ISceneNode *selectedNode = CollisionMgr->getSceneNodeAndCollisionPointFromRay(ray, intersection, hitTriangle, 0, 0);
			OriginalSmgr->getActiveCamera()->setInputReceiverEnabled(false);

			SelectedTransformer = selectedNode;

			if (selectedNode == ArrowX || selectedNode == RingX || selectedNode == MassX) {
				Plane.setPlane(AveragePosition, core::vector3df(0.f, 0.f, -1.f));
				setTransformerColor(SelectedTransformer, CP3D_TRANSFORMER_X_COLOR_SELECTED);
			}
			else if (selectedNode == ArrowY || selectedNode == RingY || selectedNode == MassY) {
				Plane.setPlane(AveragePosition, core::vector3df(-1.f, 0.f, 0.f));
				setTransformerColor(SelectedTransformer, CP3D_TRANSFORMER_Y_COLOR_SELECTED);
			}
			else if (selectedNode == ArrowZ || selectedNode == RingZ || selectedNode == MassZ) {
				Plane.setPlane(AveragePosition, core::vector3df(0.f, -1.f, 0.f));
				setTransformerColor(SelectedTransformer, CP3D_TRANSFORMER_Z_COLOR_SELECTED);
			}
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
			if (SelectedTransformer) {
				if (Plane.Normal.X == -1.f)
					setTransformerColor(SelectedTransformer, CP3D_TRANSFORMER_Y_COLOR);
				else if (Plane.Normal.Y == -1.f)
					setTransformerColor(SelectedTransformer, CP3D_TRANSFORMER_Z_COLOR);
				else if (Plane.Normal.Z == -1.f)
					setTransformerColor(SelectedTransformer, CP3D_TRANSFORMER_X_COLOR);
			}

			SelectedTransformer = 0;
			OriginalSmgr->getActiveCamera()->setInputReceiverEnabled(true);

			return false;
		}

		else if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {

			if (SelectedTransformer && findAndSetMousePositionInPlane()) {
				vector3df newTransformation = AverageTransformation;

				if (SelectedTransformer == ArrowX || SelectedTransformer == RingX || SelectedTransformer == MassX)
					AverageTransformation.X = MousePositionInPlane.X - MousePosition.X;
				else if (SelectedTransformer == ArrowY || SelectedTransformer == RingY || SelectedTransformer == MassY)
					AverageTransformation.Y = MousePositionInPlane.Y - MousePosition.Y;
				else if (SelectedTransformer == ArrowZ || SelectedTransformer == RingZ || SelectedTransformer == MassZ)
					AverageTransformation.Z = MousePositionInPlane.Z - MousePosition.Z;

				for (u32 i=0; i < NodesToTransform.size(); i++) {
					ISceneNode *n = NodesToTransform[i];
					
					if (TransformerType == ETT_POSITION)
						n->setPosition(AverageTransformation);
					else if (TransformerType == ETT_ROTATION)
						n->setRotation(AverageTransformation);
					else if (TransformerType == ETT_SCALE)
						n->setScale(AverageTransformation);
				}

			}

			return false;
		}
	}

	return false;
}


} /// End namespace cp3d
