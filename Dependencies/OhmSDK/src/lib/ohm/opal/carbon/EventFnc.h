/*****************************************************************************

        EventFnc.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 40845

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



#if ! defined (ohm_opal_EventFnc_HEADER_INCLUDED)
#define  ohm_opal_EventFnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"
#include "ohm/opal/Key.h"

#include <string>
#include <map>



namespace ohm
{
namespace opal
{



class Window;

class EventFnc
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~EventFnc ();
   
   static EventFnc &
                  use_instance ();
   
   OSStatus       propagate_event_to_omfo_view (WindowRef window_ref, EventRef event_ref);
   HIViewRef      get_content_view_ref (WindowRef window_ref);
   ::Point        get_qd_mouse_location (EventRef event_ref);
   opa::Point     get_mouse_location (EventRef event_ref);
   opa::Point     get_mouse_window_location (EventRef event_ref);
   opa::Point     get_mouse_view_location (HIViewRef view_ref, EventRef event_ref);
   opa::Point     get_mouse_view_location (HIViewRef view_ref, const opa::Point & global_location);
   opa::Point     get_mouse_delta (EventRef event_ref);
   bool           has_key_modifiers_shift (EventRef event_ref);
   bool           has_key_modifiers_alt (EventRef event_ref);
   bool           has_key_modifiers_command (EventRef event_ref);
   int            get_mouse_button (EventRef event_ref);
   int            get_click_count (EventRef event_ref);
   opa::Point     get_wheel_delta (EventRef event_ref);
   bool           text_input_event_is_modifiers_changed (EventRef event_ref);
   char           get_ascii_for_scan_code (::UInt32 scan_code);
   void           print_key (EventRef event_ref);
   archi::UInt32  get_key_modifiers (EventRef event_ref);
   std::string    get_characters (EventRef event_ref);
   ControlPartCode
                  get_control_part_code  (EventRef event_ref);
   DragRef        get_drag_ref (EventRef event_ref);

   void           attach_monitor (EventTargetRef event_target_ref);
   void           attach_monitor (OSType event_class, EventTargetRef event_target_ref);
   
   static void    print_debug (EventRef event_ref);

   static OSStatus
                  handle_event_proc (EventHandlerCallRef handler_call_ref, EventRef event_ref, void * user_data);
   
   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  EventFnc ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   





/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventFnc (const EventFnc &other);
   EventFnc &     operator = (const EventFnc &other);
   bool           operator == (const EventFnc &other);
   bool           operator != (const EventFnc &other);

}; // class EventFnc



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_EventFnc_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
