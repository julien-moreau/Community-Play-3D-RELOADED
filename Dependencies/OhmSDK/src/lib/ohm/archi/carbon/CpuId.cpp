/*****************************************************************************

        carbon/CpuId.cpp
        Copyright (c) 2002 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/CpuId.h"

#if (ohm_archi_PROC == ohm_archi_PROC_X86)
   #include "ohm/archi/x86/CpuIdx86.h"
#endif

#include <CoreServices/CoreServices.h>

#include <cassert>



namespace ohm
{
namespace archi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



CpuId::~CpuId ()
{
}


CpuId &  CpuId::use_instance ()
{
   static CpuId   instance;

   return (instance);
}




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



CpuId::CpuId ()
:  _impl_ptr (0)  // No use here
{
   reset_features ();

#if (ohm_archi_PROC == ohm_archi_PROC_X86)

   x86::CpuIdx86  cpuid_x86;
   cpuid_x86.collect_interesting_info ();

   if (cpuid_x86._cpuid_flag)
   {
      _mmx_flag     = cpuid_x86._mmx_flag;
      _3dnow_flag   = cpuid_x86._3dnow_flag;
      _sse_flag     = cpuid_x86._sse_flag;
      _sse2_flag    = cpuid_x86._sse2_flag;
      _sse3_flag    = cpuid_x86._sse3_flag;
      _cas_128_flag = cpuid_x86._cas_128_flag;
   }
   
#else

   long cpu_attributes;
   OSErr err = Gestalt (gestaltPowerPCProcessorFeatures, &cpu_attributes); 

   if (err == noErr)
   {
      _altivec_flag = ( 1 << gestaltPowerPCHasVectorInstructions) & cpu_attributes;
   }

#endif

   build_generic_flags ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace archi
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
