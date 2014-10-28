/*****************************************************************************

        Typesetter.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45859

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



#if ! defined (ohm_opal_Typesetter_HEADER_INCLUDED)
#define  ohm_opal_Typesetter_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/opa/Point.h"
#include "ohm/txt/unicode/Char.h"

#include "ohm/opal/private/Glyph.h"
#include "ohm/opal/Scripture.h"

#include <map>
#include <vector>



namespace ohm
{
namespace opal
{


class GraphicContextInterface;
class Color;

class Typesetter
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class ReorderedCharRange
   {
   public:
      size_t         _start;
      size_t         _end;
   };
   
   class Char
   {
   public:
      txt::unicode::Char
                     _c;
      size_t         _glyph_index;
   };

   typedef std::vector <Char> CharArray;
   
   class Run
   {
   public:
      Scripture::Range
                  _scripture_range;
      GlyphRange  _glyph_range;
      
      float       _ascent;
      float       _descent;
      float       _leading;
      float       _width;
      bool        _image_flag;
      
      float       _margin_left;
      float       _margin_right;
      float       _image_top;
      float       _image_bottom;
   };
   
   typedef  std::vector <Run> RunArray;
   
   typedef bool Direction;
   
   class LineInfo
   {
   public:
      std::vector <Run>
                  _run_arr;
      float       _space_before;
      float       _ascent;
      float       _descent;
      float       _leading;
      float       _space_after;
      
      float       _width;

      float       _margin_left;
      float       _margin_right;
      float       _image_top;
      float       _image_bottom;

      bool        _explicit_line_break_flag;
      Scripture::Range
                  _scripture_range;
      Direction   _direction; // for paragraph, interpret left or right aligned (head/tail aligned)
   };
   
   typedef std::vector <LineInfo> LineInfoArray;

                  Typesetter (Scripture & scripture);
   virtual        ~Typesetter ();
   
   float          suggest_line_break (size_t & scripture_end, float & height, size_t scripture_start, float width);
   float          suggest_char_break (size_t & scripture_end, float & height, size_t scripture_start, float width);
   const LineInfo *
                  layout_line (const Scripture::Range & scripture_range, float width, bool justify_flag, const LineInfo * last_line_info_ptr);
   LineInfoArray::iterator
                  begin_line ();
   LineInfoArray::iterator
                  end_line ();
   
   size_t         location_to_position (opa::Point location);
   size_t         location_to_position (const LineInfo & line_info, opa::Point location, bool last_line_flag);
   size_t         location_to_position (const Run & run, opa::Point location);
   size_t         location_to_position (const Glyph & glyph, opa::Point location);
   
   void           draw (GraphicContextInterface & context, const opa::Point & pt, const LineInfo & line_info);
   void           draw (GraphicContextInterface & context, const opa::Point & pt, const Run & run);

   void           get_caret_location (opa::Point & location, size_t pos, opa::Point offset);
   void           get_selection_location (opa::Point & location, float & width, size_t pos, opa::Point offset);

   void           draw_caret (GraphicContextInterface & context, size_t pos, opa::Point offset, const Color & color);
   
   // access for glyph substitution
   Glyph &        get_glyph (size_t pos);
   GlyphArray::iterator
                  get_glyph_iterator (size_t pos);
   void           erase_glyph (size_t pos, GlyphRange & glyph_range);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           preprocess ();
   void           process_bidi_cmap ();
   void           process_glyph_subtitutions (const Scripture::Range & scripture_range, const Run & run);
   
   void           measure (float & block_width, float & block_padding, float & block_ascent, float & block_descent, const Scripture::Range & scripture_range);
   void           measure (float & block_width, float & block_padding, float & block_ascent, float & block_descent, const Scripture::Range & scripture_range, const Run & run);

   void           layout_run (Run & dst_run, float & x, const Scripture::Range & scripture_range, const Run & src_run);

   void           justify ();
   
   void           scripture_pos_to_char_pos (size_t & char_pos, size_t scripture_pos);
   
   void           scripture_range_to_glyph_range (GlyphRange & glyph_range, const Scripture::Range & scripture_range, const Run & run);
   void           scripture_pos_to_glyph_pos (size_t & glyph_pos, size_t scripture_pos, const Run & run);
   RunArray::iterator
                  find_run (size_t scripture_pos);
   
   Scripture &    _scripture;

   std::vector <size_t>
                  _scripture_pos_to_char_pos_arr;
   
   CharArray      _char_array;
   RunArray       _run_array;
   
   GlyphArray     _glyph_array;
   
   LineInfoArray  _line_info_array;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Typesetter ();
                  Typesetter (const Typesetter &other);
   Typesetter &   operator = (const Typesetter &other);
   bool           operator == (const Typesetter &other);
   bool           operator != (const Typesetter &other);

}; // class Typesetter



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Typesetter_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
