/*****************************************************************************

        MenuStdIdLink.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53901

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



#if defined (ohm_opal_MenuStdIdLink_CURRENT_CODEHEADER)
   #error Recursive inclusion of MenuStdIdLink code header.
#endif
#define  ohm_opal_MenuStdIdLink_CURRENT_CODEHEADER

#if ! defined (ohm_opal_MenuStdIdLink_CODEHEADER_INCLUDED)
#define  ohm_opal_MenuStdIdLink_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : bind_std_id
==============================================================================
*/

template <class T, void (T::*proc) (MenuEvent &)>
void  MenuStdIdLink::bind (T & receiver, MenuStdId std_id)
{
   unbind ();
   
   MenuRoot & menu_root = MenuBar::use_instance ().use_root ();
   
   MenuItem * item_ptr = menu_root.find_item_recursive (std_id);
   
   if (item_ptr != 0)
   {
      util::Delegate delegate;
      
      delegate.param_ref_set <T, MenuEvent, proc> (receiver);
      
      item_ptr->bind_std_id (delegate);
      
      _std_id = std_id;
   }
   else
   {
      ohm_util_TRACE_ERROR1 ("cannot find menu with std id %1%", std_id);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_MenuStdIdLink_CODEHEADER_INCLUDED

#undef ohm_opal_MenuStdIdLink_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
