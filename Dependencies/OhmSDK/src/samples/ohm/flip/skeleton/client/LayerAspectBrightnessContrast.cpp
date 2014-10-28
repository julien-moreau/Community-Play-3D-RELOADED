/*****************************************************************************

        LayerAspectBrightnessContrast.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70462

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

#include "client/LayerAspectBrightnessContrast.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

LayerAspectBrightnessContrast::LayerAspectBrightnessContrast ()
:  _brightness (*this, &LayerAspectBrightnessContrast::aspect_changed)
,  _contrast (*this, &LayerAspectBrightnessContrast::aspect_changed)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LayerAspectBrightnessContrast::display (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   context.save ();
   
   opa::Point origin = _origin.get_cur_value ();
   
   context.translate (origin._x, origin._y);
   
   _aspect.set_brightness (_brightness.get_cur_value ());
   _aspect.set_contrast (_contrast.get_cur_value ());
   
   context.set (_aspect);
   
   {
      opak::AspectColorProcessorBase & aspect = _aspect;
      context.set (aspect);
   }
   
   display_children (context);
   
   context.restore ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LayerAspectBrightnessContrast::~LayerAspectBrightnessContrast ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : aspect_changed
==============================================================================
*/

void  LayerAspectBrightnessContrast::aspect_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
