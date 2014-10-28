/*****************************************************************************

        LabelSimple.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66502

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

#include "ohm/opa/Constants.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/AutoContextState.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/private/Clock.h"
#include "ohm/opak/LabelSimple.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

LabelSimple::LabelSimple ()
:  _font_size (*this, &LabelSimple::font_changed)
,  _width (*this, &LabelSimple::bbox_changed)
,  _str (*this, &LabelSimple::str_changed)
,  _color (*this, &LabelSimple::color_changed)
,  _font_ptr (0)
,  _update_flag (false)
{
}



/*
==============================================================================
Name: restore_display
==============================================================================
*/

void LabelSimple::restore_display ()
{
   _font_ptr = 0;
   _lsh_aptr.reset ();
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  LabelSimple::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   
   if (_update_flag && is_ok ())
   {
      float width = _width.get_cur_value ();
      std::string str = _str.get_cur_value ();
      
      _lsh_aptr = std::auto_ptr <opak::LabelSimpleHelper> (
         new opak::LabelSimpleHelper (*_font_ptr, true)
      );
      
      _lsh_aptr->set_available_width (width);
      _lsh_aptr->set (str);
   }
   
   if (_lsh_aptr.get () != 0)
   {
      opal::Color color = _color.get_cur_value ();
      
      _lsh_aptr->display (context, opa::Point::_zero, color);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

LabelSimple::~LabelSimple ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : font_changed
==============================================================================
*/

void  LabelSimple::font_changed ()
{
   float font_size = _font_size.get_cur_value ();
   
   _font_ptr = &opal::FontManager::use_instance ().get_font (
      opa::Constants::_default_font_0, font_size
   );
   
   bbox_changed ();
}



/*
==============================================================================
Name : bbox_changed
==============================================================================
*/

void  LabelSimple::bbox_changed ()
{
   _update_flag = true;
   
   float ascent = 0.f;
   float descent = 0.f;
   float width = _width.get_cur_value ();
   
   if (_font_ptr != 0)
   {
      ascent = _font_ptr->get_ascent ();
      descent = _font_ptr->get_descent ();
   }
   
   opa::Rect bbox (
      0, - ascent, width, ascent + descent
   );
   
   set_bounding_box (bbox);
}



/*
==============================================================================
Name : str_changed
==============================================================================
*/

void  LabelSimple::str_changed ()
{
   _update_flag = true;
   
   invalidate_all ();
}



/*
==============================================================================
Name : color_changed
==============================================================================
*/

void  LabelSimple::color_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : is_ok
==============================================================================
*/

bool  LabelSimple::is_ok ()
{
   bool ok_flag = true;
   
   ok_flag &= _font_ptr != 0;
   ok_flag &= _width.get_cur_value () > 0.0;
   ok_flag &= !_str.get_cur_value ().empty ();
   
   return ok_flag;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
