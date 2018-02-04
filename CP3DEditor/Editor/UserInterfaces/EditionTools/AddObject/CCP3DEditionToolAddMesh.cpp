
#include "stdafx.h"
#include "../../../Core/CCP3DEditorCore.h"
#include "../../../GUIElements/CGUIManager.h"
#include "../../../GUIElements/GUIFileSelector/CGUIFileSelector.h"
#include "../../CCP3DCustomView.h"
#include "../../CCP3DEditionTool.h"
#include "CCP3DEditionToolAddMesh.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DEditionToolAddMesh::CCP3DEditionToolAddMesh(CCP3DEditorCore *editorCore, bool animated)
	: EditorCore(editorCore), Animated(animated), MeshPath(""), Node(0), Mesh(0)
{
	/// Configure
	EditionTool = editorCore->getEditionTool();
	Driver = editorCore->getRenderingEngine()->getVideoDriver();
	Smgr = editorCore->getRenderingEngine()->getSceneManager();
	Gui = editorCore->getRenderingEngine()->getGUIEnvironment();

	Rengine = editorCore->getRenderingEngine();
	Handler = Rengine->getHandler();

	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);
}

CCP3DEditionToolAddMesh::~CCP3DEditionToolAddMesh() {
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);
}

void CCP3DEditionToolAddMesh::createInterface() {
	/// Tabs
	AddMeshTab = EditionTool->addTab(Animated ? "Add Animated Mesh" : "Add Static Mesh");
	
	/// Parameters
	EditionTool->setNewZone(AddMeshTab, "Mesh");
	FileOpenDialog = EditionTool->addField(AddMeshTab, EGUIET_FILE_OPEN_DIALOG);
	ViewPort = EditionTool->addField(AddMeshTab, EGUIET_MESH_VIEWER);

	EditionTool->setNewZone(AddMeshTab, "Parameters");
	AutoAnimate = EditionTool->addField(AddMeshTab, EGUIET_CHECK_BOX, DefaultEditionToolCallback("Auto animate"));

	EditionTool->setNewZone(AddMeshTab, "Finish");
	AddButton = EditionTool->addField(AddMeshTab, EGUIET_BUTTON, DefaultEditionToolCallback("Ok"));
}

void CCP3DEditionToolAddMesh::configure() {
	ViewPortSmgr = Smgr->createNewSceneManager(false);
	ViewPort.ViewPort->setSceneManager(ViewPortSmgr);
	ViewPortSmgr->addCameraSceneNodeMaya();
	ViewPort.ViewPort->setOverrideColor(SColor(255, 0, 0, 0));
	ViewPort.ViewPort->enableOverrideColor(true);

	AutoAnimate.CheckBox->setChecked(true);
}

void CCP3DEditionToolAddMesh::apply() {

}

void CCP3DEditionToolAddMesh::clear() {
	ViewPortSmgr->clear();
	Mesh = 0;
	Node = 0;
	MeshPath = "";

	ViewPort.ViewPort->setSceneManager(0);
}

bool CCP3DEditionToolAddMesh::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == FileOpenDialog.FileOpenData.OpenButton) {
				ui::ICP3DGUIManager *guiMgr = EditorCore->getGUIManager();
				FileOpenDialog.FileOpenData.FileOpenDialog = guiMgr->createFileOpenDialog("Select mesh...", 0, ui::ICP3DFileSelector::EFST_OPEN_DIALOG);
				return true;
			}
			else if (event.GUIEvent.Caller == AddButton.Button) {
				if (Node && Mesh && MeshPath != "") {
					ISceneManager *smgr = EditorCore->getRenderingEngine()->getSceneManager();
					Mesh = smgr->getMesh(MeshPath);

					if (Animated) {
						Node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh *)Mesh);
						s32 startFrame = ((IAnimatedMeshSceneNode *)Node)->getStartFrame();

						if (AutoAnimate.CheckBox->isChecked()) {
							s32 endFrame = ((IAnimatedMeshSceneNode *)Node)->getEndFrame();
							((IAnimatedMeshSceneNode *)Node)->setFrameLoop(startFrame, endFrame);
							((IAnimatedMeshSceneNode *)Node)->setLoopMode(true);
						}
						else {
							((IAnimatedMeshSceneNode *)Node)->setFrameLoop(startFrame, startFrame);
							((IAnimatedMeshSceneNode *)Node)->setLoopMode(false);
						}
					}
					else {
						Mesh->setHardwareMappingHint(EHM_STATIC, EBT_VERTEX_AND_INDEX);
						Node = smgr->addMeshSceneNode(Mesh);
					}

					EditorCore->getEngine()->getSceneNodeCreator()->configureSceneNode(Node);

					EditorCore->getRenderingEngine()->getHandler()->getDepthPassManager()->addNodeToPass(Node);
					EditorCore->getRenderingEngine()->getHandler()->getGeneralPassManager()->addNodeToPass(Node);

					SEvent ev;
					ev.EventType = EET_USER_EVENT;
					ev.UserEvent.UserData1 = EIE_NODE_ADDED;
					ev.UserEvent.UserData2 = (s32)Node;
					EditorCore->getEngine()->getEventReceiver()->OnEvent(ev);

					ViewPortSmgr->clear();
					Node = 0;
					Mesh = 0;
					MeshPath = "";
				}
				return true;
			}
		}
		else if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == FileOpenDialog.FileOpenData.FileOpenDialog) {
				if (Node)
					Node->remove();
				if (Mesh)
					ViewPortSmgr->getMeshCache()->removeMesh(Mesh);

				MeshPath = FileOpenDialog.FileOpenData.FileOpenDialog->getFileName();
				Mesh = ViewPortSmgr->getMesh(MeshPath);

				if (!Mesh) {
					EditorCore->getGUIManager()->createMessageBox(L"Cannot load mesh", L"Cannot load the mesh...", EMBF_OK, true);
					MeshPath = "";
				}
				else {
					FileOpenDialog.FileOpenData.EditBox->setText(stringw(MeshPath).c_str());

					if (Animated) {
						Node = ViewPortSmgr->addAnimatedMeshSceneNode((IAnimatedMesh *)Mesh);
						IAnimatedMeshSceneNode *anode = (IAnimatedMeshSceneNode *)Node;
						anode->setFrameLoop(anode->getStartFrame(), anode->getEndFrame());
						anode->setLoopMode(true);
					}
					else {
						Mesh->setHardwareMappingHint(EHM_STATIC, EBT_VERTEX_AND_INDEX);
						Node = ViewPortSmgr->addMeshSceneNode(Mesh);
					}

					Node->setMaterialFlag(EMF_LIGHTING, false);
					Node->setMaterialType(EMT_SOLID);
				}

				return true;
			}
		}
	}

	return false;
}


} /// End namespace cp3d
