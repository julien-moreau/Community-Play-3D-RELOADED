/*****************************************************************************

        Size.hpp
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



#if defined (ohm_opa_Size_CURRENT_CODEHEADER)
   #error Recursive inclusion of Size code header.
#endif
#define  ohm_opa_Size_CURRENT_CODEHEADER

#if ! defined (ohm_opa_Size_CODEHEADER_INCLUDED)
#define  ohm_opa_Size_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/math/Approx.h"

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

Size::Size ()
:  _width (0)
,  _height (0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Size::Size (float x, float y)
:  _width (x)
,  _height (y)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Size::Size (const Size & other)
:  _width (other._width)
,  _height (other._height)
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Size &   Size::operator = (const Size & other)
{
   _width = other._width;
   _height = other._height;
   
   return *this;
}



/*
==============================================================================
Name : operator +=
Description :
   offset origin
==============================================================================
*/

Size &   Size::operator += (const Size & size)
{
   _width += size._width;
   _height += size._height;
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

Size &   Size::operator -= (const Size & size)
{
   _width -= size._width;
   _height -= size._height;
   
   return *this;
}



/*
==============================================================================
Name : operator *=
==============================================================================
*/

Size &   Size::operator *= (float scale)
{
   _width *= scale;
   _height *= scale;
   
   return *this;
}



/*
==============================================================================
Name : operator -
==============================================================================
*/

Size  Size::operator - ()
{
   Size ret_val;
   
   ret_val._width = - _width;
   ret_val._height = - _height;
   
   return ret_val;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Size::operator == (const Size &other) const
{
   bool ret_val
      = (_width == other._width)
      && (_height == other._height)
   ;
   
   return ret_val;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Size::operator != (const Size &other) const
{
   assert (&other != 0);
   
   bool ret_val
      = (_width != other._width)
      || (_height != other._height)
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

bool  Size::operator < (const Size &other) const
{
   int c = lang::cmp (_width, other._width);
   if (c != 0) return c < 0;

   c = lang::cmp (_height, other._height);
   return c < 0;
}



/*
==============================================================================
Name : is_null
Description :
   returns true if _width or _height are strictly negative
==============================================================================
*/

bool  Size::is_null () const
{
   bool ret_val = (_width < 0) || (_height < 0);
   
   return ret_val;
}



/*
==============================================================================
Name : is_empty
   returns true if it is null or either width or height is 0
==============================================================================
*/

bool  Size::is_empty () const
{
   bool ret_val = (_width <= 0) || (_height <= 0);
   
   return ret_val;
}



/*
==============================================================================
Name : is_infinite
==============================================================================
*/

bool  Size::is_infinite () const
{
   using namespace std;
   
   /*bool ret_val
      = ((_width > 0) && (isinf (_width) != 0))
      || ((_height > 0) && (isinf (_height) != 0))
   ;
   
   if (!ret_val)
   {
      ret_val = (_width == HUGE_VALF) || (_height == HUGE_VALF);
   }*/
   
   bool ret_val = (_width >= FLT_MAX) || (_height >= FLT_MAX);
   
   return ret_val;
}



/*
==============================================================================
Name : norm_1
==============================================================================
*/

float Size::norm_1 () const
{
   return (lang::abs (_width) + lang::abs (_height));
}



/*
==============================================================================
Name : norm_2
==============================================================================
*/

float Size::norm_2 () const
{
   using namespace std;
   
   return sqrt (_width * _width + _height * _height);
}



/*
==============================================================================
Name : norm_2_sqr
==============================================================================
*/

float Size::norm_2_sqr () const
{
   return _width * _width + _height * _height;
}



/*
==============================================================================
Name : inv_norm_2
==============================================================================
*/

float Size::inv_norm_2 () const
{
   // in the case where norm_2 is exactly 1, we want an exact value,
   // which math::Approx::rsqrt won't give
   
   float norm_sqr = _width * _width + _height * _height;
   
   if (norm_sqr == 1.f) // don't use math::is_equal
   {
      return 1.f;
   }
   else
   {
      return math::Approx::rsqrt (norm_sqr);
   }
}



/*
==============================================================================
Name : norm_inf
==============================================================================
*/

float Size::norm_inf () const
{
   return lang::max (lang::abs (_width), lang::abs (_height));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



#endif   // ohm_opa_Size_CODEHEADER_INCLUDED

#undef ohm_opa_Size_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
