/*****************************************************************************

        BundleUnscale.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52448

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

#include "ohm/opa/BundleUnscale.h"
#include "ohm/opa/private/FrameUnscale.h"
#include "ohm/opa/private/PrimUnscale.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

BundleUnscale::BundleUnscale ()
:  opa::Bundle ("Unscale")
,  _frame_aptr ()
,  _layer_aptr ()
{
   _frame_aptr = new FrameUnscale;
   
   _layer_aptr = new PrimUnscale;
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_aptr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_aptr);
   set_slot (0, *_layer_aptr);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

BundleUnscale::BundleUnscale (opa::Bundle & parent_bundle, size_t slot_nbr)
:  opa::Bundle ("Unscale")
,  _frame_aptr ()
,  _layer_aptr ()
{
   _frame_aptr = new FrameUnscale;
   _layer_aptr = new PrimUnscale;
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_aptr);
   
   parent_bundle.embed (*this, slot_nbr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_aptr);
   set_slot (0, *_layer_aptr);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundleUnscale::~BundleUnscale ()
{
}



/*
==============================================================================
Name : set_origin
==============================================================================
*/

void  BundleUnscale::set_origin (const opa::Point & origin, double transition_time, opak::TransitionFunction transition_function)
{
   _frame_aptr->set_origin (origin);
   _layer_aptr->_origin.stop ().push (origin, transition_time, transition_function);
}



/*
==============================================================================
Name : get_origin
==============================================================================
*/

const opa::Point &   BundleUnscale::get_origin () const
{
   return _frame_aptr->get_origin ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
