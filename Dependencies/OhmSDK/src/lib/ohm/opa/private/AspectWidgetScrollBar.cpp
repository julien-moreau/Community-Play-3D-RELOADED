/*****************************************************************************

        AspectWidgetScrollBar.cpp
        Copyright (c) 2011 Ohm Force

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/private/AspectWidgetScrollBar.h"
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
Name: AspectWidgetScrollBar
==============================================================================
*/

AspectWidgetScrollBar::AspectWidgetScrollBar ()
{
}



/*
==============================================================================
Name: ~AspectWidgetScrollBar
==============================================================================
*/

AspectWidgetScrollBar::~AspectWidgetScrollBar ()
{
}



/*
==============================================================================
Name: make
==============================================================================
*/

void AspectWidgetScrollBar::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      {
         opal::Bitmap bitmap = _be._back;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY, .15f);
         
         _back = opal::SprungImage (context.get (), bitmap, Rect (3, 3, 1, 1));
      }

      {
         opal::Bitmap bitmap = _be._front;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY, .30f);
         
         _front = opal::SprungImage (context.get (), bitmap, Rect (2, 2, 1, 1));
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: do_init
==============================================================================
*/

void AspectWidgetScrollBar::do_init (opak::GraphicContext & context)
{
   load_base_elements ();

   // opal::ImageManager & im = opal::ImageManager::use_instance ();
   
   // non colorized ressources
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: load_base_elements
==============================================================================
*/

void AspectWidgetScrollBar::load_base_elements ()
{
   if (_be._back.size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
      
      bool ok_flag = true;
      
      ok_flag = il.load (_be._back, "ohm.widget.scrollbar.back.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front, "ohm.widget.scrollbar.front.png");
      assert (ok_flag);
   }
}



AspectWidgetScrollBar::BaseElements AspectWidgetScrollBar::_be;



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

