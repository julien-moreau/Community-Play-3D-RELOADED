/*****************************************************************************

        win32/CpuId.cpp
        Copyright (c) 2002 Ohm Force

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

#include "ohm/archi/CpuId.h"
#include "ohm/archi/x86/CpuIdx86.h"

#include <cassert>



namespace ohm
{
namespace archi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



CpuId::~CpuId ()
{
// delete _impl_ptr;
// _impl_ptr = 0;
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

   // Generic flags
   build_generic_flags ();
}



}  // namespace archi
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
