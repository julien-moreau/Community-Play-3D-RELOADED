/*****************************************************************************

        BinaryStreamInput.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70200

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



#if defined (ohm_util_BinaryStreamInput_CURRENT_CODEHEADER)
   #error Recursive inclusion of BinaryStreamInput code header.
#endif
#define  ohm_util_BinaryStreamInput_CURRENT_CODEHEADER

#if ! defined (ohm_util_BinaryStreamInput_CODEHEADER_INCLUDED)
#define  ohm_util_BinaryStreamInput_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/Err.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : read_ubyte
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read_ubyte (T & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UByte val_d = 0;
   int err = read (val_d, error_0);
   
   if (err == 0)
   {
      val = static_cast <T> (val_d);
   }
   
   return err;
}



/*
==============================================================================
Name : read_uint32
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read_uint32 (T & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 val_d = 0;
   int err = read (val_d, error_0);
   
   if (err == 0)
   {
      val = static_cast <T> (val_d);
   }
   
   return err;
}



/*
==============================================================================
Name : read_int32
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read_int32 (T & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::Int32 val_d = 0;
   int err = read (val_d, error_0);
   
   if (err == 0)
   {
      val = static_cast <T> (val_d);
   }
   
   return err;
}



/*
==============================================================================
Name : read_float32
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read_float32 (T & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::Float32 val_d = 0;
   int err = read (val_d, error_0);
   
   if (err == 0)
   {
      val = static_cast <T> (val_d);
   }
   
   return err;
}



/*
==============================================================================
Name : read_float64
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read_float64 (T & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::Float64 val_d = 0;
   int err = read (val_d, error_0);
   
   if (err == 0)
   {
      val = static_cast <T> (val_d);
   }
   
   return err;
}



/*
==============================================================================
Name : read
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read (std::list <T> & elem_arr, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 size = 0;
   int err = read (size, error_0);
   if (err != Err_OK) return err;
   
   for (archi::UInt32 i = 0 ; i < size ; ++i)
   {
      T elem;
      err = read (elem, error_0);
      if (err != Err_OK) return err;
      
      elem_arr.push_back (elem);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read (std::vector <T> & elem_arr, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 size = 0;
   int err = read (size, error_0);
   if (err != Err_OK) return err;
   
   for (archi::UInt32 i = 0 ; i < size ; ++i)
   {
      T elem;
      err = read (elem, error_0);
      if (err != Err_OK) return err;
      
      elem_arr.push_back (elem);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read (std::set <T> & elem_set, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 size = 0;
   int err = read (size, error_0);
   if (err != Err_OK) return err;
   
   for (archi::UInt32 i = 0 ; i < size ; ++i)
   {
      T elem;
      err = read (elem, error_0);
      if (err != Err_OK) return err;
      
      elem_set.insert (elem);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

template <class T, class U>
int   BinaryStreamInput::read (std::map <T, U> & elem_map, const char * error_0)
{
   assert (_data_ptr != 0);
   
   archi::UInt32 size = 0;
   int err = read (size, error_0);
   if (err != Err_OK) return err;
   
   for (archi::UInt32 i = 0 ; i < size ; ++i)
   {
      T key;
      err = read (key, error_0);
      if (err != Err_OK) return err;
      
      U val;
      err = read (val, error_0);
      if (err != Err_OK) return err;
      
      bool ok_flag = elem_map.insert (std::make_pair (key, val)).second;
      assert (ok_flag);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

template <class T, class U>
int   BinaryStreamInput::read (std::pair <T, U> & val, const char * error_0)
{
   assert (_data_ptr != 0);
   
   int err = read (val.first, error_0);
   if (err != Err_OK) return err;
   
   err = read (val.second, error_0);
   
   return err;
}



/*
==============================================================================
Name : read
==============================================================================
*/

template <class T>
int   BinaryStreamInput::read (T & val, const char * error_0)
{
   int err = val.read (*this);
   
   if (err != 0)
   {
      if (error_0) ohm_util_TRACE_ERROR1 ("%1%", error_0);
   }
   
   return err;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



#undef dd_break

#endif   // ohm_util_BinaryStreamInput_CODEHEADER_INCLUDED

#undef ohm_util_BinaryStreamInput_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
