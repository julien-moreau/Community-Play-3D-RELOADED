/*****************************************************************************

        GraphicContext.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67385

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

#include "ohm/opak/base/GraphicContext.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

GraphicContext::GraphicContext (opal::GraphicContextInterface & context)
:  _context (context)
,  _graphic_state_stack ()
{
   _graphic_state_stack.push_back (GraphicState ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

GraphicContext::~GraphicContext ()
{
   size_t stack_size = _graphic_state_stack.size ();
   
   assert (stack_size == 1);
}



/*
==============================================================================
Name : get
==============================================================================
*/

opal::GraphicContextInterface &  GraphicContext::get ()
{
   return _context;
}



/*
==============================================================================
Name : apply
==============================================================================
*/

void  GraphicContext::apply (opa::Point & pt, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply (pt, round_flag);
}



/*
==============================================================================
Name : apply
==============================================================================
*/

void  GraphicContext::apply (opa::Size & sz, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply (sz, round_flag);
}



/*
==============================================================================
Name : apply
==============================================================================
*/

void  GraphicContext::apply (opa::Rect & rect, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply (rect, round_flag);
}



/*
==============================================================================
Name: apply
==============================================================================
*/

void  GraphicContext::apply (opa::Quad & quad, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply (quad, round_flag);
}



/*
==============================================================================
Name : apply_inverse
==============================================================================
*/

void  GraphicContext::apply_inverse (opa::Point & pt, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply_inverse (pt, round_flag);
}



/*
==============================================================================
Name : apply_inverse
==============================================================================
*/

void  GraphicContext::apply_inverse (opa::Size & sz, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.apply_inverse (sz, round_flag);
}



/*
==============================================================================
Name : apply_inverse
==============================================================================
*/

void  GraphicContext::apply_inverse (opa::Rect & rect, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();

   graphic_state._affine_transform.apply_inverse (rect, round_flag);
}



/*
==============================================================================
Name: apply_inverse
==============================================================================
*/

void  GraphicContext::apply_inverse (opa::Quad & quad, bool round_flag) const
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();

   graphic_state._affine_transform.apply_inverse (quad, round_flag);
}



/*
==============================================================================
Name: get_inv_px
==============================================================================
*/

opa::Point  GraphicContext::get_inv_px ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.back ();
   
   return graphic_state.get_inv_px ();
}



/*
==============================================================================
Name : use_tranform
==============================================================================
*/

const opa::AffineTransform &  GraphicContext::use_transform ()
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.back ();
   
   return graphic_state._affine_transform;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

void  GraphicContext::save ()
{
   assert (_graphic_state_stack.size () > 0);
   
   _graphic_state_stack.push_back (_graphic_state_stack.back ());
   _aspect_stack.save ();
   _mark_stack.save ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

void  GraphicContext::restore ()
{
   assert (_graphic_state_stack.size () > 1);
   
   _graphic_state_stack.pop_back ();
   _aspect_stack.restore ();
   _mark_stack.restore ();
}



/*
==============================================================================
Name : translate
==============================================================================
*/

void  GraphicContext::translate (float tx, float ty)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.translate (tx, ty);
}



/*
==============================================================================
Name : translate
==============================================================================
*/

void  GraphicContext::translate (float & old_tx, float & old_ty, float tx, float ty)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.translate (old_tx, old_ty, tx, ty);
}



/*
==============================================================================
Name : override_translate
==============================================================================
*/

void  GraphicContext::override_translate (float tx, float ty)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.override_translate (tx, ty);
}



/*
==============================================================================
Name : scale
==============================================================================
*/

void  GraphicContext::scale (float sx, float sy)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.scale (sx, sy);
   
   graphic_state.invalidate_inv_px ();
}



/*
==============================================================================
Name : rotate
==============================================================================
*/

void  GraphicContext::rotate (float rot)
{
   if (rot != 0.f)   // very common case
   {
      assert (_graphic_state_stack.size () > 0);
      
      GraphicState & graphic_state = _graphic_state_stack.back ();
      
      graphic_state._affine_transform.rotate (rot);
   }
}



/*
==============================================================================
Name : mark
==============================================================================
*/

void  GraphicContext::mark (const std::type_info * info_ptr, archi::UInt32 mark_id)
{
   _mark_stack.set (info_ptr, mark_id);
}



/*
==============================================================================
Name : get_stack_size
==============================================================================
*/

size_t   GraphicContext::get_stack_size () const
{
   return _graphic_state_stack.size ();
}



/*
==============================================================================
Name : copy_stack
==============================================================================
*/

void  GraphicContext::copy_stack (GraphicContext & other)
{
   _graphic_state_stack = other._graphic_state_stack;
   _aspect_stack = other._aspect_stack;
   _mark_stack = other._mark_stack;
}



/*
==============================================================================
Name : reset_stack
==============================================================================
*/

void  GraphicContext::reset_stack ()
{
   _graphic_state_stack.clear ();
   _graphic_state_stack.push_back (GraphicState ());
   _aspect_stack.reset ();
   _mark_stack.reset ();
}



/*
==============================================================================
Name : reset_to_identity
==============================================================================
*/

void  GraphicContext::reset_to_identity ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.back ();
   
   graphic_state._affine_transform.reset ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
