#include "Stdafx.h"
#include "CBabylonFileLoader.h"

#include "CJSONParser.h"

namespace cp3d {
namespace engine {

using namespace irr;
using namespace scene;
using namespace core;
using namespace io;

//! Constructor
CBabylonSceneFileLoader::CBabylonSceneFileLoader(ISceneManager *smgr, IFileSystem* fs)
	: Smgr(smgr), FileSystem(fs)
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
	CJSONParser *p = new CJSONParser(file);
	SJSONObjectTree *object = p->parse();

	f64 num = object->getNumber("number");

	delete object;
	
	return true;
}

} /// end namespace cp3d
} /// end namespace engine
