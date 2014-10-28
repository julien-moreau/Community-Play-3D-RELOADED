/*****************************************************************************

        FrameWidgetMenuClose.cpp
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 49822

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

#include "ohm/opa/private/FrameWidgetMenuClose.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opal/base/EventDispatcherStandard.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: FrameWidgetMenuClose
==============================================================================
*/

FrameWidgetMenuClose::FrameWidgetMenuClose (BundleView & bundle, WidgetMenuRoot & menu_root)
:  _bundle (bundle)
,  _menu_root (menu_root)
,  _dispatcher (*this)
,  _dispatcher_block ()
{
   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameWidgetMenuClose>::Button_LEFT, 
      opal::EventDispatcherStandardMouse <FrameWidgetMenuClose>::ClickCount_ONE, 
      &FrameWidgetMenuClose::mouse_button_released
   );

   _dispatcher._mouse.configure_mouse_button_released (
      opal::Event::Pass_SPATIAL,
      opal::EventDispatcherStandardMouse <FrameWidgetMenuClose>::Button_RIGHT, 
      opal::EventDispatcherStandardMouse <FrameWidgetMenuClose>::ClickCount_ONE, 
      &FrameWidgetMenuClose::mouse_button_released
   );

   _dispatcher._user_input.configure_lost (
      &FrameWidgetMenuClose::user_input_lost
   );
   
   add (_dispatcher);
   add (_dispatcher_block);
}



/*
==============================================================================
Name: ~FrameWidgetMenuClose
==============================================================================
*/

FrameWidgetMenuClose::~FrameWidgetMenuClose ()
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: mouse_button_released
==============================================================================
*/

void  FrameWidgetMenuClose::mouse_button_released (opal::Event & event)
{
   if (_menu_root.can_select ())
   {
      _bundle.req_release_popup_menu ();
   }
   
   event.block ();
}



/*
==============================================================================
Name : user_input_lost
==============================================================================
*/

void  FrameWidgetMenuClose::user_input_lost (opal::Event & event)
{
   _bundle.req_release_popup_menu ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

