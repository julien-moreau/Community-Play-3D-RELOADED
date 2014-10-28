/*****************************************************************************

        Interlocked_common.hpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 67486

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



#if defined (ohm_sys_Interlocked_common_CURRENT_CODEHEADER)
   #error Recursive inclusion of Interlocked_common code header.
#endif
#define  ohm_sys_Interlocked_common_CURRENT_CODEHEADER

#if ! defined (ohm_sys_Interlocked_common_CODEHEADER_INCLUDED)
#define  ohm_sys_Interlocked_common_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/Vect128MemOpAlign.h"



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#if defined (ohm_archi_HAS_CAS_128)
   
#if (ohm_archi_SYS == ohm_archi_SYS_LINUX)
// nothing

#else
   
bool Interlocked::Data128::operator == (const Data128 & other) const
{
   const archi::Int64 * lhs_ptr = 
      reinterpret_cast <const archi::Int64 *> (&      _data [0]);
   const archi::Int64 * rhs_ptr = 
      reinterpret_cast <const archi::Int64 *> (&other._data [0]);

   return (lhs_ptr [0] == rhs_ptr [0] && lhs_ptr [1] == rhs_ptr [1]);
}



bool Interlocked::Data128::operator != (const Data128 & other) const
{
   return (! (*this == other));
}

#endif



#endif   // ohm_archi_HAS_CAS_128
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



#endif   // ohm_sys_Interlocked_common_CODEHEADER_INCLUDED

#undef ohm_sys_Interlocked_common_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

