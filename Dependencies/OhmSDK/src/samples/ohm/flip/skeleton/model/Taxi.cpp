/*****************************************************************************

        Taxi.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70477

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

#include "model/Taxi.h"

#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/EnumDescription.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
Note :
   Rather than putting a _display_name into the client, we will fetch it
   when we would need it, as it might actually change
   User name is guaranteed to be unique and non changing, it is the one
   used for authentification
==============================================================================
*/

void  Taxi::declare ()
{
   using namespace ohm::flip;

   EnumDescription <Color>::use ().set_name ("skeleton.Taxi.Color");
   EnumDescription <Color>::use ().push_enum_desc (Color_WHITE, "WHITE");
   EnumDescription <Color>::use ().push_enum_desc (Color_BLACK, "BLACK");
   EnumDescription <Color>::use ().push_enum_desc (Color_YELLOW, "YELLOW");
   EnumDescription <Color>::use ().push_enum_desc (Color_BLUE, "BLUE");
   EnumDescription <Color>::use ().push_enum_desc (Color_GREEN, "GREEN");
   
   EnumDescManager::declare (EnumDescription <Color>::use ());
   
   ClassDescription <Taxi>::use ().set_name ("skeleton.Taxi");
   ClassDescription <Taxi>::use ().inherit <SpacialObject> ();
   ClassDescription <Taxi>::use ().push_var_desc (&Taxi::_width, "_width");
   ClassDescription <Taxi>::use ().push_var_desc (&Taxi::_color, "_color");
   
   ClassDescManager::declare (ClassDescription <Taxi>::use ());
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Taxi::Taxi (ohm::flip::DocumentBase & document)
:  SpacialObject (document)
,  _width (document)
,  _color (document)

,  _tx_session_guard (document)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Taxi::~Taxi ()
{
}



/*
==============================================================================
Name : ext_set_width
==============================================================================
*/

void  Taxi::ext_set_width (ohm::archi::Float64 width)
{
   _width = width;
}



/*
==============================================================================
Name : width_changed
==============================================================================
*/

bool  Taxi::width_changed () const
{
   return _width.did_value_change ();
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

ohm::archi::Float64  Taxi::get_width () const
{
   return _width;
}



/*
==============================================================================
Name : get_color_name_0
==============================================================================
*/

const char *   Taxi::get_color_name_0 (Color color)
{
   assert (color >= 0);
   assert (color < Color_NBR_ELT);
   
   static const char * color_name_0 [] = {
      "White",
      "Black",
      "Yellow",
      "Blue",
      "Green"
   };
   
   return color_name_0 [color];
}



/*
==============================================================================
Name : ext_set_color
==============================================================================
*/

void  Taxi::ext_set_color (Color color)
{
   _color = color;
}



/*
==============================================================================
Name : color_changed
==============================================================================
*/

bool  Taxi::color_changed () const
{
   return _color.did_value_change ();
}



/*
==============================================================================
Name : get_color
==============================================================================
*/

Taxi::Color Taxi::get_color () const
{
   return _color;
}



/*
==============================================================================
Name : intersect_width_delta_range
==============================================================================
*/

void  Taxi::intersect_width_delta_range (RangeFloat64 & range)
{
   using namespace ohm;
   
   archi::Float64 width = get_width ();
   
   RangeFloat64 sub_range;
   sub_range._min = _min_width - width;
   sub_range._max = _max_width - width;
   
   range &= sub_range;
}


const ohm::archi::Float64  Taxi::_min_width = 180.0;
const ohm::archi::Float64  Taxi::_max_width = 260.0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
