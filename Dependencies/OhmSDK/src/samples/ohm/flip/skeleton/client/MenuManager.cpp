/*****************************************************************************

        MenuManager.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71422

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

#include "client/MenuManager.h"

#include "client/Constants.h"

#include "model/Root.h"

#include "ohm/archi/def.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opal/MenuBar.h"
#include "ohm/opal/MenuItem.h"
#include "ohm/task/RunLoopGui.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

MenuManager::MenuManager (ohm::opa::BundleView & view)
:  _view (view)
,  _root_ptr (0)
{
   using namespace ohm;
   
   opal::PostActionListener * pal_ptr = this;
   _view.add (*pal_ptr);
   
   setup_menu ();
   update_menu ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MenuManager::~MenuManager ()
{
   try
   {
      using namespace ohm;
   
      opal::PostActionListener * pal_ptr = this;
      _view.remove (*pal_ptr);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  MenuManager::bind (model::Root & root)
{
   _root_ptr = &root;
}



/*
==============================================================================
Name : notify_pre_action
Description :
==============================================================================
*/

void  MenuManager::notify_pre_action ()
{
   // nothing
}



/*
==============================================================================
Name : notify_post_action
Description :
==============================================================================
*/

void  MenuManager::notify_post_action ()
{
   using namespace ohm;
   
   opal::MenuRoot & menu_root = opal::MenuBar::use_instance ().use_root ();
   
   /*if (menu_root.needs_update ())
   {
      update_menu ();
   }*/
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : setup_menu
==============================================================================
*/

void  MenuManager::setup_menu ()
{
   using namespace ohm;
   
   opal::MenuRoot & menu_root = opal::MenuBar::use_instance ().use_root ();
      
   setup_menu_file (menu_root);
   setup_menu_edit (menu_root);

   opal::MenuBar::use_instance ().update ();
}



/*
==============================================================================
Name : setup_menu_file
==============================================================================
*/

void  MenuManager::setup_menu_file (ohm::opal::MenuRoot & menu_root)
{
   using namespace ohm;
   
   opal::MenuItem & menu = menu_root.insert_menu_item (menu_root.end ());
   
   menu.set_title ("File");
   menu.set_identifier (Constants::_menu_file_id_name_0);
   
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Exit");
      menu_item.set_identifier (Constants::_menu_file_item_id_exit_0);
      menu_item.bind <
         MenuManager, &MenuManager::req_exit
      > (*this);
   }
#endif
}



/*
==============================================================================
Name : setup_menu_edit
==============================================================================
*/

void  MenuManager::setup_menu_edit (ohm::opal::MenuRoot & menu_root)
{
   using namespace ohm;
   
   opal::MenuItem & menu = menu_root.insert_menu_item (menu_root.end ());
   
   menu.set_title ("Edit");
   menu.set_identifier (Constants::_menu_edit_id_name_0);
   
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Undo");
      menu_item.set_identifier (Constants::_menu_edit_item_id_undo_0);
      menu_item.set_std_id (opal::MenuStdId_UNDO);
      menu_item.set_keystroke (opal::Key::Command + opal::Key (opal::KeyChar_Z));
      menu_item.bind <
         MenuManager, &MenuManager::req_edit_undo
      > (*this);
   }
   
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Redo");
      menu_item.set_identifier (Constants::_menu_edit_item_id_redo_0);
      menu_item.set_std_id (opal::MenuStdId_REDO);
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
      menu_item.set_keystroke (opal::Key::Command + opal::Key::Shift + opal::Key (opal::KeyChar_Z));
#else
      menu_item.set_keystroke (opal::Key::Command + opal::Key (opal::KeyChar_Y));
#endif
      menu_item.bind <
         MenuManager, &MenuManager::req_edit_redo
      > (*this);
   }
   
   menu.insert_menu_separator (menu.end ());
   
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Cut");
      menu_item.set_identifier (Constants::_menu_edit_item_id_cut_0);
      menu_item.set_std_id (opal::MenuStdId_CUT);
      menu_item.set_keystroke (opal::Key::Command + opal::Key (opal::KeyChar_X));
      menu_item.bind <
         MenuManager, &MenuManager::req_edit_cut
      > (*this);
   }
   
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Copy");
      menu_item.set_identifier (Constants::_menu_edit_item_id_copy_0);
      menu_item.set_std_id (opal::MenuStdId_COPY);
      menu_item.set_keystroke (opal::Key::Command + opal::Key (opal::KeyChar_C));
      menu_item.bind <
         MenuManager, &MenuManager::req_edit_copy
      > (*this);
   }
   
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Paste");
      menu_item.set_identifier (Constants::_menu_edit_item_id_paste_0);
      menu_item.set_std_id (opal::MenuStdId_PASTE);
      menu_item.set_keystroke (opal::Key::Command + opal::Key (opal::KeyChar_V));
      menu_item.bind <
         MenuManager, &MenuManager::req_edit_paste
      > (*this);
   }
   
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Delete");
      menu_item.set_identifier (Constants::_menu_edit_item_id_delete_0);
      menu_item.set_keystroke (opal::Key(opal::KeyCode_BACKSPACE));

      // handled directly in FrameRoot
   }
   
   {
      opal::MenuItem & menu_item = menu.insert_menu_item (menu.end ());
      
      menu_item.set_title ("Select All");
      menu_item.set_identifier (Constants::_menu_edit_item_id_select_all_0);
      menu_item.set_std_id (opal::MenuStdId_SELECT_ALL);
      menu_item.set_keystroke (opal::Key::Command + opal::Key (opal::KeyChar_A));
      menu_item.bind <
         MenuManager, &MenuManager::req_edit_select_all
      > (*this);
   }

   menu.insert_menu_separator (menu.end ());
   
   {
      opal::MenuItem & sub_menu = menu.insert_menu_item (menu.end ());
      
      sub_menu.set_title ("New");
      
      {
         opal::MenuItem & menu_item = sub_menu.insert_menu_item (sub_menu.end ());
         
         menu_item.set_title ("New Taxi");
         menu_item.set_identifier (Constants::_menu_edit_item_id_new_taxi_0);
         menu_item.set_keystroke (opal::Key::Command + opal::Key (opal::KeyChar_N));
         menu_item.bind <
            MenuManager, &MenuManager::req_project_new_taxi
         > (*this);
      }

      {
         opal::MenuItem & menu_item = sub_menu.insert_menu_item (sub_menu.end ());
         
         menu_item.set_title ("New Pedestrian");
         menu_item.set_identifier (Constants::_menu_edit_item_id_new_pedestrian_0);
         menu_item.set_keystroke (opal::Key::Command + opal::Key::Alt + opal::Key (opal::KeyChar_N));
         menu_item.bind <
            MenuManager, &MenuManager::req_project_new_pedestrian
         > (*this);
      }
   }

   menu.insert_menu_separator (menu.end ());

   {
      opal::MenuItem & sub_menu = menu.insert_menu_item (menu.end ());
      
      sub_menu.set_title ("Color");
      
      {
         opal::MenuItem & menu_item = sub_menu.insert_menu_item (sub_menu.end ());
         
         menu_item.set_title ("White");
         menu_item.set_identifier (Constants::_menu_edit_item_id_color_white_0);
         menu_item.set_keystroke (opal::Key (opal::KeyCode_1));
         menu_item.bind <
            MenuManager, &MenuManager::req_project_color
         > (*this);
      }

      {
         opal::MenuItem & menu_item = sub_menu.insert_menu_item (sub_menu.end ());
         
         menu_item.set_title ("Black");
         menu_item.set_identifier (Constants::_menu_edit_item_id_color_black_0);
         menu_item.set_keystroke (opal::Key (opal::KeyCode_2));
         menu_item.bind <
            MenuManager, &MenuManager::req_project_color
         > (*this);
      }

      {
         opal::MenuItem & menu_item = sub_menu.insert_menu_item (sub_menu.end ());
         
         menu_item.set_title ("Yellow");
         menu_item.set_identifier (Constants::_menu_edit_item_id_color_yellow_0);
         menu_item.set_keystroke (opal::Key (opal::KeyCode_3));
         menu_item.bind <
            MenuManager, &MenuManager::req_project_color
         > (*this);
      }

      {
         opal::MenuItem & menu_item = sub_menu.insert_menu_item (sub_menu.end ());
         
         menu_item.set_title ("Blue");
         menu_item.set_identifier (Constants::_menu_edit_item_id_color_blue_0);
         menu_item.set_keystroke (opal::Key (opal::KeyCode_4));
         menu_item.bind <
            MenuManager, &MenuManager::req_project_color
         > (*this);
      }

      {
         opal::MenuItem & menu_item = sub_menu.insert_menu_item (sub_menu.end ());
         
         menu_item.set_title ("Green");
         menu_item.set_identifier (Constants::_menu_edit_item_id_color_green_0);
         menu_item.set_keystroke (opal::Key (opal::KeyCode_5));
         menu_item.bind <
            MenuManager, &MenuManager::req_project_color
         > (*this);
      }
   }
}



/*
==============================================================================
Name : update_menu
==============================================================================
*/

void  MenuManager::update_menu ()
{
   using namespace ohm;
   
   opal::MenuRoot & menu_root = opal::MenuBar::use_instance ().use_root ();
   assert (&menu_root != 0);
   
   update_menu_file (menu_root);
   update_menu_edit (menu_root);

   opal::MenuBar::use_instance ().update ();
}



/*
==============================================================================
Name : update_menu_file
==============================================================================
*/

void  MenuManager::update_menu_file (ohm::opal::MenuRoot & menu_root)
{
}



/*
==============================================================================
Name : update_menu_edit
==============================================================================
*/

void  MenuManager::update_menu_edit (ohm::opal::MenuRoot & menu_root)
{
   using namespace ohm;
   
   menu_root.enable (client::Constants::_menu_edit_item_id_undo_0, true);
   menu_root.enable (client::Constants::_menu_edit_item_id_redo_0, true);
   menu_root.enable (client::Constants::_menu_edit_item_id_cut_0, true);
   menu_root.enable (client::Constants::_menu_edit_item_id_copy_0, true);
   menu_root.enable (client::Constants::_menu_edit_item_id_paste_0, true);
   menu_root.enable (client::Constants::_menu_edit_item_id_delete_0, true);
   menu_root.enable (client::Constants::_menu_edit_item_id_select_all_0, true);
}



/*
==============================================================================
Name : req_exit
==============================================================================
*/

void  MenuManager::req_exit (ohm::opal::MenuEvent & event)
{
   using namespace ohm;
   
   task::RunLoopGui::use_instance ().quit ();
}



/*
==============================================================================
Name : req_edit_undo
==============================================================================
*/

void  MenuManager::req_edit_undo (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;
   
   model::ClientData & client_data = model::ClientData::use_self (root);
   
   std::string name;
   client_data.undo (name);
}



/*
==============================================================================
Name : req_edit_redo
==============================================================================
*/

void  MenuManager::req_edit_redo (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;
   
   model::ClientData & client_data = model::ClientData::use_self (root);
   
   std::string name;
   client_data.redo (name);
}



/*
==============================================================================
Name : req_edit_cut
==============================================================================
*/

void  MenuManager::req_edit_cut (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;
   
   model::ClientData & client_data = model::ClientData::use_self (root);
   
   client_data.copy_selection ();
   client_data.tx_delete_selection ();
}



/*
==============================================================================
Name : req_edit_copy
==============================================================================
*/

void  MenuManager::req_edit_copy (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;
   
   model::ClientData & client_data = model::ClientData::use_self (root);
   
   client_data.copy_selection ();
}



/*
==============================================================================
Name : req_edit_paste
==============================================================================
*/

void  MenuManager::req_edit_paste (ohm::opal::MenuEvent & event)
{
   using namespace ohm;
   
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;
   model::ClientData & client_data = model::ClientData::use_self (root);
   
   client_data.tx_paste ();
}



/*
==============================================================================
Name : req_edit_select_all
==============================================================================
*/

void  MenuManager::req_edit_select_all (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;
   
   model::ClientData & client_data = model::ClientData::use_self (root);
   
   client_data.tx_select_all ();
}



/*
==============================================================================
Name : req_project_new_taxi
==============================================================================
*/

void  MenuManager::req_project_new_taxi (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;

   root.tx_add_taxi (100, 100, model::Taxi::_min_width, model::Taxi::Color_YELLOW);
}



/*
==============================================================================
Name : req_project_new_pedestrian
==============================================================================
*/

void  MenuManager::req_project_new_pedestrian (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   model::Root & root = *_root_ptr;

   root.tx_add_pedestrian (100, 100);
}



/*
==============================================================================
Name : req_project_color
==============================================================================
*/

void  MenuManager::req_project_color (ohm::opal::MenuEvent & event)
{
   if (_root_ptr == 0) return;
   
   const std::string & identifier = event._menu_item_ptr->get_identifier ();
   
   model::Root & root = *_root_ptr;
   
   model::ClientData & client_data = model::ClientData::use_self (root);
   model::Selection & selection = client_data.use_selection ();
   
   selection.start_session ();
   
   if (identifier == Constants::_menu_edit_item_id_color_white_0)
   {
      selection.session_taxi_set_color (model::Taxi::Color_WHITE);
   }
   else if (identifier == Constants::_menu_edit_item_id_color_black_0)
   {
      selection.session_taxi_set_color (model::Taxi::Color_BLACK);
   }
   else if (identifier == Constants::_menu_edit_item_id_color_yellow_0)
   {
      selection.session_taxi_set_color (model::Taxi::Color_YELLOW);
   }
   else if (identifier == Constants::_menu_edit_item_id_color_blue_0)
   {
      selection.session_taxi_set_color (model::Taxi::Color_BLUE);
   }
   else if (identifier == Constants::_menu_edit_item_id_color_green_0)
   {
      selection.session_taxi_set_color (model::Taxi::Color_GREEN);
   }
   
   selection.commit_session ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
