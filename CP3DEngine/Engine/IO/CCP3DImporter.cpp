
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
	rendering::ICP3DRenderingEngine *rengine = Engine->getRenderingEngine();
	rendering::ICP3DHandler *handler = Engine->getRenderingEngine()->getHandler();
	rendering::ICP3DLightSceneNode *light = 0;

	IAttributes *attr = Device->getFileSystem()->createEmptyAttributes(Device->getVideoDriver());
	ISceneManager *smgr = rengine->getSceneManager();
	array<SMaterial> materials;

	ISceneNode *node = 0;
	IMesh *mesh = 0;
	ESCENE_NODE_TYPE type = ESNT_UNKNOWN;

	while (Reader->read()) {
		stringw current = Reader->getNodeName();
		stringw value = Reader->getAttributeValue(L"value");

		if (Reader->getNodeType() == EXN_ELEMENT_END) {
			break;
		}
		else if (current == "Type") {
			type = (ESCENE_NODE_TYPE)Reader->getAttributeValueAsInt(L"value");
		}
		else if (current == "Mesh") {
			if (value == "plane_mesh") {
				mesh = smgr->getMesh("plane_mesh");
				if (!mesh)
					mesh = smgr->addHillPlaneMesh("plane_mesh", dimension2d<f32>(100.f, 100.f), dimension2d<u32>(50, 50), 0, 0.f, dimension2d<f32>(0.f, 0.f), dimension2d<f32>(50.f, 50.f));
			}
			setAttribute(attr, Reader->getAttributeValue(L"type"));
		}
		else if (current == "Material") {
			materials.push_back(importMaterial());
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
		node = smgr->addCubeSceneNode(50.f, 0, -1, vector3df(0.f, 25.f, 0.f), vector3df(0.f, 45.f, 0.f));
	else if (type == ESNT_LIGHT) {
		light = Engine->getRenderingEngine()->createLightSceneNode(false, false);
		node = *light;
	}

	if (node) {
		node->deserializeAttributes(attr);
		Engine->getSceneNodeCreator()->configureSceneNode(node);

		const u32 materialSize = materials.size();
		if (materialSize) {
			for (u32 i = 0; i < materialSize; i++)
				node->getMaterial(i) = materials[i];
		}

		/// Shadows
		if (type == ESNT_ANIMATED_MESH || type == ESNT_MESH || type == ESNT_CUBE || type == ESNT_SPHERE || type == ESNT_OCTREE) {
			bool shadowed = attr->getAttributeAsBool("Shadowed");
			if (shadowed) {
				rendering::E_SHADOW_MODE mode = (rendering::E_SHADOW_MODE)attr->getAttributeAsInt("ShadowMode");
				handler->addShadowToNode(node, rendering::EFT_NONE, mode);
			}
		}
		else if (type == ESNT_LIGHT) {
			bool shadows = attr->getAttributeAsBool("ComputeShadows");
			bool computeNormal = attr->getAttributeAsBool("ComputeNormalMapping");
			if (computeNormal) {
				light->setComputeNormalMapping(true);
			}
			if (shadows) {
				rengine->setLightSceneNodeComputeShadows(light, true);
				light->setLightStrength(attr->getAttributeAsFloat("LightStrength"));

				rendering::SShadowLight *slight = light->getShadowLight();
				slight->setPosition(node->getPosition());
				slight->setFarValue(attr->getAttributeAsFloat("FarValue"));
				slight->setFOV(attr->getAttributeAsFloat("FrontOfView"));
				slight->setNearValue(attr->getAttributeAsFloat("NearValue"));
				slight->setDirectionalLight(attr->getAttributeAsBool("IsDirectional"));
				slight->setUseRoundSpotLight(attr->getAttributeAsBool("IsSpotLight"));
				slight->setShadowMapResolution(attr->getAttributeAsFloat("MapResolution"));
				slight->setTarget(attr->getAttributeAsVector3d("Target"));
				slight->setMustAutoRecalculate(attr->getAttributeAsBool("AutoRecalculate"));
			}
		}
	}
}

SMaterialLayer CCP3DExporter::importMaterialLayer() {
	SMaterialLayer TextureLayer;
	IAttributes *attr = Device->getFileSystem()->createEmptyAttributes(Device->getVideoDriver());

	while (Reader->read()) {
		stringw current = Reader->getNodeName();

		if (Reader->getNodeType() == EXN_ELEMENT_END)
			break;
		else
			setAttribute(attr, Reader->getAttributeValue(L"type"));
	}

	TextureLayer.Texture = Device->getVideoDriver()->getTexture(attr->getAttributeAsString("TexturePath"));

	attr->drop();
	return TextureLayer;
}

SMaterial CCP3DExporter::importMaterial() {
	SMaterial mat;
	IAttributes *attr = Device->getFileSystem()->createEmptyAttributes(Device->getVideoDriver());

	SMaterialLayer TextureLayer[video::MATERIAL_MAX_TEXTURES];
	u32 TextureLayerID = 0;

	while (Reader->read()) {
		stringw current = Reader->getNodeName();

		if (Reader->getNodeType() == EXN_ELEMENT_END)
			break;
		else if (current == "TextureLayer") {
			TextureLayer[TextureLayerID] = importMaterialLayer();
			TextureLayerID++;
		}
		else {
			setAttribute(attr, Reader->getAttributeValue(L"type"));
		}
	}

	mat.MaterialType = (E_MATERIAL_TYPE)attr->getAttributeAsInt("MaterialType");
	mat.AmbientColor = attr->getAttributeAsColor("AmbiantColor");
	mat.DiffuseColor = attr->getAttributeAsColor("DiffuseColor");
	mat.EmissiveColor = attr->getAttributeAsColor("EmissiveColor");
	mat.SpecularColor = attr->getAttributeAsColor("SpecularColor");
	mat.Shininess = attr->getAttributeAsFloat("Shininess");
	mat.Thickness = attr->getAttributeAsFloat("Thickness");
	mat.ZBuffer = attr->getAttributeAsInt("ZBuffer");
	mat.AntiAliasing = attr->getAttributeAsInt("AntiAliasing");
	mat.ColorMask = attr->getAttributeAsInt("ColorMask");
	mat.ColorMaterial = attr->getAttributeAsInt("ColorMaterial");
	mat.BlendOperation = (E_BLEND_OPERATION)attr->getAttributeAsInt("BlendOperation");
	mat.PolygonOffsetFactor = attr->getAttributeAsInt("PolygonOffsetFactor");
	mat.Wireframe = attr->getAttributeAsBool("Wireframe");
	mat.PointCloud = attr->getAttributeAsBool("PointCloud");
	mat.Lighting = attr->getAttributeAsBool("Lighting");
	mat.BackfaceCulling = attr->getAttributeAsBool("BackFaceCulling");
	mat.FrontfaceCulling = attr->getAttributeAsBool("FrontFaceCulling");
	mat.FogEnable = attr->getAttributeAsBool("FogEnable");
	mat.NormalizeNormals = attr->getAttributeAsBool("NormalizeNormals");
	mat.UseMipMaps = attr->getAttributeAsBool("UseMipMaps");

	for (u32 i = 0; i < video::MATERIAL_MAX_TEXTURES; i++)
		mat.TextureLayer[i] = SMaterialLayer(TextureLayer[i]);

	attr->drop();
	return mat;
}

void CCP3DExporter::importScene() {
	/// To implement
}

} /// End namespace engine
} /// End namespace cp3d
