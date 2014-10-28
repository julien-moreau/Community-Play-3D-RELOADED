/*****************************************************************************

        SprungImage.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/opal/SprungImage.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/dsp/Cropper.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

SprungImage::SprungImage ()
:  _eng_ptr (0)
{
   for (int i = 0 ; i < NBR_Position_TOTAL ; ++i)
   {
      _col_width [i] = 0;
      _row_height [i] = 0;
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

SprungImage::SprungImage (const SprungImage & other)
:  _eng_ptr (other._eng_ptr)
{
   this->operator = (other);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

SprungImage::SprungImage (GraphicContextInterface & gc, const Bitmap & bitmap, const opa::Rect & rect, SprungMode sprung_mode_x, SprungMode sprung_mode_y, FilterMode filter_mode)
:  _eng_ptr (&gc.use_engine ())
{
   opa::Size image_size (float (bitmap.get_width ()), float (bitmap.get_height ()));
   
   assert (rect._origin._x >= 0);
   assert (rect._origin._y >= 0);
   assert (rect._origin._x + rect._size._width <= image_size._width);
   assert (rect._origin._y + rect._size._height <= image_size._height);
   
   float last_width = image_size._width - rect._size._width - rect._origin._x;
   float last_height = image_size._height - rect._size._height - rect._origin._y;
   
   opa::Rect rect_0 (0, 0, rect._origin._x, rect._origin._y);
   opa::Rect rect_1 (rect._origin._x, 0, rect._size._width, rect._origin._y);
   opa::Rect rect_2 (
      rect._size._width + rect._origin._x, 0,
      last_width, rect._origin._y
   );
   
   opa::Rect rect_3 (0, rect._origin._y, rect._origin._x, rect._size._height);
   opa::Rect rect_4 (rect._origin._x, rect._origin._y, rect._size._width, rect._size._height);
   opa::Rect rect_5 (
      rect._size._width + rect._origin._x, rect._origin._y,
      last_width, rect._size._height
   );
   
   opa::Rect rect_6 (0, rect._origin._y + rect._size._height, rect._origin._x, last_height);
   opa::Rect rect_7 (rect._origin._x, rect._origin._y + rect._size._height, rect._size._width, last_height);
   opa::Rect rect_8 (
      rect._size._width + rect._origin._x, rect._origin._y + rect._size._height,
      last_width, last_height
   );
   
   Cropper cropper;

   if (!rect_0.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_0._origin._x), 
         archi::Int32 (rect_0._origin._y), 
         archi::UInt32 (rect_0._size._width), 
         archi::UInt32 (rect_0._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [0] = Image (
            gc, sub_bitmap, SprungMode_DISTEND, SprungMode_DISTEND, filter_mode
         );
      }
   }

   if (!rect_1.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_1._origin._x), 
         archi::Int32 (rect_1._origin._y), 
         archi::UInt32 (rect_1._size._width), 
         archi::UInt32 (rect_1._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [1] = Image (
            gc, sub_bitmap, sprung_mode_x, SprungMode_DISTEND, filter_mode
         );
      }
   }

   if (!rect_2.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_2._origin._x), 
         archi::Int32 (rect_2._origin._y), 
         archi::UInt32 (rect_2._size._width), 
         archi::UInt32 (rect_2._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [2] = Image (
            gc, sub_bitmap, SprungMode_DISTEND, SprungMode_DISTEND, filter_mode
         );
      }
   }

   if (!rect_3.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_3._origin._x), 
         archi::Int32 (rect_3._origin._y), 
         archi::UInt32 (rect_3._size._width), 
         archi::UInt32 (rect_3._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [3] = Image (
            gc, sub_bitmap, SprungMode_DISTEND, sprung_mode_y, filter_mode
         );
      }
   }

   if (!rect_4.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_4._origin._x), 
         archi::Int32 (rect_4._origin._y), 
         archi::UInt32 (rect_4._size._width), 
         archi::UInt32 (rect_4._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [4] = Image (
            gc, sub_bitmap, sprung_mode_x, sprung_mode_y, filter_mode
         );
      }
   }

   if (!rect_5.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_5._origin._x), 
         archi::Int32 (rect_5._origin._y), 
         archi::UInt32 (rect_5._size._width), 
         archi::UInt32 (rect_5._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [5] = Image (
            gc, sub_bitmap, SprungMode_DISTEND, sprung_mode_y, filter_mode
         );
      }
   }

   if (!rect_6.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_6._origin._x), 
         archi::Int32 (rect_6._origin._y), 
         archi::UInt32 (rect_6._size._width), 
         archi::UInt32 (rect_6._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [6] = Image (
            gc, sub_bitmap, SprungMode_DISTEND, SprungMode_DISTEND, filter_mode
         );
      }
   }

   if (!rect_7.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_7._origin._x), 
         archi::Int32 (rect_7._origin._y), 
         archi::UInt32 (rect_7._size._width), 
         archi::UInt32 (rect_7._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [7] = Image (
            gc, sub_bitmap, sprung_mode_x, SprungMode_DISTEND, filter_mode
         );
      }
   }

   if (!rect_8.is_empty ())
   {
      Bitmap sub_bitmap;
      cropper.set (
         archi::Int32 (rect_8._origin._x), 
         archi::Int32 (rect_8._origin._y), 
         archi::UInt32 (rect_8._size._width), 
         archi::UInt32 (rect_8._size._height)
      );
      cropper.process (sub_bitmap, bitmap);
      
      if (!sub_bitmap.is_fully_transparent ())
      {
         _image_arr [8] = Image (
            gc, sub_bitmap, SprungMode_DISTEND, SprungMode_DISTEND, filter_mode
         );
      }
   }

   _col_width [Position_FIRST] = size_t (rect_0._size._width);
   _col_width [Position_MIDDLE] = size_t (rect_1._size._width);
   _col_width [Position_LAST] = size_t (rect_2._size._width);

   _row_height [Position_FIRST] = size_t (rect_0._size._height);
   _row_height [Position_MIDDLE] = size_t (rect_3._size._height);
   _row_height [Position_LAST] = size_t (rect_6._size._height);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

SprungImage &  SprungImage::operator = (const SprungImage & other)
{
   size_t i;
   
   for (i = 0 ; i < NBR_Position_TOTAL ; ++i)
   {
      _col_width [i] = other._col_width [i];
      _row_height [i] = other._row_height [i];
   }
   
   for (i = 0 ; i < NBR_IMAGE ; ++i)
   {
      _image_arr [i] = other._image_arr [i];
   }
   
   _eng_ptr = other._eng_ptr;
   
   return *this;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SprungImage::~SprungImage ()
{
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  SprungImage::empty () const
{
   return (get_width () == 0) && (get_height () == 0);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

size_t   SprungImage::get_width () const
{
   return _col_width [0] + _col_width [1] + _col_width [2];
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

size_t   SprungImage::get_height () const
{
   return _row_height [0] + _row_height [1] + _row_height [2];
}



/*
==============================================================================
Name : get_col_width
==============================================================================
*/

size_t   SprungImage::get_col_width (Position position) const
{
   assert (position < NBR_Position_TOTAL);
   
   return _col_width [position];
}



/*
==============================================================================
Name : get_row_height
==============================================================================
*/

size_t   SprungImage::get_row_height (Position position) const
{
   assert (position < NBR_Position_TOTAL);
   
   return _row_height [position];
}



/*
==============================================================================
Name : get_cols_width
==============================================================================
*/

void  SprungImage::get_cols_width (size_t & first, size_t & middle, size_t & last, const opa::Rect & rect) const
{
   first = _col_width [Position_FIRST];
   last = _col_width [Position_LAST];
   middle = size_t (rect._size._width) - first - last;
}



/*
==============================================================================
Name : get_rows_height
==============================================================================
*/

void  SprungImage::get_rows_height (size_t & first, size_t & middle, size_t & last, const opa::Rect & rect) const
{
   first = _row_height [Position_FIRST];
   last = _row_height [Position_LAST];
   middle = size_t (rect._size._height) - first - last;
}



/*
==============================================================================
Name : operator ()
==============================================================================
*/

const Image &  SprungImage::operator () (Position col, Position row) const
{
   assert (col < NBR_Position_TOTAL);
   assert (row < NBR_Position_TOTAL);
   
   return _image_arr [row * NBR_Position_TOTAL + col];
}



/*
==============================================================================
Name : operator ()
==============================================================================
*/

Image &  SprungImage::operator () (Position col, Position row)
{
   assert (col < NBR_Position_TOTAL);
   assert (row < NBR_Position_TOTAL);
   
   return _image_arr [row * NBR_Position_TOTAL + col];
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  SprungImage::operator == (const SprungImage & other) const
{
   size_t i;
   
   for (i = 0 ; i < NBR_Position_TOTAL ; ++i)
   {
      if (_col_width [i] != other._col_width [i])
      {
         return false;
      }
      
      if (_row_height [i] != other._row_height [i])
      {
         return false;
      }
   }
   
   for (i = 0 ; i < NBR_IMAGE ; ++i)
   {
      if (_image_arr [i] != other._image_arr [i])
      {
         return false;
      }
   }
   
   return true;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  SprungImage::operator != (const SprungImage & other) const
{
   return !this->operator == (other);
}



/*
==============================================================================
Name : retain
==============================================================================
*/

void  SprungImage::retain ()
{
   for (size_t i = 0 ; i < NBR_IMAGE ; ++i)
   {
      _image_arr [i].retain ();
   }
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  SprungImage::release ()
{
   for (size_t i = 0 ; i < NBR_IMAGE ; ++i)
   {
      _image_arr [i].release ();
   }
}



/*
==============================================================================
Name : release_check_total
==============================================================================
*/

void  SprungImage::release_check_total ()
{
   for (size_t i = 0 ; i < NBR_IMAGE ; ++i)
   {
      size_t ref_cnt = _image_arr [i].release ();
      assert (ref_cnt == 0);
   }
}



/*
==============================================================================
Name : hole_center
==============================================================================
*/

void  SprungImage::hole_center ()
{
   static Image empty_image;
   
   _image_arr [4] = empty_image;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
