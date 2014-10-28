/*****************************************************************************

        LayerEffect.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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
#include "ohm/opak/LayerEffect.h"
#include "ohm/opak/base/CachePool.h"
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

LayerEffect::LayerEffect ()
:  _active_flag (*this, &LayerEffect::active_flag_changed)
,  _proc (0)
,  _var_0 (*this, &LayerEffect::var_changed)
,  _var_1 (*this, &LayerEffect::var_changed)
,  _var_2 (*this, &LayerEffect::var_changed)
,  _var_3 (*this, &LayerEffect::var_changed)
{
   _var_0.init (0.f);
   _var_1.init (0.f);
   _var_2.init (0.f);
   _var_3.init (0.f);

   set_no_bounding_box ();
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LayerEffect::display (GraphicContext & context)
{
   float active_flag = _active_flag.get_cur_value ();
   
   if (!active_flag)
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
      
      context.save ();
      
      add_mark (context);
      
      context.translate (origin._x, origin._y);
      
      context.get ().save_graphic_state ();
      
      context.get ().concat_translate (
         clip_bounding_box._origin._x,
         clip_bounding_box._origin._y
      );
      
      if (_proc != 0)
      {
         context.get ().set_fragment_program (_proc);

         float var_0 = _var_0.get_cur_value ();
         float var_1 = _var_1.get_cur_value ();
         float var_2 = _var_2.get_cur_value ();
         float var_3 = _var_3.get_cur_value ();

         context.get ().set_fragment_local_vars (var_0, var_1, var_2, var_3);
      }
      
      context.get ().draw (cache_image, opa::Point::_zero);

      context.get ().restore_graphic_state ();
      
      context.restore ();
      
      CachePool::use_instance ().give_back (cache_item);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LayerEffect::~LayerEffect ()
{
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  LayerEffect::invalidate_all ()
{
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag || was_visible_flag)
   {
      invalidate_children ();
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : active_flag_changed
==============================================================================
*/

void  LayerEffect::active_flag_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : var_changed
==============================================================================
*/

void  LayerEffect::var_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
