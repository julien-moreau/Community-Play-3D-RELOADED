/*****************************************************************************

        ColorLab.cpp
        Copyright (c) 2010 Ohm Force

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

#include "ohm/opal/ColorLab.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : interpolate
==============================================================================
*/

ColorLab ColorLab::interpolate (const ColorLab & color_0, const ColorLab & color_1, float mix_factor)
{
   ColorLab ret_val;
   ret_val._L = lang::limit (color_0._L + mix_factor * (color_1._L - color_0._L), 0.f, 100.f);
   ret_val._a = lang::limit (color_0._a + mix_factor * (color_1._a - color_0._a), -128.f, 128.f);
   ret_val._b = lang::limit (color_0._b + mix_factor * (color_1._b - color_0._b), -128.f, 128.f);
   
   return ret_val;
}
   


/*
==============================================================================
Name : ctor
==============================================================================
*/

ColorLab::ColorLab ()
:  _L (0.f)
,  _a (0.f)
,  _b (0.f)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ColorLab::ColorLab (const ColorLab & other)
:  _L (other._L)
,  _a (other._a)
,  _b (other._b)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ColorLab::ColorLab (float L, float a, float b)
:  _L (L)
,  _a (a)
,  _b (b)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ColorLab::~ColorLab ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ColorLab &  ColorLab::operator = (const ColorLab & other)
{
   _L = other._L;
   _a = other._a;
   _b = other._b;
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  ColorLab::operator == (const ColorLab & other) const
{
   return
      (_L == other._L)
      && (_a == other._a)
      && (_b == other._b)
   ;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  ColorLab::operator != (const ColorLab &other) const
{
   return ! operator == (other);
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

bool  ColorLab::operator < (const ColorLab &other) const
{
   int c = lang::cmp (_L, other._L);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_a, other._a);
   if (c != 0) return c < 0;

   c = lang::cmp (_b, other._b);
   return c < 0;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
