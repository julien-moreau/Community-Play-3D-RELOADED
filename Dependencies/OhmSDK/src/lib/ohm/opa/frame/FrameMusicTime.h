/*****************************************************************************

        FrameMusicTime.h
        Copyright (c) 2012 Ohm Force

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



#if ! defined (ohm_opa_FrameMusicTime_HEADER_INCLUDED)
#define  ohm_opa_FrameMusicTime_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/frame/TrackerMusicTimeValue.h"

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcherStandard.h"



namespace ohm
{
namespace opa
{



class WidgetMusicTime;

class FrameMusicTime
:  public opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  FrameMusicTime (WidgetMusicTime & widget, int field);
   virtual        ~FrameMusicTime ();
   
   // opal::Frame
   virtual void   mouse_entered (opal::Event & event);
   virtual void   mouse_exited (opal::Event & event);

   virtual void   mouse_target_lost (const opal::Event & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   // delegates
   void           mouse_moved (opal::Event & event);
   void           mouse_cursor (opal::Event & event);

   void           mouse_button_pressed (opal::Event & event);
   void           mouse_button_released (opal::Event & event);
   void           mouse_dragged (opal::Event & event);

   void           mouse_button_pressed_twice (opal::Event & event);
   void           mouse_button_released_twice (opal::Event & event);

   void           user_input_lost (opal::Event & event);

   WidgetMusicTime &
                  _widget;
   const int      _field;

   opal::EventDispatcherStandard <FrameMusicTime>
                  _dispatcher;

   TrackerMusicTimeValue
                  _tracker_value;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameMusicTime (const FrameMusicTime &other);
   FrameMusicTime &  operator = (const FrameMusicTime &other);
   bool           operator == (const FrameMusicTime &other) const;
   bool           operator != (const FrameMusicTime &other) const;

}; // class FrameMusicTime



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/client/ctlb/FrameMusicTime.hpp"



#endif   // ohm_opa_FrameMusicTime_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

