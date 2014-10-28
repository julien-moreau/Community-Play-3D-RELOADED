/*****************************************************************************

        LayerTransparency.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 60267

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

#include "ohm/opal/Cache.h"
#include "ohm/opak/LayerTransparency.h"
#include "ohm/opak/base/CachePool.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

LayerTransparency::LayerTransparency ()
{
   set_no_bounding_box ();
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LayerTransparency::display (GraphicContext & context)
{
   float opacity = _opacity.get_cur_value ();
   BlendMode blend_mode = _blend_mode.get_cur_value ();
   
   bool neutral_flag
      = (opacity == 1.f)
      && (blend_mode == BlendMode_NORMAL)
   ;
   
   if (neutral_flag)
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
      CachePool::Item cache_item = CachePool::use_instance ().take (context);
      
      opal::Cache & cache = cache_item.use_cache ();
      GraphicContext & cache_context = cache_item.use_context ();
      
      opa::Rect clip_bounding_box = context.get ().get_clip_bounding_box ();
      opa::Point origin = _origin.get_cur_value ();
      
      opa::Rect cache_rect = clip_bounding_box;
      cache_rect._origin._x = 0;
      cache_rect._origin._y = 0;
      
      if (
         (cache_rect._size._width > cache.get_size ()._width)
         || (cache_rect._size._height > cache.get_size ()._height)
         )
      {
         cache.resize (cache_rect._size);
      }
      
      cache.invalidate_all ();
      
      if (cache)
      {
         opal::AutoContextState  auto_context_state (cache);
            
         cache.clip (cache.get_invalid_rect ());
         cache.clear ();
         
         cache_context.copy_stack (context);
         
         cache_context.translate (origin._x, origin._y);
         
         cache.concat_translate (
            - clip_bounding_box._origin._x,
            - clip_bounding_box._origin._y
         );
         
         display_children (cache_context);
         
         cache_context.reset_stack ();
         
         cache.validate ();
      }
      
      opal::Image cache_image = cache.get_image ();
      
      {
         Opacifiable::Auto auto_opacifiable (*this, context);
         BlendModable::Auto auto_blend_modable (*this, context);
         
         context.save ();
         
         add_mark (context);
         
         context.translate (origin._x, origin._y);
         
         {
            opal::AutoContextState  auto_context_state (context.get ());
            
            context.get ().concat_translate (
               clip_bounding_box._origin._x,
               clip_bounding_box._origin._y
            );
            
            context.get ().draw (cache_image, opa::Point (0, 0));
         }
         
         context.restore ();
      }
      
      CachePool::use_instance ().give_back (cache_item);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LayerTransparency::~LayerTransparency ()
{
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  LayerTransparency::invalidate_all ()
{
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag || was_visible_flag)
   {
      invalidate_children ();
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
