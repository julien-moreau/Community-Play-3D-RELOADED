/*****************************************************************************

        SpacialObject.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70455

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

#include "model/SpacialObject.h"

#include "ohm/flip/ClassDescription.h"

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

void  SpacialObject::declare ()
{
   using namespace ohm::flip;
   
   ClassDescription <SpacialObject>::use ().set_name ("skeleton.SpacialObject");
   ClassDescription <SpacialObject>::use ().push_var_desc (&SpacialObject::_position_x, "_position_x");
   ClassDescription <SpacialObject>::use ().push_var_desc (&SpacialObject::_position_y, "_position_y");
   
   ClassDescManager::declare (ClassDescription <SpacialObject>::use ());
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

SpacialObject::SpacialObject (ohm::flip::DocumentBase & document)
:  ohm::flip::Object (document)
,  _position_x (document)
,  _position_y (document)

,  _tx_session_guard (document)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SpacialObject::~SpacialObject ()
{
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

bool  SpacialObject::operator < (const SpacialObject & other)
{
   return get_ref () < other.get_ref ();
}



/*
==============================================================================
Name : update_client_data_cache
==============================================================================
*/

void  SpacialObject::update_client_data_cache ()
{
   // nothing
}



/*
==============================================================================
Name : ext_set_position
==============================================================================
*/

void  SpacialObject::ext_set_position (ohm::archi::Float64 position_x, ohm::archi::Float64 position_y)
{
   _position_x = position_x;
   _position_y = position_y;
}



/*
==============================================================================
Name : ext_set_position_x
==============================================================================
*/

void  SpacialObject::ext_set_position_x (ohm::archi::Float64 position_x)
{
   _position_x = position_x;
}



/*
==============================================================================
Name : ext_set_y
==============================================================================
*/

void  SpacialObject::ext_set_position_y (ohm::archi::Float64 position_y)
{
   _position_y = position_y;
}



/*
==============================================================================
Name : position_changed
==============================================================================
*/

bool  SpacialObject::position_changed () const
{
   return _position_x.did_value_change () || _position_y.did_value_change ();
}



/*
==============================================================================
Name : get_position_x
==============================================================================
*/

ohm::archi::Float64  SpacialObject::get_position_x () const
{
   return _position_x;
}



/*
==============================================================================
Name : get_position_y
==============================================================================
*/

ohm::archi::Float64  SpacialObject::get_position_y () const
{
   return _position_y;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
