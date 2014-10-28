/*****************************************************************************

        BundleRoot.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "BundleRoot.h"
#include "ModelRoot.h"

#include "ohm/opa/BundleView.h"
#include "ohm/opa/EventBundleView.h"
#include "ohm/opal/Frame.h"

#include <cassert>



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   This bundle is in charge or representing data model class 'Root'.
   It creates a frame to receive user inputs, and a primitive to display.
   The frames will translate user inputs events to the bundle abstraction
   which will in turn communicate it to '_root'. the data model framework
   will callback with '_root' changes, on which the primitive will react.
==============================================================================
*/

BundleRoot::BundleRoot (ModelRoot & root, ohm::opa::BundleView & parent)
:  ohm::opa::Bundle ("BundleRoot")
,  _root (root)
,  _parent (parent)
{
   using namespace ohm;
   
   // create one frame and primitive for the whole bundle
   // make them embeddable so that they can be pushed
   // to a bundle
   
   _frame_aptr = new FrameRoot (*this);
   set_embeddable (*_frame_aptr);
   
   _prim_aptr = new PrimRoot;
   set_embeddable (*_prim_aptr);
   
   // create one slot to allow this bundle to embed
   // other bundles (through their respective frames/primitives)
   // in particular, PrimRoot is a PrimitiveContainer
   
   set_nbr_slot (NBR_Slot_TOTAL);
   set_slot (Slot_BASE, *_frame_aptr);
   set_slot (Slot_BASE, *_prim_aptr);
   
   // bind this bundle to receive bundle events, in particular
   // to receive view/window size changes
   
   bind <
      BundleRoot, 
      opa::EventBundleView, 
      &BundleRoot::process_event
   > ();
   
   //
   
   const opa::Size & bounds = _parent.get_bounds ();

   _frame_aptr->set_bounds (bounds);
   _prim_aptr->_size = bounds;
   _prim_aptr->_color = opal::Color (1.f, .75f);
   _prim_aptr->_alpha = 1.f;
   
   update ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundleRoot::~BundleRoot ()
{
}



/*
==============================================================================
Name : update
Description :
   Called from SkeletonGui, this indicates that something changed in
   the '_root' data model element.
   Update its visual representation.
==============================================================================
*/

void  BundleRoot::update ()
{
}



/*
==============================================================================
Name : move
Description :
   Called from FrameRoot, this indicates that a user input occured, and that
   the frame translate the event to something matching the bundle abstraction.
   Here it consists of moving something to 'point'.
   We are going to control the model data from here, and 'update' above will
   be called sometime later.
==============================================================================
*/

void  BundleRoot::move (const ohm::opa::Point & point)
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: process_event
Description :
   Receiver of bundle events.
   Here we only process layout changed event, which indicates that a
   view/window size changed.
==============================================================================
*/

void BundleRoot::process_event (ohm::opa::EventBundleView & event)
{
   using namespace ohm;
   
   if (event._type == opa::EventBundleView::Type_LAYOUT_CHANGED)
   {
      // our parent BundleView is attached to the view and
      // already have the view/window size
      const opa::Size & bounds = _parent.get_bounds ();

      _frame_aptr->set_bounds (bounds);
      _prim_aptr->_size = bounds;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
