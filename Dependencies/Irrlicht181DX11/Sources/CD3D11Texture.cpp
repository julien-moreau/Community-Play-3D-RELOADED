// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#define _IRR_DONT_DO_MEMORY_DEBUGGING_HERE

#include "CD3D11Driver.h"
#include "CD3D11Texture.h"
#include "os.h"

#include "CImage.h"
#include "CColorConverter.h"

namespace irr
{
namespace video
{

//! rendertarget constructor
CD3D11Texture::CD3D11Texture(CD3D11Driver* driver, const core::dimension2d<u32>& size,
						   const io::path& name, const ECOLOR_FORMAT format, u32 arraySlices,
						   u32 sampleCount, u32 sampleQuality)
						   : ITexture(name), Texture(0), TextureBuffer(0), 
						   Device(0), Context(0), Driver(driver),
						   RTView(0), SRView(0),
						   TextureDimension(D3D11_RESOURCE_DIMENSION_TEXTURE2D), 
						   MipLevelLocked(0), NumberOfMipLevels(0), ArraySliceLocked(0), NumberOfArraySlices(arraySlices),
						   SampleCount(sampleCount), SampleQuality(sampleQuality), 
						   LastMapDirection((D3D11_MAP)0), DepthSurface(0),
						   HardwareMipMaps(false)

{
#ifdef _DEBUG
	setDebugName("CD3D11Texture");
#endif

	DriverType = EDT_DIRECT3D11;
	OriginalSize = size;
	Size = size;
	IsRenderTarget = true;

	Device = driver->getExposedVideoData().D3D11.D3DDev11;
	if (Device)
	{
		Device->AddRef();
		Device->GetImmediateContext( &Context );
	}

	createRenderTarget(format);
}


//! constructor
CD3D11Texture::CD3D11Texture(IImage* image, CD3D11Driver* driver,
			   u32 flags, const io::path& name, u32 arraySlices, void* mipmapData)
			   : ITexture(name), Texture(0), TextureBuffer(0),
			   Device(0), Context(0), Driver(driver),   
			   RTView(0), SRView(0), 
			   TextureDimension(D3D11_RESOURCE_DIMENSION_TEXTURE2D),
			   LastMapDirection((D3D11_MAP)0), DepthSurface(0), MipLevelLocked(0), NumberOfMipLevels(0), 
			   ArraySliceLocked(0), NumberOfArraySlices(arraySlices), SampleCount(1), SampleQuality(0),
			   HardwareMipMaps(false)
{
#ifdef _DEBUG
	setDebugName("CD3D11Texture");
#endif

	DriverType = EDT_DIRECT3D11;
	HasMipMaps = Driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);

	Device = driver->getExposedVideoData().D3D11.D3DDev11;
	if (Device)
	{
		Device->AddRef();
		Device->GetImmediateContext( &Context );
	}

	// Load a dds file
	if (core::hasFileExtension(name, "dds"))
	{
		
	}

	if (image)
	{
		if (createTexture(flags, image))
		{
			if (copyTexture(image))
				regenerateMipMapLevels(mipmapData);
		}
		else
			os::Printer::log("Could not create Direct3D11 Texture.", ELL_WARNING);
	}
}


//! destructor
CD3D11Texture::~CD3D11Texture()
{	
	if (DepthSurface)
	{
		if (DepthSurface->drop())
			Driver->removeDepthSurface(DepthSurface);
	}

	if(RTView)
		RTView->Release();

	if(SRView)
		SRView->Release();

	if(Texture)
		Texture->Release();

	if(TextureBuffer)
		TextureBuffer->Release();

	if(Context)
		Context->Release();

	if(Device)
		Device->Release();
}

//! return texture resource
ID3D11Resource* CD3D11Texture::getTextureResource() const
{
	return Texture;
}

//! return render target view
ID3D11RenderTargetView* CD3D11Texture::getRenderTargetView() const
{
	return RTView;
}

//! return shader resource view
ID3D11ShaderResourceView* CD3D11Texture::getShaderResourceView() const
{
	// Emulate "auto" mipmap generation
	if( IsRenderTarget && SRView )
		Context->GenerateMips( SRView );

	return SRView;
}

//! lock function
void* CD3D11Texture::lock(E_TEXTURE_LOCK_MODE mode, u32 mipmapLevel)
{
	bool ronly;
	if (mode == ETLM_READ_ONLY)
		ronly = true;
	else
		ronly = false;
	return lock(ronly,mipmapLevel,0);

}

void* CD3D11Texture::lock(bool readOnly, u32 mipmapLevel, u32 arraySlice)
{
	if(!Texture || !createTextureBuffer())
		return 0;

	HRESULT hr = S_OK;

	// Record mip level locked to use in unlock
	MipLevelLocked = mipmapLevel;
	ArraySliceLocked = arraySlice;

	// set map direction
	if (readOnly) 
		LastMapDirection = D3D11_MAP_READ;
	else 
		LastMapDirection = (D3D11_MAP)(D3D11_MAP_READ | D3D11_MAP_WRITE);

	// if read, and this is a render target texture (i.ex.: GPU will write data to texture)
	// shall sync data from main texture to texture buffer
	if ( (IsRenderTarget == true) && (LastMapDirection & D3D11_MAP_READ) )
	{
		Context->CopyResource( TextureBuffer, Texture );
	}

	// Map texture buffer
	D3D11_MAPPED_SUBRESOURCE mappedData;
	hr = Context->Map( TextureBuffer,
								D3D11CalcSubresource(MipLevelLocked,		// mip level to lock
													 ArraySliceLocked,		// array slice (only 1 slice for now)
													 NumberOfMipLevels), 	// number of mip levels
								LastMapDirection, 							// direction to map
								0,
								&mappedData );								// mapped result
	
	if(FAILED(hr))
	{
		logFormatError(hr, "Could not map texture buffer");

		return NULL;
	}

	Pitch = mappedData.RowPitch;

	return mappedData.pData;
}

//! unlock function
void CD3D11Texture::unlock()
{
	if (!Texture)
		return;

	// unlock texture buffer
	Context->Unmap( TextureBuffer, D3D11CalcSubresource(MipLevelLocked, ArraySliceLocked, NumberOfMipLevels) );

	// copy texture buffer to main texture ONLY if buffer was write
	if (LastMapDirection && D3D11_MAP_WRITE)
	{
		Context->CopyResource( Texture, TextureBuffer );
	}
}

u32 CD3D11Texture::getNumberOfArraySlices() const
{
	return NumberOfArraySlices;
}

//! Regenerates the mip map levels of the texture. Useful after locking and
//! modifying the texture
void CD3D11Texture::regenerateMipMapLevels(void* mipmapData)
{
	if( SRView && HardwareMipMaps )
		Context->GenerateMips( SRView );
}

void CD3D11Texture::createRenderTarget(const ECOLOR_FORMAT format)
{
	HRESULT hr = S_OK;

	// are texture size restrictions there ?
	if(!Driver->queryFeature(EVDF_TEXTURE_NPOT))
	{
		if (Size != OriginalSize)
			os::Printer::log("RenderTarget size has to be a power of two", ELL_INFORMATION);
	}

	Size = Size.getOptimalSize(!Driver->queryFeature(EVDF_TEXTURE_NPOT), !Driver->queryFeature(EVDF_TEXTURE_NSQUARE), true, Driver->getMaxTextureSize().Width);

	DXGI_FORMAT d3dformat = Driver->getD3DColorFormat();

	if(ColorFormat == ECF_UNKNOWN)
	{
		// get irrlicht format from backbuffer
		// (This will get overwritten by the custom format if it is provided, else kept.)
		ColorFormat = Driver->getColorFormat();
		setPitch(d3dformat);

		// Use color format if provided.
		if(format != ECF_UNKNOWN)
		{
			ColorFormat = format;
			d3dformat = Driver->getD3DFormatFromColorFormat(format);
			setPitch(d3dformat); // This will likely set pitch to 0 for now.
		}
	}
	else
	{
		d3dformat = Driver->getD3DFormatFromColorFormat(ColorFormat);
	}

	switch (ColorFormat)
	{
	case ECF_A8R8G8B8:
	case ECF_A1R5G5B5:
	case ECF_A16B16G16R16F:
	case ECF_A32B32G32R32F:
		HasAlpha = true;
		break;
	default:
		break;
	}

	// creating texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(D3D11_TEXTURE2D_DESC) );
	desc.ArraySize = NumberOfArraySlices;
	desc.CPUAccessFlags = 0;
	desc.Format = d3dformat;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = SampleCount;
	desc.SampleDesc.Quality = SampleQuality;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	// test if Direct3D support automatic mip map generation
	if (Driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS) && 
		Driver->querySupportForColorFormat(d3dformat, D3D11_FORMAT_SUPPORT_MIP_AUTOGEN))
	{
		desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 0;
	}
	else
	{
		//desc.MiscFlags = 0;
		desc.MipLevels = 1;
	}

	// If array size == 6, force cube texture
	if (desc.ArraySize == 6)
	{
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	}

	// If multisampled, mip levels shall be 1
	if (desc.SampleDesc.Count > 1)
	{
		desc.MiscFlags &= ~D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 1;
	}
	
	// Texture size
	desc.Width = Size.Width;
	desc.Height = Size.Height;

	// create texture
	hr = Device->CreateTexture2D( &desc, NULL, &Texture );
	if (FAILED(hr))
	{
		logFormatError(hr, "Could not create render target texture");

		return;
	}

	// Get texture description to update some fields
	Texture->GetDesc( &desc );
	NumberOfMipLevels = desc.MipLevels;
	Size.Width = desc.Width;
	Size.Height = desc.Height;

	// create views
	createViews();
}

//! creates the hardware texture
bool CD3D11Texture::createTexture(u32 flags, IImage* image)
{
	HRESULT hr = S_OK;
	OriginalSize = image->getDimension();

	core::dimension2d<u32> optSize = OriginalSize.getOptimalSize(!Driver->queryFeature(EVDF_TEXTURE_NPOT),
															  !Driver->queryFeature(EVDF_TEXTURE_NSQUARE), 
															  true,
															  Driver->getMaxTextureSize().Width);

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Color format for DX 10 driver shall be different that for DX 9
	// - B5G5R5A1 family is deprecated in DXGI, and doesn't exists in DX 10
	// - Irrlicht color format follows DX 9 (alpha first), and DX 10 is alpha last
	switch(getTextureFormatFromFlags(flags))
	{
		case ETCF_ALWAYS_16_BIT:
		case ETCF_ALWAYS_32_BIT:
			format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
		case ETCF_OPTIMIZED_FOR_QUALITY:
		{
			switch(image->getColorFormat())
			{
			case ECF_R16F:
				format = DXGI_FORMAT_R16_FLOAT;
				break;

			case ECF_R32F:
				format = DXGI_FORMAT_R32_FLOAT;
				break;

			case ECF_G16R16F:
				format = DXGI_FORMAT_R16G16_FLOAT;
				break;

			case ECF_G32R32F:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;

			case ECF_A16B16G16R16F:
				format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				break;

			case ECF_A32B32G32R32F:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;

			case ECF_A1R5G5B5:
			case ECF_R5G6B5:
			case ECF_R8G8B8:
			case ECF_A8R8G8B8:
			default:
				format = DXGI_FORMAT_R8G8B8A8_UNORM; 
				break;
			}
		}
		break;
		case ETCF_OPTIMIZED_FOR_SPEED:
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		default:
			break;
	}

	// Check hardware support for automatic mipmap support
	if(HasMipMaps && Driver->queryFeature(EVDF_MIP_MAP_AUTO_UPDATE))
	{
		UINT support = 0;
		Device->CheckFormatSupport(format, &support);

		if( support && D3D11_FORMAT_SUPPORT_MIP_AUTOGEN )
			HardwareMipMaps = true;
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(D3D11_TEXTURE2D_DESC) );
	desc.ArraySize = NumberOfArraySlices;
	desc.CPUAccessFlags = 0;
	desc.Format = format;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = SampleCount;
	desc.SampleDesc.Quality = SampleQuality;
	desc.Width = optSize.Width;
	desc.Height = optSize.Height;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	// test if Direct3D support automatic mip map generation
	// AND creation flag is true
	if (HasMipMaps && Driver->querySupportForColorFormat(format, D3D11_FORMAT_SUPPORT_MIP_AUTOGEN))
	{
		desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 0;
	}
	else
	{
		desc.MipLevels = 1;		// Set only one mip level if do not support auto mip generation
	}

	// If array size == 6, force cube texture
	if (desc.ArraySize == 6)
	{
		desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	}

	// If multisampled, mip levels shall be 1
	if (desc.SampleDesc.Count > 1)
	{
		desc.MiscFlags &= ~D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 1;
	}

	// create texture
	hr = Device->CreateTexture2D( &desc, NULL, &Texture );
	if (FAILED(hr))
	{
		logFormatError(hr, "Could not create texture");

		return false;
	}

	// get color format
	ColorFormat = Driver->getColorFormatFromD3DFormat(format);

	switch (ColorFormat)
	{
	case ECF_A8R8G8B8:
	case ECF_A1R5G5B5:
	case ECF_DXT1:
	case ECF_DXT2:
	case ECF_DXT3:
	case ECF_DXT4:
	case ECF_DXT5:
	case ECF_A16B16G16R16F:
	case ECF_A32B32G32R32F:
		HasAlpha = true;
		break;
	default:
		break;
	}

	setPitch(format);

	// Get texture description to update number of mipmaps
	Texture->GetDesc( &desc );
	NumberOfMipLevels = desc.MipLevels;
	Size.Width = desc.Width;
	Size.Height = desc.Height;

	// create views to bound texture to pipeline
	return createViews();
}

//! copies the image to the texture
bool CD3D11Texture::copyTexture(IImage* image)
{
	void* ptr = lock();
	if (ptr)
		image->copyToScaling(ptr, Size.Width, Size.Height, ColorFormat, Pitch);
	unlock();
						
	return true;
}

void CD3D11Texture::setPitch(DXGI_FORMAT d3dformat)
{
	Pitch = Driver->getBitsPerPixel(d3dformat) * Size.Width;
}

bool CD3D11Texture::createTextureBuffer()
{
	if (!Texture)
	{
		os::Printer::log("Error creating texture buffer: main texture is null", ELL_ERROR);
		return false;
	}
	
	D3D11_TEXTURE2D_DESC desc;
	Texture->GetDesc( &desc );

	desc.BindFlags = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = Device->CreateTexture2D( &desc, NULL, &TextureBuffer );
	if (FAILED(hr))
	{
		logFormatError(hr, "Could not create texture buffer");

		return false;
	}

	// sync main texture contents with texture buffer
	Context->CopyResource( TextureBuffer, Texture );

	return true;
}

bool CD3D11Texture::createViews()
{
	if (!Texture)
		return false;

	HRESULT hr = S_OK;
	DXGI_FORMAT format = Driver->getD3DFormatFromColorFormat( ColorFormat );

	// create render target view only if needed
	if(IsRenderTarget)
	{
		if(RTView)
			RTView->Release();

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		::ZeroMemory( &rtvDesc, sizeof( rtvDesc ) );
		rtvDesc.Format = format;

		// check if texture is array and/or multisampled
		if (SampleCount > 1 && NumberOfArraySlices > 1)		// multisampled array
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
			rtvDesc.Texture2DMSArray.ArraySize = NumberOfArraySlices;
			rtvDesc.Texture2DMSArray.FirstArraySlice = 0;
		}
		else if (SampleCount > 1)	// only multisampled
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		}
		else if (NumberOfArraySlices > 1)	// only array
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.ArraySize = NumberOfArraySlices;
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
			rtvDesc.Texture2DArray.MipSlice = 0;
		}
		else	// simple texture
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
		}

		hr = Device->CreateRenderTargetView( Texture, &rtvDesc, &RTView );
		if (FAILED(hr))
		{
			logFormatError(hr, "Could not create render target view");

			return false;
		}
	}

	// create shader resource view
	if(SRView)
		SRView->Release();

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	::ZeroMemory( &srvDesc, sizeof( srvDesc ) );
	srvDesc.Format = format;

	// check if texture is array and/or multisampled
	if (SampleCount > 1 && NumberOfArraySlices > 1)		// multisampled array
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
		srvDesc.Texture2DMSArray.ArraySize = NumberOfArraySlices;
	}
	else if (SampleCount > 1)	// only multisampled
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else if (NumberOfArraySlices > 1)	// only array
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.ArraySize = NumberOfArraySlices;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.MipLevels = NumberOfMipLevels;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
	}
	else	// simple texture
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = NumberOfMipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;	
	}
		
	hr = Device->CreateShaderResourceView( Texture, &srvDesc, &SRView );
	if (FAILED(hr))
	{
		logFormatError(hr, "Could not create shader resource view");

		return false;
	}

	return true;
}

}
}

#endif;