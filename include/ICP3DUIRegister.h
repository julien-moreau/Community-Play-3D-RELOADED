#ifndef _H_ICP3D_UI_REGISTER_INCLUDED__
#define _H_ICP3D_UI_REGISTER_INCLUDED__

namespace cp3d {

class ICP3DEditor;

namespace ui {

#define REGISTER_UI(ClassName, InstanceName) \
	class ClassName; \
	\
	class I##ClassName { \
		protected: \
			ClassName *InstanceName; \
			\
			void CreateUI(ICP3DEditor *editorCore) { \
				InstanceName = new ClassName(editorCore); \
			} \
	}; \


} /// End ui namespace
} /// End cp3d namespace

#endif
