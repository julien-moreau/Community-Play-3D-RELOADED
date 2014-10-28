/*****************************************************************************

        PrimLabel.cpp
        Copyright (c) 2010 Ohm Force

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

#include "ohm/opa/prim/PrimLabel.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opal/FontManager.h"

#include <cassert>



namespace ohm
{
namespace opa
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimLabel
==============================================================================
*/

PrimLabel::PrimLabel ()
:  _label (*this, &PrimLabel::label_changed)
,  _width (*this, &PrimLabel::width_changed)
,  _flush_factor (*this, &PrimLabel::flush_factor_changed)
,  _underline_flag (*this, &PrimLabel::underline_flag_changed)
,  _font_ptr (0)
,  _lsh_aptr ()
{
}



/*
==============================================================================
Name: init_display
==============================================================================
*/

void PrimLabel::init_display ()
{
   _font_ptr = &opal::FontManager::use_instance ().get_font (
      Constants::_default_font_0, 12.f
   );
   
   _lsh_aptr = std::auto_ptr <opak::LabelSimpleHelper> (
      new opak::LabelSimpleHelper (*_font_ptr, true)
   );
}



/*
==============================================================================
Name: restore_display
==============================================================================
*/

void PrimLabel::restore_display ()
{
   _font_ptr = 0;
   _lsh_aptr.reset ();
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimLabel::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetLabel & aspect_label = aspect._label;
   
   assert (_lsh_aptr.get () != 0);
   
   AspectWidgetLabel::ColorType color_type
      = AspectWidgetLabel::ColorType_DEFAULT;
   
   switch (aspect_label._cur_style)
   {
   case AspectWidgetLabel::Style_DEFAULT:
      color_type = AspectWidgetLabel::ColorType_DEFAULT;
      break;

   case AspectWidgetLabel::Style_LIGHT:
      color_type = AspectWidgetLabel::ColorType_LIGHT;
      break;

   case AspectWidgetLabel::Style_SECTION:
      color_type = AspectWidgetLabel::ColorType_SECTION;
      break;

   case AspectWidgetLabel::Style_BUTTON:
      color_type = AspectWidgetLabel::ColorType_BUTTON;
      break;

   case AspectWidgetLabel::Style_BUTTON_ON:
      color_type = AspectWidgetLabel::ColorType_BUTTON_ON;
      break;
   
   default:
      break;
   }
   
   const opal::Color & color = aspect_label._color_arr [color_type];
   
   const float width = _width.get_cur_value ();
   const float flush_factor = _flush_factor.get_cur_value ();
   const float text_width = _lsh_aptr->get_cur_width ();
   const opa::Point inv_px = context.get_inv_px ();
   
   opa::Point pt;
   pt._x = flush_factor * (width - text_width) * inv_px._x;
   pt._y = 0.f;
   
   _lsh_aptr->display (context, pt, color);
   
   bool underline_flag = _underline_flag.get_cur_value ();
   
   if (underline_flag && (_font_ptr != 0))
   {
      float underline_position = _font_ptr->get_underline_position ();
      
      opa::Rect rect (
         pt._x, - underline_position * inv_px._y,
         text_width * inv_px._x, inv_px._y
      );
      
      opak::use (rect)
         .hint ()
         .color (color)
         .display (context);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimLabel
==============================================================================
*/

PrimLabel::~PrimLabel ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: label_changed
==============================================================================
*/

void PrimLabel::label_changed ()
{
   assert (_lsh_aptr.get () != 0);
   
   std::string label = _label.get_cur_value ();
   
   _lsh_aptr->set (label);
   
   invalidate_all ();
}



/*
==============================================================================
Name: width_changed
==============================================================================
*/

void PrimLabel::width_changed ()
{
   assert (_lsh_aptr.get () != 0);
   assert (_font_ptr != 0);
   
   float width = _width.get_cur_value ();
   
   _lsh_aptr->set_available_width (width);
   
   float ascent = _font_ptr->get_ascent ();
   float descent = _font_ptr->get_descent ();
   
   opa::Rect bbox (
      0, - ascent, width, ascent + descent
   );
   
   set_bounding_box (bbox);
}



/*
==============================================================================
Name: flush_factor_changed
==============================================================================
*/

void PrimLabel::flush_factor_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name: underline_flag_changed
==============================================================================
*/

void PrimLabel::underline_flag_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

