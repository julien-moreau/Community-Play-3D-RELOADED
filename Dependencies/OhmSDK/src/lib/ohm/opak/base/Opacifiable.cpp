/*****************************************************************************

        Opacifiable.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36576

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

#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opak/base/Opacifiable.h"
#include "ohm/lang/fnc.h"

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

Opacifiable::Opacifiable ()
:  _opacity (*this, &Opacifiable::opacity_changed)
{
   _opacity.init (1.f);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Opacifiable::~Opacifiable ()
{
}



/*
==============================================================================
Name : is_visible
==============================================================================
*/

bool  Opacifiable::is_visible ()
{
   return Primitive::is_visible () && (_opacity.get_cur_value () > _min_visibility_step);
}



/*
==============================================================================
Name : was_visible
==============================================================================
*/

bool  Opacifiable::was_visible ()
{
   return Primitive::was_visible () && (_opacity.get_prev_value () > _min_visibility_step);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : was_visible
==============================================================================
*/

void Opacifiable::do_opacity_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : Auto::ctor
==============================================================================
*/

Opacifiable::Auto::Auto (Opacifiable & primitive, GraphicContext & context)
:  _primitive (primitive)
,  _context (context)
{
   _primitive.Opacifiable::pre_display (_context);
}



/*
==============================================================================
Name : Auto::dtor
==============================================================================
*/

Opacifiable::Auto::~Auto ()
{
   _primitive.Opacifiable::post_display (_context);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const float Opacifiable::_min_visibility_step = 1.f / 255.f;

/*
==============================================================================
Name : pre_display
==============================================================================
*/

void  Opacifiable::pre_display (GraphicContext & context)
{
   float opacity = _opacity.get_cur_value ();
   
   context.get ().set_alpha (opacity);
}



/*
==============================================================================
Name : post_display
==============================================================================
*/

void  Opacifiable::post_display (GraphicContext & context)
{
   context.get ().set_alpha (1.f);
}



/*
==============================================================================
Name : opacity_changed
==============================================================================
*/

void  Opacifiable::opacity_changed ()
{
   do_opacity_changed ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
