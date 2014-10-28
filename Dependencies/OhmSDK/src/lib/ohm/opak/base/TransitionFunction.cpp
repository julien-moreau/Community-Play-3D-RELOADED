/*****************************************************************************

        TransitionFunction.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53160

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

#include "ohm/opak/base/TransitionFunction.h"
#include "ohm/math/def.h"

#include <cmath>
#include <cassert>



namespace ohm
{
namespace opak
{



/*
==============================================================================
Name : step
==============================================================================
*/

float step (float t)
{
   if (t >= 1.f)
   {
      return 1.f;
   }
   else
   {
      return 0.f;
   }
}



/*
==============================================================================
Name : inverse_step
==============================================================================
*/

float inverse_step (float t)
{
   if (t > 0.f)
   {
      return 1.f;
   }
   else
   {
      return 0.f;
   }
}



/*
==============================================================================
Name : linear
==============================================================================
*/

float linear (float t)
{
   return t;
}



/*
==============================================================================
Name : ease_in
==============================================================================
*/

float ease_in (float t)
{
   return t * t;
}



/*
==============================================================================
Name : ease_out
==============================================================================
*/

float ease_out (float t)
{
   return 2 * t - t * t;
}



/*
==============================================================================
Name : ease_in_ease_out
==============================================================================
*/

float ease_in_ease_out (float t)
{
   float t2 = t * t;
   
   return - 2 * t * t2 + 3 * t2;
}



/*
==============================================================================
Name : elastic
==============================================================================
*/

float elastic (float t)
{
   using namespace std;
   
   return 0.25f * float (exp (-t) * sin (4.0 * math::PI * t) + (1.0 - exp (-10.0 * t)));
}



}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
