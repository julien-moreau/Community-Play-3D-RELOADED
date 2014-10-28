/*****************************************************************************

        TrackerTaxiPosition.cpp
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

#include "client/TrackerTaxiPosition.h"

#include "client/BundleTaxi.h"
#include "client/Cursor.h"
#include "client/FrameTaxi.h"

#include "ohm/flip/DocumentClient.h"
#include "ohm/task/RunLoopGui.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

TrackerTaxiPosition::TrackerTaxiPosition (BundleTaxi & bundle, FrameTaxi & frame)
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

TrackerTaxiPosition::~TrackerTaxiPosition ()
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

bool  TrackerTaxiPosition::do_start (ohm::opal::Event & event)
{
   using namespace ohm;
   
   bool ok_flag = _bundle.req_start ();
   
   if (ok_flag)
   {
      _copy_flag = event._key.has_alt_modifier ();
      
      _bundle.req_change_copy_mode (_copy_flag);

      _frame.acquire_keyboard_focus ();
      
      opa::Point mouse_location = event._mouse._location;
      
      _rel_origin = mouse_location;
      
      update_cursor (event);
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : do_track
==============================================================================
*/

void  TrackerTaxiPosition::do_track (ohm::opal::Event & event)
{
   using namespace ohm;
   
   opa::Point mouse_location = event._mouse._location;
   opa::Point new_location = mouse_location + _frame.get_origin () - _rel_origin;
   
   _bundle.req_change_position (new_location._x);
}



/*
==============================================================================
Name : do_key_changed
==============================================================================
*/

void  TrackerTaxiPosition::do_key_changed (ohm::opal::Event & event)
{
   using namespace ohm;
   
   const bool copy_flag = event._key.has_alt_modifier ();
   
   if (_copy_flag != copy_flag)
   {
      _copy_flag = copy_flag;
      _bundle.req_change_copy_mode (copy_flag);
   }

   update_cursor (event);
}



/*
==============================================================================
Name : do_commit
==============================================================================
*/

void  TrackerTaxiPosition::do_commit (ohm::opal::Event & event)
{
   using namespace ohm;
   
   _frame.release_keyboard_focus ();
         
   bool copy_flag = event._key.has_alt_modifier ();
   
   _bundle.req_commit (copy_flag);
}



/*
==============================================================================
Name : do_revert
==============================================================================
*/

void  TrackerTaxiPosition::do_revert ()
{
   using namespace ohm;
   
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

void  TrackerTaxiPosition::update_cursor (ohm::opal::Event & event)
{
   bool copy_flag = event._key.has_alt_modifier ();
   
   Cursor::use ().arrow ().move ().copy (copy_flag).set ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
