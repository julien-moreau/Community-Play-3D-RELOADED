/*****************************************************************************

        EventDispatcherStandardUserInput.hpp
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



#if defined (ohm_opal_EventDispatcherStandardUserInput_CURRENT_CODEHEADER)
   #error Recursive inclusion of EventDispatcherStandardUserInput code header.
#endif
#define  ohm_opal_EventDispatcherStandardUserInput_CURRENT_CODEHEADER

#if ! defined (ohm_opal_EventDispatcherStandardUserInput_CODEHEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardUserInput_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"



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
EventDispatcherStandardUserInput <T>::EventDispatcherStandardUserInput (T & object)
:  _object (object)

,  _user_input_lost_proc (0)
{
   assert (&object != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
EventDispatcherStandardUserInput <T>::~EventDispatcherStandardUserInput ()
{
}



/*
==============================================================================
Name : configure_lost
==============================================================================
*/

template <class T>
void  EventDispatcherStandardUserInput <T>::configure_lost (Proc user_input_lost_proc)
{
   _user_input_lost_proc = user_input_lost_proc;
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

template <class T>
void  EventDispatcherStandardUserInput <T>::get_cnt (EventTypeCounter & counter)
{
   if (_user_input_lost_proc != 0)
   {
      counter.add_user_input_lost ();
   }
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

template <class T>
void  EventDispatcherStandardUserInput <T>::dispatch (Event & event)
{
   switch (event._type)
   {
   case Event::Type_USER_INPUT_LOST:
      if (_user_input_lost_proc != 0)
      {
         (_object.*_user_input_lost_proc) (event);
      }
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



#endif   // ohm_opal_EventDispatcherStandardUserInput_CODEHEADER_INCLUDED

#undef ohm_opal_EventDispatcherStandardUserInput_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
