/*****************************************************************************

        Constants.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/Constants.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const float Constants::_metric_taxi_height = 66.f;
const float Constants::_metric_pedestrian_width = 41.f;
const float Constants::_metric_pedestrian_height = 55.f;

const char * Constants::_cmd_arg_username_0 = "username";
const char * Constants::_cmd_arg_document_path_0 = "document_path";

const double   Constants::_default_timeout = 10.0; // Default timeout in seconds for network operations.


const char * Constants::_menu_file_id_name_0 = "file";
const char * Constants::_menu_file_item_id_exit_0 = "file.exit";

const char * Constants::_menu_edit_id_name_0 = "edit";

const char * Constants::_menu_edit_item_id_undo_0 = "edit.undo";
const char * Constants::_menu_edit_item_id_redo_0 = "edit.redo";

const char * Constants::_menu_edit_item_id_cut_0 = "edit.cut";
const char * Constants::_menu_edit_item_id_copy_0 = "edit.copy";
const char * Constants::_menu_edit_item_id_paste_0 = "edit.paste";
const char * Constants::_menu_edit_item_id_delete_0 = "edit.delete";
const char * Constants::_menu_edit_item_id_select_all_0 = "edit.select_all";

const char * Constants::_menu_edit_item_id_new_taxi_0 = "edit.new.taxi";
const char * Constants::_menu_edit_item_id_new_pedestrian_0 = "edit.new.pedestrian";

const char * Constants::_menu_edit_item_id_color_white_0 = "edit.color.white";
const char * Constants::_menu_edit_item_id_color_black_0 = "edit.color.black";
const char * Constants::_menu_edit_item_id_color_yellow_0 = "edit.color.yellow";
const char * Constants::_menu_edit_item_id_color_blue_0 = "edit.color.blue";
const char * Constants::_menu_edit_item_id_color_green_0 = "edit.color.green";



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

