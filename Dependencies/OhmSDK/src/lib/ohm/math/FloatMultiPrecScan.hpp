/*****************************************************************************

        FloatMultiPrecScan.hpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30384

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



#if ! defined (ohm_math_FloatMultiPrecScan_CODEHEADER_INCLUDED)
#define  ohm_math_FloatMultiPrecScan_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/math/fnc.h"

#include <cassert>



namespace ohm
{
namespace math
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
FloatMultiPrecScan <NW>::FloatMultiPrecScan ()
:  _base ()
,  _recomposed ()
,  _nbr_digits ()
{
   // Nothing
}



template <int NW>
void  FloatMultiPrecScan <NW>::start (int base)
{
   assert (base > 0);

   _base = base;
   _recomposed.clear ();
   _nbr_digits = 0;
}



template <int NW>
void  FloatMultiPrecScan <NW>::add_digit (int digit)
{
   assert (digit >= 0);
   assert (digit < _base.to_dbl ());

   _recomposed *= _base;
   _recomposed += digit;

   ++ _nbr_digits;
}



template <int NW>
typename FloatMultiPrecScan <NW>::FloatType  FloatMultiPrecScan <NW>::get_number (int exponent) const
{
   assert (_nbr_digits > 0);

   const FloatType   mult = ipow (_base, exponent + 1 - _nbr_digits);
   const FloatType   nbr = _recomposed * mult;

   return (nbr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace math
}  // namespace ohm



#endif   // ohm_math_FloatMultiPrecScan_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
