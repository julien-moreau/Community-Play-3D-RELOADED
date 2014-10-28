/*****************************************************************************

        CacheImpl.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64829

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

#include "ohm/opal/directx/CacheImpl.h"
#include "ohm/opal/Cache.h"
#include "ohm/opal/def.h"
#include "ohm/opal/directx/DirectXEngine.h"

#include "ohm/math/fnc.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace opal
{



#if (ohm_opal_API == ohm_opal_API_DIRECTX)

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

CacheImpl::CacheImpl (GraphicContextInterface & gc, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
:  _engine (gc.use_engine ())
,  _surface_ptr (0)
,  _mem_texture_ptr (0)
,  _size (opa::Size::_zero)
,  _image ()
,  _invalid_flag (false)
,  _invalid_rect ()
{
   _image._name_0 = "DirectX Cache";

   // default
   _conf._sprung_mode_x = sprung_mode_x;
   _conf._sprung_mode_y = sprung_mode_y;
   _conf._filter_mode = filter_mode;

   // add ourselves to the cache manager
   _engine.use_cache_manager ().add (*this);

   //ohm_util_TRACE_DEBUG1 ("CacheManager +    %1%", _engine.use_cache_manager ().size ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

CacheImpl::~CacheImpl ()
{
   try
   {
      release ();

      // remove ourselves from the cache manager
      _engine.use_cache_manager ().remove (*this);

      //ohm_util_TRACE_DEBUG1 ("CacheManager -    %1%", _engine.use_cache_manager ().size ());
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  CacheImpl::release ()
{
   _engine.check_thread_id ();
   _engine.notify_surface_released (_surface_ptr);
   
   if (!_image.empty ())
   {
      assert (_surface_ptr != 0);

      ULONG ref_count = _surface_ptr->Release ();
      _surface_ptr = 0;

      size_t cnt = _image.release ();
      // cnt might not be 0, if the client cached the result of 'get_image'
   }
   else
   {
      assert (_surface_ptr == 0);
   }
}



/*
==============================================================================
Name : reset
==============================================================================
*/

void  CacheImpl::reset ()
{
   assert (_image.empty ());
   assert (_surface_ptr == 0);
   
   resize (_size);
}



/*
==============================================================================
Name : save
Description :
   Save the content of the render target into system memory.
==============================================================================
*/

void  CacheImpl::save ()
{
   if ((!_image.empty ()) && (!_size.is_empty ()))
   {
      ::IDirect3DDevice9 * device_ptr = _engine.get_d3d_device ();
      assert (device_ptr != 0);
      assert (_surface_ptr != 0);
      assert (_mem_texture_ptr == 0);
      
      // create mem surface

      ::HRESULT result = device_ptr->CreateTexture (
         _image._tex_width,
         _image._tex_height,
         1, // levels
         0,
         D3DFMT_A8R8G8B8,
         D3DPOOL_SYSTEMMEM,
         &_mem_texture_ptr,
         0  // reserved
      );
      assert (result == D3D_OK);
      assert (_mem_texture_ptr != 0);
      
      IDirect3DSurface9 * mem_surface_ptr = 0;
      result = _mem_texture_ptr->GetSurfaceLevel (0, &mem_surface_ptr);
      assert (result == D3D_OK);
      assert (mem_surface_ptr != 0);
      
      // copy from video memory to system memory

      result = device_ptr->GetRenderTargetData (
         _surface_ptr, mem_surface_ptr
      );
      assert (result == D3D_OK);
      
      // release
      
      ULONG ref_count = _surface_ptr->Release ();
      _surface_ptr = 0;
      
      ref_count = mem_surface_ptr->Release ();
      mem_surface_ptr = 0;
      
      // don't release the image, but its texture
      _engine.release (_image);
   }
}



/*
==============================================================================
Name : restore
Description :
   Restore the content of the render target from system memory.
==============================================================================
*/

void  CacheImpl::restore ()
{
   if ((!_image.empty ()) && (!_size.is_empty ()))
   {
      ::IDirect3DDevice9 * device_ptr = _engine.get_d3d_device ();
      assert (device_ptr != 0);
      assert (_surface_ptr == 0);
      assert (_mem_texture_ptr != 0);
      
      // create render target
      
      _engine.create (_image, D3DUSAGE_RENDERTARGET);
      
      ::HRESULT result = _image._data_ref->_texture_ptr->GetSurfaceLevel (0, &_surface_ptr);
      assert (result == D3D_OK);
      assert (_surface_ptr != 0);
      
      IDirect3DSurface9 * mem_surface_ptr = 0;
      result = _mem_texture_ptr->GetSurfaceLevel (0, &mem_surface_ptr);
      assert (result == D3D_OK);
      assert (mem_surface_ptr != 0);

      // update texture
      
      result = device_ptr->UpdateSurface (
         mem_surface_ptr, 0, _surface_ptr, 0
      );
      assert (result == D3D_OK);
      
      // release

      ULONG ref_count = mem_surface_ptr->Release ();
      mem_surface_ptr = 0;
      
      ref_count = _mem_texture_ptr->Release ();
      _mem_texture_ptr = 0;
   }
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

   if (_image.empty () || (rounded_size != _size))
   {
      release ();
      
      if (!rounded_size.is_empty ())
      {
         _engine.prepare (
            _image,
            size_t (rounded_size._width), size_t (rounded_size._height)
         );
         
         _engine.create (_image, _conf);

         HRESULT hres = _image._data_ref->_texture_ptr->GetSurfaceLevel (0, &_surface_ptr);
         assert (hres == D3D_OK);
         assert (_surface_ptr != 0);
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
   assert (_surface_ptr != 0);

   _engine.update_context (_surface_ptr, _size, graphic_state);
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

/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#endif   // (ohm_opal_API == ohm_opal_API_DIRECTX)


}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
