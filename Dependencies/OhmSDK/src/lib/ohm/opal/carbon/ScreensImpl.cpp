/*****************************************************************************

        ScreensImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57623

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

#include "ohm/opal/carbon/ScreensImpl.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

ScreensImpl::~ScreensImpl ()
{
}



/*
==============================================================================
Name : get_layout
==============================================================================
*/

std::vector <opa::Rect> ScreensImpl::get_layout ()
{
   CGDisplayCount count = 0;
   CGGetActiveDisplayList (0, 0, &count);
   
   assert (count > 0);
   
   std::vector <CGDirectDisplayID> display_id_arr (count);
   
   // first index in array is guaranteed to be the main display
   CGGetActiveDisplayList (
      display_id_arr.size (),
      &display_id_arr [0],
      &count
   );
   
   assert (count == display_id_arr.size ());
   
   std::vector <opa::Rect> ret_val;
   
   for (int i = 0 ; i < display_id_arr.size () ; ++i)
   {
      CGDirectDisplayID display_id = display_id_arr [i];
      
      CGRect cg_rect = CGDisplayBounds (display_id);
      
      opa::Rect rect;
      rect.from_CGRect (cg_rect);
      
      ret_val.push_back (rect);
   }
   
   return ret_val;
}



/*
==============================================================================
Name : get_main_screen_without_decorations
==============================================================================
*/

opa::Rect   ScreensImpl::get_main_screen_without_decorations ()
{
   CGDirectDisplayID main_id = CGMainDisplayID ();
   
   CGRect cg_rect;
   
   OSStatus err = HIWindowGetAvailablePositioningBounds (
      main_id,
      kHICoordSpaceScreenPixel,
      &cg_rect
   );
   
   if (err != 0)
   {
      cg_rect = CGDisplayBounds (main_id);
      
      cg_rect.origin.y = GetMBarHeight ();
      cg_rect.size.height -= cg_rect.origin.y;
   }
   
   opa::Rect ret_val;
   ret_val.from_CGRect (cg_rect);
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
