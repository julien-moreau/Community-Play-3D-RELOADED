/*****************************************************************************

        Typesetter.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70452

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

#include "ohm/txt/unicode/Categ.h"
#include "ohm/txt/unicode/CharData.h"
#include "ohm/lang/fnc.h"

#include "ohm/opal/private/Typesetter.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/Font.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/private/TypescriptMeasure.h"

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

Typesetter::Typesetter (Scripture & scripture)
:  _scripture (scripture)
{
   preprocess ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Typesetter::~Typesetter ()
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
Name : suggest_line_break
==============================================================================
*/

float Typesetter::suggest_line_break (size_t & scripture_end, float & height, size_t scripture_start, float width)
{
   if (_scripture.size () == 0)
   {
      scripture_end = 0;
      height = 0.f;
      
      return 0.f;
   }
   
   float cur_width = 0;
   float cur_visual_width = 0;
   float ascent = 0;
   float descent = 0;
   
   size_t pos = scripture_start;
   
   size_t break_pos = _scripture.get_next_explicit_break (pos);
   
   for (; pos < break_pos ;)
   {
      size_t next_pos = _scripture.get_next_possible_line_break (pos);
      
      float block_width = 0.f;
      float block_padding = 0.f;
      float block_ascent = 0.f;
      float block_descent = 0.f;
      
      Scripture::Range scripture_range;
      scripture_range._start = pos;
      scripture_range._end = next_pos;
      
      measure (block_width, block_padding, block_ascent, block_descent, scripture_range);
      
      if (cur_width + block_width > width)
      {
         break;
      }
      
      cur_visual_width = cur_width + block_width;
      cur_width += block_width + block_padding;
      
      if (block_ascent > ascent) ascent = block_ascent;
      if (block_descent > descent) descent = block_descent;
      
      pos = next_pos;
   }
   
   if (pos == scripture_start)
   {
      // no block fits. In that case we
      // will have a local line overload, so reply
      // the next possible line break
      
      size_t next_pos = _scripture.get_next_possible_line_break (pos);
      
      float block_width = 0.f;
      float block_padding = 0.f;
      float block_ascent = 0.f;
      float block_descent = 0.f;
      
      Scripture::Range scripture_range;
      scripture_range._start = pos;
      scripture_range._end = next_pos;
      
      measure (block_width, block_padding, block_ascent, block_descent, scripture_range);
      
      cur_visual_width = block_width;
      pos = next_pos;
      ascent = block_ascent;
      descent = block_descent;
   }
   
   scripture_end = pos;
   height = ascent + descent;
   
   return cur_visual_width;
}



/*
==============================================================================
Name : suggest_char_break
==============================================================================
*/

float Typesetter::suggest_char_break (size_t & scripture_end, float & height, size_t scripture_start, float width)
{
   if (_scripture.size () == 0)
   {
      scripture_end = 0;
      height = 0.f;
      
      return 0.f;
   }
   
   float cur_width = 0;
   float cur_visual_width = 0;
   float ascent = 0;
   float descent = 0;
   
   size_t pos = scripture_start;
   
   size_t break_pos = _scripture.get_next_explicit_break (pos);
   
   for (; pos < break_pos ;)
   {
      size_t next_pos = _scripture.get_next_cluster_boundary (pos);
      
      float block_width = 0.f;
      float block_padding = 0.f;
      float block_ascent = 0.f;
      float block_descent = 0.f;
      
      Scripture::Range scripture_range;
      scripture_range._start = pos;
      scripture_range._end = next_pos;
      
      measure (block_width, block_padding, block_ascent, block_descent, scripture_range);
      
      if (cur_width + block_width > width)
      {
         break;
      }
      
      cur_visual_width = cur_width + block_width;
      cur_width += block_width + block_padding;
      
      if (block_ascent > ascent) ascent = block_ascent;
      if (block_descent > descent) descent = block_descent;
      
      pos = next_pos;
   }
   
   if (pos == scripture_start)
   {
      // no block fits. In that case we
      // will have a local line overload, so reply
      // the next possible line break
      
      size_t next_pos = _scripture.get_next_cluster_boundary (pos);
      
      float block_width = 0.f;
      float block_padding = 0.f;
      float block_ascent = 0.f;
      float block_descent = 0.f;
      
      Scripture::Range scripture_range;
      scripture_range._start = pos;
      scripture_range._end = next_pos;
      
      measure (block_width, block_padding, block_ascent, block_descent, scripture_range);
      
      cur_visual_width = block_width;
      pos = next_pos;
      ascent = block_ascent;
      descent = block_descent;
   }
   
   scripture_end = pos;
   height = ascent + descent;
   
   return cur_visual_width;
}



/*
==============================================================================
Name : layout_line
==============================================================================
*/

const Typesetter::LineInfo *  Typesetter::layout_line (const Scripture::Range & scripture_range, float width, bool justify_flag, const LineInfo * last_line_info_ptr)
{
   // last_line_ptr is used (_line_direction, _explicit_line_break_flag) to determine line direction
   // width is used for justification
   
   _line_info_array.insert (_line_info_array.end (), LineInfo ());
   
   size_t line_nbr = _line_info_array.size () - 1;
   
   {
      LineInfo & line_info = _line_info_array [line_nbr];
      
      line_info._space_before = 0.f;
      line_info._ascent = 0.f;
      line_info._descent = 0.f;
      line_info._leading = 0.f;
      line_info._space_after = 0.f;

      line_info._width = 0.f;

      line_info._margin_left = 0.f;
      line_info._margin_right = 0.f;
      line_info._image_top = 0.f;
      line_info._image_bottom = 0.f;
      
#pragma message ("check this")

      line_info._explicit_line_break_flag = false;
      line_info._scripture_range = scripture_range;
      
#pragma message ("check this")

      line_info._direction = 0;
      
      float x = 0.f;
      
      for (size_t pos = scripture_range._start ; pos < scripture_range._end ;)
      {
         size_t next_pos = _scripture.get_next_run (pos);
         lang::min_in_place (next_pos, scripture_range._end);
         
         Scripture::Range run_range;
         run_range._start = pos;
         run_range._end = next_pos;
         
         const RunArray::iterator run_it = find_run (pos);
         assert (run_it != _run_array.end ());
         
         Run new_run;
         
         layout_run (new_run, x, run_range, *run_it);
         
         lang::max_in_place (line_info._ascent, new_run._ascent);
         lang::max_in_place (line_info._descent, new_run._descent);
         lang::max_in_place (line_info._leading, new_run._leading);

         line_info._width += new_run._width;
         
         if (pos == scripture_range._start)
         {
            line_info._margin_left = new_run._margin_left;
         }
         
         line_info._margin_right = new_run._margin_right;

         lang::min_in_place (line_info._image_top, new_run._image_top);
         lang::max_in_place (line_info._image_bottom, new_run._image_bottom);
         
         line_info._run_arr.push_back (new_run);

         pos = next_pos;
      }
   }
   
#pragma message ("to do")
   // if the last char of the whole scripture is an explicit break
   // then create a fake line
   
   if (scripture_range._end == _scripture.size ())
   {
      assert (_scripture.size () > 0);
      
      txt::unicode::Char c = _scripture.get_char (_scripture.size () - 1);
      
      txt::unicode::LineBreakProp line_break_prop
         = txt::unicode::CharData::get_line_break_prop (c);
      
      bool explicit_break_flag = false;
      
      switch (line_break_prop)
      {
      case txt::unicode::LineBreakProp_PS:
      case txt::unicode::LineBreakProp_BK:
      case txt::unicode::LineBreakProp_CR:
      case txt::unicode::LineBreakProp_LF:
      case txt::unicode::LineBreakProp_NL:
         explicit_break_flag = true;
         break;
      
      default:
         break;
      }
      
      if (explicit_break_flag)
      {
         LineInfoArray::iterator it
            = _line_info_array.insert (_line_info_array.end (), LineInfo ());
         
         LineInfo & sup_line_info = *it;
         
         LineInfo & line_info = _line_info_array [line_nbr];
         
         sup_line_info._space_before = 0.f;
         sup_line_info._ascent = line_info._ascent;
         sup_line_info._descent = line_info._descent;
         sup_line_info._leading = line_info._leading;
         sup_line_info._space_after = 0.f;

         sup_line_info._width = 0.f;
         sup_line_info._explicit_line_break_flag = true;
         sup_line_info._scripture_range._start = _scripture.size ();
         sup_line_info._scripture_range._end = _scripture.size ();
         sup_line_info._direction = line_info._direction;
      }
   }
   
   if (justify_flag)
   {
      justify ();
   }
   
   return &_line_info_array [line_nbr];
}



/*
==============================================================================
Name : begin_line
==============================================================================
*/

Typesetter::LineInfoArray::iterator Typesetter::begin_line ()
{
   return _line_info_array.begin ();
}



/*
==============================================================================
Name : end_line
==============================================================================
*/

Typesetter::LineInfoArray::iterator Typesetter::end_line ()
{
   return _line_info_array.end ();
}



/*
==============================================================================
Name : location_to_position
==============================================================================
*/

size_t   Typesetter::location_to_position (opa::Point location)
{
   if (_line_info_array.empty ())
   {
      return 0;
   }
   
   // position is relative from top/left
   
   // all lines are in a stack
   
   LineInfo & first_line = _line_info_array.front ();
   
   location._y -= first_line._space_before + first_line._ascent;
   
   LineInfoArray::iterator it = _line_info_array.begin ();
   LineInfoArray::iterator it_end = _line_info_array.end ();
   LineInfoArray::iterator it_last = it_end;
   --it_last;
   
   for (; it != it_end ;)
   {
      LineInfo & line_info = *it;
      
      bool in_line_flag
         = location._y <= line_info._descent + line_info._leading + line_info._space_after;
      ;
      
      bool last_line_flag = it == it_last;
      
      if (in_line_flag || last_line_flag)
      {
         size_t pos = location_to_position (line_info, location, last_line_flag);
         
         return pos;
      }
      
      // update to next line relative position
      
      location._y -= line_info._descent + line_info._leading + line_info._space_after;
      
      ++it;
       
      LineInfo & next_line_info = *it;
      
      location._y -= next_line_info._space_before + next_line_info._ascent;
   }
   
   return 0;
}



/*
==============================================================================
Name : location_to_position
==============================================================================
*/

size_t   Typesetter::location_to_position (const LineInfo & line_info, opa::Point location, bool last_line_flag)
{
   // position is relative to line left/baseline
   
   // at that moment we know that the click is in the line
   
   size_t start = line_info._scripture_range._start;
   size_t end = line_info._scripture_range._end - 1;
   
   if (last_line_flag)
   {
      start = line_info._scripture_range._start;
      end = line_info._scripture_range._end;
   }
   
   if (location._x <= 0.f)
   {
      return  line_info._direction ? end : start;
   }
   
   const RunArray & run_arr = line_info._run_arr;
   
   RunArray::const_iterator it = run_arr.begin ();
   const RunArray::const_iterator it_end = run_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const Run & run = *it;
      
      if (location._x < run._width)
      {
         size_t pos = location_to_position (run, location);
      
         // at the end
         lang::limit_in_place (pos, start, end);
         
         return pos;
      }
      
      location._x -= run._width;
   }
   
   return  line_info._direction ? start : end;
}



/*
==============================================================================
Name : location_to_position
==============================================================================
*/

size_t   Typesetter::location_to_position (const Run & run, opa::Point location)
{
   // position is relative to *run* left/baseline
   
   GlyphArray::iterator it = _glyph_array.begin () + run._glyph_range._start;
   const GlyphArray::iterator it_end = _glyph_array.begin () + run._glyph_range._end;
   
   for (; it != it_end ; ++it)
   {
      const Glyph & glyph = *it;
      
      if (location._x < glyph._width)
      {
         size_t pos = location_to_position (glyph, location);
         
         return pos;
      }
      
      location._x -= glyph._width;
   }
   
   assert (false);
   return 0;
}



/*
==============================================================================
Name : location_to_position
==============================================================================
*/

size_t   Typesetter::location_to_position (const Glyph & glyph, opa::Point location)
{
   if (!glyph._ligature_flag)
   {
      if (location._x < glyph._width * .5f)
      {
         return glyph._direction ? glyph._scripture_index + 1 : glyph._scripture_index;
      }
      else
      {
         return glyph._direction ? glyph._scripture_index : glyph._scripture_index + 1;
      }
   }
   else
   {
      /*get glyph caret info
      apply same idea than before // normally char index are consecutive*/
      
#pragma message ("check this")
      return 0;
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  Typesetter::draw (GraphicContextInterface & context, const opa::Point & pt, const LineInfo & line_info)
{
   const std::vector <Run> & run_arr = line_info._run_arr;
   
   opa::Point position = pt;
   
   for (size_t i = 0 ; i < run_arr.size () ; ++i)
   {
      const Run & run = run_arr [i];
      
      draw (context, position, run);
      
      position._x += run._width;
   }
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  Typesetter::draw (GraphicContextInterface & context, const opa::Point & pt, const Run & run)
{
   const Font * old_font_ptr = 0;
   const Font::Glyphs * old_glyphs_ptr = 0;
   
   for (size_t pos = run._glyph_range._start ; pos < run._glyph_range._end ; ++pos)
   {
      const Glyph & glyph = _glyph_array [pos];
      size_t scripture_pos = glyph._scripture_index;
      
      const Font * font_ptr = _scripture.get_font (scripture_pos);
      
      if (font_ptr != 0)
      {
         const Font::Glyphs * glyphs_ptr = 0;
         
         if (font_ptr == old_font_ptr)
         {
            assert (old_glyphs_ptr != 0);
            glyphs_ptr = old_glyphs_ptr;
         }
         else
         {
            glyphs_ptr = &font_ptr->get_glyphs (context);
            
            old_font_ptr = font_ptr;
            old_glyphs_ptr = glyphs_ptr;
         }
         
         Scripture::CharStyle char_style = _scripture.get_char_style (scripture_pos);
         
         opa::Point offset;
         const Image & image = glyphs_ptr->get_glyph_mask (offset, glyph._gid);
         
         opa::Point draw_pt = pt + opa::Point (glyph._x, glyph._y) + offset;
         
         if (char_style == Scripture::CharStyle_CARVED)
         {
            const Color & second_color = _scripture.get_second_color (scripture_pos);
            
            context.draw (image, draw_pt - opa::Point (0, -1), second_color);
            context.draw (image, draw_pt - opa::Point (0, 1), second_color);
         }
         else if (char_style == Scripture::CharStyle_BORDERED)
         {
            const Color & second_color = _scripture.get_second_color (scripture_pos);
            
            context.draw (image, draw_pt - opa::Point (0, -1), second_color);
            context.draw (image, draw_pt - opa::Point (0, 1), second_color);
            context.draw (image, draw_pt - opa::Point (-1, 0), second_color);
            context.draw (image, draw_pt - opa::Point (1, 0), second_color);
         }
         
         const Color & color = _scripture.get_color (scripture_pos);
         
         context.draw (image, draw_pt, color);
      }
      else
      {
         Image * image_ptr = _scripture.get_image (scripture_pos);
         assert (image_ptr != 0);
         
         context.draw (*image_ptr, pt + opa::Point (glyph._x, glyph._y));
      }
   }
}



/*
==============================================================================
Name : get_caret_location
==============================================================================
*/

void  Typesetter::get_caret_location (opa::Point & location, size_t pos, opa::Point offset)
{
   if (!_line_info_array.empty ())
   {
      LineInfoArray::iterator it = _line_info_array.begin ();
      LineInfoArray::iterator it_end = _line_info_array.end ();
      LineInfoArray::iterator it_last = it_end;
      --it_last;
      
      for (; it != it_end ; ++it)
      {
         const LineInfo & line_info = *it;
         
         bool null_range_flag
            = line_info._scripture_range._start == line_info._scripture_range._end;
         
         bool in_line_flag = pos >= line_info._scripture_range._start;
         bool last_line_flag = it == it_last;
         
         if ((!null_range_flag) && (!last_line_flag))
         {
            in_line_flag &= pos < line_info._scripture_range._end;
         }
         
         if (in_line_flag)
         {
            float space_before = line_info._space_before;
            float ascent = line_info._ascent;
            float descent = line_info._descent;
            
            if (pos < _scripture.size ())
            {
               const RunArray & run_arr = line_info._run_arr;
               
               RunArray::const_iterator rit = run_arr.begin ();
               RunArray::const_iterator rit_end = run_arr.end ();
               
               float x = 0.f;
               
               for (; rit != rit_end ; ++rit)
               {
                  const Run & run = *rit;
                  
                  if ((pos >= run._scripture_range._start)
                     && (pos < run._scripture_range._end))
                  {
                     size_t glyph_pos = 0;
                     
                     scripture_pos_to_glyph_pos (glyph_pos, pos, run);
                     
                     const Glyph & glyph = _glyph_array [glyph_pos];
                     
                     x += glyph._x;
                     
                     location._x = offset._x + x;
                     location._y = offset._y + space_before + ascent;

                     break;
                  }
                  
                  x += run._width;
               }
            }
            else
            {
               assert (pos == _scripture.size ());
               
               const RunArray & run_arr = line_info._run_arr;
               
               float x = 0.f;
                  
               if (!run_arr.empty ())
               {
                  RunArray::const_iterator rit = run_arr.begin ();
                  RunArray::const_iterator rit_end = run_arr.end ();
                  RunArray::const_iterator rit_last = rit_end;
                  --rit_last;
                  
                  for (; rit != rit_last ; ++rit)
                  {
                     const Run & run = *rit;
                     
                     x += run._width;
                  }
                  
                  const Run & run = run_arr.back ();
                  
                  assert (run._glyph_range._end > 0);
                  
                  size_t glyph_pos = run._glyph_range._end - 1;
                  assert (glyph_pos >= run._glyph_range._start);
                  
                  const Glyph & glyph = _glyph_array [glyph_pos];
                  
                  x += glyph._x + glyph._width;
               }
               
               location._x = offset._x + x;
               location._y = offset._y + space_before + ascent;
            }
            
            break;
         }
         
         offset._y
            += line_info._space_before
            + line_info._ascent
            + line_info._descent
            + line_info._leading
            + line_info._space_after;
      }
   }
   else
   {
      const Font & font = _scripture.get_main_state_font ();
      
#pragma message ("to update with real space before in scripture state")
      const float space_before = 0.f;
      const float ascent = font.get_ascent ();
      const float descent = font.get_descent ();
      
      location._x = offset._x;
      location._y = offset._y + space_before + ascent;
   }
}



/*
==============================================================================
Name : get_selection_location
==============================================================================
*/

void  Typesetter::get_selection_location (opa::Point & location, float & width, size_t pos, opa::Point offset)
{
   assert (!_line_info_array.empty ());
   
   LineInfoArray::iterator it = _line_info_array.begin ();
   LineInfoArray::iterator it_end = _line_info_array.end ();
   LineInfoArray::iterator it_last = it_end;
   --it_last;
   
   for (; it != it_end ; ++it)
   {
      const LineInfo & line_info = *it;
      
      bool null_range_flag
         = line_info._scripture_range._start == line_info._scripture_range._end;
      
      bool in_line_flag = pos >= line_info._scripture_range._start;
      bool last_line_flag = it == it_last;
      
      if ((!null_range_flag) && (!last_line_flag))
      {
         in_line_flag &= pos < line_info._scripture_range._end;
      }
      
      if (in_line_flag)
      {
         float space_before = line_info._space_before;
         float ascent = line_info._ascent;
         float descent = line_info._descent;
         
         assert (pos < _scripture.size ());
         
         const RunArray & run_arr = line_info._run_arr;
         
         RunArray::const_iterator rit = run_arr.begin ();
         RunArray::const_iterator rit_end = run_arr.end ();
         
         float x = 0.f;
         
         for (; rit != rit_end ; ++rit)
         {
            const Run & run = *rit;
            
            if ((pos >= run._scripture_range._start)
               && (pos < run._scripture_range._end))
            {
               size_t glyph_pos = 0;
               
               scripture_pos_to_glyph_pos (glyph_pos, pos, run);

               const Glyph & glyph = _glyph_array [glyph_pos];
               
               x += glyph._x;
               
               location._x = offset._x + x;
               location._y = offset._y + space_before + ascent;
               width = glyph._width;

               break;
            }
            
            x += run._width;
         }
         
         break;
      }
      
      offset._y
         += line_info._space_before
         + line_info._ascent
         + line_info._descent
         + line_info._leading
         + line_info._space_after;
   }
}



/*
==============================================================================
Name : draw_caret
==============================================================================
*/

void  Typesetter::draw_caret (GraphicContextInterface & context, size_t pos, opa::Point offset, const Color & color)
{
   context.save_graphic_state ();
   context.set_fill_color (color);
   
   if (!_line_info_array.empty ())
   {
      LineInfoArray::iterator it = _line_info_array.begin ();
      LineInfoArray::iterator it_end = _line_info_array.end ();
      LineInfoArray::iterator it_last = it_end;
      --it_last;
      
      for (; it != it_end ; ++it)
      {
         const LineInfo & line_info = *it;
         
         bool null_range_flag
            = line_info._scripture_range._start == line_info._scripture_range._end;
         
         bool in_line_flag = pos >= line_info._scripture_range._start;
         bool last_line_flag = it == it_last;
         
         if ((!null_range_flag) && (!last_line_flag))
         {
            in_line_flag &= pos < line_info._scripture_range._end;
         }
         
         if (in_line_flag)
         {
            float space_before = line_info._space_before;
            float ascent = line_info._ascent;
            float descent = line_info._descent;
            
            if (pos < _scripture.size ())
            {
               const RunArray & run_arr = line_info._run_arr;
               
               RunArray::const_iterator rit = run_arr.begin ();
               RunArray::const_iterator rit_end = run_arr.end ();
               
               float x = 0.f;
               
               for (; rit != rit_end ; ++rit)
               {
                  const Run & run = *rit;
                  
                  if ((pos >= run._scripture_range._start)
                     && (pos < run._scripture_range._end))
                  {
                     size_t glyph_pos = 0;
                     
                     scripture_pos_to_glyph_pos (glyph_pos, pos, run);
                     
                     const Glyph & glyph = _glyph_array [glyph_pos];
                     
                     x += glyph._x;
                     
                     opa::Rect rect (
                        offset._x + x, offset._y + space_before,
                        1, ascent + descent
                     );
                     
                     context.fill (rect);

                     break;
                  }
                  
                  x += run._width;
               }
            }
            else
            {
               assert (pos == _scripture.size ());
               
               const RunArray & run_arr = line_info._run_arr;
               
               float x = 0.f;
                  
               if (!run_arr.empty ())
               {
                  RunArray::const_iterator rit = run_arr.begin ();
                  RunArray::const_iterator rit_end = run_arr.end ();
                  RunArray::const_iterator rit_last = rit_end;
                  --rit_last;
                  
                  for (; rit != rit_last ; ++rit)
                  {
                     const Run & run = *rit;
                     
                     x += run._width;
                  }
                  
                  const Run & run = run_arr.back ();
                  
                  assert (run._glyph_range._end > 0);
                  
                  size_t glyph_pos = run._glyph_range._end - 1;
                  assert (glyph_pos >= run._glyph_range._start);
                  
                  const Glyph & glyph = _glyph_array [glyph_pos];
                  
                  x += glyph._x + glyph._width;
               }
               
               opa::Rect rect (
                  offset._x + x, offset._y + space_before,
                  1, ascent + descent
               );
               
               context.fill (rect);
            }
            
            break;
         }
         
         offset._y
            += line_info._space_before
            + line_info._ascent
            + line_info._descent
            + line_info._leading
            + line_info._space_after;
      }
   }
   else
   {
      const Font & font = _scripture.get_main_state_font ();
      
#pragma message ("to update with real space before in scripture state")
      const float space_before = 0.f;
      const float ascent = font.get_ascent ();
      const float descent = font.get_descent ();
      
      opa::Rect rect (
         offset._x, offset._y + space_before,
         1, ascent + descent
      );
      
      context.fill (rect);
   }
   
   context.restore_graphic_state ();
}



/*
==============================================================================
Name : get_glyph
==============================================================================
*/

Glyph &  Typesetter::get_glyph (size_t pos)
{
   assert (pos < _glyph_array.size ());
   
   return _glyph_array [pos];
}



/*
==============================================================================
Name : get_glyph_iterator
==============================================================================
*/

GlyphArray::iterator Typesetter::get_glyph_iterator (size_t pos)
{
   assert (pos <= _glyph_array.size ());  // can be end ()
   
   return _glyph_array.begin () + pos;
}



/*
==============================================================================
Name : erase_glyph
Description :
   Erase a glyph from the glyph array, and do all operations needed internally
   to remap positions.
   once erased the next glyph will have 'pos' position;
==============================================================================
*/

void  Typesetter::erase_glyph (size_t pos, GlyphRange & glyph_range)
{
   assert (pos < _glyph_array.size ());
   assert (pos >= glyph_range._start);
   assert (pos < glyph_range._end);
   
   Glyph & glyph = _glyph_array [pos];
   
   for (size_t i = glyph._char_index + 1 ; i < _char_array.size () ; ++i)
   {
      _char_array [i]._glyph_index -= 1;
   }
   
   RunArray::iterator it = _run_array.begin ();
   const RunArray::iterator it_end = _run_array.end ();
   
   for (; it != it_end ; ++it)
   {
      Run & run = *it;
      
      if ((run._glyph_range._start <= pos) && (pos < run._glyph_range._end))
      {
         run._glyph_range._end -= 1;
      }
      else if (pos < run._glyph_range._start)
      {
         run._glyph_range._start -= 1;
         run._glyph_range._end -= 1;
      }
   }
   
   _glyph_array.erase (_glyph_array.begin () + pos);
   glyph_range._end -= 1;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : preprocess
==============================================================================
*/

void  Typesetter::preprocess ()
{
   /*
   since cmap & bidi algo are interchangeable, we apply both
   at the same time, so that we have glyph ordered in visual order
   with map to the char index.
   */
   
   process_bidi_cmap ();

   for (size_t pos = 0 ; pos < _scripture.size () ;)
   {
      size_t next_pos = _scripture.get_next_run (pos);
      
      Scripture::Range run_range;
      run_range._start = pos;
      run_range._end = next_pos;
      
      const RunArray::iterator run_it = find_run (pos);
      assert (run_it != _run_array.end ());
      
      Run & run = *run_it;
      
      if (!run._image_flag)
      {
         process_glyph_subtitutions (run_range, run);
      }
      
      pos = next_pos;
   }  
}



/*
==============================================================================
Name : process_bidi_cmap
==============================================================================
*/

void  Typesetter::process_bidi_cmap ()
{
   /* process bidi and resolved bidi */
   
   /* make initial glyph array on resolved bidi */
   
   /* extract runs */
   
   _scripture_pos_to_char_pos_arr.resize (_scripture.size ());
   _char_array.resize (_scripture.size ());
   _glyph_array.resize (_scripture.size ());
   
   for (size_t pos = 0 ; pos < _scripture.size () ;)
   {
      size_t next_pos = _scripture.get_next_run (pos);
      
      const Font * font_ptr = _scripture.get_font (pos);
      // might be null
      
      if (font_ptr == 0)
      {
         assert (_scripture.get_image (pos) != 0);
      }
      
      Direction direction = _scripture.get_direction (pos);
         
      Run run;
      run._scripture_range._start = pos;
      run._scripture_range._end = next_pos;
      run._glyph_range._start = pos;
      run._glyph_range._end = next_pos;
      
      if (font_ptr != 0)
      {
         run._ascent = font_ptr->get_ascent ();
         run._descent = font_ptr->get_descent ();
         run._leading = font_ptr->get_leading ();
         run._image_flag = false;
      }
      else
      {
         run._ascent = 0;
         run._descent = 0;
         run._leading = 0;
         
         for (size_t i = pos ; i < next_pos ; ++i)
         {
            Image * image_ptr = _scripture.get_image (i);
            assert (image_ptr != 0);
            
            float image_height = float (image_ptr->_height);
            
            if (image_height > run._ascent)
            {
               run._ascent = image_height;
            }
         }
         
         run._image_flag = true;
      }
      
      run._width = 0.f;
      
      _run_array.push_back (run);

      for (size_t i = 0 ; i < next_pos - pos ; ++i)
      {
         if (direction)
         {
            _scripture_pos_to_char_pos_arr [pos + i] = next_pos - 1 - i;
         }
         else
         {
            _scripture_pos_to_char_pos_arr [pos + i] = pos + i;
         }
      }
      
      for (size_t i = 0 ; i < next_pos - pos ; ++i)
      {
         Char & char_elem = _char_array [pos + i];
         
         if (direction)
         {
            char_elem._c = _scripture.get_char (next_pos - 1 - i);
         }
         else
         {
            char_elem._c = _scripture.get_char (pos + i);
         }
         
         char_elem._glyph_index = pos + i;
         
         Glyph & glyph = _glyph_array [pos + i];
         
         if (font_ptr != 0)
         {
            glyph._gid = font_ptr->get_gid (char_elem._c);
            glyph._image_flag = false;
         }
         else
         {
            glyph._gid = 0;
            glyph._image_flag = true;
         }
         
         glyph._char_index = pos + i;
         
         if (direction)
         {
            glyph._scripture_index = next_pos - 1 - i;
         }
         else
         {
            glyph._scripture_index = pos + i;
         }
         
         glyph._direction = direction;
         glyph._separator_categ_class_flag
            = txt::unicode::Categ_conv_to_class (
               txt::unicode::CharData::get_categ (char_elem._c)
            ) == txt::unicode::CategClass_SEPARATOR;
         glyph._ligature_flag = false;
      }
      
      for (size_t i = 0 ; i < next_pos - pos ; ++i)
      {
         Glyph & glyph = _glyph_array [pos + i];
         
         if (i == 0)
         {
            if (glyph._direction)
            {
               glyph._final_flag = true;
            }
            else
            {
               glyph._initial_flag = true;
            }
         }
         
         if (i == next_pos - pos - 1)
         {
            if (glyph._direction)
            {
               glyph._initial_flag = true;
            }
            else
            {
               glyph._final_flag = true;
            }
         }
         
         if (i > 0)
         {
            const Glyph & prev_glyph = _glyph_array [pos + i - 1];
            
            if (prev_glyph._separator_categ_class_flag)
            {
               if (glyph._direction)
               {
                  glyph._final_flag = true;
               }
               else
               {
                  glyph._initial_flag = true;
               }
            }
         }
         
         if (i < next_pos - pos - 1)
         {
            const Glyph & next_glyph = _glyph_array [pos + i + 1];
            
            if (next_glyph._separator_categ_class_flag)
            {
               if (glyph._direction)
               {
                  glyph._initial_flag = true;
               }
               else
               {
                  glyph._final_flag = true;
               }
            }
         }
      }
      
      pos = next_pos;
   }
}



/*
==============================================================================
Name : process_glyph_subtitutions
==============================================================================
*/

void  Typesetter::process_glyph_subtitutions (const Scripture::Range & scripture_range, const Run & run)
{
   GlyphRange glyph_range;
   scripture_range_to_glyph_range (glyph_range, scripture_range, run);
   
   const Font * font_ptr = _scripture.get_font (scripture_range._start);
   assert (font_ptr != 0);
   
   font_ptr->process_substitution (*this, glyph_range);
   
   // if we remove a glyph, make char points to -1 glyph index
   // if we add a glyph, don't touche char
   // if we move a glyph, update char glyph index
}



/*
==============================================================================
Name : measure
==============================================================================
*/

void  Typesetter::measure (float & block_width, float & block_padding, float & block_ascent, float & block_descent, const Scripture::Range & scripture_range)
{
   block_width = 0.f;
   block_padding = 0.f;
   
   RunArray::iterator it = find_run (scripture_range._start);
   assert (it != _run_array.end ());
   
   size_t pos = scripture_range._start;
   
   for (; pos < scripture_range._end ;)
   {
      const Run & run = *it;
      
      size_t next_pos = lang::min (run._scripture_range._end, scripture_range._end);
      
      Scripture::Range run_scripture_range;
      run_scripture_range._start = pos;
      run_scripture_range._end = next_pos;
      
      float run_block_width = 0;
      float run_block_padding = 0;
      float run_block_ascent = 0;
      float run_block_descent = 0;
      
      measure (run_block_width, run_block_padding, run_block_ascent, run_block_descent, run_scripture_range, run);

      block_width += run_block_width + block_padding;
      block_padding = run_block_padding;
      if (run_block_ascent > block_ascent) block_ascent = run_block_ascent;
      if (run_block_descent > block_descent) block_descent = run_block_descent;
      
      pos = next_pos;
      ++it;
   }
}



/*
==============================================================================
Name : measure
==============================================================================
*/

void  Typesetter::measure (float & block_width, float & block_padding, float & block_ascent, float & block_descent, const Scripture::Range & scripture_range, const Run & run)
{
   block_width = 0.f;
   block_padding = 0.f;
   
   const Font * font_ptr = _scripture.get_font (scripture_range._start);
   
   if (font_ptr != 0)
   {
      block_ascent = font_ptr->get_ascent ();
      block_descent = font_ptr->get_descent ();
      
      GlyphRange glyph_range;
      scripture_range_to_glyph_range (glyph_range, scripture_range, run);
      
      TypescriptMeasure measure;
      
      font_ptr->measure (
         measure,
         _glyph_array, glyph_range._start, glyph_range._end
      );
      
      block_width = measure._width;
      block_padding = measure._padding;
   }
   else
   {
      block_ascent = 0.f;
      block_descent = 0.f;
      
      for (size_t i = scripture_range._start ; i < scripture_range._end ; ++i)
      {
         Image * image_ptr = _scripture.get_image (i);
         assert (image_ptr != 0);
         
         float image_height = float (image_ptr->_height);
         
         if (image_height > block_ascent)
         {
            block_ascent = image_height;
         }
         
         block_width += image_ptr->_width;
      }
   }
}



/*
==============================================================================
Name : layout_run
==============================================================================
*/

void  Typesetter::layout_run (Run & dst_run, float & x, const Scripture::Range & scripture_range, const Run & src_run)
{
   dst_run._scripture_range = scripture_range;
   
   scripture_range_to_glyph_range (dst_run._glyph_range, scripture_range, src_run);
   
   dst_run._ascent = src_run._ascent;
   dst_run._descent = src_run._descent;
   dst_run._leading = src_run._leading;
   
   const Font * font_ptr = _scripture.get_font (scripture_range._start);

   if (font_ptr != 0)
   {
      _glyph_array [dst_run._glyph_range._start]._x = x;
      
      TypescriptMeasure measure;
      
      font_ptr->process_position (
         measure,
         _glyph_array, dst_run._glyph_range._start, dst_run._glyph_range._end
      );
      
      dst_run._width = measure._width + measure._padding;
      dst_run._margin_left = measure._image_left;
      dst_run._margin_right = measure._image_right - dst_run._width;
      dst_run._image_top = measure._image_top;
      dst_run._image_bottom = measure._image_bottom;
      
      x += dst_run._width;
   }
   else
   {
      dst_run._width = 0.f;
      
      x = 0.f;
      
      size_t glyph_pos = dst_run._glyph_range._start;
      for (size_t i = scripture_range._start ; i < scripture_range._end ; ++i, ++glyph_pos)
      {
         Image * image_ptr = _scripture.get_image (i);
         assert (image_ptr != 0);
         
         float image_width = float (image_ptr->_width);
         
         _glyph_array [glyph_pos]._x = x;
         _glyph_array [glyph_pos]._width = image_width;
         
         dst_run._width += image_width;
         
         x += image_width;
      }

      dst_run._margin_left = 0;
      dst_run._margin_right = 0;
      dst_run._image_top = - dst_run._ascent;
      dst_run._image_bottom = dst_run._descent;
   }
}



/*
==============================================================================
Name : justify
==============================================================================
*/

void  Typesetter::justify ()
{
#pragma message ("to do")
}



/*
==============================================================================
Name : scripture_pos_to_char_pos
==============================================================================
*/

void  Typesetter::scripture_pos_to_char_pos (size_t & char_pos, size_t scripture_pos)
{
   assert (scripture_pos < _scripture_pos_to_char_pos_arr.size ());
   
   char_pos = _scripture_pos_to_char_pos_arr [scripture_pos];
}



/*
==============================================================================
Name : char_range_to_glyph_range
==============================================================================
*/

void  Typesetter::scripture_range_to_glyph_range (GlyphRange & glyph_range, const Scripture::Range & scripture_range, const Run & run)
{
   assert (scripture_range._start < _scripture.size ());
   assert (scripture_range._end <= _scripture.size ());

   assert (scripture_range._start >= run._scripture_range._start);
   assert (scripture_range._start < run._scripture_range._end);
   assert (scripture_range._end >= run._scripture_range._start);
   assert (scripture_range._end <= run._scripture_range._end);
   
   scripture_pos_to_glyph_pos (glyph_range._start, scripture_range._start, run);
   scripture_pos_to_glyph_pos (glyph_range._end, scripture_range._end - 1, run);
   
   if (glyph_range._start > glyph_range._end)
   {
      std::swap (glyph_range._start, glyph_range._end);
   }
   
   glyph_range._end += 1;
   
   assert (glyph_range._start >= run._glyph_range._start);
   assert (glyph_range._start < run._glyph_range._end);
   assert (glyph_range._end >= run._glyph_range._start);
   assert (glyph_range._end <= run._glyph_range._end);
}



/*
==============================================================================
Name : scripture_pos_to_glyph_pos
==============================================================================
*/

void  Typesetter::scripture_pos_to_glyph_pos (size_t & glyph_pos, size_t scripture_pos, const Run & run)
{
   assert (scripture_pos >= run._scripture_range._start);
   assert (scripture_pos <= run._scripture_range._end);
   
   size_t char_pos = 0;
   scripture_pos_to_char_pos (char_pos, scripture_pos);
   
   glyph_pos = _char_array [char_pos]._glyph_index;
}



/*
==============================================================================
Name : find_run
==============================================================================
*/

Typesetter::RunArray::iterator   Typesetter::find_run (size_t pos)
{
   RunArray::iterator it = _run_array.begin ();
   RunArray::iterator it_end = _run_array.end ();

   for (; it != it_end ; ++it)
   {
      Run & run = *it;
      
      if (
         (pos >= run._scripture_range._start)
         && (pos < run._scripture_range._end)
         )
      {
         break;
      }
   }
   
   return it;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
