/*****************************************************************************

        AspectWidgetCheckBox.cpp
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

#include "ohm/opa/private/AspectWidgetCheckBox.h"
#include "ohm/opa/private/AspectWidget.h"

#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/BitmapContext.h"
#include "ohm/opak/base/GraphicContext.h"

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

AspectWidgetCheckBox::AspectWidgetCheckBox ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidgetCheckBox::~AspectWidgetCheckBox ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidgetCheckBox::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      for (int i = 0 ; i < State_NBR_ELT ; ++i)
      {
         for (int j = 0 ; j < SupState_NBR_ELT ; ++j)
         {
            opal::Bitmap back = _be._base;
            
            if (j == SupState_NORMAL)
            {
               aspect_widget.colorize_premul (back, AspectWidget::ColorScheme_GRAY, .45f);
            }
            else
            {
               aspect_widget.colorize_premul (back, AspectWidget::ColorScheme_GRAY, .55f);
            }
            
            opal::Bitmap front;

            switch (i)
            {
            case State_OFF:
               // nothing
               break;

            case State_ON:
               front = _be._on;
               break;

            case State_MIXED:
               front = _be._mixed;
               break;

            case State_DOT:
               front = _be._dot;
               break;
            }
            
            aspect_widget.colorize_premul (front, AspectWidget::ColorScheme_STRONG, .8f);
            
            size_t width = back.get_width ();
            size_t height = back.get_height ();
            
            opal::Bitmap bitmap;
         
            bitmap.resize (width, height);
            bitmap.clear_rect (0, 0, width, height);
         
            opal::BitmapContext bitmap_context (bitmap);

            bitmap_context.draw (back);
            bitmap_context.draw (front);
            
            _button_arr [i][j] = opal::Image (context.get (), bitmap);
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectWidgetCheckBox::do_init (opak::GraphicContext & context)
{
   load_base_elements ();
   
   //opal::ImageManager & im = opal::ImageManager::use_instance ();
   
   // non colorized ressources
   
   /*_over = im.get_sprung_image (
      "ohm.widget.checkbox.over.png", opa::Rect (0, 0, 1, 9)
   );*/
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_base_elements
==============================================================================
*/

void  AspectWidgetCheckBox::load_base_elements ()
{
   if (_be._base.size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      ok_flag = il.load (_be._base, "ohm.widget.checkbox.base.png");
      assert (ok_flag);

      ok_flag = il.load (_be._on, "ohm.widget.checkbox.on.png");
      assert (ok_flag);

      ok_flag = il.load (_be._mixed, "ohm.widget.checkbox.mixed.png");
      assert (ok_flag);

      ok_flag = il.load (_be._dot, "ohm.widget.checkbox.dot.png");
      assert (ok_flag);
   }
}



AspectWidgetCheckBox::BaseElements  AspectWidgetCheckBox::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
