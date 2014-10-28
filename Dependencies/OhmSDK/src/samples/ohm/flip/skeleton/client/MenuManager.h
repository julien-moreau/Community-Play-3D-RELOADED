/*****************************************************************************

        MenuManager.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71391

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



#if ! defined (client_MenuManager_HEADER_INCLUDED)
#define  client_MenuManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/MenuRoot.h"
#include "ohm/opal/private/PostActionListener.h"



namespace ohm
{
namespace opa
{
class BundleView;
}
}

namespace model
{
class Root;
}



namespace client
{



class MenuManager
:  public ohm::opal::PostActionListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  MenuManager (ohm::opa::BundleView & view);
   virtual        ~MenuManager ();
   
   void           bind (model::Root & root);
   
   // ohm::opal::PostActionListener
   virtual void   notify_pre_action ();
   virtual void   notify_post_action ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           setup_menu ();
   void           setup_menu_file (ohm::opal::MenuRoot & menu_root);
   void           setup_menu_edit (ohm::opal::MenuRoot & menu_root);

   void           update_menu ();
   void           update_menu_file (ohm::opal::MenuRoot & menu_root);
   void           update_menu_edit (ohm::opal::MenuRoot & menu_root);
   
   void           req_exit (ohm::opal::MenuEvent & event);

   void           req_edit_undo (ohm::opal::MenuEvent & event);
   void           req_edit_redo (ohm::opal::MenuEvent & event);
   void           req_edit_cut (ohm::opal::MenuEvent & event);
   void           req_edit_copy (ohm::opal::MenuEvent & event);
   void           req_edit_paste (ohm::opal::MenuEvent & event);
   void           req_edit_select_all (ohm::opal::MenuEvent & event);

   void           req_project_new_taxi (ohm::opal::MenuEvent & event);
   void           req_project_new_pedestrian (ohm::opal::MenuEvent & event);

   void           req_project_color (ohm::opal::MenuEvent & event);

   ohm::opa::BundleView &
                  _view;
   
   model::Root *  _root_ptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MenuManager ();
                  MenuManager (const MenuManager &other);
   MenuManager &  operator = (const MenuManager &other);
   bool           operator == (const MenuManager &other);
   bool           operator != (const MenuManager &other);

}; // class MenuManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/MenuManager.hpp"



#endif   // client_MenuManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
