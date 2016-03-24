
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

CCP3DExporter::CCP3DExporter(CCP3DEngine *engine) : Engine(engine), ProjectDirectory(""), ProjectDirectoryLow("")
{
	Device = Engine->getRenderingEngine()->getDevice();
}

CCP3DExporter::~CCP3DExporter() {

}

bool CCP3DExporter::addAttributes(stringc name, IAttributes *attributes) {
	for (u32 i = 0; i < Attributes.size(); i++) {
		if (Attributes[i].Attributes == attributes || Attributes[i].Name == name) {
			return false;
		}
	}

	Attributes.push_back({ name, attributes });
	return true;
}

bool CCP3DExporter::removeAttributes(irr::io::IAttributes *attributes) {
	for (u32 i = 0; i < Attributes.size(); i++) {
		if (Attributes[i].Attributes == attributes) {
			Attributes.erase(i);
			return true;
		}
	}

	return false;
}

bool CCP3DExporter::removeAttributes(stringc name) {
	for (u32 i = 0; i < Attributes.size(); i++) {
		if (Attributes[i].Name == name) {
			Attributes.erase(i);
			return true;
		}
	}

	return false;
}

stringw CCP3DExporter::getValue(IAttributes *attributes, u32 indice) {
	E_ATTRIBUTE_TYPE type = attributes->getAttributeType(indice);

	switch (type) {
		case EAT_INT:
			return stringw(attributes->getAttributeAsInt(indice));
			break;
		case EAT_FLOAT:
			return stringw(attributes->getAttributeAsFloat(indice));
			break;
		case EAT_STRING: {
			return attributes->getAttributeAsStringW(indice);
		}
			break;
		case EAT_BOOL: {
			bool value = attributes->getAttributeAsBool(indice);
			return (value == false) ? "false" : "true";
		}
			break;
		case EAT_ENUM:
			return stringw(attributes->getAttributeAsEnumeration(indice));
			break;
		case EAT_COLOR: {
			SColor color = attributes->getAttributeAsColor(indice);
			return stringw(color.getAlpha()) + "," + stringw(color.getRed()) + "," + stringw(color.getGreen()) + "," + stringw(color.getBlue());
		}
			break;
		case EAT_COLORF: {
			SColorf colorf = attributes->getAttributeAsColorf(indice);
			return stringw(colorf.r) + "," + stringw(colorf.g) + "," + stringw(colorf.b) + "," + stringw(colorf.a);
		}
			break;
		case EAT_VECTOR3D: {
			vector3df v3d = attributes->getAttributeAsVector3d(indice);
			return stringw(v3d.X) + "," + stringw(v3d.Y) + "," + stringw(v3d.Z);
		}
			break;
		case EAT_POSITION2D: {
			position2di p2d = attributes->getAttributeAsPosition2d(indice);
			return stringw(p2d.X) + "," + stringw(p2d.Y);
		}
			break;
		case EAT_VECTOR2D: {
			vector2df v2d = attributes->getAttributeAsVector2d(indice);
			return stringw(v2d.X) + "," + stringw(v2d.Y);
		}
			break;
		case EAT_RECT: {
			rect<s32> r = attributes->getAttributeAsRect(indice);
			return stringw(r.UpperLeftCorner.X) + "," + stringw(r.UpperLeftCorner.Y) + "," + stringw(r.LowerRightCorner.X) + "," + stringw(r.LowerRightCorner.Y);
		}
			break;
		case EAT_MATRIX: {
			matrix4 mat = attributes->getAttributeAsMatrix(indice);
			stringw matW = "";
			for (u32 i = 0; i < 16; i++) {
				matW += stringw(mat.pointer()[i]) + ",";
			}
			return matW;
		}
			break;
		case irr::io::EAT_QUATERNION:
			break;
		case irr::io::EAT_BBOX:
			break;
		case irr::io::EAT_PLANE:
			break;
		case irr::io::EAT_TRIANGLE3D:
			break;
		case irr::io::EAT_LINE2D:
			break;
		case irr::io::EAT_LINE3D:
			break;
		case irr::io::EAT_STRINGWARRAY:
			break;

		case EAT_INTARRAY:
		case EAT_FLOATARRAY: {
			array<stringw> array = attributes->getAttributeAsArray(indice);
			stringw arrayW = "";
			for (u32 i = 0; i < array.size(); i++) {
				arrayW += array[i] + ", ";
			}
		}
			break;
		case irr::io::EAT_BINARY:
			break;
		case EAT_TEXTURE:
			return "texture";
			break;
		case irr::io::EAT_USER_POINTER:
			break;
		case EAT_DIMENSION2D: {
			dimension2du dim = attributes->getAttributeAsDimension2d(indice);
			return stringw(dim.Width) + ", " + stringw(dim.Height);
		}
			break;
		case EAT_COUNT:
			break;
		case EAT_UNKNOWN:
			break;
		default: break;
	}

	return L"";
}

stringc CCP3DExporter::removeProjectDirectory(stringc path) {
	path.remove(ProjectDirectory);
	path.remove(ProjectDirectoryLow);

	return path;
}

void CCP3DExporter::serializeNode(ISceneNode *node, IAttributes *attributes) {
	using namespace rendering;

	SAttributeReadWriteOptions options;
	options.Flags = EARWF_USE_RELATIVE_PATHS;
	options.Filename = ProjectDirectory.c_str();

	node->serializeAttributes(attributes, &options);

	ICP3DHandler *handler = Engine->getRenderingEngine()->getHandler();
	ESCENE_NODE_TYPE type = node->getType();

	if (node->getParent())
		attributes->addString("Parent", node->getParent()->getName());

	if (type == ESNT_LIGHT) {
		ICP3DLightSceneNode *light = Engine->getRenderingEngine()->getLightSceneNode((ILightSceneNode *)node);
		attributes->addBool("ComputeNormalMapping", light->isComputingNormalMapping());
		attributes->addBool("ComputeShadows", light->isComputingShadows());
		attributes->addFloat("LightStrength", light->getLightStrength());

		if (light->isComputingShadows()) {
			SShadowLight *slight = light->getShadowLight();
			attributes->addFloat("FarValue", slight->getFarValue());
			attributes->addFloat("FrontOfView", slight->getFrontOfView());
			attributes->addFloat("NearValue", slight->getNearValue());
			attributes->addBool("IsDirectional", slight->isDirectional());
			attributes->addBool("IsSpotLight", slight->usingRoundSpotLight());
			attributes->addInt("MapResolution", slight->getShadowMapResolution());
			attributes->addVector3d("Target", slight->getTarget());
			attributes->addBool("AutoRecalculate", slight->mustAutoRecalculate());
		}

	}
	else {
		attributes->addBool("Shadowed", handler->isNodeShadowed(node));
		attributes->addInt("ShadowMode", handler->getShadowModeForNode(node));

		if (type == ESNT_TEXT) {
			IBillboardTextSceneNode *tnode = (IBillboardTextSceneNode *)node;
			attributes->addString("Text", tnode->getText());
			attributes->addColor("TextColor", tnode->getTextColor());
			attributes->addFloat("SizeW", tnode->getSize().Width);
			attributes->addFloat("SizeH", tnode->getSize().Height);
		}
	}
}

void CCP3DExporter::exportMaterial(SMaterial &material) {
	IAttributes *attr = Device->getFileSystem()->createEmptyAttributes(Device->getVideoDriver());

	/// Material
	attr->addInt("MaterialType", material.MaterialType);
	attr->addColor("AmbiantColor", material.AmbientColor);
	attr->addColor("DiffuseColor", material.DiffuseColor);
	attr->addColor("EmissiveColor", material.EmissiveColor);
	attr->addColor("SpecularColor", material.SpecularColor);
	attr->addFloat("Shininess", material.Shininess);
	attr->addFloat("Thickness", material.Thickness);
	attr->addInt("ZBuffer", material.ZBuffer);
	attr->addInt("AntiAliasing", material.AntiAliasing);
	attr->addInt("ColorMask", material.ColorMask);
	attr->addInt("ColorMaterial", material.ColorMaterial);
	attr->addInt("BlendOperation", material.BlendOperation);
	attr->addInt("PolygonOffsetFactor", material.PolygonOffsetFactor);
	attr->addBool("Wireframe", material.Wireframe);
	attr->addBool("PointCloud", material.PointCloud);
	attr->addBool("Lighting", material.Lighting);
	attr->addBool("BackFaceCulling", material.BackfaceCulling);
	attr->addBool("FrontFaceCulling", material.FrontfaceCulling);
	attr->addBool("FogEnable", material.FogEnable);
	attr->addBool("NormalizeNormals", material.NormalizeNormals);
	attr->addBool("UseMipMaps", material.UseMipMaps);

	Writer->writeElement(L"Material");
	Writer->writeLineBreak();
	writeAttributes(attr);

	/// TextureLayer
	for (u32 i = 0; i < video::MATERIAL_MAX_TEXTURES; i++) {
		attr->clear();
		Writer->writeElement(L"TextureLayer");
		Writer->writeLineBreak();

		ITexture *texture = material.TextureLayer[i].Texture;
		if (texture) {
			stringc path = removeProjectDirectory(texture->getName().getPath().c_str());
			attr->addString("TexturePath", path.c_str());
		}

		writeAttributes(attr);

		Writer->writeClosingTag(L"TextureLayer");
		Writer->writeLineBreak();
	}

	/// Finish
	Writer->writeClosingTag(L"Material");
	Writer->writeLineBreak();

	attr->drop();
}

void CCP3DExporter::writeAttributes(IAttributes *attributes) {
	for (u32 i = 0; i < attributes->getAttributeCount(); i++) {
		stringw attrName = attributes->getAttributeName(i);

		Writer->writeElement(attrName.c_str(), true, L"type", attributes->getAttributeTypeString(i), L"value", getValue(attributes, i).c_str());
		Writer->writeLineBreak();
	}
}

void CCP3DExporter::writeAttributes(SAttribute attributes, ISceneNode *node) {
	IAttributes *attr = attributes.Attributes;
	stringw name = attributes.Name;

	Writer->writeElement(name.c_str());
	Writer->writeLineBreak();

	writeAttributes(attr);

	if (node && node->getType() != ESNT_LIGHT) {
		for (u32 i = 0; i < node->getMaterialCount(); i++)
			exportMaterial(node->getMaterial(i));
	}

	Writer->writeClosingTag(name.c_str());
	Writer->writeLineBreak();
}

bool CCP3DExporter::exportProject(stringc filename) {
	ProjectDirectory = Device->getFileSystem()->getFileDir(filename) + "/";
	ProjectDirectoryLow = stringc(Device->getFileSystem()->getFileDir(filename) + "/").make_lower();

	Writer = Device->getFileSystem()->createXMLWriter(filename);

	if (!Writer)
		return false;

	/// Write header
	Writer->writeXMLHeader();

	/// Write scene
	exportScene();

	/// Write external attributes
	for (u32 i = 0; i < Attributes.size(); i++) {
		writeAttributes(Attributes[i], 0);
	}

	/// Close XML writer
	Writer->drop();
	Writer = 0;

	return true;
}

void CCP3DExporter::exportScene() {
	IVideoDriver *driver = Device->getVideoDriver();
	ISceneManager *smgr = Engine->getRenderingEngine()->getSceneManager();
	IAttributes *attr = Device->getFileSystem()->createEmptyAttributes(driver);

	Writer->writeElement(L"SceneManager");
	Writer->writeLineBreak();

	array<ISceneNode *> nodes;
	smgr->getSceneNodesFromType(ESNT_ANY, nodes);

	for (u32 i = 0; i < nodes.size(); i++) {
		if (nodes[i]->getType() == ESNT_SCENE_MANAGER)
			continue;

		stringc name = nodes[i]->getName();
		ISceneNode *node = nodes[i];

		attr->addInt("Type", s32(nodes[i]->getType()));
		serializeNode(nodes[i], attr);
		writeAttributes({ "SceneNode", attr }, nodes[i]);

		attr->clear();
	}

	Writer->writeClosingTag(L"SceneManager");
	Writer->writeLineBreak();
}

} /// End namespace engine
} /// End namespace cp3d
