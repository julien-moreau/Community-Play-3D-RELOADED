/*****************************************************************************

        EventDispatcherStandardKeyboard.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_opal_EventDispatcherStandardKeyboard_HEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardKeyboard_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcher.h"
#include "ohm/opal/Event.h"



namespace ohm
{
namespace opal
{



template <class T>
class EventDispatcherStandardKeyboard
:  public EventDispatcher
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (T::*Proc) (opal::Event & event);
   
                  EventDispatcherStandardKeyboard (T & object);
   virtual        ~EventDispatcherStandardKeyboard ();
   
   void           configure_key (Event::Pass pass, Proc key_pressed_proc);
   void           configure_key (Event::Pass pass, Proc key_pressed_proc, Proc key_released_proc);
   void           configure_key (Event::Pass pass, Proc key_pressed_proc, Proc key_repeated_proc, Proc key_released_proc);
   void           configure_characters (Event::Pass pass, Proc characters_typed_proc);
   void           configure_catch_focus (Proc keyboard_catch_focus_proc);

   // EventDispatcher
   virtual void   get_cnt (EventTypeCounter & counter);
   virtual void   dispatch (Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           call_key_pressed (Event & event);
   void           call_key_repeated (Event & event);
   void           call_key_released (Event & event);
   void           call_characters_typed (Event & event);
   void           call_keyboard_catch_focus (Event & event);

   T &            _object;
   
   Proc           _key_pressed_proc_arr [Event::Pass_NBR_ELT];
   Proc           _key_repeated_proc_arr [Event::Pass_NBR_ELT];
   Proc           _key_released_proc_arr [Event::Pass_NBR_ELT];
   Proc           _characters_typed_proc_arr [Event::Pass_NBR_ELT];
   Proc           _keyboard_catch_focus_proc;
   



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventDispatcherStandardKeyboard ();
                  EventDispatcherStandardKeyboard (const EventDispatcherStandardKeyboard &other);
   EventDispatcherStandardKeyboard &
                  operator = (const EventDispatcherStandardKeyboard &other);
   bool           operator == (const EventDispatcherStandardKeyboard &other);
   bool           operator != (const EventDispatcherStandardKeyboard &other);

}; // class EventDispatcherStandardKeyboard



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcherStandardKeyboard.hpp"



#endif   // ohm_opal_EventDispatcherStandardKeyboard_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
