
#include "stdafx.h"
#include "CCP3DExporter.h"

#include "../../CP3DEngine.h"

using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;

namespace cp3d {
namespace engine {

void CCP3DExporter::setAttribute(IAttributes *attributes, stringw type) {
	stringc name = Reader->getNodeName();
	stringc value = Reader->getAttributeValue(L"value");
	array<stringc> values;

	value.split(values, ",");

	if (type == "int") {
		attributes->addInt(name.c_str(), Reader->getAttributeValueAsInt(L"value"));
	}
	else if (type == "string") {
		attributes->addString(name.c_str(), value.c_str());
	}
	else if (type == "bool") {
		attributes->addBool(name.c_str(), value == "true");
	}
	else if (type == "float") {
		attributes->addFloat(name.c_str(), Reader->getAttributeValueAsFloat(L"value"));
	}
	else if (type == "vector3d") {
		attributes->addVector3d(name.c_str(), vector3df(core::fast_atof(values[0].c_str()), core::fast_atof(values[1].c_str()), core::fast_atof(values[2].c_str())));
	}
	else if (type == "colorf") {
		attributes->addColorf(name.c_str(), SColorf(core::fast_atof(values[0].c_str()), core::fast_atof(values[1].c_str()), core::fast_atof(values[2].c_str()), core::fast_atof(values[3].c_str())));
	}
	else if (type == "color") {
		attributes->addColor(name.c_str(), SColor(std::atoi(values[0].c_str()), std::atoi(values[1].c_str()), std::atoi(values[2].c_str()), std::atoi(values[3].c_str())));
	}
}

bool CCP3DExporter::importProject(irr::core::stringc filename) {
	Reader = Device->getFileSystem()->createXMLReader(filename);

	if (!Reader)
		return false;

	while (Reader->read()) {
		if (Reader->getNodeType() == EXN_ELEMENT) {
			stringw current = Reader->getNodeName();

			if (current == "SceneManager") {
				importScene();
			}
			else if (current == "SceneNode") {
				importNode();
			}
		}
	}

	return true;
}

void CCP3DExporter::importNode() {
	IAttributes *attr = Device->getFileSystem()->createEmptyAttributes(Device->getVideoDriver());
	ISceneManager *smgr = Engine->getRenderingEngine()->getSceneManager();

	ISceneNode *node = 0;
	ESCENE_NODE_TYPE type = ESNT_UNKNOWN;

	while (Reader->read()) {
		stringw current = Reader->getNodeName();

		if (Reader->getNodeType() == EXN_ELEMENT_END) {
			break;
		}
		else if (current == "Type") {
			type = (ESCENE_NODE_TYPE)Reader->getAttributeValueAsInt(L"value");
		}
		else {
			setAttribute(attr, Reader->getAttributeValue(L"type"));
		}
	}

	if (type == ESNT_MESH)
		node = smgr->addMeshSceneNode(0, 0, -1, vector3df(0.f), vector3df(0.f), vector3df(0.f), true);
	else if (type == ESNT_ANIMATED_MESH)
		node = smgr->addAnimatedMeshSceneNode(0, 0, -1, vector3df(0.f), vector3df(0.f), vector3df(0.f), true);
	else if (type == ESNT_CUBE)
		node = smgr->addCubeSceneNode(50.f, 0, -1);
	else if (type == ESNT_LIGHT) {
		rendering::ICP3DLightSceneNode *light = Engine->getRenderingEngine()->createLightSceneNode(true, true);
		node = *light;
	}

	if (node) {
		node->deserializeAttributes(attr);
		Engine->getSceneNodeCreator()->configureSceneNode(node);
	}
}

void CCP3DExporter::importScene() {
	/// To implement
}

} /// End namespace engine
} /// End namespace cp3d
