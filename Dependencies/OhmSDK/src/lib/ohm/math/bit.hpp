/*****************************************************************************

        bit.hpp
        Copyright (c) 2002 Ohm Force

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



#if ! defined (ohm_math_bit_CODEHEADER_INCLUDED)
#define  ohm_math_bit_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (   (ohm_archi_PROC == ohm_archi_PROC_X86)  \
     && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC) \
     && (ohm_archi_COMPILER >= ohm_archi_COMPILER_VC2005))

#include <intrin.h>
#pragma intrinsic (_BitScanReverse)

#if (ohm_archi_WORD_SIZE == 64)
   #pragma intrinsic (_BitScanReverse64)
#endif

#endif   // ohm_archi_PROC_X86 && ohm_archi_COMPILER_CLASS_VC

#include <cassert>



namespace ohm
{
namespace math
{



bool  is_pow_2 (long x)
{
   return ((x & -x) == x);
}



/*
==============================================================================
Name: get_next_pow2
Description:
   Computes the exponent of the power of two equal to or immediately greater
   than the parameter. It is the base-2 log rounded toward plus infinity.
Input parameters:
   - x: Number which we want to compute the base-2 log.
Returns: The exponent
Throws: Nothing
==============================================================================
*/

int   get_next_pow2 (long x)
{
   assert (x > 0);

#if (   (ohm_archi_PROC == ohm_archi_PROC_X86)  \
     && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC))

   #if (ohm_archi_COMPILER < ohm_archi_COMPILER_VC2005)

   int            p;
   -- x;

   if (x == 0)
   {
      p = 0;
   }
   else
   {
      __asm
      {
         xor            eax, eax
         bsr            eax, x
         inc            eax
         mov            p, eax
      }
   }

   #else

   unsigned long  p;
   if (_BitScanReverse (&p, x - 1) == 0)
   {
      p = 0;
   }
   else
   {
      ++ p;
   }

   #endif

#else

   --x;
   int            p = 0;

   while ((x & ~0xFFFFL) != 0)
   {
      p += 16;
      x >>= 16;
   }
   while ((x & ~0xFL) != 0)
   {
      p += 4;
      x >>= 4;
   }
   while (x > 0)
   {
      ++p;
      x >>= 1;
   }

#endif

   return (int (p));
}



/*
==============================================================================
Name: get_prev_pow2
Description:
   Computes the exponent of the power of two equal to or immediately lower
   than the parameter. It is the base-2 log rounded toward minus infinity.
Input parameters:
   - x: Number which we want to compute the base-2 log.
Returns: The exponent
Throws: Nothing
==============================================================================
*/

int   get_prev_pow2 (archi::UInt32 x)
{
   assert (x > 0);

#if (   (ohm_archi_PROC == ohm_archi_PROC_X86)  \
     && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC))

   #if (ohm_archi_COMPILER < ohm_archi_COMPILER_VC2005)

   int            p;
   __asm
   {
      xor            eax, eax
      bsr            eax, x
      mov            p, eax
   }

   #else

   unsigned long  p;
   _BitScanReverse (&p, x);

   #endif

#else

   int            p = -1;

   while ((x & ~(archi::UInt32 (0xFFFF))) != 0)
   {
      p += 16;
      x >>= 16;
   }
   while ((x & ~(archi::UInt32 (0xF))) != 0)
   {
      p += 4;
      x >>= 4;
   }
   while (x > 0)
   {
      ++p;
      x >>= 1;
   }

#endif

   return (int (p));
}



int   get_prev_pow2 (archi::UInt64 x)
{
   assert (x > 0);

#if (   (ohm_archi_PROC == ohm_archi_PROC_X86)  \
     && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC) \
     && (ohm_archi_WORD_SIZE == 64))

   unsigned long  p;
   _BitScanReverse64 (&p, x);

#else

   int            p = -1;

   const archi::UInt32  msw = archi::UInt32 (x >> 32);
   if (msw != 0)
   {
      p = get_prev_pow2 (msw) + 32;
   }
   else
   {
      p = get_prev_pow2 (archi::UInt32 (x));
   }

#endif

   return (int (p));
}



}  // namespace math
}  // namespace ohm



#endif   // ohm_math_bit_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
