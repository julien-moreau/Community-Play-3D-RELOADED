/*****************************************************************************

        FrameDialogBlock.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58927

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

#include "ohm/opa/private/FrameDialogBlock.h"
#include "ohm/opa/WidgetDialog.h"

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

FrameDialogBlock::FrameDialogBlock (WidgetDialog & widget)
:  opal::Frame ("FrameDialogBlock")
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

FrameDialogBlock::~FrameDialogBlock ()
{
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

void  FrameDialogBlock::get_cnt (opal::EventTypeCounter & counter)
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

      counter.add_key_pressed (pass);
      counter.add_key_repeated (pass);
      counter.add_key_released (pass);
      counter.add_character_typed (pass);
   }
/* 
   counter.add_key_pressed (opal::Event::Pass_CRUMB);
   counter.add_key_repeated (opal::Event::Pass_CRUMB);
   counter.add_key_released (opal::Event::Pass_CRUMB);
   counter.add_character_typed (opal::Event::Pass_CRUMB);
*/ counter.add_keyboard_catch_focus ();

}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

void  FrameDialogBlock::dispatch (opal::Event & event)
{  
   event.deflect ();
   event.block ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
