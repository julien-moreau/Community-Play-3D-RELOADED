#ifndef _H_ICP3D_UI_REGISTER_INCLUDED__
#define _H_ICP3D_UI_REGISTER_INCLUDED__

namespace cp3d {
namespace ui {

#define REGISTER_UI(ClassName, InstanceName) \
	class ClassName; \
	\
	class I##ClassName { \
		protected: \
			ClassName *InstanceName; \
			\
			void CreateUI(CCP3DEditorCore *editorCore) { \
				InstanceName = new ClassName(editorCore); \
			} \
			\
			void RemoveUI() { \
				InstanceName->remove(); \
				delete InstanceName; \
			} \
	}; \


} /// End ui namespace
} /// End cp3d namespace

#endif
