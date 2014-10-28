/*****************************************************************************

        FrameRoot.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71268

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



#if ! defined (client_FrameRoot_HEADER_INCLUDED)
#define  client_FrameRoot_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/TrackerRootMarquee.h"
#include "client/TrackerRootHand.h"

#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcherStandard.h"



namespace model
{
class Root;
}



namespace client
{



class BundleRoot;

class FrameRoot
:  public ohm::opal::Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  FrameRoot (BundleRoot & bundle);
   virtual        ~FrameRoot ();

   // ohm::opal::Frame
   virtual void   mouse_target_lost (const ohm::opal::Event & event);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   // delegates
   void           mouse_button_pressed (ohm::opal::Event & event);
   void           mouse_button_released (ohm::opal::Event & event);
   void           mouse_dragged (ohm::opal::Event & event);
   void           mouse_cursor (ohm::opal::Event & event);
   void           mouse_wheel_moved (ohm::opal::Event & event);
   void           crumb_key_pressed (ohm::opal::Event & event);
   void           crumb_key_released (ohm::opal::Event & event);
   void           user_input_lost (ohm::opal::Event & event);
   void           conduct_changed (ohm::opal::Event & event);


   BundleRoot &   _bundle;
   ohm::opal::EventDispatcherStandard <FrameRoot>
                  _dispatcher;
   
   TrackerRootMarquee
                  _tracker_marquee;
   TrackerRootHand
                  _tracker_hand;
   
   bool           _command_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FrameRoot ();
                  FrameRoot (const FrameRoot &other);
   FrameRoot &    operator = (const FrameRoot &other);
   bool           operator == (const FrameRoot &other);
   bool           operator != (const FrameRoot &other);

}; // class FrameRoot



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/FrameRoot.hpp"



#endif   // client_FrameRoot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
