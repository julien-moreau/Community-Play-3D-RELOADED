/*****************************************************************************

        HelperSprungImage.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67460

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

#include "ohm/opak/base/HelperSprungImage.h"
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
Name : display
==============================================================================
*/

void  HelperSprungImage::display (opak::GraphicContext & context)
{
   if (_margined_flag)
   {
      const opa::Point inv_px = context.get_inv_px ();
      
      _rect._origin._x -= _margin._left * inv_px._x;
      _rect._origin._y -= _margin._top * inv_px._y;
      _rect._size._width += (_margin._left + _margin._right) * inv_px._x;
      _rect._size._height += (_margin._top + _margin._bottom) * inv_px._y;
   }
   
   if (_rect.is_empty ())
   {
      return;
   }
   
   // two modes : maintain borders or transform borders
   
   // for now we don't support repeat sprung mode
   // to do
   
   const opal::Image & im_00 = (*_image_ptr) (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_10 = (*_image_ptr) (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_20 = (*_image_ptr) (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_FIRST
   );
   const opal::Image & im_01 = (*_image_ptr) (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_11 = (*_image_ptr) (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_21 = (*_image_ptr) (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_MIDDLE
   );
   const opal::Image & im_02 = (*_image_ptr) (
      opal::SprungImage::Position_FIRST, opal::SprungImage::Position_LAST
   );
   const opal::Image & im_12 = (*_image_ptr) (
      opal::SprungImage::Position_MIDDLE, opal::SprungImage::Position_LAST
   );
   const opal::Image & im_22 = (*_image_ptr) (
      opal::SprungImage::Position_LAST, opal::SprungImage::Position_LAST
   );
   
   assert (im_11._sprung_mode_x == opal::SprungMode_DISTEND);
   assert (im_11._sprung_mode_y == opal::SprungMode_DISTEND);
   
   float       width_0 = float (
      _image_ptr->get_col_width (opal::SprungImage::Position_FIRST)
   );
   float       width_2 = float (
      _image_ptr->get_col_width (opal::SprungImage::Position_LAST)
   );

   float       height_0 = float (
      _image_ptr->get_row_height (opal::SprungImage::Position_FIRST)
   );
   float       height_2 = float (
      _image_ptr->get_row_height (opal::SprungImage::Position_LAST)
   );

   if (_scale_border_flag)
   {
      float width_1 = _rect._size._width - (width_0 + width_2);
      float height_1 = _rect._size._height - (height_0 + height_2);
      
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
      context.translate (_rect._origin._x, _rect._origin._y);
      
      context.apply (pt_00, _hint_flag);
      context.apply (pt_10, _hint_flag);
      context.apply (pt_20, _hint_flag);
      context.apply (pt_30, _hint_flag);
      context.apply (pt_01, _hint_flag);
      context.apply (pt_11, _hint_flag);
      context.apply (pt_21, _hint_flag);
      context.apply (pt_31, _hint_flag);
      context.apply (pt_02, _hint_flag);
      context.apply (pt_12, _hint_flag);
      context.apply (pt_22, _hint_flag);
      context.apply (pt_32, _hint_flag);
      context.apply (pt_03, _hint_flag);
      context.apply (pt_13, _hint_flag);
      context.apply (pt_23, _hint_flag);
      context.apply (pt_33, _hint_flag);
      
      context.restore ();
      
      context.get ().draw (
         im_00,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (im_00), _color
      );
      
      context.get ().draw (
         im_10,
         opa::Quad (pt_10, pt_20, pt_21, pt_11),
         opa::Quad (im_10), _color
      );
      
      context.get ().draw (
         im_20,
         opa::Quad (pt_20, pt_30, pt_31, pt_21),
         opa::Quad (im_20), _color
      );
      
      context.get ().draw (
         im_01,
         opa::Quad (pt_01, pt_11, pt_12, pt_02),
         opa::Quad (im_01), _color
      );
      
      context.get ().draw (
         im_11,
         opa::Quad (pt_11, pt_21, pt_22, pt_12),
         opa::Quad (im_11), _color
      );
      
      context.get ().draw (
         im_21,
         opa::Quad (pt_21, pt_31, pt_32, pt_22),
         opa::Quad (im_21), _color
      );
      
      context.get ().draw (
         im_02,
         opa::Quad (pt_02, pt_12, pt_13, pt_03),
         opa::Quad (im_02), _color
      );
      
      context.get ().draw (
         im_12,
         opa::Quad (pt_12, pt_22, pt_23, pt_13),
         opa::Quad (im_12), _color
      );
      
      context.get ().draw (
         im_22,
         opa::Quad (pt_22, pt_32, pt_33, pt_23),
         opa::Quad (im_22), _color
      );
   }
   else
   {
      using namespace std;

#if 1    
      opa::Point pt_00 (
         _rect._origin._x,
         _rect._origin._y
      );
      
      opa::Point pt_30;
      opa::Point pt_03;
      opa::Point pt_33;
      
      opa::Size vx (_rect._size._width, 0.f);
      opa::Size vy (0.f, _rect._size._height);

      opa::Size nvx;
      opa::Size nvy;
      
      if (_hint_flag)
      {
         if (_hint_absolute_flag)
         {
            // preserve positions
            
            pt_30 = opa::Point (
               pt_00._x + vx._width,
               pt_00._y + vx._height
            );
            pt_03 = opa::Point (
               pt_00._x + vy._width,
               pt_00._y + vy._height
            );
            pt_33 = opa::Point (
               pt_00._x + vx._width + vy._width,
               pt_00._y + vx._height + vy._height
            );
            
            context.apply (pt_00, true);
            context.apply (pt_30, true);
            context.apply (pt_03, true);
            context.apply (pt_33, true);
            
            context.apply (vx);
            context.apply (vy);
            
            float inv_nnvx = vx.inv_norm_2 ();
            float inv_nnvy = vy.inv_norm_2 ();
            
            nvx = opa::Size (vx);
            nvy = opa::Size (vy);
            
            nvx._width *= inv_nnvx;
            nvx._height *= inv_nnvx;

            nvy._width *= inv_nnvy;
            nvy._height *= inv_nnvy;
         }
         else
         {
            // preserve bounds
            
            context.apply (vx);
            context.apply (vy);
            
            float inv_nnvx = vx.inv_norm_2 ();
            float inv_nnvy = vy.inv_norm_2 ();
            
            nvx = opa::Size (vx);
            nvy = opa::Size (vy);
            
            nvx._width *= inv_nnvx;
            nvx._height *= inv_nnvx;

            nvy._width *= inv_nnvy;
            nvy._height *= inv_nnvy;
            
            context.apply (pt_00, true);

            vx.nearest_integral ();
            vy.nearest_integral ();

            pt_30 = opa::Point (
               pt_00._x + vx._width,
               pt_00._y + vx._height
            );
            pt_03 = opa::Point (
               pt_00._x + vy._width,
               pt_00._y + vy._height
            );
            pt_33 = opa::Point (
               pt_00._x + vx._width + vy._width,
               pt_00._y + vx._height + vy._height
            );
         }

         nvx.nearest_integral ();
         nvy.nearest_integral ();
      }
      else
      {
         context.apply (vx);
         context.apply (vy);
         
         float inv_nnvx = vx.inv_norm_2 ();
         float inv_nnvy = vy.inv_norm_2 ();
         
         nvx = opa::Size (vx);
         nvy = opa::Size (vy);
         
         nvx._width *= inv_nnvx;
         nvx._height *= inv_nnvx;

         nvy._width *= inv_nnvy;
         nvy._height *= inv_nnvy;
         
         context.apply (pt_00);

         pt_30 = opa::Point (
            pt_00._x + vx._width,
            pt_00._y + vx._height
         );
         pt_03 = opa::Point (
            pt_00._x + vy._width,
            pt_00._y + vy._height
         );
         pt_33 = opa::Point (
            pt_00._x + vx._width + vy._width,
            pt_00._y + vx._height + vy._height
         );
      }

#elif 0     
      opa::Size vx (_rect._size._width, 0.f);
      opa::Size vy (0.f, _rect._size._height);
      
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
         _rect._origin._x,
         _rect._origin._y
      );
      
      context.apply (pt_00, _hint_flag);

      if (_hint_flag && !_hint_absolute_flag)
      {
         vx.nearest_integral ();
         vy.nearest_integral ();
         nvx.nearest_integral ();
         nvy.nearest_integral ();
      }
      
      // outer points
      
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

      if (_hint_flag && _hint_absolute_flag)
      {
         pt_30.nearest_integral ();
         pt_03.nearest_integral ();
         pt_33.nearest_integral ();
      }
#endif
      
      // inner points
      
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
      
      // draw
      
      context.get ().draw (
         im_00,
         opa::Quad (pt_00, pt_10, pt_11, pt_01),
         opa::Quad (im_00), _color
      );
      
      context.get ().draw (
         im_10,
         opa::Quad (pt_10, pt_20, pt_21, pt_11),
         opa::Quad (im_10), _color
      );
      
      context.get ().draw (
         im_20,
         opa::Quad (pt_20, pt_30, pt_31, pt_21),
         opa::Quad (im_20), _color
      );
      
      context.get ().draw (
         im_01,
         opa::Quad (pt_01, pt_11, pt_12, pt_02),
         opa::Quad (im_01), _color
      );
      
      context.get ().draw (
         im_11,
         opa::Quad (pt_11, pt_21, pt_22, pt_12),
         opa::Quad (im_11), _color
      );
      
      context.get ().draw (
         im_21,
         opa::Quad (pt_21, pt_31, pt_32, pt_22),
         opa::Quad (im_21), _color
      );
      
      context.get ().draw (
         im_02,
         opa::Quad (pt_02, pt_12, pt_13, pt_03),
         opa::Quad (im_02), _color
      );
      
      context.get ().draw (
         im_12,
         opa::Quad (pt_12, pt_22, pt_23, pt_13),
         opa::Quad (im_12), _color
      );
      
      context.get ().draw (
         im_22,
         opa::Quad (pt_22, pt_32, pt_33, pt_23),
         opa::Quad (im_22), _color
      );
   }  
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
