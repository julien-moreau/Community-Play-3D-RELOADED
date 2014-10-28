/*****************************************************************************

        AspectWidgetComboBox.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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
   #pragma warning (4 : 4355) // "'this' : used in seq member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/private/AspectWidgetComboBox.h"
#include "ohm/opa/private/AspectWidget.h"

#include "ohm/opal/BitmapContext.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/dsp/Cropper.h"
#include "ohm/opal/dsp/MarkFinder.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

AspectWidgetComboBox::AspectWidgetComboBox ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidgetComboBox::~AspectWidgetComboBox ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidgetComboBox::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      opal::Bitmap arrow = _be._arrow;
      
      aspect_widget.colorize_premul (arrow, AspectWidget::ColorScheme_STRONG, .85f);
      
      size_t width = arrow.get_width ();
      size_t height = arrow.get_height ();

      for (int i = 0 ; i < State_NBR_ELT ; ++i)
      {
         opal::Bitmap base = _be._base_arr [i];
         
         aspect_widget.colorize_premul (base, AspectWidget::ColorScheme_GRAY);
         
         opal::Bitmap bitmap;
         
         bitmap.resize (width, height);
         bitmap.clear_rect (0, 0, width, height);
      
         opal::BitmapContext bitmap_context (bitmap);
         
         bitmap_context.draw (base);
         bitmap_context.draw (arrow);
         
         _button_arr [i] = opal::SprungImage (context.get (), bitmap, opa::Rect (10, 0, 1, 23));
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectWidgetComboBox::do_init (opak::GraphicContext & context)
{
   load_base_elements ();
   
   // opal::ImageManager & im = opal::ImageManager::use_instance ();
   
   // non colorized ressources
   
   /*_over = im.get_sprung_image (
      "ohm.widget.combo_box.over.png", opa::Rect (0, 0, 1, 15)
   );*/
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_base_elements
==============================================================================
*/

void  AspectWidgetComboBox::load_base_elements ()
{
   if (_be._base_arr [State_OFF].size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      ok_flag = il.load (_be._base_arr [State_OFF], "ohm.widget.dropdown.off.png");
      assert (ok_flag);

      ok_flag = il.load (_be._base_arr [State_ON], "ohm.widget.dropdown.on.png");
      assert (ok_flag);

      ok_flag = il.load (_be._base_arr [State_OVER], "ohm.widget.dropdown.over.png");
      assert (ok_flag);

      ok_flag = il.load (_be._arrow, "ohm.widget.dropdown.arrow.png");
      assert (ok_flag);
   }
}



AspectWidgetComboBox::BaseElements  AspectWidgetComboBox::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
