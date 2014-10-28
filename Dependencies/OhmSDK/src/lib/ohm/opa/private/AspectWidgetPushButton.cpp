/*****************************************************************************

        AspectWidgetPushButton.cpp
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

#include "ohm/opa/private/AspectWidgetPushButton.h"
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

AspectWidgetPushButton::AspectWidgetPushButton ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidgetPushButton::~AspectWidgetPushButton ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidgetPushButton::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      for (int i = 0 ; i < State_NBR_ELT ; ++i)
      {
         opal::Bitmap bitmap = _be._button_arr [i];
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
         
         _button_arr [i] = opal::SprungImage (context.get (), bitmap, opa::Rect (17, 0, 1, 29));
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectWidgetPushButton::do_init (opak::GraphicContext & context)
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

void  AspectWidgetPushButton::load_base_elements ()
{
   if (_be._button_arr [State_OFF].size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      ok_flag = il.load (_be._button_arr [State_OFF], "ohm.widget.pushbutton.off.png");
      assert (ok_flag);
      
      ok_flag = il.load (_be._button_arr [State_ON], "ohm.widget.pushbutton.on.png");
      assert (ok_flag);

      ok_flag = il.load (_be._button_arr [State_OVER], "ohm.widget.pushbutton.over.png");
      assert (ok_flag);
   }
}



AspectWidgetPushButton::BaseElements   AspectWidgetPushButton::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
