/*****************************************************************************

        Font.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59588

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

#include "ohm/opal/Font.h"
#include "ohm/opal/freetype/FTLibrary.h"
#include "ohm/opal/freetype/FontResources.h"
#include "ohm/archi/def.h"
#include "ohm/opal/def.h"

#include "ohm/opal/private/TypescriptMeasure.h"

#if (ohm_opal_API == ohm_opal_API_OPENGL)
   #include "ohm/opal/opengl/OpenGLEngine.h"

#elif (ohm_opal_API == ohm_opal_API_DIRECTX)
   #include "ohm/opal/directx/DirectXEngine.h"

#endif




#include <cassert>



namespace ohm
{
namespace opal
{



/*
==============================================================================
Threading :
   to prevent locking in the video thread, each used font has a copy in the
   user input thread as well as in the video thread. This is directly
   handled at the FontManager level.
==============================================================================
*/



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Font::Font (const std::string & name, float size)
:  _thread_id (sys::ThreadMgr::get_current_thread_id ())
,  _name (name)
,  _size (size)
,  _face (0)
,  _units_per_em (0)
,  _ascent (0.f)
,  _descent (0.f)
,  _leading (0.f)

,  _engine_glyphs_map ()
,  _font_table_glyph_metrics_aptr ()
,  _font_table_kern_aptr ()
{
   init ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Font::~Font ()
{
   try
   {
      if (_face != 0)
      {
         FT_Error err = FT_Done_Face (_face);
         assert (err == 0);
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : release
==============================================================================
*/

void  Font::release (Engine & engine)
{
   EngineGlyphsMap::iterator it = _engine_glyphs_map.find (&engine);
   
   if (it != _engine_glyphs_map.end ())
   {
      _engine_glyphs_map.erase (it);
   }
}



/*
==============================================================================
Name : get_thread_id
==============================================================================
*/

sys::ThreadMgr::IdType  Font::get_thread_id () const
{
   return _thread_id;
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

const std::string &  Font::get_name () const
{
   return _name;
}



/*
==============================================================================
Name : get_size
==============================================================================
*/

float Font::get_size () const
{
   return _size;
}



/*
==============================================================================
Name : get_ascent
==============================================================================
*/

float Font::get_ascent () const
{
   return _ascent;
}



/*
==============================================================================
Name : get_descent
==============================================================================
*/

float Font::get_descent () const
{
   return _descent;
}



/*
==============================================================================
Name : get_leading
==============================================================================
*/

float Font::get_leading () const
{
   return _leading;
}



/*
==============================================================================
Name : get_underline_position
==============================================================================
*/

float Font::get_underline_position () const
{
   return _underline_position;
}



/*
==============================================================================
Name : get_underline_thickness
==============================================================================
*/

float Font::get_underline_thickness () const
{
   return _underline_thickness;
}



/*
==============================================================================
Name : get_strikethrough_position
==============================================================================
*/

float Font::get_strikethrough_position () const
{
   return _strikethrough_position;
}



/*
==============================================================================
Name : get_strikethrough_thickness
==============================================================================
*/

float Font::get_strikethrough_thickness () const
{
   return _strikethrough_thickness;
}



/*
==============================================================================
Name : get_gid
==============================================================================
*/

archi::UInt32  Font::get_gid (txt::unicode::Char c) const
{
   assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   
   archi::UInt32 ret_val = FT_Get_Char_Index (_face, c);
   
   return ret_val;
}



/*
==============================================================================
Name : get_glyphs
==============================================================================
*/

const Font::Glyphs & Font::get_glyphs (GraphicContextInterface & gc) const
{
   assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   
   Engine & engine = gc.use_engine ();
   
   EngineGlyphsMap::iterator it = _engine_glyphs_map.find (&engine);
   
   if (it == _engine_glyphs_map.end ())
   {
      it = _engine_glyphs_map.insert (std::make_pair (&engine, Glyphs (engine, *this))).first;
   }
   
   assert (it != _engine_glyphs_map.end ());
   
   Glyphs & glyphs = it->second;
   
   return glyphs;
}



/*
==============================================================================
Name : process_substitution
==============================================================================
*/

void  Font::process_substitution (Typesetter & typesetter, GlyphRange & glyph_range) const
{
   assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   
   _font_table_gsub_aptr->process_substitution (typesetter, glyph_range);
}



/*
==============================================================================
Name : measure
==============================================================================
*/

float Font::measure (archi::UInt32 gid) const
{
   assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   
   return _font_table_glyph_metrics_aptr->measure (gid);
}



/*
==============================================================================
Name : measure
==============================================================================
*/

void  Font::measure (TypescriptMeasure & measure, const std::vector <Glyph> & glyph_arr, size_t start, size_t end) const
{
   assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   
   _font_table_glyph_metrics_aptr->measure (measure, glyph_arr, start, end);
   
   _font_table_kern_aptr->measure (measure, glyph_arr, start, end);
}



/*
==============================================================================
Name : process_position
==============================================================================
*/

void  Font::process_position (TypescriptMeasure & measure, std::vector <Glyph> & glyph_arr, size_t start, size_t end) const
{
   assert (_thread_id == sys::ThreadMgr::get_current_thread_id ());
   
   _font_table_glyph_metrics_aptr->process_position (measure, glyph_arr, start, end);
   
   _font_table_kern_aptr->process_position (measure, glyph_arr, start, end);
}



/*
==============================================================================
Name : Glyphs::ctor
==============================================================================
*/

Font::Glyphs::Glyphs (Engine & engine, const Font & parent)
:  _parent (parent)
,  _engine (engine)
,  _gid_glyph_info_map ()
{
}



/*
==============================================================================
Name : Glyphs::dtor
==============================================================================
*/

Font::Glyphs::~Glyphs ()
{
}



/*
==============================================================================
Name : Glyphs::get_glyph_mask
==============================================================================
*/

const Image &  Font::Glyphs::get_glyph_mask (opa::Point & offset, archi::UInt32 gid) const
{
   GidGlyphInfoMap::iterator it = _gid_glyph_info_map.find (gid);
   
   if (it == _gid_glyph_info_map.end ())
   {
      it = _parent.load_glyph (_gid_glyph_info_map, _engine, gid);
   }
   
   assert (it != _gid_glyph_info_map.end ());
   
   const GlyphInfo & glyph_info = it->second;
   
   offset = glyph_info._offset;
   
   return glyph_info._image;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init
==============================================================================
*/

void  Font::init ()
{
   FT_Library & library = FTLibrary::use_instance ().get ();
   
   FontResources::Result result = FontResources::use_instance ().get (_name);
   
   FT_Error err = 0;
   
   if (result._memory_flag)
   {
      err = FT_New_Memory_Face (
         library,
         (const FT_Byte *) result._data_ptr,
         result._data_size,
         0,
         &_face
      );
      assert (err == 0);
      assert (_face != 0);
   }
   else
   {
      err = FT_New_Face (
         library,
         result._path.c_str (),
         0,
         &_face
      );
      assert (err == 0);
      assert (_face != 0);
   }
   
   int char_height = int (_size * 64.f);
   
   err = FT_Set_Char_Size (
      _face,
      0, // char width,
      char_height,
      72, 72   // h & v resolutions
   );
   assert (err == 0);
   
   err = FT_Select_Charmap (
      _face,
      FT_ENCODING_UNICODE
   );

   FT_Size scaled_size_ptr = _face->size;
   assert (scaled_size_ptr != 0);
   
   _units_per_em = _face->units_per_EM;
   assert (_units_per_em != 0);
   
   const FT_Size_Metrics & scaled_metrics = scaled_size_ptr->metrics;
   
   _ascent = float (scaled_metrics.ascender) * (1.f / 64.f);
   assert (_ascent != 0);
   
   _descent = float (- scaled_metrics.descender) * (1.f / 64.f);
   assert (_descent != 0);
   
   _leading = float (
      scaled_metrics.height
      - scaled_metrics.ascender
      + scaled_metrics.descender
   ) * (1.f / 64.f);
   // leading might be null (ex : times)
   
   FT_UShort ppem = scaled_metrics.y_ppem;
      
   FT_Short up = _face->underline_position;
   
   _underline_position = (float (up) * float (ppem)) / float (_units_per_em);
   
   FT_Short ut = _face->underline_thickness;

   _underline_thickness = (float (ut) * float (ppem)) / float (_units_per_em);
   
   _strikethrough_position = _ascent * .3f;
   
   _strikethrough_thickness = _underline_thickness;
   
   // list available tables
   
   _font_table_glyph_metrics_aptr = std::auto_ptr <FontTableGlyphMetrics> (
      new FontTableGlyphMetrics (_face)
   );

   _font_table_kern_aptr = std::auto_ptr <FontTableKern> (
      new FontTableKern (_face, _units_per_em, _size)
   );

   _font_table_gsub_aptr = std::auto_ptr <FontTableGlyphSubstitution> (
      new FontTableGlyphSubstitution (_face)
   );
}


   
/*
==============================================================================
Name : load_glyph
==============================================================================
*/

Font::GidGlyphInfoMap::iterator  Font::load_glyph (GidGlyphInfoMap & gid_glyph_info_map, Engine & engine, archi::UInt32 gid) const
{
   GidGlyphInfoMap::iterator it
      = gid_glyph_info_map.insert (std::make_pair (gid, GlyphInfo ())).first;

   GlyphInfo & glyph_info = it->second;
   
   opa::Rect image_bounds;
   get_glyph_image_metrics (image_bounds, gid);
   
   Image & image = glyph_info._image;
   
   engine.prepare (
      image, size_t (image_bounds._size._width), size_t (image_bounds._size._height)
   );
   
   PixArgb * buf_ptr = new PixArgb [image._scanline_width * image._tex_height];
   
   ::memset (buf_ptr, 0, image._scanline_width * image._tex_height * sizeof (PixArgb));
   
   glyph_info._offset = image_bounds._origin;
   
   render_glyph (
      buf_ptr,
      image._width, image._scanline_width, image._tex_height,
      gid, - glyph_info._offset
   );
   
   Image::Conf conf;
   conf._sprung_mode_x = SprungMode_DISTEND;
   conf._sprung_mode_y = SprungMode_DISTEND;
   conf._filter_mode = FilterMode_NEAREST;
   
   bool buf_owner_flag = true;
   engine.create (image, buf_ptr, conf, buf_owner_flag);
   
   return it;
}



/*
==============================================================================
Name : get_glyph_image_metrics
Description :
   Get image metrics of a glyph with index 'gid'
   'image_bounds' : relative to glyph pen, bounds that enclose completely
   the image of a glyph when rendered.
==============================================================================
*/

void  Font::get_glyph_image_metrics (opa::Rect & image_bounds, archi::UInt32 gid) const
{
   // do not render
   FT_Int32 load_flags = 0;
   load_flags |= FT_LOAD_DEFAULT;
   load_flags |= FT_LOAD_TARGET_LIGHT;
   
   FT_Error err = FT_Load_Glyph (_face, gid, load_flags);
   assert (err == 0);
   
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
      
      image_bounds._origin._x = float (metrics.horiBearingX) * div64;
      image_bounds._origin._y = - float (metrics.horiBearingY) * div64;
      image_bounds._size._width = float (metrics.width) * div64;
      image_bounds._size._height = float (metrics.height) * div64;
   }
   else
   {
      image_bounds = opa::Rect::_zero;
   }
}



/*
==============================================================================
Name : render_glyph
==============================================================================
*/

void  Font::render_glyph (PixArgb * buf_ptr, int buf_width, int buf_scanline_width, int buf_height, archi::UInt32 glyph_index, opa::Point origin) const
{
   if (glyph_index == 0) return;
   
   FT_GlyphSlot slot = _face->glyph;
   
   FT_Int32 load_flags = 0;
   load_flags |= FT_LOAD_RENDER;
   load_flags |= FT_LOAD_TARGET_LIGHT;
   
   FT_Error err = FT_Load_Glyph (_face, glyph_index, load_flags);
   
   if (err == 0)
   {
      FT_Int left = slot->bitmap_left;
      FT_Int top = slot->bitmap_top;
      
      FT_Bitmap & bitmap = slot->bitmap;
      int rows = bitmap.rows;
      int width = bitmap.width;
      int pitch = bitmap.pitch;
      unsigned char * bits_ptr = bitmap.buffer;
      
      assert (
         (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
         || (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
      );
      assert (
         (bitmap.num_grays == 256)
         || (bitmap.num_grays == 0)
      );
      
      int x = int (origin._x);
      int y = int (origin._y);
      
      bool ok_flag = true;
      ok_flag &= x + left >= 0;
      ok_flag &= y - top >= 0;
      ok_flag &= x + left + width <= buf_width;
      ok_flag &= y - top + rows <= buf_height;
      
      assert (ok_flag);
      
      if (ok_flag)
      {
         buf_ptr += (y - top) * buf_scanline_width + x + left;
         
         for (int j = 0 ; j < rows ; ++j)
         {
            for (int i = 0 ; i < width ; ++i)
            {
               PixArgb & pix = buf_ptr [i];
               
               int val = 0;
               
               if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
               {
                  val = bits_ptr [i];
               }
               else if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
               {
                  val = bits_ptr [i >> 3];
                  
                  val &= (1 << (7 - (i & 7)));
                  
                  val = (val != 0) ? 255 : 0;
               }
               
               // do not erase previous blit of an adjacent glyph
               val = lang::max (val, int (pix._a));

               pix._a = val;
               pix._r = val;
               pix._g = val;
               pix._b = val;
            }
            
            buf_ptr += buf_scanline_width;
            bits_ptr += pitch;
         }
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
