/*****************************************************************************

        PrimUnscale.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 41075

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

PrimUnscale::PrimUnscale ()
:  _scale_x (1.f)
,  _scale_y (1.f)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimUnscale::display (opak::GraphicContext & context)
{
   context.save ();
   
   add_mark (context);
   
   opa::Point origin = _origin.get_cur_value ();
   
   context.translate (origin._x, origin._y);
   
   opa::Size px_vec_x (1.f, 0.f);
   context.apply (px_vec_x);
   _scale_x = px_vec_x.inv_norm_2 ();

   opa::Size px_vec_y (0.f, 1.f);
   context.apply (px_vec_y);
   _scale_y = px_vec_y.inv_norm_2 ();
   
   context.scale (_scale_x, _scale_y);
   
   display_children (context);
   
   context.restore ();
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  PrimUnscale::invalidate (const opa::Rect & rect, const opa::Margin & margin)
{
   ParentPtrArr::const_iterator it = get_parent_ptr_arr ().begin ();
   const ParentPtrArr::const_iterator it_end = get_parent_ptr_arr ().end ();
   
   opa::Rect parent_rel_rect = rect;

   parent_rel_rect._origin._x *= _scale_x;
   parent_rel_rect._origin._y *= _scale_y;
   parent_rel_rect._size._width *= _scale_x;
   parent_rel_rect._size._height *= _scale_y;
   
   parent_rel_rect += _origin.get_cur_value ();
   
   for (; it != it_end ; ++it)
   {
      Primitive * parent_ptr = *it;
      
      parent_ptr->invalidate (parent_rel_rect, margin);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimUnscale::~PrimUnscale ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
