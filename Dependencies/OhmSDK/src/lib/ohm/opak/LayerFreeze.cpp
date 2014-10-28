/*****************************************************************************

        LayerFreeze.cpp
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
#include "ohm/opak/LayerFreeze.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/math/def.h"
#include "ohm/util/trace.h"

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

LayerFreeze::LayerFreeze ()
:  _freeze_flag (*this, &LayerFreeze::freeze_flag_changed)
{
   set_no_bounding_box ();
}



/*
==============================================================================
Name : restore_display
==============================================================================
*/

void  LayerFreeze::restore_display ()
{
   if (!_cache_item.is_empty ())
   {
      CachePool::use_instance ().give_back (_cache_item);
   }
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LayerFreeze::display (GraphicContext & context)
{
   float freeze_flag = _freeze_flag.get_cur_value ();
   
   bool snapshot_flag = false;
   
   if (_cache_item.is_empty () && freeze_flag)
   {
      _cache_item = CachePool::use_instance ().take (context);
      snapshot_flag = true;
   }
   else if ((!_cache_item.is_empty ()) && (!freeze_flag))
   {
      CachePool::use_instance ().give_back (_cache_item);
   }
   
   if (!freeze_flag)
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
      assert (!_cache_item.is_empty ());
      
      opa::Size size = _size.get_cur_value ();
      
      opal::Cache & cache = _cache_item.use_cache ();
      
      opa::Point origin = _origin.get_cur_value ();
      
      if (snapshot_flag)
      {
         GraphicContext & cache_context = _cache_item.use_context ();
         
         opa::Rect cache_rect (opa::Point::_zero, size);
         
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
            
            display_children (cache_context);
            
            cache_context.reset_stack ();
            
            cache.validate ();
         }
      }
      
      opal::Image cache_image = cache.get_image ();
      
      context.save ();
      
      add_mark (context);
      
      //ohm_util_TRACE_INFO2 ("draw %1% %2%", pt._x, pt._y);
      
      context.translate (origin._x, origin._y);
      //context.translate (pt._x, pt._y);
      
      context.get ().save_graphic_state ();
      
      opa::Point pt (0, 0);
      context.apply (pt);
      
      //context.get ().draw (cache_image, pt);
      //context.get ().draw (cache_image, opa::Point::_zero);
      context.get ().draw (cache_image, pt);

      context.get ().restore_graphic_state ();
      
      context.restore ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LayerFreeze::~LayerFreeze ()
{
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  LayerFreeze::invalidate_all ()
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
Name : freeze_flag_changed
==============================================================================
*/

void  LayerFreeze::freeze_flag_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
