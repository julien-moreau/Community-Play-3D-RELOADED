/*****************************************************************************

        Cache.cpp
        Copyright (c) 2008 Ohm Force

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

#include "ohm/opal/def.h"

#if (ohm_opal_API == ohm_opal_API_OPENGL)
   #include "ohm/opal/opengl/OpenGLEngine.h"

#elif (ohm_opal_API == ohm_opal_API_DIRECTX)
   #include "ohm/opal/directx/DirectXEngine.h"
   
#else
   #error Unsupported Architecture
   
#endif



#include <cassert>



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

Cache::Cache (GraphicContextInterface & gc, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
:  _engine (gc.use_engine ())
,  _cache_impl (gc, sprung_mode_x, sprung_mode_y, filter_mode)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Cache::~Cache ()
{
}




/*
==============================================================================
Name : operator bool
Description :
   Returns true if the cache needs an update
==============================================================================
*/

Cache::operator bool ()
{
   if (_cache_impl.get_size ().is_empty ())
   {
      return false;
   }
   else
   {
      return _cache_impl.is_dirty ();
   }
}



/*
==============================================================================
Name : resize
Description :
   Change the size of the cache, and invalidates it if and only if
   the size is different from the internal one.
==============================================================================
*/

void  Cache::resize (const opa::Size & size)
{
   _cache_impl.resize (size);
   
   set_clip_rect (opa::Rect (opa::Point::_zero, size));
}




/*
==============================================================================
Name : get_size
==============================================================================
*/

const opa::Size & Cache::get_size ()
{
   return do_get_context_size ();
}



/*
==============================================================================
Name : get_image
==============================================================================
*/

Image Cache::get_image () const
{
   return _cache_impl.get_image ();
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  Cache::invalidate_all ()
{
   _cache_impl.invalidate_all ();
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  Cache::invalidate (const opa::Rect & rect)
{
   _cache_impl.invalidate (rect);
}



/*
==============================================================================
Name : validate
==============================================================================
*/

void  Cache::validate ()
{
   _cache_impl.validate ();
}



/*
==============================================================================
Name : get_invalid_rect
==============================================================================
*/

const opa::Rect & Cache::get_invalid_rect () const
{
   return _cache_impl.get_invalid_rect ();
}



/*
==============================================================================
Name : clear_invalid_rect
==============================================================================
*/

void  Cache::clear_invalid_rect ()
{
   save_graphic_state ();
   
   clip (get_invalid_rect ());
   
   clear ();
   
   restore_graphic_state ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_use_engine
==============================================================================
*/

Engine & Cache::do_use_engine ()
{
   return _engine;
}



/*
==============================================================================
Name : do_update_context
==============================================================================
*/

void  Cache::do_update_context (const GraphicState & graphic_state)
{
   _cache_impl.update_context (graphic_state);
}



/*
==============================================================================
Name : do_get_context_size
==============================================================================
*/

const opa::Size & Cache::do_get_context_size ()
{
   return _cache_impl.get_size ();
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  Cache::do_fill (const opa::Path & path, const Color & color)
{
   _cache_impl.fill (path, color);
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  Cache::do_fill (const opa::Rect & rect, const Color & color)
{
   _cache_impl.fill (rect, color);
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  Cache::do_fill (const opa::Quad & vertices, const Color & color)
{
   _cache_impl.fill (vertices, color);
}



/*
==============================================================================
Name : do_stroke
==============================================================================
*/

void  Cache::do_stroke (const opa::Path & path, const Color & color)
{
   _cache_impl.stroke (path, color);
}



/*
==============================================================================
Name : do_draw
==============================================================================
*/

void  Cache::do_draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha)
{
   _cache_impl.draw (image, vertices, tex_coords, alpha);
}



/*
==============================================================================
Name : do_draw
==============================================================================
*/

void  Cache::do_draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color)
{
   _cache_impl.draw (mask, vertices, tex_coords, color);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
