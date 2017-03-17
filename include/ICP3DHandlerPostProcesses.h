#ifndef _H_ICP3D_HANDLER_POST_PROCESSES_INCLUDED__
#define _H_ICP3D_HANDLER_POST_PROCESSES_INCLUDED__

#include <functional>
#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class CCP3DHandler;
class ICP3DHandler;

//! Custom post-processing render callback
class IPostProcessingRenderCallback {
	friend class CCP3DHandler;
public:
	//! on pre-render callback
	//! \param effect: the handler rendering the post-process
	virtual void OnPreRender(ICP3DHandler* handler)
	{ }

	//! on post-render callback
	//! \param effect: the handler rendering the post-process
	virtual void OnPostRender(ICP3DHandler* handler)
	{ }

protected:
	irr::s32 MaterialType;
};

//! Depth shader callback
class ICP3DHandlerPostProcesses {

public:

	//! Adds a post-process from a file
	//! \param filename: the file containing the shader's code
	//! \param callback: the callback derived from cp3d::rendering::IPostProcessingRenderCallback
	virtual irr::u32 addPostProcessingEffectFromFile(const irr::core::stringc &filename, IPostProcessingRenderCallback *callback = 0) = 0;
	//! Adds a post-process from a string
	//! \param shader: the string containing the shader's code
	//! \param callback: the callback derived from cp3d::rendering::IPostProcessingRenderCallback
	virtual irr::u32 addPostProcessingEffectFromString(const irr::core::stringc &shader, IPostProcessingRenderCallback *callback = 0) = 0;
	//! Adds a custom post-process created using IGPUProgramingServices*
	//! Allow you to debug your shader in the awesome tool named Microsoft Visual Studio =D
	//! \param MaterialType: the material index given by IGPUProgramingServices*
	//! \param callbacl: the material's callback
	virtual void addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0) = 0;
	//! Removes a post-processing effect from the pipeline
	//! \param materialType: the material type index to remove
	//! \return: returns true if removed, false if not found
	virtual bool removePostProcessingEffect(irr::s32 materialType, const bool deleteCallback = true) = 0;

	//! Returns pointer of the post-process callback
	//! \param materialType: the material type of the post-process (can be the direct index [0..x])
	virtual IPostProcessingRenderCallback *getPostProcessingCallback(irr::s32 materialType) = 0;

	//! Drops post-process at index "index" and replaces by the new one
	//! \param index: the index of the post-process to replace (can be the materialType)
	//! \param filename: the name of the post-process file
	//! \param callback: the callback pointer of the post-process
	virtual irr::u32 replacePostProcessAtIndex(irr::s32 index, const irr::core::stringc &filename, IPostProcessingRenderCallback *callback = 0, irr::video::ITexture *customRtt = 0) = 0;

	//! Sets a post processing effect constant (OnSetConstant)
	//! \param materialType: the material to configure
	//! \param name: the name of the constant
	//! \param data: the constant's data
	//! \param count: the amount of floats in *data
	virtual void setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, const irr::f32* data, const irr::u32 count) = 0;

	//! Sets the post-processing render callback for the given Materialtype
	//! \param MaterialType: the material type indice for the post-process program
	//! \param callback: the callback to set
	virtual void setPostProcessingRenderCallback(irr::s32 materialType, IPostProcessingRenderCallback* callback) = 0;
	//! Sets the post-processing render callback for the given Materialtype using lambas
	//! \param MaterialType: the material type indice for the post-process program
	//! \param OnPreRender: OnPreRender lamba function to set
	//! \param OnPostRender: OnPostRender lambda function to set
	virtual void setPostProcessingRenderCallback(const irr::s32 &materialType,
		std::function<void(ICP3DHandler *handler)> OnPreRender = [](ICP3DHandler *handler) {},
		std::function<void(ICP3DHandler *handler)> OnPostRender = [](ICP3DHandler *handler) {}) = 0;

	//! Adds a custom UserMapSampler texture
	//! \param userTexture: the texture to set at UserMapSampler
	virtual void setPostProcessingUserTexture(irr::video::ITexture* userTexture) = 0;
	//! Sets a custom texture at index "index"
	//! \param index: the index of the texture layer (0 to MATERIAL_MAX_TEXTURES)
	//! \param texture: the texture to set
	virtual void setPostProcessingTextureAtIndex(const irr::u32 &index, irr::video::ITexture *texture) = 0;

	//! Sets the custom RTT of the post-process
	//! Then, it can be used anywhere else as buffers are saved
	//! \param materialType: the material type
	//! \param size: the RTT size
	//! \param name: the RTT name
	//! \param format: the RTT color format
	virtual void setPostProcessCustomRTT(const irr::s32 &materialType, const irr::core::dimension2du &size, const irr::core::stringc& name, const irr::video::ECOLOR_FORMAT format = irr::video::ECF_A8R8G8B8) = 0;

	//! Removes the custom RTT of the given post-process
	//! \param materialType: the material type
	virtual void removePostProcessCustomRTT(const irr::s32 &materialType) = 0;

	//! Returns the custom RTT of the given post-process
	//! \param materialType: the material type
	virtual irr::video::ITexture *getPostProcessCustomRTT(const irr::s32 &materialType) = 0;

};


} /// End rendering namespace
} /// End cp3d namespace

#endif
