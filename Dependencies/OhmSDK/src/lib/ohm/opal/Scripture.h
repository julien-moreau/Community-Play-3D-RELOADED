/*****************************************************************************

        Scripture.h
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



#if ! defined (ohm_opal_Scripture_HEADER_INCLUDED)
#define  ohm_opal_Scripture_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/unicode/StringU.h"
#include "ohm/txt/unicode/Char.h"

#include "ohm/opal/Font.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/Image.h"

#include <vector>



namespace ohm
{
namespace opal
{



class Glyph;
class GraphicContextInterface;

class Scripture
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef std::vector <archi::UByte>  RawData;
   
   class Range
   {
   public:
      size_t      _start;
      size_t      _end;
   };
   
   typedef archi::UByte Direction;  // 0 = LTR, 1 = RTL

   typedef archi::UByte CharStyleType;
   
   enum CharStyle
   {
                  CharStyle_NORMAL = 0,
                  CharStyle_CARVED,
                  CharStyle_BORDERED,
   };
   
   class Run
   {
   public:
      Font *      _font_ptr;
      Direction   _direction;
   };
   
                  Scripture ();
                  Scripture (const Scripture &other);
   virtual        ~Scripture ();

   Scripture &    operator = (const Scripture &other);
   bool           operator == (const Scripture &other) const;
   bool           operator != (const Scripture &other) const;
   
   // control
   
   void           begin_edit ();
   void           end_edit ();
   
   void           read (const archi::UByte * raw_data_ptr, size_t raw_data_size);
   void           write (RawData & raw_data) const;
   void           convert_fonts ();
   
   // characters
   
   void           clear ();
   void           erase (size_t first, size_t last);
   void           insert (size_t loc, const txt::unicode::StringU & str);
   void           insert (size_t loc, txt::unicode::Char c);
   void           insert (size_t loc, const std::string & utf8_str);
   void           insert_paragraph_separator (size_t loc);
   void           insert (size_t loc, Image & image);
   void           truncate (size_t nbr);
   size_t         size () const;
   
   // attributes state
   
   void           push_state_font (const Font & font);
   void           clear_state_font ();
   const Font &   get_main_state_font () const;
   void           set_state_char_style (CharStyle char_style);
   CharStyle      get_state_char_style () const;
   void           set_state_color (const Color & color);
   const Color &  get_state_color () const;
   void           set_state_second_color (const Color & color);
   const Color &  get_state_second_color () const;
   
   // attributes plane manipulation
   
   void           set_font (size_t first, size_t last, const Font & font);
   void           set_char_style (size_t first, size_t last, CharStyle char_style);
   void           set_color (size_t first, size_t last, const Color & color);
   void           set_second_color (size_t first, size_t last, const Color & second_color);
   
   // after end_edit
   
   size_t         get_next_run (size_t start_loc) const;
   
   size_t         get_next_cluster_boundary (size_t start_loc) const;
   size_t         get_previous_cluster_boundary (size_t start_loc) const;
   size_t         get_next_font (size_t start_loc) const;
   size_t         get_next_direction (size_t start_loc) const;
   size_t         get_next_possible_line_break (size_t start_loc) const;
   size_t         get_next_explicit_break (size_t start_loc) const;
   
   // accessors
   
   txt::unicode::Char
                  get_char (size_t loc) const;
   const Font *   get_font (size_t loc) const;
   CharStyle      get_char_style (size_t loc) const;
   const Color &  get_color (size_t loc) const;
   const Color &  get_second_color (size_t loc) const;
   Image *        get_image (size_t loc) const;
   Direction      get_direction (size_t loc) const;
   
   // helpers
   std::string    get_utf8_str () const;
   
   // debug
   
   void           print_debug ();
   void           print_debug (size_t pos);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum LineBreak
   {
                  LineBreak_POSSIBLE = 0,
                  LineBreak_PROHIBITED,
                  LineBreak_EXPLICIT,
   };
   
   void           update_line_break ();
   void           update_direction ();
   
   template <class T>
   void           push (RawData & raw_data, size_t & pos, const std::vector <T> & data_arr) const
   {
      {
         size_t data_size = sizeof (size_t);
         assert (pos + data_size <= raw_data.size ());
         
         size_t data = data_arr.size ();
         memcpy (&raw_data [pos], &data, data_size);
         pos += data_size;
      }
      if (data_arr.size () > 0)
      {
         size_t data_size = data_arr.size () * sizeof (T);
         assert (pos + data_size <= raw_data.size ());
         const T & data = data_arr [0];
         memcpy (&raw_data [pos], &data, data_size);
         pos += data_size;
      }
   }

   template <class T>
   void           pop (const archi::UByte * raw_data_ptr, size_t raw_data_size, size_t & pos, std::vector <T> & data_arr)
   {
      {
         size_t data_size = sizeof (size_t);
         assert (pos + data_size <= raw_data_size);
         
         size_t data = 0;
         memcpy (&data, &raw_data_ptr [pos], data_size);
         pos += data_size;
         
         data_arr.resize (data);
      }
         
      if (data_arr.size () > 0) 
      {
         assert (pos + data_arr.size () * sizeof (T) <= raw_data_size);

         memcpy (&data_arr [0], &raw_data_ptr [pos], data_arr.size () * sizeof (T));
         pos += data_arr.size () * sizeof (T);
      }
   }
   
   std::vector <txt::unicode::Char>
                  _char_arr;
   
//#warning Scripture.h : optimize this

   std::vector <const Font *>
                  _font_ptr_arr;
   std::vector <CharStyleType>
                  _char_style_arr;
   std::vector <Color>
                  _color_arr; // maybe a Color * instead and cache the colors
   std::vector <Color>
                  _second_color_arr;   // maybe a Color * instead and cache the colors
   std::vector <Image *>
                  _image_ptr_arr;


   std::vector <Direction>
                  _direction_arr;
   std::vector <LineBreak>
                  _line_break_arr;
   
   // State
   std::vector <const Font *>
                  _cur_font_ptr_arr;
   CharStyle      _cur_char_style;
   Color          _cur_color;
   Color          _cur_second_color;
   
   static txt::unicode::Char
                  _paragraph_separator;
   
   static Color   _default_color;
   
   // multiple planes

   // characters
   // breaks
   // clusters
   // scripts

   // fonts
   // glyphs
   
   // runs (scripts & fonts)
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class Scripture



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Scripture_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
