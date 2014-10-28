/*****************************************************************************

        Interlocked.cpp
        Copyright (c) 2008 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 28114

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


/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/fnc.h"
#include "ohm/sys/Interlocked.h"

#include <libkern/OSAtomic.h>
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
   
   archi::Int32   old_val = dest;
   while (! ::OSAtomicCompareAndSwap32Barrier (
      old_val, 
      excg, 
      const_cast <int32_t *> (reinterpret_cast <int32_t volatile *> (&dest))
   ))
   {
      old_val = dest;
   }

   return (old_val);
}



archi::Int32   Interlocked::compare_exchange_int32 (archi::Int32 volatile &dest, archi::Int32 excg, archi::Int32 comp)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

   return (::OSAtomicCompareAndSwap32Barrier (
      comp,
      excg,
      const_cast <int32_t *> (reinterpret_cast <int32_t volatile *> (&dest))
   ) ? comp : excg);
}



#if defined (ohm_archi_HAS_CAS_64)



archi::Int64   Interlocked::exchange_int64 (archi::Int64 volatile &dest, archi::Int64 excg)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

   archi::Int64   old_val = dest;
   while (!::OSAtomicCompareAndSwap64Barrier (
      old_val, 
      excg, 
      const_cast <int64_t *> (reinterpret_cast <int64_t volatile *> (&dest))
   ))
   {
      old_val = dest;
   }
   
   return (old_val);
}



archi::Int64   Interlocked::compare_exchange_int64 (archi::Int64 volatile &dest, archi::Int64 excg, archi::Int64 comp)
{
   assert (archi::is_ptr_aligned_nonzero (&dest));

   return (::OSAtomicCompareAndSwap64Barrier (
      comp, 
      excg, 
      const_cast <int64_t *> (reinterpret_cast <int64_t volatile *> (&dest))
   ) ? comp : excg);
}



#endif



#if defined (ohm_archi_HAS_CAS_128)



void Interlocked::exchange_int128 (Data128 &old, volatile Data128 &dest, const Data128 &excg)
{
   assert (archi::is_ptr_aligned_nonzero (&old));
   assert (archi::is_ptr_aligned_nonzero (&dest));
   assert (archi::is_ptr_aligned_nonzero (&excg));

   asm volatile ( 
   "  mov             %[excg], %%r9       \n"

   "  push           %%rbx                \n"
   "  mov            %[dest], %%rsi       \n"
   "  movq           (%%r9), %%rbx        \n"
   "  mov            8(%%r9), %%rcx       \n"

   "cas_loop%=:   \n"
   "  mov            (%%rsi), %%rax       \n"
   "  mov            8(%%rsi), %%rdx      \n"
   "  lock cmpxchg16b   (%%rsi)           \n"
   "  jnz            cas_loop%=           \n"

   "  mov             %[old], %%r10       \n"
   "  movq           %%rax, (%%r10)       \n"
   "  movq           %%rdx, 8(%%r10)      \n"
   "  pop            %%rbx                \n"
   : 
   : [excg] "m" (&excg)
   , [old]  "m" (&old)
   , [dest] "m" (&dest)
   : "rsi", "rax", "rcx", "rdx", "r9", "r10"
   );
}



void Interlocked::compare_exchange_int128 (Data128 &old, volatile Data128 &dest, const Data128 &excg, const Data128 &comp)
{
   assert (archi::is_ptr_aligned_nonzero (&old));
   assert (archi::is_ptr_aligned_nonzero (&dest));
   assert (archi::is_ptr_aligned_nonzero (&excg));
   assert (archi::is_ptr_aligned_nonzero (&comp));

   asm volatile (
   "  mov             %[comp], %%r8       \n"
   "  mov             %[excg], %%r9       \n"
   "  mov             %[old],  %%r10      \n"
   "  mov             %[dest], %%rsi      \n"
   
   "  push           %%rbx                \n"
   "  movq            (%%r8), %%rax       \n"
   "  movq           8(%%r8), %%rdx       \n"
   "  movq            (%%r9), %%rbx       \n"
   "  movq           8(%%r9), %%rcx       \n"
   "  lock cmpxchg16b   (%%rsi)           \n"
   "  movq           %%rax, (%%r10)       \n"
   "  movq           %%rdx, 8(%%r10)      \n"
   "  pop            %%rbx                \n"
   :
   : [old]  "m" (&old)
   , [dest] "m" (&dest)
   , [excg] "m" (&excg)
   , [comp] "m" (&comp)
   : "rsi", "rax", "rcx", "rdx", "r8", "r9", "r10"
   );
}



#endif



void *   Interlocked::exchange_ptr (void * volatile &dest_ptr, void *excg_ptr)
{
   assert (archi::is_ptr_aligned_nonzero (&dest_ptr));

   // There is a OSAtomicCompareAndSwapPtrBarrier method, but only for 10.5 (Leopard)
   
#if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5)

   void *         old_ptr = dest_ptr;
   
   while (! ::OSAtomicCompareAndSwapPtrBarrier (old_ptr, excg_ptr, &dest_ptr))
   {
      old_ptr = dest_ptr;
   }
      
   return (old_ptr);
   
#else

   #ifndef __LP64__
   
      int32_t old_val = reinterpret_cast <int32_t> (dest_ptr);
      
      while (! ::OSAtomicCompareAndSwap32Barrier (
         old_val, 
         reinterpret_cast <int32_t> (excg_ptr), 
         const_cast <int32_t *> (reinterpret_cast <volatile int32_t *> (&dest_ptr))
      ))
      {
         old_val = reinterpret_cast <int32_t> (dest_ptr);
      }
      
      return (reinterpret_cast <void *> (old_val));
         
   #else
      #error ohm::sys::Interlocked::exchange_ptr(): we do not support for 64 bits before MacOS X 10.5
   #endif
   
#endif
}



void *   Interlocked::compare_exchange_ptr (void * volatile &dest_ptr, void *excg_ptr, void *comp_ptr)
{
   assert (archi::is_ptr_aligned_nonzero (&dest_ptr));

   // There is a OSAtomicCompareAndSwapPtrBarrier method, but only for 10.5 (Leopard)
   
#if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5)

   return (::OSAtomicCompareAndSwapPtrBarrier (
      comp_ptr,
      excg_ptr,
      &dest_ptr
   ) ? comp_ptr : excg_ptr);

#else

   #ifndef __LP64__

      return (::OSAtomicCompareAndSwap32Barrier (
         reinterpret_cast <int32_t> (comp_ptr), 
         reinterpret_cast <int32_t> (excg_ptr), 
         const_cast <int32_t *> (reinterpret_cast <int32_t volatile *> (&dest_ptr))
      ) ? comp_ptr : excg_ptr);
         
   #else
      #error ohm::sys::Interlocked::compare_exchange_ptr: we do not support for 64 bits before MacOS X 10.5
   #endif
   
#endif
   
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
