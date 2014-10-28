/*****************************************************************************

        ColorXYZ.cpp
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

#include "ohm/opal/ColorXYZ.h"
#include "ohm/lang/fnc.h"

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

ColorXYZ::ColorXYZ ()
:  _X (0.f)
,  _Y (0.f)
,  _Z (0.f)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ColorXYZ::ColorXYZ (const ColorXYZ & other)
:  _X (other._X)
,  _Y (other._Y)
,  _Z (other._Z)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ColorXYZ::ColorXYZ (float X, float Y, float Z)
:  _X (X)
,  _Y (Y)
,  _Z (Z)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ColorXYZ::~ColorXYZ ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ColorXYZ &  ColorXYZ::operator = (const ColorXYZ & other)
{
   _X = other._X;
   _Y = other._Y;
   _Z = other._Z;
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  ColorXYZ::operator == (const ColorXYZ & other) const
{
   return
      (_X == other._X)
      && (_Y == other._Y)
      && (_Z == other._Z)
   ;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  ColorXYZ::operator != (const ColorXYZ &other) const
{
   return ! operator == (other);
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

bool  ColorXYZ::operator < (const ColorXYZ &other) const
{
   int c = lang::cmp (_X, other._X);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_Y, other._Y);
   if (c != 0) return c < 0;

   c = lang::cmp (_Z, other._Z);
   return c < 0;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
