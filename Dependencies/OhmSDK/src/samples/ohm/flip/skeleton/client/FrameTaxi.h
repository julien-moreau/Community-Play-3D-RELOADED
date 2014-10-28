/*****************************************************************************

        FrameTaxi.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71391

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



#if ! defined (client_FrameTaxi_HEADER_INCLUDED)
#define  client_FrameTaxi_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/TrackerTaxiPosition.h"
#include "client/TrackerTaxiWidth.h"

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcherStandard.h"



namespace model
{
class Taxi;
}



namespace client
{



class BundleTaxi;

class FrameTaxi
:  public ohm::opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FrameTaxi (BundleTaxi & bundle);
   virtual        ~FrameTaxi ();

   // opal::Frame
   virtual void   mouse_entered (ohm::opal::Event & event);
   virtual void   mouse_exited (ohm::opal::Event & event);

   virtual void   mouse_target_lost (const ohm::opal::Event & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum MouseZone
   {
                  MouseZone_NONE = 0,
                  MouseZone_POSITION,
                  MouseZone_WIDTH,
   };
   
   // delegates
   void           mouse_button_pressed (ohm::opal::Event & event);
   void           mouse_button_released (ohm::opal::Event & event);
   void           mouse_dragged (ohm::opal::Event & event);
   void           mouse_moved (ohm::opal::Event & event);
   void           mouse_cursor (ohm::opal::Event & event);
   void           user_input_lost (ohm::opal::Event & event);
   void           conduct_changed (ohm::opal::Event & event);
   void           focus_key_changed (ohm::opal::Event & event);
   
   MouseZone      get_mouse_zone (ohm::opal::Event & event);
   
   BundleTaxi &   _bundle;
   ohm::opal::EventDispatcherStandard <FrameTaxi>
                  _dispatcher;
   
   TrackerTaxiPosition
                  _tracker_position;
   TrackerTaxiWidth
                  _tracker_width;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameTaxi ();
                  FrameTaxi (const FrameTaxi &other);
   FrameTaxi &    operator = (const FrameTaxi &other);
   bool           operator == (const FrameTaxi &other);
   bool           operator != (const FrameTaxi &other);

}; // class FrameTaxi



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/FrameTaxi.hpp"



#endif   // client_FrameTaxi_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
