/*****************************************************************************

        Scripture.cpp
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

#include "ohm/txt/Err.h"
#include "ohm/txt/unicode/CharData.h"
#include "ohm/txt/utf8/def.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/lang/fnc.h"

#include "ohm/opa/Constants.h"
#include "ohm/opal/Scripture.h"
#include "ohm/opal/FontManager.h"

#include <cassert>



namespace ohm
{
namespace opal
{


/*

processing :

get next block (font, direction, possible, explicit)

process block char to glyphs
process glyphs

measure block 
   2 rects : with and without rect, relative to base line
   one to see if it fits in the line
   the other to compose (much like padding in css)

   typically inline image measurement goes here

once a line is made :
we can deduce line height
we would do justification by spacing blocks uniformly


*/

// using OBJ replacement char
// property as a plane
// baseline_rel_y is a plane property

//#error font family, substitution



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Scripture::Scripture ()
:  _char_arr ()
,  _font_ptr_arr ()
,  _image_ptr_arr ()
,  _direction_arr ()
,  _line_break_arr ()

,  _cur_font_ptr_arr ()
,  _cur_char_style (CharStyle_NORMAL)
,  _cur_color (1.f, 0.f, 0.f, 0.f)
,  _cur_second_color (0.f, 0.f, 0.f, 0.f)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Scripture::Scripture (const Scripture &other)
:  _char_arr (other._char_arr)
,  _font_ptr_arr (other._font_ptr_arr)
,  _char_style_arr (other._char_style_arr)
,  _color_arr (other._color_arr)
,  _second_color_arr (other._second_color_arr)
,  _image_ptr_arr (other._image_ptr_arr)
,  _direction_arr (other._direction_arr)
,  _line_break_arr (other._line_break_arr)

,  _cur_font_ptr_arr (other._cur_font_ptr_arr)
,  _cur_char_style (other._cur_char_style)
,  _cur_color (other._cur_color)
,  _cur_second_color (other._cur_second_color)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Scripture::~Scripture ()
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
Name : operator =
==============================================================================
*/

Scripture & Scripture::operator = (const Scripture &other)
{
   _char_arr = other._char_arr;
   _font_ptr_arr = other._font_ptr_arr;
   _char_style_arr = other._char_style_arr;
   _color_arr = other._color_arr;
   _second_color_arr = other._second_color_arr;
   _image_ptr_arr = other._image_ptr_arr;
   _direction_arr = other._direction_arr;
   _line_break_arr = other._line_break_arr;

   _cur_font_ptr_arr = other._cur_font_ptr_arr;
   _cur_color = other._cur_color;
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Scripture::operator == (const Scripture &other) const
{
   bool equal_flag = true;
   
   if (equal_flag)
   {
      equal_flag &= _char_arr == other._char_arr;
   }
   
   if (equal_flag)
   {
      equal_flag &= _font_ptr_arr == other._font_ptr_arr;
   }

   if (equal_flag)
   {
      equal_flag &= _char_style_arr == other._char_style_arr;
   }

   if (equal_flag)
   {
      equal_flag &= _color_arr == other._color_arr;
   }

   if (equal_flag)
   {
      equal_flag &= _second_color_arr == other._second_color_arr;
   }

   if (equal_flag)
   {
      equal_flag &= _image_ptr_arr == other._image_ptr_arr;
   }
   
   return equal_flag;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Scripture::operator != (const Scripture &other) const
{
   return !operator == (other);
}



/*
==============================================================================
Name : begin_edit
==============================================================================
*/

void  Scripture::begin_edit ()
{
#pragma message ("had editing_flag checks")
}



/*
==============================================================================
Name : end_edit
==============================================================================
*/

void  Scripture::end_edit ()
{
   update_line_break ();
   update_direction ();
}



/*
==============================================================================
Name : read
==============================================================================
*/

void  Scripture::read (const archi::UByte * raw_data_ptr, size_t raw_data_size)
{
   size_t pos = 0;
   
   pop (raw_data_ptr, raw_data_size , pos, _char_arr);
   pop (raw_data_ptr, raw_data_size , pos, _font_ptr_arr);
   pop (raw_data_ptr, raw_data_size , pos, _char_style_arr);
   pop (raw_data_ptr, raw_data_size , pos, _color_arr);
   pop (raw_data_ptr, raw_data_size , pos, _second_color_arr);
   pop (raw_data_ptr, raw_data_size , pos, _image_ptr_arr);
   pop (raw_data_ptr, raw_data_size , pos, _direction_arr);
   pop (raw_data_ptr, raw_data_size , pos, _line_break_arr);
   pop (raw_data_ptr, raw_data_size , pos, _cur_font_ptr_arr);
   
   {
      assert (pos + sizeof (_cur_char_style) <= raw_data_size);

      memcpy (&_cur_char_style, &raw_data_ptr [pos], sizeof (_cur_char_style));
      pos += sizeof (_cur_char_style);
   }

   {
      assert (pos + sizeof (_cur_color) <= raw_data_size);

      memcpy (&_cur_color, &raw_data_ptr [pos], sizeof (_cur_color));
      pos += sizeof (_cur_color);
   }

   {
      assert (pos + sizeof (_cur_second_color) <= raw_data_size);

      memcpy (&_cur_second_color, &raw_data_ptr [pos], sizeof (_cur_second_color));
      pos += sizeof (_cur_second_color);
   }
   
   assert (pos == raw_data_size);
   
   convert_fonts ();
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  Scripture::write (RawData & raw_data) const
{
   size_t raw_data_size = 0;
   
   raw_data_size += sizeof (size_t) + _char_arr.size () * sizeof (txt::unicode::Char);
   raw_data_size += sizeof (size_t) + _font_ptr_arr.size () * sizeof (const Font *);
   raw_data_size += sizeof (size_t) + _char_style_arr.size () * sizeof (CharStyleType);
   raw_data_size += sizeof (size_t) + _color_arr.size () * sizeof (Color);
   raw_data_size += sizeof (size_t) + _second_color_arr.size () * sizeof (Color);
   raw_data_size += sizeof (size_t) + _image_ptr_arr.size () * sizeof (Image *);
   raw_data_size += sizeof (size_t) + _direction_arr.size () * sizeof (Direction);
   raw_data_size += sizeof (size_t) + _line_break_arr.size () * sizeof (LineBreak);
   raw_data_size += sizeof (size_t) + _cur_font_ptr_arr.size () * sizeof (const Font *);
   raw_data_size += sizeof (_cur_char_style);
   raw_data_size += sizeof (_cur_color);
   raw_data_size += sizeof (_cur_second_color);
   
   raw_data.resize (raw_data_size);
   
   size_t pos = 0;
   
   push (raw_data, pos, _char_arr);
   push (raw_data, pos, _font_ptr_arr);
   push (raw_data, pos, _char_style_arr);
   push (raw_data, pos, _color_arr);
   push (raw_data, pos, _second_color_arr);
   push (raw_data, pos, _image_ptr_arr);
   push (raw_data, pos, _direction_arr);
   push (raw_data, pos, _line_break_arr);
   push (raw_data, pos, _cur_font_ptr_arr);
   
   {
      size_t data_size = sizeof (_cur_char_style);
      assert (pos + data_size <= raw_data.size ());
      
      memcpy (&raw_data [pos], &_cur_char_style, data_size);
      pos += data_size;
   }

   {
      size_t data_size = sizeof (_cur_color);
      assert (pos + data_size <= raw_data.size ());
      
      memcpy (&raw_data [pos], &_cur_color, data_size);
      pos += data_size;
   }

   {
      size_t data_size = sizeof (_cur_second_color);
      assert (pos + data_size <= raw_data.size ());
      
      memcpy (&raw_data [pos], &_cur_second_color, data_size);
      pos += data_size;
   }
   
   assert (pos == raw_data_size);
}



/*
==============================================================================
Name : convert_fonts
Description :
   Convert scripture fonts from main thread to particular graphic context.
==============================================================================
*/

void  Scripture::convert_fonts ()
{
   const size_t size = _font_ptr_arr.size ();
   
   const Font * old_font_ptr = 0;
   const Font * new_font_ptr = 0;
   
   FontManager & fm = FontManager::use_instance ();
   
   for (size_t i = 0 ; i < size ; ++i)
   {
      const Font * & cur_font_ptr = _font_ptr_arr [i];
      assert (cur_font_ptr != 0);
      
      if (cur_font_ptr == old_font_ptr)
      {
         // optimization
         cur_font_ptr = new_font_ptr;
      }
      else
      {
         old_font_ptr = cur_font_ptr;
         new_font_ptr = &fm.get_font_for_thread (*cur_font_ptr);
         cur_font_ptr = new_font_ptr;
      }
   }
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  Scripture::clear ()
{
   erase (0, size ());
}



/*
==============================================================================
Name : erase
==============================================================================
*/

void  Scripture::erase (size_t first, size_t last)
{
   assert (first <= last);
   assert (first <= size ());
   assert (last <= size ());
   
   if (first < last)
   {
      _char_arr.erase (_char_arr.begin () + first, _char_arr.begin () + last);
      _font_ptr_arr.erase (_font_ptr_arr.begin () + first, _font_ptr_arr.begin () + last);
      _char_style_arr.erase (_char_style_arr.begin () + first, _char_style_arr.begin () + last);
      _color_arr.erase (_color_arr.begin () + first, _color_arr.begin () + last);
      _second_color_arr.erase (_second_color_arr.begin () + first, _second_color_arr.begin () + last);
      _image_ptr_arr.erase (_image_ptr_arr.begin () + first, _image_ptr_arr.begin () + last);
   }
}



/*
==============================================================================
Name : insert
==============================================================================
*/

void  Scripture::insert (size_t loc, const txt::unicode::StringU & str)
{
   assert (loc <= size ());
   
   {
      std::vector <txt::unicode::Char>::iterator it = _char_arr.begin ();
      it += loc;
      
      _char_arr.insert (it, str.begin (), str.end ());
   }
   
   {
      std::vector <const Font *> sub_font_ptr_arr;
      sub_font_ptr_arr.resize (str.size ());
      
      for (size_t j = 0 ; j < str.size () ; ++j)
      {
         txt::unicode::Char c = str [j];
         
         const Font * font_ptr = 0;
         
         {
            for (size_t i = 0 ; i < _cur_font_ptr_arr.size () ; ++i)
            {
               const Font * cur_font_ptr = _cur_font_ptr_arr [i];
               
               archi::UInt32 gid = cur_font_ptr->get_gid (c);
               
               if (gid != 0)
               {
                  font_ptr = cur_font_ptr;
                  break;
               }
            }
         }
         
         // we set a default font
         if (font_ptr == 0)
         {
            if (!_cur_font_ptr_arr.empty ())
            {
               font_ptr = _cur_font_ptr_arr [0];
            }
            else
            {
               font_ptr = &FontManager::use_instance ().get_font (opa::Constants::_default_font_0, 12.f);
            }
         }
         
         sub_font_ptr_arr [j] = font_ptr;
      }
      
      std::vector <const Font *>::iterator it = _font_ptr_arr.begin ();
      it += loc;
      
      _font_ptr_arr.insert (it, sub_font_ptr_arr.begin (), sub_font_ptr_arr.end ());
   }
   
   {
      std::vector <Image *>::iterator it = _image_ptr_arr.begin ();
      it += loc;
      
      _image_ptr_arr.insert (it, str.size (), 0);
   }
   
   {
      std::vector <CharStyleType>::iterator it = _char_style_arr.begin ();
      it += loc;
      
      _char_style_arr.insert (it, str.size (), _cur_char_style);
   }

   {
      std::vector <Color>::iterator it = _color_arr.begin ();
      it += loc;
      
      _color_arr.insert (it, str.size (), _cur_color);
   }

   {
      std::vector <Color>::iterator it = _second_color_arr.begin ();
      it += loc;
      
      _second_color_arr.insert (it, str.size (), _cur_second_color);
   }
}



/*
==============================================================================
Name : insert
==============================================================================
*/

void  Scripture::insert (size_t loc, txt::unicode::Char c)
{
   txt::unicode::StringU str;
   str.push_back (c);
   
   insert (loc, str);
}



/*
==============================================================================
Name : insert
==============================================================================
*/

void  Scripture::insert (size_t loc, const std::string & utf8_str)
{
   txt::unicode::StringU ustr;
   
   int err = txt::Err_OK;
   size_t i = 0;

   while ((err == txt::Err_OK) && (utf8_str [i] != '\0'))
   {
      txt::unicode::Char   ucs4_char;
      int len = 0;

      err = txt::utf8::Codec8::decode_char (ucs4_char, &utf8_str [i], len);
      
      if (err == txt::Err_OK)
      {
         i += len;
         ustr += ucs4_char;
      }
   }
   
   insert (loc, ustr);
}



/*
==============================================================================
Name : insert_paragraph_separator
==============================================================================
*/

void  Scripture::insert_paragraph_separator (size_t loc)
{
   txt::unicode::StringU str (1, _paragraph_separator);
   
   insert (loc, str);
}



/*
==============================================================================
Name : insert
==============================================================================
*/

void  Scripture::insert (size_t loc, Image & image)
{
   assert (loc <= size ());
   
   {
      std::vector <txt::unicode::Char>::iterator it = _char_arr.begin ();
      it += loc;
      
      // insert OBJ REPLACEMENT CHAR
      _char_arr.insert (it, txt::unicode::Char (0xFFFC));
   }
   
   {
      std::vector <const Font *>::iterator it = _font_ptr_arr.begin ();
      it += loc;
      
      _font_ptr_arr.insert (it, 0);
   }
   
   {
      std::vector <Image *>::iterator it = _image_ptr_arr.begin ();
      it += loc;
      
      _image_ptr_arr.insert (it, &image);
   }
}



/*
==============================================================================
Name : truncate
==============================================================================
*/

void  Scripture::truncate (size_t nbr)
{
   if (nbr < size ())
   {
      erase (nbr, size ());
   }
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   Scripture::size () const
{
   return _char_arr.size ();
}



/*
==============================================================================
Name : push_state_font
==============================================================================
*/

void  Scripture::push_state_font (const Font & font)
{
   assert (&font != 0);
   
   _cur_font_ptr_arr.push_back (&font);
}



/*
==============================================================================
Name : clear_state_font
==============================================================================
*/

void  Scripture::clear_state_font ()
{
   _cur_font_ptr_arr.clear ();
}



/*
==============================================================================
Name : get_main_state_font
Description :
   Return first font of the family
==============================================================================
*/

const Font &   Scripture::get_main_state_font () const
{
   if (!_cur_font_ptr_arr.empty ())
   {
      return *_cur_font_ptr_arr.front ();
   }
   else
   {
      return FontManager::use_instance ().get_font (opa::Constants::_default_font_0, 12.f);
   }
}



/*
==============================================================================
Name : set_state_char_style
==============================================================================
*/

void  Scripture::set_state_char_style (CharStyle char_style)
{
   _cur_char_style = char_style;
}



/*
==============================================================================
Name : get_state_char_style
==============================================================================
*/

Scripture::CharStyle Scripture::get_state_char_style () const
{
   return _cur_char_style;
}



/*
==============================================================================
Name : set_state_color
==============================================================================
*/

void  Scripture::set_state_color (const Color & color)
{
   _cur_color = color;
}



/*
==============================================================================
Name : get_state_color
==============================================================================
*/

const Color &  Scripture::get_state_color () const
{
   return _cur_color;
}



/*
==============================================================================
Name : set_state_second_color
==============================================================================
*/

void  Scripture::set_state_second_color (const Color & color)
{
   _cur_second_color = color;
}



/*
==============================================================================
Name : get_state_second_color
==============================================================================
*/

const Color &  Scripture::get_state_second_color () const
{
   return _cur_second_color;
}



/*
==============================================================================
Name : set_font
==============================================================================
*/

void  Scripture::set_font (size_t first, size_t last, const Font & font)
{
   _font_ptr_arr.resize (last, 0);
   
   for (size_t i = first ; i < last ; ++i)
   {
      _font_ptr_arr [i] = &font;
   }
}



/*
==============================================================================
Name : set_char_style
==============================================================================
*/

void  Scripture::set_char_style (size_t first, size_t last, CharStyle char_style)
{
   _char_style_arr.resize (last, 0);
   
   for (size_t i = first ; i < last ; ++i)
   {
      _char_style_arr [i] = char_style;
   }
}



/*
==============================================================================
Name : set_color
==============================================================================
*/

void  Scripture::set_color (size_t first, size_t last, const Color & color)
{
   _color_arr.resize (last, color);
   
   for (size_t i = first ; i < last ; ++i)
   {
      _color_arr [i] = color;
   }
}



/*
==============================================================================
Name : set_second_color
==============================================================================
*/

void  Scripture::set_second_color (size_t first, size_t last, const Color & color)
{
   _second_color_arr.resize (last, color);
   
   for (size_t i = first ; i < last ; ++i)
   {
      _second_color_arr [i] = color;
   }
}



/*
==============================================================================
Name : get_next_run
==============================================================================
*/

size_t   Scripture::get_next_run (size_t start_loc) const
{
   // first change in a font, direction, possible line break (incl. explicit)

   assert (start_loc < size ());
   assert (size () == _direction_arr.size ());
   assert (size () == _line_break_arr.size ());
   
   size_t font_change_loc = get_next_font (start_loc);
   size_t direction_change_loc = get_next_direction (start_loc);
   size_t break_loc = get_next_explicit_break (start_loc);
   
   return lang::min (
      lang::min (font_change_loc, size ()),
      lang::min (direction_change_loc, break_loc)
   );
}



/*
==============================================================================
Name : get_next_cluster_boundary
==============================================================================
*/

size_t   Scripture::get_next_cluster_boundary (size_t start_loc) const
{
   if (start_loc >= size ())
   {
      return size ();
   }
   
   txt::unicode::Char prev_c = _char_arr [start_loc];
   ++start_loc;
   
   for (; start_loc <= size () ; ++start_loc)
   {
      if (start_loc < size ())
      {
         txt::unicode::Char next_c = _char_arr [start_loc];
         
         bool boundary_flag = txt::unicode::CharData::is_grapheme_cluster_boundary (
            prev_c, next_c
         );
         
         if (boundary_flag)
         {
            break;
         }

         prev_c = next_c;
      }
      else
      {
         // end of text is a cluster boundary
         break;
      }
   }
   
   return start_loc;
}



/*
==============================================================================
Name : get_previous_cluster_boundary
==============================================================================
*/

size_t   Scripture::get_previous_cluster_boundary (size_t start_loc) const
{
   if (start_loc == 0)
   {
      return 0;
   }
   
   --start_loc;
   txt::unicode::Char prev_c = _char_arr [start_loc];
   
   while (true)
   {
      if (start_loc < size ())
      {
         if (start_loc + 1 < _char_arr.size ())
         {
            txt::unicode::Char next_c = _char_arr [start_loc + 1];
            
            bool boundary_flag = txt::unicode::CharData::is_grapheme_cluster_boundary (
               prev_c, next_c
            );
            
            if (boundary_flag)
            {
               break;
            }

            prev_c = next_c;
         }
         else
         {
            // end of text is cluster boundary
            break;
         }
      }
      else
      {
         // end of text is a cluster boundary
         break;
      }
      
      if (start_loc == 0)
      {
         break;
      }
      
      --start_loc;
   }
   
   return start_loc;
}



/*
==============================================================================
Name : get_next_font
==============================================================================
*/

size_t   Scripture::get_next_font (size_t start_loc) const
{
   assert (start_loc < size ());
   
   const Font * prev_font_ptr = get_font (start_loc);
   
   ++start_loc;
   for (; start_loc <= size () ; ++start_loc)
   {
      if (start_loc < size ())
      {
         const Font * next_font_ptr = get_font (start_loc);
         
         if (next_font_ptr != prev_font_ptr)
         {
            break;
         }
      }
      else
      {
         // end of text is a font boundary
         break;
      }
   }
   
   return start_loc;
}



/*
==============================================================================
Name : get_next_direction
==============================================================================
*/

size_t   Scripture::get_next_direction (size_t start_loc) const
{
   assert (start_loc < size ());
   assert (size () == _direction_arr.size ());
   
   Direction prev_direction = _direction_arr [start_loc];
   
   ++start_loc;
   for (; start_loc <= size () ; ++start_loc)
   {
      if (start_loc < size ())
      {
         Direction next_direction = _direction_arr [start_loc];
         
         if (next_direction != prev_direction)
         {
            break;
         }
      }
      else
      {
         // end of text is a font boundary
         break;
      }
   }
   
   return start_loc;
}



/*
==============================================================================
Name : get_next_possible_line_break
==============================================================================
*/

size_t   Scripture::get_next_possible_line_break (size_t start_loc) const
{
   assert (start_loc < size ());
   assert (size () == _line_break_arr.size ());
   
   ++start_loc;
   for (; start_loc <= size () ; ++start_loc)
   {
      if (start_loc < size ())
      {
         // line break is shifter on the right
         LineBreak line_break = _line_break_arr [start_loc - 1];
         
         if (
            (line_break == LineBreak_EXPLICIT)
            || (line_break == LineBreak_POSSIBLE)
            )
         {
            break;
         }
      }
      else
      {
         // end of text is a font boundary
         break;
      }
   }
   
   return start_loc;
}



/*
==============================================================================
Name : get_next_explicit_break
==============================================================================
*/

size_t   Scripture::get_next_explicit_break (size_t start_loc) const
{
   assert (start_loc < size ());
   assert (size () == _line_break_arr.size ());
   
   ++start_loc;
   for (; start_loc <= size () ; ++start_loc)
   {
      if (start_loc < size ())
      {
         LineBreak line_break = _line_break_arr [start_loc - 1];
         
         if (line_break == LineBreak_EXPLICIT)
         {
            break;
         }
      }
      else
      {
         // end of text is a font boundary
         break;
      }
   }
   
   return start_loc;
}



/*
==============================================================================
Name : get_char
==============================================================================
*/

txt::unicode::Char   Scripture::get_char (size_t loc) const
{
   assert (loc < size ());
   
   return _char_arr [loc];
}



/*
==============================================================================
Name : get_font
==============================================================================
*/

const Font *   Scripture::get_font (size_t loc) const
{
   assert (loc < size ());
   assert (loc < _font_ptr_arr.size ());
   
   const Font * font_ptr = 0;
   
   if (loc < _font_ptr_arr.size ())
   {
      font_ptr = _font_ptr_arr [loc];
   }
   else
   {
      font_ptr = 0;
   }

   // maybe null
   return font_ptr;
}



/*
==============================================================================
Name : get_char_style
==============================================================================
*/

Scripture::CharStyle Scripture::get_char_style (size_t loc) const
{
   assert (loc < size ());
   
   if (loc < _char_style_arr.size ())
   {
      return static_cast <CharStyle> (_char_style_arr [loc]);
   }
   else
   {
      return CharStyle_NORMAL;
   }
}



/*
==============================================================================
Name : get_color
==============================================================================
*/

const Color &  Scripture::get_color (size_t loc) const
{
   assert (loc < size ());
   
   if (loc < _color_arr.size ())
   {
      return _color_arr [loc];
   }
   else
   {
      return _default_color;
   }
}



/*
==============================================================================
Name : get_second_color
==============================================================================
*/

const Color &  Scripture::get_second_color (size_t loc) const
{
   assert (loc < size ());
   
   if (loc < _color_arr.size ())
   {
      return _second_color_arr [loc];
   }
   else
   {
      return _default_color;
   }
}



/*
==============================================================================
Name : get_image
==============================================================================
*/

Image *  Scripture::get_image (size_t loc) const
{
   assert (loc < size ());
   
   if (loc < _image_ptr_arr.size ())
   {
      Image * image_ptr = _image_ptr_arr [loc];
      assert (image_ptr != 0);
      
      return image_ptr;
   }
   else
   {
      return 0;
   }
}



/*
==============================================================================
Name : get_direction
==============================================================================
*/

Scripture::Direction Scripture::get_direction (size_t loc) const
{
   assert (loc < size ());
   assert (size () == _direction_arr.size ());

   return _direction_arr [loc];
}



/*
==============================================================================
Name : get_utf8_str
Description :
   Returns the whole scripture converted in utf8
==============================================================================
*/

std::string Scripture::get_utf8_str () const
{
   std::string ret_str;
   
   char utf8_0 [txt::utf8::MAX_BYTE_SEQ_LEN + 1];
   
   size_t len = size ();
   size_t pos = 0;
   
   for ( ; pos < len ; ++pos)
   {
      const txt::unicode::Char uc = get_char (pos);
      
      if (txt::utf8::Codec8::encode_char (utf8_0, uc) == 0)
      {
         ret_str += utf8_0;
      }
   }
   
   return (ret_str);
}



/*
==============================================================================
Name : print_debug
==============================================================================
*/

void  Scripture::print_debug ()
{
   for (size_t i = 0 ; i < size () ; ++i)
   {
      print_debug (i);
   }
}



/*
==============================================================================
Name : print_debug
==============================================================================
*/

void  Scripture::print_debug (size_t pos)
{
   assert (pos < size ());
   
   txt::unicode::Char c = _char_arr [pos];
   
   using namespace ohm;
   
   char utf8_0 [32];
   
   // LTR override
   utf8_0 [0] = char (0xE2);
   utf8_0 [1] = char (0x80);
   utf8_0 [2] = char (0xAD);
   
   printf ("%4ld ", pos);
   
   txt::utf8::Codec8::encode_char (&utf8_0 [3], c);
   
   bool cluster_flag = get_next_cluster_boundary (pos) == pos + 1;
   
   printf ("%s ", utf8_0);
   if (cluster_flag)
   {
      printf ("_");
   }
   
   printf (" ");
   
   LineBreak line_break = _line_break_arr [pos];
   
   switch (line_break)
   {
   case LineBreak_POSSIBLE:
      printf ("-");
      break;

   case LineBreak_PROHIBITED:
      printf ("|");
      break;

   case LineBreak_EXPLICIT:
      printf ("!");
      break;
   }
   
   printf (" ");
   
   if (_direction_arr [pos] == 0)
   {
      printf ("L");
   }
   else if (_direction_arr [pos] == 1)
   {
      printf ("R");
   }
   else
   {
      assert (false);
   }
   
   printf ("\n");
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_line_break
==============================================================================
*/

void  Scripture::update_line_break ()
{
   std::vector <txt::unicode::LineBreakProp> line_break_prop_arr;

   for (size_t i = 0 ; i < size () ; ++i)
   {
      txt::unicode::Char c = _char_arr [i];
      
      txt::unicode::LineBreakProp line_break_prop
         = txt::unicode::CharData::get_line_break_prop (c);
      
      // reclassification
      
      if ((line_break_prop == txt::unicode::LineBreakProp_XX)
         || (line_break_prop == txt::unicode::LineBreakProp_AI))
      {
         line_break_prop = txt::unicode::LineBreakProp_AL;
      }
      
      if (line_break_prop == txt::unicode::LineBreakProp_CB)
      {
         line_break_prop = txt::unicode::LineBreakProp_B2;
      }
      
      line_break_prop_arr.push_back (line_break_prop);
   }
   
   std::vector <txt::unicode::LineBreakAction> line_break_action_arr;
   
   const size_t      lbp_sz = line_break_prop_arr.size ();
   line_break_action_arr.resize (lbp_sz);
   for (size_t pos = 0; pos < lbp_sz; )
   {
      pos += txt::unicode::CharData::find_line_breaks (
         &line_break_action_arr [pos],
         &line_break_prop_arr [pos],
         line_break_prop_arr.size () - pos
      );
   }
   
   _line_break_arr.resize (size ());

   for (size_t i = 0 ; i < size () ; ++i)
   {
      txt::unicode::LineBreakAction line_break_action = line_break_action_arr [i];

      switch (line_break_action)
      {
      case txt::unicode::LineBreakAction_DIRECT:
      case txt::unicode::LineBreakAction_INDIRECT:
      case txt::unicode::LineBreakAction_COMBINING_INDIRECT:
         _line_break_arr [i] = LineBreak_POSSIBLE;
         break;

      case txt::unicode::LineBreakAction_COMBINING_PROHIBITED:
      case txt::unicode::LineBreakAction_PROHIBITED:
         _line_break_arr [i] = LineBreak_PROHIBITED;
         break;

      case txt::unicode::LineBreakAction_EXPLICIT:
         _line_break_arr [i] = LineBreak_EXPLICIT;
         break;
      
      default:
         assert (false);
         break;
      }
   }
}



/*
==============================================================================
Name : update_direction
==============================================================================
*/

void  Scripture::update_direction ()
{
   // fake implementation of bidi algorithm
   // we only consider paragraph level
   // we don't process embedding or override
   
   _direction_arr.resize (size ());
   
   int paragraph_level = -1;
   
   // first pass mark known directions
   
   for (size_t i = 0 ; i < size () ; ++i)
   {
      txt::unicode::Char c = _char_arr [i];
      
      txt::unicode::BidiClass bidi_class
         = txt::unicode::CharData::get_bidi_class (c);
      
      switch (bidi_class)
      {
      case txt::unicode::BidiClass_L:
      case txt::unicode::BidiClass_LRE:
      case txt::unicode::BidiClass_LRO:
      case txt::unicode::BidiClass_EN:
      case txt::unicode::BidiClass_ES:
      case txt::unicode::BidiClass_ET:
         _direction_arr [i] = 0;
         break;

      case txt::unicode::BidiClass_R:
      case txt::unicode::BidiClass_AL:
      case txt::unicode::BidiClass_RLE:
      case txt::unicode::BidiClass_RLO:
      case txt::unicode::BidiClass_AN:
         _direction_arr [i] = 1;
         break;

      default:
         // nothing
         // we do that in second pass
         break;
      }
      
   }
   
   // second pass resolve unknown directions
   
   for (size_t i = 0 ; i < size () ; ++i)
   {
      if (paragraph_level == -1)
      {
         paragraph_level = 0;
         
         for (size_t j = i ; j < size () ; ++j)
         {
            txt::unicode::Char c = _char_arr [j];
            
            txt::unicode::BidiClass bidi_class
               = txt::unicode::CharData::get_bidi_class (c);
            
            if (bidi_class == txt::unicode::BidiClass_L)
            {
               paragraph_level = 0;
               break;
            }
            else if (
               (bidi_class == txt::unicode::BidiClass_R)
               || (bidi_class == txt::unicode::BidiClass_AL)
               )
            {
               paragraph_level = 1;
               break;
            }
         }
      }
      
      txt::unicode::Char c = _char_arr [i];
      
      txt::unicode::BidiClass bidi_class
         = txt::unicode::CharData::get_bidi_class (c);
      
      switch (bidi_class)
      {
      case txt::unicode::BidiClass_L:
      case txt::unicode::BidiClass_LRE:
      case txt::unicode::BidiClass_LRO:
      case txt::unicode::BidiClass_EN:
      case txt::unicode::BidiClass_ES:
      case txt::unicode::BidiClass_ET:
      case txt::unicode::BidiClass_R:
      case txt::unicode::BidiClass_AL:
      case txt::unicode::BidiClass_RLE:
      case txt::unicode::BidiClass_RLO:
      case txt::unicode::BidiClass_AN:
         // nothing (already marked)
         break;

      case txt::unicode::BidiClass_PDF:
         _direction_arr [i] = paragraph_level;
         break;

      case txt::unicode::BidiClass_NSM:
      case txt::unicode::BidiClass_CS:
         if (i > 0)
         {
            _direction_arr [i] = _direction_arr [i - 1];
         }
         else
         {
            _direction_arr [i] = paragraph_level;
         }
         break;

      case txt::unicode::BidiClass_B:
      case txt::unicode::BidiClass_S:
         _direction_arr [i] = paragraph_level;
         paragraph_level = -1;
         break;

      case txt::unicode::BidiClass_WS:
      case txt::unicode::BidiClass_BN:
      case txt::unicode::BidiClass_ON:
         {
            bool ok_flag = (i > 0) && (i + 1 < _direction_arr.size ());
            
            if (ok_flag)
            {
               if (_direction_arr [i - 1] == _direction_arr [i + 1])
               {
                  _direction_arr [i] = _direction_arr [i - 1];
               }
               else
               {
                  _direction_arr [i] = paragraph_level;
               }
            }
            else
            {
               _direction_arr [i] = paragraph_level;
            }
         }
         break;
      
      default:
         assert (false);
         break;
      }
      
   }
}



txt::unicode::Char   Scripture::_paragraph_separator = 0x2029;
Color Scripture::_default_color (1.f, 0.f, 0.f, 0.f);



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
