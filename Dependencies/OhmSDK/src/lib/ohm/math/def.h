/*****************************************************************************

        def.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 18839

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



#if defined (ohm_math_def_CURRENT_HEADER)
   #error Recursive inclusion of ohm/math/def header.
#endif
#define  ohm_math_def_CURRENT_HEADER

#if ! defined (ohm_math_def_HEADER_INCLUDED)
#define  ohm_math_def_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace math
{



const double   EPSILON  = 1e-9;

const double   PI       = 3.1415926535897932384626433832795;

// Exp
const double   EXP1     = 2.7182818284590452353602874713527;

// Log
const double   LN2      = 0.69314718055994530941723212145818;
const double   LN10     = 2.3025850929940456840179914546844;
const double   LOG10_2  = 0.30102999566398119521373889472449;
const double   LOG2_E   = 1.0 / LN2;
const double   LOG2_10  = LN10 / LN2;

// Square root
const double   SQRT2    = 1.41421356237309514547462185873883;
const double   SQRT3    = 1.73205080756887719317660412343685;
const double   SQRT5    = 2.2360679774997898050514777423814;

// Cubic root
const double   CURT2    = 1.25992104989487319066654436028330;
const double   CURT3    = 1.44224957030740852381711647467455;
const double   CURT4    = 1.58740105196819936139718265621923;
const double   CURT5    = 1.70997594667669705614798658643849;

// Power of 2
const double   TWOP32   = 4294967296.0;
const double   TWOPM32  = 1.0 / TWOP32;

// Misc
const float    ANTI_DENORMAL_F32 = 1e-20f;
const double   ANTI_DENORMAL_F64 = 1e-290;



}  // namespace math
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_math_def_HEADER_INCLUDED

#undef ohm_math_def_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
