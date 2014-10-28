/*****************************************************************************

        Point.hpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65617

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



#if defined (ohm_opa_Point_CURRENT_CODEHEADER)
   #error Recursive inclusion of Point code header.
#endif
#define  ohm_opa_Point_CURRENT_CODEHEADER

#if ! defined (ohm_opa_Point_CODEHEADER_INCLUDED)
#define  ohm_opa_Point_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"

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

Point::Point ()
:  _x (0)
,  _y (0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Point::Point (float x, float y)
:  _x (x)
,  _y (y)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Point::Point (const Point & other)
:  _x (other._x)
,  _y (other._y)
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Point &  Point::operator = (const Point & other)
{
   _x = other._x;
   _y = other._y;
   
   return *this;
}



/*
==============================================================================
Name : operator +=
Description :
   offset origin
==============================================================================
*/

Point &  Point::operator += (const Point & point)
{
   _x += point._x;
   _y += point._y;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
Description :
   offset origin
==============================================================================
*/

Point &  Point::operator -= (const Point & point)
{
   _x -= point._x;
   _y -= point._y;
   
   return *this;
}



/*
==============================================================================
Name : operator -
Description :
   neg origin
==============================================================================
*/

Point Point::operator - () const
{
   Point ret_val;
   
   ret_val._x = - _x;
   ret_val._y = - _y;
   
   return ret_val;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Point::operator == (const Point &other) const
{
   bool ret_val
      = (_x == other._x)
      && (_y == other._y)
   ;
   
   return ret_val;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Point::operator != (const Point &other) const
{
   bool ret_val
      = (_x != other._x)
      || (_y != other._y)
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

bool  Point::operator < (const Point &other) const
{
   int c = lang::cmp (_x, other._x);
   if (c != 0) return c < 0;

   c = lang::cmp (_y, other._y);
   return c < 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_Point_CODEHEADER_INCLUDED

#undef ohm_opa_Point_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
