/*****************************************************************************

        LayerFlip.cpp
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

#include "ohm/opak/LayerFlip.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/Cache.h"
#include "ohm/opak/base/CachePool.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/math/def.h"

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

LayerFlip::LayerFlip ()
:  _rotation (*this, &LayerFlip::rotation_changed)
,  _amount_px (*this, &LayerFlip::amount_px_changed)
{
   set_no_bounding_box ();
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LayerFlip::display (GraphicContext & context)
{
   float rotation = _rotation.get_cur_value ();
   
   rotation = fmod (rotation, 1.f);
   
   if (rotation == 0.f)
   {
      context.save ();
   
      add_mark (context);
      
      opa::Point origin = _origin.get_cur_value ();
      
      context.translate (origin._x, origin._y);
      
      display_children (context);
      
      context.restore ();
   }
   else
   {
      context.save ();
      
      add_mark (context);
      
      opa::Point origin = _origin.get_cur_value ();
      opa::Size size = _size.get_cur_value ();
      
      context.translate (origin._x, origin._y);
      
      opa::Point origin_sc = opa::Point::_zero;
      context.apply (origin_sc);
      
      opa::Size size_sc = size;
      context.apply (size_sc);
      
      opa::Rect clip_bbox = context.get ().get_clip_bounding_box ();
      
      if (
         (size_sc._width > clip_bbox._size._width)
         || (size_sc._height > clip_bbox._size._height)
         )
      {
         // for now we don't display the flip
      }
      else
      {
         CachePool::Item cache_item = CachePool::use_instance ().take (context);
         
         opal::Cache & cache = cache_item.use_cache ();
         GraphicContext & cache_context = cache_item.use_context ();
         
         size_sc.upper_integral ();
            
         if (cache.get_size () != size_sc + opa::Size (2, 2))
         {
            cache.resize (size_sc + opa::Size (2, 2));
            cache.invalidate_all ();
         }
         
         rotation *= 2.f * float (math::PI);
         float cosr = cos (rotation);
         float sinr = sin (rotation);
         
         if (cache)
         {
            opal::AutoContextState  auto_context_state (cache);
               
            cache.clip (cache.get_invalid_rect ());
            //cache.set_clear_color (opal::Color::_yellow);
            cache.clear ();
            
            cache.save_graphic_state ();
            
            cache_context.copy_stack (context);
            
            cache.concat_translate (
               - origin_sc._x + 1.f,
               - origin_sc._y + 1.f
            );
            
            display_children (cache_context);
            
            cache_context.reset_stack ();
            
            cache.restore_graphic_state ();
            
            cache.save_graphic_state ();
            
            cache.set_blend_mode_multiply ();
            
            float g = 1.f - sinr * .3f;
            
            cache.set_fill_color (opal::Color (1.f, g, g, g));
            
            cache.fill (opa::Rect (opa::Point::_zero, size_sc));
            
            cache.restore_graphic_state ();
            
            cache.validate ();
         }
         
         opal::Image cache_image = cache.get_image ();
         
         const opa::Point inv_pt = context.get_inv_px ();
         
         float inv_x = inv_pt._x;
         float inv_y = inv_pt._y;
         
         bool hinted_flag = false;
         
         float amount_px = _amount_px.get_cur_value ();
         
         if (is_horizontal ())
         {
            float dh = inv_y * amount_px * sinr;
            float s = .5f * size._width;
            
            opa::Quad quad (
               opa::Point (s - s * cosr - inv_x, - dh - inv_y),
               opa::Point (s + s * cosr + inv_x, + dh - inv_y),
               opa::Point (s + s * cosr + inv_x, size._height - dh + inv_y),
               opa::Point (s - s * cosr - inv_x, size._height + dh + inv_y)
            );
            
            PrimitiveHelper::display (
               context, cache_image, quad, hinted_flag
            );
         }
         else
         {
            float dw = inv_x * amount_px * sinr;
            float s = .5f * size._height;
            
            opa::Quad quad (
               opa::Point (+ dw - inv_x, s - s * cosr - inv_y),
               opa::Point (size._width - dw + inv_x, s - s * cosr - inv_y),
               opa::Point (size._width + dw + inv_x, s + s * cosr + inv_y),
               opa::Point (- dw - inv_x, s + s * cosr + inv_y)
            );
            
            PrimitiveHelper::display (
               context, cache_image, quad, hinted_flag
            );
         }
         
         CachePool::use_instance ().give_back (cache_item);
      }

      context.restore ();
   }
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  LayerFlip::invalidate (const opa::Rect & rect, const opa::Margin & margin)
{
   float rotation = _rotation.get_cur_value ();
   
   rotation = fmod (rotation, 1.f);
   
   if (rotation == 0.f)
   {
      Primitive::invalidate (rect, margin);
   }
   else
   {
      invalidate_all ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LayerFlip::~LayerFlip ()
{
}



/*
==============================================================================
Name : do_size_changed
==============================================================================
*/

void  LayerFlip::do_size_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  LayerFlip::invalidate_all ()
{
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag || was_visible_flag)
   {
      do_invalidate (opa::Rect::_infinity, opa::Rect::_zero);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : clip_rect_changed
==============================================================================
*/

void  LayerFlip::rotation_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : amount_px_changed
==============================================================================
*/

void  LayerFlip::amount_px_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
