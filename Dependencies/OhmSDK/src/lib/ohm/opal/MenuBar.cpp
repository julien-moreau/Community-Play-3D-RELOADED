/*****************************************************************************

        MenuBar.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70519

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

#include "ohm/opal/MenuBar.h"
#include "ohm/opal/MenuItem.h"

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/MenuBarImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/opal/carbon/MenuBarImpl.h"

#else
   #error Operation System not supported

#endif

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

MenuBar::~MenuBar ()
{
}



/*
==============================================================================
Name : use_root
==============================================================================
*/

MenuRoot &  MenuBar::use_root ()
{
   return _menu_root;
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  MenuBar::update ()
{
   if (_menu_root.was_modified ())
   {
      _menu_root.begin_update ();
      
      MenuBarImpl::use_instance ().update (_menu_root);
      
      _menu_root.end_update ();
      
      _menu_root.synchronize ();
   }
}



/*
==============================================================================
Name : post
Note :
   A synthetized opal event was posted, check if menu bar should be act.
   Returns true iff the menu event was catched.
==============================================================================
*/

bool  MenuBar::post (const Key & key)
{
   bool catched_flag = false;
   
   MenuItem * menu_item_ptr = _menu_root.find_item_recursive (key);
   
   if (menu_item_ptr != 0)
   {
      MenuItem & menu_item = *menu_item_ptr;
      
      catched_flag = true;
      
      _menu_event._menu_item_ptr = &menu_item;
      _menu_event._handled_flag = false;
      
      //menu_item.signal_pre ();
      
      if (menu_item.get_std_id () != MenuStdId_NONE)
      {
         util::Delegate & delegate = menu_item.use_std_id_delegate ();
         
         if (delegate.is_set ())
         {
            delegate.param_ref_signal (_menu_event);
         }
      }
      
      if (!_menu_event._handled_flag)
      {
         util::Delegate & delegate = menu_item.use_delegate ();
         
         if (delegate.is_set ())
         {
            delegate.param_ref_signal (_menu_event);
         }
      }
      
      //menu_item.signal_post ();
      
      _menu_event.reset ();
   }
   
   return catched_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

MenuBar::MenuBar ()
:  _menu_root ()
,  _menu_event ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
