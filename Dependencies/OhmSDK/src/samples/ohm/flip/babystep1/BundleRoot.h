/*****************************************************************************

        BundleRoot.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70401

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



#if ! defined (BundleRoot_HEADER_INCLUDED)
#define  BundleRoot_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "FrameRoot.h"
#include "PrimRoot.h"

#include "ohm/opa/Bundle.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opal/AutoFramePtr.h"



namespace ohm
{
namespace opa
{
class EventBundleView;
class BundleView;
}
}



class ModelRoot;

class BundleRoot
:  public ohm::opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Slot
   {
                  Slot_BASE = 0,
                  
                  NBR_Slot_TOTAL
   };

                  BundleRoot (ModelRoot & root, ohm::opa::BundleView & parent);
   virtual        ~BundleRoot ();
   
   // Called from SkeletonGui
   void           update ();
   
   // Called from FrameRoot
   void           move (const ohm::opa::Point & point);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   // delegates
   void           process_event (ohm::opa::EventBundleView & event);


   ModelRoot &    _root;
   ohm::opa::BundleView &
                  _parent;

   ohm::opal::AutoFramePtr <FrameRoot>
                  _frame_aptr;
   ohm::opak::AutoPrimPtr <PrimRoot>
                  _prim_aptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundleRoot ();
                  BundleRoot (const BundleRoot &other);
   BundleRoot &   operator = (const BundleRoot &other);
   bool           operator == (const BundleRoot &other);
   bool           operator != (const BundleRoot &other);

}; // class BundleRoot



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "BundleRoot.hpp"



#endif   // BundleRoot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
