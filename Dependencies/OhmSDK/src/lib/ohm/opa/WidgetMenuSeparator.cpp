/*****************************************************************************

        WidgetMenuSeparator.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53326

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

#include "ohm/opa/WidgetMenuSeparator.h"
#include "ohm/opa/frame/FrameMenuItem.h"
#include "ohm/opa/prim/PrimMenuItem.h"
#include "ohm/opal/FontManager.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetMenuSeparator::~WidgetMenuSeparator ()
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMenuSeparator::WidgetMenuSeparator ()
{
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetMenuSeparator::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = Rect (0, 0, 23.f + 25.f, 8.f);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetMenuSeparator::do_get_margin (Margin & margin)
{
   margin = Margin::_zero;
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetMenuSeparator::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   // nothing
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetMenuSeparator::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   // nothing
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetMenuSeparator::do_enable (bool enabled_flag)
{
   // nothing
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetMenuSeparator::do_show (bool show_flag)
{
   // nothing
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetMenuSeparator::do_activate (bool activate_flag)
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
