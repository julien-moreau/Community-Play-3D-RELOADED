/*****************************************************************************

        MenuContainer.h
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



#if ! defined (ohm_opal_MenuContainer_HEADER_INCLUDED)
#define  ohm_opal_MenuContainer_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/MenuBase.h"
#include "ohm/opal/MenuEvent.h"
#include "ohm/opal/MenuStdId.h"
#include "ohm/mem/SharedPtr.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opal
{



class MenuItem;
class Key;

class MenuContainer
:  public MenuBase
{

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef mem::SharedPtr <MenuBase> MenuSPtr;
   
   enum MenuState
   {
                  MenuState_RESIDENT = 0,
                  MenuState_ADDED,
                  MenuState_REMOVED,
   };
   
   class MenuInfo
   {
   public:
      MenuSPtr    _menu_sptr;
      MenuState   _state;
   };
   
   typedef std::list <MenuInfo> MenuInfoList;
   

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class iterator
   {
   public:
                  iterator ();
                  iterator (const iterator & other);
      virtual     ~iterator ();
      
      iterator &  operator = (const iterator & other);
      MenuBase &  operator * ();
      MenuBase *  operator -> ();
      iterator &  operator ++ ();
      iterator    operator ++ (int);
      bool        operator == (const iterator & other) const;
      bool        operator != (const iterator & other) const;

      bool        was_added () const;
      bool        was_removed () const;
   
   private:
      friend class MenuContainer;
                  iterator (MenuInfoList::iterator it, MenuContainer & parent);
      MenuInfoList::iterator
                  _it;
      MenuContainer *
                  _parent_ptr;
   };
   
                  MenuContainer (int);
                  MenuContainer (int, MenuContainer & parent);
   virtual        ~MenuContainer ();
   
   // MenuBase
   virtual void   synchronize ();
   
   bool           empty () const;
   size_t         size () const;
   
   iterator       begin ();
   iterator       end ();
   iterator       find (MenuBase & menu);
   MenuBase *     find (const std::string & identifier);
   MenuBase *     find_recursive (const std::string & identifier);
   MenuItem *     find_item (const std::string & identifier);
   MenuItem *     find_item_recursive (const std::string & identifier);
   MenuItem *     find_item (MenuStdId identifier);
   MenuItem *     find_item_recursive (MenuStdId identifier);
   MenuItem *     find_item (const Key & key);
   MenuItem *     find_item_recursive (const Key & key);
   
   MenuItem &     insert_menu_item (iterator it);
   void           insert_menu_separator (iterator it);
   void           insert_menu_separator_and_quit (iterator it);
   void           erase (iterator it);
   void           clear ();
   
   // helper
   template <class T, void (T::*proc) (MenuEvent &)>
   void           bind (T & receiver, const std::string & identifier, bool bind_flag = true);
   void           bind (const util::Delegate & delegate, const std::string & identifier, bool bind_flag);
   void           enable (const std::string & identifier, bool enable_flag = true);
   void           mark (const std::string & identifier, bool mark_flag = true);
   void           enable_recursive (bool enable_flag = true);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class iterator;
   
   MenuInfoList   _menu_info_list;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MenuContainer ();
                  MenuContainer (const MenuContainer &other);
   MenuContainer &   operator = (const MenuContainer &other);
   bool           operator == (const MenuContainer &other);
   bool           operator != (const MenuContainer &other);

}; // class MenuContainer



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/MenuContainer.hpp"


#endif   // ohm_opal_MenuContainer_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
