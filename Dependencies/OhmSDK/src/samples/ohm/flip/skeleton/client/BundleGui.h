/*****************************************************************************

        BundleGui.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71435

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



#if ! defined (client_BundleGui_HEADER_INCLUDED)
#define  client_BundleGui_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/BundleRoot.h"
#include "client/BundleTaxiGhost.h"

#include "model/Root.h"

#include "ohm/flip/DocumentObserver.h"
#include "ohm/opa/Bundle.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opak/Layer.h"
#include "ohm/opak/AutoPrimPtr.h"

#include <list>



namespace ohm
{
namespace opa
{
class EventBundleView;
class BundleView;
}
}


namespace model
{
class SelectionChangeInfo;
}


namespace client
{



class BundleRoot;

class BundleGui
:  public ohm::flip::DocumentObserver <model::Root>
,  public ohm::opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Slot
   {
                  Slot_BASE = 0,
                  Slot_CONTENT,
                  
                  Slot_NBR_ELT
   };
   
                  BundleGui (ohm::opa::BundleView & parent);
   virtual        ~BundleGui ();
   
   void           update ();
   void           set (const ohm::opa::Rect & rect, double tt, ohm::opak::TransitionFunction tf);
   
   // ohm::flip::DocumentObserver <model::Root>
   virtual void   document_changed (model::Root & root);
   virtual void   signal (ohm::flip::Object * obj_ptr, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data);   



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           document_changed_selection (model::ClientData & client_data);
   void           document_changed_view_info (model::ClientData & client_data);

   void           document_changed (model::SpacialObject & spacial_object);
   void           document_changed (model::Taxi & taxi);
   void           document_changed (model::Pedestrian & pedestrian);

   void           signal (model::Selection & selection, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data);
   void           signal_start (model::Selection & selection, const ohm::flip::DataMaster & signal_data);
   void           signal_end (model::Selection & selection, const ohm::flip::DataMaster & signal_data);
   void           signal_change (model::Selection & selection, const ohm::flip::DataMaster & signal_data);
   void           signal_change (model::Selection & selection, const model::SelectionChangeInfo & info);

   void           handle_event (ohm::opa::EventBundleView & event);
   
   ohm::opa::BundleView &
                  _parent;

   ohm::opal::AutoFramePtr <ohm::opal::Frame>
                  _frame_base_aptr;
   ohm::opak::AutoPrimPtr <ohm::opak::Layer>
                  _layer_base_aptr;

   ohm::opal::AutoFramePtr <ohm::opal::Frame>
                  _frame_content_aptr;
   ohm::opak::AutoPrimPtr <ohm::opak::Layer>
                  _layer_content_aptr;

   BundleRoot *   _bnd_root_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundleGui ();
                  BundleGui (const BundleGui &other);
   BundleGui &    operator = (const BundleGui &other);
   bool           operator == (const BundleGui &other);
   bool           operator != (const BundleGui &other);

}; // class BundleGui



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/BundleGui.hpp"



#endif   // client_BundleGui_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
