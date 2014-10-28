/*****************************************************************************

        Array.hpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 13777

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



#if defined (ohm_mem_Array_CURRENT_CODEHEADER)
   #error Recursive inclusion of Array code header.
#endif
#define  ohm_mem_Array_CURRENT_CODEHEADER

#if ! defined (ohm_mem_Array_CODEHEADER_INCLUDED)
#define  ohm_mem_Array_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <new>

#include <cassert>



namespace ohm
{
namespace mem
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: operator []
Description: Access one array element
Input parameters:
   - pos: Element position, [0 ; LENGTH-1]
Returns: Reference on requested data
Throws: Nothing
==============================================================================
*/

template <class T, long LENGTH>
const typename Array <T, LENGTH>::Element &  Array <T, LENGTH>::operator [] (long pos) const
{
   assert (pos >= 0);
   assert (pos < LENGTH);

   return (_data [pos]);
}



template <class T, long LENGTH>
typename Array <T, LENGTH>::Element &  Array <T, LENGTH>::operator [] (long pos)
{
   assert (pos >= 0);
   assert (pos < LENGTH);

   return (_data [pos]);
}



/*
==============================================================================
Name: size
Description:
   Return the array size (capacity, number of elements)
Returns: The number of elements.
Throws: Nothing
==============================================================================
*/

template <class T, long LENGTH>
long  Array <T, LENGTH>::size ()
{
   return (LENGTH);
}



/*
==============================================================================
Name: length
Description: Return the array size (capacity, number of elements)
Returns: The number of elements.
Throws: Nothing
==============================================================================
*/

template <class T, long LENGTH>
long  Array <T, LENGTH>::length ()
{
   return (size ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace mem
}  // namespace ohm



#endif   // ohm_mem_Array_CODEHEADER_INCLUDED

#undef ohm_mem_Array_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
