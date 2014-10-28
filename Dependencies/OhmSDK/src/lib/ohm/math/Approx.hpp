/*****************************************************************************

        Approx.hpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 71518

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



#if ! defined (ohm_math_Approx_CODEHEADER_INCLUDED)
#define  ohm_math_Approx_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/math/fnc.h"

#include <cassert>
#include <cmath>

#if (   (ohm_archi_PROC == ohm_archi_PROC_X86)  \
     && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC) \
     && (ohm_archi_WORD_SIZE == 64))
#include <intrin.h>
#include <mmintrin.h>
#include <xmmintrin.h>

#elif ( (ohm_archi_PROC == ohm_archi_PROC_PPC)  \
     && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC))
#include <ppc_intrinsics.h>

#endif

// architecture proc & compiler class

#if   ((ohm_archi_PROC == ohm_archi_PROC_X86)   \
   && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC))
   #define ohm_math_APPROX_X86_VC

#elif ((ohm_archi_PROC == ohm_archi_PROC_X86)   \
   && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC))
   #define ohm_math_APPROX_X86_GCC

#else
   #undef ohm_math_APPROX_X86_VC
   #undef ohm_math_APPROX_X86_GCC

#endif



namespace ohm
{
namespace math
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



int   Approx::round_int (double x)
{
   assert (x <= double (INT_MAX));
   assert (x >= double (INT_MIN));

#if defined (ohm_math_APPROX_X86_VC)

   assert (x <= double (INT_MAX/2));
   assert (x >= double (INT_MIN/2));

   static const float   round_to_nearest = 0.5f;
   int            i;

#if (ohm_archi_WORD_SIZE == 64)

   const double   xx = x + x + round_to_nearest;
   const __m128d  x_128d = _mm_set_sd (xx);
   i = _mm_cvtsd_si32 (x_128d);
   i >>= 1;

#else

   __asm
   {
      fld            x
      fadd           st, st (0)
      fadd           round_to_nearest
      fistp          i
      sar            i, 1
   }

#endif

   assert (i == math::round_int (x));

   return (i);

#elif defined (ohm_math_APPROX_X86_GCC)

   assert (x <= double (INT_MAX/2));
   assert (x >= double (INT_MIN/2));

   int            i;

   static const float   round_to_nearest = 0.5f;
   
   const double   xx = x + x + round_to_nearest;
   
   asm (
      "fldl          %[x]           \n"
      "fistpl        %[i]           \n"
      "sarl          $1, %[i]       \n"
   :  [i]   "=m"  (i)
   :  [x]   "m"   (xx)
   :  "st"
   );

   assert (i == math::round_int (x));

   return (i);

#else

   // Slow
   return (math::round_int (x));

#endif
}



// Doesn't give the right result for very small negative values.
int   Approx::floor_int (double x)
{
   assert (x <= double (INT_MAX));
   assert (x >= double (INT_MIN));

#if defined (ohm_math_APPROX_X86_VC)

   assert (x <= double (INT_MAX/2));
   assert (x >= double (INT_MIN/2));

   int            i;
   static const float   round_toward_m_i = -0.5f;

#if (ohm_archi_WORD_SIZE == 64)

   const double   xx = x + x + round_toward_m_i;
   const __m128d  x_128d = _mm_set_sd (xx);
   i = _mm_cvtsd_si32 (x_128d);
   i >>= 1;

#else

   __asm
   {
      fld            x
      fadd           st, st (0)
      fadd           round_toward_m_i
      fistp          i
      sar            i, 1
   }

#endif

   using namespace std;
   assert (i == math::floor_int (x) || fabs (i - x) < 1e-10);

   return (i);

#elif defined (ohm_math_APPROX_X86_GCC)

   assert (x <= double (INT_MAX/2));
   assert (x >= double (INT_MIN/2));

   int            i;
   
   static const float   round_toward_m_i = -0.5f;
   
   const double   xx = x + x + round_toward_m_i;
   
   asm (
      "fldl          %[x]           \n"
      "fistpl        %[i]           \n"
      "sarl          $1, %[i]       \n"
   :  [i]   "=m"  (i)
   :  [x]   "m"   (xx)
   :  //"st"
   );
   
   using namespace std;
   assert (i == math::floor_int (x) || fabs (i - x) < 1e-10);

   return (i);

#else

   // Slow
   return (math::floor_int (x));

#endif
}



// Doesn't give the right result for very small positive values.
int   Approx::ceil_int (double x)
{
   assert (x <= double (INT_MAX));
   assert (x >= double (INT_MIN));

#if defined (ohm_math_APPROX_X86_VC)

   assert (x <= double (INT_MAX/2));
   assert (x >= double (INT_MIN/2));

   int            i;
   static const float   round_toward_p_i = -0.5f;

#if (ohm_archi_WORD_SIZE == 64)

   const double   xx = round_toward_p_i - (x + x);
   const __m128d  x_128d = _mm_set_sd (xx);
   i = _mm_cvtsd_si32 (x_128d);
   i >>= 1;

#else

   __asm
   {
      fld            x
      fadd           st, st (0)
      fsubr          round_toward_p_i
      fistp          i
      sar            i, 1
   }

#endif

   using namespace std;
   assert (-i == math::ceil_int (x) || fabs (-i - x) < 1e-10);

   return (-i);

#elif defined (ohm_math_APPROX_X86_GCC)

   assert (x <= double (INT_MAX/2));
   assert (x >= double (INT_MIN/2));

   int            i;

   static const float   round_toward_p_i = -0.5f;
   
   const double   xx = round_toward_p_i - (x + x);
   
   asm (
      "fldl          %[x]           \n"
      "fistpl        %[i]           \n"
      "sarl          $1, %[i]       \n"
   :  [i]   "=m"  (i)
   :  [x]   "m"   (xx)
   :  //"st"
   );

   using namespace std;
   assert (-i == math::ceil_int (x) || fabs (-i - x) < 1e-10);

   return (-i);

#else

   // Slow
   return (math::ceil_int (x));

#endif
}



/*
==============================================================================
Name: flt_to_int
Description:
   Rounds a value to an integer as quickly as possible. Exact rounding method
   is undefined, however maximum difference between original and rounded
   values is guaranteed to be 1 or less.
Input parameters:
   - x: Value to be rounded. Should be in the [INT_MIN ; INT_MAX] range.
Returns: Rounded value.
Throws: Nothing
==============================================================================
*/

int   Approx::flt_to_int (float x)
{
   int            val;

#if defined (ohm_math_APPROX_X86_VC)

   #if (ohm_archi_WORD_SIZE == 64)

   val = flt_to_int (double (x));

   #else

   __asm
   {
      fld            x;
      fistp          val;
   }

   #endif

#elif defined (ohm_math_APPROX_X86_GCC)

// __asm__ ("fistl %0" : "=m" (val) : "t" (x));
   asm (
      "flds          %[x]     \n"
      "fistpl        %[v]     \n"
   :  [v]   "=m"  (val)
   :  [x]   "m"   (x)
   :  
   );

#else

   // Slow
   val = int (x);

#endif

   return (val);
}



int   Approx::flt_to_int (double x)
{
   int            val;

#if defined (ohm_math_APPROX_X86_VC)

   #if (ohm_archi_WORD_SIZE == 64)

   const __m128d  x_128d = _mm_set_sd (x);
   val = _mm_cvtsd_si32 (x_128d);

   #else

   __asm
   {
      fld            x;
      fistp          val;
   }

   #endif

#elif defined (ohm_math_APPROX_X86_GCC)

// __asm__ ("fistl %0" : "=m" (val) : "t" (x));
   asm (
      "fldl          %[x]     \n"
      "fistpl        %[v]     \n"
   :  [v]   "=m"  (val)
   :  [x]   "m"   (x)
   :  
   );

#else

   // Slow
   val = int (x);

#endif

   return (val);
}



float Approx::log2 (float val)
{
   assert (val > 0);

   int * const    exp_ptr = reinterpret_cast <int *> (&val);
   int            x = *exp_ptr;
   const int      log_2 = ((x >> 23) & 255) - 128;
   x &= ~(255 << 23);
   x += 127 << 23;
   *exp_ptr = x;

   const float    k = 0.5f;
   const float    a = (k - 1) / (k + 1);
   const float    b = (4 - 2*k) / (k + 1);   // 1 - 3*a
   const float    c = 2*a;
   val = (a * val + b) * val + c;

   return (val + log_2);
}



float Approx::exp2 (float val)
{
   // Store address of float as long pointer
   long *            p = (long *) (&val);

   // Truncated val for integer power of 2
   const long     tx = floor_int (val);

   // Float remainder of power of 2
   val -= static_cast <float> (tx);

   // Quadratic approximation of 2^x in [0 ; 1]
   const float    a = 1.0f / 3.0f;
   const float    b = 2.0f / 3.0f;
   const float    c = 1.0f;
   val = (a * val + b) * val + c;

   // Add integer power of 2 to exponent
   *p += (tx << 23);

   assert (val >= 0);

   return (val);
}



/*
==============================================================================
Name: exp2_part_u16_u32_o2
Description:
   Computes a fixed-point approximation of the following function:
   [0 ; 1[ -> [0.5 ; 1[
     f : x -> 2^(x-1)
   The approximation uses the following polynomial:
   f(x) = (((x+1)^2) + 2) / 6
Input parameters:
   - val: Unsigned linear input, Fixed-point 0:16 data. Integer part
      is automatically discarded.
Returns: Unsigned exponential output, fixed-point 0:32 data, range is
   [ 0x80000000 ; 0xFFFFFFFF ]
Throws: Nothing
==============================================================================
*/

archi::UInt32  Approx::exp2_part_u16_u32_o2 (int val)
{
   archi::UInt32  result;

#if (defined (ohm_math_APPROX_X86_VC) && (ohm_archi_WORD_SIZE == 32))

   __asm
   {
      mov            eax, val
      shl            eax, 15           ; eax = pitch_frac [31/31 bits]
      or             eax, 080000000h   ; eax = pitch_frac + 1  [32/31 bits]
      mul            eax
      mov            eax, edx          ; eax = (pitch_frac + 1) ^ 2 [32/30 bits]
      mov            edx, 2863311531   ; 2/3 [32 bits], rounded to +oo
      mul            edx               ; eax = 2/3 (pitch_frac + 1) ^ 2 [32/30 bits]
      add            edx, 1431655766   ; + 4/3 [30 bits] + 1
      mov            result, edx
   }

#elif defined (ohm_math_APPROX_X86_GCC)

   __asm__ (
      "shll          $15,           %0          \n"
      "orl           $0x80000000,   %0          \n"
      "movl          %0,            %%eax       \n"
      "mull          %%eax                      \n"
      "movl          %%edx,         %%eax       \n"
      "movl          $2863311531,   %%edx       \n"
      "mull          %%edx                      \n"
      "addl          $1431655766,   %%edx       \n"
      "movl          %%edx,         %1          \n"
      : "=r"(result)
      : "0" (val)
      :  "%eax", "%edx"
   );
   
#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)
   
   result = val;
   result <<= 15;
   result |= 0x80000000;
   result = __mulhwu (result, result);
   result = __mulhwu (result, 2863311531UL);
   result += 1431655766UL;

#else

   const int      resol = 16;
   const archi::UInt32  mask = (1 << resol) - 1;

   val &= mask;
   archi::Int64   step_64 (val + (1 << resol));
   step_64 *= step_64;
   step_64 *= (1 << (62 - (resol + 1) * 2)) / 3 + 1820;
   result = (archi::UInt32) (step_64 >> (62 - 32 - 1));
   result += 1431641208;
   
#endif
   
   assert ((result & 0x80000000UL) != 0);

   return (result);
}



// Way more accurate (error < 8e-5 -> ~0.08 cent)
// Polynomial approximation by Olli Niemitalo
// y = (((1/150 * x + 2/75) * x + 3/25) * x + 26/75) * x + 0.5
archi::UInt32  Approx::exp2_part_u16_u32_o4 (int val)
{
   archi::UInt32  result;

#if (defined (ohm_math_APPROX_X86_VC) && (ohm_archi_WORD_SIZE == 32))
   __asm
   {
      mov            ecx, val
      shl            ecx, 16
      mov            edx, 001B4E81Bh   ; 1/150
      mov            eax, ecx
      mul            edx               ; edx = 1/150 * x
      add            edx, 006D3A06Dh   ; 2/75
      mov            eax, edx
      mul            ecx               ; (1/150 * x + 2/75) * x
      add            edx, 01EB851EBh   ; 3/25
      mov            eax, edx
      mul            ecx               ; (1/150 * x + 2/75) * x + 3/25) * x
      add            edx, 058BF258Ch   ; 26/75
      mov            eax, edx
      mul            ecx               ; (((1/150 * x + 2/75) * x + 3/25) * x + 26/75) * x
      add            edx, 080000000h   ; (((1/150 * x + 2/75) * x + 3/25) * x + 26/75) * x + 0.5
      mov            result, edx
   }

#elif defined (ohm_math_APPROX_X86_GCC)

   __asm__ (
      "shll          $16,           %0          \n"
      "movl          $0x01B4E81B,   %%edx       \n"
      "movl          %0,            %%eax       \n"
      "mull          %%edx                      \n"
      "addl          $0x06D3A06D,   %%edx       \n"
      "movl          %%edx,         %%eax       \n"
      "mull          %0                         \n"
      "addl          $0x1EB851EB,   %%edx       \n"
      "movl          %%edx,         %%eax       \n"
      "mull          %0                         \n"
      "addl          $0x58BF258C,   %%edx       \n"
      "movl          %%edx,         %%eax       \n"
      "mull          %0                         \n"
      "addl          $0x80000000,   %%edx       \n"
      "movl          %%edx,         %1          \n"
      : "=r"(result)
      : "0" (val)
      :  "%eax", "%edx"
   );
   
#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)
   
   archi::UInt32  x;

   x = val;
   x <<= 16;
   result = __mulhwu (x, 0x01B4E81B);
   result += 0x06D3A06D;
   result = __mulhwu (result, x);
   result += 0x1EB851EB;
   result = __mulhwu (result, x);
   result += 0x58BF258C;
   result = __mulhwu (result, x);
   result += 0x80000000;

#else

   const archi::UInt32  x_32 = static_cast <archi::UInt32> (val) << 16;
   result = static_cast <archi::UInt32> ((archi::UInt64 (x_32) * 0x001B4E81B) >> 32);
   result += 0x06D3A06D;
   result = static_cast <archi::UInt32> ((result * archi::UInt64 (x_32)) >> 32);
   result += 0x1EB851EB;
   result = static_cast <archi::UInt32> ((result * archi::UInt64 (x_32)) >> 32);
   result += 0x58BF258C;
   result = static_cast <archi::UInt32> ((result * archi::UInt64 (x_32)) >> 32);
   result += 0x80000000;

#endif
   
   assert ((result & 0x80000000UL) != 0);

   return (result);
}



// Valid range is ]-pi/2 ; +pi/2[
// abs (approx/real - 1) < 7e-4 over the specified range.
double   Approx::tan (double x)
{
   assert (x > -PI * 0.5);
   assert (x < +PI * 0.5);

   const double   poly = compute_tan_poly (x);
   const double   result = x / poly;

   return (result);
}



double   Approx::cot (double x)
{
   assert (x >= -PI * 0.5);
   assert (x <= +PI * 0.5);
   assert (x != 0);

   const double   poly = compute_tan_poly (x);
   const double   result = poly / x;

   return (result);
}



double   Approx::tan_taylor_o5 (double x)
{
   const double   x_2 = x * x;
   const double   c_1 = 1;
   const double   c_3 = 1.0 / 3;
   const double   c_5 = 2.0 / 15;

   return (((c_5 * x_2 + c_3) * x_2 + c_1) * x);
}



// Maximum absolute relative error: 3.0 %
// Continuous up to the 3rd derivate.
double   Approx::tanh_o4 (double x)
{
   const double   xa = fabs (x);
   const double   x2 = x * x;
   const double   x4 = x2 * x2;
   const double   poly = xa + x2 + x4;
   const double   resa = poly / (1 + poly);
   const double   result = (x < 0) ? -resa : resa;

   return (result);
}



// Maximum absolute relative error: 0.4 %
// Polynomial designed for a good balance between harmonic distortion and accuracy:
// amp = 2; order = 6; scanrange = 0.15; nharm = 256; g = logspace (-5, log10 (scanrange), 10001); p = polyfit (g, (1 ./ (1 - tanh (g)) - 1) ./ g, order - 1); p = [p 0]; p(length(p)-2:length(p)-1) = [1 1];n = 65536; cycles = n/(2*nharm); t = linspace (0, (n-1)/n, n); x = sin (cycles*t*2*pi) * amp; yref = tanh (x); y = sign (x) .* (1 - 1 ./ (1 + polyval (p, abs (x)))); tt = (0:(n-1)/2)/cycles; yy = 20*log10 (abs (fft (y))); yy = yy(1:n/2); yyref = 20*log10 (abs (fft (yref))); yyref = yyref (1:n/2); first = max (yyref); yy = yy - first; yyref = yyref - first; subplot (1,2,1); semilogx (tt, yy, tt, yyref); grid on; axis ([0.9 nharm -300 5]); subplot (1,2,2); plot (x, y, x, yref); grid on; p
double   Approx::tanh_o6 (double x)
{
   const double   xa = fabs (x);
   const double   poly = ((((  0.05003809554385  * xa
                             + 0.13246694329321) * xa
                             + 0.33339269594710) * xa
                             + 0.66666493197038) * xa
                             + 1               ) * xa
                             + 1;
   const double   result = (x * poly) / (1 + xa * poly);

   return (result);
}



// Chris Lomont, Fast Inverse Square Root, 2003
// http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
float Approx::rsqrt (float x)
{
   assert (x > 0);

   const float    xhalf = 0.5f * x;
   int            i = *(int *) &x;  // Get bits for floating value
   i = 0x5F375A86 - (i >> 1);       // Gives initial guess y0
   x = *(float *) &i;               // Convert bits back to float
   x = x * (1.5f - xhalf * x * x);  // Newton step, repeating increases accuracy

   return (x);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



double   Approx::compute_tan_poly (const double x)
{
   const double   x2 = x * x;

   // -32.0 / (9 * PI*PI*PI*PI) [x^4] and -28.0 / (9 * PI*PI) [x^2] for the 2-coef version (err < 1%).
   const double   a = -0.06255 * 64 / (PI*PI*PI*PI*PI*PI);
   const double   b = -0.108   * 16 / (PI*PI*PI*PI);
   const double   c = -0.82945 *  4 / (PI*PI);
   const double   d = 1;
   const double   poly = ((a * x2 + b) * x2 + c) * x2 + d;

   return (poly);
}



}  // namespace math
}  // namespace ohm



#endif   // ohm_math_Approx_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
