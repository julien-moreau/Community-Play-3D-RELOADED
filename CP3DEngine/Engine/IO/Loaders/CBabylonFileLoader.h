#ifndef __C_BABYLON_FILE_LOADER_H_INCLUDED__
#define __C_BABYLON_FILE_LOADER_H_INCLUDED__

#include <irrlicht.h>

#include "SJSONObject.h"

namespace cp3d {
namespace engine {

//! Meshloader capable of loading .babylon scenes.
class CBabylonSceneFileLoader : public irr::scene::ISceneLoader
{
public:

	/// ctor and dtor
	CBabylonSceneFileLoader(irr::scene::ISceneManager *smgr, irr::io::IFileSystem* fs, irr::ILogger* logger);
	virtual ~CBabylonSceneFileLoader();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (i.e. ".stl")
	virtual bool isALoadableFileExtension(const irr::io::path& filename) const;

	//! Returns true if the class might be able to load this file.
	virtual bool isALoadableFileFormat(irr::io::IReadFile *file) const;

	//! Loads the scene into the scene manager.
	virtual bool loadScene(irr::io::IReadFile* file, irr::scene::ISceneUserDataSerializer* userDataSerializer = 0, irr::scene::ISceneNode* rootNode = 0);

private:
	/// Datas
	irr::scene::ISceneManager *Smgr;
	irr::io::IFileSystem *FileSystem;
	irr::ILogger* Logger;

	SJSONObjectTree *Scene;

	irr::core::map<irr::core::stringc, irr::video::SMaterial> Materials;

	/// Methods
	void parseMaterials();
	void parseMeshes();

	irr::video::SColorf getColor3(const JSONArray& arr);
	irr::core::vector3df getVector3(const JSONArray& arr);
};

} /// end namespace cp3d
} /// end namespace engine

#endif
