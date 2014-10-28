/*****************************************************************************

        Quad.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54636

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

#include "ohm/opa/Quad.h"
#include "ohm/opal/Image.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Quad::Quad ()
:  _is_rect_flag (true)
,  _is_affine_flag (true)
{
   _pt_arr [0] = Point::_zero;
   _pt_arr [1] = Point::_zero;
   _pt_arr [2] = Point::_zero;
   _pt_arr [3] = Point::_zero;
}




/*
==============================================================================
Name : ctor
==============================================================================
*/

Quad::Quad (const Rect & rect)
:  _is_rect_flag (true)
,  _is_affine_flag (true)
{
   _pt_arr [0] = rect._origin;
   _pt_arr [1] = rect._origin + Point (rect._size._width, 0);
   _pt_arr [2] = rect._origin + Point (rect._size._width, rect._size._height);
   _pt_arr [3] = rect._origin + Point (0, rect._size._height);
}




/*
==============================================================================
Name : ctor
==============================================================================
*/

Quad::Quad (const opal::Image & image)
:  _is_rect_flag (true)
,  _is_affine_flag (true)
{
   _pt_arr [0] = Point::_zero;
   _pt_arr [1] = Point (image._tex_s, 0);
   _pt_arr [2] = Point (image._tex_s, image._tex_t);
   _pt_arr [3] = Point (0, image._tex_t);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Quad::Quad (const Point & pt1, const Point & pt2, const Point & pt3, const Point & pt4)
:  _is_rect_flag (false)
,  _is_affine_flag (false)
{
   set (pt1, pt2, pt3, pt4);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Quad::Quad (const Quad & other)
:  _is_rect_flag (other._is_rect_flag)
,  _is_affine_flag (other._is_affine_flag)
{
   for (int i = 0 ; i < 4 ; ++i)
   {
      _pt_arr [i] = other._pt_arr [i];
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Quad::~Quad ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Quad &   Quad::operator = (const Quad & other)
{
   for (int i = 0 ; i < 4 ; ++i)
   {
      _pt_arr [i] = other._pt_arr [i];
   }
   
   _is_rect_flag = other._is_rect_flag;
   _is_affine_flag = other._is_affine_flag;
   
   return *this;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Quad::set (const Point & pt1, const Point & pt2, const Point & pt3, const Point & pt4)
{
   _is_rect_flag = false;
   _is_affine_flag = false;
   
   _pt_arr [0] = pt1;
   _pt_arr [1] = pt2;
   _pt_arr [2] = pt3;
   _pt_arr [3] = pt4;

   float v21x = pt2._x - pt1._x;
   float v21y = pt2._y - pt1._y;

   float v34x = pt3._x - pt4._x;
   float v34y = pt3._y - pt4._y;
   
   if (is_equal (v21x, v34x) && is_equal (v21y, v34y))
   {
      _is_affine_flag = true;
      
      float v32x = pt3._x - pt2._x;
      float v32y = pt3._y - pt2._y;
      
      if (
         (is_equal (v21y, 0) && is_equal (v32x, 0))
         || (is_equal (v21x, 0) && is_equal (v32y, 0))
         )
      {
         _is_rect_flag = true;
      }
   }
}



/*
==============================================================================
Name : is_affine
==============================================================================
*/

bool  Quad::is_affine () const
{
   return _is_affine_flag;
}



/*
==============================================================================
Name : is_rect
==============================================================================
*/

bool  Quad::is_rect () const
{
   return _is_rect_flag;
}



/*
==============================================================================
Name : get_affine_transformation
==============================================================================
*/

void  Quad::get_affine_transformation (float & tx, float & ty, float & a, float & b, float & c, float & d) const
{
   assert (_is_affine_flag);
   
   tx = _pt_arr [0]._x;
   ty = _pt_arr [0]._y;
   
   // 1,0 goes to quad [1]
   
   a = _pt_arr [1]._x;
   b = _pt_arr [1]._y;

   // 0,1 goes to quad [3]
   
   c = _pt_arr [3]._x;
   d = _pt_arr [3]._y;
}



/*
==============================================================================
Name : get_rect
==============================================================================
*/

void  Quad::get_rect (Rect & rect) const
{
   assert (_is_rect_flag);
   
   float min_x = lang::min (_pt_arr [0]._x, _pt_arr [1]._x);
   lang::min_in_place (min_x, _pt_arr [2]._x);

   float max_x = lang::max (_pt_arr [0]._x, _pt_arr [1]._x);
   lang::max_in_place (max_x, _pt_arr [2]._x);

   float min_y = lang::min (_pt_arr [0]._y, _pt_arr [1]._y);
   lang::min_in_place (min_y, _pt_arr [2]._y);

   float max_y = lang::max (_pt_arr [0]._y, _pt_arr [1]._y);
   lang::max_in_place (max_y, _pt_arr [2]._y);
   
   rect._origin._x = min_x;
   rect._origin._y = min_y;
   rect._size._width = max_x - min_x;
   rect._size._height = max_y - min_y;
}



/*
==============================================================================
Name : get_bounding_box
==============================================================================
*/

void  Quad::get_bounding_box (Rect & rect) const
{
   if (_is_rect_flag)
   {
      get_rect (rect);
   }
   else
   {
      float min_x = lang::min (
         lang::min (_pt_arr [0]._x, _pt_arr [1]._x),
         lang::min (_pt_arr [2]._x, _pt_arr [3]._x)
      );

      float max_x = lang::max (
         lang::max (_pt_arr [0]._x, _pt_arr [1]._x),
         lang::max (_pt_arr [2]._x, _pt_arr [3]._x)
      );

      float min_y = lang::min (
         lang::min (_pt_arr [0]._y, _pt_arr [1]._y),
         lang::min (_pt_arr [2]._y, _pt_arr [3]._y)
      );

      float max_y = lang::max (
         lang::max (_pt_arr [0]._y, _pt_arr [1]._y),
         lang::max (_pt_arr [2]._y, _pt_arr [3]._y)
      );

      rect._origin._x = min_x;
      rect._origin._y = min_y;
      rect._size._width = max_x - min_x;
      rect._size._height = max_y - min_y;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
