
#include "stdafx.h"

#define CP3DR_COMPILE_EDITOR
#include <CP3DCompileConfig.h>

int main(int argc, char* argv[])
{
	/// Create the editor and run it
	cp3d::ICP3DEditor *editor = cp3d::createEditor();
	editor->runEditor();

	return EXIT_SUCCESS;
}

