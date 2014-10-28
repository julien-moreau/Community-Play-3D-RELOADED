/*****************************************************************************

        fnc.hpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 26059

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



#if defined (ohm_archi_fnc_CURRENT_CODEHEADER)
   #error Recursive inclusion of fnc code header.
#endif
#define  ohm_archi_fnc_CURRENT_CODEHEADER

#if ! defined (ohm_archi_fnc_CODEHEADER_INCLUDED)
#define  ohm_archi_fnc_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>
#include <cstddef>



namespace ohm
{
namespace archi
{



/*==========================================================================*/
/*      Name: revert_byte_order_*                                           */
/*      Description: Revert input data byte order. Little endian data are   */
/*                   converted to Big Endian and vice-versa.                */
/*      Input parameters:                                                   */
/*        - x : data whose byte are to revert.                              */
/*      Returns: Data, byte reversed                                        */
/*      Throws: Nothing.                                                    */
/*==========================================================================*/

Int16 revert_byte_order_s16 (Int16 x)
{
   return (   (x << 8)
           + ((x >> 8) & 0xFF));
}



UInt16   revert_byte_order_u16 (UInt16 x)
{
   return (  (x << 8)
           + (x >> 8));
}



Int32 revert_byte_order_s32 (Int32 x)
{
   return (   (x << 24)
           + ((x <<  8) & 0x00FF0000)
           + ((x >>  8) & 0x0000FF00)
           + ((x >> 24) & 0x000000FF));
}



UInt32   revert_byte_order_u32 (UInt32 x)
{
   return (   (x << 24)
           + ((x <<  8) & 0x00FF0000)
           + ((x >>  8) & 0x0000FF00)
           +  (x >> 24));
}



Int64 revert_byte_order_s64 (Int64 x)
{
   const Int64 hi_64 = Int64 (revert_byte_order_s32 (Int32 (x))) << 32;
   const UInt32 lo_32 = UInt32 (revert_byte_order_u32 (UInt32 (x >> 32)));
   const Int64 mask = ohm_archi_DECL_INT64 (0xFFFFFFFF);
   const Int64 lo_64 = lo_32 & mask;
   const Int64 ret_val = hi_64 + lo_64;
   
   return (ret_val);
}



UInt64   revert_byte_order_u64 (UInt64 x)
{
   const UInt64 hi_64 = UInt64 (revert_byte_order_u32 (UInt32 (x))) << 32;
   const UInt32 lo_32 = UInt32 (revert_byte_order_u32 (UInt32 (x >> 32)));
   const UInt64 mask = ohm_archi_DECL_UINT64 (0xFFFFFFFF);
   const UInt64 lo_64 = lo_32 & mask;
   const UInt64 ret_val = hi_64 + lo_64;
   
   return (ret_val);
}



template <class T>
bool  is_ptr_aligned (const T *ptr)
{
   return (is_ptr_aligned (ptr, sizeof (T)));
}

template <class T>
bool  is_ptr_aligned (const T *ptr, int align)
{
   assert (align > 0);
   assert ((align & -align) == align);

   return ((reinterpret_cast <ptrdiff_t> (ptr) & (align - 1)) == 0);
}



template <class T>
bool  is_ptr_aligned_nonzero (const T *ptr)
{
   return (is_ptr_aligned_nonzero (ptr, sizeof (T)));
}

template <class T>
bool  is_ptr_aligned_nonzero (const T *ptr, int align)
{
   assert (align > 0);
   assert ((align & -align) == align);

   return (ptr != 0 && is_ptr_aligned (ptr, align));
}



}  // namespace archi
}  // namespace ohm



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_archi_fnc_CODEHEADER_INCLUDED

#undef ohm_archi_fnc_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
