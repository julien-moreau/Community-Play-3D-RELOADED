/*****************************************************************************

        MenuBarImpl.cpp
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 70182

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/win32/MenuBarImpl.h"
#include "ohm/opal/win32/ApplicationImpl.h"
#include "ohm/opal/MenuBar.h"
#include "ohm/opal/MenuRoot.h"
#include "ohm/opal/MenuItem.h"
#include "ohm/opal/MenuSeparator.h"
#include "ohm/opal/View.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/win32/RunLoopGuiImpl.h"

#include <cassert>

#define ohm_opal_MENU_BAR_IMPL_PREFS_SHORTCUT_FLAG



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
   ::DestroyMenu (_hmenu);
   _hmenu = 0;

   ::DestroyAcceleratorTable (_haccel);
   _haccel = 0;
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  MenuBarImpl::release ()
{
   task::RunLoopGui::use_instance ().remove (_observer);
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  MenuBarImpl::bind (::HWND hwnd)
{
   assert (_hwnd == 0);

   _hwnd = hwnd;

   BOOL check_flag = ::SetMenu (_hwnd, _hmenu);
   assert (check_flag == TRUE);

   task::RunLoopGui::use_instance ().use_impl ().bind (*this);
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  MenuBarImpl::unbind (::HWND hwnd)
{
   assert (_hwnd == hwnd);
   ::SetMenu (_hwnd, 0);
   
   task::RunLoopGui::use_instance ().use_impl ().unbind (*this);
}



/*
==============================================================================
Name : update
==============================================================================
*/

bool  MenuBarImpl::handle (MenuId id, bool shortcut_flag)
{
   bool ret_val = false;

   MenuMap::iterator it = _menu_map.find (id);

   if (it != _menu_map.end ())
   {
      opal::MenuItem * item_ptr =  it->second;
      
      assert (item_ptr != 0);

      ret_val = handle (*item_ptr, shortcut_flag);
   }
   else
   {
      assert (false);
   }

   return ret_val;
}



/*
==============================================================================
Name : update
==============================================================================
*/

bool  MenuBarImpl::handle (opal::MenuStdId opal_id, bool shortcut_flag)
{
   bool ret_val = false;

   ItemPredicate predicate (opal_id);

   MenuMap::iterator it = _menu_map.begin ();

   if (it != _menu_map.end ())
   {
      opal::MenuItem * item_ptr =  it->second;
      
      assert (item_ptr != 0);

      ret_val = handle (*item_ptr, shortcut_flag);
   }

   return ret_val;
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  MenuBarImpl::update (MenuRoot & menu_root)
{
   // for now, the easy way
   clear_menu ();

   archi::UInt16  id = 0;
   AccelArray     accel_arr;
   update (menu_root, _hmenu, id, accel_arr);

   assert (_haccel == 0);
   if (accel_arr.size () > 0)
   {
      _haccel = ::CreateAcceleratorTable (&accel_arr [0], accel_arr.size ());
   }

   ::DrawMenuBar (_hwnd);
}



bool  MenuBarImpl::translate (::MSG & msg)
{
   bool ret_val = false;

   if (_haccel != 0 && _hwnd != 0)
   {
      if (msg.message == WM_INPUTLANGCHANGEREQUEST)
      {
         task::Event & event = ohm_task_EVENT_GET;
         
         event.push (EventType_UPDATE);
         
         _observer.send (event);
      }

      ret_val = (::TranslateAccelerator (_hwnd, _haccel, &msg) != 0);
   }

   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

MenuBarImpl::MenuBarImpl ()
:  _hmenu (0)
,  _hwnd (0)
,  _haccel (0)
,  _observer ("ohm.opal.MenuBarImpl")
{
   _hmenu = ::CreateMenu ();

#if 0
   MENUINFO menu_info;
   ::memset (&menu_info, 0, sizeof (menu_info));
   menu_info.cbSize = sizeof (menu_info);
   menu_info.fMask = MIM_STYLE;
   menu_info.dwStyle = MNS_NOTIFYBYPOS;
   ::SetMenuInfo (_hmenu, &menu_info);
#endif
   
   _observer.bind_process <
      MenuBarImpl,
      &MenuBarImpl::process
   > (*this);

   task::RunLoopGui::use_instance ().add (_observer);
}



void  MenuBarImpl::process (task::Event & event)
{
   EventType type = event.pop <EventType> ();
   
   assert (type == EventType_UPDATE);
   
   // if we use partial update, we'll need to call a specific function
   // which forces an update on the whole menu
   update (MenuBar::use_instance ().use_root ());
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ItemPredicate
==============================================================================
*/

MenuBarImpl::ItemPredicate::ItemPredicate (opal::MenuStdId id)
:  _id (id)
{
}



/*
==============================================================================
Name: operator bool
==============================================================================
*/

bool MenuBarImpl::ItemPredicate::operator () (opal::MenuItem * item_ptr) const
{
   bool ret_val = false;
   
   if (item_ptr != 0)
   {
      if (item_ptr->get_std_id () == _id)
      {
         ret_val = true;
      }
   }
   
   return ret_val;
}



void  MenuBarImpl::clear_menu ()
{
/* int item_count = ::GetMenuItemCount (_hmenu);

   for (int i = 0 ; i < item_count ; ++i)
   {
      ::MENUITEMINFO item_info;
      ::memset (&item_info, 0, sizeof (item_info));
      item_info.cbSize = sizeof (item_info);
      item_info.fMask = MIIM_SUBMENU;

      BOOL ok_flag = ::GetMenuItemInfo (_hmenu, i, TRUE, &item_info);
      assert (ok_flag == TRUE);

      ::DeleteMenu (
   }
*/
   BOOL ok_flag = TRUE;

   while (ok_flag)
   {
      ok_flag = ::DeleteMenu (_hmenu, 0, MF_BYPOSITION);
   }
   MenuMap ().swap (_menu_map);

   if (_haccel != 0)
   {
      ok_flag = ::DestroyAcceleratorTable (_haccel);
      assert (ok_flag == TRUE);
      _haccel = 0;
   }
}



void  MenuBarImpl::update (MenuContainer & menu_container, HMENU hmenu, archi::UInt16 & id, AccelArray & accel_arr)
{
   assert (&menu_container != 0);
   assert (hmenu != 0);

   MenuContainer::iterator it = menu_container.begin ();
   MenuContainer::iterator it_end = menu_container.end ();

   unsigned int position = 0;

   for ( ; it != it_end ; ++it, ++position)
   {
      MenuBase & menu_base = *it;
      
      MenuItem * menu_item_ptr = dynamic_cast <MenuItem *> (&menu_base);
      MenuSeparator * menu_separator_ptr = dynamic_cast <MenuSeparator *> (&menu_base);

      assert ((hmenu != _hmenu) || (menu_separator_ptr == 0));
      
      MENUITEMINFO item_info;
      ::memset (&item_info, 0, sizeof (item_info));
      item_info.cbSize = sizeof (MENUITEMINFO);

      if (menu_item_ptr != 0)
      {
         std::string menu_name = menu_item_ptr->get_title ();
         const opal::Key & key = menu_item_ptr->get_keystroke ();
         bool preferences_flag = menu_item_ptr->is_preferences ();

         if (menu_item_ptr->is_marked ())
         {
            item_info.fMask |= MIIM_STATE;
            item_info.fState |= MFS_CHECKED;
         }
         
         if (!menu_item_ptr->is_enabled ())
         {
            item_info.fMask |= MIIM_STATE;
            item_info.fState |= MFS_DISABLED;
         }
         
         if (key != Key::None)
         {
            ::ACCEL accel;
            std::string shortcut_str;

            bool ret_flag = key.to_menu_info (accel, shortcut_str);

            if (ret_flag)
            {
               accel.cmd = id;

               menu_name += '\t';
               menu_name += shortcut_str;
               
               if (menu_item_ptr->is_enabled ())
               {
                  accel_arr.push_back (accel);
               }
            }
         }
#if defined (ohm_opal_MENU_BAR_IMPL_PREFS_SHORTCUT_FLAG)
         else if (preferences_flag)
         {
            ::ACCEL accel;
            std::string shortcut_str;
            
            key.to_menu_modifier (accel, shortcut_str);

            accel.key = ',';
            shortcut_str += ',';
            
            accel.cmd = id;

            menu_name += '\t';
            menu_name += shortcut_str;
            
            if (menu_item_ptr->is_enabled ())
            {
               accel_arr.push_back (accel);
            }
         }
#endif

         item_info.fMask |= MIIM_STRING;
         item_info.dwTypeData = ::LPSTR (menu_name.c_str ());

         item_info.fMask |= MIIM_DATA;
         item_info.dwItemData = ::ULONG_PTR (menu_item_ptr);

         item_info.fMask |= MIIM_ID;
         item_info.wID = id;
         
         assert (_menu_map.find (id) == _menu_map.end ());

         _menu_map.insert (std::make_pair (id, menu_item_ptr));
   
         ++id;

         if (!menu_item_ptr->empty ())
         {
            // submenu

            ::HMENU hsubmenu = ::CreatePopupMenu ();

            item_info.fMask |= MIIM_SUBMENU;
            item_info.hSubMenu = hsubmenu;

            update (*menu_item_ptr, hsubmenu, id, accel_arr);
         }

         ::InsertMenuItem (hmenu, position, TRUE, &item_info);
      }
      else if (menu_separator_ptr != 0)
      {
         item_info.fMask |= MIIM_FTYPE;
         item_info.fType = MFT_SEPARATOR;

         ::InsertMenuItem (hmenu, position, TRUE, &item_info);
      }
      else
      {
         assert (false);
      }
   }
}



bool MenuBarImpl::handle (MenuItem & item, bool shortcut_flag)
{
   bool ret_val = false;

   bool process_flag = true;
   
   const Key & key = item.get_keystroke ();

   ViewImpl * view_impl_ptr = ViewImpl::get (_hwnd);
   assert (view_impl_ptr != 0);

   View & view = view_impl_ptr->use_view ();

   if (key != Key::None && shortcut_flag && (item.get_std_id () == MenuStdId_NONE))
   {
      if (view_impl_ptr != 0)
      {
         if (view.get_keyboard_focus_frame_ptr () != 0)
         {
            process_flag = false;
         }
      }
   }

   if (process_flag)
   {
      MenuEvent menu_event;
      menu_event._menu_item_ptr = &item;
      menu_event._handled_flag = false;
      
      item.signal_pre ();
      
      if (item.get_std_id () != MenuStdId_NONE)
      {
         util::Delegate & delegate = item.use_std_id_delegate ();
         
         if (delegate.is_set ())
         {
            delegate.param_ref_signal (menu_event);
         }
      }
      
      if (!menu_event._handled_flag)
      {
         util::Delegate & delegate = item.use_delegate ();
         
         if (delegate.is_set ())
         {
            delegate.param_ref_signal (menu_event);
         }
      }
      
      item.signal_post ();
      
      ret_val = true;   
   }

   if (shortcut_flag)
   {
      // send an opal keyboard event
      task::RunLoopGui::use_instance ().notify_pre_gui_event ();
#if defined ohm_opal_VIEW_USE_EVENT_MGR
      if (! view_impl_ptr->key_pressed (key, false))
#else
      if (! view.key_pressed (key))
#endif
      {
         std::string typed_str;

         if (key.to_utf_8 (typed_str))
         {
#if defined ohm_opal_VIEW_USE_EVENT_MGR
            view_impl_ptr->characters_typed (typed_str);
#else
            view.characters_typed (typed_str);
#endif
         }
      }
      task::RunLoopGui::use_instance ().notify_post_gui_event ();
   }

   return ret_val;
}


}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

