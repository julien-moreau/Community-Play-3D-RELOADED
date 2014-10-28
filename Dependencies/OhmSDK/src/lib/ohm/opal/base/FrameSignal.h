/*****************************************************************************

        FrameSignal.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71263

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



#if ! defined (ohm_opal_FrameSignal_HEADER_INCLUDED)
#define  ohm_opal_FrameSignal_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/def.h"
#include "ohm/opal/base/EventDispatcherStandard.h"
#include "ohm/util/Delegate.h"



namespace ohm
{
namespace opal
{



class FrameSignal
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class Event
   {
   public:
                  Event (FrameSignal & frame);
      
      FrameSignal &
                  _frame;
      opal::Event *
                  _event_ptr;
      bool        _gesture_flag;
      bool        _in_flag;
      double      _gesture_start_time;
      double      _previous_time;
      double      _time;
   
   private:
                  Event ();
   };
   
                  FrameSignal ();
   virtual        ~FrameSignal ();
   
   template <class T, void (T::*rollover_begin) (Event &), void (T::*rollover_end) (Event &)>
   void           bind_rollover (T & receiver);
   template <class T, void (T::*gesture_begin) (Event &), void (T::*gesture_end) (Event &)>
   void           bind_gesture (T & receiver);
   template <class T, void (T::*gesture_entered) (Event &), void (T::*gesture_exited) (Event &)>
   void           bind_gesture_trip (T & receiver);
   template <class T, void (T::*gesture_signal) (Event &)>
   void           bind_gesture_signal (T & receiver);
   template <class T, void (T::*gesture_timer) (Event &)>
   void           bind_gesture_timer (T & receiver);

   template <class T, void (T::*hit_test) (Event &)>
   void           bind_hit_test (T & receiver);
   
   void           set_signal_on_pressed (bool signal_on_pressed_flag);
   bool           is_signal_on_pressed () const;

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)
   void           set_cursor_on_rollover (const std::string & path, const opa::Point & hot_point);
   void           set_cursor_on_rollover (const std::pair <std::string, opa::Point> & cursor);
   void           set_cursor_on_rollover (Application::StdCursor cursor);
#endif

   // Frame interface
   virtual void   mouse_entered (opal::Event & event);
   virtual void   mouse_exited (opal::Event & event);

   // delegates
   void           mouse_button_pressed (opal::Event & event);
   void           mouse_button_released (opal::Event & event);
   void           mouse_dragged (opal::Event & event);
   void           mouse_cursor (opal::Event & event);

   void           user_input_lost (opal::Event & event);

   void           timer_fired (opal::Event & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           mouse_handle (bool in_flag);
   
   void           hit_test_default (Event & event);
   
   opal::EventDispatcherStandard <FrameSignal>
                  _dispatcher;
   
   Event          _event;
   
   bool           _signal_on_pressed_flag;

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)   
   std::string    _cursor_path;
   opa::Point     _cursor_hot_point;
   Application::StdCursor
                  _cursor_std;
#endif

   util::Delegate _rollover_begin;
   util::Delegate _rollover_end;

   util::Delegate _gesture_begin;
   util::Delegate _gesture_end;

   util::Delegate _gesture_entered;
   util::Delegate _gesture_exited;

   util::Delegate _gesture_signal;
   util::Delegate _gesture_timer;

   util::Delegate _hit_test;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameSignal (const FrameSignal &other);
   FrameSignal &  operator = (const FrameSignal &other);
   bool           operator == (const FrameSignal &other);
   bool           operator != (const FrameSignal &other);

}; // class FrameSignal



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opal/base/FrameSignal.hpp"



#endif   // ohm_opal_FrameSignal_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
