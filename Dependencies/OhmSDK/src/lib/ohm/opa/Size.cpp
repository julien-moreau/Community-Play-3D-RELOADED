/*****************************************************************************

        Size.cpp
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

#include "ohm/opa/Size.h"
#include "ohm/opa/Point.h"
#include "ohm/math/fnc.h"

#include <cassert>
#include <cmath>
#include <cfloat>



namespace std {}

namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +=
==============================================================================
*/

Size &   Size::operator += (const Point & point)
{
   _width += point._x;
   _height += point._y;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

Size &   Size::operator -= (const Point & point)
{
   _width -= point._x;
   _height -= point._y;
   
   return *this;
}



/*
==============================================================================
Name : lower_integral
Description :
   floor coordinates
==============================================================================
*/

void  Size::lower_integral ()
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

void  Size::upper_integral ()
{
   *this = ohm::opa::upper_integral (*this);
}



/*
==============================================================================
Name : nearest_integral
Description :
   round coordinates
==============================================================================
*/

void  Size::nearest_integral ()
{
   *this = ohm::opa::nearest_integral (*this);
}



/*
==============================================================================
Name : from_CGSize
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
void  Size::from_CGSize (const CGSize & size)
{
   _width = size.width;
   _height = size.height;
}
#endif



/*
==============================================================================
Name : to_CGSize
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
CGSize   Size::to_CGSize () const
{
   CGSize ret_val;
   ret_val.width = _width;
   ret_val.height = _height;
   
   return ret_val;
}
#endif



const Size  Size::_zero (0, 0);
const Size  Size::_infinity (FLT_MAX, FLT_MAX);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator +
==============================================================================
*/

Size  operator + (const Size & l_op, const Point & r_op)
{
   Size ret_val = l_op;
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Size  operator - (const Size & l_op, const Point & r_op)
{
   Size ret_val = l_op;
   ret_val -= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator +
==============================================================================
*/

Size  operator + (const Size & l_op, const Size & r_op)
{
   Size ret_val = l_op;
   ret_val += r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Size  operator - (const Size & l_op, const Size & r_op)
{
   Size ret_val = l_op;
   ret_val -= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

Size operator * (const Size & l_op, float r_op)
{
   Size ret_val = l_op;
   ret_val *= r_op;
   
   return ret_val;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

Size operator * (float l_op, const Size & r_op)
{
   Size ret_val = r_op;
   ret_val *= l_op;
   
   return ret_val;
}



/*
==============================================================================
Name : lower_integral
==============================================================================
*/

Size  lower_integral (const Size & other)
{
   using namespace std;
   
   Size ret_val (
      floor (other._width),
      floor (other._height)
   );
   
   return ret_val;
}



/*
==============================================================================
Name : upper_integral
==============================================================================
*/

Size  upper_integral (const Size & other)
{
   using namespace std;
   
   Size ret_val (
      ceil (other._width),
      ceil (other._height)
   );
   
   return ret_val;
}



/*
==============================================================================
Name : nearest_integral
==============================================================================
*/

Size  nearest_integral (const Size & other)
{
   using namespace std;
   
   Size ret_val (
      float (math::round (other._width)),
      float (math::round (other._height))
   );
   
   return ret_val;
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
