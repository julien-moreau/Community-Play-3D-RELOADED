/*****************************************************************************

        CpuId.hpp
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



#if defined (ohm_archi_CpuId_CURRENT_CODEHEADER)
   #error Recursive inclusion of CpuId code header.
#endif
#define  ohm_archi_CpuId_CURRENT_CODEHEADER

#if ! defined (ohm_archi_CpuId_CODEHEADER_INCLUDED)
#define  ohm_archi_CpuId_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace archi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool  CpuId::has_simd_float32_2 () const
{
   return (_simd_float32_2_flag);
}



bool  CpuId::has_simd_float32_4 () const
{
   return (_simd_float32_4_flag);
}



bool  CpuId::has_simd_float64_2 () const
{
   return (_simd_float64_2_flag);
}



bool  CpuId::has_cas_128 () const
{
   return (_cas_128_flag);
}



bool  CpuId::has_altivec () const
{
   return (_altivec_flag);
}



bool  CpuId::has_mmx () const
{
   return (_mmx_flag);
}



bool  CpuId::has_3dnow () const
{
   return (_3dnow_flag);
}



bool  CpuId::has_sse () const
{
   return (_sse_flag);
}



bool  CpuId::has_sse2 () const
{
   return (_sse2_flag);
}



bool  CpuId::has_sse3 () const
{
   return (_sse3_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace archi
}  // namespace ohm



#endif   // ohm_archi_CpuId_CODEHEADER_INCLUDED

#undef ohm_archi_CpuId_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
