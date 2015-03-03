#include "Stdafx.h"

#include "CUIAddMesh.h"
#include "../../../Core/CCP3DEditorCore.h"

using namespace irr;
using namespace scene;
using namespace gui;
using namespace core;
using namespace video;

namespace cp3d {
namespace ui {

CUIAddMesh::CUIAddMesh(CCP3DEditorCore *editorCore, const bool animated)
	: EditorCore(editorCore), Animated(animated), Node(0)
{
	CreateUI(editorCore);
	EditorCore->getEngine()->getEventReceiver()->addEventReceiver(this);

	// Configure
	if (!animated) {
		UIInstance->AutoAnimate->setVisible(false);
	}

	// Viewport
	ViewPortMgr = EditorCore->getRenderingEngine()->getSceneManager()->createNewSceneManager(false);
	ViewPortMgr->addCameraSceneNodeMaya();
	UIInstance->Viewport->setSceneManager(ViewPortMgr);
}

CUIAddMesh::~CUIAddMesh() {
	RemoveUI();
	EditorCore->getEngine()->getEventReceiver()->removeEventReceiver(this);

	// Viewport
	ViewPortMgr->drop();
}

bool CUIAddMesh::OnEvent(const SEvent& event) {
	
	if (event.EventType == EET_GUI_EVENT) {

		// Close window
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE || event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == UIInstance->Window || event.GUIEvent.Caller == UIInstance->CancelButton) {
				delete this;
				return true;
			}
		}

		// Buttons
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			// Ok Button
			if (event.GUIEvent.Caller == UIInstance->OkButton) {
				if (Node && Mesh && MeshPath != "") {
					ISceneManager *smgr = EditorCore->getRenderingEngine()->getSceneManager();
					Mesh = smgr->getMesh(MeshPath);

					if (Animated) {
						Node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh *)Mesh);
						s32 startFrame = ((IAnimatedMeshSceneNode *)Node)->getStartFrame();

						if (UIInstance->AutoAnimate->isChecked()) {
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

					SEvent ev;
					ev.EventType = EET_USER_EVENT;
					ev.UserEvent.UserData1 = EIE_NODE_ADDED;
					ev.UserEvent.UserData2 = (s32)Node;
					EditorCore->getEngine()->getEventReceiver()->OnEvent(ev);

					delete this;
				}

				return true;
			}

			// Browse button
			if (event.GUIEvent.Caller == UIInstance->BrowseButton) {
				ui::ICP3DGUIManager *guiMgr = EditorCore->getGUIManager();
				BrowseDialog = guiMgr->createFileOpenDialog("Select mesh...", 0, ui::ICP3DFileSelector::EFST_OPEN_DIALOG);
				return true;
			}
		}

		// Scroll bars
		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			if (event.GUIEvent.Caller == UIInstance->ScaleScrollBar) {
				if (Node)
					Node->setScale(vector3df((f32)UIInstance->ScaleScrollBar->getPos()));

				return true;
			}
		}

		// Browse dialog
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == BrowseDialog) {
				if (Node)
					Node->remove();
				if (Mesh)
					ViewPortMgr->getMeshCache()->removeMesh(Mesh);

				MeshPath = BrowseDialog->getFileName();
				Mesh = ViewPortMgr->getMesh(MeshPath);

				if (!Mesh) {
					EditorCore->getGUIManager()->createMessageBox(L"Cannot load mesh", L"Cannot load the mesh...", EMBF_OK, true);
					MeshPath = "";
				}
				else {

					if (Animated) {
						Node = ViewPortMgr->addAnimatedMeshSceneNode((IAnimatedMesh *)Mesh);
						IAnimatedMeshSceneNode *anode = (IAnimatedMeshSceneNode *)Node;
						anode->setFrameLoop(anode->getStartFrame(), anode->getEndFrame());
						anode->setLoopMode(true);
					}
					else {
						Mesh->setHardwareMappingHint(EHM_STATIC, EBT_VERTEX_AND_INDEX);
						Node = ViewPortMgr->addMeshSceneNode(Mesh);
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


} // end namespace ui
} // end namespace cp3d


