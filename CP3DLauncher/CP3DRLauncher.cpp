
#include "stdafx.h"

#define CP3DR_COMPILE_EDITOR
#define CP3DR_COMPILE_ENGINE
#define CP3DR_COMPILE_RENDERING_ENGINE
#include <CP3DCompileConfig.h>
#include <ICP3DEditor.h>

int main(int argc, char* argv[])
{
	/// Create the editor and run it
	cp3d::ICP3DEditor *editor = cp3d::createEditor();
	editor->runEditor();

	return EXIT_SUCCESS;
}
