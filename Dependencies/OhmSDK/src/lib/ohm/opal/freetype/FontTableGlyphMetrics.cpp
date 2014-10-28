/*****************************************************************************

        FontTableGlyphMetrics.cpp
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

#include "ohm/opal/freetype/FontTableGlyphMetrics.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/private/Typesetter.h"
#include "ohm/opal/private/TypescriptMeasure.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

FontTableGlyphMetrics::FontTableGlyphMetrics (FT_Face face)
:  _face (face)
{
   assert (face != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FontTableGlyphMetrics::~FontTableGlyphMetrics ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : measure
==============================================================================
*/

float FontTableGlyphMetrics::measure (archi::UInt32 gid)
{
   float ret_val = 0.f;
   
   GlyphMetricsMap::iterator it = _glyph_metrics_map.find (gid);
   
   if (it == _glyph_metrics_map.end ())
   {
      it = load (gid);
   }
   
   if (it != _glyph_metrics_map.end ())
   {
      ret_val = it->second._advance;
   }
   
   return ret_val;
}



/*
==============================================================================
Name : measure
==============================================================================
*/

void  FontTableGlyphMetrics::measure (TypescriptMeasure & measure, const std::vector <Glyph> & glyph_arr, size_t start, size_t end)
{
   assert (!glyph_arr.empty ());
   
   for (size_t i = start ; i < end ; ++i)
   {
      const Glyph & glyph = glyph_arr [i];
      
      archi::UInt32 gid = glyph._gid;
      bool white_flag = glyph._separator_categ_class_flag;
      
      GlyphMetricsMap::iterator it = _glyph_metrics_map.find (gid);
      
      if (it == _glyph_metrics_map.end ())
      {
         it = load (gid);
      }
      
      if (it != _glyph_metrics_map.end ())
      {
         float advance = it->second._advance;
         
         if (white_flag)
         {
            measure._padding += advance;
         }
         else
         {
            measure._width += advance;
         }
      }
   }
}



/*
==============================================================================
Name : process_position
==============================================================================
*/

void  FontTableGlyphMetrics::process_position (TypescriptMeasure & measure, std::vector <Glyph> & glyph_arr, size_t start, size_t end)
{
   assert (!glyph_arr.empty ());
   
   float pos = glyph_arr [0]._x; // cumulative
   
   for (size_t i = start ; i < end ; ++i)
   {
      Glyph & glyph = glyph_arr [i];
      
      archi::UInt32 gid = glyph._gid;
      bool white_flag = glyph._separator_categ_class_flag;
      
      GlyphMetricsMap::const_iterator it = _glyph_metrics_map.find (gid);
      
      if (it != _glyph_metrics_map.end ())
      {
         const GlyphMetrics & glyph_metrics = it->second;
         
         float advance = glyph_metrics._advance;
         
         glyph._x = pos;
         glyph._width = advance;
         
         // TODO: check this
         
         if (white_flag)
         {
            measure._padding += advance;
         }
         else
         {
            measure._width += advance;
         }
         
         float image_left = pos + glyph_metrics._image_bounds._origin._x;
         float image_right
            = pos + glyph_metrics._image_bounds._origin._x
            + glyph_metrics._image_bounds._size._width;

         float image_top = glyph_metrics._image_bounds._origin._y;
         float image_bottom
            = glyph_metrics._image_bounds._origin._y
            + glyph_metrics._image_bounds._size._height;
         
         lang::min_in_place (measure._image_left, image_left);
         lang::max_in_place (measure._image_right, image_right);
         lang::min_in_place (measure._image_top, image_top);
         lang::max_in_place (measure._image_bottom, image_bottom);

         pos += advance;
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : load_glyph
==============================================================================
*/

FontTableGlyphMetrics::GlyphMetricsMap::iterator   FontTableGlyphMetrics::load (archi::UInt32 gid)
{
#pragma message ("FontTableGlyphMetrics: check this")

   // http://freetype.org/freetype2/docs/reference/ft2-quick_advance.html

   // do not render

   FT_Int32 load_flags = 0;
   load_flags |= FT_LOAD_DEFAULT;
   load_flags |= FT_LOAD_TARGET_LIGHT;

   FT_Error err = FT_Load_Glyph (_face, gid, load_flags);
   assert (err == 0);
   
   GlyphMetrics glyph_metrics;
   
   if (err == 0)
   {
      FT_GlyphSlot slot = _face->glyph;
      
      // the glyph format is FT_GLYPH_FORMAT_OUTLINE
      // it looks like the glyph outline metrics
      // are already all multiples of 64,
      // and the final bitmap renderered when format
      // is FT_GLYPH_FORMAT_BITMAP have dimensions,
      // left bearing and top bearing exactly of
      // those glyph metrics divided by 64
      
      FT_Glyph_Metrics & metrics = slot->metrics;
      
      const float div64 = (1.f / 64.f);
      
      glyph_metrics._advance = float (metrics.horiAdvance) * div64;

      glyph_metrics._image_bounds._origin._x = float (metrics.horiBearingX) * div64;
      glyph_metrics._image_bounds._origin._y = - float (metrics.horiBearingY) * div64;
      glyph_metrics._image_bounds._size._width = float (metrics.width) * div64;
      glyph_metrics._image_bounds._size._height = float (metrics.height) * div64;
   }
   else
   {
      glyph_metrics._advance = 0.f;
      glyph_metrics._image_bounds = opa::Rect::_zero;
   }
   
   GlyphMetricsMap::iterator it
      = _glyph_metrics_map.insert (std::make_pair (gid, glyph_metrics)).first;
   
   return it;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
