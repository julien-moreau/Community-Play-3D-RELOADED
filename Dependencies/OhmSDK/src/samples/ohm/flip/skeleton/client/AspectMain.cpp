/*****************************************************************************

        AspectMain.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70465

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
   #pragma warning (4 : 4355) // "'this' : used in layo member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/AspectMain.h"

#include "ohm/opal/ColorEngine.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/txt/out/Print.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

AspectMain::AspectMain ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AspectMain::~AspectMain ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  AspectMain::make (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
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
}



/*
==============================================================================
Name : get_selection
==============================================================================
*/

int   AspectMain::get_selection (bool selected_flag, bool ext_selected_flag, bool conflict_flag)
{
   if (conflict_flag)
   {
      return Selection_CONFLICT;
   }
   else
   {
      if (selected_flag)
      {
         return ext_selected_flag ? Selection_MIXED : Selection_ON;
      }
      else
      {
         return ext_selected_flag ? Selection_OTHER : Selection_OFF;
      }
   }
}



/*
==============================================================================
Name : get_color
==============================================================================
*/

const ohm::opal::Color &   AspectMain::get_color (ColorScheme color_scheme, float norm) const
{
   using namespace ohm;
   
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

const ohm::opal::Color &   AspectMain::get_color_gray (float norm) const
{
   return get_color (ColorScheme_GRAY, norm);
}



/*
==============================================================================
Name : colorize_premul
==============================================================================
*/

void  AspectMain::colorize_premul (ohm::opal::Bitmap & bitmap, ColorScheme color_scheme) const
{
   colorize_premul (bitmap, color_scheme, 0.f, 1.f);
}



/*
==============================================================================
Name : colorize_premul
==============================================================================
*/

void  AspectMain::colorize_premul (ohm::opal::Bitmap & bitmap, ColorScheme color_scheme, float norm) const
{
   using namespace ohm;
   
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

void  AspectMain::colorize_premul (ohm::opal::Bitmap & bitmap, ColorScheme color_scheme, float norm_min, float norm_max) const
{
   using namespace ohm;
   
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

void  AspectMain::process_premul (ohm::opal::Bitmap & bitmap) const
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

void  AspectMain::do_init (ohm::opak::GraphicContext & context)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : make_color_schemes
==============================================================================
*/

void  AspectMain::make_color_schemes (const ohm::opak::AspectColorProcessorBase * aspect_ptr)
{
   using namespace ohm;
   
   // ColorScheme_GRAY
   {
      opal::Gradient <opal::ColorLab> gradient;
      
      gradient.add (0.f, opal::ColorLab (0));
      gradient.add (1.f, opal::ColorLab (100));
      
      make_color_scheme (ColorScheme_GRAY, gradient, aspect_ptr);
   }
   
   // ColorScheme_LIGHT
   {
      opal::Gradient <opal::ColorLab> gradient;

      gradient.add (0.f, opal::ColorLab (0, 4, 4));
      gradient.add (.7f, opal::ColorLab (70, 4, 4));
      gradient.add (.8f, opal::ColorLab (80, 0, 6));
      gradient.add (1.f, opal::ColorLab (100, 0, 6));
      
      make_color_scheme (ColorScheme_LIGHT, gradient, aspect_ptr);
   }

   // ColorScheme_STRONG
   {
      opal::Gradient <opal::ColorLab> gradient;
      
      gradient.add (0.f, opal::ColorLab (0));
      gradient.add (.86f, opal::ColorLab (86, 0, 71));
      gradient.add (.96f, opal::ColorLab (96, -10, 53));
      gradient.add (1.f, opal::ColorLab (98, -16, 93));
      
      make_color_scheme (ColorScheme_STRONG, gradient, aspect_ptr);
   }

   // ColorScheme_OTHER
   {
      opal::Gradient <opal::ColorLab> gradient;

      float a = 51;
      float b = 105;
      
      gradient.add (0.f, opal::ColorLab (0));
      gradient.add (.76f, opal::ColorLab (79, a, b));
      gradient.add (1.f, opal::ColorLab (98, a, b));
      
      make_color_scheme (ColorScheme_OTHER, gradient, aspect_ptr);
   }

   // ColorScheme_CONFLICT
   {
      opal::Gradient <opal::ColorLab> gradient;
      
      gradient.add (0.f, opal::ColorLab (0));
      gradient.add (.83f, opal::ColorLab (83, 24, 10));
      gradient.add (1.f, opal::ColorLab (59, 70, 46));
      
      make_color_scheme (ColorScheme_CONFLICT, gradient, aspect_ptr);
   }

   // ColorScheme_RED
   {
      opal::Gradient <opal::ColorLab> gradient;
      
      gradient.add (0.f, opal::ColorLab (0, 70, 46));
      gradient.add (1.f, opal::ColorLab (59, 70, 46));
      
      make_color_scheme (ColorScheme_RED, gradient, aspect_ptr);
   }
}



/*
==============================================================================
Name : make_color_schemes
==============================================================================
*/

void  AspectMain::make_color_scheme (ColorScheme color_scheme, const ohm::opal::Gradient <ohm::opal::ColorLab> & gradient, const ohm::opak::AspectColorProcessorBase * aspect_ptr)
{
   using namespace ohm;
   
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

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
