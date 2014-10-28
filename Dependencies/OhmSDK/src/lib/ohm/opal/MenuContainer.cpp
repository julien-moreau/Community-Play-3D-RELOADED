/*****************************************************************************

        MenuContainer.cpp
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

#include "ohm/opal/MenuContainer.h"
#include "ohm/opal/MenuItem.h"
#include "ohm/opal/MenuSeparator.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

MenuContainer::iterator::iterator ()
:  _it ()
,  _parent_ptr (0)
{
}



/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

MenuContainer::iterator::iterator (const iterator & other)
:  _it (other._it)
,  _parent_ptr (other._parent_ptr)
{
}



/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

MenuContainer::iterator::~iterator ()
{
}



/*
==============================================================================
Name : iterator::operator =
==============================================================================
*/

MenuContainer::iterator &  MenuContainer::iterator::operator = (const iterator & other)
{
   _it = other._it;
   _parent_ptr = other._parent_ptr;
   
   return *this;
}



/*
==============================================================================
Name : iterator::operator *
==============================================================================
*/

MenuBase &  MenuContainer::iterator::operator * ()
{
   MenuSPtr & menu_sptr = _it->_menu_sptr;
   
   return *menu_sptr;
}



/*
==============================================================================
Name : iterator::operator ->
==============================================================================
*/

MenuBase *  MenuContainer::iterator::operator -> ()
{
   MenuSPtr & menu_sptr = _it->_menu_sptr;
   
   return &*menu_sptr;
}



/*
==============================================================================
Name : iterator::operator ++ (pre)
==============================================================================
*/

MenuContainer::iterator &  MenuContainer::iterator::operator ++ ()
{
   assert (_parent_ptr != 0);

   assert (_it != _parent_ptr->_menu_info_list.end ());
   
   bool total_iterate_flag = _parent_ptr->is_updating ();
   
   if (total_iterate_flag)
   {
      ++_it;
   }
   else
   {
      assert (_it->_state != MenuState_REMOVED);
      
      while (true)
      {
         ++_it;
         
         if (_it == _parent_ptr->_menu_info_list.end ())
         {
            break;
         }
         
         if (_it->_state != MenuState_REMOVED)
         {
            break;
         }
      }
   }
   
   return *this;
}



/*
==============================================================================
Name : iterator::operator ++ (post)
==============================================================================
*/

MenuContainer::iterator MenuContainer::iterator::operator ++ (int)
{
   iterator tmp = *this;

   ++*this;
   
   return tmp;
}



/*
==============================================================================
Name : iterator::operator ==
==============================================================================
*/

bool  MenuContainer::iterator::operator == (const iterator & other) const
{
   return _it == other._it;
}



/*
==============================================================================
Name : iterator::operator !=
==============================================================================
*/

bool  MenuContainer::iterator::operator != (const iterator & other) const
{
   return _it != other._it;
}



/*
==============================================================================
Name : iterator::was_added
==============================================================================
*/

bool  MenuContainer::iterator::was_added () const
{
   return (_it->_state == MenuState_ADDED);
}



/*
==============================================================================
Name : iterator::was_removed
==============================================================================
*/

bool  MenuContainer::iterator::was_removed () const
{
   return (_it->_state == MenuState_REMOVED);
}



/*
==============================================================================
Name : iterator::ctor
==============================================================================
*/

MenuContainer::iterator::iterator (MenuInfoList::iterator it, MenuContainer & parent)
:  _it (it)
,  _parent_ptr (&parent)
{
   assert (&parent != 0);
}



/*
==============================================================================
Name : ctor
Description :
   Root version
==============================================================================
*/

MenuContainer::MenuContainer (int)
:  MenuBase (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

MenuContainer::MenuContainer (int, MenuContainer & parent)
:  MenuBase (parent)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MenuContainer::~MenuContainer ()
{
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  MenuContainer::synchronize ()
{
   MenuInfoList::iterator it = _menu_info_list.begin ();
   const MenuInfoList::iterator it_end = _menu_info_list.end ();
   
   for (; it != it_end ;)
   {
      MenuInfoList::iterator next_it = it;
      ++next_it;
      
      MenuInfo & menu_info = *it;
      
      if (menu_info._state == MenuState_REMOVED)
      {
         _menu_info_list.erase (it);
      }
      else
      {
         menu_info._state = MenuState_RESIDENT;
         
         MenuBase & menu = *it->_menu_sptr;
         
         menu.synchronize ();
      }
      
      it = next_it;
   }
   
   MenuBase::synchronize ();
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  MenuContainer::empty () const
{
   return (size () == 0);
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   MenuContainer::size () const
{
   if (is_updating ())
   {
      return _menu_info_list.size ();
   }
   else
   {
      size_t ret_val = 0;
      
      MenuInfoList::const_iterator it = _menu_info_list.begin ();
      const MenuInfoList::const_iterator it_end = _menu_info_list.end ();
      
      for (; it != it_end ; ++it)
      {
         const MenuInfo & menu_info = *it;
         
         if (menu_info._state != MenuState_REMOVED)
         {
            ++ret_val;
         }
      }
      
      return ret_val;
   }
}



/*
==============================================================================
Name : begin
==============================================================================
*/

MenuContainer::iterator MenuContainer::begin ()
{
   iterator it (_menu_info_list.begin (), *this);
   
   return it;
}



/*
==============================================================================
Name : end
==============================================================================
*/

MenuContainer::iterator MenuContainer::end ()
{
   iterator it (_menu_info_list.end (), *this);
   
   return it;
}



/*
==============================================================================
Name : find
==============================================================================
*/

MenuContainer::iterator MenuContainer::find (MenuBase & menu)
{
   bool updating_flag = is_updating ();
   
   MenuInfoList::iterator it = _menu_info_list.begin ();
   const MenuInfoList::iterator it_end = _menu_info_list.end ();
   
   for (; it != it_end ; ++it)
   {
      MenuInfo & menu_info = *it;
      
      if ((menu_info._state != MenuState_REMOVED) || updating_flag)
      {
         MenuSPtr & menu_sptr = menu_info._menu_sptr;

         MenuBase * menu_iptr = menu_sptr.get ();
         assert (menu_iptr != 0);
         
         if (menu_iptr == &menu)
         {
            break;
         }
      }
   }
   
   return iterator (it, *this);
}



/*
==============================================================================
Name : find
==============================================================================
*/

MenuBase *  MenuContainer::find (const std::string & identifier)
{
   bool updating_flag = is_updating ();
   
   MenuInfoList::iterator it = _menu_info_list.begin ();
   const MenuInfoList::iterator it_end = _menu_info_list.end ();
   
   for (; it != it_end ; ++it)
   {
      MenuInfo & menu_info = *it;
      
      if ((menu_info._state != MenuState_REMOVED) || updating_flag)
      {
         MenuSPtr & menu_sptr = menu_info._menu_sptr;

         MenuBase * menu_ptr = menu_sptr.get ();
         assert (menu_ptr != 0);
         
         if (menu_ptr->get_identifier () == identifier)
         {
            return menu_ptr;
         }
      }
   }

   return 0;
}



/*
==============================================================================
Name : find_recursive
==============================================================================
*/

MenuBase *  MenuContainer::find_recursive (const std::string & identifier)
{
   bool updating_flag = is_updating ();
   
   MenuBase * menu_ptr = find (identifier);
   
   if (menu_ptr == 0)
   {
      MenuInfoList::iterator it = _menu_info_list.begin ();
      const MenuInfoList::iterator it_end = _menu_info_list.end ();
      
      for (; it != it_end ; ++it)
      {
         MenuInfo & menu_info = *it;
         
         if ((menu_info._state != MenuState_REMOVED) || updating_flag)
         {
            MenuSPtr & menu_sptr = it->_menu_sptr;

            MenuBase * menu_iptr = menu_sptr.get ();
            assert (menu_iptr != 0);
            
            MenuContainer * menu_container_ptr
               = dynamic_cast <MenuContainer *> (menu_iptr);
            
            if (menu_container_ptr != 0)
            {
               menu_ptr = menu_container_ptr->find (identifier);
               
               if (menu_ptr != 0)
               {
                  break;
               }
            }
         }
      }
   }
   
   return menu_ptr;
}



/*
==============================================================================
Name : find_item
==============================================================================
*/

MenuItem *  MenuContainer::find_item (const std::string & identifier)
{
   MenuBase * menu_ptr = find (identifier);
   
   return dynamic_cast <MenuItem *> (menu_ptr);
}



/*
==============================================================================
Name : find_item_recursive
==============================================================================
*/

MenuItem *  MenuContainer::find_item_recursive (const std::string & identifier)
{
   MenuBase * menu_ptr = find_recursive (identifier);
   
   return dynamic_cast <MenuItem *> (menu_ptr);
}



/*
==============================================================================
Name : find_item
==============================================================================
*/

MenuItem *  MenuContainer::find_item (MenuStdId identifier)
{
   bool updating_flag = is_updating ();
   
   MenuInfoList::iterator it = _menu_info_list.begin ();
   const MenuInfoList::iterator it_end = _menu_info_list.end ();
   
   for (; it != it_end ; ++it)
   {
      MenuInfo & menu_info = *it;
      
      if ((menu_info._state != MenuState_REMOVED) || updating_flag)
      {
         MenuSPtr & menu_sptr = menu_info._menu_sptr;

         MenuBase * menu_ptr = menu_sptr.get ();
         assert (menu_ptr != 0);
         
         MenuItem * item_ptr = dynamic_cast <MenuItem *> (menu_ptr);
         
         if (item_ptr != 0)
         {
            if (item_ptr->get_std_id () == identifier)
            {
               return item_ptr;
            }
         }
      }
   }

   return 0;
}



/*
==============================================================================
Name : find_item_recursive
==============================================================================
*/

MenuItem *  MenuContainer::find_item_recursive (MenuStdId identifier)
{
   bool updating_flag = is_updating ();
   
   MenuItem * menu_ptr = find_item (identifier);
   
   if (menu_ptr == 0)
   {
      MenuInfoList::iterator it = _menu_info_list.begin ();
      const MenuInfoList::iterator it_end = _menu_info_list.end ();
      
      for (; it != it_end ; ++it)
      {
         MenuInfo & menu_info = *it;
         
         if ((menu_info._state != MenuState_REMOVED) || updating_flag)
         {
            MenuSPtr & menu_sptr = it->_menu_sptr;

            MenuBase * menu_iptr = menu_sptr.get ();
            assert (menu_iptr != 0);
            
            MenuContainer * menu_container_ptr
               = dynamic_cast <MenuContainer *> (menu_iptr);
            
            if (menu_container_ptr != 0)
            {
               menu_ptr = menu_container_ptr->find_item (identifier);
               
               if (menu_ptr != 0)
               {
                  break;
               }
            }
         }
      }
   }
   
   return menu_ptr;
}



/*
==============================================================================
Name : find_item
==============================================================================
*/

MenuItem *  MenuContainer::find_item (const Key & key)
{
   bool updating_flag = is_updating ();
   
   MenuInfoList::iterator it = _menu_info_list.begin ();
   const MenuInfoList::iterator it_end = _menu_info_list.end ();
   
   for (; it != it_end ; ++it)
   {
      MenuInfo & menu_info = *it;
      
      if ((menu_info._state != MenuState_REMOVED) || updating_flag)
      {
         MenuSPtr & menu_sptr = menu_info._menu_sptr;

         MenuBase * menu_ptr = menu_sptr.get ();
         assert (menu_ptr != 0);
         
         MenuItem * item_ptr = dynamic_cast <MenuItem *> (menu_ptr);
         
         if (item_ptr != 0)
         {
            if (item_ptr->get_keystroke () == key)
            {
               return item_ptr;
            }
         }
      }
   }

   return 0;
}



/*
==============================================================================
Name : find_item_recursive
==============================================================================
*/

MenuItem *  MenuContainer::find_item_recursive (const Key & key)
{
   bool updating_flag = is_updating ();
   
   MenuItem * menu_ptr = find_item (key);
   
   if (menu_ptr == 0)
   {
      MenuInfoList::iterator it = _menu_info_list.begin ();
      const MenuInfoList::iterator it_end = _menu_info_list.end ();
      
      for (; it != it_end ; ++it)
      {
         MenuInfo & menu_info = *it;
         
         if ((menu_info._state != MenuState_REMOVED) || updating_flag)
         {
            MenuSPtr & menu_sptr = it->_menu_sptr;

            MenuBase * menu_iptr = menu_sptr.get ();
            assert (menu_iptr != 0);
            
            MenuContainer * menu_container_ptr
               = dynamic_cast <MenuContainer *> (menu_iptr);
            
            if (menu_container_ptr != 0)
            {
               menu_ptr = menu_container_ptr->find_item (key);
               
               if (menu_ptr != 0)
               {
                  break;
               }
            }
         }
      }
   }
   
   return menu_ptr;
}



/*
==============================================================================
Name : insert_menu_item
==============================================================================
*/

MenuItem &  MenuContainer::insert_menu_item (iterator it)
{
   MenuItem * menu_ptr = new MenuItem (*this);
   
   MenuSPtr menu_sptr = MenuSPtr (menu_ptr);
   
   MenuInfo menu_info;
   menu_info._menu_sptr = menu_sptr;
   menu_info._state = MenuState_ADDED;
   
   _menu_info_list.insert (it._it, menu_info);
   
   modify ();
   
   return *menu_ptr;
}



/*
==============================================================================
Name : insert_menu_separator
==============================================================================
*/

void  MenuContainer::insert_menu_separator (iterator it)
{
   MenuSeparator * menu_ptr = new MenuSeparator (*this);
   
   MenuSPtr menu_sptr = MenuSPtr (menu_ptr);
   
   MenuInfo menu_info;
   menu_info._menu_sptr = menu_sptr;
   menu_info._state = MenuState_ADDED;
   
   _menu_info_list.insert (it._it, menu_info);
   
   modify ();
}



/*
==============================================================================
Name : insert_menu_separator_and_quit
==============================================================================
*/

void  MenuContainer::insert_menu_separator_and_quit (iterator it)
{
   insert_menu_separator (it);
   MenuItem & menu_item = insert_menu_item (it);
   
   menu_item.set_quit ();
}



/*
==============================================================================
Name : erase
==============================================================================
*/

void  MenuContainer::erase (iterator it)
{
   assert (it._it->_state != MenuState_REMOVED);
   
   it._it->_state = MenuState_REMOVED;
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  MenuContainer::clear ()
{
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ;)
   {
      iterator it_next = it;
      ++it_next;
      
      // does not invalidate it
      erase (it);
      
      it = it_next;
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  MenuContainer::bind (const util::Delegate & delegate, const std::string & identifier, bool bind_flag)
{
   MenuItem * menu_ptr = find_item_recursive (identifier);
   
   if (menu_ptr != 0)
   {
      if (bind_flag)
      {
         menu_ptr->bind (delegate);
      }
      else
      {
         menu_ptr->unbind ();
      }
   }
   else
   {
      if (bind_flag)
      {
         ohm_util_TRACE_ERROR1 ("Cannot bind menu with id %1%", identifier.c_str ());
      }
      else
      {
         ohm_util_TRACE_ERROR1 ("Cannot unbind menu with id %1%", identifier.c_str ());
      }
   }
}



/*
==============================================================================
Name : enable
==============================================================================
*/

void  MenuContainer::enable (const std::string & identifier, bool enable_flag)
{
   MenuItem * menu_ptr = find_item_recursive (identifier);
   
   if (menu_ptr != 0)
   {
      menu_ptr->enable (enable_flag);
   }
   else
   {
      if (enable_flag)
      {
         ohm_util_TRACE_ERROR1 ("Cannot enable menu with id %1%", identifier.c_str ());
      }
      else
      {
         ohm_util_TRACE_ERROR1 ("Cannot disable menu with id %1%", identifier.c_str ());
      }
   }
}



/*
==============================================================================
Name : mark
==============================================================================
*/

void  MenuContainer::mark (const std::string & identifier, bool mark_flag)
{
   MenuItem * menu_ptr = find_item_recursive (identifier);
   
   if (menu_ptr != 0)
   {
      menu_ptr->mark (mark_flag);
   }
   else
   {
      if (mark_flag)
      {
         ohm_util_TRACE_ERROR1 ("Cannot mark menu with id %1%", identifier.c_str ());
      }
      else
      {
         ohm_util_TRACE_ERROR1 ("Cannot unmark menu with id %1%", identifier.c_str ());
      }
   }
}



/*
==============================================================================
Name : enable_recursive
==============================================================================
*/

void  MenuContainer::enable_recursive (bool enable_flag)
{
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ; ++it)
   {
      MenuBase * menu_ptr = &*it;
      
      MenuItem * item_ptr = dynamic_cast <MenuItem *> (menu_ptr);
      
      if (item_ptr != 0)
      {
         item_ptr->enable (enable_flag);
         
         item_ptr->enable_recursive (enable_flag);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
