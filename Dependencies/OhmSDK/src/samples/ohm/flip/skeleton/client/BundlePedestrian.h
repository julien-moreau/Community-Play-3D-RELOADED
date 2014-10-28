/*****************************************************************************

        BundlePedestrian.h
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



#if ! defined (client_BundlePedestrian_HEADER_INCLUDED)
#define  client_BundlePedestrian_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/FramePedestrian.h"
#include "client/PrimPedestrian.h"
#include "client/SelectionStateMgr.h"


#include "ohm/opa/Bundle.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opal/AutoFramePtr.h"



namespace model
{
class Pedestrian;
}


namespace client
{



class BundlePedestrian
:  public ohm::opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BundlePedestrian (model::Pedestrian & pedestrian, ohm::opa::Bundle & parent);
   virtual        ~BundlePedestrian ();
   
   model::Pedestrian &
                  use_pedestrian ();
   
   void           update_selection_state (bool selected_flag, ohm::archi::Int32 user_id);
   void           update_conflict ();
   void           update_position ();
   
   bool           req_start ();
   void           req_commit (bool copy_flag);
   void           req_revert ();

   void           req_change_copy_mode (bool copy_flag);
   void           req_change_position (const ohm::opa::Point & position);
   
   void           req_highlight (bool highlight_flag);
   void           req_select (bool toggle_flag);
   void           req_delete ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   model::Pedestrian &
                  _pedestrian;
   ohm::opa::Bundle &
                  _parent;

   ohm::opal::AutoFramePtr <FramePedestrian>
                  _frame_aptr;
   ohm::opak::AutoPrimPtr <PrimPedestrian>
                  _prim_aptr;

   SelectionStateMgr
                  _sel_state_mgr;
   
   // ghost state memory
   ohm::archi::Float64
                  _position_x;
   ohm::archi::Float64
                  _position_y;
   ohm::archi::Float64
                  _width;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundlePedestrian ();
                  BundlePedestrian (const BundlePedestrian &other);
   BundlePedestrian &
                  operator = (const BundlePedestrian &other);
   bool           operator == (const BundlePedestrian &other);
   bool           operator != (const BundlePedestrian &other);

}; // class BundlePedestrian



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/BundlePedestrian.hpp"



#endif   // client_BundlePedestrian_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
