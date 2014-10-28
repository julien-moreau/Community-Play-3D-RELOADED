/*****************************************************************************

        HelperRect.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68169

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

#include "ohm/opak/base/HelperRect.h"
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
Name : display
Note :
   for now it is automatically hinted
   but we would need to implement the antialiased version
==============================================================================
*/

void  HelperRect::display (opak::GraphicContext & context)
{
   if (_margined_flag)
   {
      const opa::Point inv_px = context.get_inv_px ();
      
      _rect._origin._x -= _margin._left * inv_px._x;
      _rect._origin._y -= _margin._top * inv_px._y;
      _rect._size._width += (_margin._left + _margin._right) * inv_px._x;
      _rect._size._height += (_margin._top + _margin._bottom) * inv_px._y;
   }
   
   opal::GraphicContextInterface & ctx = context.get ();
   
   ctx.save_graphic_state ();
   ctx.set_fill_color (_color);

   if (_hint_flag)
   {
      if (_hint_absolute_flag)
      {
         context.apply (_rect._origin);
         opa::Size sz_x (_rect._size._width, 0.f);
         opa::Size sz_y (0.f, _rect._size._height);
         
         context.apply (sz_x);
         context.apply (sz_y);
         
         opa::Point pt0 = _rect._origin;
         opa::Point pt1 = _rect._origin + sz_x;
         opa::Point pt2 = _rect._origin + sz_x + sz_y;
         opa::Point pt3 = _rect._origin + sz_y;
         
         pt0.nearest_integral ();
         pt1.nearest_integral ();
         pt2.nearest_integral ();
         pt3.nearest_integral ();
         
         opa::Quad quad (pt0, pt1, pt2, pt3);
         
         ctx.fill (quad);
      }
      else
      {
         context.apply (_rect._origin, true);
   
         opa::Size sz_x (_rect._size._width, 0.f);
         opa::Size sz_y (0.f, _rect._size._height);
         
         context.apply (sz_x, true);
         context.apply (sz_y, true);
         
         opa::Quad quad (
            _rect._origin, _rect._origin + sz_x, _rect._origin + sz_x + sz_y, _rect._origin + sz_y
         );
         
         ctx.fill (quad);
      }
   }
   else
   {
      context.apply (_rect._origin);
      opa::Size sz_x (_rect._size._width, 0.f);
      opa::Size sz_y (0.f, _rect._size._height);
      
      context.apply (sz_x);
      context.apply (sz_y);
      
      opa::Point pt0 = _rect._origin;
      opa::Point pt1 = _rect._origin + sz_x;
      opa::Point pt2 = _rect._origin + sz_x + sz_y;
      opa::Point pt3 = _rect._origin + sz_y;
      
      opa::Quad quad (pt0, pt1, pt2, pt3);
      
      ctx.fill (quad);
   }
   
   ctx.restore_graphic_state ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
