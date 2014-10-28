/*****************************************************************************

        Rect.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69378

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

#include "ohm/opa/Rect.h"
#include "ohm/opa/Margin.h"
#include "ohm/lang/fnc.h"
#include "ohm/math/fnc.h"

#include <cmath>
#include <cassert>



namespace std {}

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

Rect::Rect ()
:  _origin (Point::_zero)
,  _size (Size::_zero)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Rect::Rect (const Point & origin, const Size & size)
:  _origin (origin)
,  _size (size)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Rect::Rect (const Margin & margin)
:  _origin (- margin._left, - margin._top)
,  _size (margin._left + margin._right, margin._top + margin._bottom)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Rect::Rect (float x, float y, float width, float height)
:  _origin (x, y)
,  _size (width, height)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Rect::Rect (const Rect & other)
:  _origin (other._origin)
,  _size (other._size)
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Rect &   Rect::operator = (const Rect & other)
{
   _origin = other._origin;
   _size = other._size;
   
   return *this;
}



/*
==============================================================================
Name : operator +=
Description :
   offset origin
==============================================================================
*/

Rect &   Rect::operator += (const Point & point)
{
   _origin += point;
   
   return *this;
}



/*
==============================================================================
Name : operator +=
Description :
   offset origin
==============================================================================
*/

Rect &   Rect::operator += (const Size & size)
{
   _origin += size;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
Description :
   offset origin
==============================================================================
*/

Rect &   Rect::operator -= (const Point & point)
{
   _origin -= point;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
Description :
   offset origin
==============================================================================
*/

Rect &   Rect::operator -= (const Size & size)
{
   _origin -= size;
   
   return *this;
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

Rect &   Rect::operator += (const Margin & margin)
{
   _origin._x -= margin._left;
   _origin._y -= margin._top;
   _size._width += margin._left + margin._right;
   _size._height += margin._top + margin._bottom;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

Rect &   Rect::operator -= (const Margin & margin)
{
   _origin._x += margin._left;
   _origin._y += margin._top;
   _size._width -= margin._left + margin._right;
   _size._height -= margin._top + margin._bottom;
   
   return *this;
}



/*
==============================================================================
Name : operator &=
Description :
   intersect with 'rect'
==============================================================================
*/

Rect &   Rect::operator &= (const Rect & rect)
{
   *this = *this & rect;
   
   return *this;
}



/*
==============================================================================
Name : operator |=
Description :
   union with 'rect'
==============================================================================
*/

Rect &   Rect::operator |= (const Rect & rect)
{
   *this = *this | rect;
   
   return *this;
}



/*
==============================================================================
Name : accumulate
Description :
   do nothing if rect is empty
   else union with 'rect' if '*this' is not empty
   other assign 'rect' to '*this'
==============================================================================
*/

Rect &   Rect::accumulate (const Rect & rect)
{
   if (!rect.is_empty ())
   {
      if (is_empty ())
      {
         operator = (rect);
      }
      else
      {
         operator |= (rect);
      }
   }
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Rect::operator == (const Rect &other) const
{
   bool ret_val
      = (_origin == other._origin)
      && (_size == other._size)
   ;
   
   return ret_val;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Rect::operator != (const Rect &other) const
{
   bool ret_val
      = (_origin != other._origin)
      || (_size != other._size)
   ;
   
   return ret_val;
}



/*
==============================================================================
Name : operator <
Note :
   Lexicographical order to embed in ordered containers
==============================================================================
*/

bool  Rect::operator < (const Rect &other) const
{
   int c = lang::cmp (_origin, other._origin);
   if (c != 0) return c < 0;

   c = lang::cmp (_size, other._size);
   return c < 0;
}



/*
==============================================================================
Name : set_left
==============================================================================
*/

void  Rect::set_left (float left)
{
   _origin._x = left;
}



/*
==============================================================================
Name : set_top
==============================================================================
*/

void  Rect::set_top (float top)
{
   _origin._y = top;
}



/*
==============================================================================
Name : set_right
Description :
   set right without modifying origin
==============================================================================
*/

void  Rect::set_right (float right)
{
   _size._width = right - _origin._x;
}



/*
==============================================================================
Name : set_bottom
Description :
   set bottom without modifying origin
==============================================================================
*/

void  Rect::set_bottom (float bottom)
{
   _size._height = bottom - _origin._y;
}



/*
==============================================================================
Name : get_left
==============================================================================
*/

float Rect::get_left () const
{
   return _origin._x;
}



/*
==============================================================================
Name : get_top
==============================================================================
*/

float Rect::get_top () const
{
   return _origin._y;
}



/*
==============================================================================
Name : get_right
==============================================================================
*/

float Rect::get_right () const
{
   return _origin._x + _size._width;
}



/*
==============================================================================
Name : get_bottom
==============================================================================
*/

float Rect::get_bottom () const
{
   return _origin._y + _size._height;
}



/*
==============================================================================
Name : is_null
Description :
   returns true if rect is invalid (that is width or height are strictly
   negative)
==============================================================================
*/

bool  Rect::is_null () const
{
   bool ret_val = _size.is_null ();
   
   return ret_val;
}



/*
==============================================================================
Name : is_empty
Description :
   returns true if rect is null or if rect has either 0 width or height
==============================================================================
*/

bool  Rect::is_empty () const
{
   bool ret_val = _size.is_empty ();
   
   return ret_val;
}



/*
==============================================================================
Name : is_infinite
Description :
   returns true if rect has infinite size
==============================================================================
*/

bool  Rect::is_infinite () const
{
   bool ret_val = _size.is_infinite ();
   
   return ret_val;
}




/*
==============================================================================
Name : contains
Description :
   returns true if '*this' contains 'point'
==============================================================================
*/

bool  Rect::contains (const Point & point) const
{
   bool ret_val = true;
   ret_val &= (point._x >= _origin._x);
   ret_val &= (point._y >= _origin._y);
   ret_val &= (point._x < _origin._x + _size._width);
   ret_val &= (point._y < _origin._y + _size._height);
   
   return ret_val;
}



/*
==============================================================================
Name : contains
Description :
   returns true if '*this' completely contains 'rect'
==============================================================================
*/

bool  Rect::contains (const Rect & rect) const
{
   Rect inter = *this & rect;
   
   bool ret_val = inter == rect;
   
   return ret_val;
}



/*
==============================================================================
Name : integral
Description :
   transform to smallest rectangle with integral coordinates that contains
   '*this'
==============================================================================
*/

void  Rect::integral ()
{
   *this = ohm::opa::integral (*this);
}



/*
==============================================================================
Name : round
Description :
   transform so that each corner is rounded to the to the closest integral
   value.
==============================================================================
*/

void  Rect::round ()
{
   float left = float (math::round (_origin._x));
   float top = float (math::round (_origin._y));
   float right = float (math::round (_origin._x + _size._width));
   float bottom = float (math::round (_origin._y + _size._height));
   
   _origin._x = left;
   _origin._y = top;
   _size._width = right - left;
   _size._height = bottom - top;
}



/*
==============================================================================
Name : is_intersecting
Description :
   returns true if '*this' and 'rect' intersection is non null
==============================================================================
*/

bool  Rect::is_intersecting (const Rect & rect) const
{
   Rect inter = *this & rect;
   
   bool ret_val = !inter.is_empty ();
   
   return ret_val;
}



/*
==============================================================================
Name : standardize
Description :
   returns '*this' as a non null rectangle
==============================================================================
*/

Rect  Rect::standardize () const
{
   Rect ret_val = *this;
   
   if (ret_val._size._width < 0)
   {
      ret_val._size._width = - ret_val._size._width;
      ret_val._origin._x -= ret_val._size._width;
   }
   
   if (ret_val._size._height < 0)
   {
      ret_val._size._height = - ret_val._size._height;
      ret_val._origin._y -= ret_val._size._height;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : get_barycenter
Description :
   returns the barycenter of the 4 points where :
   - 'norm_val_x' is the weight on the x axis
   - 'norm_val_y' is the weight on the y axis
   Examples :
   - .5, .5 gives the center point of the rectangle
   - 1, 1 gives the bottom right point of the rectangle
   
==============================================================================
*/

Point Rect::get_barycenter (float norm_val_x, float norm_val_y) const
{
   return _origin + opa::Size (_size._width * norm_val_x, _size._height * norm_val_y);
}



/*
==============================================================================
Name : from_CGRect
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
void  Rect::from_CGRect (const CGRect & rect)
{
   _origin.from_CGPoint (rect.origin);
   _size.from_CGSize (rect.size);
}
#endif



/*
==============================================================================
Name : to_CGRect
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
CGRect   Rect::to_CGRect () const
{
   CGRect ret_val;
   ret_val.origin = _origin.to_CGPoint ();
   ret_val.size = _size.to_CGSize ();
   
   return ret_val;
}
#endif



#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
void Rect::from_RECT (const ::RECT & rect)
{
   _origin._x = float (rect.left);
   _origin._y = float (rect.top);
   _size._width = float (rect.right - rect.left);
   _size._height = float (rect.bottom - rect.top);
}
#endif



#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
::RECT Rect::to_RECT () const
{
   ::RECT ret_val;
   ret_val.left = LONG (_origin._x);
   ret_val.top = LONG (_origin._y);
   ret_val.bottom = LONG (_origin._x + _size._height);
   ret_val.right = LONG (_origin._y + _size._width);

   return ret_val;
}
#endif



/*
==============================================================================
Name : _zero
==============================================================================
*/

const Rect  Rect::_zero (opa::Point::_zero, opa::Size::_zero);



/*
==============================================================================
Name : _union_stable
Description :
  a rectangle (almost) union-stable, that is :
  _union_stable | x == x is true for (almost) any x.
==============================================================================
*/

const Rect  Rect::_union_stable (1e+34f, 1e+34f, -2e+34f, -2e+34f);



/*
==============================================================================
Name : _intersection_stable
Description :
  a rectangle (almost) intersection-stable, that is :
  _intersection_stable & x == x is true for (almost) any x.
==============================================================================
*/

const Rect  Rect::_intersection_stable (-1e+34f, -1e+34f, 2e+34f, 2e+34f);



/*
==============================================================================
Name : _infinity
Description :
  a rectangle (almost) infinite.
  (pratically the same than _intersection_stable)
==============================================================================
*/

const Rect  Rect::_infinity (-1e+34f, -1e+34f, 2e+34f, 2e+34f);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +
==============================================================================
*/

Rect  operator + (const Rect & l_op, const Point & r_op)
{
   Rect ret_val = l_op;
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator +
==============================================================================
*/

Rect  operator + (const Rect & l_op, const Size & r_op)
{
   Rect ret_val = l_op;
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Rect  operator - (const Rect & l_op, const Point & r_op)
{
   Rect ret_val = l_op;
   ret_val -= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Rect  operator - (const Rect & l_op, const Size & r_op)
{
   Rect ret_val = l_op;
   ret_val -= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator &
Description :
   Returns intersection of 'l_op' and 'r_op'
   (points that are both in 'l_op' and 'r_op')
==============================================================================
*/

Rect  operator & (const Rect & l_op, const Rect & r_op)
{
   Rect ret_val;
   ret_val._origin._x = lang::max (l_op._origin._x, r_op._origin._x);
   ret_val._origin._y = lang::max (l_op._origin._y, r_op._origin._y);
   
   Point l_br = l_op._origin + l_op._size;
   Point r_br = r_op._origin + r_op._size;
   
   Point ret_br;
   ret_br._x = lang::min (l_br._x, r_br._x);
   ret_br._y = lang::min (l_br._y, r_br._y);
   
   ret_val._size._width = ret_br._x - ret_val._origin._x;
   ret_val._size._height = ret_br._y - ret_val._origin._y;
   
   return ret_val;
}



/*
==============================================================================
Name : operator |
Description :
   Returns union of 'l_op' and 'r_op'
   (smallest rectangle containing points which are either in 'l_op' or 'r_op'
   or both)
==============================================================================
*/

Rect  operator | (const Rect & l_op, const Rect & r_op)
{
   Rect ret_val;
   ret_val._origin._x = lang::min (l_op._origin._x, r_op._origin._x);
   ret_val._origin._y = lang::min (l_op._origin._y, r_op._origin._y);
   
   Point l_br = l_op._origin + l_op._size;
   Point r_br = r_op._origin + r_op._size;
   
   Point ret_br;
   ret_br._x = lang::max (l_br._x, r_br._x);
   ret_br._y = lang::max (l_br._y, r_br._y);
   
   ret_val._size._width = ret_br._x - ret_val._origin._x;
   ret_val._size._height = ret_br._y - ret_val._origin._y;
   
   return ret_val;
}



/*
==============================================================================
Name : integral
Description :
   Returns smallest rectangle with integral coordinated that contains 'rect'
==============================================================================
*/

Rect  integral (const Rect & rect)
{
   using namespace std;
   
   Rect ret_val;
   Point br = rect._origin + rect._size;
   
   ret_val._origin = lower_integral (rect._origin);
   
   Point br_int = upper_integral (br) - ret_val._origin;
   ret_val._size._width = br_int._x;
   ret_val._size._height = br_int._y;
   
   return ret_val;
}



/*
==============================================================================
Name : are_intersecting
Description :
   Returns true is 'rect_0' and 'rect_1' intersection is non null
==============================================================================
*/

bool  are_intersecting (const Rect & rect_0, const Rect & rect_1)
{
   Rect inter = rect_0 & rect_1;
   
   bool ret_val = !inter.is_null ();
   
   return ret_val;
}



/*
==============================================================================
Name : standardize
Description :
   Returns 'rect' as a non null rectangle
==============================================================================
*/

Rect  standardize (const Rect & rect)
{
   Rect ret_val = rect;
   
   if (ret_val._size._width < 0)
   {
      ret_val._size._width = - ret_val._size._width;
      ret_val._origin._x -= ret_val._size._width;
   }
   
   if (ret_val._size._height < 0)
   {
      ret_val._size._height = - ret_val._size._height;
      ret_val._origin._y -= ret_val._size._height;
   }
   
   return ret_val;
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
