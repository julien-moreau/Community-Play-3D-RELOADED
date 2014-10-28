/*****************************************************************************

        MenuBarImpl.h
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 62058

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
#include "ohm/archi/types.h"
#include "ohm/opal/MenuStdId.h"
#include "ohm/task/win32/AcceleratorInterface.h"
#include "ohm/task/Observer.h"

#include <map>
#include <vector>

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#include <windows.h>


namespace ohm
{
namespace opal
{



class MenuRoot;
class MenuContainer;
class MenuItem;
class Window;

class MenuBarImpl
:  public util::SingletonBase <MenuBarImpl>
,  public task::AcceleratorInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef archi::UInt16                        MenuId;

   virtual        ~MenuBarImpl ();
   
   void           release ();
   
   void           bind (::HWND hwnd);
   void           unbind (::HWND hwnd);
   bool           handle (MenuId id, bool shortcut_flag);
   bool           handle (opal::MenuStdId opal_id, bool shortcut_flag);

   void           update (MenuRoot & menu_root);

   // task::AcceleratorInterface
   virtual bool   translate (::MSG & msg);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  MenuBarImpl ();


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ohm_util_SingletonBase_INVITE_SINGLETON (MenuBarImpl);
   
   class ItemPredicate
   {
   public:
                  ItemPredicate (opal::MenuStdId id);
      virtual     ~ItemPredicate () {}

      bool        operator () (opal::MenuItem * item_ptr) const;

   private:
      opal::MenuStdId
                  _id;
   };

   enum EventType
   {
                  EventType_UPDATE = 0,
   };
   
   typedef std::map <MenuId, MenuItem *>        MenuMap;
   typedef std::vector <::ACCEL>                AccelArray;


   void           clear_menu ();
   void           update (MenuContainer & menu_container, HMENU hmenu, archi::UInt16 & id, AccelArray & accel_arr);
   bool           handle (MenuItem & item, bool shortcut_flag);
   
   // RunLoopGui observer delegate
   void           process (task::Event & event);


   ::HMENU        _hmenu;
   ::HWND         _hwnd;
   MenuMap        _menu_map;
   HACCEL         _haccel;
   
   task::Observer _observer;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MenuBarImpl (const MenuBarImpl &other);
   MenuBarImpl &  operator = (const MenuBarImpl &other);
   bool           operator == (const MenuBarImpl &other) const;
   bool           operator != (const MenuBarImpl &other) const;

}; // class MenuBarImpl



}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/MenuBarImpl.hpp"



#endif   // ohm_opal_MenuBarImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

