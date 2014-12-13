// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#define _IRR_DONT_DO_MEMORY_DEBUGGING_HERE

#include "CD3D11Driver.h"
#include "CD3D11HardwareBuffer.h"
#include "os.h"

namespace irr
{
namespace video
{

CD3D11HardwareBuffer::CD3D11HardwareBuffer(CD3D11Driver* driver, E_HARDWARE_BUFFER_TYPE type, 
							E_HARDWARE_BUFFER_ACCESS accessType, u32 size, u32 flags, const void* initialData)
: Device(0)
, Context(0)
, Buffer(0)
, UAView(0)
, SRView(0)
, Driver(driver)
, TempStagingBuffer(0)
, UseTempStagingBuffer(false)
, LastMapDirection((D3D11_MAP)0)
, Size(size)
, Type(type)
, Flags(flags)
, AccessType(accessType)
{
#ifdef _DEBUG
	setDebugName("CD3D11HardwareBuffer");
#endif

	Device=driver->getExposedVideoData().D3D11.D3DDev11;
	if (Device)
	{
		Device->AddRef();
		Device->GetImmediateContext( &Context );
	}

	createInternalBuffer(initialData);

	// set need of staging buffer
	if (AccessType == EHBA_DYNAMIC && AccessType == EHBA_IMMUTABLE)
		UseTempStagingBuffer = true;
}

CD3D11HardwareBuffer::~CD3D11HardwareBuffer()
{
	if(TempStagingBuffer)
		TempStagingBuffer->drop();
	
	if(SRView)
		SRView->Release();

	if(UAView)
		UAView->Release();

	if(Buffer)
		Buffer->Release();

	if(Context)
		Context->Release();

	if(Device)
		Device->Release();
}

//! Lock function.
void* CD3D11HardwareBuffer::lock(bool readOnly)
{
	if (!Buffer)
		return 0;

	// Define map direction
	switch( AccessType )
	{
	case EHBA_DYNAMIC:
		if( readOnly )
			return 0;
		LastMapDirection = D3D11_MAP_WRITE_DISCARD;		// always write with DX 11 dynamic buffers
		break;

	case EHBA_IMMUTABLE:
		if( !readOnly )
			return 0;		// Cannot be mapped for read			
		LastMapDirection = D3D11_MAP_READ;
		break;

	case EHBA_SYSTEM_MEMORY:
		LastMapDirection = (D3D11_MAP)(D3D11_MAP_WRITE | D3D11_MAP_READ);
		break;

	default:
		if( readOnly )
			LastMapDirection = D3D11_MAP_READ;
		else
			LastMapDirection = (D3D11_MAP)(D3D11_MAP_WRITE | D3D11_MAP_READ);
		break;
	}

	// If using staging, return its pointer
	if(UseTempStagingBuffer)
	{
		if( !TempStagingBuffer )
			TempStagingBuffer = new CD3D11HardwareBuffer(Driver, EHBT_SYSTEM, EHBA_SYSTEM_MEMORY, 0, Size, 0);

		// Copy to staging if needs to read data
		if (LastMapDirection && D3D11_MAP_READ)
			TempStagingBuffer->copyFromBuffer(this, 0, 0, Size);

		return TempStagingBuffer->lock(readOnly);
	}

	// Otherwise, map this buffer
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = Context->Map(Buffer, 0, LastMapDirection, 0, &mappedData);
	if (FAILED(hr))
		return 0;

	return mappedData.pData;
}

//! Unlock function. Must be called after a lock() to the buffer.
void CD3D11HardwareBuffer::unlock()
{
	if (!Buffer)
		return;

	// If using staging, return its pointer
	if(UseTempStagingBuffer)
	{
		TempStagingBuffer->unlock();
		
		// If write, copy staging to this
		if (LastMapDirection && D3D11_MAP_WRITE)
			this->copyFromBuffer(TempStagingBuffer, 0, 0, Size);

		return;
	}

	// Otherwise, unmap this
	Context->Unmap(Buffer, 0);
}

//! Copy data from system memory
void CD3D11HardwareBuffer::copyFromMemory(const void* sysData, u32 offset, u32 length)
{
	if( AccessType == EHBA_IMMUTABLE )
		return;		// Immutable cannot be modified

	if (Buffer)
	{
		D3D11_BOX box;
		box.left = offset;
		box.top = 0;
		box.front = 0;
		box.right = length;
		box.bottom = 1;
		box.back = 1;
		Context->UpdateSubresource(Buffer, 0, &box, sysData, 0, 0);
	}
}

//! Copy data from another buffer
void CD3D11HardwareBuffer::copyFromBuffer(IHardwareBuffer* buffer, u32 srcOffset, u32 destOffset, u32 length)
{
	if( AccessType == EHBA_IMMUTABLE )
		return;		// Immutable cannot be modified

	if (!Buffer)
		return;

	if (buffer->getDriverType() != EDT_DIRECT3D11)
	{
		os::Printer::log("Fatal Error: Tried to copy data from a buffer not owned by this driver.", ELL_ERROR);
		return;
	}

	CD3D11HardwareBuffer* srcBuffer = static_cast<CD3D11HardwareBuffer*>(buffer);

	// try fast copy if possible
	if (srcOffset == 0 && destOffset == 0 && length == Size 
		&& Size == buffer->size() )
	{
		Context->CopyResource( Buffer, srcBuffer->getBufferResource() );
	}
	else	// else, copy subregion
	{
		D3D11_BOX srcBox;
		srcBox.left = (UINT)srcOffset;
		srcBox.right = (UINT)srcOffset + length;
		srcBox.top = 0;
		srcBox.bottom = 1;
		srcBox.front = 0;
		srcBox.back = 1;

		Context->CopySubresourceRegion(Buffer, 0, (UINT)destOffset, 0, 0, 
												srcBuffer->getBufferResource(), 0, &srcBox);
	}
}

//! Get size of buffer in bytes
u32 CD3D11HardwareBuffer::size() const
{
	return Size;
}

//! Get driver type of buffer.
E_DRIVER_TYPE CD3D11HardwareBuffer::getDriverType() const
{
	return EDT_DIRECT3D11;
}

//! Get type of buffer.
E_HARDWARE_BUFFER_TYPE CD3D11HardwareBuffer::getType() const
{
	return Type;
}

u32 CD3D11HardwareBuffer::getFlags() const
{
	return Flags;
}

//! return DX 11 buffer
ID3D11Buffer* CD3D11HardwareBuffer::getBufferResource() const
{
	return Buffer;
}

//! return unordered access view
ID3D11UnorderedAccessView* CD3D11HardwareBuffer::getUnorderedAccessView() const
{
	return UAView;
}

ID3D11ShaderResourceView* CD3D11HardwareBuffer::getShaderResourceView() const
{
	return SRView;
}

bool CD3D11HardwareBuffer::createInternalBuffer(const void* initialData)
{
	HRESULT hr = 0;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = Size;
	desc.StructureByteStride = 0;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags =	(AccessType == EHBA_DYNAMIC)		? D3D11_CPU_ACCESS_WRITE :
							(AccessType == EHBA_SYSTEM_MEMORY)	? D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE :
																0;
	
	desc.Usage = (AccessType == EHBA_DYNAMIC)		? D3D11_USAGE_DYNAMIC	:
				 (AccessType == EHBA_DEFAULT)		? D3D11_USAGE_DEFAULT	:
				 (AccessType == EHBA_IMMUTABLE)		? D3D11_USAGE_IMMUTABLE	:
													  D3D11_USAGE_STAGING;

	desc.BindFlags = 0;

	// Check bind flags
	switch(Type)
	{
	case EHBT_VERTEX:
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;

	case EHBT_INDEX:
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		break;

	case EHBT_STREAM_OUTPUT:
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		break;

	case EHBT_COMPUTE:
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		break;

	case EHBT_SHADER_RESOURCE:
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		break;

	case EHBT_CONSTANTS:
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		break;

	case EHBT_SYSTEM:
		desc.BindFlags = 0;
		break;
	}

	// Load initial data
	D3D11_SUBRESOURCE_DATA* data = 0;
	if (initialData)
	{
		data = new D3D11_SUBRESOURCE_DATA();
		data->pSysMem = initialData;
		data->SysMemPitch = 0;
		data->SysMemSlicePitch = 0;
	}

	// Create buffer
	hr = Device->CreateBuffer(&desc, data, &Buffer);
	if (FAILED(hr))
	{
		os::Printer::log("Error creating hardware buffer", ELL_ERROR);
		return false;
	}

	// If buffer is of type shader resource, create view
	if (Type == EHBT_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		SRVDesc.Buffer.ElementOffset = 0;
		SRVDesc.Buffer.ElementWidth = desc.ByteWidth / 4;

		hr = Device->CreateShaderResourceView(Buffer, &SRVDesc, &SRView);
		if (FAILED(hr))
		{
			os::Printer::log("Error creating shader resource view for buffer", ELL_ERROR);
			return false;
		}
	}

	// If buffer if of type compute, create view
	if (Type == EHBT_COMPUTE )
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;

		if (Driver->queryFeature(EVDF_COMPUTING_SHADER_5_0))
		{
			UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
			UAVDesc.Buffer.NumElements = desc.ByteWidth / 4;	// size in floats
		}
		else if (Driver->queryFeature(EVDF_COMPUTING_SHADER_4_0))
		{
			UAVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
			UAVDesc.Buffer.NumElements = desc.ByteWidth;		// size in bytes
		}

		hr = Device->CreateUnorderedAccessView(Buffer, &UAVDesc, &UAView);
		if (FAILED(hr))
		{
			os::Printer::log("Error creating unordered access view for buffer", ELL_ERROR);
			return false;
		}
	}

	return true;
}

}
}

#endif