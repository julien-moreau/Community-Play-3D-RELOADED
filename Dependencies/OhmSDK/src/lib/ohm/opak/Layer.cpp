/*****************************************************************************

        Layer.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 47587

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

#include "ohm/opak/Layer.h"

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

Layer::Layer ()
:  _fast_invalidate_flag (true)
{
   set_no_bounding_box ();
}



/*
==============================================================================
Name : enable_fast_invalidate
Description :
   Since the Layer has not a way to know quickly its bounding box,
   invalidating the whole layer normally is achieved by forwarding
   invalidation requests to the layer's children.
   
   If the client knows that invalidating the layer will almost invalidate
   everything (for example the layer has an infinte background, etc.),
   then it may use the fast invalidate feature which will invalidate
   an almost infinite rectangle.
==============================================================================
*/

void  Layer::enable_fast_invalidate (bool enable_flag)
{
   //#warning we could detect infinite primitives to automatically enable this property
   
   _fast_invalidate_flag = enable_flag;
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  Layer::display (GraphicContext & context)
{
   context.save ();
   
   add_mark (context);
   
   opa::Point origin = _origin.get_cur_value ();
   
   context.translate (origin._x, origin._y);
   
   display_children (context);
   
   context.restore ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Layer::~Layer ()
{
}



/*
==============================================================================
Name : invalidate_all
==============================================================================
*/

void  Layer::invalidate_all ()
{
   bool is_visible_flag = is_visible ();
   bool was_visible_flag = was_visible ();
   
   if (is_visible_flag || was_visible_flag)
   {
      if (_fast_invalidate_flag)
      {
         do_invalidate (opa::Rect::_infinity, opa::Margin::_zero);
      }
      else
      {
         invalidate_children ();
      }
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
