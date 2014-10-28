/*****************************************************************************

        FrameDialogBack.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71208

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

#include "ohm/opal/def.h"

#include "ohm/opa/private/FrameDialogBack.h"
#include "ohm/opa/WidgetDialog.h"
#include "ohm/opal/Application.h"

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

FrameDialogBack::FrameDialogBack (WidgetDialog & widget)
:  opal::Frame ("FrameDialogBack")
,  _widget (widget)
{
   opal::EventDispatcher & dispatcher = *this;
   
   add (dispatcher);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameDialogBack::~FrameDialogBack ()
{
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

void  FrameDialogBack::get_cnt (opal::EventTypeCounter & counter)
{
   for (size_t i = opal::Event::Pass_FOCUS ; i <= opal::Event::Pass_SPATIAL ; ++i)
   {
      opal::Event::Pass pass = static_cast <opal::Event::Pass> (i);
      
      counter.add_mouse_moved (pass, 0);
      counter.add_mouse_moved (pass, 1);
      counter.add_mouse_moved (pass, 2);

      counter.add_mouse_button_state_changed (pass, 0, 1);
      counter.add_mouse_button_state_changed (pass, 1, 1);
      counter.add_mouse_button_state_changed (pass, 2, 1);
      counter.add_mouse_button_state_changed (pass, 0, 2);
      counter.add_mouse_button_state_changed (pass, 1, 2);
      counter.add_mouse_button_state_changed (pass, 2, 2);

      counter.add_mouse_wheel_moved (pass);

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)      
      counter.add_mouse_cursor ();
#endif
   }
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

void  FrameDialogBack::dispatch (opal::Event & event)
{
   if (event._type == opal::Event::Type_MOUSE_BUTTON_PRESSED)
   {
      _widget.shake ();
   }
#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   else if (event._type == opal::Event::Type_MOUSE_CURSOR)
   {
      opal::Application::use_instance ().set_cursor_arrow ();
   }
#endif
   
   event.block ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
