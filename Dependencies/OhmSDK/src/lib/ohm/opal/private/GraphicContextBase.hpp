/*****************************************************************************

        GraphicContextBase.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70432

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



#if defined (ohm_opal_GraphicContextBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of GraphicContextBase code header.
#endif
#define  ohm_opal_GraphicContextBase_CURRENT_CODEHEADER

#if ! defined (ohm_opal_GraphicContextBase_CODEHEADER_INCLUDED)
#define  ohm_opal_GraphicContextBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Image.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opa/Quad.h"
#include "ohm/math/fnc.h"



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : draw
Description :
   Draw image.
   Check to see if we can use hardware sampling repetition
==============================================================================
*/

template <class T>
void  GraphicContextBase::draw (const GraphicState & graphic_state, const Image & image_or_mask, const opa::Rect & rect, const T & alpha_or_color)
{
   assert (_graphic_state_stack.size () > 0);
   assert (!image_or_mask.empty ());
   
   if (image_or_mask._software_wrap_repeat_x_flag || image_or_mask._software_wrap_repeat_y_flag)
   {
      draw_software_repeat (graphic_state, image_or_mask, rect, alpha_or_color);
   }
   else
   {
      draw_hardware_repeat (graphic_state, image_or_mask, rect, alpha_or_color);
   }
}



/*
==============================================================================
Name : draw_hardware_repeat
Description :
   Use sampler repetition capabilities in hardware.
   This can be used either :
   - when the hardware supports NPOT & normalized coords
   - or when the hardware supports POT & & normalized coords, but it happens
     that the displayed image has POT width & height
==============================================================================
*/

template <class T>
void  GraphicContextBase::draw_hardware_repeat (const GraphicState & graphic_state, const Image & image_or_mask, const opa::Rect & rect, const T & alpha_or_color)
{
   assert (_graphic_state_stack.size () > 0);
   assert (!image_or_mask.empty ());
   assert (image_or_mask._width > 0);
   assert (image_or_mask._height > 0);
   
   float tex_s_mul = 1.f;  // distend
   float tex_t_mul = 1.f;
   
   if (image_or_mask._sprung_mode_x == SprungMode_REPEAT)
   {
      assert (!image_or_mask._software_wrap_repeat_x_flag);
      tex_s_mul = rect._size._width / float (image_or_mask._width);
   }
   
   if (image_or_mask._sprung_mode_y == SprungMode_REPEAT)
   {
      assert (!image_or_mask._software_wrap_repeat_y_flag);
      tex_t_mul = rect._size._height / float (image_or_mask._height);
   }
   
   opa::Quad vertices (rect);
   opa::Quad tex_coords (opa::Rect (
      opa::Point::_zero,
      opa::Size (
         image_or_mask._tex_s * tex_s_mul,
         image_or_mask._tex_t * tex_t_mul
      )
   ));
   
   do_update_context (graphic_state);
   
   do_draw (image_or_mask, vertices, tex_coords, alpha_or_color);
}



/*
==============================================================================
Name : draw_software_repeat
Description :
   Emulate texture repetition in software by issuing all the commands for
   each needed repetition
==============================================================================
*/

template <class T>
void  GraphicContextBase::draw_software_repeat (const GraphicState & graphic_state, const Image & image_or_mask, const opa::Rect & rect, const T & alpha_or_color)
{
   assert (_graphic_state_stack.size () > 0);
   assert (!image_or_mask.empty ());
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   do_update_context (graphic_state);
   
   if ((image_or_mask._sprung_mode_x == SprungMode_DISTEND)
      && (image_or_mask._sprung_mode_y == SprungMode_DISTEND))
   {
      opa::Quad quad (rect);
      
      do_draw (image_or_mask, opa::Quad (rect), opa::Quad (image_or_mask), alpha_or_color);
   }
   else if ((image_or_mask._sprung_mode_x == SprungMode_DISTEND)
      && (image_or_mask._sprung_mode_y == SprungMode_REPEAT))
   {
      opa::Rect cur_visible_rect = rect & local_clip_rect;
      
      int loop_y = math::ceil_int (rect._size._height / image_or_mask._height);
      
      opa::Rect image_rect = rect;
      image_rect._size._height = float (image_or_mask._height);
      
      opa::Quad tex_coords (image_or_mask);

      for (int y = 0 ; y < loop_y ; ++y)
      {
         opa::Quad quad (image_rect);
         
         do_draw (image_or_mask, quad, tex_coords, alpha_or_color);
         
         image_rect._origin._y += image_rect._size._height;
      }
   }
   else if ((image_or_mask._sprung_mode_x == SprungMode_REPEAT)
      && (image_or_mask._sprung_mode_y == SprungMode_DISTEND))
   {
      opa::Rect cur_visible_rect = rect & local_clip_rect;
      
      int loop_x = math::ceil_int (rect._size._width / image_or_mask._width);

      opa::Rect image_rect = rect;
      image_rect._size._width = float (image_or_mask._width);
      
      opa::Quad tex_coords (image_or_mask);

      for (int x = 0 ; x < loop_x ; ++x)
      {
         opa::Quad quad (image_rect);
         
         do_draw (image_or_mask, quad, tex_coords, alpha_or_color);
         
         image_rect._origin._x += image_rect._size._width;
      }
   }
   else  // SprungMode_REPEAT, SprungMode_REPEAT
   {
      opa::Rect cur_visible_rect = rect & local_clip_rect;
      
      int loop_y = math::ceil_int (rect._size._height / image_or_mask._height);
      int loop_x = math::ceil_int (rect._size._width / image_or_mask._width);

      opa::Rect image_rect = rect;
      image_rect._size._width = float (image_or_mask._width);
      image_rect._size._height = float (image_or_mask._height);
      
      opa::Quad tex_coords (image_or_mask);
      
      for (int y = 0 ; y < loop_y ; ++y)
      {
         opa::Rect image_rect2 = image_rect;
         
         for (int x = 0 ; x < loop_x ; ++x)
         {
            opa::Quad quad (image_rect2);
            
            do_draw (image_or_mask, quad, tex_coords, alpha_or_color);
            
            image_rect2._origin._x += image_rect._size._width;
         }
         
         image_rect._origin._y += image_rect._size._height;
      }
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

template <class T>
void  GraphicContextBase::draw (const GraphicState & graphic_state, const SprungImage & image_or_mask, const opa::Rect & rect, const T & alpha_or_color)
{
   assert (_graphic_state_stack.size () > 0);
   
   size_t first_col_width;
   size_t middle_col_width;
   size_t last_col_width;
   image_or_mask.get_cols_width (first_col_width, middle_col_width, last_col_width, rect);

   size_t first_row_height;
   size_t middle_row_height;
   size_t last_row_height;
   image_or_mask.get_rows_height (first_row_height, middle_row_height, last_row_height, rect);
   
   opa::Rect sub_rect;
   
   do_update_context (graphic_state);
   
   // 1st row
   sub_rect._origin = rect._origin;
   sub_rect._size._height = float (first_row_height);
   sub_rect._size._width = float (first_col_width);
   
   const Image & image_00 = image_or_mask (SprungImage::Position_FIRST, SprungImage::Position_FIRST);
   const Image & image_10 = image_or_mask (SprungImage::Position_MIDDLE, SprungImage::Position_FIRST);
   const Image & image_20 = image_or_mask (SprungImage::Position_LAST, SprungImage::Position_FIRST);
   const Image & image_01 = image_or_mask (SprungImage::Position_FIRST, SprungImage::Position_MIDDLE);
   const Image & image_11 = image_or_mask (SprungImage::Position_MIDDLE, SprungImage::Position_MIDDLE);
   const Image & image_21 = image_or_mask (SprungImage::Position_LAST, SprungImage::Position_MIDDLE);
   const Image & image_02 = image_or_mask (SprungImage::Position_FIRST, SprungImage::Position_LAST);
   const Image & image_12 = image_or_mask (SprungImage::Position_MIDDLE, SprungImage::Position_LAST);
   const Image & image_22 = image_or_mask (SprungImage::Position_LAST, SprungImage::Position_LAST);
   
   if (!image_00.empty ())
   {
      do_draw (image_00, opa::Quad (sub_rect), opa::Quad (image_00), alpha_or_color);
   }
   
   sub_rect._origin._x += sub_rect._size._width;
   sub_rect._size._width = float (middle_col_width);
   if ((!image_10.empty ()) && (middle_col_width > 0))
   {
      draw (graphic_state, image_10, sub_rect, alpha_or_color);
   }
   
   sub_rect._origin._x += sub_rect._size._width;
   sub_rect._size._width = float (last_col_width);
   if (!image_20.empty ())
   {
      do_draw (image_20, opa::Quad (sub_rect), opa::Quad (image_20), alpha_or_color);
   }
   
   // 2nd row
   if (middle_row_height > 0)
   {
      sub_rect._origin = rect._origin;
      sub_rect._origin._y += first_row_height;
      sub_rect._size._height = float (middle_row_height);

      sub_rect._size._width = float (first_col_width);
      if (!image_01.empty ())
      {
         draw (graphic_state, image_01, sub_rect, alpha_or_color);
      }

      sub_rect._origin._x += sub_rect._size._width;
      sub_rect._size._width = float (middle_col_width);
      if ((!image_11.empty ()) && (middle_col_width > 0))
      {
         draw (graphic_state, image_11, sub_rect, alpha_or_color);
      }
      
      sub_rect._origin._x += sub_rect._size._width;
      sub_rect._size._width = float (last_col_width);
      if (!image_21.empty ())
      {
         draw (graphic_state, image_21, sub_rect, alpha_or_color);
      }
   }

   // 3rd row
   sub_rect._origin = rect._origin;
   sub_rect._origin._y += first_row_height + middle_row_height;
   sub_rect._size._height = float (last_row_height);

   sub_rect._size._width = float (first_col_width);
   if (!image_02.empty ())
   {
      do_draw (image_02, opa::Quad (sub_rect), opa::Quad (image_02), alpha_or_color);
   }

   sub_rect._origin._x += sub_rect._size._width;
   sub_rect._size._width = float (middle_col_width);
   if ((!image_12.empty ()) && (middle_col_width > 0))
   {
      draw (graphic_state, image_12, sub_rect, alpha_or_color);
   }
   
   sub_rect._origin._x += sub_rect._size._width;
   sub_rect._size._width = float (last_col_width);
   if (!image_22.empty ())
   {
      do_draw (image_22, opa::Quad (sub_rect), opa::Quad (image_22), alpha_or_color);
   }
}



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_GraphicContextBase_CODEHEADER_INCLUDED

#undef ohm_opal_GraphicContextBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
