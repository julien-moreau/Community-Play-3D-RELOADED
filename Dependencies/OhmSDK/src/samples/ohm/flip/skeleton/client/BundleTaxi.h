/*****************************************************************************

        BundleTaxi.h
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



#if ! defined (client_BundleTaxi_HEADER_INCLUDED)
#define  client_BundleTaxi_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/FrameTaxi.h"
#include "client/PrimTaxi.h"
#include "client/SelectionStateMgr.h"

#include "ohm/opa/Bundle.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opal/AutoFramePtr.h"



namespace model
{
class Taxi;
}


namespace client
{



class BundleTaxi
:  public ohm::opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BundleTaxi (model::Taxi & taxi, ohm::opa::Bundle & parent);
   virtual        ~BundleTaxi ();
   
   model::Taxi &  use_taxi ();
   
   void           update_selection_state (bool selected_flag, ohm::archi::Int32 user_id);
   void           update_conflict ();
   void           update_position ();
   void           update_width ();
   void           update_color ();
   
   bool           req_start ();
   void           req_commit (bool copy_flag);
   void           req_revert ();

   void           req_change_copy_mode (bool copy_flag);
   void           req_change_position (float position_x);
   void           req_change_width (float width);
   
   void           req_highlight (bool highlight_flag);
   void           req_select (bool toggle_flag);
   void           req_delete ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   model::Taxi &  _taxi;
   ohm::opa::Bundle &
                  _parent;

   ohm::opal::AutoFramePtr <FrameTaxi>
                  _frame_aptr;
   ohm::opak::AutoPrimPtr <PrimTaxi>
                  _prim_aptr;

   SelectionStateMgr
                  _sel_state_mgr;
   
   // ghost state memory
   ohm::archi::Float64
                  _position_x;
   ohm::archi::Float64
                  _width;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundleTaxi ();
                  BundleTaxi (const BundleTaxi &other);
   BundleTaxi &   operator = (const BundleTaxi &other);
   bool           operator == (const BundleTaxi &other);
   bool           operator != (const BundleTaxi &other);

}; // class BundleTaxi



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/BundleTaxi.hpp"



#endif   // client_BundleTaxi_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
