/*****************************************************************************

        Aspect.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70485

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
   #pragma warning (4 : 4355) // "'this' : used in tl member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/Aspect.h"

#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opal/FontManager.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Aspect::Aspect ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Aspect::~Aspect ()
{
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  Aspect::make (ohm::opak::GraphicContext & context)
{
   bool update_flag = need_make (context);
   
   const AspectMain & aspect = context.get <AspectMain> ();
   
   update_flag |= aspect.update (_aspect_key);
   
   if (update_flag)
   {
      make (context, aspect);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_init
==============================================================================
*/

void  Aspect::do_init (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   load_base_elements ();

   // non colorized ressources

   opal::ImageManager & im = opal::ImageManager::use_instance ();

   _taxi_shadow = im.get_image (context.get (), "skeleton.taxi.shadow.png");
   _taxi_reflect = im.get_image (context.get (), "skeleton.taxi.reflect.png");
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_base_elements
==============================================================================
*/

void  Aspect::load_base_elements ()
{
   using namespace ohm;
   
   if (_be._back.size () == 0)
   {
      opal::ImageLoader & il = opal::ImageLoader::use_instance ();
   
      bool ok_flag = true;
      
      ok_flag = il.load (_be._back, "skeleton.noise.png");
      assert (ok_flag);

      ok_flag = il.load (_be._taxi_selection, "skeleton.taxi.selection.png");
      assert (ok_flag);

      ok_flag = il.load (_be._taxi_back, "skeleton.taxi.back.png");
      assert (ok_flag);

      ok_flag = il.load (_be._taxi_main, "skeleton.taxi.color.png");
      assert (ok_flag);

      ok_flag = il.load (_be._taxi_front, "skeleton.taxi.front.png");
      assert (ok_flag);

      ok_flag = il.load (_be._pedestrian_selection, "skeleton.pedestrian.selection.png");
      assert (ok_flag);

      ok_flag = il.load (_be._pedestrian_main, "skeleton.pedestrian.png");
      assert (ok_flag);
   }
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  Aspect::make (ohm::opak::GraphicContext & context, const AspectMain & aspect)
{
   using namespace ohm;
   
   {
      opal::Bitmap bitmap = _be._back;
   
      aspect.process_premul (bitmap);
      
      _back = opal::Image (context.get (), bitmap);
   }
   
   // taxi
   
   for (int i = 0 ; i < AspectMain::ColorScheme_NBR_ELT ; ++i)
   {
      opal::Bitmap bitmap = _be._taxi_selection;
      
      aspect.colorize_premul (bitmap, AspectMain::ColorScheme (i), .9f);
      
      _taxi_selection_arr [i] = opal::Image (context.get (), bitmap);
   }

   {
      opal::Bitmap bitmap = _be._taxi_back;
   
      aspect.process_premul (bitmap);
      
      _taxi_back = opal::Image (context.get (), bitmap);
   }

   {
      opal::Bitmap bitmap = _be._taxi_front;
   
      aspect.process_premul (bitmap);
      
      _taxi_front = opal::Image (context.get (), bitmap);
   }
   
   _taxi_color_arr [model::Taxi::Color_WHITE] = opal::ColorLab (90);
   _taxi_color_arr [model::Taxi::Color_BLACK] = opal::ColorLab (10);
   _taxi_color_arr [model::Taxi::Color_YELLOW] = opal::ColorLab (90, -4, 89);
   _taxi_color_arr [model::Taxi::Color_BLUE] = opal::ColorLab (70, -9, -52);
   _taxi_color_arr [model::Taxi::Color_GREEN] = opal::ColorLab (90, -47, 83);

   _taxi_color_h_arr [model::Taxi::Color_WHITE] = opal::ColorLab (100);
   _taxi_color_h_arr [model::Taxi::Color_BLACK] = opal::ColorLab (20);
   _taxi_color_h_arr [model::Taxi::Color_YELLOW] = opal::ColorLab (100, -4, 89);
   _taxi_color_h_arr [model::Taxi::Color_BLUE] = opal::ColorLab (80, -9, -52);
   _taxi_color_h_arr [model::Taxi::Color_GREEN] = opal::ColorLab (100, -47, 83);
   
   {
      opal::Bitmap bitmap = _be._taxi_main;
      
      aspect.colorize_premul (bitmap, AspectMain::ColorScheme_GRAY, 1.f);
      
      _taxi_main = opal::Image (context.get (), bitmap);
   }
   
   // pedestrian
   
   for (int i = 0 ; i < AspectMain::ColorScheme_NBR_ELT ; ++i)
   {
      opal::Bitmap bitmap = _be._pedestrian_selection;
      
      aspect.colorize_premul (bitmap, AspectMain::ColorScheme (i), .9f);
      
      _pedestrian_selection_arr [i] = opal::Image (context.get (), bitmap);
   }

   {
      opal::Bitmap bitmap = _be._pedestrian_main;
      
      aspect.colorize_premul (bitmap, AspectMain::ColorScheme_GRAY, .1f);
      
      _pedestrian_main = opal::Image (context.get (), bitmap);
   }

   {
      opal::Bitmap bitmap = _be._pedestrian_main;
      
      aspect.colorize_premul (bitmap, AspectMain::ColorScheme_GRAY, .3f);
      
      _pedestrian_main_h = opal::Image (context.get (), bitmap);
   }
}



Aspect::BaseElements Aspect::_be;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
