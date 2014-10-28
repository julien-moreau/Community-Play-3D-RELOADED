/*****************************************************************************

        BitMask.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 55134

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



#if defined (ohm_math_BitMask_CURRENT_CODEHEADER)
   #error Recursive inclusion of BitMask code header.
#endif
#define  ohm_math_BitMask_CURRENT_CODEHEADER

#if ! defined (ohm_math_BitMask_CODEHEADER_INCLUDED)
#define  ohm_math_BitMask_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>
#include <climits>



namespace ohm
{
namespace math
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Generates a bit mask with nbr_bits bits set.
template <class T>
T  BitMask <T>::make (int nbr_bits)
{
   assert (nbr_bits > 0);
   assert (nbr_bits <= int (sizeof (T) * CHAR_BIT));

   // This decomposition is intended to avoid the MSVC warning when nbr_bits
   // is equal to the T bitdepth.
   const int      nbh_1 = nbr_bits >> 1;
   const int      nbh_2 = nbr_bits - nbh_1;
   const T        one (1);
   const T        h_1 = (one << nbh_1) - 1;
   const T        h_2 = (one << nbh_2) - 1;

   return ((h_1 << nbh_2) + h_2);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace math
}  // namespace ohm



#endif   // ohm_math_BitMask_CODEHEADER_INCLUDED

#undef ohm_math_BitMask_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
