/*****************************************************************************

        MenuBarImpl.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57659

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



#if ! defined (ohm_opal_MenuBarImpl_HEADER_INCLUDED)
#define  ohm_opal_MenuBarImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/MenuEvent.h"
#include "ohm/util/SingletonBase.h"



namespace ohm
{
namespace opal
{


class MenuRoot;
class MenuContainer;
class MenuItem;

class MenuBarImpl
:  public util::SingletonBase <MenuBarImpl>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~MenuBarImpl ();
   
   void           update (MenuRoot & menu_root);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  MenuBarImpl ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (MenuBarImpl);
   
   void           update (MenuRef menu_ref, MenuContainer & container);
   void           update_props (MenuRef menu_ref, MenuItemIndex index, MenuItem & menu_item);
   
   void           install_match_key (MenuRef menu_ref);
   
   static OSStatus
                  handle_event_proc (EventHandlerCallRef handler_call_ref, EventRef event_ref, void * user_data);
   OSStatus       handle_event (EventHandlerCallRef handler_call_ref, EventRef event_ref);
   OSStatus       handle_menu_event (EventRef event_ref);
   OSStatus       handle_command_event (EventRef event_ref);
   
   void           process_menu_event (MenuItem & menu_item);
   
   EventHandlerRef
                  _app_event_handler_ref;
   MenuID         _cur_menu_id;
   MenuEvent      _menu_event;
   
   MenuItem *     _prefs_menu_item_ptr;
   
   static const ::OSType
                  _command_id_opal;
   static const ::OSType
                  _creator_ohm_force;
   static const ::OSType
                  _tag_menu_item_ptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MenuBarImpl (const MenuBarImpl &other);
   MenuBarImpl &  operator = (const MenuBarImpl &other);
   bool           operator == (const MenuBarImpl &other);
   bool           operator != (const MenuBarImpl &other);

}; // class MenuBarImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_MenuBarImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
