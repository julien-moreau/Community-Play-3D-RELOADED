
#include "stdafx.h"
#include "../Core/CCP3DInterfaceController.h"
#include "../Core/CCP3DEditorCore.h"

#include <ICP3DEditionTool.h>

#include "CCP3DSceneGraph.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DSceneGraph::CCP3DSceneGraph(CCP3DEditorCore *editorCore) : EditorCore(editorCore), WindowWidth(400), SelectedSceneNode(0)
{
	/// Configure
	editorCore->getEngine()->getEventReceiver()->addEventReceiver(this);

	Rengine = editorCore->getRenderingEngine();
	Driver = Rengine->getVideoDriver();
	Smgr = Rengine->getSceneManager();
	Gui = Rengine->getGUIEnvironment();

	/// GUI
	Window = Gui->addWindow(rect<s32>(Driver->getScreenSize().Width - WindowWidth, 0, Driver->getScreenSize().Width, 0), false, L"Scene Graph", 0, -1);
	Window->getCloseButton()->setVisible(false);
	Window->setDraggable(false);

	Graph = Gui->addTreeView(rect<s32>(0, 0, 0, 0), Window, -1, true, true, true);
	RootNode = Graph->getRoot();
	SceneNode = RootNode->addChildBack(L"Scene", L"SCENE");
	IGUIImageList* imageList = Gui->createImageList(Driver->getTexture("GUI/icons.png"), dimension2d<s32>(24, 24), true);
	if (imageList) {
		Graph->setImageList(imageList);
		imageList->drop();
	}

	/// Finish
	SControlDescriptor descriptor(EICC_LEFT);
	descriptor.MaxWidth = std::numeric_limits<s32>::max();
	descriptor.MinWidth = 200;
	editorCore->getInterfaceController()->addElement(this, descriptor);

	OnResize();
}

CCP3DSceneGraph::~CCP3DSceneGraph() {

}

void CCP3DSceneGraph::OnPreRender() {

}

void CCP3DSceneGraph::OnResize() {
	/// Select the new y position of the window
	s32 positionY = 0, count = 0;
	core::list<IGUIElement *>::ConstIterator it = Gui->getRootGUIElement()->getChildren().begin();
	for (; it != Gui->getRootGUIElement()->getChildren().end(); ++it) {
		EGUI_ELEMENT_TYPE type = (*it)->getType();
		if (type == EGUIET_MENU || type == EGUIET_TOOL_BAR) {
			positionY += (*it)->getRelativePosition().getHeight();
			count++;
		}
	}
	positionY -= 2 * count;

	/// Resize elements
	Window->setRelativePosition(rect<s32>(Driver->getScreenSize().Width - Window->getRelativePosition().getWidth(), positionY,
										  Driver->getScreenSize().Width, Driver->getScreenSize().Height));
	Graph->setRelativePosition(rect<s32>(5, 25, Window->getRelativePosition().getWidth() - 5, Window->getRelativePosition().getHeight() - 5));
}

bool CCP3DSceneGraph::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {

		if (event.GUIEvent.EventType == EGET_TREEVIEW_NODE_SELECT) {
			if (event.GUIEvent.Caller == Graph) {
				IGUITreeViewNode *node = Graph->getSelected();
				if (!node)
					return true;

				if (SelectedSceneNode)
					SelectedSceneNode->setDebugDataVisible(EDS_OFF);

				/// Get node's data (ISceneNode *)
				ISceneNode *sceneNode = (ISceneNode*)node->getData();
				sceneNode = dynamic_cast<ISceneNode *>(sceneNode);
				if (!sceneNode) {
					SelectedSceneNode = 0;
					return true;
				}

				SelectedSceneNode = sceneNode;
				void *data = sceneNode;

				/// Send selected event to all event receivers
				SEvent ev;
				ev.EventType = EET_USER_EVENT;
				ev.UserEvent.UserData1 = EIE_NODE_SELECTED;
				ev.UserEvent.UserData2 = (s32)data;
				EditorCore->getEngine()->getEventReceiver()->OnEvent(ev);

				SelectedSceneNode->setDebugDataVisible(EDS_MESH_WIRE_OVERLAY);
				
				return true;
			}
		}

	}

	else if (event.EventType == EET_USER_EVENT) {

		if (event.UserEvent.UserData1 == EIE_NODE_CHANGED) {
			ISceneNode *node = (ISceneNode *)event.UserEvent.UserData2;
			node = dynamic_cast<ISceneNode *>(node);

			if (!node)
				return false;

			IGUITreeViewNode *n = Graph->getSelected();
			if (!n)
				return false;

			ISceneNode *node2 = (ISceneNode *)n->getData();
			if (node != node2)
				return false;

			n->setData(node);
			n->setText(stringw(node->getName()).c_str());

			return false;
		}

	}

	return false;
}

void CCP3DSceneGraph::fillGraph(irr::scene::ISceneNode *start) {
	if (start == 0)
		start = Smgr->getRootSceneNode();

	fillGraphRecursively(start, SceneNode);
	SceneNode->setExpanded(true);
}

void CCP3DSceneGraph::fillGraphRecursively(ISceneNode *start, IGUITreeViewNode *treeNode) {

	ISceneNodeList::ConstIterator it = start->getChildren().begin();

	for (; it != start->getChildren().end(); ++it) {

		ESCENE_NODE_TYPE type = (*it)->getType();
		s32 imageIndex = getIconFromType(type);
		stringw icon = ((*it)->isVisible()) ? "" : "Hidden";

		void *data = *it;

		IGUITreeViewNode *node = treeNode->addChildBack(stringw((*it)->getName()).c_str(), icon.c_str(), imageIndex, -1, data);
		fillGraphRecursively(*it, node);

	}

}

irr::s32 CCP3DSceneGraph::getIconFromType(ESCENE_NODE_TYPE type) {
	s32 index = 0;
	switch (type) {
	case ESNT_ANIMATED_MESH: index = 25 * 12 + 18; break;
	case ESNT_CAMERA: index = 25 * 22 + 10; break;
	case ESNT_CUBE: index = 25 + 2; break;
	case ESNT_EMPTY: case ESNT_TEXT: index = 25 * 22 + 2; break;
	case ESNT_LIGHT: index = 25 * 18 + 14; break;
	default: index = 4; break;
	}

	return index;
}

irr::u32 CCP3DSceneGraph::getLightSceneNodeIndex(irr::scene::ILightSceneNode *node) {
	for (u32 i=0; i < Rengine->getLightSceneNodeCount(); i++) {
		ILightSceneNode *light = *Rengine->getLightSceneNode(i);
		if (light == node)
			return i;
	}

	return -1;
}

} /// End namespace cp3d
