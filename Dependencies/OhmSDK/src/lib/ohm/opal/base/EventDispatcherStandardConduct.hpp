/*****************************************************************************

        EventDispatcherStandardConduct.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65062

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



#if defined (ohm_opal_EventDispatcherStandardConduct_CURRENT_CODEHEADER)
   #error Recursive inclusion of EventDispatcherStandardConduct code header.
#endif
#define  ohm_opal_EventDispatcherStandardConduct_CURRENT_CODEHEADER

#if ! defined (ohm_opal_EventDispatcherStandardConduct_CODEHEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardConduct_CODEHEADER_INCLUDED



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
EventDispatcherStandardConduct <T>::EventDispatcherStandardConduct (T & object)
:  _object (object)
{
   assert (&object != 0);

   ohm_lang_CHECK_CST (Event_Pass_SPATIAL_is_1, Event::Pass_SPATIAL == 1);
   ohm_lang_CHECK_CST (Event_Pass_CRUMB_is_2, Event::Pass_CRUMB == 2);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
EventDispatcherStandardConduct <T>::~EventDispatcherStandardConduct ()
{
}



/*
==============================================================================
Name : configure_lost
==============================================================================
*/

template <class T>
template <class ConductType>
void  EventDispatcherStandardConduct <T>::configure (Event::Pass pass, Proc conduct_changed_proc)
{
   assert (pass >= Event::Pass_SPATIAL);
   assert (pass <= Event::Pass_CRUMB);
   
   const std::type_info * type_info_ptr = &typeid (ConductType);

   _conduct_changed_proc_arr_map [type_info_ptr]._proc_arr [pass - 1] = conduct_changed_proc;
}



/*
==============================================================================
Name: configure
==============================================================================
*/

template <class T>
void EventDispatcherStandardConduct <T>::configure (Event::Pass pass, Proc conduct_changed_proc)
{
   configure <ConductNone> (pass, conduct_changed_proc);
}



/*
==============================================================================
Name : get_cnt
==============================================================================
*/

template <class T>
void  EventDispatcherStandardConduct <T>::get_cnt (EventTypeCounter & counter)
{
   typename ProcArrMap::iterator it = _conduct_changed_proc_arr_map.begin ();
   const typename ProcArrMap::iterator it_end = _conduct_changed_proc_arr_map.end ();
   
   for (; it != it_end ; ++it)
   {
      ProcArr & proc_arr = it->second._proc_arr;
      
      for (size_t i = 0 ; i < PASS_NBR_ELT ; ++i)
      {
         if (proc_arr [i] != 0)
         {
            Event::Pass pass = static_cast <Event::Pass> (i + 1);
            
            counter.add_conduct_changed (pass);
         }
      }
   }
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

template <class T>
void  EventDispatcherStandardConduct <T>::dispatch (Event & event)
{
   switch (event._type)
   {
   case Event::Type_CONDUCT_CHANGED:
      call_conduct_changed (event);
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
Name : call_conduct_changed
==============================================================================
*/

template <class T>
void  EventDispatcherStandardConduct <T>::call_conduct_changed (Event & event)
{
   int pass = event._pass - 1;

   assert (pass >= 0);
   assert (pass < PASS_NBR_ELT);
   
   Proc proc = 0;
   
   assert (event._conduct_change._type_info_ptr != 0);
   
   {
      typename ProcArrMap::iterator it
         = _conduct_changed_proc_arr_map.find (event._conduct_change._type_info_ptr);
      
      if (it != _conduct_changed_proc_arr_map.end ())
      {
         ProcArr & proc_arr = it->second._proc_arr;
         
         proc = proc_arr [pass];
      }
   }
   
   if (proc == 0)
   {
      static const std::type_info * none_info_ptr = & typeid (ConductNone);
      
      typename ProcArrMap::iterator it
         = _conduct_changed_proc_arr_map.find (none_info_ptr);
      
      if (it != _conduct_changed_proc_arr_map.end ())
      {
         ProcArr & proc_arr = it->second._proc_arr;
         
         proc = proc_arr [pass];
      }
   }

   if (proc != 0)
   {
      (_object.*proc) (event);
   }
}



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_EventDispatcherStandardConduct_CODEHEADER_INCLUDED

#undef ohm_opal_EventDispatcherStandardConduct_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
