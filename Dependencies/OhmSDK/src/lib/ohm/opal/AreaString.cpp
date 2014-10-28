/*****************************************************************************

        AreaString.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58260

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

#include "ohm/opal/AreaString.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/def.h"
#include "ohm/txt/Err.h"
#include "ohm/txt/utf8/Codec8.h"
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
Description :
==============================================================================
*/

AreaString::AreaString ()
:  _size ()
,  _auto_height_flag (false)

,  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr (new Typesetter (_scripture))
,  _typesetter_need_layout_flag (true)

,  _image_bounds ()
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

AreaString::AreaString (const Scripture & scripture)
:  _size ()
,  _auto_height_flag (false)

,  _scripture ()
,  _editing_flag (false)

,  _typesetter_aptr (new Typesetter (_scripture))
,  _typesetter_need_layout_flag (true)

,  _image_bounds ()
{
   begin_edit ();
   _scripture = scripture;
   end_edit ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

AreaString::~AreaString ()
{
}



/*
==============================================================================
Name : set_bounds
==============================================================================
*/

void  AreaString::set_bounds (const opa::Size & size)
{
   opa::Size tsize = size;
   lang::max_in_place (tsize._width, 0.f);
   lang::max_in_place (tsize._height, 0.f);
   
   if (_size != tsize)
   {
      _size = tsize;
      _typesetter_need_layout_flag = true;
   }
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

const opa::Size & AreaString::get_bounds () const
{
   return _size;
}



/*
==============================================================================
Name : enable_auto_height
==============================================================================
*/

void  AreaString::enable_auto_height (bool auto_height_flag)
{
   if (_auto_height_flag != auto_height_flag)
   {
      _auto_height_flag = auto_height_flag;
      _typesetter_need_layout_flag = true;
   }
}



/*
==============================================================================
Name : begin_edit
==============================================================================
*/

void  AreaString::begin_edit ()
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

void  AreaString::end_edit ()
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

Scripture & AreaString::use_scripture ()
{
   return _scripture;
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float AreaString::get_height ()
{
   assert (!_editing_flag);
   assert (_auto_height_flag);
   
   update ();
   
   return _size._height;
}



/*
==============================================================================
Name : get_image_bounds
==============================================================================
*/

const opa::Rect & AreaString::get_image_bounds () const
{
   assert (!_editing_flag);
   
   update ();
   
   return _image_bounds;
}



/*
==============================================================================
Name : draw_selection
==============================================================================
*/

void  AreaString::draw_selection (GraphicContextInterface & context, size_t first_pos, size_t second_pos, const opal::Color & color) const
{
   context.save_graphic_state ();
   
   context.set_fill_color (color);
   
   assert (!_editing_flag);
   
   if (first_pos != second_pos)
   {
      if (first_pos > second_pos)
      {
         std::swap (first_pos, second_pos);
      }
      
      update ();
      
      if (_typesetter_aptr.get () != 0)
      {
         Typesetter & typesetter (*_typesetter_aptr);
         opa::Point pt = opa::Point::_zero;
               
         Typesetter::LineInfoArray::iterator it = typesetter.begin_line ();
         Typesetter::LineInfoArray::iterator it_end = typesetter.end_line ();
         
         for (; it != it_end ; ++it)
         {
            pt._y += it->_space_before;
            pt._y += it->_ascent;
            
            size_t cur_first_pos = first_pos;
            size_t cur_second_pos = second_pos;
            
            ohm::lang::max_in_place (cur_first_pos, it->_scripture_range._start);
            ohm::lang::min_in_place (cur_second_pos, it->_scripture_range._end);
            
            for ( ; cur_first_pos < cur_second_pos ;)
            {
               // TODO: to rework
               // problem : pos is a position between glyph
               // we need to address a glyph with pos, and
               // to select left and right of glyph
               
               opa::Point location;
               float width;
               typesetter.get_selection_location (
                  location, width, cur_first_pos, opa::Point::_zero
               );
               
               opa::Rect rect (
                  location._x,
                  pt._y - it->_ascent,
                  width,
                  it->_ascent + it->_descent + it->_leading
               );
               
               context.fill (rect);
               
               cur_first_pos = _scripture.get_next_cluster_boundary (cur_first_pos);
            }

            pt._y += it->_descent;
            pt._y += it->_leading;
            pt._y += it->_space_after;
         }
      }
   }

   context.restore_graphic_state ();
}



/*
==============================================================================
Name : draw_text
==============================================================================
*/

bool  AreaString::draw_text (GraphicContextInterface & context) const
{
   assert (!_editing_flag);
   
   update ();
   
   if (_typesetter_aptr.get () != 0)
   {
      Typesetter & typesetter (*_typesetter_aptr);
      opa::Point pt = opa::Point::_zero;
            
      Typesetter::LineInfoArray::iterator it = typesetter.begin_line ();
      Typesetter::LineInfoArray::iterator it_end = typesetter.end_line ();
      
      for (; it != it_end ; ++it)
      {
         pt._y += it->_space_before;
         pt._y += it->_ascent;
         
         typesetter.draw (context, pt, *it);

         pt._y += it->_descent;
         pt._y += it->_leading;
         pt._y += it->_space_after;
      }
   }
   
   return _text_cropped_flag;
}



/*
==============================================================================
Name : draw_caret
==============================================================================
*/

void  AreaString::draw_caret (GraphicContextInterface & context, size_t first_pos, size_t second_pos, const opal::Color & color) const
{
   assert (!_editing_flag);
   
   update ();
   
   if (first_pos == second_pos)
   {
      if (_typesetter_aptr.get () != 0)
      {
         Typesetter & typesetter (*_typesetter_aptr);
         
         typesetter.draw_caret (context, first_pos, opa::Point::_zero, color);
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  AreaString::update () const
{
   if (_typesetter_aptr.get () != 0)
   {
      Typesetter & typesetter (*_typesetter_aptr);
      
      if (_typesetter_need_layout_flag)
      {
         float total_height = 0.f;
         _text_cropped_flag = false;
         
         Typesetter::LineInfo * last_line_info_ptr = 0;
         float y = 0.f;
         
         for (size_t pos = 0 ; pos < _scripture.size () ; )
         {
            size_t next_pos = 0;
            float height = 0;
            float width = _size._width;
            typesetter.suggest_line_break (next_pos, height, pos, width);
            
            total_height += height;
            
            if (!_auto_height_flag)
            {
               if (total_height >= _size._height)
               {
                  _text_cropped_flag = true;
                  break;
               }
            }
            
            Scripture::Range range;
            range._start = pos;
            range._end = next_pos;
            
            // TODO: handle justification from the scripture plane properties
            bool justify_flag = false;

            const Typesetter::LineInfo * line_info_ptr = typesetter.layout_line (
               range, width, justify_flag, last_line_info_ptr
            );
            assert (line_info_ptr != 0);
            
            y += line_info_ptr->_space_before;
            y += line_info_ptr->_ascent;
            
            lang::min_in_place (_image_bounds._origin._x, line_info_ptr->_margin_left);
            lang::min_in_place (_image_bounds._origin._y, y + line_info_ptr->_image_top);
            lang::max_in_place (
               _image_bounds._size._width,
               line_info_ptr->_width
                  + line_info_ptr->_margin_right
                  - line_info_ptr->_margin_left
            );
            lang::max_in_place (
               _image_bounds._size._height,
               y + line_info_ptr->_image_bottom - _image_bounds._origin._y
            );
            
            y += line_info_ptr->_descent;
            y += line_info_ptr->_leading;
            y += line_info_ptr->_space_after;
            
            pos = next_pos;
         }
         
         if (_auto_height_flag)
         {
            _size._height = total_height;
         }
         
         _typesetter_need_layout_flag = false;
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
