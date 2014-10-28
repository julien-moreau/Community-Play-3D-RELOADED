/*****************************************************************************

        Interlocked.cpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68270

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/fnc.h"
#include "ohm/lang/def.h"
#include "ohm/sys/Interlocked.h"

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
#include <intrin.h>
#endif

#include <cassert>
#include <cstddef>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



archi::Int32   Interlocked::exchange_int32 (archi::Int32 volatile &dest, archi::Int32 excg)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   const archi::Int32   old =
      _InterlockedExchange (reinterpret_cast <volatile long *> (&dest), excg);

   return (old);

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

   #error Interlocked::exchange_int32() undefined for this compiler (to do)

#else // Default implementation (slow)

   archi::Int32   old;
   do
   {
      old = dest;
   }
   while (compare_exchange_int32 (dest, excg, old) != old);

   return (old);

#endif   // ohm_archi_COMPILER_CLASS
}



archi::Int32   Interlocked::compare_exchange_int32 (archi::Int32 volatile &dest, archi::Int32 excg, archi::Int32 comp)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   const archi::Int32   old = _InterlockedCompareExchange (
      reinterpret_cast <volatile long *> (&dest),
      excg,
      comp
   );

   return (old);

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

/*
   mov            esi, [dest]
   mov            ecx, [dword ptr excg]
   mov            eax, [dword ptr comp]
   cmpxchg        [esi], ecx
   mov            [dword ptr old], eax
*/

   #error Interlocked::compare_exchange_int32() undefined for this compiler (to do)

#else

   #error Interlocked::compare_exchange_int32() undefined for this compiler

#endif   // ohm_archi_COMPILER_CLASS
}



#undef   ohm_sys_Interlocked_DEST_CAST



#if defined (ohm_archi_HAS_CAS_64)



archi::Int64   Interlocked::exchange_int64 (archi::Int64 volatile &dest, archi::Int64 excg)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   archi::Int64   old;

   #if (ohm_archi_WORD_SIZE == 64)

   old = _InterlockedExchange64 (&dest, excg);

   #else

   __asm
   {
      push           ebx
      mov            esi, [dest]
      mov            ebx, [dword ptr excg    ]
      mov            ecx, [dword ptr excg + 4]

   cas_loop:
      mov            eax, [esi    ]
      mov            edx, [esi + 4]
      lock cmpxchg8b [esi]
      jnz            cas_loop

      mov            [dword ptr old    ], eax
      mov            [dword ptr old + 4], edx
      pop            ebx
   }

   #endif

   return (old);

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

/*
   push           ebx
   mov            esi, [dest]
   mov            ebx, [dword ptr excg    ]
   mov            ecx, [dword ptr excg + 4]

cas_loop:
   mov            eax, [esi    ]
   mov            edx, [esi + 4]
   lock cmpxchg8b [esi]
   jnz            cas_loop

   mov            [dword ptr old    ], eax
   mov            [dword ptr old + 4], edx
   pop            ebx
*/

   #error Interlocked::exchange_int64() undefined for this compiler (to do)

#else // Default implementation (slow)

   archi::Int64   old;
   do
   {
      old = dest;
   }
   while (compare_exchange_int64 (dest, excg, old) != old);

   return (old);

#endif   // ohm_archi_COMPILER_CLASS
}



archi::Int64   Interlocked::compare_exchange_int64 (archi::Int64 volatile &dest, archi::Int64 excg, archi::Int64 comp)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   const archi::Int64   old =
      _InterlockedCompareExchange64 (&dest, excg, comp);

   return (old);

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

/*
   push           ebx
   mov            esi, [dest]
   mov            eax, [dword ptr comp    ]
   mov            edx, [dword ptr comp + 4]
   mov            ebx, [dword ptr excg    ]
   mov            ecx, [dword ptr excg + 4]
   lock cmpxchg8b [esi]
   mov            [dword ptr old    ], eax
   mov            [dword ptr old + 4], edx
   pop            ebx
*/

   #error Interlocked::compare_exchange_int64() undefined for this compiler (to do)

#else

   #error Interlocked::compare_exchange_int64() undefined for this compiler

#endif   // ohm_archi_COMPILER_CLASS
}



#endif   // ohm_archi_HAS_CAS_64



#if defined (ohm_archi_HAS_CAS_128)



void  Interlocked::exchange_int128 (Data128 &old, volatile Data128 &dest, const Data128 &excg)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

/*
   mov            r9, [qword ptr excg]
   mov            r10, [qword ptr old]
   push           rbx
   mov            rsi, [dest]
   mov            rbx, [r9    ]
   mov            rcx, [r9 + 8]

cas_loop:
   mov            rax, [rsi    ]
   mov            rdx, [rsi + 8]
   lock cmpxchg16b   [rsi]
   jnz            cas_loop

   mov            [r10    ], rax
   mov            [r10 + 8], rdx
   pop            rbx
*/

   #error Interlocked::exchange_int128() undefined for this compiler (to do)

#else // Default implementation (slow)

   Data128        tmp;
   do
   {
      old = *(const Data128 *) (&dest);
      compare_exchange_int128 (tmp, dest, excg, old);
   }
   while (tmp != old);

#endif   // ohm_archi_COMPILER_CLASS
}



void  Interlocked::compare_exchange_int128 (Data128 &old, volatile Data128 &dest, const Data128 &excg, const Data128 &comp)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   #if (ohm_archi_COMPILER >= ohm_archi_COMPILER_VC2008)

      const archi::Int64   excg_lo = ((const archi::Int64 *) &excg) [0];
      const archi::Int64   excg_hi = ((const archi::Int64 *) &excg) [1];

      old = comp;

      _InterlockedCompareExchange128 (
         reinterpret_cast <volatile archi::Int64 *> (&dest),
         excg_hi,
         excg_lo,
         reinterpret_cast <archi::Int64 *> (&old)
      );

   #else

      /*** To do ***/
      ohm_lang_CRASH ();
      assert (false);
      // Requires external assembly code

   #endif

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

/*
   mov            r8, [qword ptr comp]
   mov            r9, [qword ptr excg]
   mov            r10, [qword ptr old]
   push           rbx
   mov            rsi, [dest]
   mov            rax, [r8    ]
   mov            rdx, [r8 + 8]
   mov            rbx, [r9    ]
   mov            rcx, [r9 + 8]
   lock cmpxchg16b   [rsi]
   mov            [r10    ], rax
   mov            [r10 + 8], rdx
   pop            rbx
*/

   #error Interlocked::compare_exchange_int128() undefined for this compiler (to do)

#else

   #error Interlocked::compare_exchange_int128() undefined for this compiler

#endif   // ohm_archi_COMPILER_CLASS
}



#endif   // ohm_archi_HAS_CAS_128



void *   Interlocked::exchange_ptr (void * volatile &dest_ptr, void *excg_ptr)
{
   assert (archi::is_ptr_aligned_nonzero (&dest_ptr));

#if (ohm_archi_WORD_SIZE == 64)

   return (reinterpret_cast <void *> (
      exchange_int64 (
         *reinterpret_cast <archi::Int64 volatile *> (&dest_ptr),
         reinterpret_cast <archi::Int64> (excg_ptr)
      )
   ));

#else

   return (reinterpret_cast <void *> (
      exchange_int32 (
         *reinterpret_cast <archi::Int32 volatile *> (&dest_ptr),
         reinterpret_cast <archi::Int32> (excg_ptr)
      )
   ));

#endif
}



void *   Interlocked::compare_exchange_ptr (void * volatile &dest_ptr, void *excg_ptr, void *comp_ptr)
{
   assert (archi::is_ptr_aligned_nonzero (&dest_ptr));

#if (ohm_archi_WORD_SIZE == 64)

   return (reinterpret_cast <void *> (
      compare_exchange_int64 (
         *reinterpret_cast <archi::Int64 volatile *> (&dest_ptr),
         reinterpret_cast <archi::Int64> (excg_ptr),
         reinterpret_cast <archi::Int64> (comp_ptr)
      )
   ));

#else

   return (reinterpret_cast <void *> (
      compare_exchange_int32 (
         *reinterpret_cast <archi::Int32 volatile *> (&dest_ptr),
         reinterpret_cast <archi::Int32> (excg_ptr),
         reinterpret_cast <archi::Int32> (comp_ptr)
      )
   ));

#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
