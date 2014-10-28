/*****************************************************************************

        PrimText.cpp
        Copyright (c) 2012 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
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

#include "ohm/opa/prim/PrimText.h"

#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opal/AreaString.h"
#include "ohm/opal/FontManager.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimText
==============================================================================
*/

PrimText::PrimText ()
:  _text (*this, &PrimText::something_changed)
{
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimText::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const opa::Size size = _size.get_cur_value ();
   const std::string text = _text.get_cur_value ();
   
   opa::Rect clip_rect = opa::Rect (opa::Point::_zero, size);
   context.apply (clip_rect);
   
   context.get ().save_graphic_state ();
   context.get ().clip (clip_rect);
   
   if (! text.empty ())
   {
      const AspectWidget & aspect = context.get <AspectWidget> ();
      const AspectWidgetLabel & aspect_label = aspect._label;

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

      const float str_x_margin = 6.f;

      const opal::Font & font
         = opal::FontManager::use_instance ().get_font (opa::Constants::_default_font_0, 12.f);
      
      opal::Scripture scripture;

      scripture.push_state_font (font);
      scripture.set_state_color (color);
      
      scripture.begin_edit ();
      scripture.insert (0, text);
      scripture.end_edit ();

      opal::AreaString area_str (scripture);
      
      area_str.set_bounds (opa::Size (
         size._width - 2.f * str_x_margin, 
         0.f
      ));
      area_str.enable_auto_height ();
      
      opak::PrimitiveHelper::display (
         context, area_str, opa::Point (str_x_margin, 0)
      );
   }

   context.get ().restore_graphic_state ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimText
==============================================================================
*/

PrimText::~PrimText ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: something_changed
==============================================================================
*/

void PrimText::something_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

