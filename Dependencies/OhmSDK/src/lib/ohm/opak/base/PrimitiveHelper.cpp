/*****************************************************************************

        PrimitiveHelper.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65630

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

#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opak/base/GraphicContext.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opa/Quad.h"
#include "ohm/opal/AreaString.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opal/AutoContextState.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opa::Rect & rect, const opal::Color & color, bool hinted_flag)
{
   // for now it is automatically hinted
   // but we would need to implement the antialiased version
   
   opa::Quad quad (rect);
   
   context.apply (quad);
   
   context.get ().save_graphic_state ();
   context.get ().set_fill_color (color);
   context.get ().fill (quad);
   context.get ().restore_graphic_state ();
}



/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::Image & image, const opa::Point & pt, bool scale_image_flag, bool hinted_flag)
{
   const float w = float (image._width);
   const float h = float (image._height);

   if (scale_image_flag)
   {
      opa::Point pt_0 (0.f, 0.f);
      opa::Point pt_1 (w,   0.f);
      opa::Point pt_2 (w,   h);
      opa::Point pt_3 (0.f, h);

      context.save ();
      context.translate (pt._x, pt._y);
      
      context.apply (pt_0, hinted_flag);
      context.apply (pt_1, hinted_flag);
      context.apply (pt_2, hinted_flag);
      context.apply (pt_3, hinted_flag);
      
      context.restore ();
      
      context.get ().draw (
         image,
         opa::Quad (pt_0, pt_1, pt_2, pt_3),
         opa::Quad (image)
      );
   }
   else
   {
      opa::Point pt_00 (pt);
      
      context.apply (pt_00);
      
      opa::Size vx (1.f, 0.f);
      opa::Size vy (0.f, 1.f);
      
      context.apply (vx);
      context.apply (vy);
      
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
      
      if (hinted_flag)
      {
         pt_00.nearest_integral ();
         pt_10.nearest_integral ();
         pt_11.nearest_integral ();
         pt_01.nearest_integral ();
      }
      
      context.get ().draw (
         image,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (image)
      );
   }
}



/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::Image & image, const opa::Rect & rect, bool hinted_flag)
{
   const float w = rect._size._width;
   const float h = rect._size._height;

   opa::Point pt_0 (0.f, 0.f);
   opa::Point pt_1 (w,   0.f);
   opa::Point pt_2 (w,   h);
   opa::Point pt_3 (0.f, h);

   context.save ();
   context.translate (rect._origin._x, rect._origin._y);
   
   context.apply (pt_0, hinted_flag);
   context.apply (pt_1, hinted_flag);
   context.apply (pt_2, hinted_flag);
   context.apply (pt_3, hinted_flag);
   
   context.restore ();
   
   context.get ().draw (
      image,
      opa::Quad (pt_0, pt_1, pt_2, pt_3),
      opa::Quad (image)
   );
}



/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::Image & image, const opa::Rect & rect, const opa::Rect & tex_rect, bool hinted_flag)
{
   const float w = rect._size._width;
   const float h = rect._size._height;

   opa::Point pt_0 (0.f, 0.f);
   opa::Point pt_1 (w,   0.f);
   opa::Point pt_2 (w,   h);
   opa::Point pt_3 (0.f, h);

   context.save ();
   context.translate (rect._origin._x, rect._origin._y);
   
   context.apply (pt_0, hinted_flag);
   context.apply (pt_1, hinted_flag);
   context.apply (pt_2, hinted_flag);
   context.apply (pt_3, hinted_flag);
   
   context.restore ();
   
   opa::Rect t_rect = tex_rect;
   t_rect._origin._x *= image._tex_s;
   t_rect._size._width *= image._tex_s;
   t_rect._origin._y *= image._tex_t;
   t_rect._size._height *= image._tex_t;
   
   context.get ().draw (
      image,
      opa::Quad (pt_0, pt_1, pt_2, pt_3),
      opa::Quad (t_rect)
   );
}



/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::Image & image, const opa::Quad & quad, bool hinted_flag)
{
   opa::Quad tquad = quad;
   
   context.apply (tquad);
   
   context.get ().draw (
      image,
      tquad,
      opa::Quad (image)
   );
}



/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::Image & mask, const opal::Color & color, const opa::Point & pt, bool scale_image_flag, bool hinted_flag)
{
   const float w = float (mask._width);
   const float h = float (mask._height);

   if (scale_image_flag)
   {
      opa::Point pt_0 (0.f, 0.f);
      opa::Point pt_1 (w,   0.f);
      opa::Point pt_2 (w,   h);
      opa::Point pt_3 (0.f, h);

      context.save ();
      context.translate (pt._x, pt._y);
      
      context.apply (pt_0, hinted_flag);
      context.apply (pt_1, hinted_flag);
      context.apply (pt_2, hinted_flag);
      context.apply (pt_3, hinted_flag);
      
      context.restore ();
      
      context.get ().draw (
         mask,
         opa::Quad (pt_0, pt_1, pt_2, pt_3),
         opa::Quad (mask),
         color
      );
   }
   else
   {
      opa::Point pt_00 (pt);
      
      context.apply (pt_00);
      
      opa::Size vx (1.f, 0.f);
      opa::Size vy (0.f, 1.f);
      
      context.apply (vx);
      context.apply (vy);
      
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
      
      if (hinted_flag)
      {
         pt_00.nearest_integral ();
         pt_10.nearest_integral ();
         pt_11.nearest_integral ();
         pt_01.nearest_integral ();
      }
      
      context.get ().draw (
         mask,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (mask),
         color
      );
   }
}




/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::Image & mask, const opal::Color & color, const opa::Rect & rect, bool hinted_flag)
{
   const float w = rect._size._width;
   const float h = rect._size._height;

   opa::Point pt_0 (0.f, 0.f);
   opa::Point pt_1 (w,   0.f);
   opa::Point pt_2 (w,   h);
   opa::Point pt_3 (0.f, h);

   context.save ();
   context.translate (rect._origin._x, rect._origin._y);
   
   context.apply (pt_0, hinted_flag);
   context.apply (pt_1, hinted_flag);
   context.apply (pt_2, hinted_flag);
   context.apply (pt_3, hinted_flag);
   
   context.restore ();
   
   context.get ().draw (
      mask,
      opa::Quad (pt_0, pt_1, pt_2, pt_3),
      opa::Quad (mask),
      color
   );
}



/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::SprungImage & sprung_image, const opa::Rect & rect, bool transform_borders_flag, bool hinted_flag)
{
   if (rect.is_empty ())
   {
      return;
   }
   
   // two modes : maintain borders or transform borders
   
   // for now we don't support repeat sprung mode
   // to do
   
   const opal::Image & im_00 = sprung_image (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_10 = sprung_image (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_20 = sprung_image (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_01 = sprung_image (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_11 = sprung_image (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_21 = sprung_image (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_02 = sprung_image (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_LAST
   );
   const opal::Image & im_12 = sprung_image (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_LAST
   );
   const opal::Image & im_22 = sprung_image (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_LAST
   );
   
   assert (im_11._sprung_mode_x == opal::SprungMode_DISTEND);
   assert (im_11._sprung_mode_y == opal::SprungMode_DISTEND);
   
   float       width_0 = float (
      sprung_image.get_col_width (opal::SprungImage::Position_FIRST)
   );
   float       width_2 = float (
      sprung_image.get_col_width (opal::SprungImage::Position_LAST)
   );

   float       height_0 = float (
      sprung_image.get_row_height (opal::SprungImage::Position_FIRST)
   );
   float       height_2 = float (
      sprung_image.get_row_height (opal::SprungImage::Position_LAST)
   );

   if (transform_borders_flag)
   {
      float width_1 = rect._size._width - (width_0 + width_2);
      float height_1 = rect._size._height - (height_0 + height_2);
      
      opa::Point pt_00 (0.f, 0.f);
      opa::Point pt_10 (width_0, 0.f);
      opa::Point pt_20 (width_0 + width_1, 0.f);
      opa::Point pt_30 (width_0 + width_1 + width_2, 0.f);
      opa::Point pt_01 (0.f, height_0);
      opa::Point pt_11 (width_0, height_0);
      opa::Point pt_21 (width_0 + width_1, height_0);
      opa::Point pt_31 (width_0 + width_1 + width_2, height_0);
      opa::Point pt_02 (0.f, height_0 + height_1);
      opa::Point pt_12 (width_0, height_0 + height_1);
      opa::Point pt_22 (width_0 + width_1, height_0 + height_1);
      opa::Point pt_32 (width_0 + width_1 + width_2, height_0 + height_1);
      opa::Point pt_03 (0.f, height_0 + height_1 + height_2);
      opa::Point pt_13 (width_0, height_0 + height_1 + height_2);
      opa::Point pt_23 (width_0 + width_1, height_0 + height_1 + height_2);
      opa::Point pt_33 (width_0 + width_1 + width_2, height_0 + height_1 + height_2);
      
      context.save ();
      context.translate (rect._origin._x, rect._origin._y);
      
      context.apply (pt_00, hinted_flag);
      context.apply (pt_10, hinted_flag);
      context.apply (pt_20, hinted_flag);
      context.apply (pt_30, hinted_flag);
      context.apply (pt_01, hinted_flag);
      context.apply (pt_11, hinted_flag);
      context.apply (pt_21, hinted_flag);
      context.apply (pt_31, hinted_flag);
      context.apply (pt_02, hinted_flag);
      context.apply (pt_12, hinted_flag);
      context.apply (pt_22, hinted_flag);
      context.apply (pt_32, hinted_flag);
      context.apply (pt_03, hinted_flag);
      context.apply (pt_13, hinted_flag);
      context.apply (pt_23, hinted_flag);
      context.apply (pt_33, hinted_flag);
      
      context.restore ();
      
      context.get ().draw (
         im_00,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (im_00)
      );
      
      context.get ().draw (
         im_10,
         opa::Quad (pt_10, pt_20, pt_21, pt_11),
         opa::Quad (im_10)
      );
      
      context.get ().draw (
         im_20,
         opa::Quad (pt_20, pt_30, pt_31, pt_21),
         opa::Quad (im_20)
      );
      
      context.get ().draw (
         im_01,
         opa::Quad (pt_01, pt_11, pt_12, pt_02),
         opa::Quad (im_01)
      );
      
      context.get ().draw (
         im_11,
         opa::Quad (pt_11, pt_21, pt_22, pt_12),
         opa::Quad (im_11)
      );
      
      context.get ().draw (
         im_21,
         opa::Quad (pt_21, pt_31, pt_32, pt_22),
         opa::Quad (im_21)
      );
      
      context.get ().draw (
         im_02,
         opa::Quad (pt_02, pt_12, pt_13, pt_03),
         opa::Quad (im_02)
      );
      
      context.get ().draw (
         im_12,
         opa::Quad (pt_12, pt_22, pt_23, pt_13),
         opa::Quad (im_12)
      );
      
      context.get ().draw (
         im_22,
         opa::Quad (pt_22, pt_32, pt_33, pt_23),
         opa::Quad (im_22)
      );
   }
   else
   {
      using namespace std;
      
      opa::Size vx (rect._size._width, 0.f);
      opa::Size vy (0.f, rect._size._height);
      
      context.apply (vx);
      context.apply (vy);
      
      float inv_nnvx = vx.inv_norm_2 ();
      float inv_nnvy = vy.inv_norm_2 ();
      
      opa::Size nvx (vx);
      opa::Size nvy (vy);
      
      nvx._width *= inv_nnvx;
      nvx._height *= inv_nnvx;

      nvy._width *= inv_nnvy;
      nvy._height *= inv_nnvy;
      
      opa::Point pt_00 (
         rect._origin._x,
         rect._origin._y
      );
      
      context.apply (pt_00);
      
      opa::Point pt_30 (
         pt_00._x + vx._width,
         pt_00._y + vx._height
      );
      opa::Point pt_03 (
         pt_00._x + vy._width,
         pt_00._y + vy._height
      );
      opa::Point pt_33 (
         pt_00._x + vx._width + vy._width,
         pt_00._y + vx._height + vy._height
      );
      
      if (hinted_flag)
      {
         pt_00.nearest_integral ();
         pt_30.nearest_integral ();
         pt_03.nearest_integral ();
         pt_33.nearest_integral ();
      }
      
      opa::Point pt_10 (
         pt_00._x + width_0 * nvx._width,
         pt_00._y + width_0 * nvx._height
      );

      opa::Point pt_20 (
         pt_30._x - width_2 * nvx._width,
         pt_30._y - width_2 * nvx._height
      );

      opa::Point pt_01 (
         pt_00._x + height_0 * nvy._width,
         pt_00._y + height_0 * nvy._height
      );

      opa::Point pt_11 (
         pt_10._x + height_0 * nvy._width,
         pt_10._y + height_0 * nvy._height
      );

      opa::Point pt_21 (
         pt_20._x + height_0 * nvy._width,
         pt_20._y + height_0 * nvy._height
      );

      opa::Point pt_31 (
         pt_30._x + height_0 * nvy._width,
         pt_30._y + height_0 * nvy._height
      );

      opa::Point pt_13 (
         pt_03._x + width_0 * nvx._width,
         pt_03._y + width_0 * nvx._height
      );

      opa::Point pt_23 (
         pt_33._x - width_2 * nvx._width,
         pt_33._y - width_2 * nvx._height
      );

      opa::Point pt_02 (
         pt_03._x - height_2 * nvy._width,
         pt_03._y - height_2 * nvy._height
      );

      opa::Point pt_12 (
         pt_13._x - height_2 * nvy._width,
         pt_13._y - height_2 * nvy._height
      );

      opa::Point pt_22 (
         pt_23._x - height_2 * nvy._width,
         pt_23._y - height_2 * nvy._height
      );

      opa::Point pt_32 (
         pt_33._x - height_2 * nvy._width,
         pt_33._y - height_2 * nvy._height
      );
      
      /*if (hinted_flag)
      {
         pt_00.nearest_integral ();
         pt_10.nearest_integral ();
         pt_20.nearest_integral ();
         pt_30.nearest_integral ();
         pt_01.nearest_integral ();
         pt_11.nearest_integral ();
         pt_21.nearest_integral ();
         pt_31.nearest_integral ();
         pt_02.nearest_integral ();
         pt_12.nearest_integral ();
         pt_22.nearest_integral ();
         pt_32.nearest_integral ();
         pt_03.nearest_integral ();
         pt_13.nearest_integral ();
         pt_23.nearest_integral ();
         pt_33.nearest_integral ();
      }*/
      
      context.get ().draw (
         im_00,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (im_00)
      );
      
      context.get ().draw (
         im_10,
         opa::Quad (pt_10, pt_20, pt_21, pt_11),
         opa::Quad (im_10)
      );
      
      context.get ().draw (
         im_20,
         opa::Quad (pt_20, pt_30, pt_31, pt_21),
         opa::Quad (im_20)
      );
      
      context.get ().draw (
         im_01,
         opa::Quad (pt_01, pt_11, pt_12, pt_02),
         opa::Quad (im_01)
      );
      
      context.get ().draw (
         im_11,
         opa::Quad (pt_11, pt_21, pt_22, pt_12),
         opa::Quad (im_11)
      );
      
      context.get ().draw (
         im_21,
         opa::Quad (pt_21, pt_31, pt_32, pt_22),
         opa::Quad (im_21)
      );
      
      context.get ().draw (
         im_02,
         opa::Quad (pt_02, pt_12, pt_13, pt_03),
         opa::Quad (im_02)
      );
      
      context.get ().draw (
         im_12,
         opa::Quad (pt_12, pt_22, pt_23, pt_13),
         opa::Quad (im_12)
      );
      
      context.get ().draw (
         im_22,
         opa::Quad (pt_22, pt_32, pt_33, pt_23),
         opa::Quad (im_22)
      );
   }
}



/*
==============================================================================
Name : display
Description :
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::SprungImage & sprung_mask, const opal::Color & color, const opa::Rect & rect, bool transform_borders_flag, bool hinted_flag)
{
   if (rect.is_empty ())
   {
      return;
   }
   
   // two modes : maintain borders or transform borders
   
   // for now we don't support repeat sprung mode
   // to do
   
   const opal::Image & im_00 = sprung_mask (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_10 = sprung_mask (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_20 = sprung_mask (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_01 = sprung_mask (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_11 = sprung_mask (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_21 = sprung_mask (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_02 = sprung_mask (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_LAST
   );
   const opal::Image & im_12 = sprung_mask (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_LAST
   );
   const opal::Image & im_22 = sprung_mask (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_LAST
   );
   
   assert (im_11._sprung_mode_x == opal::SprungMode_DISTEND);
   assert (im_11._sprung_mode_y == opal::SprungMode_DISTEND);
   
   float       width_0 = float (
      sprung_mask.get_col_width (opal::SprungImage::Position_FIRST)
   );
   float       width_2 = float (
      sprung_mask.get_col_width (opal::SprungImage::Position_LAST)
   );

   float       height_0 = float (
      sprung_mask.get_row_height (opal::SprungImage::Position_FIRST)
   );
   float       height_2 = float (
      sprung_mask.get_row_height (opal::SprungImage::Position_LAST)
   );

   if (transform_borders_flag)
   {
      float       width_1 = rect._size._width - (width_0 + width_2);
      float       height_1 = rect._size._height - (height_0 + height_2);
      
      opa::Point  pt_00 (0.f, 0.f);
      opa::Point  pt_10 (width_0, 0.f);
      opa::Point  pt_20 (width_0 + width_1, 0.f);
      opa::Point  pt_30 (width_0 + width_1 + width_2, 0.f);
      opa::Point  pt_01 (0.f, height_0);
      opa::Point  pt_11 (width_0, height_0);
      opa::Point  pt_21 (width_0 + width_1, height_0);
      opa::Point  pt_31 (width_0 + width_1 + width_2, height_0);
      opa::Point  pt_02 (0.f, height_0 + height_1);
      opa::Point  pt_12 (width_0, height_0 + height_1);
      opa::Point  pt_22 (width_0 + width_1, height_0 + height_1);
      opa::Point  pt_32 (width_0 + width_1 + width_2, height_0 + height_1);
      opa::Point  pt_03 (0.f, height_0 + height_1 + height_2);
      opa::Point  pt_13 (width_0, height_0 + height_1 + height_2);
      opa::Point  pt_23 (width_0 + width_1, height_0 + height_1 + height_2);
      opa::Point  pt_33 (width_0 + width_1 + width_2, height_0 + height_1 + height_2);
      
      context.save ();
      context.translate (rect._origin._x, rect._origin._y);
      
      context.apply (pt_00, hinted_flag);
      context.apply (pt_10, hinted_flag);
      context.apply (pt_20, hinted_flag);
      context.apply (pt_30, hinted_flag);
      context.apply (pt_01, hinted_flag);
      context.apply (pt_11, hinted_flag);
      context.apply (pt_21, hinted_flag);
      context.apply (pt_31, hinted_flag);
      context.apply (pt_02, hinted_flag);
      context.apply (pt_12, hinted_flag);
      context.apply (pt_22, hinted_flag);
      context.apply (pt_32, hinted_flag);
      context.apply (pt_03, hinted_flag);
      context.apply (pt_13, hinted_flag);
      context.apply (pt_23, hinted_flag);
      context.apply (pt_33, hinted_flag);
      
      context.restore ();
      
      context.get ().draw (
         im_00,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (im_00),
         color
      );
      
      context.get ().draw (
         im_10,
         opa::Quad (pt_10, pt_20, pt_21, pt_11),
         opa::Quad (im_10),
         color
      );
      
      context.get ().draw (
         im_20,
         opa::Quad (pt_20, pt_30, pt_31, pt_21),
         opa::Quad (im_20),
         color
      );
      
      context.get ().draw (
         im_01,
         opa::Quad (pt_01, pt_11, pt_12, pt_02),
         opa::Quad (im_01),
         color
      );
      
      context.get ().draw (
         im_11,
         opa::Quad (pt_11, pt_21, pt_22, pt_12),
         opa::Quad (im_11),
         color
      );
      
      context.get ().draw (
         im_21,
         opa::Quad (pt_21, pt_31, pt_32, pt_22),
         opa::Quad (im_21),
         color
      );
      
      context.get ().draw (
         im_02,
         opa::Quad (pt_02, pt_12, pt_13, pt_03),
         opa::Quad (im_02),
         color
      );
      
      context.get ().draw (
         im_12,
         opa::Quad (pt_12, pt_22, pt_23, pt_13),
         opa::Quad (im_12),
         color
      );
      
      context.get ().draw (
         im_22,
         opa::Quad (pt_22, pt_32, pt_33, pt_23),
         opa::Quad (im_22),
         color
      );
   }
   else
   {
      using namespace std;
      
      opa::Size vx (rect._size._width, 0.f);
      opa::Size vy (0.f, rect._size._height);
      
      context.apply (vx);
      context.apply (vy);
      
      float inv_nnvx = vx.inv_norm_2 ();
      float inv_nnvy = vy.inv_norm_2 ();
      
      opa::Size nvx (vx);
      opa::Size nvy (vy);
      
      nvx._width *= inv_nnvx;
      nvx._height *= inv_nnvx;

      nvy._width *= inv_nnvy;
      nvy._height *= inv_nnvy;
      
      opa::Point pt_00 (
         rect._origin._x,
         rect._origin._y
      );
      
      context.apply (pt_00);
      
      opa::Point pt_30 (
         pt_00._x + vx._width,
         pt_00._y + vx._height
      );
      opa::Point pt_03 (
         pt_00._x + vy._width,
         pt_00._y + vy._height
      );
      opa::Point pt_33 (
         pt_00._x + vx._width + vy._width,
         pt_00._y + vx._height + vy._height
      );
      
      opa::Point pt_10 (
         pt_00._x + width_0 * nvx._width,
         pt_00._y + width_0 * nvx._height
      );

      opa::Point pt_20 (
         pt_30._x - width_2 * nvx._width,
         pt_30._y - width_2 * nvx._height
      );

      opa::Point pt_01 (
         pt_00._x + height_0 * nvy._width,
         pt_00._y + height_0 * nvy._height
      );

      opa::Point pt_11 (
         pt_10._x + height_0 * nvy._width,
         pt_10._y + height_0 * nvy._height
      );

      opa::Point pt_21 (
         pt_20._x + height_0 * nvy._width,
         pt_20._y + height_0 * nvy._height
      );

      opa::Point pt_31 (
         pt_30._x + height_0 * nvy._width,
         pt_30._y + height_0 * nvy._height
      );

      opa::Point pt_13 (
         pt_03._x + width_0 * nvx._width,
         pt_03._y + width_0 * nvx._height
      );

      opa::Point pt_23 (
         pt_33._x - width_2 * nvx._width,
         pt_33._y - width_2 * nvx._height
      );

      opa::Point pt_02 (
         pt_03._x - height_2 * nvy._width,
         pt_03._y - height_2 * nvy._height
      );

      opa::Point pt_12 (
         pt_13._x - height_2 * nvy._width,
         pt_13._y - height_2 * nvy._height
      );

      opa::Point pt_22 (
         pt_23._x - height_2 * nvy._width,
         pt_23._y - height_2 * nvy._height
      );

      opa::Point pt_32 (
         pt_33._x - height_2 * nvy._width,
         pt_33._y - height_2 * nvy._height
      );
      
      if (hinted_flag)
      {
         pt_00.nearest_integral ();
         pt_10.nearest_integral ();
         pt_20.nearest_integral ();
         pt_30.nearest_integral ();
         pt_01.nearest_integral ();
         pt_11.nearest_integral ();
         pt_21.nearest_integral ();
         pt_31.nearest_integral ();
         pt_02.nearest_integral ();
         pt_12.nearest_integral ();
         pt_22.nearest_integral ();
         pt_32.nearest_integral ();
         pt_03.nearest_integral ();
         pt_13.nearest_integral ();
         pt_23.nearest_integral ();
         pt_33.nearest_integral ();
      }

      context.get ().draw (
         im_00,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (im_00),
         color
      );
      
      context.get ().draw (
         im_10,
         opa::Quad (pt_10, pt_20, pt_21, pt_11),
         opa::Quad (im_10),
         color
      );
      
      context.get ().draw (
         im_20,
         opa::Quad (pt_20, pt_30, pt_31, pt_21),
         opa::Quad (im_20),
         color
      );
      
      context.get ().draw (
         im_01,
         opa::Quad (pt_01, pt_11, pt_12, pt_02),
         opa::Quad (im_01),
         color
      );
      
      context.get ().draw (
         im_11,
         opa::Quad (pt_11, pt_21, pt_22, pt_12),
         opa::Quad (im_11),
         color
      );
      
      context.get ().draw (
         im_21,
         opa::Quad (pt_21, pt_31, pt_32, pt_22),
         opa::Quad (im_21),
         color
      );
      
      context.get ().draw (
         im_02,
         opa::Quad (pt_02, pt_12, pt_13, pt_03),
         opa::Quad (im_02),
         color
      );
      
      context.get ().draw (
         im_12,
         opa::Quad (pt_12, pt_22, pt_23, pt_13),
         opa::Quad (im_12),
         color
      );
      
      context.get ().draw (
         im_22,
         opa::Quad (pt_22, pt_32, pt_33, pt_23),
         opa::Quad (im_22),
         color
      );
   }
}



/*
==============================================================================
Name : display
Note :
   We always try to hint text
==============================================================================
*/

void  PrimitiveHelper::display (opak::GraphicContext & context, const opal::LabelString & label_str, const opa::Point & pt, float flush_factor)
{
   const opa::Rect & typographic_bounds = label_str.get_typographic_bounds ();
   const opa::Point & offset = typographic_bounds._origin;
   
   const opa::Point inv_px = context.get_inv_px ();
   
   opa::Point rev_offset = offset;
   rev_offset._x *= inv_px._x;
   rev_offset._y *= inv_px._y;
   
   opa::Point image_pt = pt;
   image_pt += rev_offset;
   
   float flush_offset = - floor (flush_factor * typographic_bounds._size._width);
   
   float rev_flush_offset = flush_offset * inv_px._x;
   
   image_pt += opa::Point (rev_flush_offset, 0.f);
   
   const opa::AffineTransform & aff_transform = context.use_transform ();
   
   if (aff_transform.is_translation_hint ())
   {
      opa::Point pt_0 (0.f, 0.f);
      
      image_pt -= typographic_bounds._origin;
      
      context.save ();
      context.translate (image_pt._x, image_pt._y);
      
      bool hinted_flag = true;
      context.apply (pt_0, hinted_flag);
      
      context.restore ();
      
      context.get ().save_graphic_state ();
      context.get ().concat_translate (pt_0._x, pt_0._y);
      
      label_str.draw (context.get ());
      context.get ().restore_graphic_state ();
   }
   else
   {
      opal::Cache cache (
         context.get (), opal::SprungMode_DISTEND, opal::SprungMode_DISTEND,
         opal::FilterMode_LINEAR
      );
      
      cache.resize (typographic_bounds._size);
      
      cache.invalidate_all ();
      
      if (cache)
      {
         opal::AutoContextState  auto_context_state (cache);
            
         cache.clip (cache.get_invalid_rect ());
         cache.clear ();
         
         cache.concat_translate (
            - typographic_bounds._origin._x,
            - typographic_bounds._origin._y
         );
         
         label_str.draw (cache);
         
         cache.validate ();
      }
      
      opal::Image cache_image = cache.get_image ();
      
      bool scale_image_flag = false;
      bool hinted_flag = true;
      
      PrimitiveHelper::display (
         context, cache_image, image_pt, scale_image_flag, hinted_flag
      );
   }
}



/*
==============================================================================
Name : display
Note :
   We always try to hint text
==============================================================================
*/

bool  PrimitiveHelper::display (opak::GraphicContext & context, const opal::AreaString & aera_str, const opa::Point & pt)
{
   const opa::Point & image_offset = aera_str.get_image_bounds ()._origin;
   
   opa::Point image_pt = pt;
   image_pt += image_offset;
   
   opa::Point pt_0 (0.f, 0.f);

   context.save ();
   context.translate (image_pt._x, image_pt._y);
   
   bool hinted_flag = true;
   context.apply (pt_0, hinted_flag);

   context.restore ();
   
   context.get ().save_graphic_state ();
   context.get ().concat_translate (pt_0._x, pt_0._y);
   
   bool cropped_flag = aera_str.draw_text (context.get ());
   context.get ().restore_graphic_state ();
   
   return cropped_flag;
}



/*
==============================================================================
Name : display
Note :
   We always try to hint text
==============================================================================
*/

bool  PrimitiveHelper::display (opak::GraphicContext & context, const opal::AreaString & aera_str, const opa::Point & pt, size_t caret_first, size_t caret_second, const opal::Color & caret_color, const opal::Color & selection_color)
{
   const opa::Point & image_offset = aera_str.get_image_bounds ()._origin;
   
   opa::Point image_pt = pt;
   image_pt += image_offset;
   
   opa::Point pt_0 (0.f, 0.f);

   context.save ();
   context.translate (image_pt._x, image_pt._y);
   
   bool hinted_flag = true;
   context.apply (pt_0, hinted_flag);

   context.restore ();
   
   context.get ().save_graphic_state ();
   context.get ().concat_translate (pt_0._x, pt_0._y);
   
   aera_str.draw_selection (context.get (), caret_first, caret_second, selection_color);
   bool cropped_flag = aera_str.draw_text (context.get ());
   aera_str.draw_caret (context.get (), caret_first, caret_second, caret_color);
   context.get ().restore_graphic_state ();
   
   return cropped_flag;
}



/*
==============================================================================
Name : display_line
==============================================================================
*/

void  PrimitiveHelper::display_line (opak::GraphicContext & context, const opal::Image & line_image, const opa::Point & pt_0, const opa::Point & pt_1, float & phase, bool hinted_flag)
{
   display_line (context, line_image, opal::Color::_white, pt_0, pt_1, phase, hinted_flag);
}



/*
==============================================================================
Name : display_line
==============================================================================
*/

void  PrimitiveHelper::display_line (opak::GraphicContext & context, const opal::Image & line_mask, const opal::Color & color, const opa::Point & pt_0, const opa::Point & pt_1, float & phase, bool hinted_flag)
{
   if (pt_0 != pt_1)
   {
      const float size = float (line_mask._height);
      const float half_sizem1 = (size - 1.f) * .5f;
      
      opa::Point tpt0 = pt_0;
      opa::Point tpt1 = pt_1;
   
      context.apply (tpt0);
      context.apply (tpt1);
      
      if (tpt0 != tpt1)
      {
         opa::Size vec (tpt1._x - tpt0._x, tpt1._y - tpt0._y);
      
         opa::Size vecn;
         vecn._height = - vec._width;
         vecn._width = vec._height;
         
         float inv_norm2 = vecn.inv_norm_2 ();
         
         vecn._width *= inv_norm2;
         vecn._height *= inv_norm2;
         
         opa::Point pt00 = tpt0 - half_sizem1 * vecn;
         opa::Point pt10 = tpt1 - half_sizem1 * vecn;
         
         if (hinted_flag)
         {
            pt00.nearest_integral ();
            pt10.nearest_integral ();
         }
         
         opa::Size dpt (
            pt10._x - pt00._x,
            pt10._y - pt00._y
         );
         
         const float mul = dpt.norm_2 () / float (line_mask._width);
         
         opa::Point pt11 = pt10 + size * vecn;
         opa::Point pt01 = pt00 + size * vecn;
         
         // warning : dist might be too big, sometimes
         
         float off_s = line_mask._tex_s * phase;
         float tex_s = line_mask._tex_s * mul;
         
         phase += mul;
         
         opa::Point tex00 (off_s, 0);
         opa::Point tex10 (off_s + tex_s, 0);
         opa::Point tex11 (off_s + tex_s, line_mask._tex_t);
         opa::Point tex01 (off_s, line_mask._tex_t);
         
         opa::Quad tex (tex00, tex10, tex11, tex01);
         
         context.get ().draw (
            line_mask,
            opa::Quad (pt00, pt10, pt11, pt01),
            tex,
            color
         );
      }
   }
}



/*
==============================================================================
Name : display_point
==============================================================================
*/

void  PrimitiveHelper::display_point (opak::GraphicContext & context, const opal::Image & pt_image, const opa::Point & pt, bool hinted_flag)
{
   display_point (context, pt_image, opal::Color::_white, pt, hinted_flag);
}



/*
==============================================================================
Name : display_point
==============================================================================
*/

void  PrimitiveHelper::display_point (opak::GraphicContext & context, const opal::Image & pt_mask, const opal::Color & color, const opa::Point & pt, bool hinted_flag)
{
   const float size = float (pt_mask._height);
   const float half_sizem1 = (size - 1.f) * .5f;
   
   opa::Point tpt = pt;
   
   context.apply (tpt);
   
   opa::Point pt00 = tpt + opa::Point (- half_sizem1, - half_sizem1);

   if (hinted_flag)
   {
      pt00.nearest_integral ();
   }
   
   opa::Point pt10 = pt00 + opa::Point (size, 0.f);
   opa::Point pt11 = pt00 + opa::Point (size, size);
   opa::Point pt01 = pt00 + opa::Point (0.f, size);
   
   context.get ().draw (
      pt_mask,
      opa::Quad (pt00, pt10, pt11, pt01),
      opa::Quad (pt_mask),
      color
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PrimitiveHelper::PrimitiveHelper ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimitiveHelper::~PrimitiveHelper ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
