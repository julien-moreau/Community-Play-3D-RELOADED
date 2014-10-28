/*****************************************************************************

        TrackerPedestrianPosition.h
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



#if ! defined (client_TrackerPedestrianPosition_HEADER_INCLUDED)
#define  client_TrackerPedestrianPosition_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/base/TrackerBase.h"
#include "ohm/opa/Point.h"
#include "ohm/task/Timer.h"



namespace client
{



class BundlePedestrian;
class FramePedestrian;

class TrackerPedestrianPosition
:  public ohm::opal::TrackerBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  TrackerPedestrianPosition (BundlePedestrian & bundle, FramePedestrian & frame);
   virtual        ~TrackerPedestrianPosition ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual bool   do_start (ohm::opal::Event & event);
   virtual void   do_track (ohm::opal::Event & event);
   virtual void   do_key_changed (ohm::opal::Event & event);
   virtual void   do_commit (ohm::opal::Event & event);
   virtual void   do_revert ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           update_cursor (ohm::opal::Event & event);
   
   BundlePedestrian &
                  _bundle;
   FramePedestrian &
                  _frame;

   ohm::opa::Point
                  _rel_origin;
   bool           _copy_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TrackerPedestrianPosition ();
                  TrackerPedestrianPosition (const TrackerPedestrianPosition &other);
   TrackerPedestrianPosition &
                  operator = (const TrackerPedestrianPosition &other);
   bool           operator == (const TrackerPedestrianPosition &other);
   bool           operator != (const TrackerPedestrianPosition &other);

}; // class TrackerPedestrianPosition



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/TrackerPedestrianPosition.hpp"



#endif   // client_TrackerPedestrianPosition_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
