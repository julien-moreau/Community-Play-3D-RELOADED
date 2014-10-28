/*****************************************************************************

        FloatMultiPrecScan.h
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30449

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



#if ! defined (ohm_math_FloatMultiPrecScan_HEADER_INCLUDED)
#define  ohm_math_FloatMultiPrecScan_HEADER_INCLUDED
#pragma once

#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/math/FloatMultiPrec.h"



namespace ohm
{
namespace math
{



template <int NW>
class FloatMultiPrecScan
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef FloatMultiPrec <NW>   FloatType;

                  FloatMultiPrecScan ();
   virtual        ~FloatMultiPrecScan () {}

   void           start (int base);
   void           add_digit (int digit);
   FloatType      get_number (int exponent) const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   FloatType      _base;
   FloatType      _recomposed;      // Number as currently printed, multiplied by _recomp_div
   int            _nbr_digits;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  FloatMultiPrecScan (const FloatMultiPrecScan <NW> &other);
   FloatMultiPrecScan <NW> &
                  operator = (const FloatMultiPrecScan <NW> &other);
   bool           operator == (const FloatMultiPrecScan <NW> &other) const;
   bool           operator != (const FloatMultiPrecScan <NW> &other) const;

}; // class FloatMultiPrecScan



}  // namespace math
}  // namespace ohm



#include "ohm/math/FloatMultiPrecScan.hpp"



#endif   // ohm_math_FloatMultiPrecScan_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
