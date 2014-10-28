/*****************************************************************************

        EventTypeCounter.h
        Copyright (c) 2010 Ohm Force

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



#if ! defined (ohm_opal_EventTypeCounter_HEADER_INCLUDED)
#define  ohm_opal_EventTypeCounter_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Event.h"



namespace ohm
{
namespace opal
{



class Event;

class EventTypeCounter
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  EventTypeCounter ();
   virtual        ~EventTypeCounter ();
   
   bool           empty () const;
   bool           does_support (const Event & event) const;
   
   void           increment (const EventTypeCounter & other);
   void           decrement (const EventTypeCounter & other);
   
   void           add_all ();
   
   void           add_mouse_moved (Event::Pass pass, int button_nbr);
   void           add_mouse_button_state_changed (Event::Pass pass, int button_nbr, int click_count);
   void           add_mouse_button_state_changed (Event::Pass pass, int button_nbr);
   void           add_mouse_wheel_moved (Event::Pass pass);
   void           add_mouse_catch_capture ();
   void           add_mouse_cursor ();
   
   void           add_key_pressed (Event::Pass pass);
   void           add_key_repeated (Event::Pass pass);
   void           add_key_released (Event::Pass pass);
   void           add_character_typed (Event::Pass pass);
   void           add_keyboard_catch_focus ();
   
   void           add_user_input_lost ();
   
   void           add_conduct_changed (Event::Pass pass);
   void           add_custom_sent (Event::Pass pass);

   void           add_data_dropped (Event::Pass pass);

   void           add_debug ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum Metrics
   {              
                  // button nbr :
                  // - 0 : mouse moved
                  // - 1 : mouse dragged for any button nbr
                  Metrics_MOVED_NBR_BUTTON = 2,
                  
                  // button nbr :
                  // - 0 : left button
                  // - 1 : all other buttons
                  // click count :
                  // - 0 : 1 click
                  // - 1 : 2 clicks & all other 2 clicks
                  Metrics_BUT_CHD_NBR_BUTTON = 2,
                  Metrics_BUT_CHD_NBR_CLICK = 2,
                  
                  // pass nbr :
                  // - 0 : Pass_FOCUS & Pass_SPATIAL
                  // - 1 : Pass_CRUMB
                  Metrics_PASS_NBR = 2,
   };
   
   enum Base
   {
                  Base_MOUSE_MOVED = 0,
                  
                  Base_MOUSE_BUTTON_CHANGED
                     = Base_MOUSE_MOVED
                     + Metrics_PASS_NBR * Metrics_MOVED_NBR_BUTTON,

                  Base_MOUSE_WHEEL_MOVED
                     = Base_MOUSE_BUTTON_CHANGED
                     + Metrics_PASS_NBR
                        * Metrics_BUT_CHD_NBR_BUTTON
                        * Metrics_BUT_CHD_NBR_CLICK,
                  
                  Base_CATCH_CAPTURE
                     = Base_MOUSE_WHEEL_MOVED
                     + Metrics_PASS_NBR,
                  
                  Base_CURSOR,

                  Base_KEY_PRESSED,
                  
                  Base_KEY_REPEATED
                     = Base_KEY_PRESSED
                     + Metrics_PASS_NBR,

                  Base_KEY_RELEASED
                     = Base_KEY_REPEATED
                     + Metrics_PASS_NBR,

                  Base_CHARACTER_TYPED
                     = Base_KEY_RELEASED
                     + Metrics_PASS_NBR,

                  Base_KEYBOARD_CATCH_FOCUS
                     = Base_CHARACTER_TYPED
                     + Metrics_PASS_NBR,
                  
                  Base_USER_INPUT_LOST,
                  Base_CONDUCT_CHANGED,
                  Base_CUSTOM_SENT
                     = Base_CONDUCT_CHANGED
                     + Metrics_PASS_NBR,

                  Base_DATA_DROPPED
                     = Base_CUSTOM_SENT
                     + Metrics_PASS_NBR,
                  
                  Base_DEBUG
                     = Base_DATA_DROPPED
                     + Metrics_PASS_NBR,

                  Base_NBR_ELT
   };
   
   inline int     get_pass_plane (Event::Pass pass) const;
   inline int     get_button_nbr_plane (const Event & event) const;
   inline int     get_button_nbr_plane (int button_nbr) const;
   inline int     get_click_count_plane (int click_count) const;
   
   size_t         _type_cnt_arr [Base_NBR_ELT];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventTypeCounter (const EventTypeCounter &other);
   EventTypeCounter &
                  operator = (const EventTypeCounter &other);
   bool           operator == (const EventTypeCounter &other);
   bool           operator != (const EventTypeCounter &other);

}; // class EventTypeCounter



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opal/base/EventTypeCounter.hpp"



#endif   // ohm_opal_EventTypeCounter_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
