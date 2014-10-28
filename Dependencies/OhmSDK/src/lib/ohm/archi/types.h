/*****************************************************************************

        types.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 22896

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



#if defined (ohm_archi_types_CURRENT_HEADER)
   #error Recursive inclusion of ohm/archi/types header.
#endif
#define  ohm_archi_types_CURRENT_HEADER

#if ! defined (ohm_archi_types_HEADER_INCLUDED)
#define  ohm_archi_types_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if    (ohm_archi_PROC == ohm_archi_PROC_X86)   \
    && (   (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC) \
        || (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC))
   #include <xmmintrin.h>
#endif

#include <climits>



// Char type check
#if CHAR_BIT != 8
   #error 'char' type is not 8 bits-wide !
#endif   // CHAR_BITS



namespace ohm
{
namespace archi
{



// Bytes
typedef  signed char          SByte;
typedef  unsigned char        UByte;



// 16-bit integers
#if USHRT_MAX != 0xFFFFU
   #error 'short' type is not 16 bits-wide !
#endif

typedef  short                Int16;
typedef  unsigned short       UInt16;



// 32-bit integers
#if UINT_MAX != 0xFFFFFFFFU
   #error 'int' type is not 32 bits-wide !
#endif

typedef  int                  Int32;
typedef  unsigned int         UInt32;



// 64-bit integers
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   typedef  signed __int64             Int64;
   typedef  unsigned __int64           UInt64;

   #define  ohm_archi_DECL_INT64(x)    (x##i64)
   #define  ohm_archi_DECL_UINT64(x)   (x##ui64)

#elif    (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC) \
      || (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_CW)

   typedef  signed long long           Int64;
   typedef  unsigned long long         UInt64;

   #define  ohm_archi_DECL_INT64(x)    (x##ll)
   #define  ohm_archi_DECL_UINT64(x)   (x##ull)

#else
   #error No 64-bit integer type defined for this compiler !
#endif



const Int64 UINT64_MAX_VAL = ohm_archi_DECL_UINT64 (0xFFFFFFFFFFFFFFFF);
const Int64 INT64_MAX_VAL = ohm_archi_DECL_INT64 (0x7FFFFFFFFFFFFFFF);
const Int64 INT64_MIN_VAL = ohm_archi_DECL_INT64 (0x8000000000000000);

// As it is a macro and will be expanded where it is used, we have to specify
// the full Int64 scope.
#define  ohm_archi_MAKE_INT64( hi, lo)    ((static_cast <ohm::archi::Int64> (hi) << 32) + static_cast <unsigned long> (lo))
#define  ohm_archi_MAKE_UINT64( hi, lo)   ((static_cast <ohm::archi::UInt64> (hi) << 32) + static_cast <unsigned long> (lo))



// Floating-point types
typedef  float                Float32;
typedef  double               Float64;



// SIMD vector types
#if (ohm_archi_PROC == ohm_archi_PROC_X86)
   typedef  __m128   Vect128;
#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)
   typedef  vector float   Vect128;
#else
   #error No SIMD 128-bit vector type defined for this platform !
#endif



}  // namespace archi
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_archi_types_HEADER_INCLUDED

#undef ohm_archi_types_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
