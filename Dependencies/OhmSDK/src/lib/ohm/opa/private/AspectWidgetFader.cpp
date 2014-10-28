/*****************************************************************************

        AspectWidgetFader.cpp
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 62123

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

#include "ohm/opa/private/AspectWidgetFader.h"
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
Name: AspectWidgetFader
==============================================================================
*/

AspectWidgetFader::AspectWidgetFader ()
{
}



/*
==============================================================================
Name: ~AspectWidgetFader
==============================================================================
*/

AspectWidgetFader::~AspectWidgetFader ()
{
}



/*
==============================================================================
Name: make
==============================================================================
*/

void AspectWidgetFader::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      {
         opal::Bitmap bitmap = _be._rail_hori;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
         
         _rail_hori = opal::SprungImage (context.get (), bitmap, Rect (0, 0, 1, 2));
      }

      {
         opal::Bitmap bitmap = _be._rail_verti;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
         
         _rail_verti = opal::SprungImage (context.get (), bitmap, Rect (0, 0, 2, 1));
      }

      {
         opal::Bitmap bitmap = _be._handle;
         
         aspect_widget.process_premul (bitmap);
         
         _handle = opal::Image (context.get (), bitmap);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: do_init
==============================================================================
*/

void AspectWidgetFader::do_init (opak::GraphicContext & context)
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

void AspectWidgetFader::load_base_elements ()
{
   if (_be._rail_hori.size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
      
      bool ok_flag = true;
      
      ok_flag = il.load (_be._rail_hori, "ohm.widget.fader.rail.hori.png");
      assert (ok_flag);

      ok_flag = il.load (_be._rail_verti, "ohm.widget.fader.rail.verti.png");
      assert (ok_flag);

      ok_flag = il.load (_be._handle, "ohm.widget.fader.handle.png");
      assert (ok_flag);
   }
}



AspectWidgetFader::BaseElements AspectWidgetFader::_be;



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

