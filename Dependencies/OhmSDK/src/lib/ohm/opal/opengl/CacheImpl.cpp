/*****************************************************************************

        CacheImpl.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61579

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

#include "ohm/opal/opengl/CacheImpl.h"
#include "ohm/opal/Cache.h"
#include "ohm/opal/def.h"
#include "ohm/opal/opengl/OpenGLEngine.h"

#include <cassert>



namespace ohm
{
namespace opal
{



#if (ohm_opal_API == ohm_opal_API_OPENGL)

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

CacheImpl::CacheImpl (GraphicContextInterface & gc, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
:  _engine (gc.use_engine ())
,  _frame_buffer_id (0)
,  _size (opa::Size::_zero)
,  _image ()
,  _invalid_flag (false)
,  _invalid_rect ()
{
   _image._name_0 = "OpenGL Cache";

   // default
   _conf._sprung_mode_x = sprung_mode_x;
   _conf._sprung_mode_y = sprung_mode_y;
   _conf._filter_mode = filter_mode;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

CacheImpl::~CacheImpl ()
{
   release ();
}



/*
==============================================================================
Name : resize
==============================================================================
*/

void  CacheImpl::resize (const opa::Size & size)
{
   _engine.check_thread_id ();
   
   opa::Size rounded_size = upper_integral (size);
   
   if ((_frame_buffer_id == 0) || (rounded_size != _size))
   {
      release ();
      
      if (!rounded_size.is_empty ())
      {
         _engine.prepare (
            _image,
            rounded_size._width, rounded_size._height
         );
         
         // some card do not accept small values of width or height
         // so make the minimum texture width at least 32x32
         
         bool small_width_flag = _image._tex_width < 32;
         bool small_height_flag = _image._tex_height < 32;
         
         size_t tex_width = _image._tex_width;
         size_t tex_height = _image._tex_height;
         
         if (small_width_flag)
         {
            _image._tex_width = 32;
         }
         
         if (small_height_flag)
         {
            _image._tex_height = 32;
         }
         
         _engine.create (_frame_buffer_id, _image, _conf);

         if (small_width_flag)
         {
            _image._tex_s *= float (tex_width) / float (_image._tex_width);
         }
         
         if (small_height_flag)
         {
            _image._tex_t *= float (tex_height) / float (_image._tex_height);
         }
      }
      
      _size = rounded_size;
      
      invalidate_all ();
   }
}



/*
==============================================================================
Name : get_size
==============================================================================
*/

const opa::Size & CacheImpl::get_size ()
{
   return _size;
}



/*
==============================================================================
Name : get_image
==============================================================================
*/

Image CacheImpl::get_image () const
{
   return _image;
}



/*
==============================================================================
Name : is_dirty
==============================================================================
*/

bool  CacheImpl::is_dirty () const
{
   return _invalid_flag;
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  CacheImpl::invalidate_all ()
{
   if (!_size.is_empty ())
   {
      _invalid_flag = true;
      _invalid_rect = opa::Rect (opa::Point::_zero, _size);
   }
}



/*
==============================================================================
Name : invalidate
==============================================================================
*/

void  CacheImpl::invalidate (const opa::Rect & rect)
{
   opa::Rect local_rect = rect & opa::Rect (opa::Point::_zero, _size);
   
   if (!local_rect.is_empty ())
   {
      _invalid_flag = true;
   
      if (_invalid_rect.is_empty ())
      {
         _invalid_rect = local_rect;
      }
      else
      {
         _invalid_rect |= local_rect;
      }
   }
}



/*
==============================================================================
Name : validate
==============================================================================
*/

void  CacheImpl::validate ()
{
   _invalid_flag = false;
   _invalid_rect = opa::Rect::_zero;
}
   


/*
==============================================================================
Name : get_invalid_rect
==============================================================================
*/

const opa::Rect & CacheImpl::get_invalid_rect () const
{
   assert (_invalid_flag);
   
   return _invalid_rect;
}



/*
==============================================================================
Name : switch_context
==============================================================================
*/

void  CacheImpl::update_context (const GraphicState & graphic_state)
{
   assert (_frame_buffer_id != 0);
   
   _engine.update_context (_frame_buffer_id, _size, graphic_state);
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  CacheImpl::fill (const opa::Path & path, const Color & color)
{
   _engine.fill (path, color);
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  CacheImpl::fill (const opa::Rect & rect, const Color & color)
{
   _engine.fill (rect, color);
}



/*
==============================================================================
Name : fill
==============================================================================
*/

void  CacheImpl::fill (const opa::Quad & vertices, const Color & color)
{
   _engine.fill (vertices, color);
}



/*
==============================================================================
Name : stroke
==============================================================================
*/

void  CacheImpl::stroke (const opa::Path & path, const Color & color)
{
   _engine.stroke (path, color);
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  CacheImpl::draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha)
{
   _engine.draw (image, vertices, tex_coords, alpha);
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  CacheImpl::draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color)
{
   _engine.draw (mask, vertices, tex_coords, color);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : release
==============================================================================
*/

void  CacheImpl::release ()
{
   _engine.check_thread_id ();
   
   if (_frame_buffer_id != 0)
   {
      _image.release ();
      
      _engine.release (_frame_buffer_id);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#endif   // (ohm_opal_API == ohm_opal_API_OPENGL)


}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
