/*****************************************************************************

        BinaryStreamInput.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70845

        This file is part of Ohm Force Libraries.

        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        version 2 as published by the Free Software Foundation;

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program;
        if not, write to the Free Software Foundation, Inc.,
        59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/trace.h"

#include <vector>

#include <cstring>
#include <cassert>



#define OHM_UTIL_BINARY_STREAM_INPUT_DEBUG_FLAG

#if defined (OHM_UTIL_BINARY_STREAM_INPUT_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_UTIL_BINARY_STREAM_INPUT_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define bsi_break static const archi::UByte * __data_stream_ptr__ = &_data_ptr [_pos]; assert (false); __data_stream_ptr__
#else
   #define bsi_break ((void) 0)
#endif



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

BinaryStreamInput::BinaryStreamInput ()
:  _data_ptr (0)
,  _data_size (0)
,  _pos (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

BinaryStreamInput::BinaryStreamInput (const BinaryStreamInput & other)
:  _data_ptr (other._data_ptr)
,  _data_size (other._data_size)
,  _pos (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

BinaryStreamInput::BinaryStreamInput (const archi::UByte * data_ptr, size_t data_size)
:  _data_ptr (data_ptr)
,  _data_size (data_size)
,  _pos (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

BinaryStreamInput::BinaryStreamInput (const std::vector <archi::UByte> & data)
:  _data_ptr (&data [0])
,  _data_size (data.size ())
,  _pos (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BinaryStreamInput::~BinaryStreamInput ()
{
   // nothing
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

BinaryStreamInput &  BinaryStreamInput::operator = (const BinaryStreamInput &other)
{
   _data_ptr = other._data_ptr;
   _data_size = other._data_size;
   _pos = 0;
   
   return *this;
}



/*
==============================================================================
Name : data
==============================================================================
*/

const archi::UByte * BinaryStreamInput::data () const
{
   assert (_data_ptr != 0);
   
   return &_data_ptr [_pos];
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   BinaryStreamInput::size () const
{
   if (_data_ptr != 0)
   {
      return _data_size - _pos;
   }
   else
   {
      return 0;
   }
}


   
/*
==============================================================================
Name : empty
==============================================================================
*/

bool  BinaryStreamInput::empty () const
{
   return size () == 0;
}


   
/*
==============================================================================
Name : seek_beg
Description :
   seek to 'relative_pos' relative to begin of the stream
==============================================================================
*/

int   BinaryStreamInput::seek_beg (size_t relative_pos, const char * error_0)
{
   assert (_data_ptr != 0);
   
   _pos = relative_pos;

   if (_pos > _data_size)
   {
      _pos = _data_size;
      
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : seek_end
Description :
   seek to - 'neg_relative_pos' relative to end of the stream
==============================================================================
*/

int   BinaryStreamInput::seek_end (size_t neg_relative_pos, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (neg_relative_pos > _data_size)
   {
      _pos = 0;
      
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   _pos = _data_size - neg_relative_pos;

   return Err_OK;
}



/*
==============================================================================
Name : advance
==============================================================================
*/

int   BinaryStreamInput::advance (long relative_pos, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (relative_pos > 0)
   {
      _pos += relative_pos;
      
      if (_pos > _data_size)
      {
         _pos = _data_size;
         
         if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
         bsi_break;
         return Err_EOS;
      }
   }
   else if (relative_pos < 0)
   {
      if (_pos < size_t (-relative_pos))
      {
         _pos = 0;
         
         if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
         bsi_break;
         return Err_EOS;
      }
      
      _pos -= size_t (-relative_pos);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : tell
==============================================================================
*/

size_t   BinaryStreamInput::tell () const
{
   assert (_data_ptr != 0);
   
   return _pos;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (bool & val, const char * error_0)
{
   archi::UByte val_d = 0;

   int err = read (val_d, error_0);
   
   if (err == Err_OK)
   {
      val = val_d != 0;
   }
   
   return err;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (archi::UByte & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (_pos + 1 > _data_size)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   val = _data_ptr [_pos];
   ++_pos;
   
   return Err_OK;
}



/*
==============================================================================
Name: read
==============================================================================
*/

int BinaryStreamInput::read (archi::SByte & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (_pos + 1 > _data_size)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   archi::UByte &  tval = reinterpret_cast <archi::UByte &> (val);
   
   tval = _data_ptr [_pos];
   ++_pos;
   
   return Err_OK;
}


   
/*
==============================================================================
Name: read
==============================================================================
*/

int BinaryStreamInput::read (archi::UInt16 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (_pos + 2 > _data_size)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   val |= (_data_ptr [_pos] << 8);
   ++_pos;
   
   val |= (_data_ptr [_pos] << 0);
   ++_pos;
   
   return Err_OK;
}



/*
==============================================================================
Name: read
==============================================================================
*/

int BinaryStreamInput::read (archi::Int16 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (_pos + 2 > _data_size)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   archi::UInt16 & tval = reinterpret_cast <archi::UInt16 &> (val);
   
   tval |= (_data_ptr [_pos] << 8);
   ++_pos;
   
   tval |= (_data_ptr [_pos] << 0);
   ++_pos;
   
   return Err_OK;
}





/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (archi::UInt32 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (_pos + 4 > _data_size)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   val = (_data_ptr [_pos] << 24);
   ++_pos;

   val |= (_data_ptr [_pos] << 16);
   ++_pos;

   val |= (_data_ptr [_pos] << 8);
   ++_pos;

   val |= (_data_ptr [_pos] << 0);
   ++_pos;
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (archi::Int32 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (_pos + 4 > _data_size)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   archi::UInt32 & tval = reinterpret_cast <archi::UInt32 &> (val);
   
   tval = (_data_ptr [_pos] << 24);
   ++_pos;

   tval |= (_data_ptr [_pos] << 16);
   ++_pos;

   tval |= (_data_ptr [_pos] << 8);
   ++_pos;

   tval |= (_data_ptr [_pos] << 0);
   ++_pos;
   
   return Err_OK;
}

   
/*
==============================================================================
Name: read
==============================================================================
*/

int BinaryStreamInput::read (archi::UInt64 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 val0 = 0;
   int err = read (val0, error_0);
   
   if (err != Err_OK)
   {
      return err;
   }
   
   archi::UInt32 val1 = 0;
   err = read (val1, error_0);
   
   if (err != Err_OK)
   {
      return err;
   }
   
   val = archi::UInt64 (val0);
   val <<= 32LL;
   val += archi::UInt64 (val1);
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (archi::Int64 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 val0 = 0;
   int err = read (val0, error_0);
   
   if (err != Err_OK)
   {
      return err;
   }

   archi::UInt32 val1 = 0;
   err = read (val1, error_0);
   
   if (err != Err_OK)
   {
      return err;
   }
   
   archi::UInt64 & tval = reinterpret_cast <archi::UInt64 &> (val);
   
   tval = archi::Int64 (val0);
   tval <<= 32LL;
   tval += archi::Int64 (val1);
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (archi::Float32 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 val0 = 0;
   int err = read (val0, error_0);
   
   if (err != Err_OK)
   {
      return err;
   }

   union {archi::Float32 _f; archi::UInt32 _d;} uval;
   uval._d = val0;
   
   val = uval._f;
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (archi::Float64 & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::Int64 val0 = 0;
   int err = read (val0, error_0);
   
   if (err != Err_OK)
   {
      return err;
   }

   union {archi::Float64 _f; archi::Int64 _d;} uval;
   uval._d = val0;
   
   val = uval._f;
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (void * & val, const char * error_0)
{
#if defined (ohm_archi_WORD_SIZE)
   #if (ohm_archi_WORD_SIZE == 32)
      return read (reinterpret_cast <archi::UInt32 &> (val), error_0);

   #elif (ohm_archi_WORD_SIZE == 64)
      return read (reinterpret_cast <archi::UInt64 &> (val), error_0);

   #else
      #error ohm_archi_WORD_SIZE size not supported !
      
   #endif

#else
   #error ohm_archi_WORD_SIZE not defined !

#endif
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (const void * & val, const char * error_0)
{
#if defined (ohm_archi_WORD_SIZE)
   #if (ohm_archi_WORD_SIZE == 32)
      return read (reinterpret_cast <archi::UInt32 &> (val), error_0);

   #elif (ohm_archi_WORD_SIZE == 64)
      return read (reinterpret_cast <archi::UInt64 &> (val), error_0);

   #else
      #error ohm_archi_WORD_SIZE size not supported !
      
   #endif

#else
   #error ohm_archi_WORD_SIZE not defined !

#endif
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (std::string & str, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 str_size = 0;
   int err = read (str_size, error_0);
   
   if (err != Err_OK)
   {
      return err;
   }

   if (_pos + str_size > _data_size)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   str.assign ((char *) &_data_ptr [_pos], (char *) &_data_ptr [_pos + str_size]);
   
   _pos += str_size;
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   BinaryStreamInput::read (const archi::UByte * & data_ptr, size_t data_size, const char * error_0)
{
   assert (_data_ptr != 0);
   
   if (_pos + data_size > _data_size)
   {
      data_ptr = 0;
      
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
      bsi_break;
      return Err_EOS;
   }
   
   data_ptr = &_data_ptr [_pos];
   _pos += data_size;
   
   return Err_OK;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
