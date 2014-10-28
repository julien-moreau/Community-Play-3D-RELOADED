/*****************************************************************************

        PrimTip.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70452

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

#include "ohm/opa/prim/PrimTip.h"
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
Name: PrimTip
==============================================================================
*/

PrimTip::PrimTip ()
:  _layout (*this, &PrimTip::bbox_changed)
,  _label (*this, &PrimTip::label_changed)
{
}



/*
==============================================================================
Name : init_display
==============================================================================
*/

void  PrimTip::init_display ()
{
   const opal::Font & font = opal::FontManager::use_instance ().get_font (
      opa::Constants::_default_font_0, 11.f
   );
   
   _label_aptr = std::auto_ptr <opak::LabelSimpleHelper> (
      new opak::LabelSimpleHelper (font, true)
   );

   _label_aptr->set_available_width (1000.f);
}



/*
==============================================================================
Name : restore_display
==============================================================================
*/

void  PrimTip::restore_display ()
{
   _label_aptr.reset ();
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimTip::display (opak::GraphicContext & context)
{
   opak::Primitive::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();

   opa::Margin margin;
   opa::Point label_pt;
   compute_metrics (margin, label_pt);
   
   context.get ().set_alpha (.7f);
   
   const Layout layout = _layout.get_cur_value ();
   
   if (layout == Layout_LEFT)
   {
      opak::use (aspect._tip_left)
         .rect (opa::Rect::_zero)
         .margin (margin)
         .hint ()
         .display (context);
   }
   else if (layout == Layout_RIGHT)
   {
      opak::use (aspect._tip_right)
         .rect (opa::Rect::_zero)
         .margin (margin)
         .hint ()
         .display (context);
   }
   else if (layout == Layout_DOWN_RIGHT)
   {
      opak::use (aspect._tip_down_right)
         .rect (opa::Rect::_zero)
         .margin (margin)
         .hint ()
         .display (context);
   }
   
   opa::Point inv_px = context.get_inv_px ();
   label_pt._x *= inv_px._x;
   label_pt._y *= inv_px._y;
   
   _label_aptr->display (
      context, label_pt, opal::Color::_white
   );
   
   context.get ().set_alpha (1.f);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimTip
==============================================================================
*/

PrimTip::~PrimTip ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: bbox_changed
==============================================================================
*/

void  PrimTip::bbox_changed ()
{
   opa::Margin margin;
   opa::Point label_pt;
   compute_metrics (margin, label_pt);
   set_bounding_box (opa::Rect::_zero, margin);
   
   invalidate_all ();
}



/*
==============================================================================
Name: label_changed
==============================================================================
*/

void PrimTip::label_changed ()
{
   std::string label = _label.get_cur_value ();
   
   _label_aptr->set (label);
   
   bbox_changed ();
}



/*
==============================================================================
Name: compute_metrics
==============================================================================
*/

void  PrimTip::compute_metrics (opa::Margin & margin, opa::Point & label_pt)
{
   margin = opa::Margin::_zero;
   
   const Layout layout = _layout.get_cur_value ();
   const float label_width = ceil (_label_aptr->get_cur_width ());
   const float width = 6 + label_width + 6;
   const float height = ((layout == Layout_LEFT) || (layout == Layout_RIGHT)) ? 21.f : 26.f;
   
   switch (layout)
   {
   case Layout_LEFT:
      margin = opa::Margin (0, 11, width + 5, 10);
      label_pt = opa::Point (5 + 6, 14);
      break;

   case Layout_TOP_LEFT:
      margin = opa::Margin (11, 0, width - 11, height);
      label_pt = opa::Point (6, 5 + 14);
      break;

   case Layout_RIGHT:
      margin = opa::Margin (width + 5, 11, 0, 10);
      label_pt = opa::Point (- width - 5 + 6, - 11 + 14);
      break;

   case Layout_DOWN_RIGHT:
      margin = opa::Margin (width - 11, height, 11, 0);
      label_pt = opa::Point (- (width - 11) + 6, - height + 14);
      break;
   
   default:
      break;
   }
}



/*
==============================================================================
Name: compute_bbox
==============================================================================
*/

/*void   PrimTip::compute_bbox (opa::Rect & bbox)
{
   const Layout layout = _layout.get_cur_value ();
   const float cur_width = ceil (_label_aptr->get_cur_width ());
   
   // raw bbox, not layouted, and without arrow
   
   bbox._origin._x = 0;
   bbox._origin._y = 0;
   bbox._size._width = 6 + cur_width + 6;
   bbox._size._height = 21;
   
   switch (layout)
   {
   case Layout_LEFT:
      bbox._size._width += 5;
      bbox._origin._y -= 8 + 3;
      break;

   case Layout_TOP_LEFT:
      bbox._size._height += 5;
      bbox._origin._x -= 11;
      break;

   case Layout_TOP_CENTER:
      bbox._size._height += 5;
      bbox._origin._x -= floor (bbox._size._width * .5f);
      break;

   case Layout_TOP_RIGHT:
      bbox._size._height += 5;
      bbox._origin._x -= bbox._size._width - 11;
      break;

   case Layout_RIGHT:
      bbox._size._width += 5;
      bbox._origin._x -= bbox._size._width;
      bbox._origin._y -= 8 + 3;
      break;

   case Layout_DOWN_RIGHT:
      bbox._size._height += 5;
      bbox._origin._y -= bbox._size._height;
      bbox._origin._x -= bbox._size._width - 11;
      break;

   case Layout_DOWN_CENTER:
      bbox._size._height += 5;
      bbox._origin._y -= bbox._size._height;
      bbox._origin._x -= floor (bbox._size._width * .5f);
      break;

   case Layout_DOWN_LEFT:
      bbox._size._height += 5;
      bbox._origin._y -= bbox._size._height;
      bbox._origin._x -= 11;
      break;
   }
}*/



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

