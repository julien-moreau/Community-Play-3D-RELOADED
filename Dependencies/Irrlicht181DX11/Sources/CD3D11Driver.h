// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// 
#ifndef __C_VIDEO_DIRECTX_11_H_INCLUDED__
#define __C_VIDEO_DIRECTX_11_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#ifdef _IRR_WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "CNullDriver.h"
#include "SIrrCreationParameters.h"
#include "IMaterialRendererServices.h"
#include "CD3D11CallBridge.h"

#include <d3d11.h>

namespace irr
{
namespace video
{
	inline void logFormatError(HRESULT hr, irr::core::stringc msg);

	class CD3D11VertexDeclaration;
	class CD3D11HardwareBuffer;

	struct SDepthSurface11 : public IReferenceCounted
	{
		SDepthSurface11() : Surface(0)
		{
#ifdef _DEBUG
			setDebugName("SDepthSurface");
#endif
		}
		virtual ~SDepthSurface11()
		{
			if (Surface)
				Surface->Release();
		}

		ID3D11DepthStencilView* Surface;
		core::dimension2du Size;
	};

	class CD3D11Driver : public CNullDriver, IMaterialRendererServices
	{
	public:
		friend class CD3D11HardwareBuffer;
		friend class CD3D11Texture;

		//! constructor
		CD3D11Driver(const irr::SIrrlichtCreationParameters& params,
			io::IFileSystem* io, HWND window);

		//! destructor
		virtual ~CD3D11Driver();

		//! applications must call this method before performing any rendering. returns false if failed.
		virtual bool beginScene(bool backBuffer=true, bool zBuffer=true,
				SColor color=SColor(255,0,0,0),
				const SExposedVideoData& videoData=SExposedVideoData(),
				core::rect<s32>* sourceRect=0);

		//! applications must call this method after performing any rendering. returns false if failed.
		virtual bool endScene();

		//! queries the features of the driver, returns true if feature is available
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const;

		//! sets transformation
		virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat);

		//! sets a material
		virtual void setMaterial(const SMaterial& material);

		virtual void drawMeshBuffer(const scene::IMeshBuffer* mb) _IRR_OVERRIDE_;

		virtual void draw2DVertexPrimitiveList(const void* vertices, u32 vertexCount, const void* indices,
			u32 primitiveCount, E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) _IRR_OVERRIDE_;

		virtual IHardwareBuffer* createHardwareBuffer(scene::IIndexBuffer* indexBuffer) _IRR_OVERRIDE_;

		virtual IHardwareBuffer* createHardwareBuffer(scene::IVertexBuffer* vertexBuffer) _IRR_OVERRIDE_;

		void removeAllHardwareBuffers();

				//! Create occlusion query.
		/** Use node for identification and mesh for occlusion test. */
		virtual void addOcclusionQuery(scene::ISceneNode* node,
				const scene::IMesh* mesh=0);

		//! Remove occlusion query.
		virtual void removeOcclusionQuery(scene::ISceneNode* node);

		//! Run occlusion query. Draws mesh stored in query.
		/** If the mesh shall not be rendered visible, use
		overrideMaterial to disable the color and depth buffer. */
		virtual void runOcclusionQuery(scene::ISceneNode* node, bool visible=false);

		//! Update occlusion query. Retrieves results from GPU.
		/** If the query shall not block, set the flag to false.
		Update might not occur in this case, though */
		virtual void updateOcclusionQuery(scene::ISceneNode* node, bool block=true);

		//! Return query result.
		/** Return value is the number of visible pixels/fragments.
		The value is a safe approximation, i.e. can be larger then the
		actual value of pixels. */
		virtual u32 getOcclusionQueryResult(scene::ISceneNode* node) const;

		//! sets a render target
		virtual bool setRenderTarget(video::ITexture* texture, bool clearBackBuffer,
			bool clearZBuffer, SColor color, video::ITexture* depthStencil) _IRR_OVERRIDE_;

		//! Sets multiple render targets
		virtual bool setRenderTarget(const core::array<video::IRenderTarget>& targets,
			bool clearBackBuffer, bool clearZBuffer, SColor color, video::ITexture* depthStencil) _IRR_OVERRIDE_;

		//! sets a viewport
		virtual void setViewPort(const core::rect<s32>& area);

		//! gets the area of the current viewport
		virtual const core::rect<s32>& getViewPort() const;
		
		//! is vbo recommended on this mesh? for DirectX 11 ALWAYS YES!!!!!!!!!!!
		// DirectX 11 doesn't use methods like drawPrimitiveUp (DX9) or glVertex (OpenGL)
		virtual bool isHardwareBufferRecommend(const scene::IMeshBuffer* mb) { return true; }

		//! draw
		virtual void drawHardwareBuffer(IHardwareBuffer* vertices,
				IHardwareBuffer* indices, E_VERTEX_TYPE vType=EVT_STANDARD,
				scene::E_PRIMITIVE_TYPE pType=scene::EPT_TRIANGLES,
				E_INDEX_TYPE iType=EIT_16BIT, u32 numInstances = 0);

		//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
		virtual void draw2DImage(const video::ITexture* texture, const core::position2d<s32>& destPos,
			const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
			SColor color=SColor(255,255,255,255), bool useAlphaChannelOfTexture=false);

		//! Draws a part of the texture into the rectangle.
		virtual void draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
			const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
			const video::SColor* const colors=0, bool useAlphaChannelOfTexture=false);

		//! Draws a set of 2d images, using a color and the alpha channel of the texture.
		virtual void draw2DImageBatch(const video::ITexture* texture,
				const core::array<core::position2d<s32> >& positions,
				const core::array<core::rect<s32> >& sourceRects,
				const core::rect<s32>* clipRect=0,
				SColor color=SColor(255,255,255,255),
				bool useAlphaChannelOfTexture=false);

		//!Draws an 2d rectangle with a gradient.
		virtual void draw2DRectangle(const core::rect<s32>& pos,
			SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
			const core::rect<s32>* clip);

		//! Draws a 2d line.
		virtual void draw2DLine(const core::position2d<s32>& start,
					const core::position2d<s32>& end,
					SColor color=SColor(255,255,255,255));

		//! Draws a pixel.
		virtual void drawPixel(u32 x, u32 y, const SColor & color);

		//! Draws a 3d line.
		virtual void draw3DLine(const core::vector3df& start,
			const core::vector3df& end, SColor color = SColor(255,255,255,255));

		//! initialises the Direct3D API
		bool initDriver(HWND hwnd, bool pureSoftware);

		//! \return Returns the name of the video driver. Example: In case of the DIRECT3D8
		//! driver, it would return "Direct3D8.1".
		virtual const wchar_t* getName() const;

		//! deletes all dynamic lights there are
		virtual void deleteAllDynamicLights();

		//! adds a dynamic light, returning an index to the light
		//! \param light: the light data to use to create the light
		//! \return An index to the light, or -1 if an error occurs
		virtual s32 addDynamicLight(const SLight& light);

		//! Turns a dynamic light on or off
		//! \param lightIndex: the index returned by addDynamicLight
		//! \param turnOn: true to turn the light on, false to turn it off
		virtual void turnLightOn(s32 lightIndex, bool turnOn);

		//! returns the maximal amount of dynamic lights the device can handle
		virtual u32 getMaximalDynamicLightAmount() const;

		//! Sets the dynamic ambient light color. The default color is
		//! (0,0,0,0) which means it is dark.
		//! \param color: New color of the ambient light.
		virtual void setAmbientLight(const SColorf& color);

		SColorf getAmbientLight() const;

		//! Draws a shadow volume into the stencil buffer.
		virtual void drawStencilShadowVolume(const core::array<core::vector3df>& triangles, bool zfail, u32 debugDataVisible);

		//! Fills the stencil shadow with color.
		virtual void drawStencilShadow(bool clearStencilBuffer=false,
			video::SColor leftUpEdge = video::SColor(0,0,0,0),
			video::SColor rightUpEdge = video::SColor(0,0,0,0),
			video::SColor leftDownEdge = video::SColor(0,0,0,0),
			video::SColor rightDownEdge = video::SColor(0,0,0,0));

		//! Returns the maximum amount of primitives (mostly vertices) which
		//! the device is able to render with one drawIndexedTriangleList
		//! call.
		virtual u32 getMaximalPrimitiveCount() const;

		//! Enables or disables a texture creation flag.
		virtual void setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled);

		//! Only used by the internal engine. Used to notify the driver that
		//! the window was resized.
		virtual void OnResize(const core::dimension2d<u32>& size);

		//! Can be called by an IMaterialRenderer to make its work easier.
		virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastMaterial,
			bool resetAllRenderstates);

		//! Returns type of video driver
		virtual E_DRIVER_TYPE getDriverType() const;

		//! Returns the transformation set by setTransform
		virtual const core::matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		//! Get a vertex shader constant index.
		virtual s32 getVertexShaderConstantID(const c8* name);

		//! Get a pixel shader constant index.
		virtual s32 getPixelShaderConstantID(const c8* name);

		//! Sets a vertex shader constant.
		virtual void setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1);

		//! Sets a pixel shader constant.
		virtual void setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1);

		//! Sets a constant for the vertex shader based on a name.
		virtual bool setVertexShaderConstant(s32 index, const f32* floats, int count);

		//! Sets a constant for the pixel shader based on a name.
		virtual bool setPixelShaderConstant(s32 index, const f32* floats, int count);

		//! Int interface for the above.
		virtual bool setVertexShaderConstant(s32 index, const s32* ints, int count);

		//! Int interface for the above.
		virtual bool setPixelShaderConstant(s32 index, const s32* ints, int count);

		//! Set hardware buffer for stream output stage
		virtual bool setStreamOutputBuffer(IHardwareBuffer* buffer);

		//! Returns a pointer to the IVideoDriver interface. (Implementation for
		//! IMaterialRendererServices)
		virtual IVideoDriver* getVideoDriver();

		//! Creates a render target texture.
		virtual ITexture* addRenderTargetTexture(const core::dimension2d<u32>& size,
		const io::path&name, const ECOLOR_FORMAT format);

		virtual ITexture* addRenderTargetTexture(const core::dimension2d<u32>& size,
				const io::path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN,
				u32 sampleCount = 1, u32 sampleQuality = 0, u32 arraySlices = 1);

		//! Clears the ZBuffer.
		virtual void clearZBuffer();

		//! Returns an image created from the last rendered frame.
		virtual IImage* createScreenShot(video::ECOLOR_FORMAT format=video::ECF_UNKNOWN, video::E_RENDER_TARGET target=video::ERT_FRAME_BUFFER);

		//! Set/unset a clipping plane.
		virtual bool setClipPlane(u32 index, const core::plane3df& plane, bool enable=false);

		//! Enable/disable a clipping plane.
		virtual void enableClipPlane(u32 index, bool enable);

		//! Used by CD3D11MaterialRenderer to get clip plane and status
		virtual void getClipPlane(u32 index, core::plane3df& plane, bool& enable);

		//! Returns the graphics card vendor name.
		virtual core::stringc getVendorInfo() {return VendorName;}

		//! Get the current color format of the color buffer
		/** \return Color format of the color buffer. */
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! Get the current color format of the color buffer
		/** \return Color format of the color buffer as D3D color value. */
		DXGI_FORMAT getD3DColorFormat() const;

		//! Returns the maximum texture size supported.
		virtual core::dimension2du getMaxTextureSize() const;

		//! Get D3D color format from Irrlicht color format.
		DXGI_FORMAT getD3DFormatFromColorFormat(ECOLOR_FORMAT format) const;

		//! Get Irrlicht color format from D3D color format.
		ECOLOR_FORMAT getColorFormatFromD3DFormat(DXGI_FORMAT format) const;

		//! Get index format from type
		DXGI_FORMAT getIndexType(E_INDEX_TYPE iType) const;

		//! query support for color format
		bool querySupportForColorFormat(DXGI_FORMAT format, D3D11_FORMAT_SUPPORT support);

		//! return feature level
		//! this is needed for material renderers to select correct shader profiles
		D3D_FEATURE_LEVEL getFeatureLevel() { return FeatureLevel; }

		//! Get primitive topology
		D3D11_PRIMITIVE_TOPOLOGY getTopology(scene::E_PRIMITIVE_TYPE primType) const;

		//! Get number of bits per pixel
		u32 getBitsPerPixel(DXGI_FORMAT format) const;

		//! Get number of components
		u32 getNumberOfComponents(DXGI_FORMAT format) const;

		//! Get number of indices
		u32 getIndexCount(scene::E_PRIMITIVE_TYPE primType, u32 primitiveCount) const;

		u32 getIndexSize(video::E_INDEX_TYPE iType) const;

		//! Get depth function
		D3D11_COMPARISON_FUNC getDepthFunction(E_COMPARISON_FUNC func) const;

		//! get color write enable
		D3D11_COLOR_WRITE_ENABLE getColorWriteEnable(E_COLOR_PLANE plane) const;

		bool isHardware() const { return DriverType == D3D_DRIVER_TYPE_HARDWARE; }

		// Return references to state descriptions for material renderers
		D3D11_BLEND_DESC& getBlendDesc() { return BlendDesc; }
		D3D11_RASTERIZER_DESC& getRasterizerDesc() { return RasterizerDesc; }
		D3D11_DEPTH_STENCIL_DESC& getDepthStencilDesc() { return DepthStencilDesc; }
		D3D11_SAMPLER_DESC* getSamplerDescs() { return SamplerDesc; }

		//! Check multisample quality levels
		virtual u32 queryMultisampleLevels(ECOLOR_FORMAT format, u32 numSamples) const;

	private:

		// enumeration for rendering modes such as 2d and 3d for minizing the switching of renderStates.
		enum E_RENDER_MODE
		{
			ERM_NONE = 0,	// no render state has been set yet.
			ERM_2D,		// 2d drawing rendermode
			ERM_3D,			// 3d rendering mode
			ERM_STENCIL_FILL, // stencil fill mode
			ERM_SHADOW_VOLUME_ZFAIL, // stencil volume draw mode
			ERM_SHADOW_VOLUME_ZPASS // stencil volume draw mode
		};
		E_RENDER_MODE CurrentRenderMode;

		SMaterial Material, LastMaterial;
		bool ResetRenderStates; // bool to make all renderstates be reseted if set.
		bool Transformation3DChanged;

		ITexture* CurrentTexture[MATERIAL_MAX_TEXTURES];
		ITexture* NullTexture;						// 1x1 texture replacement for NULL textures in materials
		core::matrix4 Matrices[ETS_COUNT];			// matrizes of the 3d mode we need to restore when we switch back from the 2d mode.

		// Direct3D 11 objects

		// Libraries (dxgi library is loaded automatically)
		HINSTANCE D3DLibrary;
		
		// DXGI objects
		DXGI_SWAP_CHAIN_DESC present;
		IDXGISwapChain* SwapChain;
		IDXGIAdapter* Adapter;
		IDXGIOutput* Output;
		IDXGIFactory* DXGIFactory;

		// D3D 11 Device objects
		D3D_DRIVER_TYPE DriverType;
		D3D_FEATURE_LEVEL FeatureLevel;
		ID3D11Device* Device;
		ID3D11DeviceContext* Context;
		core::stringw Name;

		// Back and depth buffers
		ID3D11RenderTargetView* DefaultBackBuffer;
		ID3D11DepthStencilView* DefaultDepthBuffer;
		ID3D11RenderTargetView* CurrentBackBuffer;
		ID3D11DepthStencilView* CurrentDepthBuffer;

		// Buffers for dynamic data
		ID3D11Buffer* DynVertexBuffer;
		ID3D11Buffer* DynIndexBuffer;
		u32 DynVertexBufferSize;
		u32 DynIndexBufferSize;
	
		SD3D11_DEPTH_STENCIL_DESC DepthStencilDesc;

		SD3D11_BLEND_DESC BlendDesc;

		SD3D11_SAMPLER_DESC SamplerDesc[MATERIAL_MAX_TEXTURES];

		SD3D11_RASTERIZER_DESC RasterizerDesc;

		core::dimension2d<u32> CurrentRendertargetSize;

		// Just one clip plane for now
		core::array<core::plane3df> ClipPlanes;
		bool ClipPlaneEnabled[3];

		core::rect<s32>* SceneSourceRect;

		//! All the lights that have been requested; a hardware limited
		//! number of them will be used at once.
		struct RequestedLight
		{
			RequestedLight(SLight const & lightData)
				: LightData(lightData), HardwareLightIndex(-1), DesireToBeOn(true) { }

			SLight	LightData;
			s32	HardwareLightIndex; // GL_LIGHT0 - GL_LIGHT7
			bool	DesireToBeOn;
		};
		core::array<RequestedLight> RequestedLights;
		SColorf AmbientLight;
		u32 MaxActiveLights;

		core::array<CD3D11HardwareBuffer*> HardwareBuffer;

		core::stringc VendorName;
		u16 VendorID;

		CD3D11CallBridge* BridgeCalls;

		core::array<SDepthSurface11*> DepthBuffers;

		u32 MaxTextureUnits;
		u32 MaxUserClipPlanes;
		f32 MaxLightDistance;
		s32 LastSetLight;
		
		ECOLOR_FORMAT ColorFormat;
		DXGI_FORMAT D3DColorFormat;
		DXGI_FORMAT DepthStencilFormat;		// Best format for depth stencil
		SIrrlichtCreationParameters Params;
	
		bool AlphaToCoverageSupport;

		//! Adds a new material renderer to the VideoDriver, based on a high level shading
		//! language.
		virtual s32 addHighLevelShaderMaterial(
			const c8* vertexShaderProgram, const c8* vertexShaderEntryPointName, E_VERTEX_SHADER_TYPE vsCompileTarget,
			const c8* pixelShaderProgram, const c8* pixelShaderEntryPointName, E_PIXEL_SHADER_TYPE psCompileTarget,
			const c8* geometryShaderProgram, const c8* geometryShaderEntryPointName, E_GEOMETRY_SHADER_TYPE gsCompileTarget,
			scene::E_PRIMITIVE_TYPE inType, scene::E_PRIMITIVE_TYPE outType, u32 verticesOut,
			IShaderConstantSetCallBack* callback,
			E_MATERIAL_TYPE baseMaterial, s32 userData, E_GPU_SHADING_LANGUAGE shadingLang);

		virtual s32 addShaderMaterial(const c8* vertexShaderProgram, const c8* pixelShaderProgram, IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial, s32 userData);

		void createMaterialRenderers();

		void draw2D3DVertexPrimitiveList(const void* vertices, u32 vertexCount, u32 pVertexSize, 
			const void* indices, u32 primitiveCount, E_VERTEX_TYPE vType, 
			scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType, bool is3D, u32 numInstances = 0);

		D3D11_TEXTURE_ADDRESS_MODE getTextureWrapMode(const u8 clamp);

		//! sets the needed renderstates
		bool setRenderStates3DMode(E_VERTEX_TYPE vType);

		//! sets the needed renderstates
		void setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel);

		bool setActiveTexture(u32 stage, video::ITexture* texture);

		//! returns a device dependent texture from a software surface (IImage)
		//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
		virtual video::ITexture* createDeviceDependentTexture(IImage* surface, const io::path& name, void* mipmapData=0);

		//! Check if a proper depth buffer for the RTT is available, otherwise create it.
		void checkDepthBuffer(ITexture* tex);

		// removes the depth struct from the DepthSurface array
		void removeDepthSurface(SDepthSurface11* depth);

		// creates a depth buffer view
		ID3D11DepthStencilView* createDepthStencilView(core::dimension2d<u32> size);

		//! returns the current size of the screen or rendertarget
		virtual const core::dimension2d<u32>& getCurrentRenderTargetSize() const;

		//! sets the needed renderstates
		void setRenderStatesStencilFillMode(bool alpha);

		//! sets the needed renderstates
		void setRenderStatesStencilShadowMode(bool zfail, u32 debugDataVisible);

		//! reallocate dynamic buffers
		virtual bool reallocateDynamicBuffers( u32 vertexBufferSize, u32 indexBufferSize );

		//! upload dynamic vertex and index data to GPU
		virtual bool uploadVertexData(const void* vertices, u32 vertexCount,
									  const void* indices, u32 indexCount,
									  E_VERTEX_TYPE vType, E_INDEX_TYPE iType);

		//! handle screen resize
		void reset();

		bool disableTextures( u32 fromStage = 0);

		virtual IVertexDescriptor* addVertexDescriptor(const core::stringc& pName);
	};
}
}

#endif // _IRR_COMPILE_WITH_DIRECT3D_11_
#endif // __C_VIDEO_DIRECTX_11_H_INCLUDED__