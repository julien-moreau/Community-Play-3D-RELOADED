
#include "stdafx.h"
#include "../Core/CCP3DEditorCore.h"

#include "CCP3DPostProcessSpy.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

namespace cp3d {

CCP3DPostProcessSpy::CCP3DPostProcessSpy(CCP3DEditorCore *editorCore)
	: ICP3DSpy(editorCore->getDevice()->getFileSystem(), editorCore->getWorkingDirectory(), "Post Processes"),
	EditorCore(editorCore)
{
	Handler = editorCore->getRenderingEngine()->getHandler();
}

CCP3DPostProcessSpy::~CCP3DPostProcessSpy() {

}

void CCP3DPostProcessSpy::prepare() {
	// Because files count can change, we refresh the files list
	u32 count = Handler->getPostProcessingRoutineSize();

	if (FilesCount != count) {
		Files.clear();

		for (u32 i = 0; i < count; i++) {
			Files.push_back(Handler->getPostProcessingRoutineName(Handler->getPostProcessID(i)));
		}

		ICP3DSpy::prepare();
		FilesCount = count;
	}
}

void CCP3DPostProcessSpy::check() {
	// Get changed files and then replace the post-processes that changed :)
	ISpyFileList changedFiles = checkFiles(Files);
	ISpyFileList::ConstIterator it = changedFiles.begin();

	for (; it != changedFiles.end(); ++it) {
		stringc path = *it;
		u32 id = Handler->getPostProcessIDByName(*it);
		rendering::IPostProcessingRenderCallback *callback = Handler->getPostProcessingCallback(id);

		if (FileSystem->existFile(path))
			Handler->replacePostProcessAtIndex(id, path, callback);
		else
			Handler->replacePostProcessAtIndex(id, WorkingDirectory + path, callback);
	}

}

} /// End namespace cp3d
