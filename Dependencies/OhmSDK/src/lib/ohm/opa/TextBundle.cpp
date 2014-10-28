/*****************************************************************************

        TextBundle.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 50066

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

#include "ohm/opa/TextBundle.h"
#include "ohm/opak/TextField.h"

#include "ohm/txt/Err.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

TextBundle::TextBundle (Bundle & parent_bundle, size_t bundle_slot_nbr)
:  Bundle (parent_bundle, "Text")
,  _frame_aptr ()
,  _text_field_aptr (0)
{
   _text_field_aptr = new opak::TextField;
   _text_field_aptr->set_name ("TextBundle::TextField");

   _frame_aptr = std::auto_ptr <opal::TextFrame> (
      new opal::TextFrame (*this, *_text_field_aptr)
   );
   parent_bundle.get_slot_frame (bundle_slot_nbr).insert_top (*_frame_aptr);
   
   parent_bundle.get_slot_layer (bundle_slot_nbr).push_back (*_text_field_aptr);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

TextBundle::~TextBundle ()
{
}



/*
==============================================================================
Name : set_origin
==============================================================================
*/

void  TextBundle::set_origin (const Point & point)
{
   _frame_aptr->set_origin (point);
   _text_field_aptr->_origin = point;
}



/*
==============================================================================
Name : set_bounds
==============================================================================
*/

void  TextBundle::set_bounds (const Size & size)
{
   _frame_aptr->set_bounds (size);
   _text_field_aptr->_size = size;
}



/*
==============================================================================
Name : push_state_font
==============================================================================
*/

void  TextBundle::push_state_font (const opal::Font & font)
{
   _frame_aptr->push_state_font (font);
}



/*
==============================================================================
Name : clear_state_font
==============================================================================
*/

void  TextBundle::clear_state_font ()
{
   _frame_aptr->clear_state_font ();
}



/*
==============================================================================
Name : set_state_color
==============================================================================
*/

void  TextBundle::set_state_color (const opal::Color & color)
{
   _frame_aptr->set_state_color (color);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
