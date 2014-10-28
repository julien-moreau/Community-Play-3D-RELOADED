/*****************************************************************************

        FrameComboBox.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 50723

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

#include "ohm/opa/frame/FrameComboBox.h"
#include "ohm/opa/WidgetComboBox.h"


#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: FrameComboBox
==============================================================================
*/

FrameComboBox::FrameComboBox (WidgetComboBox & widget)
:  opal::Frame ("FrameComboBox")
,  _widget (widget)
,  _dispatcher (*this)
{  
   _dispatcher._mouse.configure_mouse_button_pressed (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameComboBox>::Button_LEFT, 
      opal::EventDispatcherStandardMouse <FrameComboBox>::ClickCount_ONE, 
      &FrameComboBox::mouse_button_pressed
   );

   add (_dispatcher);
}



/*
==============================================================================
Name: ~FrameComboBox
==============================================================================
*/

FrameComboBox::~FrameComboBox ()
{
}



/*
==============================================================================
Name: mouse_button_pressed
==============================================================================
*/

void FrameComboBox::mouse_button_pressed (opal::Event & event)
{
   assert (&event != 0);
   
   _widget.req_open_menu (event);
   
   event.block ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


   
}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

