/*****************************************************************************

        PrimRoot.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70466

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

#include "client/PrimRoot.h"

#include <cmath>

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PrimRoot::PrimRoot ()
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimRoot::display (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   context.save ();
   
   opa::Point origin = _origin.get_cur_value ();
   
   context.translate (origin._x, origin._y);

   _aspect_main.make (context);
   context.set (_aspect_main);

   _aspect.make (context);
   context.set (_aspect);
   
   display_ground (context);
   
   display_children (context);
   
   context.restore ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimRoot::~PrimRoot ()
{
}


   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : display_ground
==============================================================================
*/

void  PrimRoot::display_ground (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   using namespace std;
   
   static const float size = 256.f;
   
   Aspect & aspect = _aspect;
   
   opa::Rect clip_rect = context.get ().get_clip_bounding_box ();
   context.apply_inverse (clip_rect._origin);
   context.apply_inverse (clip_rect._size);
   
   float left = clip_rect.get_left ();
   float top = clip_rect.get_top ();
   float right = clip_rect.get_right ();
   float bottom = clip_rect.get_bottom ();
   
   left = floor (left / size) * size;
   top = floor (top / size) * size;
   right = ceil (right / size) * size;
   bottom = ceil (bottom / size) * size;
   
   for (float y = top ; y < bottom ; y += size)
   {
      for (float x = left ; x < right ; x += size)
      {
         opak::use (aspect._back)
            .point (x, y)
            .display (context);
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
