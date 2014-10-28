/*****************************************************************************

        AspectWidget.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68114

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

#include "ohm/opa/private/AspectWidget.h"

#include "ohm/opal/BitmapContext.h"
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

AspectWidget::AspectWidget ()
:  _cur_enabled_flag (true)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectWidget::~AspectWidget ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectWidget::make (opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   _aspect_proc_ptr
      = context.get_ptr <opak::AspectColorProcessorBase> ();
   
   if (_aspect_proc_ptr != 0)
   {
      update_flag |= _aspect_proc_ptr->update (_aspect_color_proc_key);
   }
   
   if (update_flag)
   {
      use_key () = _aspect_color_proc_key;
      
      make_color_schemes (_aspect_proc_ptr);
   }

   _dialog.make (context);
   _menu.make (context);
   _list.make (context);
   _check_box.make (context);
   _push_button.make (context);
   _push_button_small.make (context);
   _push_button_tiny.make (context);
   _combo_box.make (context);
   _matrix_bool.make (context);
   _music_time.make (context);
   _scrollbar.make (context);
   _section.make (context);
   _spin.make (context);
   _fader.make (context);
   _label.make (context);
   _progress_bar.make (context);
}



/*
==============================================================================
Name : get_color
==============================================================================
*/

const opal::Color &  AspectWidget::get_color (ColorScheme color_scheme, float norm) const
{
   assert (int (color_scheme) >= 0);
   assert (int (color_scheme) < ColorScheme_NBR_ELT);
   
   int norm_d = math::round_int (
      norm * float (opal::ColorEngine::NBR_GRADIENT_COLOR - 1.f)
   );
   
   lang::limit_in_place (norm_d, 0, opal::ColorEngine::NBR_GRADIENT_COLOR - 1);
   
   const opal::ColorEngine::ColorGradient & gradient
      = _color_gradient_arr [color_scheme];
   
   return gradient._color_arr [norm_d];
}



/*
==============================================================================
Name : get_color_gray
==============================================================================
*/

const opal::Color &  AspectWidget::get_color_gray (float norm) const
{
   return get_color (ColorScheme_GRAY, norm);
}



/*
==============================================================================
Name : get_color_strong
==============================================================================
*/

const opal::Color &  AspectWidget::get_color_strong (float norm) const
{
   return get_color (ColorScheme_STRONG, norm);
}



/*
==============================================================================
Name : colorize_premul
==============================================================================
*/

void  AspectWidget::colorize_premul (opal::Bitmap & bitmap, ColorScheme color_scheme) const
{
   colorize_premul (bitmap, color_scheme, 0.f, 1.f);
}



/*
==============================================================================
Name : colorize_premul
==============================================================================
*/

void  AspectWidget::colorize_premul (opal::Bitmap & bitmap, ColorScheme color_scheme, float norm) const
{
   assert (int (color_scheme) >= 0);
   assert (int (color_scheme) < ColorScheme_NBR_ELT);
   
   int norm_d = math::round_int (
      norm * float (opal::ColorEngine::NBR_GRADIENT_COLOR - 1.f)
   );
   
   lang::limit_in_place (norm_d, 0, opal::ColorEngine::NBR_GRADIENT_COLOR - 1);
   
   const opal::Color & color
      = _color_gradient_arr [color_scheme]._color_arr [norm_d];
   
   opal::ColorEngine::use_instance ().colorize_premultiply_in_place (
      bitmap, color
   );
}



/*
==============================================================================
Name : colorize_premul
==============================================================================
*/

void  AspectWidget::colorize_premul (opal::Bitmap & bitmap, ColorScheme color_scheme, float norm_min, float norm_max) const
{
   assert (int (color_scheme) >= 0);
   assert (int (color_scheme) < ColorScheme_NBR_ELT);
   
   int norm_min_d = math::round_int (
      norm_min * float (opal::ColorEngine::NBR_GRADIENT_COLOR - 1.f)
   );
   
   lang::limit_in_place (norm_min_d, 0, opal::ColorEngine::NBR_GRADIENT_COLOR - 1);

   int norm_max_d = math::round_int (
      norm_max * float (opal::ColorEngine::NBR_GRADIENT_COLOR - 1.f)
   );

   lang::limit_in_place (norm_max_d, 0, opal::ColorEngine::NBR_GRADIENT_COLOR - 1);
   
   const opal::ColorEngine::PixArgbGradient & gradient
      = _pix_argb_gradient_arr [color_scheme];
   
   opal::ColorEngine::use_instance ().colorize_premultiply_in_place (
      bitmap, gradient, norm_min_d, norm_max_d
   );
}



/*
==============================================================================
Name : process_premul
==============================================================================
*/

void  AspectWidget::process_premul (opal::Bitmap & bitmap) const
{
   if (_aspect_proc_ptr != 0)
   {
      _aspect_proc_ptr->process_color (bitmap.data (), bitmap.size ());
   }
   
   bitmap.premultiply ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  AspectWidget::do_init (opak::GraphicContext & context)
{
   opal::ImageManager & im = opal::ImageManager::use_instance ();

   // non colorizes ressources

   _tool_tip_back = im.get_sprung_image (
      context.get (), "ohm.widget.tooltip.png",
      opa::Rect (30, 0, 1, 36)
   );
   
   _tool_tip_back2 = im.get_image (context.get (), "ohm.widget.tooltip.png");
   
   _tip_left = im.get_sprung_image (
      context.get (), "ohm.widget.tip.left.png", opa::Rect (6, 0, 1, 21)
   );

   _tip_up_left = im.get_sprung_image (
      context.get (), "ohm.widget.tip.up.png", opa::Rect (21, 6, 1, 1)
   );

   _tip_up_right = im.get_sprung_image (
      context.get (), "ohm.widget.tip.up.png", opa::Rect (1, 6, 1, 1)
   );

   _tip_right = im.get_sprung_image (
      context.get (), "ohm.widget.tip.right.png", opa::Rect (1, 0, 1, 21)
   );

   _tip_down_left = im.get_sprung_image (
      context.get (), "ohm.widget.tip.down.png", opa::Rect (21, 1, 1, 1)
   );

   _tip_down_right = im.get_sprung_image (
      context.get (), "ohm.widget.tip.down.png", opa::Rect (1, 1, 1, 1)
   );
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : make_color_schemes
==============================================================================
*/

void  AspectWidget::make_color_schemes (const opak::AspectColorProcessorBase * aspect_ptr)
{
   // ColorScheme_GRAY
   {
      opal::Gradient <opal::ColorLab> gradient;
      
      gradient.add (0.f, opal::ColorLab (0));
      gradient.add (1.f, opal::ColorLab (100));
      
      make_color_scheme (ColorScheme_GRAY, gradient, aspect_ptr);
   }
   
   // ColorScheme_STRONG
   {
      opal::Gradient <opal::ColorLab> gradient;
      
      /*gradient.add (0.f, opal::ColorLab (0));
      gradient.add (.86f, opal::ColorLab (86, 0, 71));
      gradient.add (.96f, opal::ColorLab (96, -10, 53));
      gradient.add (1.f, opal::ColorLab (98, -16, 93));*/
      
      gradient.add (0.f, opal::ColorLab (0));
      gradient.add (.80f, opal::ColorLab (80, 8, 81));
      gradient.add (.85f, opal::ColorLab (85, 4, 80));
      gradient.add (.95f, opal::ColorLab (95, 4, 80));
      gradient.add (1.f, opal::ColorLab (100, 4, 80));
      
      make_color_scheme (ColorScheme_STRONG, gradient, aspect_ptr);
   }
}



/*
==============================================================================
Name : make_color_schemes
==============================================================================
*/

void  AspectWidget::make_color_scheme (ColorScheme color_scheme, const opal::Gradient <opal::ColorLab> & gradient, const opak::AspectColorProcessorBase * aspect_ptr)
{
   for (int i = 0 ; i < opal::ColorEngine::NBR_GRADIENT_COLOR ; ++i)
   {
      float norm = float (i) / float (opal::ColorEngine::NBR_GRADIENT_COLOR - 1.f);
      
      opal::ColorLab color_lab = gradient.get (norm);
      
      opal::Color color = color_lab;
      
      if (aspect_ptr != 0)
      {
         aspect_ptr->process_color (color);
      }
      
      _color_gradient_arr [color_scheme]._color_arr [i] = color;
      _pix_argb_gradient_arr [color_scheme]._pix_argb_arr [i] = color.to_PixArgb ();
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
