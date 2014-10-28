/*****************************************************************************

        Vect128MemOpUnalign.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 42242

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



#if defined (ohm_archi_Vect128MemOpUnalign_CURRENT_CODEHEADER)
   #error Recursive inclusion of Vect128MemOpUnalign code header.
#endif
#define  ohm_archi_Vect128MemOpUnalign_CURRENT_CODEHEADER

#if ! defined (ohm_archi_Vect128MemOpUnalign_CODEHEADER_INCLUDED)
#define  ohm_archi_Vect128MemOpUnalign_CODEHEADER_INCLUDED



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



bool  Vect128MemOpUnalign::check_address (const void *ptr)
{
   return (ptr != 0);
}



Vect128  Vect128MemOpUnalign::load (const void *ptr)
{
   assert (check_address (ptr));

#if (ohm_archi_PROC == ohm_archi_PROC_X86)

   return (_mm_loadu_ps (reinterpret_cast <const float *> (ptr)));

#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)

   const vector unsigned char msq = vec_ld ( 0, reinterpret_cast <const unsigned char *> (ptr));      // most significant quadword
   const vector unsigned char lsq = vec_ld (15, reinterpret_cast <const unsigned char *> (ptr));      // least significant quadword
   const vector unsigned char mask = vec_lvsl (0, reinterpret_cast <const unsigned char *> (ptr)); // create the permute mask
   return (vec_perm (msq, lsq, mask));                      // align the data

#else
   #error Operation not defined for this platform !
#endif
}



void  Vect128MemOpUnalign::store (void *ptr, const Vect128 &val)
{
   assert (check_address (ptr));

#if (ohm_archi_PROC == ohm_archi_PROC_X86)

   _mm_storeu_ps (reinterpret_cast <float *> (ptr), val);

#elif (ohm_archi_PROC == ohm_archi_PROC_PPC)

   vector unsigned char msq = vec_ld ( 0, reinterpret_cast <const unsigned char *> (ptr));   // most significant quadword
   vector unsigned char lsq = vec_ld (15, reinterpret_cast <const unsigned char *> (ptr));   // least significant quadword
   vector unsigned char edge_align = vec_lvsl (0, reinterpret_cast <const unsigned char *> (ptr)); // permute map to extract edges
   vector unsigned char edges = vec_perm (lsq, msq, edge_align);  // extract the edges
   vector unsigned char align = vec_lvsr (0, reinterpret_cast <const unsigned char *> (ptr));   // permute map to misalign data
   msq = vec_perm (edges, src, align); // misalign the data (MSQ)
   lsq = vec_perm (src, edges, align); // misalign the data (LSQ)
   vec_st (lsq, 15, reinterpret_cast <unsigned char *> (ptr)); // Store the LSQ part first
   vec_st (msq,  0, reinterpret_cast <unsigned char *> (ptr)); // Store the MSQ part

#else
   #error Operation not defined for this platform !
#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace archi
}  // namespace ohm



#endif   // ohm_archi_Vect128MemOpUnalign_CODEHEADER_INCLUDED

#undef ohm_archi_Vect128MemOpUnalign_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
