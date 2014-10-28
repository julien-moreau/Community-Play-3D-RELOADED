/*****************************************************************************

        x86/CpuIdx86.h
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



#if ! defined (ohm_archi_x86_CpuIdx86_HEADER_INCLUDED)
#define  ohm_archi_x86_CpuIdx86_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace archi
{
namespace x86
{



class CpuIdx86
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CpuIdx86 ();
                  ~CpuIdx86 () {}

   void           collect_interesting_info ();

   static bool    has_cpuid ();
   static void    call_cpuid (int fnc_nbr, int &v_eax, int &v_ebx, int &v_ecx, int &v_edx);

   bool           _cpuid_flag;
   bool           _mmx_flag;
   bool           _3dnow_flag;
   bool           _sse_flag;
   bool           _sse2_flag;
   bool           _sse3_flag;
   bool           _tsc_invariant;
   bool           _cas_128_flag;
   int            _family;
   int            _model;
   int            _stepping;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  CpuIdx86 (const CpuIdx86 &other);
   CpuIdx86 &     operator = (const CpuIdx86 &other);
   bool           operator == (const CpuIdx86 &other);
   bool           operator != (const CpuIdx86 &other);

}; // class CpuIdx86



}  // namespace x86
}  // namespace archi
}  // namespace ohm



//#include  "ohm/archi/x86/CpuIdx86.hpp"



#endif   // ohm_archi_x86_CpuIdx86_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
