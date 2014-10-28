/*****************************************************************************

        fnc.hpp
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



#if defined (ohm_math_fnc_CURRENT_CODEHEADER)
   #error Recursive inclusion of fnc code header.
#endif
#define  ohm_math_fnc_CURRENT_CODEHEADER

#if ! defined (ohm_math_fnc_CODEHEADER_INCLUDED)
#define  ohm_math_fnc_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/lang/def.h"
#include "ohm/lang/fnc.h"
#include "ohm/math/def.h"

#include <cassert>
#include <climits>
#include <cfloat>
#include <cmath>



namespace std {}

namespace ohm
{
namespace math
{



/*
==============================================================================
Name: round
Description:
   Rounds to the nearest integer: -0.5 <= round (x) - x < 0.5
Input parameters:
   - x: value to be rounded.
Returns: The rounded value
Throws: Nothing
==============================================================================
*/

double   round (double x)
{
   using namespace std;

   return (floor (x + 0.5));
}



/*
==============================================================================
Name: round_int
Description:
   Rounds to the nearest integer, then converts to integer type.
   -0.5 <= round_int (x) - x < 0.5
Input parameters:
   - x: Value to be rounded. Should be in the [INT_MIN ; INT_MAX] range.
Returns: Rounded value.
Throws: Nothing
==============================================================================
*/

int   round_int (double x)
{
   assert (x <= double (INT_MAX));
   assert (x >= double (INT_MIN));

   return (static_cast <int> (round (x)));
}



archi::Int64   round_int64 (double x)
{
   assert (x >= double (archi::INT64_MIN_VAL));
   assert (x <= double (archi::INT64_MAX_VAL));

   using namespace std;

   return (static_cast <archi::Int64> (floor (x + 0.5)));
}



int   floor_int (double x)
{
   assert (x <= double (INT_MAX));
   assert (x >= double (INT_MIN));

   using namespace std;

   return (static_cast <int> (floor (x)));
}



archi::Int64   floor_int64 (double x)
{
   assert (x >= double (archi::INT64_MIN_VAL));
   assert (x <= double (archi::INT64_MAX_VAL));

   using namespace std;

   return (static_cast <archi::Int64> (floor (x)));
}



int   ceil_int (double x)
{
   assert (x <= double (INT_MAX));
   assert (x >= double (INT_MIN));

   using namespace std;

   return (static_cast <int> (ceil (x)));
}



archi::Int64   ceil_int64 (double x)
{
   assert (x >= double (archi::INT64_MIN_VAL));
   assert (x <= double (archi::INT64_MAX_VAL));

   using namespace std;

   return (static_cast <archi::Int64> (ceil (x)));
}



// T must be an integer type.
template <class T>
bool  have_different_signs (T x, T y)
{
   return (((x ^ y) >> (sizeof (x) * CHAR_BIT - 1)) != 0);
}



// T must be an integer type.
template <class T>
T  div_int_floor (T x, T y)
{
   assert (y > 0);

   if (x < 0)
   {
      x -= y - 1;
   }

   return (x / y);
}



// T must be an integer type.
template <class T>
T  div_int_ceil (T x, T y)
{
   assert (y > 0);

   if (x > 0)
   {
      x += y - 1;
   }

   return (x / y);
}



/*
==============================================================================
Name: is_null
Description:
   Check if a floating point value can be considered as 0. Actually compares
   the number absolute value with a very small number (EPSILON).
Input parameters:
   - x: Value to be tested
Returns: true if the value is close to 0.
Throws: Nothing
==============================================================================
*/

bool  is_null (double x)
{
   using namespace std;

   return (fabs (x) < EPSILON);
}



/*
==============================================================================
Name: is_equal
Description:
   Tests loose equality, taking into account rounding errors and
   real number representation issues.
Input parameters:
   - x: Number to compare to y
   - y: Number to compare to x
Returns: true if numbers are almost equal.
Throws: Nothing
==============================================================================
*/

bool  is_equal (double x, double y)
{
   return (is_equal_tol (x, y, EPSILON));
}



bool  is_equal_tol (double x, double y, double d)
{
   using namespace std;
   
   assert (d >= 0);

   return (is_equal_delta (x, y, fabs (x) * d));
}



bool  is_equal_delta (double x, double y, double d)
{
   assert (d >= 0);

   using namespace std;

   return (fabs (x - y) <= d);
}



bool  is_denormal (const float &f)
{
   ohm_lang_CHECK_CST (SizeofIntEqSizeofFloat, sizeof (int) == sizeof (float));
   const int      x = *reinterpret_cast <const int *> (&f);

   return (   (x & 0x7f800000) == 0
           && (x & 0x807FFFFF) != 0);
}



bool  is_finite (const double &d)
{
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   using namespace std;
   return (_finite (d) != 0);

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

   return (std::isfinite (d));

#else
   #error To be implemented for this compiler
#endif
}



bool  is_nan (const double &d)
{
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   using namespace std;
   return (_isnan (d) != 0);

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

   return (std::isnan (d));

#else
   #error To be implemented for this compiler
#endif
}



double   sinc (double x)
{
   if (x == 0)
   {
      return (1);
   }

   using namespace std;

   const double   pi_x = x * PI;

   return (sin (pi_x) / pi_x);
}



double   acosh (double x)
{
   assert (x >= 1);

   using namespace std;

   const double   tmp = x + sqrt (x*x - 1);
   assert (tmp > 0);

   return (log (tmp));
}



double   asinh (double x)
{
   using namespace std;

   const double   tmp = x + sqrt (x*x + 1);
   assert (tmp > 0);

   return (log (tmp));
}



double   atanh (double x)
{
   assert (fabs (x) < 1);

   const double   tmp = (1 + x) / (1 - x);
   assert (tmp > 0);

   using namespace std;

   return (0.5 * log (tmp));
}



// U must be a signed integer type
template <class T, class U>
T  ipow (T x, U n)
{
   const U        abs_n = lang::abs (n);
   const T        z (ipowp (x, abs_n));

   return ((n < U (0)) ? T (1) / z : z);
}



// U must be an integer type (signed or not)
template <class T, class U>
T  ipowp (T x, U n)
{
   assert (! (n < U (0)));

   T              z (1);
   while (n != U (0))
   {
      if ((n & U (1)) != U (0))
      {
         z *= x;
      }
      n >>= 1;
      x *= x;
   }

   return (z);
}



/*
==============================================================================
Name: pseudo_exp
Description:
   Computes a pseudo-exponential function for a value x. This function reaches
   0 for x = 0 and 1 for x = 1. Function is C-oo and strictly increasing.
   The function formula is:
      f(x) = (p^(cx) - 1) / (p^c - 1)
   Near 1 the function is equivalent to:
      f(x) ~ p^(cx) / p^c
   Near 0 the function is equivalent to:
      f(x) ~ x * c * log (p) / p^c
Input parameters:
   - x: Number to exponentiate. >= 0
   - p: Exponential base. > 0
   - c: Function curvature. The smaller c, the more linear the function.
      The bigger c, the more exponential-like the function. > 0.
Returns: Function value, >= 0.
Throws: Nothing
==============================================================================
*/

double   pseudo_exp (double x, double p, double c)
{
   assert (x >= 0);
   assert (p > 0);
   assert (c > 0);

   using namespace std;

   const double   clogp = c * log (p);
   const double   num = exp (clogp * x) - 1;
   const double   den = exp (clogp) - 1;

   return (num / den);
}



double   pseudo_expe (double x, double c)
{
   assert (x >= 0);
   assert (c > 0);

   using namespace std;

   const double   num = exp (c * x) - 1;
   const double   den = exp (c) - 1;

   return (num / den);
}



double   pseudo_exp2 (double x, double c)
{
   assert (x >= 0);
   assert (c > 0);

   using namespace std;

   const double   clogp = c * LN2;
   const double   num = exp (clogp * x) - 1;
   const double   den = exp (clogp) - 1;

   return (num / den);
}



double   pseudo_exp10 (double x, double c)
{
   assert (x >= 0);
   assert (c > 0);

   using namespace std;

   const double   clogp = c * LN10;
   const double   num = exp (clogp * x) - 1;
   const double   den = exp (clogp) - 1;

   return (num / den);
}



/*
==============================================================================
Name: pseudo_log
Description:
   Inverse function for pseudo_exp(). Performs:
      g(y) = log (y * (p^c-1) + 1) / (c * log (p))
Input parameters:
   - x: Number to logarithmise. >= 0
   - p: Exponential base. > 0
   - c: Function curvature. The smaller c, the more linear the function.
      The bigger c, the more exponential-like the function. > 0.
Returns: Function value, >= 0.
Throws: Nothing
==============================================================================
*/

double   pseudo_log (double y, double p, double c)
{
   assert (y >= 0);
   assert (p > 0);
   assert (c > 0);

   using namespace std;

   const double   clogp = c * log (p);
   const double   num = log (y * (exp (clogp) - 1) + 1);

   return (num / clogp);
}



double   pseudo_loge (double y, double c)
{
   assert (y >= 0);
   assert (c > 0);

   using namespace std;

   const double   num = log (y * (exp (c) - 1) + 1);

   return (num / c);
}



double   pseudo_log2 (double y, double c)
{
   assert (y >= 0);
   assert (c > 0);

   using namespace std;

   const double   clogp = c * LN2;
   const double   num = log (y * (exp (clogp) - 1) + 1);

   return (num / clogp);
}



double   pseudo_log10 (double y, double c)
{
   assert (y >= 0);
   assert (c > 0);

   using namespace std;

   const double   clogp = c * LN10;
   const double   num = log (y * (exp (clogp) - 1) + 1);

   return (num / clogp);
}



}  // namespace math
}  // namespace ohm



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_math_fnc_CODEHEADER_INCLUDED

#undef ohm_math_fnc_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
