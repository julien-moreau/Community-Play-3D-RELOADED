/*****************************************************************************

        endian.hpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 5142

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



#if defined (ohm_archi_endian_CURRENT_CODEHEADER)
   #error Recursive inclusion of endian code header.
#endif
#define  ohm_archi_endian_CURRENT_CODEHEADER

#if ! defined (ohm_archi_endian_CODEHEADER_INCLUDED)
#define  ohm_archi_endian_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace archi
{



/*==========================================================================*/
/*      Name: preserve_endian                                               */
/*      Description: Returns a data as it is coded in memory. Very similar  */
/*                   to *reinterpret_cast <Type *> (&x), excepted we don't  */
/*                   need a temporary memory storage. Thus, these functions */
/*                   are useful to initialize arrays with explicit BE/LE    */
/*                   data.                                                  */
/*      Input parameters:                                                   */
/*        - x : Data to convert.                                            */
/*      Returns: Data, byte-reversed or not, depending on the input data.   */
/*      Throws: Nothing.                                                    */
/*==========================================================================*/

Int16 preserve_endian (Int16 x)
{
   return (x);
}

UInt16   preserve_endian (UInt16 x)
{
   return (x);
}

Int32 preserve_endian (Int32 x)
{
   return (x);
}

UInt32   preserve_endian (UInt32 x)
{
   return (x);
}

Int64 preserve_endian (Int64 x)
{
   return (x);
}

Int16 preserve_endian (Int16Rev x)
{
   return (revert_byte_order_s16 (x));
}

UInt16   preserve_endian (UInt16Rev x)
{
   return (revert_byte_order_u16 (x));
}

Int32 preserve_endian (Int32Rev x)
{
   return (revert_byte_order_s32 (x));
}

UInt32   preserve_endian (UInt32Rev x)
{
   return (revert_byte_order_u32 (x));
}

Int64 preserve_endian (Int64Rev x)
{
   return (revert_byte_order_s64 (x));
}



}  // namespace archi
}  // namespace ohm



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_archi_endian_CODEHEADER_INCLUDED

#undef ohm_archi_endian_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
