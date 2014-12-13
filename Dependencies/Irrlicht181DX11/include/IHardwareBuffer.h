// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_HARDWARE_BUFFER_H_INCLUDED__
#define __I_HARDWARE_BUFFER_H_INCLUDED__

#include "IReferenceCounted.h"
#include "EDriverTypes.h"

namespace irr
{
namespace video
{

enum E_HARDWARE_BUFFER_TYPE
{
	//! Does not change anything
	EHBT_NONE=0,
	//! Change the vertex mapping
	EHBT_VERTEX,
	//! Change the index mapping
	EHBT_INDEX,
	//! Change the stream output/feedback mapping (stream output also include vertex)
	EHBT_STREAM_OUTPUT,
	//! Change the shader resource mapping
	EHBT_SHADER_RESOURCE,
	//! Change the compute mapping
	EHBT_COMPUTE,
	//! Change the constants mapping
	EHBT_CONSTANTS,
	//! Change the system mapping
	EHBT_SYSTEM
};

enum E_HARDWARE_BUFFER_ACCESS
{
	EHBA_DYNAMIC = 0,
	EHBA_DEFAULT,
	EHBA_IMMUTABLE,
	EHBA_SYSTEM_MEMORY
};

enum E_HARDWARE_BUFFER_FLAGS
{
	EHBF_INDEX_16_BITS = 1,
	EHBF_INDEX_32_BITS = 1 << 1,
	EHBF_VERTEX_ADDITIONAL_BIND = 1 << 2,
	EHBF_INDEX_ADDITIONAL_BIND = 1 << 3,
	EHBF_SHADER_ADDITIONAL_BIND = 1 << 4,
	EHBF_DRAW_INDIRECT_ARGS = 1 << 5,
	EHBF_COMPUTE_RAW = 1 << 6,
	EHBF_COMPUTE_STRUCTURED = 1 << 7,
	EHBF_COMPUTE_APPEND = 1 << 8,
	EHBF_COMPUTE_CONSUME = 1 << 9
};

class IHardwareBuffer : public virtual IReferenceCounted
{
public:

	IHardwareBuffer()
	{
	}

	//! Lock function.
	/** Locks the Buffer and returns a pointer to access the
	data. After lock() has been called and all operations on the data
	are done, you must call unlock().
	Locks are not accumulating, hence one unlock will do for an arbitrary
	number of previous locks.
	\param readOnly Specifies that no changes to the locked buffer are
	made. Unspecified behavior will arise if still write access happens.
	\return Returns a pointer to the data. 0 is returned, if
	the texture cannot be locked. */
	virtual void* lock(bool readOnly = false) = 0;

	//! Unlock function. Must be called after a lock() to the buffer.
	/** One should avoid to call unlock more than once before another lock. */
	virtual void unlock() = 0;

	//! Copy data from system memory
	virtual void copyFromMemory(const void* sysData, u32 offset, u32 length) = 0;

	//! Copy data from another buffer
	virtual void copyFromBuffer(IHardwareBuffer* buffer, u32 srcOffset, u32 descOffset, u32 length) = 0;

	//! Get size of buffer in bytes
	virtual u32 size() const = 0;

	//! Get driver type of buffer.
	/** This is the driver, which created the buffer. This method is used
	internally by the video devices, to check, if they may use a texture
	because textures may be incompatible between different devices.
	\return Driver type of buffer. */
	virtual E_DRIVER_TYPE getDriverType() const = 0;

	//! Get type of buffer.
	virtual E_HARDWARE_BUFFER_TYPE getType() const = 0;

	//! Get flags for buffer
	virtual u32 getFlags() const = 0;

};

}
}

#endif