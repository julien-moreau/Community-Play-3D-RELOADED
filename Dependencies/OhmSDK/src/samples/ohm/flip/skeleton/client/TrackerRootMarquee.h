/*****************************************************************************

        TrackerRootMarquee.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70481

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



#if ! defined (client_TrackerRootMarquee_HEADER_INCLUDED)
#define  client_TrackerRootMarquee_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"



namespace ohm
{
namespace opal
{
class Frame;
class Event;
}
}



namespace client
{



class BundleRoot;
class FrameRoot;

class TrackerRootMarquee
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TrackerRootMarquee (BundleRoot & bundle, FrameRoot & frame);
   virtual        ~TrackerRootMarquee ();
   
   bool           is_triggered () const;
   bool           is_tracking () const;
   
   void           trigger ();
   void           start (ohm::opal::Event & event);
   void           track (ohm::opal::Event & event);
   void           stop (ohm::opal::Event & event);
   void           stop_revert ();
   void           mouse_target_lost ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum State
   {
                  State_VOID = 0,
                  State_TRIGGERED,
                  State_TRACKING,
                  State_STOPPING,
                  State_REVERTING,
   };

   BundleRoot &   _bundle;
   FrameRoot &    _frame;
   State          _state;
   ohm::opa::Point
                  _origin;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TrackerRootMarquee ();
                  TrackerRootMarquee (const TrackerRootMarquee &other);
   TrackerRootMarquee &    operator = (const TrackerRootMarquee &other);
   bool           operator == (const TrackerRootMarquee &other);
   bool           operator != (const TrackerRootMarquee &other);

}; // class TrackerRootMarquee



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/TrackerRootMarquee.hpp"



#endif   // client_TrackerRootMarquee_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
