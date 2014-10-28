/*****************************************************************************

        Bitmap.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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

#include "ohm/opal/Bitmap.h"
#include "ohm/opal/ColorEngine.h"
#include "ohm/opal/ColorProcessorInterface.h"

#include <algorithm>

#include <cstring>
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

Bitmap::Bitmap ()
:  _width (0)
,  _height (0)
,  _data_ptr (0)
,  _release_data_flag (false)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Bitmap::Bitmap (const Bitmap &other)
:  _width (0)
,  _height (0)
,  _data_ptr (0)
,  _release_data_flag (false)
{
   if (other._data_ptr != 0)
   {
      using namespace std;
      
      resize (other._width, other._height);
      
      const size_t size = _width * _height;
      
      memcpy (_data_ptr, other._data_ptr, size * sizeof (PixArgb));
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Bitmap::~Bitmap ()
{
   release ();
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Bitmap & Bitmap::operator = (const Bitmap &other)
{
   if (other._data_ptr != 0)
   {
      using namespace std;
      
      resize (other._width, other._height);
      
      const size_t size = _width * _height;
      
      memcpy (_data_ptr, other._data_ptr, size * sizeof (PixArgb));
   }
   
   return *this;
}



/*
==============================================================================
Name : operator []
==============================================================================
*/

PixArgb &   Bitmap::operator [] (size_t idx)
{
   assert (_data_ptr != 0);
   
   assert (idx < _width * _height);
   
   return _data_ptr [idx];
}



/*
==============================================================================
Name : operator []
==============================================================================
*/

const PixArgb &   Bitmap::operator [] (size_t idx) const
{
   assert (_data_ptr != 0);
   
   assert (idx < _width * _height);
   
   return _data_ptr [idx];
}



/*
==============================================================================
Name : operator ()
==============================================================================
*/

PixArgb &   Bitmap::operator () (size_t x, size_t y)
{
   assert (x < _width);
   assert (y < _height);
   
   return _data_ptr [y * _width + x];
}



/*
==============================================================================
Name : operator ()
==============================================================================
*/

const PixArgb &   Bitmap::operator () (size_t x, size_t y) const
{
   assert (x < _width);
   assert (y < _height);
   
   return _data_ptr [y * _width + x];
}



/*
==============================================================================
Name : resize
==============================================================================
*/

void  Bitmap::resize (size_t width, size_t height)
{
   release ();
   
   _width = width;
   _height = height;
   
   _data_ptr = new PixArgb [_width * _height];
   _release_data_flag = true;
}



/*
==============================================================================
Name : swap
==============================================================================
*/

void  Bitmap::swap (Bitmap & other)
{
   std::swap (_width, other._width);
   std::swap (_height, other._height);
   std::swap (_data_ptr, other._data_ptr);
   std::swap (_release_data_flag, other._release_data_flag);
}



/*
==============================================================================
Name : premultiply
==============================================================================
*/

void  Bitmap::premultiply ()
{
   const size_t size = _width * _height;

   for (size_t i = 0 ; i < size ; ++i)
   {
      PixArgb & pix = _data_ptr [i];
      
      int r = pix._r;
      int g = pix._g;
      int b = pix._b;
      int a = pix._a;
      
      r = (r * a) >> 8;
      g = (g * a) >> 8;
      b = (b * a) >> 8;
      
      pix._r = r;
      pix._g = g;
      pix._b = b;
   }
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   Bitmap::size () const
{
   return _width * _height;
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

size_t   Bitmap::get_width () const
{
   return _width;
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

size_t   Bitmap::get_height () const
{
   return _height;
}



/*
==============================================================================
Name : is_fully_transparent
==============================================================================
*/

bool  Bitmap::is_fully_transparent () const
{
   size_t im_size = size ();
   
   bool transparent_flag = true;
   
   for (size_t i = 0 ; i < im_size ; ++i)
   {
      const PixArgb & pix = _data_ptr [i];
      
      if (pix._a != 0)
      {
         transparent_flag = false;
         
         break;
      }
   }
   
   return transparent_flag;
}



/*
==============================================================================
Name : data
==============================================================================
*/

PixArgb *   Bitmap::data ()
{
   // can be null
   
   return _data_ptr;
}



/*
==============================================================================
Name : data
==============================================================================
*/

const PixArgb *   Bitmap::data () const
{
   // can be null
   
   return _data_ptr;
}



/*
==============================================================================
Name : clear_rect
==============================================================================
*/

void  Bitmap::clear_rect (size_t x, size_t y, size_t width, size_t height)
{
   assert (x + width <= _width);

   size_t end_y = y + height;
   assert (end_y <= _height);
   
   for (; y < end_y ; ++y)
   {
      ::memset (&_data_ptr [y * _width + x], 0, width * sizeof (PixArgb));
   }
}



/*
==============================================================================
Name : colorize_premultiply
==============================================================================
*/

void  Bitmap::colorize_premultiply (const Color & color)
{
   ColorEngine & ce = ColorEngine::use_instance ();
   
   ce.colorize_premultiply_in_place (*this, color);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : release
==============================================================================
*/

void  Bitmap::release ()
{
   if (_data_ptr != 0)
   {
      if (_release_data_flag)
      {
         delete [] _data_ptr;
      }
      
      _data_ptr = 0;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
