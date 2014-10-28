/*****************************************************************************

        Font.h
        Copyright (c) 2008 Ohm Force

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



#if ! defined (ohm_opal_Font_HEADER_INCLUDED)
#define  ohm_opal_Font_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/freetype/FontTableGlyphMetrics.h"
#include "ohm/opal/freetype/FontTableKern.h"
#include "ohm/opal/freetype/FontTableGlyphSubstitution.h"
#include "ohm/opal/private/Glyph.h"

#include "ohm/opa/Rect.h"

#include "ohm/sys/Mutex.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/txt/unicode/Char.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <string>
#include <map>



namespace ohm
{
namespace opal
{



class Typesetter;
class TypescriptMeasure;
class GraphicContextInterface;

class Font
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class GlyphInfo
   {
   public:
      Image       _image;
      opa::Point  _offset;
   };
   
   typedef std::map <archi::UInt32, GlyphInfo>  GidGlyphInfoMap;
   
   class Glyphs
   {
   public:
                  Glyphs (Engine & engine, const Font & parent);
      virtual     ~Glyphs ();
      
      const Image &
                  get_glyph_mask (opa::Point & offset, archi::UInt32 gid) const;
   
   private:
      const Font &
                  _parent;
      Engine &    _engine;
      mutable GidGlyphInfoMap
                  _gid_glyph_info_map;
   };
   
                  Font (const std::string & name, float size);
   virtual        ~Font ();
   
   void           release (Engine & engine);
   
   sys::ThreadMgr::IdType
                  get_thread_id () const;
   const std::string &
                  get_name () const;
   float          get_size () const;

   float          get_ascent () const;
   float          get_descent () const;
   float          get_leading () const;

   float          get_underline_position () const;
   float          get_underline_thickness () const;
   float          get_strikethrough_position () const;
   float          get_strikethrough_thickness () const;

   archi::UInt32  get_gid (txt::unicode::Char c) const;
   const Glyphs & get_glyphs (GraphicContextInterface & gc) const;

   void           process_substitution (Typesetter & typesetter, GlyphRange & glyph_range) const;

   float          measure (archi::UInt32 gid) const;
   void           measure (TypescriptMeasure & measure, const std::vector <Glyph> & glyph_arr, size_t start, size_t end) const;
   void           process_position (TypescriptMeasure & measure, std::vector <Glyph> & glyph_arr, size_t start, size_t end) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class Glyphs;
   
   typedef std::map <Engine *, Glyphs> EngineGlyphsMap;
   
   void           init ();
   
   GidGlyphInfoMap::iterator
                  load_glyph (GidGlyphInfoMap & gid_glyph_info_map, Engine & engine, archi::UInt32 gid) const;
   void           get_glyph_image_metrics (opa::Rect & image_bounds, archi::UInt32 gid) const;
   void           render_glyph (PixArgb * buf_ptr, int buf_width, int buf_scanline_width, int buf_height, archi::UInt32 glyph_index, opa::Point origin) const;
   
   const sys::ThreadMgr::IdType
                  _thread_id;

   const std::string
                  _name;
   const float    _size;

   FT_Face        _face;

   archi::UInt32  _units_per_em;
   
   float          _ascent;
   float          _descent;
   float          _leading;

   float          _underline_position;
   float          _underline_thickness;
   float          _strikethrough_position;
   float          _strikethrough_thickness;
   
   mutable EngineGlyphsMap
                  _engine_glyphs_map;
   
   std::auto_ptr <FontTableGlyphMetrics>
                  _font_table_glyph_metrics_aptr;
   std::auto_ptr <FontTableKern>
                  _font_table_kern_aptr;
   std::auto_ptr <FontTableGlyphSubstitution>
                  _font_table_gsub_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Font ();
                  Font (const Font &other);
   Font &         operator = (const Font &other);
   bool           operator == (const Font &other);
   bool           operator != (const Font &other);

}; // class Font



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Font_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
