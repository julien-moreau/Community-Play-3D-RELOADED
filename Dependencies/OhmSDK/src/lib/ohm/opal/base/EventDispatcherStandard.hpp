/*****************************************************************************

        EventDispatcherStandard.hpp
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



#if defined (ohm_opal_EventDispatcherStandard_CURRENT_CODEHEADER)
   #error Recursive inclusion of EventDispatcherStandard code header.
#endif
#define  ohm_opal_EventDispatcherStandard_CURRENT_CODEHEADER

#if ! defined (ohm_opal_EventDispatcherStandard_CODEHEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandard_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Event.h"



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
EventDispatcherStandard <T>::EventDispatcherStandard (T & object)
:  _object (object)
,  _mouse (object)
,  _keyboard (object)
,  _user_input (object)
,  _conduct (object)
,  _custom (object)
,  _data (object)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
EventDispatcherStandard <T>::~EventDispatcherStandard ()
{
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

template <class T>
void  EventDispatcherStandard <T>::get_cnt (EventTypeCounter & counter)
{
   _mouse.get_cnt (counter);
   _keyboard.get_cnt (counter);
   _user_input.get_cnt (counter);
   _conduct.get_cnt (counter);
   _custom.get_cnt (counter);
   _data.get_cnt (counter);
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

template <class T>
void  EventDispatcherStandard <T>::dispatch (Event & event)
{
   switch (event._type)
   {
   case Event::Type_MOUSE_MOVED:
   case Event::Type_MOUSE_BUTTON_PRESSED:
   case Event::Type_MOUSE_BUTTON_RELEASED:
   case Event::Type_MOUSE_WHEEL_MOVED:
   case Event::Type_MOUSE_CATCH_CAPTURE:
   case Event::Type_MOUSE_CURSOR:
      _mouse.dispatch (event);
      break;

   case Event::Type_KEY_PRESSED:
   case Event::Type_KEY_REPEATED:
   case Event::Type_KEY_RELEASED:
   case Event::Type_CHARACTERS_TYPED:
   case Event::Type_KEYBOARD_CATCH_FOCUS:
      _keyboard.dispatch (event);
      break;

   case Event::Type_USER_INPUT_LOST:
      _user_input.dispatch (event);
      break;
   
   case Event::Type_CONDUCT_CHANGED:
      _conduct.dispatch (event);
      break;
   
   case Event::Type_CUSTOM_SENT:
      _custom.dispatch (event);
      break;
   
   case Event::Type_DATA_DROPPED:
      _data.dispatch (event);
      break;
   
   default:
      assert (false);
      break;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_EventDispatcherStandard_CODEHEADER_INCLUDED

#undef ohm_opal_EventDispatcherStandard_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
