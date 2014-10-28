/*****************************************************************************

        EventDispatcherStandardCustom.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64410

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



#if ! defined (ohm_opal_EventDispatcherStandardCustom_HEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardCustom_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcher.h"



namespace ohm
{
namespace opal
{



template <class T>
class EventDispatcherStandardCustom
:  public EventDispatcher
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (T::*Proc) (opal::Event & event);
   
                  EventDispatcherStandardCustom (T & object);
   virtual        ~EventDispatcherStandardCustom ();
   
   template <class EventType>
   void           configure (Event::Pass pass, Proc custom_sent_proc);

   // EventDispatcher
   virtual void   get_cnt (EventTypeCounter & counter);
   virtual void   dispatch (Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         PASS_NBR_ELT = 2  };
   
   typedef Proc ProcArr [PASS_NBR_ELT];
   class ProcArrB
   {
   public:
                  ProcArrB () {for (int i = 0 ; i < PASS_NBR_ELT ; ++i) _proc_arr [i] = 0;}
      ProcArr     _proc_arr;
   };
   typedef std::map <const std::type_info *, ProcArrB>   ProcArrMap;
   
   void           call_custom_sent (Event & event);

   T &            _object;
   
   ProcArrMap     _custom_sent_proc_arr_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventDispatcherStandardCustom ();
                  EventDispatcherStandardCustom (const EventDispatcherStandardCustom &other);
   EventDispatcherStandardCustom &
                  operator = (const EventDispatcherStandardCustom &other);
   bool           operator == (const EventDispatcherStandardCustom &other);
   bool           operator != (const EventDispatcherStandardCustom &other);

}; // class EventDispatcherStandardCustom



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcherStandardCustom.hpp"



#endif   // ohm_opal_EventDispatcherStandardCustom_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
