/*****************************************************************************

        BitmapContext.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 39766

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

#include "ohm/opal/BitmapContext.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/lang/fnc.h"

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

BitmapContext::BitmapContext (Bitmap & dst)
:  _dst (dst)
{
   _graphic_state_stack.push (GraphicState ());

   opa::Rect rect (
      opa::Point::_zero,
      opa::Size (float (dst.get_width ()), float (dst.get_height ()))
   );
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._total_translation = opa::Point (0, 0);
   graphic_state._global_clip_rect = rect;
   graphic_state._local_clip_rect = rect;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BitmapContext::~BitmapContext ()
{
}



/*
==============================================================================
Name : set_alpha
==============================================================================
*/

void  BitmapContext::set_alpha (float alpha)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._alpha = alpha;
}



/*
==============================================================================
Name : set_fill_color
==============================================================================
*/

void  BitmapContext::set_fill_color (const Color & color)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._fill_color = color;
}



/*
==============================================================================
Name : set_clear_color
==============================================================================
*/

void  BitmapContext::set_clear_color (const Color & color)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._clear_color = color;
}



/*
==============================================================================
Name : set_blend_mode_normal
==============================================================================
*/

void  BitmapContext::set_blend_mode_normal ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_NORMAL;
}



/*
==============================================================================
Name : set_blend_mode_multiply
==============================================================================
*/

void  BitmapContext::set_blend_mode_multiply ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_MULTIPLY;
}



/*
==============================================================================
Name : set_blend_mode_screen
==============================================================================
*/

void  BitmapContext::set_blend_mode_screen ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_SCREEN;
}



/*
==============================================================================
Name : set_blend_mode_replace
==============================================================================
*/

void  BitmapContext::set_blend_mode_replace ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_REPLACE;
}



/*
==============================================================================
Name : set_blend_mode_add
==============================================================================
*/

void  BitmapContext::set_blend_mode_add ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_ADD;
}



/*
==============================================================================
Name : set_blend_mode_max
==============================================================================
*/

void  BitmapContext::set_blend_mode_max ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_MAX;
}



/*
==============================================================================
Name : set_blend_mode_min
==============================================================================
*/

void  BitmapContext::set_blend_mode_min ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_MIN;
}



/*
==============================================================================
Name : clip
==============================================================================
*/

void  BitmapContext::clip (const opa::Rect & rect)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect global_rect = rect + graphic_state._total_translation;
   
   graphic_state._global_clip_rect &= global_rect;

   graphic_state._local_clip_rect
      = graphic_state._global_clip_rect
      - graphic_state._total_translation
   ;
}



/*
==============================================================================
Name : save_graphic_state
==============================================================================
*/

void  BitmapContext::save_graphic_state ()
{
   assert (_graphic_state_stack.size () > 0);
   
   _graphic_state_stack.push (_graphic_state_stack.top ());
}



/*
==============================================================================
Name : restore_graphic_state
==============================================================================
*/

void  BitmapContext::restore_graphic_state ()
{
   assert (_graphic_state_stack.size () > 1);
   
   _graphic_state_stack.pop ();
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  BitmapContext::fill (const opa::Rect & rect)
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect integral_rect = rect;
   integral_rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   opa::Rect visible_rect = integral_rect & local_clip_rect;
   
   if (!visible_rect.is_empty ())
   {
      if (graphic_state._alpha > _min_alpha)
      {
         Color color = graphic_state._fill_color;
         
         if (graphic_state._alpha < _max_alpha)
         {
            color._a *= graphic_state._alpha;
         }
         
         do_update_context (graphic_state);
         
         do_fill (
            size_t (visible_rect._origin._x),
            size_t (visible_rect._origin._y),
            size_t (visible_rect._size._width),
            size_t (visible_rect._size._height),
            color
         );
      }
   }
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  BitmapContext::clear ()
{
   save_graphic_state ();
   
   set_blend_mode_replace ();
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect rect;
   rect._origin = opa::Point::_zero;
   rect._size = opa::Size (float (_dst.get_width ()), float (_dst.get_height ()));
   
   set_fill_color (graphic_state._clear_color);
   
   fill (rect);
   
   restore_graphic_state ();
}



/*
==============================================================================
Name : draw
Note :
   'bitmap' is assumed to be already alpha-premultiplied
==============================================================================
*/

void  BitmapContext::draw (const Bitmap & bitmap, const opa::Point & point)
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Point integral_point = point;
   integral_point.lower_integral ();
   
   opa::Rect integral_rect = opa::Rect (
      point,
      opa::Size (float (bitmap.get_width ()), float (bitmap.get_height ()))
   );
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   opa::Rect visible_rect = integral_rect & local_clip_rect;
   
   if (!visible_rect.is_empty ())
   {
      if (graphic_state._alpha > _min_alpha)
      {
         Color color = graphic_state._fill_color;
         
         if (graphic_state._alpha < _max_alpha)
         {
            color._a *= graphic_state._alpha;
         }
         
         do_update_context (graphic_state);
         
         do_draw (
            size_t (visible_rect._origin._x),
            size_t (visible_rect._origin._y),
            size_t (visible_rect._size._width),
            size_t (visible_rect._size._height),
            bitmap,
            size_t (visible_rect._origin._x - integral_point._x),
            size_t (visible_rect._origin._y - integral_point._y)
         );
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_update_context
==============================================================================
*/

void  BitmapContext::do_update_context (const GraphicState & graphic_state)
{
   switch (graphic_state._blend_mode)
   {
   case BlendMode_NORMAL:
      _cur_blend_equation = BlendEquation_ADD;
      _cur_blend_func = BlendFunc_ONE_ONE_MINUS_SRC_ALPHA;
      break;

   case BlendMode_MULTIPLY:
      _cur_blend_equation = BlendEquation_ADD;
      _cur_blend_func = BlendFunc_DST_COLOR_ONE_MINUS_SRC_ALPHA;
      break;

   case BlendMode_SCREEN:
      _cur_blend_equation = BlendEquation_ADD;
      _cur_blend_func = BlendFunc_ONE_MINUS_DST_COLOR_ONE;
      break;

   case BlendMode_REPLACE:
      _cur_blend_equation = BlendEquation_ADD;
      _cur_blend_func = BlendFunc_ONE_ZERO;
      break;

   case BlendMode_ADD:
      _cur_blend_equation = BlendEquation_ADD;
      _cur_blend_func = BlendFunc_ONE_ONE;
      break;

   case BlendMode_MAX:
      _cur_blend_equation = BlendEquation_MAX;
      // blend func is ignored
      break;

   case BlendMode_MIN:
      _cur_blend_equation = BlendEquation_MIN;
      // blend func is ignored
      break;
   
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  BitmapContext::do_fill (size_t x, size_t y, size_t width, size_t height, const Color & color)
{
   // (x, y, width, height) is already constrained to '_dst' bounds
   
   bool replace_flag = false;
   replace_flag |= (_cur_blend_func == BlendFunc_ONE_ZERO);
   replace_flag |=
      (_cur_blend_func == BlendFunc_ONE_ONE_MINUS_SRC_ALPHA)
      && (color._a == 1.f);
   
   PixArgb src_pix;
   src_pix._a = ohm::archi::UByte (color._a * 255.f);
   src_pix._r = ohm::archi::UByte (color._r * 255.f);
   src_pix._g = ohm::archi::UByte (color._g * 255.f);
   src_pix._b = ohm::archi::UByte (color._b * 255.f);
   
   if (replace_flag)
   {
      PixArgb * pix_ptr = &_dst (x, y);
      
      for (size_t j = 0 ; j < height ; ++j)
      {
         for (size_t i = 0 ; i < width ; ++i)
         {
            pix_ptr [i] = src_pix;
         }
         
         pix_ptr += _dst.get_width ();
      }
   }
   else
   {
      PixArgb * pix_ptr = &_dst (x, y);
      
      for (size_t j = 0 ; j < height ; ++j)
      {
         for (size_t i = 0 ; i < width ; ++i)
         {
            blend (pix_ptr [i], src_pix);
         }
         
         pix_ptr += _dst.get_width ();
      }
   }
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  BitmapContext::do_draw (size_t dst_x, size_t dst_y, size_t width, size_t height, const Bitmap & bitmap, size_t src_x, size_t src_y)
{
   // (x, y, width, height) is already constrained to '_dst' bounds
   
   bool replace_flag = (_cur_blend_func == BlendFunc_ONE_ZERO);
   
   if (replace_flag)
   {
      PixArgb * dst_pix_ptr = &_dst (dst_x, dst_y);
      const PixArgb * src_pix_ptr = &bitmap (src_x, src_y);
      
      for (size_t j = 0 ; j < height ; ++j)
      {
         for (size_t i = 0 ; i < width ; ++i)
         {
            dst_pix_ptr [i] = src_pix_ptr [i];
         }
         
         dst_pix_ptr += _dst.get_width ();
         src_pix_ptr += bitmap.get_width ();
      }
   }
   else
   {
      PixArgb * dst_pix_ptr = &_dst (dst_x, dst_y);
      const PixArgb * src_pix_ptr = &bitmap (src_x, src_y);
      
      for (size_t j = 0 ; j < height ; ++j)
      {
         for (size_t i = 0 ; i < width ; ++i)
         {
            blend (dst_pix_ptr [i], src_pix_ptr [i]);
         }
         
         dst_pix_ptr += _dst.get_width ();
         src_pix_ptr += bitmap.get_width ();
      }
   }
}



/*
==============================================================================
Name : blend
==============================================================================
*/

void  BitmapContext::blend (PixArgb & dst, const PixArgb & src)
{
   PixArgb dst_color = dst;
   PixArgb src_color = src;
   
   switch (_cur_blend_func)
   {
   case BlendFunc_ONE_ONE_MINUS_SRC_ALPHA:
      {
         archi::UByte invsrca = ~src._a;
         dst_color._a = (invsrca * dst_color._a) >> 8;
         dst_color._r = (invsrca * dst_color._r) >> 8;
         dst_color._g = (invsrca * dst_color._g) >> 8;
         dst_color._b = (invsrca * dst_color._b) >> 8;
      }
      break;

   case BlendFunc_DST_COLOR_ONE_MINUS_SRC_ALPHA:
      {
         src_color._a = (src_color._a * dst_color._a) >> 8;
         src_color._r = (src_color._r * dst_color._r) >> 8;
         src_color._g = (src_color._g * dst_color._g) >> 8;
         src_color._b = (src_color._b * dst_color._b) >> 8;

         archi::UByte invsrca = ~src._a;
         dst_color._a = (invsrca * dst_color._a) >> 8;
         dst_color._r = (invsrca * dst_color._r) >> 8;
         dst_color._g = (invsrca * dst_color._g) >> 8;
         dst_color._b = (invsrca * dst_color._b) >> 8;
      }
      break;

   case BlendFunc_ONE_MINUS_DST_COLOR_ONE:
      {
         src_color._a = (src_color._a * ~dst_color._a) >> 8;
         src_color._r = (src_color._r * ~dst_color._r) >> 8;
         src_color._g = (src_color._g * ~dst_color._g) >> 8;
         src_color._b = (src_color._b * ~dst_color._b) >> 8;
      }
      break;

   case BlendFunc_ONE_ZERO:
      {
         dst_color._a = 0;
         dst_color._r = 0;
         dst_color._g = 0;
         dst_color._b = 0;
      }
      break;

   case BlendFunc_ONE_ONE:
      // nothing
      break;
   }
   
   switch (_cur_blend_equation)
   {
   case BlendEquation_ADD:
      dst._a = src_color._a + dst_color._a;
      dst._r = src_color._r + dst_color._r;
      dst._g = src_color._g + dst_color._g;
      dst._b = src_color._b + dst_color._b;
      break;

   case BlendEquation_MAX:
      dst._a = lang::max (src_color._a, dst_color._a);
      dst._r = lang::max (src_color._r, dst_color._r);
      dst._g = lang::max (src_color._g, dst_color._g);
      dst._b = lang::max (src_color._b, dst_color._b);
      break;

   case BlendEquation_MIN:
      dst._a = lang::min (src_color._a, dst_color._a);
      dst._r = lang::min (src_color._r, dst_color._r);
      dst._g = lang::min (src_color._g, dst_color._g);
      dst._b = lang::min (src_color._b, dst_color._b);
      break;
   }
}



float BitmapContext::_min_alpha = 1.f / 255.f;
float BitmapContext::_max_alpha = 254.f / 255.f;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
