/*****************************************************************************

        PrimRoot.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70401

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

#include "PrimRoot.h"

#include "ohm/opal/ImageManager.h"

#include <cassert>



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Thread : Main
==============================================================================
*/

PrimRoot::PrimRoot ()
:  _color (*this, &PrimRoot::color_changed)
,  _alpha (*this, &PrimRoot::something_changed)
,  _point (*this, &PrimRoot::something_changed)
,  _sprung (*this, &PrimRoot::something_changed)
{
}



/*
==============================================================================
Name : display
Thread : Video
==============================================================================
*/

void  PrimRoot::display (ohm::opak::GraphicContext & context)
{
   // setup the context for the primitive

   Primitive::Auto auto_primitive (*this, context);
   
   // check if we need to allocate resources on the very first display
   
   if (_image.empty ())
   {
      prepare_display (context);
   }
   
   // display a rectangle and an image
   
   display_rect (context);
   display_image (context);
   
   // display our children if any
   
   display_children (context);
   
   // the following drawing code we put is going to paint over our children if any
   // display a sprung image
   
   // display_sprung_image (context);
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
Name : prepare_display
Thread : Video
Note :
   When the project gets bigger, it is better to use the Aspect paradigm.
==============================================================================
*/

void  PrimRoot::prepare_display (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   opal::ImageManager & im = opal::ImageManager::use_instance ();
   
   _image = im.get_image (context.get (), "ohm.widget.fader.handle.png");
   
   // Sprung images are images which can extend automatically. They
   // are 9-slices based images.
   // The rectangle defines the coordinates in image coords of the
   // center slice. The center slice may overlap with one or multiple
   // image edges
   
   _sprung_image = im.get_sprung_image (
      context.get (), "ohm.widget.tooltip.png", opa::Rect (30, 0, 1, 36)
   );
}



/*
==============================================================================
Name : display_rect
Thread : Video
Description :
   Display a rectangle painted with '_color' to with coords 0, 0, '_size'
==============================================================================
*/

void  PrimRoot::display_rect (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   opa::Size size = _size.get_cur_value ();
   opal::Color color = _color.get_cur_value ();
   
   opak::use (opa::Rect (opa::Point::_zero, size))
      .color (color)
      .display (context);
}



/*
==============================================================================
Name : display_image
Thread : Video
Description :
   Display an image painted at 'point' with 'alpha'
==============================================================================
*/

void  PrimRoot::display_image (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   opa::Point point = _point.get_cur_value ();
   float alpha = _alpha.get_cur_value ();
   
   // offset so that the center of the image is on 'point'
   opa::Point offset (_image._width * .5f, _image._height * .5f);
   
   context.get ().set_alpha (alpha);
   
   opak::use (_image)
      .point (point - offset)
      .hint () // hint image to neighbour pixels to keep it "sharp"
      .display (context);
   
   context.get ().set_alpha (1.f);
}



/*
==============================================================================
Name : display_sprung_image
Thread : Video
Description :
   Display a sprung image, centered stretched with 'sprung', half transparent
==============================================================================
*/

void  PrimRoot::display_sprung_image (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   opa::Size size = _size.get_cur_value ();
   float sprung = _sprung.get_cur_value ();
   
   context.get ().set_alpha (.5f);
   
   opa::Rect rect;
   rect._origin._x = (size._width - sprung - 61) * .5f;
   rect._origin._y = (size._height - 36) * .5f;
   rect._size._width = sprung + 61;
   rect._size._height = 36;
   
   opak::use (_sprung_image)
      .rect (rect)
      .display (context);
   
   context.get ().set_alpha (1.f);
}



/*
==============================================================================
Name : color_changed
Thread : Video
Description :
   Notify that the color just changed. '_color.get_cur_value ()' will return
   the up-to-date value of '_color'.
   The color itself might be a step change from the Main thread, or an
   interpolation of colors from the main thread.

   Here since the aspect of the primitive should change, we notify the
   system that it needs to be redrawn.
==============================================================================
*/

void  PrimRoot::color_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : something_changed
Thread : Video
Note :
   This function does exactly the same as 'color_changed'.
   We could have also bound '_color' to 'something_changed'
==============================================================================
*/

void  PrimRoot::something_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
