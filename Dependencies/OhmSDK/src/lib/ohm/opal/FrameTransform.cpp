/*****************************************************************************

        FrameTransform.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70085

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

#include "ohm/opal/FrameTransform.h"
#include "ohm/util/trace.h"
#include "ohm/math/fnc.h"

#include <cassert>



namespace ohm
{
namespace opal
{


/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

FrameTransform::FrameTransform (const char * name_0)
:  Frame (name_0)
,  _scale_x (1.f)
,  _scale_y (1.f)
,  _rotation (0.f)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FrameTransform::~FrameTransform ()
{
   try
   {
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("Uncatched Exception");
      assert (false);
   }
}



/*
==============================================================================
Name : set_scale
==============================================================================
*/

void  FrameTransform::set_scale (float sx, float sy)
{
   assert (sx != 0.f);
   assert (sy != 0.f);
   assert (math::is_finite (sx));
   assert (math::is_finite (sy));
   
   _scale_x = sx;
   _scale_y = sy;
}



/*
==============================================================================
Name : set_scale_x
==============================================================================
*/

void  FrameTransform::set_scale_x (float scale)
{
   assert (scale != 0.f);
   assert (math::is_finite (scale));
   
   _scale_x = scale;
}



/*
==============================================================================
Name : set_scale_x
==============================================================================
*/

void  FrameTransform::set_scale_y (float scale)
{
   assert (scale != 0.f);
   assert (math::is_finite (scale));
   
   _scale_y = scale;
}



/*
==============================================================================
Name : set_scale
==============================================================================
*/

float FrameTransform::get_scale_x () const
{
   return _scale_x;
}



/*
==============================================================================
Name : set_scale
==============================================================================
*/

float FrameTransform::get_scale_y () const
{
   return _scale_y;
}



/*
==============================================================================
Name : set_rotation
==============================================================================
*/

void  FrameTransform::set_rotation (float rot)
{
   _rotation = rot;
}



/*
==============================================================================
Name : get_rotation
==============================================================================
*/

float FrameTransform::get_rotation () const
{
   return _rotation;
}



/*
==============================================================================
Name : push_child_transform
Description :
   Change event to child coords, but with translation relative to the
   current frame.
   WARNING : must be balanced with a 'pop_child_transform' call
==============================================================================
*/

void  FrameTransform::push_child_transform (Event & event)
{
   event.save ();
   event.rotate (_rotation);
   event.scale (_scale_x, _scale_y);
}



/*
==============================================================================
Name : pop_child_transform
==============================================================================
*/

void  FrameTransform::pop_child_transform (Event & event)
{
   event.restore ();
}



/*
==============================================================================
Name : propagate_children
==============================================================================
*/

void  FrameTransform::propagate_children (Event & event)
{
   event.save ();
   event.rotate (_rotation);
   event.scale (_scale_x, _scale_y);
   
   Frame::propagate_children (event);

   event.restore ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
