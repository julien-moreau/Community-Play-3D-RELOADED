#ifndef _H_ICP3D_EDITOR_INCLUDED__
#define _H_ICP3D_EDITOR_INCLUDED__

namespace cp3d {

class ICP3DEditor {
public:

	//! Runs the editor. Should be called only one time to run the editor
	//! after cp3d::createEditor()
	virtual void runEditor() = 0;

};

} /// End cp3d namespace

#endif