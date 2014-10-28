/*****************************************************************************

        DirectXContext.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59383

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

#include "ohm/opal/directx/DirectXContext.h"
#include "ohm/opal/directx/DirectXEngine.h"
#include "ohm/opal/def.h"

#include <cassert>



namespace ohm
{
namespace opal
{



#if (ohm_opal_API == ohm_opal_API_DIRECTX)

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

DirectXContext::DirectXContext (DirectXEngine & engine)
:  _engine (engine)
,  _context_size ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DirectXContext::~DirectXContext ()
{
}



/*
==============================================================================
Name : set_context_size
==============================================================================
*/

void  DirectXContext::set_context_size (const opa::Size & size)
{
   _context_size = size;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_use_engine
==============================================================================
*/

Engine & DirectXContext::do_use_engine ()
{
   return _engine;
}



/*
==============================================================================
Name : do_update_context
==============================================================================
*/

void  DirectXContext::do_update_context (const GraphicState & graphic_state)
{
   assert (!_context_size.is_empty ());
   
   _engine.update_context (0, _context_size, graphic_state);
}



/*
==============================================================================
Name : do_get_context_size
==============================================================================
*/

const opa::Size & DirectXContext::do_get_context_size ()
{
   return _context_size;
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  DirectXContext::do_fill (const opa::Path & path, const Color & color)
{
   _engine.fill (path, color);
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  DirectXContext::do_fill (const opa::Rect & rect, const Color & color)
{
   _engine.fill (rect, color);
}



/*
==============================================================================
Name : do_fill
==============================================================================
*/

void  DirectXContext::do_fill (const opa::Quad & vertices, const Color & color)
{
   _engine.fill (vertices, color);
}



/*
==============================================================================
Name : do_stroke
==============================================================================
*/

void  DirectXContext::do_stroke (const opa::Path & path, const Color & color)
{
   _engine.stroke (path, color);
}



/*
==============================================================================
Name : do_draw
==============================================================================
*/

void  DirectXContext::do_draw (const Image & image, const opa::Quad & vertices, const opa::Quad & tex_coords, float alpha)
{
   _engine.draw (image, vertices, tex_coords, alpha);
}



/*
==============================================================================
Name : do_draw
==============================================================================
*/

void  DirectXContext::do_draw (const Image & mask, const opa::Quad & vertices, const opa::Quad & tex_coords, const Color & color)
{
   _engine.draw (mask, vertices, tex_coords, color);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // (ohm_opal_API == ohm_opal_API_DIRECTX)


/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
