/*****************************************************************************

        AspectWidgetProgressBar.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61111

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

#include "ohm/opa/private/AspectWidgetProgressBar.h"
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
Name: AspectWidgetProgressBar
==============================================================================
*/

AspectWidgetProgressBar::AspectWidgetProgressBar ()
{
}



/*
==============================================================================
Name: ~AspectWidgetProgressBar
==============================================================================
*/

AspectWidgetProgressBar::~AspectWidgetProgressBar ()
{
}



/*
==============================================================================
Name: make
==============================================================================
*/

void AspectWidgetProgressBar::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      {
         opal::Bitmap bitmap = _be._back;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
         
         _back = opal::SprungImage (context.get (), bitmap, Rect (3, 0, 1, 14));
      }

      {
         opal::Bitmap bitmap = _be._front;
         
         aspect_widget.process_premul (bitmap);
         
         _front = opal::Image (context.get (), bitmap);
      }

      {
         opal::Bitmap bitmap = _be._front_end;
         
         aspect_widget.process_premul (bitmap);
         
         _front_end = opal::SprungImage (context.get (), bitmap, Rect (0, 0, 1, 11));
      }

      {
         opal::Bitmap bitmap = _be._indeterminate;
         
         aspect_widget.process_premul (bitmap);
         
         _indeterminate = opal::Image (context.get (), bitmap);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: do_init
==============================================================================
*/

void AspectWidgetProgressBar::do_init (opak::GraphicContext & context)
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

void AspectWidgetProgressBar::load_base_elements ()
{
   if (_be._back.size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
      
      bool ok_flag = true;
      
      ok_flag = il.load (_be._back, "ohm.widget.progress.back.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front, "ohm.widget.progress.front.png");
      assert (ok_flag);

      ok_flag = il.load (_be._front_end, "ohm.widget.progress.front.end.png");
      assert (ok_flag);

      ok_flag = il.load (_be._indeterminate, "ohm.widget.progress.indeterminate.png");
      assert (ok_flag);
   }
}



AspectWidgetProgressBar::BaseElements AspectWidgetProgressBar::_be;



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

