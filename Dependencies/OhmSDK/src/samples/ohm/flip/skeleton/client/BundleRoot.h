/*****************************************************************************

        BundleRoot.h
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



#if ! defined (client_BundleRoot_HEADER_INCLUDED)
#define  client_BundleRoot_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/Conduct.h"
#include "client/ConductMarquee.h"
#include "client/FrameRoot.h"
#include "client/PrimMarquee.h"
#include "client/PrimRoot.h"

#include "ohm/opa/Bundle.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/RectFilled.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opal/FrameConduct.h"



namespace model
{
class Root;
}


namespace client
{



class BundleGui;

class BundleRoot
:  public ohm::opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BundleRoot (model::Root & root, BundleGui & parent);
   virtual        ~BundleRoot ();
   
   bool           req_start ();
   void           req_commit ();
   void           req_revert ();
   
   void           req_focus_delta (const ohm::opa::Point & delta);
   
   void           req_start_marquee (const ohm::opa::Point & origin);
   void           req_change_marquee_size (const ohm::opa::Size & bounds);
   void           req_end_marquee (bool toggle_flag);
   
   void           req_clear_selection ();
   void           req_delete_selection ();
   void           req_move_selection (const ohm::opa::Point & delta, bool big_flag, bool copy_flag);
   void           req_select_marquee ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   model::Root &  _root;
   BundleGui &    _parent;

   ohm::opal::AutoFramePtr <ohm::opal::Frame>
                  _frame_aptr;
   ohm::opal::AutoFramePtr <ohm::opal::FrameConduct <Conduct> >
                  _frame_conduct_aptr;
   ohm::opal::AutoFramePtr <ohm::opal::FrameConduct <ConductMarquee> >
                  _frame_marquee_aptr;
   ohm::opal::AutoFramePtr <FrameRoot>
                  _frame_root_aptr;
   ohm::opak::AutoPrimPtr <PrimRoot>
                  _prim_aptr;
   ohm::opak::AutoPrimPtr <ohm::opak::Layer>
                  _layer_root_aptr;
   ohm::opak::AutoPrimPtr <PrimMarquee>
                  _prim_marquee_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundleRoot ();
                  BundleRoot (const BundleRoot &other);
   BundleRoot &   operator = (const BundleRoot &other);
   bool           operator == (const BundleRoot &other);
   bool           operator != (const BundleRoot &other);

}; // class BundleRoot



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/BundleRoot.hpp"



#endif   // client_BundleRoot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
