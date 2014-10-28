/*****************************************************************************

        FloatMultiPrecPrint.h
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30474

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



#if ! defined (ohm_math_FloatMultiPrecPrint_HEADER_INCLUDED)
#define  ohm_math_FloatMultiPrecPrint_HEADER_INCLUDED
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
class FloatMultiPrecPrint
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  FloatMultiPrec <NW>  FloatType;

                  FloatMultiPrecPrint ();
   virtual        ~FloatMultiPrecPrint () {}

   void           set_nbr (const FloatType &val, int base, int req_resol);
   void           set_nbr (double val, int base);
   void           set_nbr (float val, int base);

   int            get_exp () const;

   void           start ();
   bool           is_rem_elt () const;
   void           iterate ();
   int            get_digit () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           prepare ();
   void           iterate_internal ();

   FloatType      _v;
   FloatType      _high;
   FloatType      _low;
   FloatType      _qi;
   FloatType      _base_k;          // _base raised to the power of _k.
   FloatType      _recomposed;      // Number as currently printed, multiplied by _recomp_div
   FloatType      _recomp_div;
   int            _req_resol;       // Source resolution, in bits.
   int            _base;            // > 0, integer
   int            _k;
   int            _cur_dig;         // Current digit. For iterations.
   int            _nbr_dig;         // Number of emitted digits. -1: iteration not started.
   bool           _stop_next_flag;  // Indicates that this is the last iteration.



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  FloatMultiPrecPrint (const FloatMultiPrecPrint <NW> &other);
   FloatMultiPrecPrint <NW> &
                  operator = (const FloatMultiPrecPrint <NW> &other);
   bool           operator == (const FloatMultiPrecPrint <NW> &other) const;
   bool           operator != (const FloatMultiPrecPrint <NW> &other) const;

}; // class FloatMultiPrecPrint



}  // namespace math
}  // namespace ohm



#include "ohm/math/FloatMultiPrecPrint.hpp"



#endif   // ohm_math_FloatMultiPrecPrint_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
