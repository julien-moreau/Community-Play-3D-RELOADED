/*****************************************************************************

        Constants.h
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



#if ! defined (client_Constants_HEADER_INCLUDED)
#define  client_Constants_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace client
{



class Constants
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~Constants () {}
   
   // metrics
   
   static const float
                  _metric_taxi_height;
   static const float
                  _metric_pedestrian_width;
   static const float
                  _metric_pedestrian_height;
   
   static const char *
                  _cmd_arg_username_0;
   static const char *
                  _cmd_arg_document_path_0;

   static const double
                  _default_timeout;

   // File Menu
   static const char *
                  _menu_file_id_name_0;
   static const char *
                  _menu_file_item_id_exit_0;
                  
   // Edit Menu
   static const char *
                  _menu_edit_id_name_0;

   static const char *
                  _menu_edit_item_id_undo_0;
   static const char *
                  _menu_edit_item_id_redo_0;

   static const char *
                  _menu_edit_item_id_cut_0;
   static const char *
                  _menu_edit_item_id_copy_0;
   static const char *
                  _menu_edit_item_id_paste_0;
   static const char *
                  _menu_edit_item_id_delete_0;
   static const char *
                  _menu_edit_item_id_select_all_0;
   
   static const char *
                  _menu_edit_item_id_new_taxi_0;
   static const char *
                  _menu_edit_item_id_new_pedestrian_0;

   static const char *
                  _menu_edit_item_id_color_white_0;
   static const char *
                  _menu_edit_item_id_color_black_0;
   static const char *
                  _menu_edit_item_id_color_yellow_0;
   static const char *
                  _menu_edit_item_id_color_blue_0;
   static const char *
                  _menu_edit_item_id_color_green_0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Constants ();
                  Constants (const Constants &other);
   Constants &    operator = (const Constants &other);
   bool           operator == (const Constants &other);
   bool           operator != (const Constants &other);

}; // class Constants



}  // namespace client



//#include  "client/Constants.hpp"



#endif   // client_Constants_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

