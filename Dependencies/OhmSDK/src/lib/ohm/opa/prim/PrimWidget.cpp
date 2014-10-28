/*****************************************************************************

        PrimWidget.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54636

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

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opa/private/AspectWidget.h"

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

PrimWidget::PrimWidget ()
:  _enabled_flag (*this, &PrimWidget::enabled_flag_changed)
{
   _enabled_flag.init (true);
}



/*
==============================================================================
Name : is_enabled
Note :
   Takes into account parent enable state.
==============================================================================
*/

bool  PrimWidget::is_enabled (opak::GraphicContext & context)
{
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   return _enabled_flag.get_cur_value () && aspect._cur_enabled_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Auto::ctor
==============================================================================
*/

PrimWidget::Auto::Auto (PrimWidget & primitive, opak::GraphicContext & context)
:  _primitive (primitive)
,  _context (context)
,  _auto (primitive, context)
{
   bool enabled_flag = _primitive.is_enabled (_context);
   
   if (!enabled_flag)
   {
      _context.get ().set_alpha (.30f);
   }
}



/*
==============================================================================
Name : Auto::dtor
==============================================================================
*/

PrimWidget::Auto::~Auto ()
{
   bool enabled_flag = _primitive.is_enabled (_context);
   
   if (!enabled_flag)
   {
      _context.get ().set_alpha (1.f);
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimWidget::~PrimWidget ()
{
}



/*
==============================================================================
Name: do_enabled_flag_changed
==============================================================================
*/

void PrimWidget::do_enabled_flag_changed ()
{
   invalidate_all ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : state_changed
==============================================================================
*/

void  PrimWidget::enabled_flag_changed ()
{
   do_enabled_flag_changed ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
