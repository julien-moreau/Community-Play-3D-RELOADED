/*****************************************************************************

        HelperImage.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67739

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

#include "ohm/opak/base/HelperImage.h"
#include "ohm/opak/base/GraphicContext.h"
#include "ohm/opal/GraphicContextInterface.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : display_type_point
==============================================================================
*/

void  HelperImage::display_type_point (opak::GraphicContext & context)
{
   const float w = float (_image_ptr->_width);
   const float h = float (_image_ptr->_height);

   if (_scale_flag)
   {
      opa::Point pt_0 (0.f, 0.f);
      opa::Point pt_1 (w,   0.f);
      opa::Point pt_2 (w,   h);
      opa::Point pt_3 (0.f, h);

      context.save ();
      context.translate (_pt._x, _pt._y);
      
      context.apply (pt_0, _hint_flag);
      context.apply (pt_1, _hint_flag);
      context.apply (pt_2, _hint_flag);
      context.apply (pt_3, _hint_flag);
      
      context.restore ();
      
      context.get ().draw (
         *_image_ptr,
         opa::Quad (pt_0, pt_1, pt_2, pt_3),
         opa::Quad (*_image_ptr),
         _color
      );
   }
   else
   {
      opa::Point pt_00 (_pt);
      
      context.apply (pt_00);
      
      opa::Size vx (1.f, 0.f);
      opa::Size vy (0.f, 1.f);
      
      context.apply (vx);
      context.apply (vy);
      
#if 1
      // make vx norm w, vy norm h
      
      float invnvx = vx.inv_norm_2 () * w;
      float invnvy = vy.inv_norm_2 () * h;
      vx *= invnvx;
      vy *= invnvy;
      
      if (_hint_flag)
      {
         pt_00.nearest_integral ();
         vx.nearest_integral ();
         vy.nearest_integral ();
      }
      
      opa::Point pt_10 (pt_00 + vx);
      opa::Point pt_11 (pt_10 + vy);
      opa::Point pt_01 (pt_00 + vy);
      
#elif 0     // this is more like "absolute hint"
      // make unitary
      float invnvx = vx.inv_norm_2 ();
      float invnvy = vy.inv_norm_2 ();
      vx._width *= invnvx;
      vx._height *= invnvx;
      vy._width *= invnvy;
      vy._height *= invnvy;
      
      opa::Point pt_10 (pt_00);
      pt_10._x += w * vx._width;
      pt_10._y += w * vx._height;

      opa::Point pt_11 (pt_00);
      pt_11._x += w * vx._width + h * vy._width;
      pt_11._y += w * vx._height + h * vy._height;

      opa::Point pt_01 (pt_00);
      pt_01._x += h * vy._width;
      pt_01._y += h * vy._height;
      
      if (_hint_flag)
      {
         pt_00.nearest_integral ();
         pt_10.nearest_integral ();
         pt_11.nearest_integral ();
         pt_01.nearest_integral ();
      }
#endif
      
      context.get ().draw (
         *_image_ptr,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (*_image_ptr),
         _color
      );
   }
}



/*
==============================================================================
Name : display_type_rect
==============================================================================
*/

void  HelperImage::display_type_rect (opak::GraphicContext & context)
{
   if (_margined_flag)
   {
      const opa::Point inv_px = context.get_inv_px ();
      
      _rect._origin._x -= _margin._left * inv_px._x;
      _rect._origin._y -= _margin._top * inv_px._y;
      _rect._size._width += (_margin._left + _margin._right) * inv_px._x;
      _rect._size._height += (_margin._top + _margin._bottom) * inv_px._y;
   }
   
   const float w = _rect._size._width;
   const float h = _rect._size._height;

   opa::Point pt_0 (0.f, 0.f);
   opa::Point pt_1 (w,   0.f);
   opa::Point pt_2 (w,   h);
   opa::Point pt_3 (0.f, h);

   context.save ();
   context.translate (_rect._origin._x, _rect._origin._y);
   
   context.apply (pt_0, _hint_flag);
   context.apply (pt_1, _hint_flag);
   context.apply (pt_2, _hint_flag);
   context.apply (pt_3, _hint_flag);
   
   context.restore ();
   
   context.get ().draw (
      *_image_ptr,
      opa::Quad (pt_0, pt_1, pt_2, pt_3),
      opa::Quad (*_image_ptr),
      _color
   );
}



/*
==============================================================================
Name : display_type_rect_tex_rect
==============================================================================
*/

void  HelperImage::display_type_rect_tex_rect (opak::GraphicContext & context)
{
   if (_margined_flag)
   {
      const opa::Point inv_px = context.get_inv_px ();
      
      _rect._origin._x -= _margin._left * inv_px._x;
      _rect._origin._y -= _margin._top * inv_px._y;
      _rect._size._width += (_margin._left + _margin._right) * inv_px._x;
      _rect._size._height += (_margin._top + _margin._bottom) * inv_px._y;
   }
   
   const float w = _rect._size._width;
   const float h = _rect._size._height;

   opa::Point pt_0 (0.f, 0.f);
   opa::Point pt_1 (w,   0.f);
   opa::Point pt_2 (w,   h);
   opa::Point pt_3 (0.f, h);

   context.save ();
   context.translate (_rect._origin._x, _rect._origin._y);
   
   context.apply (pt_0, _hint_flag);
   context.apply (pt_1, _hint_flag);
   context.apply (pt_2, _hint_flag);
   context.apply (pt_3, _hint_flag);
   
   context.restore ();
   
   opa::Rect t_rect = _tex_rect;
   t_rect._origin._x *= _image_ptr->_tex_s;
   t_rect._size._width *= _image_ptr->_tex_s;
   t_rect._origin._y *= _image_ptr->_tex_t;
   t_rect._size._height *= _image_ptr->_tex_t;
   
   context.get ().draw (
      *_image_ptr,
      opa::Quad (pt_0, pt_1, pt_2, pt_3),
      opa::Quad (t_rect),
      _color
   );
}



/*
==============================================================================
Name : display_type_quad
==============================================================================
*/

void  HelperImage::display_type_quad (opak::GraphicContext & context)
{
   context.apply (_quad, _hint_flag);
   
   context.get ().draw (
      *_image_ptr,
      _quad,
      opa::Quad (*_image_ptr),
      _color
   );
}



/*
==============================================================================
Name : display_type_quad_tex_rect
==============================================================================
*/

void  HelperImage::display_type_quad_tex_rect (opak::GraphicContext & context)
{
   context.apply (_quad, _hint_flag);
   
   opa::Rect t_rect = _tex_rect;
   t_rect._origin._x *= _image_ptr->_tex_s;
   t_rect._size._width *= _image_ptr->_tex_s;
   t_rect._origin._y *= _image_ptr->_tex_t;
   t_rect._size._height *= _image_ptr->_tex_t;
   
   context.get ().draw (
      *_image_ptr,
      _quad,
      opa::Quad (t_rect),
      _color
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
