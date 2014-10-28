/*****************************************************************************

        Color.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54700

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

#include "ohm/opal/Color.h"
#include "ohm/opal/ColorEngine.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/lang/fnc.h"
#include "ohm/math/Approx.h"

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

Color Color::interpolate (const Color & color_0, const Color & color_1, float mix_factor)
{
   Color ret_val;
   ret_val._a = lang::limit (color_0._a + mix_factor * (color_1._a - color_0._a), 0.f, 1.f);
   ret_val._r = lang::limit (color_0._r + mix_factor * (color_1._r - color_0._r), 0.f, 1.f);
   ret_val._g = lang::limit (color_0._g + mix_factor * (color_1._g - color_0._g), 0.f, 1.f);
   ret_val._b = lang::limit (color_0._b + mix_factor * (color_1._b - color_0._b), 0.f, 1.f);
   
   return ret_val;
}
   


/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color ()
:  _a (0.f)
,  _r (0.f)
,  _g (0.f)
,  _b (0.f)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Color::~Color ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (float a, float r, float g, float b)
:  _a (a)
,  _r (r)
,  _g (g)
,  _b (b)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (int a8, int r8, int g8, int b8)
:  _a (float (a8) * (1.f / 255.f))
,  _r (float (r8) * (1.f / 255.f))
,  _g (float (g8) * (1.f / 255.f))
,  _b (float (b8) * (1.f / 255.f))
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (float r, float g, float b)
:  _a (1.f)
,  _r (r)
,  _g (g)
,  _b (b)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (int r8, int g8, int b8)
:  _a (1.f)
,  _r (float (r8) * (1.f / 255.f))
,  _g (float (g8) * (1.f / 255.f))
,  _b (float (b8) * (1.f / 255.f))
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (float a, float gray)
:  _a (a)
,  _r (gray)
,  _g (gray)
,  _b (gray)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (int a8, int gray8)
:  _a (float (a8) * (1.f / 255.f))
,  _r (float (gray8) * (1.f / 255.f))
,  _g (float (gray8) * (1.f / 255.f))
,  _b (float (gray8) * (1.f / 255.f))
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (float gray)
:  _a (1.f)
,  _r (gray)
,  _g (gray)
,  _b (gray)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (int gray8)
:  _a (1.f)
,  _r (float (gray8) * (1.f / 255.f))
,  _g (float (gray8) * (1.f / 255.f))
,  _b (float (gray8) * (1.f / 255.f))
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (const Color & other)
:  _a (other._a)
,  _r (other._r)
,  _g (other._g)
,  _b (other._b)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color::Color (const ColorLab & other)
{
   this->operator = (other);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color &  Color::operator = (const Color & other)
{
   _a = other._a;
   _r = other._r;
   _g = other._g;
   _b = other._b;
   
   return *this;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Color &  Color::operator = (const ColorLab & other)
{
   ColorEngine & ce = opal::ColorEngine::use_instance ();
   
   ColorXYZ cxyz;

   ce.to_CIEXYZ (cxyz, other);
   
   ce.to_screen (*this, cxyz);
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Color::operator == (const Color & other) const
{
   if (_a != 0.f)
   {
      return
         (_a == other._a)
         && (_r == other._r)
         && (_g == other._g)
         && (_b == other._b)
      ;
   }
   else
   {
      return other._a == 0.f;
   }
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Color::operator != (const Color &other) const
{
   return ! operator == (other);
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

bool  Color::operator < (const Color &other) const
{
   int c = lang::cmp (_a, other._a);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_r, other._r);
   if (c != 0) return c < 0;

   c = lang::cmp (_g, other._g);
   if (c != 0) return c < 0;

   c = lang::cmp (_b, other._b);
   return c < 0;
}



/*
==============================================================================
Name : colorize
==============================================================================
*/

void  Color::colorize (Bitmap & bitmap)
{
   ColorEngine::use_instance ().colorize_premultiply_in_place (bitmap, *this);
}



/*
==============================================================================
Name : to_PixArgb
==============================================================================
*/

PixArgb  Color::to_PixArgb () const
{
   PixArgb ret_val;
   
   ret_val._a = math::Approx::round_int (_a * 255.f);
   ret_val._r = math::Approx::round_int (_r * 255.f);
   ret_val._g = math::Approx::round_int (_g * 255.f);
   ret_val._b = math::Approx::round_int (_b * 255.f);
   
   return ret_val;
}



const Color Color::_none (0.f, 0.f, 0.f, 0.f);
const Color Color::_black (1.f, 0.f, 0.f, 0.f);
const Color Color::_white (1.f, 1.f, 1.f, 1.f);
const Color Color::_yellow (1.f, 1.f, 1.f, 0.f);
const Color Color::_red (1.f, 1.f, 0.f, 0.f);



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
