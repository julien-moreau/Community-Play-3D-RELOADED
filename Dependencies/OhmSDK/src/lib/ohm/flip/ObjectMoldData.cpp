/*****************************************************************************

        ObjectMoldData.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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

#include "ohm/flip/ObjectMoldData.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ObjectMoldData::ObjectMoldData (flip::Ref ref, int class_id)
:  _ref (ref)
,  _type_id (class_id)
,  _name_value_map ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ObjectMoldData::~ObjectMoldData ()
{
}



/*
==============================================================================
Name : get_ref
==============================================================================
*/

flip::Ref   ObjectMoldData::get_ref () const
{
   return _ref;
}



/*
==============================================================================
Name : add_value
==============================================================================
*/

void  ObjectMoldData::add_value (const std::string & name, int type_id, archi::Int64 val)
{
   Value value;
   value._type_id = type_id;
   value._val_i64 = val;
   
   assert (_name_value_map.find (name) == _name_value_map.end ());
   
   _name_value_map [name] = value;
}



/*
==============================================================================
Name : add_value
==============================================================================
*/

void  ObjectMoldData::add_value (const std::string & name, int type_id, archi::Float64 val)
{
   Value value;
   value._type_id = type_id;
   value._val_f64 = val;
   
   assert (_name_value_map.find (name) == _name_value_map.end ());
   
   _name_value_map [name] = value;
}



/*
==============================================================================
Name : add_value
==============================================================================
*/

void  ObjectMoldData::add_value (const std::string & name, int type_id, const Blob::Data & val)
{
   Value value;
   value._type_id = type_id;
   value._val_blob = val;
   
   assert (_name_value_map.find (name) == _name_value_map.end ());
   
   _name_value_map [name] = value;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
