/*****************************************************************************

        EventDispatcherStandardData.h
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



#if ! defined (ohm_opal_EventDispatcherStandardData_HEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandardData_HEADER_INCLUDED

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
class EventDispatcherStandardData
:  public EventDispatcher
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (T::*Proc) (opal::Event & event);
   
                  EventDispatcherStandardData (T & object);
   virtual        ~EventDispatcherStandardData ();
   
   void           configure_dropped (Proc data_dropped_proc);
   void           configure_dropped (Event::Pass pass, Proc data_dropped_proc);

   // EventDispatcher
   virtual void   get_cnt (EventTypeCounter & counter);
   virtual void   dispatch (Event & event);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         PASS_NBR_ELT = 2  };
   
   void           call_data_dropped (Event & event);

   T &            _object;
   
   Proc           _data_dropped_proc_arr [PASS_NBR_ELT];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventDispatcherStandardData ();
                  EventDispatcherStandardData (const EventDispatcherStandardData &other);
   EventDispatcherStandardData &
                  operator = (const EventDispatcherStandardData &other);
   bool           operator == (const EventDispatcherStandardData &other);
   bool           operator != (const EventDispatcherStandardData &other);

}; // class EventDispatcherStandardData



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcherStandardData.hpp"



#endif   // ohm_opal_EventDispatcherStandardData_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
