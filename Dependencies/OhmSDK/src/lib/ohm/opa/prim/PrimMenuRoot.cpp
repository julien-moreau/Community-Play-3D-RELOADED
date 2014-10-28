/*****************************************************************************

        PrimMenuRoot.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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

#include "ohm/opa/prim/PrimMenuRoot.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opal/FontManager.h"

#include <cassert>



namespace ohm
{
namespace opa
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimMenuRoot
==============================================================================
*/

PrimMenuRoot::PrimMenuRoot ()
:  _arrow_left_opacity (*this, &PrimMenuRoot::arrow_opacity_changed)
,  _arrow_right_opacity (*this, &PrimMenuRoot::arrow_opacity_changed)
{
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimMenuRoot::display (opak::GraphicContext & context)
{
   opak::Primitive::Auto auto_prim_widget (*this, context);
   opak::Opacifiable::Auto auto_opacifiable (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetMenu & aspect_menu = aspect._menu;
   
   opa::Size size = _size.get_cur_value ();
   
   opa::Rect rect;
   rect._size = size;
   context.apply (rect);

   rect._origin._x -= 15.f;
   rect._size._width += 15.f + 15.f;
   
   rect._origin._y -= 16.f;
   rect._size._height += 16.f + 21.f;

   context.get ().draw (aspect_menu._back, rect);
   
   float opacity = _opacity.get_cur_value ();
   
   {
      float arrow_opacity = _arrow_left_opacity.get_cur_value ();
      
      if (arrow_opacity > 0.f)
      {
         context.get ().set_alpha (opacity * arrow_opacity);
         
         opa::Point pt (7.f, floor ((size._height - 9.f) * .5f));
         
         context.apply (pt);
         
         context.get ().draw (aspect_menu._arrow_left, pt);
         
         context.get ().set_alpha (opacity);
      }
   }
   
   {
      float arrow_opacity = _arrow_right_opacity.get_cur_value ();
      
      if (arrow_opacity > 0.f)
      {
         context.get ().set_alpha (opacity * arrow_opacity);
         
         opa::Point pt (size._width - 13.f, floor ((size._height - 9.f) * .5f));
         
         context.apply (pt);
         
         context.get ().draw (aspect_menu._arrow_right, pt);
         
         context.get ().set_alpha (opacity);
      }
   }
   
   display_children (context);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimMenuRoot
==============================================================================
*/

PrimMenuRoot::~PrimMenuRoot ()
{
}



/*
==============================================================================
Name: do_enabled_flag_changed
==============================================================================
*/

void PrimMenuRoot::do_enabled_flag_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name: do_size_changed
==============================================================================
*/

void  PrimMenuRoot::do_size_changed ()
{
   opa::Rect rect;
   rect._size = _size.get_cur_value ();
   
   rect._origin._x -= 15.f;
   rect._size._width += 15.f + 15.f;
   
   rect._origin._y -= 16.f;
   rect._size._height += 16.f + 21.f;
   
   set_bounding_box (rect);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: arrow_opacity_changed
==============================================================================
*/

void  PrimMenuRoot::arrow_opacity_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

