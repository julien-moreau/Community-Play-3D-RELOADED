/*****************************************************************************

        FpsDisplay.cpp
        Copyright (c) 2009 Ohm Force

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
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Constants.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/AutoContextState.h"
#include "ohm/opal/private/Clock.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opak/FpsDisplay.h"
#include "ohm/opak/base/PrimitiveHelper.h"

#include <cassert>

// when defined, this might interfere with the
// real time to display a frame since this will
// potentially grow the invalidate box

// if not defined, the real value will only be
// displayed when the invalidation box encompass
// the box of the fps display

#define  OHM_OPAK_FPS_DISPLAY_ALWAYS_REFRESH



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

FpsDisplay::FpsDisplay ()
:  _color (*this, &FpsDisplay::color_changed)
,  _last_time (-1)
,  _hundred (0)
,  _ten (0)
,  _one (0)
{
   opal::Clock::use_instance ().add (*this, 10);
}



/*
==============================================================================
Name : init_display
==============================================================================
*/

void  FpsDisplay::init_display ()
{
   const opal::Font & font = opal::FontManager::use_instance ().get_font (
      opa::Constants::_default_font_0, 12.f
   );
   
   _num_width = 0.f;
   
   for (int i = 0 ; i < 10 ; ++i)
   {
      txt::unicode::Char c = '0' + i;
      
      archi::UInt32 gid = font.get_gid (c);
      float num_width = font.measure (gid);
      
      lang::max_in_place (_num_width, num_width);
   }
   
#if defined (OHM_OPAK_FPS_DISPLAY_ALWAYS_REFRESH)
   
   opa::Rect bbox (
      0,
      - font.get_ascent (),
      3.f * _num_width,
      font.get_ascent () + font.get_descent ()
   );
   
   set_bounding_box (bbox);
#endif
}



/*
==============================================================================
Name : restore_display
==============================================================================
*/

void  FpsDisplay::restore_display ()
{
   _image_arr.clear ();
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  FpsDisplay::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   
   if (_image_arr.size () == 0)
   {
      const std::string font_name = opa::Constants::_default_font_0;
      const float font_size = 12.f;
      
      const opal::Font & font
         = opal::FontManager::use_instance ().get_font (font_name, font_size);
      
      const opal::Font::Glyphs & glyphs = font.get_glyphs (context.get ());
      
      float num_height = 0.f;
      float min_offset = 0.f;
      
      for (int i = 0 ; i < 10 ; ++i)
      {
         txt::unicode::Char c = '0' + i;
         
         archi::UInt32 gid = font.get_gid (c);
         
         opa::Point offset;
         opal::Image image = glyphs.get_glyph_mask (offset, gid);
         
         lang::max_in_place (num_height, float (image._height));
         lang::min_in_place (min_offset, float (offset._y));
         
         _image_arr.push_back (image);
         _offset_arr.push_back (offset);
      }
   }

   opa::Point offset = opa::Point::_zero;
   
   opal::Color color = _color.get_cur_value ();
   
   {
      bool scale_image_flag = false;
      bool hinted_flag = true;
      
      const opal::Image & image = _image_arr [_hundred];
      opa::Point pt = offset + _offset_arr [_hundred];
      PrimitiveHelper::display (
         context, image, color, pt, scale_image_flag, hinted_flag
      );
      offset._x += _num_width;
   }

   {
      bool scale_image_flag = false;
      bool hinted_flag = true;
      
      const opal::Image & image = _image_arr [_ten];
      opa::Point pt = offset + _offset_arr [_ten];
      PrimitiveHelper::display (
         context, image, color, pt, scale_image_flag, hinted_flag
      );
      offset._x += _num_width;
   }

   {
      bool scale_image_flag = false;
      bool hinted_flag = true;
      
      const opal::Image & image = _image_arr [_one];
      opa::Point pt = offset + _offset_arr [_one];
      PrimitiveHelper::display (
         context, image, color, pt, scale_image_flag, hinted_flag
      );
      offset._x += _num_width;
   }
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  FpsDisplay::init (double timestamp)
{
   _last_time = timestamp;
}



/*
==============================================================================
Name : tick
==============================================================================
*/

void  FpsDisplay::tick (double timestamp)
{
   if (_last_time >= 0)
   {
      double diff_time = timestamp - _last_time;
      
      if (diff_time > 0)
      {
         const double   fps = 1.0 / diff_time;
         int            fps_d = lang::min (math::round_int (fps), 999);

         assert (fps_d >= 0);
         
         _one = fps_d % 10;
         fps_d /= 10;
         
         _ten = fps_d % 10;
         fps_d /= 10;

         _hundred = fps_d % 10;
      }
   }
   
   _last_time = timestamp;
   
   invalidate_all ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

FpsDisplay::~FpsDisplay ()
{
   assert (_image_arr.empty ());
   
   opal::Clock::use_instance ().remove (*this);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : color_changed
==============================================================================
*/

void  FpsDisplay::color_changed ()
{
   // nothing
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
