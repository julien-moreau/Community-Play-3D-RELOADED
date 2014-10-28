/*****************************************************************************

        Vect128MemOpAlign.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 25938

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



#if defined (ohm_archi_Vect128MemOpAlign_CURRENT_CODEHEADER)
   #error Recursive inclusion of Vect128MemOpAlign code header.
#endif
#define  ohm_archi_Vect128MemOpAlign_CURRENT_CODEHEADER

#if ! defined (ohm_archi_Vect128MemOpAlign_CODEHEADER_INCLUDED)
#define  ohm_archi_Vect128MemOpAlign_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/fnc.h"
#if (ohm_archi_PROC == ohm_archi_PROC_X86)
   #include <xmmintrin.h>
#endif

#include <cassert>



namespace ohm
{
namespace archi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool  Vect128MemOpAlign::check_address (const void *ptr)
{
   return (is_ptr_aligned_nonzero (ptr, 16));
}



Vect128  Vect128MemOpAlign::load (const void *ptr)
{
   assert (check_address (ptr));

#if (ohm_archi_PROC == ohm_archi_PROC_X86)

   return (_mm_load_ps (reinterpret_cast <const float *> (ptr)));

#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)

   return (vec_ld (0, reinterpret_cast <const float *> (ptr)));

#else
   #error Operation not defined for this platform !
#endif
}



void  Vect128MemOpAlign::store (void *ptr, const Vect128 &val)
{
   assert (check_address (ptr));

#if (ohm_archi_PROC == ohm_archi_PROC_X86)

   _mm_store_ps (reinterpret_cast <float *> (ptr), val);

#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)

   vec_st (val, 0, reinterpret_cast <float *> (ptr));

#else
   #error Operation not defined for this platform !
#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace archi
}  // namespace ohm



#endif   // ohm_archi_Vect128MemOpAlign_CODEHEADER_INCLUDED

#undef ohm_archi_Vect128MemOpAlign_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
