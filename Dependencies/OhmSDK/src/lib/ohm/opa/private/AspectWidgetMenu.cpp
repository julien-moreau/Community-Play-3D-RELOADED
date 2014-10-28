/*****************************************************************************

        AspectWidgetMenu.cpp
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

#include "ohm/opa/private/AspectWidgetMenu.h"
#include "ohm/opa/private/AspectWidget.h"

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

AspectWidgetMenu::AspectWidgetMenu ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidgetMenu::~AspectWidgetMenu ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidgetMenu::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      {
         opal::Bitmap bitmap = _be._arrow_left;
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY, .25f);
         
         _arrow_left = opal::Image (context.get (), bitmap);
      }
      
      {
         opal::Bitmap bitmap = _be._arrow_right;
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY, .25f);
         
         _arrow_right = opal::Image (context.get (), bitmap);
      }
      
      {
         opal::Bitmap bitmap = _be._item_back;
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY, .55f, .7f);
         
         _item_back = opal::SprungImage (context.get (), bitmap, opa::Rect (0, 0, 1, 16));
      }
      
      {
         for (int i = 0 ; i < TOTAL_Selection_NBR ; ++i)
         {
            Selection selection = static_cast <Selection> (i);
            
            {
               opal::Bitmap bitmap = _be._item_mark_check;
               
               aspect_widget.colorize_premul (
                  bitmap, AspectWidget::ColorScheme_STRONG,
                  selection == Selection_OFF ? .25f : 1.f
               );
               
               _item_mark_check_arr [i] = opal::Image (context.get (), bitmap);
            }
            
            {
               opal::Bitmap bitmap = _be._item_mark_check_multi;

               aspect_widget.colorize_premul (
                  bitmap, AspectWidget::ColorScheme_STRONG,
                  selection == Selection_OFF ? .25f : 1.f
               );
               
               _item_mark_check_multi_arr [i] = opal::Image (context.get (), bitmap);
            }
            
            {
               opal::Bitmap bitmap = _be._item_sub_arrow;

               aspect_widget.colorize_premul (
                  bitmap, AspectWidget::ColorScheme_STRONG,
                  selection == Selection_OFF ? .25f : 1.f
               );
               
               _item_sub_arrow_arr [i] = opal::Image (context.get (), bitmap);
            }
         }
      }
      
      _item_text_arr [Selection_OFF][Enable_OFF]
         = aspect_widget.get_color_gray (.75f);

      _item_text_arr [Selection_ON][Enable_OFF]
         = aspect_widget.get_color_gray (.5f);

      _item_text_arr [Selection_OFF][Enable_ON]
         = aspect_widget.get_color_gray (.25f);

      _item_text_arr [Selection_ON][Enable_ON]
         = aspect_widget.get_color_gray (1.f);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectWidgetMenu::do_init (opak::GraphicContext & context)
{
   load_base_elements ();
   
   opal::ImageManager & im = opal::ImageManager::use_instance ();
   
   // non colorized ressources

   _back = im.get_sprung_image (
      context.get (), "ohm.widget.menu.back.png", opa::Rect (27, 25, 108, 95)
   );
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_base_elements
==============================================================================
*/

void  AspectWidgetMenu::load_base_elements ()
{
   if (_be._item_back.size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      ok_flag = il.load (_be._item_back, "ohm.widget.menu.item.back.png");
      assert (ok_flag);

      ok_flag = il.load (_be._item_mark_check, "ohm.widget.menu.item.mark.png");
      assert (ok_flag);

      ok_flag = il.load (_be._item_mark_check_multi, "ohm.widget.menu.item.mark.multi.png");
      assert (ok_flag);

      ok_flag = il.load (_be._item_sub_arrow, "ohm.widget.menu.item.sub_arrow.png");
      assert (ok_flag);

      ok_flag = il.load (_be._arrow_left, "ohm.widget.menu.arrow.left.png");
      assert (ok_flag);

      ok_flag = il.load (_be._arrow_right, "ohm.widget.menu.arrow.right.png");
      assert (ok_flag);
   }
}



AspectWidgetMenu::BaseElements   AspectWidgetMenu::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
