/*****************************************************************************

        TrackerTaxiWidth.cpp
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

#include "client/TrackerTaxiWidth.h"

#include "client/BundleTaxi.h"
#include "client/Cursor.h"
#include "client/FrameTaxi.h"

#include "model/ClientData.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerTaxiWidth::TrackerTaxiWidth (BundleTaxi & bundle, FrameTaxi & frame)
:  ohm::opal::TrackerBase (frame)
,  _bundle (bundle)
,  _frame (frame)
{
   enable_anti_bad_move ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerTaxiWidth::~TrackerTaxiWidth ()
{
   try
   {
      stop_revert ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : do_start
==============================================================================
*/

bool  TrackerTaxiWidth::do_start (ohm::opal::Event & event)
{
   using namespace ohm;
   
   bool ok_flag = _bundle.req_start ();
   
   if (ok_flag)
   {
      _frame.acquire_keyboard_focus ();
      
      update_cursor (event);
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : do_track
==============================================================================
*/

void  TrackerTaxiWidth::do_track (ohm::opal::Event & event)
{
   using namespace ohm;
   
   opa::Point mouse_location = event._mouse._location;
   
   float width = mouse_location._x;
   
   _bundle.req_change_width (width);
   
   update_cursor (event);
}



/*
==============================================================================
Name : do_commit
==============================================================================
*/

void  TrackerTaxiWidth::do_commit (ohm::opal::Event & event)
{
   _frame.release_keyboard_focus ();
   
   _bundle.req_commit (false);
}



/*
==============================================================================
Name : do_revert
==============================================================================
*/

void  TrackerTaxiWidth::do_revert ()
{
   _frame.release_keyboard_focus ();
   
   _bundle.req_revert ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_cursor
==============================================================================
*/

void  TrackerTaxiWidth::update_cursor (ohm::opal::Event & event)
{
   Cursor::use ().resize ().right ().set ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
