/*****************************************************************************

        EventDispatcherStandardConduct.h
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



#if ! defined (ohm_opal_EventDispatcherStandardConduct_HEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardConduct_HEADER_INCLUDED

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
class EventDispatcherStandardConduct
:  public EventDispatcher
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (T::*Proc) (opal::Event & event);
   
                  EventDispatcherStandardConduct (T & object);
   virtual        ~EventDispatcherStandardConduct ();
   
   template <class ConductType>
   void           configure (Event::Pass pass, Proc conduct_changed_proc);
   void           configure (Event::Pass pass, Proc conduct_changed_proc);

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

   class ConductNone
   {
   };
   
   void           call_conduct_changed (Event & event);

   T &            _object;
   
   ProcArrMap     _conduct_changed_proc_arr_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventDispatcherStandardConduct ();
                  EventDispatcherStandardConduct (const EventDispatcherStandardConduct &other);
   EventDispatcherStandardConduct &
                  operator = (const EventDispatcherStandardConduct &other);
   bool           operator == (const EventDispatcherStandardConduct &other);
   bool           operator != (const EventDispatcherStandardConduct &other);

}; // class EventDispatcherStandardConduct



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcherStandardConduct.hpp"



#endif   // ohm_opal_EventDispatcherStandardConduct_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
