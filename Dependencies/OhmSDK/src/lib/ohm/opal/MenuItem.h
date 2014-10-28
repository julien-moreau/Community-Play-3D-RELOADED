/*****************************************************************************

        MenuItem.h
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



#if ! defined (ohm_opal_MenuItem_HEADER_INCLUDED)
#define  ohm_opal_MenuItem_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/MenuContainer.h"
#include "ohm/opal/Key.h"
#include "ohm/opal/MenuEvent.h"
#include "ohm/opal/MenuStdId.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opal
{



class MenuItem
:  public MenuContainer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  MenuItem (MenuContainer & parent);
   virtual        ~MenuItem ();

   // MenuBase
   virtual void   synchronize ();
   
   void           set_title (const std::string & title);
   bool           was_title_modified () const;
   const std::string &
                  get_title () const;

   void           mark (bool mark_flag = true);
   bool           was_mark_modified () const;
   bool           is_marked () const;
   
   void           enable (bool enable_flag = true);
   bool           was_enable_modified () const;
   bool           is_enabled () const;
   
   void           set_keystroke (const Key & key);
   bool           was_keystroke_modified () const;
   const Key &    get_keystroke () const;
   
   void           set_std_id (MenuStdId std_id);
   bool           was_std_id_modified () const;
   MenuStdId      get_std_id () const;

   template <class T, void (T::*proc) (MenuEvent &)>
   void           bind_std_id (T & receiver);
   void           bind_std_id (const util::Delegate & delegate);
   void           unbind_std_id ();
   util::Delegate &
                  use_std_id_delegate ();

   template <class T, void (T::*proc) (MenuEvent &)>
   void           bind (T & receiver);
   void           bind (const util::Delegate & delegate);
   void           unbind ();
   bool           was_delegate_modified () const;
   util::Delegate &
                  use_delegate ();
   
   void           set_quit ();
   bool           is_quit () const;

   void           set_preferences ();
   bool           is_preferences () const;
   
   // access is reserved to MenuBarImpl
   void           set_user_data (void * user_data_ptr);
   void *         get_user_data () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::string    _title;
   bool           _title_modified_flag;

   bool           _mark_flag;
   bool           _mark_modified_flag;
   
   bool           _enable_flag;
   bool           _enable_modified_flag;
   
   opal::Key      _keystroke;
   bool           _keystroke_modified_flag;
   
   MenuStdId      _std_id;
   bool           _std_id_modified_flag;
   util::Delegate _std_id_delegate;
   
   util::Delegate _delegate;
   bool           _delegate_modified_flag;

   bool           _quit_flag;
   bool           _preferences_flag;
   
   void *         _user_data_ptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MenuItem ();
                  MenuItem (const MenuItem &other);
   MenuItem &     operator = (const MenuItem &other);
   bool           operator == (const MenuItem &other);
   bool           operator != (const MenuItem &other);

}; // class MenuItem



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/MenuItem.hpp"



#endif   // ohm_opal_MenuItem_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
