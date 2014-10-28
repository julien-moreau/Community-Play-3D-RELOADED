/*****************************************************************************

        SystemEvent.cpp
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 70451

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

#include "ohm/opal/private/SystemEvent.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: SystemEvent
==============================================================================
*/

SystemEvent::SystemEvent ()
:  _mouse_location (opa::Point::_zero)
/*,   _click_cnt_arr ()*/
,  _key ()
,  _key_change (KeyChange_NONE)
,  _char_utf8 ()
,  _wheel_delta (opa::Point::_zero)
,  _layout_changed_flag (false)
,  _mouse_exited_view_flag (false)
,  _has_drag_data_flag (false)
,  _drag_data_dropped_flag (false)
,  _keyboard_focus_lost_flag (false)
{
   for (int i = 0 ; i < MouseButton_NBR_ELT ; ++i)
   {
      _click_cnt_arr [i] = 0;
   }
}



/*
==============================================================================
Name: ~SystemEvent
==============================================================================
*/

SystemEvent::~SystemEvent ()
{
}



/*
==============================================================================
Name: operator ==
==============================================================================
*/

bool SystemEvent::operator == (const SystemEvent &other) const
{
   return (
      (_mouse_location == other._mouse_location) &&
      (_click_cnt_arr [0] == other._click_cnt_arr [0]) && 
      (_click_cnt_arr [1] == other._click_cnt_arr [1]) && 
      (_key == other._key) && 
      (_key_change == other._key_change) && 
      (_char_utf8 == other._char_utf8) && 
      (_wheel_delta == other._wheel_delta) &&
      (_layout_changed_flag == other._layout_changed_flag) &&
      (_mouse_exited_view_flag == other._mouse_exited_view_flag) &&
      (_has_drag_data_flag == other._has_drag_data_flag) &&
      (_drag_data_dropped_flag == other._drag_data_dropped_flag) &&
      (_keyboard_focus_lost_flag == other._keyboard_focus_lost_flag) &&
      true
   );
}



/*
==============================================================================
Name: operator !=
==============================================================================
*/

bool SystemEvent::operator != (const SystemEvent &other) const
{
   return ! (operator == (other));
}



/*
==============================================================================
Name: reset_transient
==============================================================================
*/

void SystemEvent::reset_transient ()
{
   _layout_changed_flag = false;
   _mouse_exited_view_flag = false;
   _has_drag_data_flag = false;
   _drag_data_dropped_flag = false;
   _keyboard_focus_lost_flag = false;
   _key_change = KeyChange_NONE;
   _char_utf8.clear ();
   _wheel_delta = opa::Point::_zero;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

