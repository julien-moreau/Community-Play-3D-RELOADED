/*****************************************************************************

        AspectWidgetMusicTime.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66675

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

#include "ohm/opa/private/AspectWidgetMusicTime.h"
#include "ohm/opa/private/AspectWidget.h"

#include "ohm/opal/BitmapContext.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/dsp/Cropper.h"
#include "ohm/opal/dsp/MarkFinder.h"
#include "ohm/txt/out/Print.h"

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

AspectWidgetMusicTime::AspectWidgetMusicTime ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidgetMusicTime::~AspectWidgetMusicTime ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidgetMusicTime::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      for (int i = 0 ; i < State_NBR_ELT ; ++i)
      {
         {
            opal::Bitmap bitmap = _be._button_left_arr [i];
            
            aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
            
            _button_left_arr [i] = opal::SprungImage (
               context.get (), bitmap, opa::Rect (6, 0, 1, 23)
            );
         }

         {
            opal::Bitmap bitmap = _be._button_middle_arr [i];
            
            aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
            
            _button_middle_arr [i] = opal::SprungImage (
               context.get (), bitmap, opa::Rect (0, 0, 1, 23)
            );
         }

         {
            opal::Bitmap bitmap = _be._button_separator_arr [i];
            
            aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
            
            _button_separator_arr [i] = opal::Image (context.get (), bitmap);
         }

         {
            opal::Bitmap bitmap = _be._button_right_arr [i];
            
            aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
            
            _button_right_arr [i] = opal::SprungImage (
               context.get (), bitmap, opa::Rect (0, 0, 1, 23)
            );
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

void  AspectWidgetMusicTime::do_init (opak::GraphicContext & context)
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

void  AspectWidgetMusicTime::load_base_elements ()
{
   if (_be._button_left_arr [State_OFF].size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      for (int i = 0 ; i < State_NBR_ELT ; ++i)
      {
         txt::out::Print printer;
         
         const char * state_0 = 0;
         switch (i)
         {
         case State_OFF:
            state_0 = "off";
            break;

         case State_ON:
            state_0 = "on";
            break;

         case State_OVER:
            state_0 = "over";
            break;
         }
         
         printer.print ("ohm.widget.music.time.%1%.left.png").arg (state_0);
         ok_flag = il.load (_be._button_left_arr [i], printer);
         assert (ok_flag);

         printer.print ("ohm.widget.music.time.%1%.middle.png").arg (state_0);
         ok_flag = il.load (_be._button_middle_arr [i], printer);
         assert (ok_flag);

         printer.print ("ohm.widget.music.time.%1%.separator.png").arg (state_0);
         ok_flag = il.load (_be._button_separator_arr [i], printer);
         assert (ok_flag);

         printer.print ("ohm.widget.music.time.%1%.right.png").arg (state_0);
         ok_flag = il.load (_be._button_right_arr [i], printer);
         assert (ok_flag);
      }
   }
}



AspectWidgetMusicTime::BaseElements AspectWidgetMusicTime::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
