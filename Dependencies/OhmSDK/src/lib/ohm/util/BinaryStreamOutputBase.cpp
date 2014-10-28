/*****************************************************************************

        BinaryStreamOutputBase.cpp
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
#include "ohm/util/BinaryStreamOutputBase.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const archi::UByte * BinaryStreamOutputBase::data () const
{
   return (do_data ());
}



size_t   BinaryStreamOutputBase::size () const
{
   return (do_size ());
}



bool  BinaryStreamOutputBase::empty () const
{
   return (do_empty ());
}



void  BinaryStreamOutputBase::clear ()
{
   do_clear ();
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

BinaryStreamOutputBase::BinaryStreamOutputBase ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BinaryStreamOutputBase::~BinaryStreamOutputBase ()
{
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (bool val)
{
   write (archi::UByte (val ? 1 : 0));
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (archi::UByte val)
{
   archi::UByte * data_ptr = do_resize_of (1);
   
   data_ptr [0] = val;
}



/*
==============================================================================
Name: write
==============================================================================
*/

void BinaryStreamOutputBase::write (archi::SByte val)
{
   write (reinterpret_cast <archi::UByte &> (val));
}



/*
==============================================================================
Name: write
==============================================================================
*/

void BinaryStreamOutputBase::write (archi::UInt16 val)
{
   archi::UByte * data_ptr = do_resize_of (2);
   
   data_ptr [0] = (val >> 8) & 255;
   data_ptr [1] = (val >> 0) & 255;
}



/*
==============================================================================
Name: write
==============================================================================
*/

void BinaryStreamOutputBase::write (archi::Int16 val)
{
   write (reinterpret_cast <archi::UInt16 &> (val));
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (archi::UInt32 val)
{
   archi::UByte * data_ptr = do_resize_of (4);
   
   data_ptr [0] = (val >> 24) & 255;
   data_ptr [1] = (val >> 16) & 255;
   data_ptr [2] = (val >> 8) & 255;
   data_ptr [3] = (val >> 0) & 255;
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (archi::Int32 val)
{
   write (reinterpret_cast <archi::UInt32 &> (val));
}

   
   
/*
==============================================================================
Name: write
==============================================================================
*/

void BinaryStreamOutputBase::write (archi::UInt64 val)
{
   archi::UInt32 val0 = archi::UInt32 (val >> 32LL);
   archi::UInt32 val1 = archi::UInt32 (val & 0xFFFFFFFFLL);
   
   write (val0);
   write (val1);
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (archi::Int64 val)
{
   write (reinterpret_cast <archi::UInt64 &> (val));
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (archi::Float32 val)
{
   union {archi::Float32 _f; archi::UInt32 _d;} uval;
   uval._f = val;
   
   write (uval._d);
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (archi::Float64 val)
{
   union {archi::Float64 _f; archi::Int64 _d;} uval;
   uval._f = val;
   
   write (uval._d);
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (void * val)
{
#if defined (ohm_archi_WORD_SIZE)
   #if (ohm_archi_WORD_SIZE == 32)
      write (reinterpret_cast <archi::UInt32> (val));

   #elif (ohm_archi_WORD_SIZE == 64)
      write (reinterpret_cast <archi::UInt64> (val));

   #else
      #error ohm_archi_WORD_SIZE size not supported !
      
   #endif

#else
   #error ohm_archi_WORD_SIZE not defined !

#endif
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (const void * val)
{
#if defined (ohm_archi_WORD_SIZE)
   #if (ohm_archi_WORD_SIZE == 32)
      write (reinterpret_cast <archi::UInt32> (val));

   #elif (ohm_archi_WORD_SIZE == 64)
      write (reinterpret_cast <archi::UInt64> (val));

   #else
      #error ohm_archi_WORD_SIZE size not supported !
      
   #endif

#else
   #error ohm_archi_WORD_SIZE not defined !

#endif
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (const std::string & str)
{
   archi::UInt32 str_size = archi::UInt32 (str.size ());
   write (str_size);
   
   write (reinterpret_cast <const archi::UByte *> (str.c_str ()), str.size ());
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  BinaryStreamOutputBase::write (const archi::UByte * data_ptr, size_t data_size)
{
   do_resize_write_bytes (data_ptr, data_size);
}



/*
==============================================================================
Name : write_front
==============================================================================
*/

void  BinaryStreamOutputBase::write_front (const archi::UByte * data_ptr, size_t data_size)
{
   do_write_front_bytes (data_ptr, data_size);
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (bool val, size_t pos)
{
   overwrite (archi::UByte (val ? 1 : 0), pos);
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (archi::UByte val, size_t pos)
{
   archi::UByte * data_ptr = do_resize_max_of (pos, 1);
   
   data_ptr [0] = val;
}



/*
==============================================================================
Name: overwrite
==============================================================================
*/

void BinaryStreamOutputBase::overwrite (archi::SByte val, size_t pos)
{
   overwrite (reinterpret_cast <archi::UByte &> (val), pos);
}



/*
==============================================================================
Name: overwrite
==============================================================================
*/

void BinaryStreamOutputBase::overwrite (archi::UInt16 val, size_t pos)
{
   archi::UByte * data_ptr = do_resize_max_of (pos, 2);
   
   data_ptr [0] = (val >> 8) & 255;
   data_ptr [1] = (val >> 0) & 255;
}



/*
==============================================================================
Name: overwrite
==============================================================================
*/

void BinaryStreamOutputBase::overwrite (archi::Int16 val, size_t pos)
{
   overwrite (reinterpret_cast <archi::UInt16 &> (val), pos);
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (archi::UInt32 val, size_t pos)
{
   archi::UByte * data_ptr = do_resize_max_of (pos, 4);
   
   data_ptr [0] = (val >> 24) & 255;
   data_ptr [1] = (val >> 16) & 255;
   data_ptr [2] = (val >> 8) & 255;
   data_ptr [3] = (val >> 0) & 255;
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (archi::Int32 val, size_t pos)
{
   overwrite (reinterpret_cast <archi::UInt32 &> (val), pos);
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (archi::UInt64 val, size_t pos)
{
   archi::UInt32 val0 = archi::UInt32 (val >> 32LL);
   archi::UInt32 val1 = archi::UInt32 (val & 0xFFFFFFFFLL);
   
   overwrite (val0, pos);
   overwrite (val1, pos + sizeof (val0));
}

   
/*
==============================================================================
Name: overwrite
==============================================================================
*/

void BinaryStreamOutputBase::overwrite (archi::Int64 val, size_t pos)
{
   overwrite (reinterpret_cast <archi::UInt64 &> (val), pos);
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (archi::Float32 val, size_t pos)
{
   union {archi::Float32 _f; archi::UInt32 _d;} uval;
   uval._f = val;
   
   overwrite (uval._d, pos);
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (archi::Float64 val, size_t pos)
{
   union {archi::Float64 _f; archi::Int64 _d;} uval;
   uval._f = val;
   
   overwrite (uval._d, pos);
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (const std::string & str, size_t pos)
{
   archi::UInt32 str_size = archi::UInt32 (str.size ());
   overwrite (str_size, pos);
   
   overwrite (
      reinterpret_cast <const archi::UByte *> (str.c_str ()), str.size (),
      pos + sizeof (str_size)
   );
}



/*
==============================================================================
Name : overwrite
==============================================================================
*/

void  BinaryStreamOutputBase::overwrite (const archi::UByte * data_ptr, size_t data_size, size_t pos)
{
   using namespace std;
   
   archi::UByte * buf_ptr = do_resize_max_of (pos, data_size);
   
   memcpy (buf_ptr, data_ptr, data_size);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
