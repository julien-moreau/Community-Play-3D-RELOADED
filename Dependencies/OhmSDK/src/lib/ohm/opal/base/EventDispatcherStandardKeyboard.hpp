/*****************************************************************************

        EventDispatcherStandardKeyboard.hpp
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



#if defined (ohm_opal_EventDispatcherStandardKeyboard_CURRENT_CODEHEADER)
   #error Recursive inclusion of EventDispatcherStandardKeyboard code header.
#endif
#define  ohm_opal_EventDispatcherStandardKeyboard_CURRENT_CODEHEADER

#if ! defined (ohm_opal_EventDispatcherStandardKeyboard_CODEHEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardKeyboard_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



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
EventDispatcherStandardKeyboard <T>::EventDispatcherStandardKeyboard (T & object)
:  _object (object)

,  _key_pressed_proc_arr ()
,  _key_repeated_proc_arr ()
,  _key_released_proc_arr ()
,  _characters_typed_proc_arr ()

,  _keyboard_catch_focus_proc (0)
{
   assert (&object != 0);
   
   for (size_t i = 0 ; i < Event::Pass_NBR_ELT ; ++i)
   {
      _key_pressed_proc_arr [i] = 0;
      _key_repeated_proc_arr [i] = 0;
      _key_released_proc_arr [i] = 0;
      _characters_typed_proc_arr [i] = 0;
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
EventDispatcherStandardKeyboard <T>::~EventDispatcherStandardKeyboard ()
{
}



/*
==============================================================================
Name : configure_key
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::configure_key (Event::Pass pass, Proc key_pressed_proc)
{
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);

   _key_pressed_proc_arr [pass] = key_pressed_proc;
}



/*
==============================================================================
Name : configure_key
Note :
   Assume that a key repeated should be treated as a key pressed
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::configure_key (Event::Pass pass, Proc key_pressed_proc, Proc key_released_proc)
{
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);

   _key_pressed_proc_arr [pass] = key_pressed_proc;
   _key_repeated_proc_arr [pass] = key_pressed_proc;
   _key_released_proc_arr [pass] = key_released_proc;
}



/*
==============================================================================
Name : configure_key
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::configure_key (Event::Pass pass, Proc key_pressed_proc, Proc key_repeated_proc, Proc key_released_proc)
{
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);

   _key_pressed_proc_arr [pass] = key_pressed_proc;
   _key_repeated_proc_arr [pass] = key_repeated_proc;
   _key_released_proc_arr [pass] = key_released_proc;
}



/*
==============================================================================
Name : configure_characters
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::configure_characters (Event::Pass pass, Proc characters_typed_proc)
{
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);

   _characters_typed_proc_arr [pass] = characters_typed_proc;
}



/*
==============================================================================
Name : configure_catch_focus
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::configure_catch_focus (Proc keyboard_catch_focus_proc)
{
   _keyboard_catch_focus_proc = keyboard_catch_focus_proc;
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::get_cnt (EventTypeCounter & counter)
{
   for (size_t i = 0 ; i < Event::Pass_NBR_ELT ; ++i)
   {
      if (_key_pressed_proc_arr [i] != 0)
      {
         counter.add_key_pressed (static_cast <Event::Pass> (i));
      }

      if (_key_repeated_proc_arr [i] != 0)
      {
         counter.add_key_repeated (static_cast <Event::Pass> (i));
      }

      if (_key_released_proc_arr [i] != 0)
      {
         counter.add_key_released (static_cast <Event::Pass> (i));
      }

      if (_characters_typed_proc_arr [i] != 0)
      {
         counter.add_character_typed (static_cast <Event::Pass> (i));
      }
   }
   
   if (_keyboard_catch_focus_proc != 0)
   {
      counter.add_keyboard_catch_focus ();
   }
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::dispatch (Event & event)
{
   switch (event._type)
   {
   case Event::Type_KEY_PRESSED:
      call_key_pressed (event);
      break;

   case Event::Type_KEY_REPEATED:
      call_key_repeated (event);
      break;

   case Event::Type_KEY_RELEASED:
      call_key_released (event);
      break;

   case Event::Type_CHARACTERS_TYPED:
      call_characters_typed (event);
      break;

   case Event::Type_KEYBOARD_CATCH_FOCUS:
      call_keyboard_catch_focus (event);
      break;
   
   default:
      assert (false);
      break;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : call_key_pressed
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::call_key_pressed (Event & event)
{
   int pass = event._pass;
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);
   
   Proc proc = _key_pressed_proc_arr [pass];
   
   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



/*
==============================================================================
Name : call_key_repeated
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::call_key_repeated (Event & event)
{
   int pass = event._pass;
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);
   
   Proc proc = _key_repeated_proc_arr [pass];
   
   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



/*
==============================================================================
Name : call_key_released
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::call_key_released (Event & event)
{
   int pass = event._pass;
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);
   
   Proc proc = _key_released_proc_arr [pass];
   
   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



/*
==============================================================================
Name : call_characters_typed
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::call_characters_typed (Event & event)
{
   int pass = event._pass;
   assert (pass >= 0);
   assert (pass < Event::Pass_NBR_ELT);
   
   Proc proc = _characters_typed_proc_arr [pass];
   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



/*
==============================================================================
Name : call_keyboard_catch_focus
==============================================================================
*/

template <class T>
void  EventDispatcherStandardKeyboard <T>::call_keyboard_catch_focus (Event & event)
{
   if (_keyboard_catch_focus_proc != 0)
   {
      if (event._keyboard_catch_focus._type == KeyboardCatchFocus::Type_GIFT)
      {
         Frame * frame_ptr = dynamic_cast <Frame *> (&_object);
         assert (frame_ptr != 0);
         
         if (frame_ptr == event._keyboard_catch_focus._gifted_frame_ptr)
         {
            (_object.*_keyboard_catch_focus_proc) (event);
         }
      }
      else
      {
         assert (false);
      }
   }
}



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_EventDispatcherStandardKeyboard_CODEHEADER_INCLUDED

#undef ohm_opal_EventDispatcherStandardKeyboard_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
