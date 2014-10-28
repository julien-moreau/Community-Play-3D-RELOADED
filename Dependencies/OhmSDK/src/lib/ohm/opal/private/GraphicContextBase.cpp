/*****************************************************************************

        GraphicContextBase.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 60935

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

#include "ohm/opal/private/GraphicContextBase.h"
#include "ohm/opa/Path.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opa/Quad.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opal/Cache.h"


#include <cassert>
#include <cmath>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

GraphicContextBase::GraphicContextBase ()
:  _graphic_state_stack ()
{
   _graphic_state_stack.push (GraphicState ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

GraphicContextBase::~GraphicContextBase ()
{
}



/*
==============================================================================
Name : use_engine
==============================================================================
*/

Engine & GraphicContextBase::use_engine ()
{
   return do_use_engine ();
}



/*
==============================================================================
Name : set_clip_rect
==============================================================================
*/

void  GraphicContextBase::set_clip_rect (const opa::Rect & rect)
{
   assert (_graphic_state_stack.size () == 1);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._total_translation = opa::Point (0, 0);
   graphic_state._global_clip_rect = rect;
   graphic_state._local_clip_rect = rect;
}



/*
==============================================================================
Name : set_alpha
==============================================================================
*/

void  GraphicContextBase::set_alpha (float alpha)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._alpha = alpha;
}



/*
==============================================================================
Name : concat_alpha
==============================================================================
*/

void  GraphicContextBase::concat_alpha (float alpha)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._alpha *= alpha;
}



/*
==============================================================================
Name : set_stroke_color
==============================================================================
*/

void  GraphicContextBase::set_stroke_color (const Color & color)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._stroke_color = color;
}



/*
==============================================================================
Name : set_fill_color
==============================================================================
*/

void  GraphicContextBase::set_fill_color (const Color & color)
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

void  GraphicContextBase::set_clear_color (const Color & color)
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

void  GraphicContextBase::set_blend_mode_normal ()
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

void  GraphicContextBase::set_blend_mode_multiply ()
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

void  GraphicContextBase::set_blend_mode_screen ()
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

void  GraphicContextBase::set_blend_mode_replace ()
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

void  GraphicContextBase::set_blend_mode_add ()
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

void  GraphicContextBase::set_blend_mode_max ()
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

void  GraphicContextBase::set_blend_mode_min ()
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._blend_mode = BlendMode_MIN;
}



/*
==============================================================================
Name : set_fragment_program
==============================================================================
*/

void  GraphicContextBase::set_fragment_program (FragmentProgramProc proc)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._fragment_program_proc = proc;
}



/*
==============================================================================
Name : set_fragment_local_vars
==============================================================================
*/

void  GraphicContextBase::set_fragment_local_vars (float var_0, float var_1, float var_2, float var_3)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._fragment_var_arr [0] = var_0;
   graphic_state._fragment_var_arr [1] = var_1;
   graphic_state._fragment_var_arr [2] = var_2;
   graphic_state._fragment_var_arr [3] = var_3;
}



/*
==============================================================================
Name : clip
==============================================================================
*/

void  GraphicContextBase::clip (const opa::Rect & rect)
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
Name : get_clip_bounding_box
==============================================================================
*/

opa::Rect   GraphicContextBase::get_clip_bounding_box ()
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   return graphic_state._local_clip_rect;
}



   
/*
==============================================================================
Name : concat_translate
==============================================================================
*/

void  GraphicContextBase::concat_translate (float tx, float ty)
{
   assert (_graphic_state_stack.size () > 0);
   
   GraphicState & graphic_state = _graphic_state_stack.top ();
   
   graphic_state._total_translation += opa::Point (tx, ty);
   
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

void  GraphicContextBase::save_graphic_state ()
{
   assert (_graphic_state_stack.size () > 0);
   
   _graphic_state_stack.push (_graphic_state_stack.top ());
}



/*
==============================================================================
Name : restore_graphic_state
==============================================================================
*/

void  GraphicContextBase::restore_graphic_state ()
{
   assert (_graphic_state_stack.size () > 1);
   
   _graphic_state_stack.pop ();
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  GraphicContextBase::fill (const opa::Path & path)
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   const opa::Rect & bounding_box = path.get_integral_bounding_box ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (bounding_box.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         if (graphic_state._alpha < _max_alpha)
         {
            Color color = graphic_state._fill_color;
            color._a *= graphic_state._alpha;
            
            do_update_context (graphic_state);
            
            do_fill (path, color);
         }
         else
         {
            do_update_context (graphic_state);
            
            do_fill (path, graphic_state._fill_color);
         }
      }
   }
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  GraphicContextBase::fill (const opa::Rect & rect)
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect integral_rect = rect;
   integral_rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (integral_rect.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         if (graphic_state._alpha < _max_alpha)
         {
            Color color = graphic_state._fill_color;
            color._a *= graphic_state._alpha;
         
            do_update_context (graphic_state);
            
            do_fill (rect, color);
         }
         else
         {
            do_update_context (graphic_state);
            
            do_fill (rect, graphic_state._fill_color);
         }
      }
   }
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  GraphicContextBase::fill (const opa::Quad & vertices)
{
   if (vertices.is_rect ())
   {
      opa::Rect rect;
      vertices.get_rect (rect);

      fill (rect);
   }
   else
   {
      assert (_graphic_state_stack.size () > 0);
      
      const GraphicState & graphic_state = _graphic_state_stack.top ();
      
      opa::Rect integral_rect;
      vertices.get_bounding_box (integral_rect);
      
      integral_rect.integral ();
      
      const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
      
      if (integral_rect.is_intersecting (local_clip_rect))
      {
         if (graphic_state._alpha > _min_alpha)
         {
            if (graphic_state._alpha < _max_alpha)
            {
               Color color = graphic_state._fill_color;
               color._a *= graphic_state._alpha;
            
               do_update_context (graphic_state);
               
               do_fill (vertices, color);
            }
            else
            {
               do_update_context (graphic_state);
               
               do_fill (vertices, graphic_state._fill_color);
            }
         }
      }
   }
}



/*
==============================================================================
Name : stroke
==============================================================================
*/

void  GraphicContextBase::stroke (const opa::Path & path)
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   const opa::Rect & bounding_box = path.get_integral_bounding_box ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (bounding_box.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         if (graphic_state._alpha < _max_alpha)
         {
            Color color = graphic_state._stroke_color;
            color._a *= graphic_state._alpha;
            
            do_update_context (graphic_state);
            
            do_stroke (path, color);
         }
         else
         {
            do_update_context (graphic_state);
            
            do_stroke (path, graphic_state._stroke_color);
         }
      }
   }
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  GraphicContextBase::clear ()
{
   save_graphic_state ();
   
   set_blend_mode_replace ();
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect rect;
   rect._origin = opa::Point::_zero;
   rect._size = do_get_context_size ();
   
   set_fill_color (graphic_state._clear_color);
   
   fill (rect);
   
   restore_graphic_state ();
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const Image & image, const opa::Point & point)
{
   assert (_graphic_state_stack.size () > 0);
   
   if (image.empty ())
   {
      return;
   }
   
   opa::Rect rect;
   rect._origin = point;
   rect._size._width = float (image._width);
   rect._size._height = float (image._height);
   
   draw (image, rect);
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const Image & image, const opa::Rect & rect)
{
   assert (_graphic_state_stack.size () > 0);
   
   if (image.empty ())
   {
      return;
   }
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect integral_rect = rect;
   integral_rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (integral_rect.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         draw (graphic_state, image, rect, graphic_state._alpha);
      }
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords)
{
   if (image.empty ())
   {
      return;
   }
   
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect rect;
   vertices.get_bounding_box (rect);
   rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (rect.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         do_update_context (graphic_state);
            
         do_draw (image, vertices, tex_coords, graphic_state._alpha);
      }
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const SprungImage & sprung_image, const opa::Rect & rect)
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect integral_rect = rect;
   integral_rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (integral_rect.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         draw (graphic_state, sprung_image, rect, graphic_state._alpha);
      }
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const Image & mask, const opa::Point & point, const Color & color)
{
   assert (_graphic_state_stack.size () > 0);
   
   if (mask.empty ())
   {
      return;
   }
   
   opa::Rect rect;
   rect._origin = point;
   rect._size._width = float (mask._width);
   rect._size._height = float (mask._height);
   
   draw (mask, rect, color);
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const Image & mask, const opa::Rect & rect, const Color & color)
{
   assert (_graphic_state_stack.size () > 0);
   
   if (mask.empty ())
   {
      return;
   }
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect integral_rect = rect;
   integral_rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (integral_rect.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         Color color_a = color;
         color_a._a *= graphic_state._alpha;
         
         draw (graphic_state, mask, rect, color_a);
      }
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color)
{
   if (mask.empty ())
   {
      return;
   }
   
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect rect;
   vertices.get_bounding_box (rect);
   rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (rect.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         Color color_a = color;
         color_a._a *= graphic_state._alpha;
         
         do_update_context (graphic_state);
         
         do_draw (mask, vertices, tex_coords, color_a);
      }
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  GraphicContextBase::draw (const SprungImage & sprung_mask, const opa::Rect & rect, const Color & color)
{
   assert (_graphic_state_stack.size () > 0);
   
   const GraphicState & graphic_state = _graphic_state_stack.top ();
   
   opa::Rect integral_rect = rect;
   integral_rect.integral ();
   
   const opa::Rect & local_clip_rect = graphic_state._local_clip_rect;
   
   if (integral_rect.is_intersecting (local_clip_rect))
   {
      if (graphic_state._alpha > _min_alpha)
      {
         Color color_a = color;
         color_a._a *= graphic_state._alpha;
         
         draw (graphic_state, sprung_mask, rect, color_a);
      }
   }
}



/*
==============================================================================
Name: use_stack_top
==============================================================================
*/

const GraphicState & GraphicContextBase::use_stack_top () const
{
   return _graphic_state_stack.top ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



float GraphicContextBase::_min_alpha = 1.f / 255.f;
float GraphicContextBase::_max_alpha = 254.f / 255.f;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
