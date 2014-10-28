/*****************************************************************************

        Approx.h
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 31981

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



#if ! defined (ohm_math_Approx_HEADER_INCLUDED)
#define  ohm_math_Approx_HEADER_INCLUDED
#pragma once

#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"



namespace ohm
{
namespace math
{



class Approx
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static ohm_archi_FORCEINLINE int
                  round_int (double x);
   static ohm_archi_FORCEINLINE int
                  floor_int (double x);
   static ohm_archi_FORCEINLINE int
                  ceil_int (double x);

   static ohm_archi_FORCEINLINE int
                  flt_to_int (float x);
   static ohm_archi_FORCEINLINE int
                  flt_to_int (double x);

   static ohm_archi_FORCEINLINE float
                  log2 (float val);
   static ohm_archi_FORCEINLINE float
                  exp2 (float val);
   static ohm_archi_FORCEINLINE archi::UInt32
                  exp2_part_u16_u32_o2 (int val);
   static ohm_archi_FORCEINLINE archi::UInt32
                  exp2_part_u16_u32_o4 (int val);

   static ohm_archi_FORCEINLINE double
                  tan (double x);
   static ohm_archi_FORCEINLINE double
                  cot (double x);
   static ohm_archi_FORCEINLINE double
                  tan_taylor_o5 (double x);

   static ohm_archi_FORCEINLINE double
                  tanh_o4 (double x);
   static ohm_archi_FORCEINLINE double
                  tanh_o6 (double x);

   static ohm_archi_FORCEINLINE float
                  rsqrt (float x);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static ohm_archi_FORCEINLINE double
                  compute_tan_poly (const double x);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  Approx ();
                  ~Approx () {}
                  Approx (const Approx &other);
   Approx &       operator = (const Approx &other);
   bool           operator == (const Approx &other) const;
   bool           operator != (const Approx &other) const;

}; // class Approx



}  // namespace math
}  // namespace ohm



#include "ohm/math/Approx.hpp"



#endif   // ohm_math_Approx_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
