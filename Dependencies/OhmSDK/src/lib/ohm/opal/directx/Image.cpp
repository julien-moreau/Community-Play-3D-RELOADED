/*****************************************************************************

        Image.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63402

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

#include "ohm/opal/Image.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/directx/DirectXEngine.h"
#include "ohm/util/trace.h"

#include <vector>
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

Image::Image ()
:  _data_ref (0)
,  _name_0 (0)
,  _width (0)
,  _height (0)
,  _tex_width (0)
,  _tex_height (0)
,  _tex_s (0.f)
,  _tex_t (0.f)
,  _sprung_mode_x (SprungMode_DISTEND)
,  _sprung_mode_y (SprungMode_DISTEND)
,  _software_wrap_repeat_x_flag (false)
,  _software_wrap_repeat_y_flag (false)
,  _scanline_width (0)
,  _filter_mode (FilterMode_NEAREST)
,  _eng_ptr (0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Image::Image (const Image & other)
:  _data_ref (other._data_ref)
,  _name_0 (other._name_0)
,  _width (other._width)
,  _height (other._height)
,  _tex_width (other._tex_width)
,  _tex_height (other._tex_height)
,  _tex_s (other._tex_s)
,  _tex_t (other._tex_t)
,  _sprung_mode_x (other._sprung_mode_x)
,  _sprung_mode_y (other._sprung_mode_y)
,  _software_wrap_repeat_x_flag (other._software_wrap_repeat_x_flag)
,  _software_wrap_repeat_y_flag (other._software_wrap_repeat_y_flag)
,  _scanline_width (other._scanline_width)
,  _filter_mode (other._filter_mode)
,  _eng_ptr (other._eng_ptr)
{
   retain ();
}



/*
==============================================================================
Name : ctor
Note :
   The input 'premultiplied_bitmap' must have its color already premultiplied
   for the opal system to render correct blends.
==============================================================================
*/

Image::Image (GraphicContextInterface & gc, const Bitmap & premultiplied_bitmap, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
:  _data_ref (0)
,  _name_0 (0)
,  _width (0)
,  _height (0)
,  _tex_width (0)
,  _tex_height (0)
,  _tex_s (0.f)
,  _tex_t (0.f)
,  _sprung_mode_x (sprung_mode_x)
,  _sprung_mode_y (sprung_mode_y)
,  _software_wrap_repeat_x_flag (false)
,  _software_wrap_repeat_y_flag (false)
,  _scanline_width (0)
,  _filter_mode (filter_mode)
,  _eng_ptr (&gc.use_engine ())
{
   using namespace std;
   
   if (premultiplied_bitmap.is_fully_transparent ())
   {
      ohm_util_TRACE_WARNING ("Tried to load a fully transparent image.");
   }
   else
   {
      assert (_eng_ptr != 0);
      _eng_ptr->prepare (
         *this,
         premultiplied_bitmap.get_width (),
         premultiplied_bitmap.get_height ()
      );
      
      PixArgb * buf_ptr = new PixArgb [_scanline_width * _tex_height];
      
      memset (buf_ptr, 0, _scanline_width * _tex_height * sizeof (PixArgb));
      
      for (size_t y = 0 ; y < premultiplied_bitmap.get_height () ; ++y)
      {
         memcpy (
            &buf_ptr [_scanline_width * y],
            &premultiplied_bitmap [premultiplied_bitmap.get_width () * y],
            premultiplied_bitmap.get_width () * sizeof (PixArgb)
         );
      }
      
      Conf conf;
      conf._sprung_mode_x = sprung_mode_x;
      conf._sprung_mode_y = sprung_mode_y;
      conf._filter_mode = filter_mode;
      
      bool buf_owner_flag = true;
      create (gc.use_engine (), buf_ptr, conf, buf_owner_flag);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Image::Image (Engine & engine, const PixArgb * pix_ptr, size_t width, size_t scanline_width, size_t height, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
:  _data_ref (0)
,  _name_0 (0)
,  _width (0)
,  _height (0)
,  _tex_width (0)
,  _tex_height (0)
,  _tex_s (0.f)
,  _tex_t (0.f)
,  _sprung_mode_x (sprung_mode_x)
,  _sprung_mode_y (sprung_mode_y)
,  _software_wrap_repeat_x_flag (false)
,  _software_wrap_repeat_y_flag (false)
,  _scanline_width (0)
,  _filter_mode (filter_mode)
,  _eng_ptr (&engine)
{
   using namespace std;
   
   assert (_eng_ptr != 0);
   _eng_ptr->prepare (*this, width, height);
   assert (_scanline_width == scanline_width);
   
   PixArgb * buf_ptr = new PixArgb [_scanline_width * _tex_height];
   
   memset (buf_ptr, 0, _scanline_width * _tex_height * sizeof (PixArgb));
   memcpy (buf_ptr, pix_ptr, _scanline_width * height * sizeof (PixArgb));
   
   Conf conf;
   conf._sprung_mode_x = sprung_mode_x;
   conf._sprung_mode_y = sprung_mode_y;
   conf._filter_mode = filter_mode;
   
   bool buf_owner_flag = true;
   create (engine, buf_ptr, conf, buf_owner_flag);
}



/*
==============================================================================
Name : operator =
Description :
==============================================================================
*/

Image &  Image::operator = (const Image & other)
{
   // image might have not been initialized
   
   if (_data_ref != other._data_ref)
   {
      if (_data_ref != 0)
      {
         release ();
      }
      
      _data_ref = other._data_ref;
      _name_0 = other._name_0;
      _width = other._width;
      _height = other._height;
      _tex_width = other._tex_width;
      _tex_height = other._tex_height;
      _tex_s = other._tex_s;
      _tex_t = other._tex_t;
      _sprung_mode_x = other._sprung_mode_x;
      _sprung_mode_y = other._sprung_mode_y;
      _software_wrap_repeat_x_flag = other._software_wrap_repeat_x_flag;
      _software_wrap_repeat_y_flag = other._software_wrap_repeat_y_flag;
      _scanline_width = other._scanline_width;
      _filter_mode = other._filter_mode;
      
      retain ();
   }
   
   _eng_ptr = other._eng_ptr;
   
   return *this;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Image::~Image ()
{
   // image might have not been initialized
   
   if (_data_ref != 0)
   {
      release ();
   }
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Image::operator == (const Image &other) const
{
   return _data_ref == other._data_ref;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Image::operator != (const Image &other) const
{
   return _data_ref != other._data_ref;
}




/*
==============================================================================
Name : empty
==============================================================================
*/

bool  Image::empty () const
{
   return _data_ref == 0;
}



/*
==============================================================================
Name : retain
==============================================================================
*/

size_t   Image::retain ()
{
   size_t ret_val = 0;
   
   if (_data_ref != 0)
   {
      _data_ref->_retain_cnt += 1;
      
      ret_val = _data_ref->_retain_cnt;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : release
==============================================================================
*/

size_t   Image::release ()
{
   size_t ret_val = 0;
   
   if (_data_ref != 0)
   {
      assert (_data_ref->_retain_cnt > 0);
      
      _data_ref->_retain_cnt -= 1;
      
      ret_val = _data_ref->_retain_cnt;
      
      if (_data_ref->_retain_cnt  == 0)
      {
         assert (_data_ref->_texture_ptr != 0);
         
         assert (_eng_ptr != 0);
         _eng_ptr->release (*this);

         if (_data_ref->_buf_owner_flag)
         {
            delete [] _data_ref->_buf_ptr;
         }
         _data_ref->_buf_ptr = 0;
         
         delete _data_ref;
      }

      _data_ref = 0;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : create
==============================================================================
*/

void  Image::create (Engine & engine, const PixArgb * buf_ptr, const Conf & conf, bool buf_owner_flag)
{
   engine.create (*this, buf_ptr, conf, buf_owner_flag);
   
   _eng_ptr = &engine;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
