/*****************************************************************************

        EventDispatcherStandard.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_opal_EventDispatcherStandard_HEADER_INCLUDED)
#define  ohm_opal_EventDispatcherStandard_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcher.h"
#include "ohm/opal/base/EventDispatcherStandardMouse.h"
#include "ohm/opal/base/EventDispatcherStandardKeyboard.h"
#include "ohm/opal/base/EventDispatcherStandardUserInput.h"
#include "ohm/opal/base/EventDispatcherStandardConduct.h"
#include "ohm/opal/base/EventDispatcherStandardCustom.h"
#include "ohm/opal/base/EventDispatcherStandardData.h"



namespace ohm
{
namespace opal
{



template <class T>
class EventDispatcherStandard
:  public EventDispatcher
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef void (T::*Proc) (Event & event);
   
                  EventDispatcherStandard (T & object);
   virtual        ~EventDispatcherStandard ();
   
   // EventDispatcher
   virtual void   get_cnt (EventTypeCounter & counter);
   virtual void   dispatch (Event & event);
   
   EventDispatcherStandardMouse <T>
                  _mouse;
   EventDispatcherStandardKeyboard <T>
                  _keyboard;
   EventDispatcherStandardUserInput <T>
                  _user_input;
   EventDispatcherStandardConduct <T>
                  _conduct;
   EventDispatcherStandardCustom <T>
                  _custom;
   EventDispatcherStandardData <T>
                  _data;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   T &            _object;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EventDispatcherStandard ();
                  EventDispatcherStandard (const EventDispatcherStandard &other);
   EventDispatcherStandard &
                  operator = (const EventDispatcherStandard &other);
   bool           operator == (const EventDispatcherStandard &other);
   bool           operator != (const EventDispatcherStandard &other);

}; // class EventDispatcherStandard



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/EventDispatcherStandard.hpp"



#endif   // ohm_opal_EventDispatcherStandard_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
