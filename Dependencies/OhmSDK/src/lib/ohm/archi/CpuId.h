/*****************************************************************************

        CpuId.h
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



#if ! defined (ohm_archi_CpuId_HEADER_INCLUDED)
#define  ohm_archi_CpuId_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace archi
{



class CpuIdImpl;

class CpuId
{

   friend class CpuIdImpl;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ~CpuId ();

   // Generic
   inline bool    has_simd_float32_2 () const;
   inline bool    has_simd_float32_4 () const;
   inline bool    has_simd_float64_2 () const;
   inline bool    has_cas_128 () const;

   // PPC specific
   inline bool    has_altivec () const;

   // x86 specific
   inline bool    has_mmx () const;
   inline bool    has_3dnow () const;
   inline bool    has_sse () const;
   inline bool    has_sse2 () const;
   inline bool    has_sse3 () const;

   static CpuId & use_instance ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  CpuId ();

   void           reset_features ();
   void           build_generic_flags ();

   CpuIdImpl *    _impl_ptr;

   bool           _simd_float32_2_flag;
   bool           _simd_float32_4_flag;
   bool           _simd_float64_2_flag;
   bool           _cas_128_flag;

   bool           _altivec_flag;
   bool           _mmx_flag;
   bool           _3dnow_flag;
   bool           _sse_flag;
   bool           _sse2_flag;
   bool           _sse3_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  CpuId (const CpuId &other);
   CpuId &        operator = (const CpuId &other);
   bool           operator == (const CpuId &other);
   bool           operator != (const CpuId &other);

}; // class CpuId



}  // namespace archi
}  // namespace ohm



#include "ohm/archi/CpuId.hpp"



#endif   // ohm_archi_CpuId_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
