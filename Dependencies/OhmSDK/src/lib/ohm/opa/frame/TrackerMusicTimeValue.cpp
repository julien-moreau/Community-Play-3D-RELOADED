/*****************************************************************************

        TrackerMusicTimeValue.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66675

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

#include "ohm/opa/frame/TrackerMusicTimeValue.h"
#include "ohm/opa/frame/FrameMusicTime.h"
#include "ohm/opa/WidgetMusicTime.h"

#include "ohm/opal/Frame.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerMusicTimeValue::TrackerMusicTimeValue (WidgetMusicTime & widget, FrameMusicTime & frame, int field)
:  TrackerBase (frame)
,  _widget (widget)
,  _frame (frame)
,  _field (field)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerMusicTimeValue::~TrackerMusicTimeValue ()
{
   stop_revert ();
}



/*
==============================================================================
Name : do_start
==============================================================================
*/

bool  TrackerMusicTimeValue::do_start (opal::Event & event)
{
   bool ok_flag = _widget.req_start ();
   
   return ok_flag;
}



/*
==============================================================================
Name : do_track
==============================================================================
*/

void  TrackerMusicTimeValue::do_track (opal::Event & event)
{
   opa::Point mouse_location = event._mouse._location;
   opa::Point click_mouse_location = event._mouse._click_location;
   
   float delta_y = mouse_location._y - click_mouse_location._y;
   
   _widget.req_change_value (_field, - delta_y);
}



/*
==============================================================================
Name : do_commit
==============================================================================
*/

void  TrackerMusicTimeValue::do_commit (opal::Event & event)
{
   _widget.req_commit ();
}



/*
==============================================================================
Name : do_revert
==============================================================================
*/

void  TrackerMusicTimeValue::do_revert ()
{
   _widget.req_revert ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
