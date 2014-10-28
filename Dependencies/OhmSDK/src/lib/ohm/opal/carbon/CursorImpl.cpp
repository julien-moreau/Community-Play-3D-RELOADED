/*****************************************************************************

        CursorImpl.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 43612

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

#include "ohm/opal/carbon/CursorImpl.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opa/Point.h"
#include "ohm/util/trace.h"
#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

CursorImpl::CursorImpl ()
:  _ns_cursor (0)
{
   static bool first_time_flag = true;
   
   if (first_time_flag)
   {
      // Must first call [[[NSWindow alloc] init] release] to get the NSWindow
      // machinery set up so that NSCursor can use a window to cache the cursor image
      [[[NSWindow alloc] init] release];
      
      first_time_flag = false;
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

CursorImpl::~CursorImpl ()
{
   try
   {
      release ();
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("Uncatched exception");
      assert (false);
   }
}



/*
==============================================================================
Name : create
==============================================================================
*/

void  CursorImpl::create (const Bitmap & bitmap, const opa::Point & hot_point)
{
   release ();
   
   CGImageRef image_ref = 0;
   
   {
      static CGColorSpaceRef color_space_ref = 0;
      
      if (color_space_ref == 0)
      {
         color_space_ref = CGColorSpaceCreateWithName (kCGColorSpaceSRGB);
      }
      
      CGContextRef context_ref = CGBitmapContextCreate (
         const_cast <PixArgb *> (bitmap.data ()),
         bitmap.get_width (),
         bitmap.get_height (),
         8,
         bitmap.get_width () * 4,
         color_space_ref,
         kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host
      );
      assert (context_ref != 0);
      
      image_ref = CGBitmapContextCreateImage (context_ref);
      
      CGContextRelease (context_ref);
      context_ref = 0;
   }
   
   assert (image_ref != 0);
   
   {
      NSRect image_rect = NSMakeRect (0, 0, 0, 0);
      image_rect.size.width = CGImageGetWidth (image_ref);
      image_rect.size.height = CGImageGetHeight (image_ref);
      
      NSImage * ns_image = [[NSImage alloc] initWithSize:image_rect.size];
      [ns_image lockFocus];
      
      CGContextRef context_ref = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
      CGContextDrawImage (context_ref, *(CGRect*)&image_rect, image_ref);
      
      [ns_image unlockFocus];
      
      NSPoint ns_pt = NSMakePoint (hot_point._x, hot_point._y);
      
      _ns_cursor = [[NSCursor alloc] initWithImage:ns_image hotSpot:ns_pt];
      
      [ns_image release];
   }
   
   CGImageRelease (image_ref);
   image_ref = 0;
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  CursorImpl::release ()
{
   if (_ns_cursor != 0)
   {
      [_ns_cursor release];
      _ns_cursor = 0;
   }
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  CursorImpl::set ()
{
   assert (_ns_cursor != 0);
   
   if (_ns_cursor != 0)
   {
      [_ns_cursor set];
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
