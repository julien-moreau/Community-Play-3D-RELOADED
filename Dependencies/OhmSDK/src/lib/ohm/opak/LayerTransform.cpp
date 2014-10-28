/*****************************************************************************

        LayerTransform.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70067

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

#include "ohm/opak/LayerTransform.h"
#include "ohm/opak/base/CachePool.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opal/Cache.h"

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

LayerTransform::LayerTransform ()
:  _rotation (*this, &LayerTransform::transformation_changed)
,  _scale (*this, &LayerTransform::transformation_changed)

,  _fast_invalidate_flag (true)
{
   set_no_bounding_box ();
   
   _rotation.init (0.f);
   _scale.init (opa::Point (1.f, 1.f));
}



/*
==============================================================================
Name : enable_fast_invalidate
Description :
   Since the LayerTransform has not a way to know quickly its bounding box,
   invalidating the whole layer normally is achieved by forwarding
   invalidation requests to the layer's children.
   
   If the client knows that invalidating the layer will almost invalidate
   everything (for example the layer has an infinte background, etc.),
   then it may use the fast invalidate feature which will invalidate
   an almost infinite rectangle.
==============================================================================
*/

void  LayerTransform::enable_fast_invalidate (bool enable_flag)
{
   //#warning we could detect infinite primitives to automatically enable this property
   
   _fast_invalidate_flag = enable_flag;
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LayerTransform::display (GraphicContext & context)
{
   context.save ();
   
   add_mark (context);
   
   opa::Point origin = _origin.get_cur_value ();
   float rotation = _rotation.get_cur_value ();
   opa::Point scale = _scale.get_cur_value ();
   
   if ((scale._x != 0.f) && (scale._y != 0.f))
   {
      context.translate (origin._x, origin._y);
      context.rotate (rotation);
      context.scale (scale._x, scale._y);
         
      bool use_cache_flag = false;
      
      const opa::AffineTransform & aff_transform = context.use_transform ();
      
      use_cache_flag
         |= (!aff_transform.is_scaling_translation_hint ())
         && (!aff_transform.is_perpendicular_hint ())
      ;
      
      // we could refine the use of cache since using it is going
      // to cost a lot
      // hints are just hints : refining here means to get
      // the real calculation
      // now in practice the hints might be true most of the time
      
      //use_cache_flag = false;
      
      if (use_cache_flag)
      {
         CachePool::Item cache_item = CachePool::use_instance ().take (context);
         
         opal::Cache & cache = cache_item.use_cache ();
         GraphicContext & cache_context = cache_item.use_context ();
         
         // get the region to update in primitive space
         
         opa::Rect clip_bounding_box = context.get ().get_clip_bounding_box ();
         
         context.apply_inverse (clip_bounding_box);
         
         // update the cache to match the update region
         
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
         
         // apply the whole transformation except for the last :
         // just translation and scale
         
         context.restore ();
         context.save ();
         
         cache.invalidate_all ();
         
         if (cache)
         {
            opal::AutoContextState  auto_context_state (cache);
               
            cache.clip (cache.get_invalid_rect ());
            cache.clear ();
            
            cache_context.copy_stack (context);
            cache_context.reset_to_identity ();
            cache_context.scale (scale._x, scale._y);
            
            cache.concat_translate (
               - clip_bounding_box._origin._x,
               - clip_bounding_box._origin._y
            );
            
            display_children (cache_context);
            
            cache_context.reset_stack ();
            
            cache.validate ();
         }
         
         context.restore ();
         context.save ();
         
         context.translate (origin._x, origin._y);
         context.rotate (rotation);
         
         opal::Image cache_image = cache.get_image ();
         
         opa::Point pt = clip_bounding_box._origin;
         
         bool scale_image_flag = true;
         bool hinted_flag = false;
         
         PrimitiveHelper::display (
            context, cache_image, pt, scale_image_flag, hinted_flag
         );

         CachePool::use_instance ().give_back (cache_item);
      }
      else
      {
         display_children (context);
      }
   }

   context.restore ();
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  LayerTransform::invalidate (const opa::Rect & rect, const opa::Margin & margin)
{
   opa::Point scale = _scale.get_cur_value ();
   
   if ((scale._x != 0.f) && (scale._y != 0.f))
   {
      ParentPtrArr::const_iterator it = get_parent_ptr_arr ().begin ();
      const ParentPtrArr::const_iterator it_end = get_parent_ptr_arr ().end ();
      
      opa::Rect parent_rel_rect = rect;

      parent_rel_rect._origin._x *= scale._x;
      parent_rel_rect._origin._y *= scale._y;
      parent_rel_rect._size._width *= scale._x;
      parent_rel_rect._size._height *= scale._y;
      
      opa::Rect margin_rect (margin);
      
      float rotation = _rotation.get_cur_value ();
      
      if (rotation != 0.f)
      {
         opa::AffineTransform transform (false);
         
         transform.rotate (rotation);
         
         transform.apply (parent_rel_rect);
         transform.apply (margin_rect);
      }
      
      opa::Margin new_margin (margin_rect);
      
      parent_rel_rect += _origin.get_cur_value ();
      
      for (; it != it_end ; ++it)
      {
         Primitive * parent_ptr = *it;
         
         parent_ptr->invalidate (parent_rel_rect, new_margin);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LayerTransform::~LayerTransform ()
{
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  LayerTransform::invalidate_all ()
{
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag || was_visible_flag)
   {
      if (_fast_invalidate_flag)
      {
         do_invalidate (opa::Rect::_infinity, opa::Rect::_zero);
      }
      else
      {
         invalidate_children ();
      }
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : transformation_changed
==============================================================================
*/

void  LayerTransform::transformation_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
