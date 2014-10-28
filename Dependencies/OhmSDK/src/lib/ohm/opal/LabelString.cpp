/*****************************************************************************

        LabelString.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 50133

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

#include "ohm/opal/LabelString.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opal/def.h"
#include "ohm/txt/Err.h"
#include "ohm/txt/utf8/Codec8.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

LabelString::LabelString ()
:  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr (new Typesetter (_scripture))
,  _typesetter_need_layout_flag (true)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

LabelString::LabelString (const Scripture & scripture)
:  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr (new Typesetter (_scripture))
,  _typesetter_need_layout_flag (true)
{
   begin_edit ();
   _scripture = scripture;
   end_edit ();
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

LabelString::LabelString (const std::string & font_name, float font_size, const std::string & str, const Color & color)
:  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr ()
,  _typesetter_need_layout_flag (true)
{
   const Font & font = FontManager::use_instance ().get_font (font_name, font_size);
   
   txt::unicode::StringU ustr;
   
   to_ustr (ustr, str);
   
   _scripture.begin_edit ();
   _scripture.push_state_font (font);
   _scripture.set_state_color (color);
   _scripture.insert (0, ustr);
   _scripture.end_edit ();
   
   //#* RDI this was a cause o a nasty error
   // maybe do this automatically ?
   _typesetter_aptr = std::auto_ptr <Typesetter> (new Typesetter (_scripture));
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

LabelString::LabelString (const std::string & font_name, float font_size, const txt::unicode::StringU & ustr, const Color & color)
:  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr ()
,  _typesetter_need_layout_flag (true)
{
   const Font & font = FontManager::use_instance ().get_font (font_name, font_size);
   
   _scripture.begin_edit ();
   _scripture.push_state_font (font);
   _scripture.set_state_color (color);
   _scripture.insert (0, ustr);
   _scripture.end_edit ();
   
   _typesetter_aptr = std::auto_ptr <Typesetter> (new Typesetter (_scripture));
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

LabelString::LabelString (const Font & font, const std::string & str, const Color & color)
:  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr ()
,  _typesetter_need_layout_flag (true)
{
   assert (&font != 0);
   
   txt::unicode::StringU ustr;
   
   to_ustr (ustr, str);

   _scripture.begin_edit ();
   _scripture.push_state_font (font);
   _scripture.set_state_color (color);
   _scripture.insert (0, ustr);
   _scripture.end_edit ();
   
   _typesetter_aptr = std::auto_ptr <Typesetter> (new Typesetter (_scripture));
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

LabelString::LabelString (const Font & font, const txt::unicode::StringU & ustr, const Color & color)
:  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr ()
,  _typesetter_need_layout_flag (true)
{
   assert (&font != 0);

   _scripture.begin_edit ();
   _scripture.push_state_font (font);
   _scripture.set_state_color (color);
   _scripture.insert (0, ustr);
   _scripture.end_edit ();
   
   _typesetter_aptr = std::auto_ptr <Typesetter> (new Typesetter (_scripture));
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

LabelString::~LabelString ()
{
}



/*
==============================================================================
Name : begin_edit
==============================================================================
*/

void  LabelString::begin_edit ()
{
   assert (!_editing_flag);
   
   _editing_flag = true;
   
   _scripture.begin_edit ();
}



/*
==============================================================================
Name : end_edit
==============================================================================
*/

void  LabelString::end_edit ()
{
   assert (_editing_flag);
   
   _scripture.end_edit ();

   _typesetter_aptr = std::auto_ptr <Typesetter> (new Typesetter (_scripture));
   _typesetter_need_layout_flag = true;

   _editing_flag = false;
}



/*
==============================================================================
Name : use_scripture
==============================================================================
*/

Scripture & LabelString::use_scripture ()
{
   assert (_editing_flag);
   
   return _scripture;
}



/*
==============================================================================
Name : get_typographic_bounds
==============================================================================
*/

const opa::Rect & LabelString::get_typographic_bounds () const
{
   assert (!_editing_flag);
   
   update ();
   
   return _typographic_bounds;
}



/*
==============================================================================
Name : get_image_bounds
==============================================================================
*/

const opa::Rect & LabelString::get_image_bounds () const
{
   assert (!_editing_flag);
   
   update ();
   
   return _image_bounds;
}



/*
==============================================================================
Name : draw
==============================================================================
*/

void  LabelString::draw (GraphicContextInterface & context) const
{
   assert (!_editing_flag);
   
   update ();
   
   if (_typesetter_aptr.get () != 0)
   {
      Typesetter & typesetter (*_typesetter_aptr);
      opa::Point pt = opa::Point::_zero;
            
      Typesetter::LineInfoArray::iterator it = typesetter.begin_line ();
      Typesetter::LineInfoArray::iterator it_end = typesetter.end_line ();
      
      // start at baseline + space_before
      if (it != it_end)
      {
         pt._y += it->_space_before;
      }

      for (; it != it_end ; ++it)
      {
         typesetter.draw (context, pt, *it);

         pt._y += it->_descent;
         pt._y += it->_leading;
         pt._y += it->_space_after;

         pt._y += it->_space_before;
         pt._y += it->_ascent;
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : to_ustr
==============================================================================
*/

void  LabelString::to_ustr (txt::unicode::StringU & ustr, const std::string & utf8) const
{
   ustr.clear ();
   
   int err = txt::Err_OK;
   size_t i = 0;

   while ((err == txt::Err_OK) && (utf8 [i] != '\0'))
   {
      txt::unicode::Char   ucs4_char;
      int len = 0;

      err = txt::utf8::Codec8::decode_char (ucs4_char, &utf8 [i], len);
      
      if (err == txt::Err_OK)
      {
         i += len;
         ustr += ucs4_char;
      }
   }
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  LabelString::update () const
{
   assert (!_editing_flag);
   
   if (_typesetter_aptr.get () != 0)
   {
      Typesetter & typesetter (*_typesetter_aptr);
      
      if (_typesetter_need_layout_flag)
      {
         if (_scripture.size () > 0)
         {
            size_t pos = 0;
            Typesetter::LineInfo * last_line_info_ptr = 0;
            size_t next_pos = 0;
            float height = 0;
            float width = 1000000.f;
            typesetter.suggest_line_break (next_pos, height, pos, width);
            
            Scripture::Range range;
            range._start = pos;
            range._end = next_pos;

            const Typesetter::LineInfo * line_info_ptr
               = typesetter.layout_line (range, width, false, last_line_info_ptr);
            
            _typesetter_need_layout_flag = false;

            _typographic_bounds._origin._x = 0;
            _typographic_bounds._origin._y = - line_info_ptr->_ascent;
            _typographic_bounds._size._width = line_info_ptr->_width;
            _typographic_bounds._size._height = line_info_ptr->_ascent + line_info_ptr->_descent;
            
            _image_bounds._origin._x = line_info_ptr->_margin_left;
            _image_bounds._origin._y = line_info_ptr->_image_top;
            _image_bounds._size._width
               = line_info_ptr->_width
               + line_info_ptr->_margin_right
               - line_info_ptr->_margin_left;
            _image_bounds._size._height
               = line_info_ptr->_image_bottom
               - line_info_ptr->_image_top;
         }
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
