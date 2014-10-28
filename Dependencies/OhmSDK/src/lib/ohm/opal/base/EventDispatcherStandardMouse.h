/*****************************************************************************

        EventDispatcherStandardMouse.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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



#if ! defined (ohm_opal_EventDispatcherStandardMouse_HEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardMouse_HEADER_INCLUDED

#if defined (_MSC_VER)
   #pragma once
   #pragma warning (4 : 4250) // "Inherits via dominance."
   #pragma warning (disable : 4351) // new behavior: elements of array 'name' will be default initialized
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcher.h"



namespace ohm
{
namespace opal
{



template <class T>
class EventDispatcherStandardMouse
:  public EventDispatcher
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum  ClickCount
   {
                  ClickCount_ONE = 1,
                  ClickCount_TWO,
                  ClickCount_THREE,
                  
                  ClickCount_NBR_ELT = ClickCount_THREE
   };
   
   enum  Button
   {
                  Button_LEFT = 1,
                  Button_RIGHT,
                  
                  Button_NBR_ELT = Button_RIGHT
   };

   typedef void (T::*Proc) (opal::Event & event);
   
                  EventDispatcherStandardMouse (T & object);
   virtual        ~EventDispatcherStandardMouse ();
   
   void           configure_extend_click_count (Button button, ClickCount count);
   void           configure_gesture (Button button, Proc mouse_button_pressed_proc, Proc mouse_button_released_proc, Proc mouse_dragged_proc);
   void           configure_gesture (Button button, ClickCount count, Proc mouse_button_pressed_proc, Proc mouse_button_released_proc, Proc mouse_dragged_proc);
   void           configure_mouse_button_pressed (Event::Pass pass, Button button, ClickCount count, Proc mouse_button_pressed_proc);
   void           configure_mouse_button_released (Event::Pass pass, Button button, ClickCount count, Proc mouse_button_released_proc);
   void           configure_mouse_dragged (Event::Pass pass, Button button, ClickCount count, Proc mouse_dragged_proc);
   void           configure_mouse_moved (Proc mouse_moved_proc);
   void           configure_mouse_moved (Event::Pass pass, Proc mouse_moved_proc);
   void           configure_wheel (Proc mouse_wheel_moved_proc);
   void           configure_wheel (Event::Pass pass, Proc mouse_wheel_moved_proc);
   void           configure_catch_capture (Proc mouse_catch_capture_proc);
   void           configure_cursor (Proc mouse_cursor_proc);

   // EventDispatcher
   virtual void   get_cnt (EventTypeCounter & counter);
   virtual void   dispatch (Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         Pass_NBR_ELT = 2  };
   
   void           call_pressed (Event & event);
   void           call_released (Event & event);
   void           call_dragged (Event & event);
   void           call_moved (Event & event);
   void           call_wheel_moved (Event & event);
   
   T &            _object;
   int            _max_nbr_click_arr [Button_NBR_ELT];
   
   Proc           _mouse_button_pressed_proc_arr [Pass_NBR_ELT][Button_NBR_ELT][ClickCount_NBR_ELT];
   Proc           _mouse_button_released_proc_arr [Pass_NBR_ELT][Button_NBR_ELT][ClickCount_NBR_ELT];
   Proc           _mouse_dragged_proc_arr [Pass_NBR_ELT][Button_NBR_ELT][ClickCount_NBR_ELT];
   Proc           _mouse_moved_proc_arr [Pass_NBR_ELT];
   Proc           _mouse_wheel_moved_proc_arr [Pass_NBR_ELT];
   Proc           _mouse_catch_capture_proc;
   Proc           _mouse_cursor_proc;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventDispatcherStandardMouse ();
                  EventDispatcherStandardMouse (const EventDispatcherStandardMouse &other);
   EventDispatcherStandardMouse &
                  operator = (const EventDispatcherStandardMouse &other);
   bool           operator == (const EventDispatcherStandardMouse &other);
   bool           operator != (const EventDispatcherStandardMouse &other);

}; // class EventDispatcherStandardMouse



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcherStandardMouse.hpp"



#endif   // ohm_opal_EventDispatcherStandardMouse_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
