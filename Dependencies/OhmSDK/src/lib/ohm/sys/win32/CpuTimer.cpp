/*****************************************************************************

        CpuTimer.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 31964

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/sys/CpuTimer.h"

#if (   ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC   \
     && ohm_archi_WORD_SIZE == 64)
#include <intrin.h>
#endif


namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



CpuTimer::CpuTimer ()
:  _impl_ptr (0)
{
   // Nothing
}



CpuTimer::~CpuTimer ()
{
// Implementation class not used and not defined
// delete _impl_ptr;
// _impl_ptr = 0;
}



archi::Int64   CpuTimer::read_clock ()
{
#if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)

   #if (ohm_archi_WORD_SIZE == 64)

   return (__rdtsc ());

   #else // ohm_archi_WORD_SIZE

   archi::UInt32  lower_bits;
   archi::Int32   higher_bits;

   __asm
   {
      rdtsc
      mov         lower_bits, eax
      mov         higher_bits, edx
   }

   return ((archi::Int64 (higher_bits) << 32) + lower_bits);

   #endif   // ohm_archi_WORD_SIZE

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

   archi::Int64   clock_cnt;

   __asm__ __volatile__ ("rdtsc" : "=A" (clock_cnt));

   return (clock_cnt);

#else    // ohm_archi_COMPILER_CLASS

   #error Not written for this compiler yet !

#endif   // ohm_archi_COMPILER_CLASS
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
