/*****************************************************************************

        TrackerRootHand.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71268

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

#include "client/TrackerRootHand.h"

#include "client/Conduct.h"
#include "client/Cursor.h"
#include "client/BundleRoot.h"
#include "client/FrameRoot.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerRootHand::TrackerRootHand (BundleRoot & bundle, FrameRoot & frame)
:  ohm::opal::TrackerBase (frame)
,  _bundle (bundle)
,  _frame (frame)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerRootHand::~TrackerRootHand ()
{
   stop_revert ();
}



/*
==============================================================================
Name: update_cursor
==============================================================================
*/

void  TrackerRootHand::update_cursor (bool pressed_flag)
{
   Cursor::use ().hand ().close (pressed_flag).set ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   
/*
==============================================================================
Name : do_start
==============================================================================
*/

bool  TrackerRootHand::do_start (ohm::opal::Event & event)
{
   bool ok_flag = _bundle.req_start ();
   
   if (ok_flag)
   {
      update_cursor (true);
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : do_track
==============================================================================
*/

void  TrackerRootHand::do_track (ohm::opal::Event & event)
{
   using namespace ohm;
   
   opa::Point delta = event._mouse._location_delta;
   
   delta._x = - delta._x;
   delta._y = - delta._y;
   
   _bundle.req_focus_delta (delta);
}



/*
==============================================================================
Name : do_commit
==============================================================================
*/

void  TrackerRootHand::do_commit (ohm::opal::Event & event)
{
   update_cursor (false);
         
   _bundle.req_commit ();
   
   Conduct * conduct_ptr = event.get <Conduct> ();
   
   if (conduct_ptr != 0)
   {
      conduct_ptr->_invalid_tool_flag = true;
      _frame.post_conduct_changed_event (*conduct_ptr);
   }
}



/*
==============================================================================
Name : do_revert
==============================================================================
*/

void  TrackerRootHand::do_revert ()
{
   update_cursor (false);
         
   _bundle.req_revert ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
