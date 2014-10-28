/*****************************************************************************

        AspectWidgetSection.cpp
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

#include "ohm/opa/private/AspectWidgetSection.h"
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

AspectWidgetSection::AspectWidgetSection ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidgetSection::~AspectWidgetSection ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidgetSection::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectWidget & aspect_widget
      = context.get <AspectWidget> ();
   
   update_flag |= aspect_widget.update (_aspect_widget_key);
   
   if (update_flag)
   {
      {
         opal::Bitmap bitmap = _be._header;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
         
         _header = opal::SprungImage (context.get (), bitmap, opa::Rect (8, 12, 1, 1));
      }
      
      {
         opal::Bitmap bitmap = _be._content;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
         
         _content = opal::SprungImage (context.get (), bitmap, opa::Rect (8, 2, 1, 1));
      }
      
      {
         opal::Bitmap bitmap = _be._footer;
         
         aspect_widget.colorize_premul (bitmap, AspectWidget::ColorScheme_GRAY);
         
         _footer = opal::SprungImage (context.get (), bitmap, opa::Rect (8, 2, 1, 1));
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectWidgetSection::do_init (opak::GraphicContext & context)
{
   load_base_elements ();
   
   // opal::ImageManager & im = opal::ImageManager::use_instance ();
   
   // non colorized ressources
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_base_elements
==============================================================================
*/

void  AspectWidgetSection::load_base_elements ()
{
   if (_be._header.size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      ok_flag = il.load (_be._header, "ohm.widget.section.header.png");
      assert (ok_flag);
      
      ok_flag = il.load (_be._content, "ohm.widget.section.content.png");
      assert (ok_flag);
      
      ok_flag = il.load (_be._footer, "ohm.widget.section.footer.png");
      assert (ok_flag);
   }
}



AspectWidgetSection::BaseElements   AspectWidgetSection::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
