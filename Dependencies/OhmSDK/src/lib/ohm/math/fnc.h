/*****************************************************************************

        fnc.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 64257

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



#if defined (ohm_math_fnc_CURRENT_HEADER)
   #error Recursive inclusion of ohm/math/fnc header.
#endif
#define  ohm_math_fnc_CURRENT_HEADER

#if ! defined (ohm_math_fnc_HEADER_INCLUDED)
#define  ohm_math_fnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"



namespace ohm
{
namespace math
{



inline double  round (double x);
inline int     round_int (double x);
inline archi::Int64  round_int64 (double x);
inline int     floor_int (double x);
inline archi::Int64  floor_int64 (double x);
inline int     ceil_int (double x);
inline archi::Int64  ceil_int64 (double x);

template <class T>
inline bool    have_different_signs (T x, T y);

template <class T>
inline T       div_int_floor (T x, T y);
template <class T>
inline T       div_int_ceil (T x, T y);

inline bool    is_null (double x);
inline bool    is_equal (double x, double y);
inline bool    is_equal_tol (double x, double y, double d);
inline bool    is_equal_delta (double x, double y, double d);
inline bool    is_denormal (const float &f);
inline bool    is_finite (const double &d);
inline bool    is_nan (const double &d);

inline double  sinc (double x);
inline double  acosh (double x);
inline double  asinh (double x);
inline double  atanh (double x);
template <class T, class U>
inline T       ipowp (T x, U n);
template <class T, class U>
inline T       ipow (T x, U n);

inline double  pseudo_exp (double x, double p, double c);
inline double  pseudo_expe (double x, double c);
inline double  pseudo_exp2 (double x, double c);
inline double  pseudo_exp10 (double x, double c);
inline double  pseudo_log (double y, double p, double c);
inline double  pseudo_loge (double y, double c);
inline double  pseudo_log2 (double y, double c);
inline double  pseudo_log10 (double y, double c);

}  // namespace math
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/math/fnc.hpp"



#endif   // ohm_math_fnc_HEADER_INCLUDED

#undef ohm_math_fnc_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
