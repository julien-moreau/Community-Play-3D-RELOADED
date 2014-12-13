
#ifndef __C_DIRECTX11_HARDWARE_BUFFER_H_INCLUDED__
#define __C_DIRECTX11_HARDWARE_BUFFER_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_WINDOWS_

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#include "IHardwareBuffer.h"

namespace irr
{
namespace video
{

class CD3D11Driver;

class CD3D11HardwareBuffer : public IHardwareBuffer
{
	// Implementation of public methods
public:
	CD3D11HardwareBuffer(CD3D11Driver* driver, E_HARDWARE_BUFFER_TYPE type, E_HARDWARE_BUFFER_ACCESS accessType, 
									u32 size, u32 flags, const void* initialData = 0);

	~CD3D11HardwareBuffer();

	//! Lock function.
	virtual void* lock(bool readOnly = false);

	//! Unlock function. Must be called after a lock() to the buffer.
	virtual void unlock();

	//! Copy data from system memory
	virtual void copyFromMemory(const void* sysData, u32 offset, u32 length);

	//! Copy data from another buffer
	virtual void copyFromBuffer(IHardwareBuffer* buffer, u32 srcOffset, u32 descOffset, u32 length);

	//! Get size of buffer in bytes
	virtual u32 size() const;

	//! Get driver type of buffer.
	virtual E_DRIVER_TYPE getDriverType() const;

	//! Get type of buffer.
	virtual E_HARDWARE_BUFFER_TYPE getType() const;

	//! Get flags
	virtual u32 getFlags() const;

	// Methods for Direct3D 11 implementation
public:
	//! return DX 11 buffer
	ID3D11Buffer* getBufferResource() const;

	//! return unordered access view
	ID3D11UnorderedAccessView* getUnorderedAccessView() const;

	//! return shader resource view
	ID3D11ShaderResourceView* getShaderResourceView() const;

private:
	friend class CD3D11Driver;

	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	ID3D11Buffer* Buffer;
	ID3D11UnorderedAccessView* UAView;
	ID3D11ShaderResourceView* SRView;

	CD3D11Driver* Driver;
	CD3D11HardwareBuffer* TempStagingBuffer;

	bool UseTempStagingBuffer;
	D3D11_MAP LastMapDirection;
	u32 Size;
	E_HARDWARE_BUFFER_TYPE Type;
	E_HARDWARE_BUFFER_ACCESS AccessType;
	u32 Flags;

	bool createInternalBuffer(const void* initialData);
};

}
}

#endif
#endif
#endif