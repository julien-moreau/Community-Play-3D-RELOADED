/*****************************************************************************

        MenuBarImpl.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70435

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

#include "ohm/opal/carbon/MenuBarImpl.h"
#include "ohm/opal/carbon/EventFnc.h"
#include "ohm/opal/MenuRoot.h"
#include "ohm/opal/MenuItem.h"
#include "ohm/opal/MenuSeparator.h"
#include "ohm/opal/View.h"
#include "ohm/task/RunLoopGui.h"

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

MenuBarImpl::~MenuBarImpl ()
{
   if (_app_event_handler_ref != 0)
   {
      RemoveEventHandler (_app_event_handler_ref);
      _app_event_handler_ref = 0;
   }
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  MenuBarImpl::update (MenuRoot & menu_root)
{
   // for now make the dumb version
   
   ClearMenuBar ();
   
   // add Preferences menu item
   EnableMenuCommand (0, kHICommandPreferences);
   
   _prefs_menu_item_ptr = 0;
   
   MenuRoot::iterator it = menu_root.begin ();
   MenuRoot::iterator it_end = menu_root.end ();
   
   for (; it != it_end ; ++it)
   {
      MenuBase & menu_base = *it;
      
      MenuItem * menu_ptr = dynamic_cast <MenuItem *> (&menu_base);
      assert (menu_ptr != 0);
      
      MenuItem & menu = *menu_ptr;
      
      MenuRef menu_ref = 0;
      OSStatus err = CreateNewMenu (
         _cur_menu_id,
         kMenuAttrAutoDisable,
         &menu_ref
      );
      assert (err == 0);
      assert (menu_ref != 0);
      ++_cur_menu_id;
      
      install_match_key (menu_ref);
      
      const std::string & title = menu.get_title ();
      
      CFStringRef title_str = CFStringCreateWithCString (
         0, title.c_str (), kCFStringEncodingUTF8
      );
      err = SetMenuTitleWithCFString (menu_ref, title_str);
      assert (err == 0);
      CFRelease (title_str);
      
      InsertMenu (menu_ref, 0);
      
      update (menu_ref, menu);
   }
   
   MenuRef menu_ref = 0;
   MenuItemIndex menu_item_idx = 0;
   
   OSStatus err = GetIndMenuItemWithCommandID (
      0, kHICommandPreferences, 1,
      &menu_ref, &menu_item_idx
   );
   assert (err == 0);
   
   if (err == 0)
   {
      if (_prefs_menu_item_ptr != 0)
      {
         EnableMenuItem (menu_ref, menu_item_idx);
         
         update_props (menu_ref, menu_item_idx, *_prefs_menu_item_ptr);
      }
      else
      {
         DisableMenuItem (menu_ref, menu_item_idx);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

MenuBarImpl::MenuBarImpl ()
:  _app_event_handler_ref (0)
,  _cur_menu_id (1)
,  _menu_event ()
,  _prefs_menu_item_ptr (0)
{
   {
      static const EventTypeSpec event_type_arr [] = {
         {kEventClassCommand, kEventCommandProcess},
      };

      OSStatus err = InstallEventHandler (
         GetApplicationEventTarget (),
         handle_event_proc,
         GetEventTypeCount (event_type_arr),
         event_type_arr,
         this,
         &_app_event_handler_ref
      );
      assert (err == 0);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  MenuBarImpl::update (MenuRef menu_ref, MenuContainer & container)
{
   MenuContainer::iterator it = container.begin ();
   MenuContainer::iterator it_end = container.end ();
   
   for (; it != it_end ; ++it)
   {
      MenuBase & menu_base = *it;
      
      MenuItem * menu_item_ptr = dynamic_cast <MenuItem *> (&menu_base);
      MenuSeparator * menu_separator = dynamic_cast <MenuSeparator *> (&menu_base);
      
      if (menu_item_ptr != 0)
      {
         bool add_flag = true;
         add_flag &= !menu_item_ptr->is_quit ();
         add_flag &= !menu_item_ptr->is_preferences ();
         
         if (add_flag)
         {
            const std::string & title = menu_item_ptr->get_title ();
            
            CFStringRef title_str = CFStringCreateWithCString (
               0, title.c_str (), kCFStringEncodingUTF8
            );
            
            MenuCommand command_id = _command_id_opal;
            
            MenuItemIndex index = 0;
            AppendMenuItemTextWithCFString (
               menu_ref,
               title_str,
               kMenuItemAttrIgnoreMeta,
               command_id,
               &index
            );
            CFRelease (title_str);
            
            update_props (menu_ref, index, *menu_item_ptr);
            
            if (!menu_item_ptr->empty ())
            {
               MenuRef sub_menu_ref = 0;
               OSStatus err = CreateNewMenu (
                  _cur_menu_id,
                  kMenuAttrAutoDisable,
                  &sub_menu_ref
               );
               assert (err == 0);
               assert (sub_menu_ref != 0);
               
               install_match_key (sub_menu_ref);
               
               const std::string & title = menu_item_ptr->get_title ();
            
               CFStringRef title_str = CFStringCreateWithCString (
                  0, title.c_str (), kCFStringEncodingUTF8
               );
               err = SetMenuTitleWithCFString (sub_menu_ref, title_str);
               assert (err == 0);
               CFRelease (title_str);
               
               InsertMenu (sub_menu_ref, kInsertHierarchicalMenu);
               SetMenuItemHierarchicalMenu (menu_ref, index, sub_menu_ref);
               ++_cur_menu_id;
               
               update (sub_menu_ref, *menu_item_ptr);
            }
         }
         else if (menu_item_ptr->is_preferences ())
         {
            _prefs_menu_item_ptr = menu_item_ptr;
         }
      }
      else if (menu_separator != 0)
      {
         bool insert_flag = true;
         
         MenuContainer::iterator next_it = it;
         ++next_it;
         
         if (next_it != it_end)
         {
            MenuBase & next_menu_base = *next_it;
      
            MenuItem * next_menu_item_ptr = dynamic_cast <MenuItem *> (&next_menu_base);
            
            if (next_menu_item_ptr != 0)
            {
               if (next_menu_item_ptr->is_quit ())
               {
                  insert_flag = false;
               }
               else if (next_menu_item_ptr->is_preferences ())
               {
                  insert_flag = false;
               }
            }
         }
         
         if (insert_flag)
         {
            MenuItemIndex index = 0;
            AppendMenuItemTextWithCFString (
               menu_ref,
               CFSTR (""),
               kMenuItemAttrIgnoreMeta | kMenuItemAttrSeparator,
               0,
               &index
            );
         }
      }
   }
}



/*
==============================================================================
Name : update_props
==============================================================================
*/

void  MenuBarImpl::update_props (MenuRef menu_ref, MenuItemIndex index, MenuItem & menu_item)
{
    CheckMenuItem (menu_ref, index, menu_item.is_marked ());
    
   if (menu_item.is_enabled ())
   {
      EnableMenuItem (menu_ref, index);
   }
   else
   {
      DisableMenuItem (menu_ref, index);
   }
   
   const Key & keystroke = menu_item.get_keystroke ();
   
   if (keystroke != Key::None)
   {
      Boolean virtual_key_flag = false;
      ::UInt16 key = 0;
      bool ok_flag = keystroke.to_menu_item_command_key (virtual_key_flag, key);
      
      if (ok_flag)
      {
         SetMenuItemCommandKey (menu_ref, index, virtual_key_flag, key);
         
         ::SInt16 glyph = 0;
         ok_flag = keystroke.to_menu_item_key_glyph (glyph);
         
         if (ok_flag)
         {
            SetMenuItemKeyGlyph (menu_ref, index, glyph);
         }
         
         ::UInt8 modifiers = kMenuNoModifiers;
         
         if (keystroke.has_shift_modifier ())
         {
            modifiers = modifiers | kMenuShiftModifier; 
         }
         
         if (keystroke.has_alt_modifier ())
         {
            modifiers = modifiers | kMenuOptionModifier; 
         }
         
         if (!keystroke.has_command_modifier ())
         {
            modifiers = modifiers | kMenuNoCommandModifier; 
         }
         
         SetMenuItemModifiers (menu_ref, index, modifiers);
      }
   }
   
   MenuItem * menu_item_ptr = &menu_item;
   
   SetMenuItemProperty (
      menu_ref, index,
      _creator_ohm_force, _tag_menu_item_ptr,
      sizeof (MenuItem *), &menu_item_ptr
   );
}



/*
==============================================================================
Name : install_match_key
==============================================================================
*/

void  MenuBarImpl::install_match_key (MenuRef menu_ref)
{
   static const EventTypeSpec event_type_arr [] = {
      {kEventClassMenu, kEventMenuMatchKey},
   };
   
   OSStatus err = InstallEventHandler (
      GetMenuEventTarget (menu_ref),
      handle_event_proc,
      GetEventTypeCount (event_type_arr),
      event_type_arr,
      this,
      0
   );
   assert (err == 0);
}



/*
==============================================================================
Name : handle_event_proc
==============================================================================
*/

OSStatus MenuBarImpl::handle_event_proc (EventHandlerCallRef handler_call_ref, EventRef event_ref, void * user_data)
{
   OSStatus ret_val = noErr;
   
   try
   {
      ret_val = MenuBarImpl::use_instance ().handle_event (handler_call_ref, event_ref);
   }
   catch (...)
   {
      assert (false);
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_event
==============================================================================
*/

OSStatus MenuBarImpl::handle_event (EventHandlerCallRef handler_call_ref, EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventClass (event_ref))
   {
   case kEventClassMenu:
      ret_val = handle_menu_event (event_ref);
      break;
      
   case kEventClassCommand:
      ret_val = handle_command_event (event_ref);
      break;
      
   default:
      assert (false);
      break;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : handle_keyboard_event
==============================================================================
*/

OSStatus MenuBarImpl::handle_menu_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventKind (event_ref))
   {
   case kEventMenuMatchKey:
      {
         // dispatch manually if no command modifier
         // else don't treat the event, it is going to be
         // well propagated
         ret_val = eventNotHandledErr;
         
         ::WindowRef window_ref = GetUserFocusWindow ();
         
         if (window_ref != 0)
         {
            ControlRef control_ref = 0;
            
            OSErr err = GetKeyboardFocus (
               window_ref,
               &control_ref
            );
            
            if (control_ref != 0)
            {
               ViewImpl * view_impl_ptr = ViewImpl::get (control_ref);
               
               if (view_impl_ptr != 0)
               {
                  // this is an opal/opak view
                  
                  View & view = view_impl_ptr->use_view ();
                  
                  if (view.get_keyboard_focus_frame_ptr () != 0)
                  {
                     // block raw kay down that do not contains command
                     
                     EventRef sub_event_ref = 0;
                     
                     err = GetEventParameter (
                        event_ref,
                        kEventParamEventRef,
                        typeEventRef,
                        0,
                        sizeof (EventRef),
                        0,
                        &sub_event_ref
                     );
                     assert (err == 0);
                     
                     if (!EventFnc::use_instance ().has_key_modifiers_command (sub_event_ref))
                     {
                        ret_val = menuItemNotFoundErr;
                     }
                  }
               }
            }
         }
      }
      break;
   
   default:
      assert (false);
      break;
   }
         
   return ret_val;
}



/*
==============================================================================
Name : handle_command_event
==============================================================================
*/

OSStatus MenuBarImpl::handle_command_event (EventRef event_ref)
{
   OSStatus ret_val = eventNotHandledErr;
   
   switch (GetEventKind (event_ref))
   {
   case kEventCommandProcess:
      {
         HICommandExtended hi_command;
      
         OSStatus err = GetEventParameter (
            event_ref,
            kEventParamDirectObject,
            typeHICommand,
            0,
            sizeof (HICommand),
            0,
            &hi_command
         );
         
         assert (err == ::noErr);
         
         ret_val = eventNotHandledErr;
         
         if (hi_command.commandID == _command_id_opal)
         {
            assert (hi_command.attributes == kHICommandFromMenu);
            
            ::UInt32 size = 0;
            OSStatus err = GetMenuItemPropertySize (
               hi_command.source.menu.menuRef,
               hi_command.source.menu.menuItemIndex, 
               _creator_ohm_force, _tag_menu_item_ptr, &size
            );
            
            if ((err == 0) && (size == sizeof (MenuItem *)))
            {
               bool ok_flag = !task::RunLoopGui::use_instance ().is_processing_event ();
               
               void * ptr = 0;

               err = GetMenuItemProperty (
                  hi_command.source.menu.menuRef,
                  hi_command.source.menu.menuItemIndex,
                  _creator_ohm_force, _tag_menu_item_ptr, size, &size, &ptr
               );
               assert (err == 0);
               assert (ptr != 0);
               
               ok_flag &= ptr != 0;
               
               if (ok_flag)
               {
                  MenuItem * menu_ptr = reinterpret_cast <MenuItem *> (ptr);
                  
                  process_menu_event (*menu_ptr);
               }
            }
         }
         else if (hi_command.commandID == kHICommandPreferences)
         {
            if (_prefs_menu_item_ptr != 0)
            {
               process_menu_event (*_prefs_menu_item_ptr);
            }
         }
      }
      break;
   
   default:
      assert (false);
      break;
   }
         
   return ret_val;
}



/*
==============================================================================
Name : process_menu_event
==============================================================================
*/

void  MenuBarImpl::process_menu_event (MenuItem & menu_item)
{
   task::RunLoopGui & run_loop = task::RunLoopGui::use_instance ();
   
   run_loop.notify_pre_gui_event ();
                  
   _menu_event._menu_item_ptr = &menu_item;
   _menu_event._handled_flag = false;
   
   menu_item.signal_pre ();
   
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
   
   menu_item.signal_post ();
   
   _menu_event.reset ();
   
   run_loop.notify_post_gui_event ();
}



const ::OSType MenuBarImpl::_command_id_opal = 'opal';
const ::OSType MenuBarImpl::_creator_ohm_force = 'OmFo';
const ::OSType MenuBarImpl::_tag_menu_item_ptr = 'MeIt';



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
