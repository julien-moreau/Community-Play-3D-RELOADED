/*****************************************************************************

        FontProxy.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 35197

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

#include "ohm/opal/FontManager.h"
#include "ohm/opak/base/FontProxy.h"

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

FontProxy::FontProxy ()
:  _name_size_pair ("", 0.f)
,  _font_ptr (0)
,  _font_need_update_flag (false)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

FontProxy::FontProxy (const FontProxy & other)
:  _name_size_pair (other._name_size_pair)
,  _font_ptr (other._font_ptr)
,  _font_need_update_flag (other._font_need_update_flag)
{
}


/*
==============================================================================
Name : dtor
==============================================================================
*/

FontProxy::~FontProxy ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

FontProxy & FontProxy::operator = (const FontProxy & other)
{
   return this->operator = (other._name_size_pair);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

FontProxy & FontProxy::operator = (const NameSizePair & name_size_pair)
{
   if (_name_size_pair != name_size_pair)
   {
      _name_size_pair = name_size_pair;
      _font_ptr = 0;
      _font_need_update_flag = true;
   }
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  FontProxy::operator == (const FontProxy &other) const
{
   return _name_size_pair == other._name_size_pair;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  FontProxy::operator != (const FontProxy &other) const
{
   return _name_size_pair != other._name_size_pair;
}



/*
==============================================================================
Name : operator const opal::Font &
==============================================================================
*/

FontProxy::operator const opal::Font & () const
{
   update ();
   
   assert (_font_ptr != 0);
   
   return *_font_ptr;
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  FontProxy::empty () const
{
   update ();
   
   return (_font_ptr == 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  FontProxy::update () const
{
   if (_font_need_update_flag)
   {
      _font_ptr = &opal::FontManager::use_instance ().get_font (
         _name_size_pair.first, _name_size_pair.second
      );

      _font_need_update_flag = false;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
