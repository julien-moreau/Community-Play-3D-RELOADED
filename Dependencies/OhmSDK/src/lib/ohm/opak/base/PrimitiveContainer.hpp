/*****************************************************************************

        PrimitiveContainer.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 41076

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



#if defined (ohm_opak_PrimitiveContainer_CURRENT_CODEHEADER)
   #error Recursive inclusion of PrimitiveContainer code header.
#endif
#define  ohm_opak_PrimitiveContainer_CURRENT_CODEHEADER

#if ! defined (ohm_opak_PrimitiveContainer_CODEHEADER_INCLUDED)
#define  ohm_opak_PrimitiveContainer_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/private/Animator.h"
#include "ohm/opal/AutoContextState.h"
#include "ohm/lang/fnc.h"



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : notify_children
==============================================================================
*/

template <class PrimitiveType>
void  PrimitiveContainer::notify_children (void (PrimitiveType::*fnc) ())
{
   PrimitivePtrOrderList::iterator it = _primitive_ptr_list.begin ();
   const PrimitivePtrOrderList::iterator it_end = _primitive_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      Primitive * primitive_ptr = it->get_primitive_ptr ();
      
      PrimitiveType * primitive_type_ptr
         = dynamic_cast <PrimitiveType *> (primitive_ptr);
      
      if (primitive_type_ptr != 0)
      {
         (primitive_type_ptr->*fnc) ();
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_PrimitiveContainer_CODEHEADER_INCLUDED

#undef ohm_opak_PrimitiveContainer_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
