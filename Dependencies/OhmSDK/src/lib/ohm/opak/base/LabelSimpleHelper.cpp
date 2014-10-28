/*****************************************************************************

        LabelSimpleHelper.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66928

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

#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opak/base/GraphicContext.h"
#include "ohm/opak/base/LabelSimpleHelper.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   'enable_ellipsis_flag' == true : display an ellipsis if the full text
   cannot be displayed
==============================================================================
*/

LabelSimpleHelper::LabelSimpleHelper (const opal::Font & font, bool enable_ellipsis_flag)
:  _font (font)
,  _enable_ellipsis_flag (enable_ellipsis_flag)
,  _available_width (0.f)
,  _cur_width (0.f)
,  _typesetter_aptr ()
,  _line_info_ptr (0)
,  _need_update_flag (false)
{
   _scripture.push_state_font (font);
   
   _typesetter_aptr.reset (
      new opal::Typesetter (_scripture)
   );
   
   if (_enable_ellipsis_flag)
   {
      _ellipsis_gid = font.get_gid (0x2026);
      _ellipsis_width = font.measure (_ellipsis_gid);
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

LabelSimpleHelper::~LabelSimpleHelper ()
{
}



/*
==============================================================================
Name : get_font
==============================================================================
*/

const opal::Font &   LabelSimpleHelper::get_font () const
{
   return _font;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  LabelSimpleHelper::set (const std::string & str)
{
   if (_str != str)
   {
      if (str.empty ())
      {
         _line_info_ptr = 0;
         _cur_width = 0.f;
         _need_update_flag = false;
      }
      else
      {
         _scripture.begin_edit ();
         _scripture.clear ();
         _scripture.insert (0, str);
         _scripture.end_edit ();
         
         _typesetter_aptr.reset ();
         _line_info_ptr = 0;
         _need_update_flag = true;
      }
      
      _str = str;
   }
}



/*
==============================================================================
Name : get_full_width
Description :
   Returns the with of the '_str' irrespective of any available width
==============================================================================
*/

float LabelSimpleHelper::get_full_width ()
{
   if (_str.empty ())
   {
      return 0.f;
   }
   else
   {
      if (_typesetter_aptr.get () == 0)
      {
         _typesetter_aptr.reset (
            new opal::Typesetter (_scripture)
         );
      }
      
      size_t scripture_end = 0;
      float height = 0.f;
      
      float big_available_width = 100000.f;
      
      float ret_val = _typesetter_aptr->suggest_char_break (
         scripture_end, height, 0, big_available_width
      );
      
      return ret_val;
   }
}



/*
==============================================================================
Name : set_available_width
==============================================================================
*/

void  LabelSimpleHelper::set_available_width (float width)
{
   if (_available_width != width)
   {
      _available_width = width;
      
      _line_info_ptr = 0;
      _need_update_flag = true;
   }
}



/*
==============================================================================
Name : get_cur_width
==============================================================================
*/

float LabelSimpleHelper::get_cur_width ()
{
   update ();
   
   return _cur_width;
}



/*
==============================================================================
Name : display
Description :
   display label at 'pt' relative to 'flush_factor' with 'scale'
   the label follows the context rotation and every glyph is hinted.
==============================================================================
*/

void  LabelSimpleHelper::display (opak::GraphicContext & context, const opa::Point & pt, const opal::Color & color, float scale)
{
   update ();
   
   if (_line_info_ptr == 0)
   {
      // nothing to display, abort
      
      return;
   }
   
   // 
   
   assert (_typesetter_aptr.get () != 0);

   opa::Point pt_base (pt);
   
   context.apply (pt_base);
      
   opa::Size vx (1.f, 0.f);
   opa::Size vy (0.f, 1.f);
   
   context.apply (vx);
   context.apply (vy);
   
   // make unitary and 'scale'
   float invnvx = vx.inv_norm_2 () * scale;
   float invnvy = vy.inv_norm_2 () * scale;
   vx._width *= invnvx;
   vx._height *= invnvx;
   vy._width *= invnvy;
   vy._height *= invnvy;
   
   // vx represents one scaled pixel shift on x relative to context (with rotation)
   // vy represents one scaled pixel shift on y relative to context (with rotation)
   
   const opal::Font::Glyphs & glyphs = _font.get_glyphs (context.get ());
   
   const std::vector <opal::Typesetter::Run> & run_arr = _line_info_ptr->_run_arr;
   
   for (size_t i = 0 ; i < run_arr.size () ; ++i)
   {
      const opal::Typesetter::Run & run = run_arr [i];
      
      size_t pos = run._glyph_range._start;
      
      opal::GlyphArray::iterator it = _typesetter_aptr->get_glyph_iterator (pos);
      
      for (; pos < run._glyph_range._end ; ++pos, ++it)
      {
         const opal::Glyph & glyph = *it;
         
         display (
            context, glyphs, glyph._gid,
            pt_base, opa::Point (glyph._x, glyph._y),
            vx, vy, color, scale
         );
      }
      
      pt_base._x += run._width * vx._width;
      pt_base._y += run._width * vx._height;
   }
   
   if (_display_ellipsis_flag)
   {
      display (
         context, glyphs, _ellipsis_gid,
         pt_base, opa::Point::_zero,
         vx, vy, color, scale
      );
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : display
==============================================================================
*/

void  LabelSimpleHelper::display (opak::GraphicContext & context, const opal::Font::Glyphs & glyphs, archi::UInt32 glyph_gid, const opa::Point & screen_pt, const opa::Point & line_pt, const opa::Size & vx, const opa::Size & vy, const opal::Color & color, float scale)
{
   opa::Point glyph_offset;
   const opal::Image & image = glyphs.get_glyph_mask (glyph_offset, glyph_gid);
   
   opa::Point offset (
      line_pt._x + glyph_offset._x,
      line_pt._y + glyph_offset._y
   );
   
   opa::Point pt_00 (
      screen_pt._x + offset._x * vx._width + offset._y * vy._width,
      screen_pt._y + offset._x * vx._height + offset._y * vy._height
   );
   
   const float w = float (image._width);
   const float h = float (image._height);

   pt_00.nearest_integral ();

   opa::Point pt_10 (pt_00);
   pt_10._x += w * vx._width;
   pt_10._y += w * vx._height;

   opa::Point pt_11 (pt_00);
   pt_11._x += w * vx._width + h * vy._width;
   pt_11._y += w * vx._height + h * vy._height;

   opa::Point pt_01 (pt_00);
   pt_01._x += h * vy._width;
   pt_01._y += h * vy._height;
   
   context.get ().draw (
      image,
      opa::Quad (pt_00, pt_10, pt_11, pt_01),
      opa::Quad (image),
      color
   );
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  LabelSimpleHelper::update ()
{
   if (_need_update_flag)
   {
      if (!_str.empty ())
      {
         if (_typesetter_aptr.get () == 0)
         {
            _typesetter_aptr.reset (
               new opal::Typesetter (_scripture)
            );
         }
         
         size_t scripture_end = 0;
         float height = 0.f;
         
         _cur_width = _typesetter_aptr->suggest_char_break (
            scripture_end, height, 0, _available_width
         );
         _display_ellipsis_flag = false;
         
         if ((scripture_end < _scripture.size ()) && _enable_ellipsis_flag)
         {
            // char break occured, readjust and add ellipsis

            _cur_width = _typesetter_aptr->suggest_char_break (
               scripture_end, height, 0, _available_width - _ellipsis_width
            );
            _cur_width += _ellipsis_width;
            _display_ellipsis_flag = true;
         }
         
         opal::Scripture::Range range;
         range._start = 0;
         range._end = scripture_end;
         
         // layout just one line
         _line_info_ptr = _typesetter_aptr->layout_line (range, _available_width, false, 0);
      }
      else
      {
         _cur_width = 0.f;
         _line_info_ptr = 0;
      }
      
      _need_update_flag = false;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
