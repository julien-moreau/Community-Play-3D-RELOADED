/*****************************************************************************

        HelperSurface.cpp
        Copyright (c) 2012 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 66876

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/HelperSurface.h"
#include "ohm/opak/base/GraphicContext.h"
#include "ohm/opal/GraphicContextInterface.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: HelperSurface
==============================================================================
*/

HelperSurface::HelperSurface ()
:  _context_ptr (0)
{
}



/*
==============================================================================
Name: display_begin
==============================================================================
*/

HelperSurface & HelperSurface::display_begin (opak::GraphicContext & context)
{
   assert (_context_ptr == 0);
   
   _context_ptr = & context;
   _counter = 0;
   _ring_pos = 0;
   
   _context_ptr->get ().save_graphic_state ();

   return *this;
}



/*
==============================================================================
Name: color
==============================================================================
*/

HelperSurface & HelperSurface::color (const opal::Color & color)
{
   assert (_context_ptr != 0);
   
   opal::GraphicContextInterface & ctx = _context_ptr->get ();

   ctx.set_fill_color (color);
   
   return *this;
}



/*
==============================================================================
Name: display
==============================================================================
*/

HelperSurface & HelperSurface::display (const opa::Point & pt_1, const opa::Point & pt_2)
{
   assert (_context_ptr != 0);

   ++_counter;
   
   PointPair & p1 = _ring [_ring_pos];
   
   _ring_pos = (_ring_pos + 1) % RING_LEN;
   
   PointPair & p2 = _ring [_ring_pos];
   
   p2.first = pt_1;
   p2.second = pt_2;
   
   _context_ptr->apply (p2.first);
   _context_ptr->apply (p2.second);
   
   if (_counter > 1)
   {
      display (*_context_ptr, p1, p2);
   }
   
   return *this;
}



/*
==============================================================================
Name: display_end
==============================================================================
*/

void HelperSurface::display_end ()
{
   assert (_context_ptr != 0);
   
   _context_ptr->get ().restore_graphic_state ();
   
   _context_ptr = 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: display
==============================================================================
*/

void HelperSurface::display (opak::GraphicContext & context, const PointPair & p1, const PointPair & p2)
{
   opa::Quad quad (p1.first, p1.second, p2.second, p2.first);

   opal::GraphicContextInterface & ctx = context.get ();

   ctx.fill (quad);
}



}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

