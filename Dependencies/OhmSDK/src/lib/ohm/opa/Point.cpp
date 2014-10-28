/*****************************************************************************

        Point.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"
#include "ohm/math/fnc.h"

#include <cassert>
#include <cmath>




namespace std {}

namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +=
Description :
   offset origin
==============================================================================
*/

Point &  Point::operator += (const Size & size)
{
   _x += size._width;
   _y += size._height;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
Description :
   offset origin
==============================================================================
*/

Point &  Point::operator -= (const Size & size)
{
   _x -= size._width;
   _y -= size._height;
   
   return *this;
}



/*
==============================================================================
Name : lower_integral
Description :
   floor coordinates
==============================================================================
*/

void  Point::lower_integral ()
{
   *this = ohm::opa::lower_integral (*this);
}



/*
==============================================================================
Name : upper_integral
Description :
   ceil coordinates
==============================================================================
*/

void  Point::upper_integral ()
{
   *this = ohm::opa::upper_integral (*this);
}



/*
==============================================================================
Name : ceil
Description :
   round coordinates
==============================================================================
*/

void  Point::nearest_integral ()
{
   *this = ohm::opa::nearest_integral (*this);
}



/*
==============================================================================
Name : from_CGPoint
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
void  Point::from_CGPoint (const CGPoint & point)
{
   _x = point.x;
   _y = point.y;
}
#endif



/*
==============================================================================
Name : to_CGPoint
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
CGPoint  Point::to_CGPoint () const
{
   CGPoint ret_val;
   ret_val.x = _x;
   ret_val.y = _y;
   
   return ret_val;
}
#endif



const Point Point::_zero (0, 0);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +
==============================================================================
*/

Point operator + (const Point & l_op, const Point & r_op)
{
   Point ret_val = l_op;
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Point operator - (const Point & l_op, const Point & r_op)
{
   Point ret_val = l_op;
   ret_val -= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator +
==============================================================================
*/

Point operator + (const Point & l_op, const Size & r_op)
{
   Point ret_val = l_op;
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Point operator - (const Point & l_op, const Size & r_op)
{
   Point ret_val = l_op;
   ret_val -= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : lower_integral
==============================================================================
*/

Point lower_integral (const Point & other)
{
   using namespace std;
   
   Point ret_val (
      floor (other._x),
      floor (other._y)
   );
   
   return ret_val;
}



/*
==============================================================================
Name : upper_integral
==============================================================================
*/

Point upper_integral (const Point & other)
{
   using namespace std;
   
   Point ret_val (
      ceil (other._x),
      ceil (other._y)
   );
   
   return ret_val;
}



/*
==============================================================================
Name : nearest_integral
==============================================================================
*/

Point nearest_integral (const Point & other)
{
   using namespace std;
   
   Point ret_val (
      float (math::round (other._x)),
      float (math::round (other._y))
   );
   
   return ret_val;
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
