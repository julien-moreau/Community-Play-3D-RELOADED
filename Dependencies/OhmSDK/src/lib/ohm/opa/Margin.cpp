/*****************************************************************************

        Margin.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 39765

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

#include "ohm/opa/Margin.h"
#include "ohm/opa/Rect.h"
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

Margin::Margin ()
:  _left (0.f)
,  _top (0.f)
,  _right (0.f)
,  _bottom (0.f)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Margin::Margin (const Rect & rect)
:  _left (- rect._origin._x)
,  _top (- rect._origin._y)
,  _right (rect._origin._x + rect._size._width)
,  _bottom (rect._origin._y + rect._size._height)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Margin::Margin (float left, float top, float right, float bottom)
:  _left (left)
,  _top (top)
,  _right (right)
,  _bottom (bottom)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Margin::Margin (const Margin & other)
:  _left (other._left)
,  _top (other._top)
,  _right (other._right)
,  _bottom (other._bottom)
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Margin & Margin::operator = (const Margin & other)
{
   _left = other._left;
   _top = other._top;
   _right = other._right;
   _bottom = other._bottom;
   
   return *this;
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

Margin & Margin::operator += (const Margin & margin)
{
   _left += margin._left;
   _top += margin._top;
   _right += margin._right;
   _bottom += margin._bottom;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

Margin & Margin::operator -= (const Margin & margin)
{
   _left -= margin._left;
   _top -= margin._top;
   _right -= margin._right;
   _bottom -= margin._bottom;
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Margin::operator == (const Margin &other) const
{
   bool ret_val
      = (_left == other._left)
      && (_top == other._top)
      && (_right == other._right)
      && (_bottom == other._bottom)
   ;
   
   return ret_val;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Margin::operator != (const Margin &other) const
{
   bool ret_val
      = (_left != other._left)
      || (_top != other._top)
      || (_right != other._right)
      || (_bottom != other._bottom)
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

bool  Margin::operator < (const Margin &other) const
{
   int c = lang::cmp (_left, other._left);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_top, other._top);
   if (c != 0) return c < 0;

   c = lang::cmp (_right, other._right);
   if (c != 0) return c < 0;

   c = lang::cmp (_bottom, other._bottom);
   return c < 0;
}



/*
==============================================================================
Name : is_none
==============================================================================
*/

bool  Margin::is_none () const
{
   return operator == (_zero);
}



/*
==============================================================================
Name : integral
Description :
   transform to smallest rectangle with integral coordinates that contains
   '*this'
==============================================================================
*/

void  Margin::integral ()
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

void  Margin::round ()
{
   _left = float (math::round (_left));
   _top = float (math::round (_top));
   _right = float (math::round (_right));
   _bottom = float (math::round (_bottom));
}



/*
==============================================================================
Name : _zero
==============================================================================
*/

const Margin   Margin::_zero (0.f, 0.f, 0.f, 0.f);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +
==============================================================================
*/

Margin   operator + (const Margin & l_op, const Margin & r_op)
{
   Margin ret_val = l_op;
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Margin   operator - (const Margin & l_op, const Margin & r_op)
{
   Margin ret_val = l_op;
   ret_val -= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : integral
Description :
   Returns smallest margin with integral coordinated that contains margin
==============================================================================
*/

Margin   integral (const Margin & margin)
{
   using namespace std;
   
   Margin ret_val;
   ret_val._left = floor (margin._left);
   ret_val._top = floor (margin._top);
   ret_val._right = ceil (margin._right);
   ret_val._bottom = ceil (margin._bottom);
   
   return ret_val;
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
