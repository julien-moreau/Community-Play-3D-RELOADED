/*****************************************************************************

        EventDispatcherStandardData.hpp
        Copyright (c) 2011 Ohm Force

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



#if defined (ohm_opal_EventDispatcherStandardData_CURRENT_CODEHEADER)
   #error Recursive inclusion of EventDispatcherStandardData code header.
#endif
#define  ohm_opal_EventDispatcherStandardData_CURRENT_CODEHEADER

#if ! defined (ohm_opal_EventDispatcherStandardData_CODEHEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardData_CODEHEADER_INCLUDED



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
EventDispatcherStandardData <T>::EventDispatcherStandardData (T & object)
:  _object (object)
{
   assert (&object != 0);

   for (size_t i = 0 ; i < PASS_NBR_ELT ; ++i)
   {
      _data_dropped_proc_arr [i] = 0;
   }

   ohm_lang_CHECK_CST (Event_Pass_SPATIAL_is_1, Event::Pass_SPATIAL == 1);
   ohm_lang_CHECK_CST (Event_Pass_CRUMB_is_2, Event::Pass_CRUMB == 2);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
EventDispatcherStandardData <T>::~EventDispatcherStandardData ()
{
}



/*
==============================================================================
Name : configure_dropped
==============================================================================
*/

template <class T>
void  EventDispatcherStandardData <T>::configure_dropped (Proc data_dropped_proc)
{
   configure_dropped (Event::Pass_SPATIAL, data_dropped_proc);
}



/*
==============================================================================
Name : configure_dropped
==============================================================================
*/

template <class T>
void  EventDispatcherStandardData <T>::configure_dropped (Event::Pass pass, Proc data_dropped_proc)
{
   assert (pass >= Event::Pass_SPATIAL);
   assert (pass <= Event::Pass_CRUMB);
   
   _data_dropped_proc_arr [pass - 1] = data_dropped_proc;
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

template <class T>
void  EventDispatcherStandardData <T>::get_cnt (EventTypeCounter & counter)
{
   for (size_t i = 0 ; i < PASS_NBR_ELT ; ++i)
   {
      if (_data_dropped_proc_arr [i] != 0)
      {
         Event::Pass pass = static_cast <Event::Pass> (i + 1);
         
         counter.add_data_dropped (pass);
      }
   }
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

template <class T>
void  EventDispatcherStandardData <T>::dispatch (Event & event)
{
   switch (event._type)
   {
   case Event::Type_DATA_DROPPED:
      call_data_dropped (event);
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
Name : call_data_dropped
==============================================================================
*/

template <class T>
void  EventDispatcherStandardData <T>::call_data_dropped (Event & event)
{
   int pass = event._pass - 1;

   assert (pass >= 0);
   assert (pass < PASS_NBR_ELT);
   
   Proc proc = _data_dropped_proc_arr [pass];
   
   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_EventDispatcherStandardData_CODEHEADER_INCLUDED

#undef ohm_opal_EventDispatcherStandardData_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
