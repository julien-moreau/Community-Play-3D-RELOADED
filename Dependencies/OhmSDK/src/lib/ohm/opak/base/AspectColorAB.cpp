/*****************************************************************************

        AspectColorAB.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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

#include "ohm/opak/base/AspectColorAB.h"
#include "ohm/lang/fnc.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

AspectColorAB::AspectColorAB ()
:  _a (0.f)
,  _b (0.f)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

AspectColorAB::AspectColorAB (const AspectColorAB &other)
:  _a (other._a)
,  _b (other._b)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectColorAB::~AspectColorAB ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

AspectColorAB &   AspectColorAB::operator = (const AspectColorAB &other)
{
   _a = other._a;
   _b = other._b;
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  AspectColorAB::operator == (const AspectColorAB &other) const
{
   return (_a == other._a) && (_b == other._b);
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  AspectColorAB::operator != (const AspectColorAB &other) const
{
   return (_a != other._a) || (_b != other._b);
}



/*
==============================================================================
Name : operator <
Note :
   Lexicographical order. just to use as a key of an associative container.
==============================================================================
*/

bool  AspectColorAB::operator < (const AspectColorAB &other) const
{
   int c = lang::cmp (_a, other._a);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_b, other._b);
   return c < 0;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  AspectColorAB::set (float a, float b)
{
   bool changed_flag = false;
   changed_flag |= _a != a;
   changed_flag |= _b != b;
   
   if (changed_flag)
   {
      util::BinaryStreamOutput bso (use_key ());
      
      bso.clear ();
      bso.write (_a);
      bso.write (_b);
   }
}



/*
==============================================================================
Name : get_a
==============================================================================
*/

float AspectColorAB::get_a () const
{
   return _a;
}



/*
==============================================================================
Name : get_b
==============================================================================
*/

float AspectColorAB::get_b () const
{
   return _b;
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectColorAB::make (GraphicContext & context)
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectColorAB::do_init (opak::GraphicContext & context)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
