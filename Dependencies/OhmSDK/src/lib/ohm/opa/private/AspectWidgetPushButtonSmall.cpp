/*****************************************************************************

        AspectWidgetPushButtonSmall.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68186

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

#include "ohm/opa/private/AspectWidgetPushButtonSmall.h"
#include "ohm/opa/private/AspectWidget.h"

#include "ohm/opal/BitmapContext.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/ImageManager.h"
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

AspectWidgetPushButtonSmall::AspectWidgetPushButtonSmall ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidgetPushButtonSmall::~AspectWidgetPushButtonSmall ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidgetPushButtonSmall::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      for (int i = 0 ; i < Type_NBR_ELT ; ++i)
      {
         for (int j = 0 ; j < State_NBR_ELT ; ++j)
         {
            opal::Bitmap back = _be._back_arr [j];
            opal::Bitmap front = _be._front_arr [i];
            
            aspect_widget.colorize_premul (back, AspectWidget::ColorScheme_GRAY);
            aspect_widget.colorize_premul (front, AspectWidget::ColorScheme_GRAY, .75f);
            
            size_t width = back.get_width ();
            size_t height = back.get_height ();
            
            opal::Bitmap bitmap;
            bitmap.resize (width, height);
            bitmap.clear_rect (0, 0, width, height);
            
            opal::BitmapContext bitmap_context (bitmap);
            
            bitmap_context.draw (back);
            
            if (j == State_ON)
            {
               bitmap_context.draw (front, Point (0, 1));
            }
            else
            {
               bitmap_context.draw (front);
            }
            
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

void  AspectWidgetPushButtonSmall::do_init (opak::GraphicContext & context)
{
   load_base_elements ();
   
   //opal::ImageManager & im = opal::ImageManager::use_instance ();
   
   // non colorized ressources
   
   /*_over = im.get_sprung_image (
      "ohm.widget.push_button.over.png", opa::Rect (0, 0, 1, 18)
   );*/
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_base_elements
==============================================================================
*/

void  AspectWidgetPushButtonSmall::load_base_elements ()
{
   if (_be._back_arr [State_OFF].size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      ok_flag = il.load (_be._back_arr [State_OFF], "ohm.widget.pushbutton.small.off.png");
      assert (ok_flag);
      
      ok_flag = il.load (_be._back_arr [State_ON], "ohm.widget.pushbutton.small.on.png");
      assert (ok_flag);

      ok_flag = il.load (_be._back_arr [State_OVER], "ohm.widget.pushbutton.small.over.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front_arr [Type_PLUS_SIGN], "ohm.widget.pushbutton.small.plus-sign.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front_arr [Type_MINUS_SIGN], "ohm.widget.pushbutton.small.minus-sign.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front_arr [Type_REFRESH_SIGN], "ohm.widget.pushbutton.small.refresh-sign.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front_arr [Type_ARROW_LEFT], "ohm.widget.pushbutton.small.left-sign.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front_arr [Type_ARROW_RIGHT], "ohm.widget.pushbutton.small.right-sign.png");
      assert (ok_flag);
   }
}



AspectWidgetPushButtonSmall::BaseElements AspectWidgetPushButtonSmall::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
