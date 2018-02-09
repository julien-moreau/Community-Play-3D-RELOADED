#include "Stdafx.h"
#include "CBabylonFileLoader.h"

#include "CJSONParser.h"

namespace cp3d {
namespace engine {

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace io;

//! Constructor
CBabylonSceneFileLoader::CBabylonSceneFileLoader(ISceneManager *smgr, IFileSystem* fs, ILogger* logger)
	: Smgr(smgr), FileSystem(fs), Logger(logger)
{ }

//! Destructor
CBabylonSceneFileLoader::~CBabylonSceneFileLoader()
{ }

//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CBabylonSceneFileLoader::isALoadableFileExtension(const path& filename) const
{
	return core::hasFileExtension(filename, "babylon");
}

//! Returns true if the class might be able to load this file.
bool CBabylonSceneFileLoader::isALoadableFileFormat(IReadFile *file) const
{
	return true;
}

//! Loads the scene into the scene manager.
bool CBabylonSceneFileLoader::loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer, ISceneNode* rootNode)
{
	CJSONParser p(file, Logger);
	Scene = p.parse();

	if (!Scene)
		return false;

	// Materials
	parseMaterials();

	// Meshes
	parseMeshes();
	
	// Clean & return
	delete Scene;

	return true;
}

//! Parses the materials of scene
void CBabylonSceneFileLoader::parseMaterials()
{
	JSONArray& materials = Scene->getArray("materials");
	const u32 size = materials.size();

	for (u32 i = 0; i < size; i++)
	{
		JSONObject* materialObject = (JSONObject*)materials[i]->Value;
		SMaterial m;

		m.Name = materialObject->getString("name");

		// Colors
		m.AmbientColor = getColor3(materialObject->getArray("ambient")).toSColor();
		m.DiffuseColor = getColor3(materialObject->getArray("diffuse")).toSColor();
		m.SpecularColor = getColor3(materialObject->getArray("specular")).toSColor();
		m.EmissiveColor = getColor3(materialObject->getArray("emissive")).toSColor();

		// Textures
		JSONObject* textureObject;

		textureObject = materialObject->getObject("diffuseTexture");
		if (textureObject && !materialObject->isNull("diffuseTexture"))
			m.TextureLayer[0].Texture = Smgr->getVideoDriver()->getTexture(textureObject->getString("name"));

		textureObject = materialObject->getObject("bumpTexture");
		if (textureObject && !materialObject->isNull("bumpTexture"))
			m.TextureLayer[1].Texture = Smgr->getVideoDriver()->getTexture(textureObject->getString("name"));

		textureObject = materialObject->getObject("specularTexture");
		if (textureObject && !materialObject->isNull("specularTexture"))
			m.TextureLayer[2].Texture = Smgr->getVideoDriver()->getTexture(textureObject->getString("name"));

		Materials[materialObject->getString("id")] = m;
	}
}

//! Parses meshes of scene
void CBabylonSceneFileLoader::parseMeshes()
{
	JSONArray& meshes = Scene->getArray("meshes");
	const u32 size = meshes.size();

	// For each mesh
	for (u32 i = 0; i < size; i++)
	{
		JSONObject* meshObject = (JSONObject*) meshes[i]->Value;
		ISceneNode* node = 0;

		if (!meshObject->has("geometryId"))
		{
			SMesh* mesh = 0;
			const bool hasGeometry = meshObject->has("indices") && !meshObject->isNull("indices");

			if (hasGeometry)
			{
				// Create geometry on the fly
				mesh = new SMesh();

				const bool hasUvs = meshObject->has("uvs") && !meshObject->isNull("uvs");

				const JSONArray& positions = meshObject->getArray("positions");
				const JSONArray& normals = meshObject->getArray("normals");
				const JSONArray& uvs = meshObject->getArray("uvs");
				const JSONArray& indices = meshObject->getArray("indices");

				JSONArray& subMeshes = meshObject->getArray("subMeshes");
				const u32 smSize = subMeshes.size();

				for (u32 sm = 0; sm < smSize; sm++)
				{
					JSONObject* subMesh = (JSONObject*)subMeshes[sm]->Value;

					const u32 verticesStart = (u32)subMesh->getNumber("verticesStart");
					const u32 verticesCount = (u32)subMesh->getNumber("verticesCount");

					const u32 indexStart = (u32)subMesh->getNumber("indexStart");
					const u32 indexCount = (u32)subMesh->getNumber("indexCount");

					SMeshBuffer* meshBuffer = new SMeshBuffer();
					meshBuffer->Vertices.set_used(verticesCount);
					meshBuffer->Indices.set_used(indexCount);

					for (u32 vi = verticesStart; vi < verticesCount; vi++)
					{
						S3DVertex v;
						v.Pos.set((f32)positions[vi * 3]->toNumber(), (f32)positions[vi * 3 + 1]->toNumber(), (f32)positions[vi * 3 + 2]->toNumber());
						v.Normal.set((f32)normals[vi * 3]->toNumber(), (f32)normals[vi * 3 + 1]->toNumber(), (f32)normals[vi * 3 + 2]->toNumber());

						if (hasUvs)
							v.TCoords.set((f32)uvs[vi * 2]->toNumber(), (f32)uvs[vi * 2 + 1]->toNumber());

						meshBuffer->Vertices[vi] = v;
					}

					for (u32 vi = indexStart; vi < indexCount; vi += 3)
					{
						meshBuffer->Indices[vi + 2] = (u16)indices[vi]->toNumber();
						meshBuffer->Indices[vi + 1] = (u16)indices[vi + 1]->toNumber();
						meshBuffer->Indices[vi] = (u16)indices[vi + 2]->toNumber();
					}

					// Finalize sub mesh
					meshBuffer->recalculateBoundingBox();
					meshBuffer->Material.FogEnable = meshObject->getBool("applyFog");

					// Add mesh buffer
					mesh->addMeshBuffer(meshBuffer);
				}

				// Setup mesh
				mesh->setDirty(EBT_VERTEX_AND_INDEX);
				mesh->recalculateBoundingBox();
			}

			node = mesh ? Smgr->addMeshSceneNode(mesh) : Smgr->addEmptySceneNode();
		}
		else
		{
			_IRR_DEBUG_BREAK_IF(1);
		}

		// Configure mesh scene node
		node->setName(meshObject->getString("name"));
		node->setPosition(getVector3(meshObject->getArray("position")));
		node->setRotation(getVector3(meshObject->getArray("rotation")) * RADTODEG);
		node->setScale(getVector3(meshObject->getArray("scaling")));
		
		if (meshObject->has("materialId") && !meshObject->isNull("materialId"))
			node->getMaterial(0) = Materials[meshObject->getString("materialId")];
	}
}

//! Converts array to vector3df structure
vector3df CBabylonSceneFileLoader::getVector3(const JSONArray& arr)
{
	return vector3df(
		(f32)arr[0]->toNumber(),
		(f32)arr[1]->toNumber(),
		(f32)arr[2]->toNumber()
	);
}

//! Converts array to SColorf structure
SColorf CBabylonSceneFileLoader::getColor3(const JSONArray& arr)
{
	return SColorf(
		(f32) arr[0]->toNumber(),
		(f32) arr[1]->toNumber(),
		(f32) arr[2]->toNumber()
	);
}

} /// end namespace cp3d
} /// end namespace engine
