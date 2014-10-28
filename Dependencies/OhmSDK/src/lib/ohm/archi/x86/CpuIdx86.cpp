/*****************************************************************************

        x86/CpuIdx86.cpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 45147

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

#include "ohm/archi/x86/CpuIdx86.h"
#include "ohm/archi/def.h"

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
#include <intrin.h>
#endif

#include <cassert>



namespace ohm
{
namespace archi
{
namespace x86
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



CpuIdx86::CpuIdx86 ()
:  _cpuid_flag (false)
,  _mmx_flag (false)
,  _3dnow_flag (false)
,  _sse_flag (false)
,  _sse2_flag (false)
,  _sse3_flag (false)
,  _cas_128_flag (false)
,  _tsc_invariant (false)
,  _family (0)
,  _model (0)
,  _stepping (0)
{
   // Nothing
}



// All flags are expected to be cleared before the function is called.
void  CpuIdx86::collect_interesting_info ()
{
   int               eax;
   int               ebx;
   int               ecx;
   int               edx;

   _cpuid_flag = has_cpuid ();

   if (_cpuid_flag)
   {
      // Basic features
      call_cpuid (0x00000001, eax, ebx, ecx, edx);

      _stepping = (eax >> 0) & 0x0F;

      _family = (eax >> 8) & 0x0F;
      if (_family == 0x0F)
      {
         _family += (eax >> 20) & 0xFF;
      }

      _model = (eax >> 4) & 0x0F;
      if (_family >= 0x0F)
      {
         _model += (eax >> 12) & 0xF0;
      }

      _mmx_flag     = ((edx & (1L << 23)) != 0);
      _sse_flag     = ((edx & (1L << 25)) != 0);
      _sse2_flag    = ((edx & (1L << 26)) != 0);
      _sse3_flag    = ((ecx & (1L <<  0)) != 0);
      _cas_128_flag = ((ecx & (1L << 13)) != 0);
      assert (_mmx_flag  || ! _sse_flag );   // SSE  but no MMX  ? Interesting...
      assert (_sse_flag  || ! _sse2_flag);   // SEE2 but no SSE  ? Interesting...
      assert (_sse2_flag || ! _sse3_flag);   // SEE3 but no SSE2 ? Interesting...

      // Extended features
      call_cpuid (0x80000000, eax, ebx, ecx, edx);
      const int   xtd_fnc_range_max = eax;
      if (xtd_fnc_range_max >= 0x80000001)
      {
         call_cpuid (0x80000001, eax, ebx, ecx, edx);
         _3dnow_flag = ((edx & (1L << 31)) != 0);
         assert (_mmx_flag || ! _3dnow_flag);   // 3DNow! but no MMX ? Interesting...
      }

      if (_family < 0x0F)
      {
         _tsc_invariant = true;
      }
      else if (xtd_fnc_range_max >= 0x80000007)
      {
         call_cpuid (0x80000007, eax, ebx, ecx, edx);
         _tsc_invariant = ((edx & (1L << 8)) != 0);
      }
   }
}



bool  CpuIdx86::has_cpuid ()
{
   // Supports cpuid instruction ?
   int               cpuid_flag = false;

#if (ohm_archi_WORD_SIZE == 64)

   cpuid_flag = true;

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   __asm
   {
      pushfd                           ; save EFLAGS
      pop               eax            ; store EFLAGS in EAX
      mov               ebx, eax       ; save in EBX for later testing
      xor               eax, 00200000h ; toggle bit 21
      push              eax            ; put to stack
      popfd                            ; save changed EAX to EFLAGS
      pushfd                           ; push EFLAGS to TOS
      pop               eax            ; store EFLAGS in EAX
      cmp               eax, ebx       ; see if bit 21 has changed
      jz                end_cpuid      ; if no change, no CPUID
      mov               cpuid_flag, 1
   end_cpuid:
   }

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

   __asm__ ("                                \n \
      pushf                                  \n \
      popl              %%eax                \n \
      movl              %%eax,         %%ecx \n \
      xorl              $0x00200000,   %%eax \n \
      pushl             %%eax                \n \
      popf                                   \n \
      pushf                                  \n \
      popl              %%eax                \n \
      cmpl              %%eax,         %%ecx \n \
      jz                end_cpuid_%=         \n \
      movl              $1,            %0    \n \
   end_cpuid_%=:                             \n \
   "
   : "=r"(cpuid_flag)
   :
   : "%eax", "%ecx"
   );

#else

   #pragma error "Unsupported compiler"

#endif

   return (static_cast <bool> (cpuid_flag));
}



void  CpuIdx86::call_cpuid (int fnc_nbr, int &v_eax, int &v_ebx, int &v_ecx, int &v_edx)
{
   assert (&v_eax != 0);
   assert (&v_ebx != 0);
   assert (&v_ecx != 0);
   assert (&v_edx != 0);

#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   int         cpu_info [4];
   __cpuid (cpu_info, fnc_nbr);
   v_eax = cpu_info [0];
   v_ebx = cpu_info [1];
   v_ecx = cpu_info [2];
   v_edx = cpu_info [3];

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
   
   long           r_eax;
   long           r_ebx;
   long           r_ecx;
   long           r_edx;

   #if (ohm_archi_WORD_SIZE == 64)

   __asm__ (
      "push %%rbx      \n\t" /* save %rbx */
      "cpuid           \n\t"
      "mov %%rbx, %1   \n\t" /* save what cpuid just put in %rbx */
      "pop %%rbx       \n\t" /* restore the old %rbx */
     : "=a"(r_eax), "=r"(r_ebx), "=c"(r_ecx), "=d"(r_edx)
     : "a"(fnc_nbr)
     : "cc");

   #else

   __asm__ (
      "pushl %%ebx      \n\t" /* save %ebx */
      "cpuid            \n\t"
      "movl %%ebx, %1   \n\t" /* save what cpuid just put in %ebx */
      "popl %%ebx       \n\t" /* restore the old %ebx */
     : "=a"(r_eax), "=r"(r_ebx), "=c"(r_ecx), "=d"(r_edx)
     : "a"(fnc_nbr)
     : "cc");

   #endif

   v_eax = r_eax;
   v_ebx = r_ebx;
   v_ecx = r_ecx;
   v_edx = r_edx;
   
#else

   #pragma error "Unsupported compiler"

#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace x86
}  // namespace archi
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
