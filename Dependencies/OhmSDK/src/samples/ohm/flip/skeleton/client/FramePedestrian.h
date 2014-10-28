/*****************************************************************************

        FramePedestrian.h
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



#if ! defined (client_FramePedestrian_HEADER_INCLUDED)
#define  client_FramePedestrian_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/TrackerPedestrianPosition.h"

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcherStandard.h"



namespace model
{
class Pedestrian;
}



namespace client
{



class BundlePedestrian;

class FramePedestrian
:  public ohm::opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FramePedestrian (BundlePedestrian & bundle);
   virtual        ~FramePedestrian ();

   // opal::Frame
   virtual void   mouse_entered (ohm::opal::Event & event);
   virtual void   mouse_exited (ohm::opal::Event & event);

   virtual void   mouse_target_lost (const ohm::opal::Event & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   // delegates
   void           mouse_button_pressed (ohm::opal::Event & event);
   void           mouse_button_released (ohm::opal::Event & event);
   void           mouse_dragged (ohm::opal::Event & event);
   void           mouse_moved (ohm::opal::Event & event);
   void           user_input_lost (ohm::opal::Event & event);
   void           conduct_changed (ohm::opal::Event & event);
   void           focus_key_changed (ohm::opal::Event & event);
   
   BundlePedestrian &
                  _bundle;
   ohm::opal::EventDispatcherStandard <FramePedestrian>
                  _dispatcher;
   
   TrackerPedestrianPosition
                  _tracker_position;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FramePedestrian ();
                  FramePedestrian (const FramePedestrian &other);
   FramePedestrian &    operator = (const FramePedestrian &other);
   bool           operator == (const FramePedestrian &other);
   bool           operator != (const FramePedestrian &other);

}; // class FramePedestrian



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/FramePedestrian.hpp"



#endif   // client_FramePedestrian_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
