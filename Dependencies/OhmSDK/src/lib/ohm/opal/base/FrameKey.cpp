/*****************************************************************************

        FrameKey.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 50611

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

#include "ohm/opal/base/FrameKey.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Event::ctor
==============================================================================
*/

FrameKey::Event::Event (FrameKey & frame)
:  _frame (frame)
,  _event_ptr (0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

FrameKey::FrameKey ()
:  opal::Frame ("FrameKey")
,  _dispatcher (*this)
,  _event (*this)
{
   _dispatcher._keyboard.configure_key (
      opal::Event::Pass_CRUMB,
      &FrameKey::key_pressed
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameKey::~FrameKey ()
{
}



/*
==============================================================================
Name: set_spatial
==============================================================================
*/

void FrameKey::set_spatial ()
{
   remove (_dispatcher);
   
   _dispatcher._keyboard.configure_key (
      opal::Event::Pass_SPATIAL,
      &FrameKey::key_pressed
   );
   
   add (_dispatcher);
}



/*
==============================================================================
Name : add_key
==============================================================================
*/

void  FrameKey::add_key (const opal::Key & key)
{
   _key_set.insert (key);
}



/*
==============================================================================
Name : clear_keys
==============================================================================
*/

void  FrameKey::clear_keys ()
{
   _key_set.clear ();
}



/*
==============================================================================
Name : key_pressed
==============================================================================
*/

void FrameKey::key_pressed (opal::Event & event)
{
   const opal::Key & key = event._key;
   
   if (_key_set.find (key) != _key_set.end ())
   {
      _event._event_ptr = &event;
      
      _delegate.param_ref_signal (_event);
      
      event.block ();
      event.skip_passes ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
