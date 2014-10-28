/*****************************************************************************

        LayerClip.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 41076

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

#include "ohm/opak/LayerClip.h"
#include "ohm/opal/GraphicContextInterface.h"

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

LayerClip::LayerClip ()
:  _clip_origin (*this, &LayerClip::clip_rect_changed)
,  _clip_size (*this, &LayerClip::clip_rect_changed)
{
}



/*
==============================================================================
Name : set_transition_time
==============================================================================
*/

void  LayerClip::set_transition_time (double delay_s)
{
   _clip_origin.set_transition_time (delay_s);
   _clip_size.set_transition_time (delay_s);
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LayerClip::display (GraphicContext & context)
{
   context.save ();
   
   add_mark (context);
   
   opa::Point origin = _origin.get_cur_value ();
   
   context.translate (origin._x, origin._y);
   
   opa::Rect clip_rect (_clip_origin.get_cur_value (), _clip_size.get_cur_value ());
   
   context.apply (clip_rect);
   
   opa::Rect cur_clip_rect = context.get ().get_clip_bounding_box ();
   
   if (cur_clip_rect.is_intersecting (clip_rect))
   {
      context.get ().save_graphic_state ();
      context.get ().clip (clip_rect);
      
      display_children (context);
      
      context.get ().restore_graphic_state ();
   }
   
   context.restore ();
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  LayerClip::invalidate (const opa::Rect & rect, const opa::Margin & margin)
{
   opa::Point clip_origin = _clip_origin.get_cur_value ();
   opa::Size clip_size = _clip_size.get_cur_value ();
   opa::Rect clip_rect (clip_origin, clip_size);
   
   opa::Rect clipped_rect = clip_rect & rect;
   
   Primitive::invalidate (clipped_rect, margin);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LayerClip::~LayerClip ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : clip_rect_changed
==============================================================================
*/

void  LayerClip::clip_rect_changed ()
{
   opa::Point clip_origin = _clip_origin.get_cur_value ();
   opa::Size clip_size = _clip_size.get_cur_value ();
   
   opa::Rect bbox (clip_origin, clip_size);
   
   set_bounding_box (bbox);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
