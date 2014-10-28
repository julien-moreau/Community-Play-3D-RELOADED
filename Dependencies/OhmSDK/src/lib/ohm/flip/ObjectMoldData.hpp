/*****************************************************************************

        ObjectMoldData.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63856

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



#if defined (ohm_flip_ObjectMoldData_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObjectMoldData code header.
#endif
#define  ohm_flip_ObjectMoldData_CURRENT_CODEHEADER

#if ! defined (ohm_flip_ObjectMoldData_CODEHEADER_INCLUDED)
#define  ohm_flip_ObjectMoldData_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/ClassDescManager.h"



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : is_of_type
==============================================================================
*/

template <class Type>
bool  ObjectMoldData::is_of_type () const
{
   int base_class_id = ClassDescription <Type>::use ().get_class_id ();
   
   return ClassDescManager::use_instance ().is_class_id_inheriting_from_class_id (
      _type_id, base_class_id
   );
}



/*
==============================================================================
Name : get_value
==============================================================================
*/

template <class T>
archi::Int64   ObjectMoldData::get_value (Int64 T::*var, const char * var_name_0) const
{
   NameValueMap::const_iterator it = _name_value_map.find (var_name_0);
   
   assert (it != _name_value_map.end ());
   
   const Value & value = it->second;
   
   return value._val_i64;
}



/*
==============================================================================
Name : get_value
==============================================================================
*/

template <class T>
Blob::Data  ObjectMoldData::get_value (Blob T::*var, const char * var_name_0) const
{
   NameValueMap::const_iterator it = _name_value_map.find (var_name_0);
   
   assert (it != _name_value_map.end ());
   
   const Value & value = it->second;
   
   return value._val_blob;
}



/*
==============================================================================
Name : get_value
==============================================================================
*/

template <class T, class U>
flip::Ref   ObjectMoldData::get_value (ObjectRef <U> T::*var, const char * var_name_0) const
{
   NameValueMap::const_iterator it = _name_value_map.find (var_name_0);
   
   assert (it != _name_value_map.end ());
   
   const Value & value = it->second;
   
   return value._val_i64;
}



/*
==============================================================================
Name : get_value
==============================================================================
*/

template <class T, class U, archi::Int64 MIN, archi::Int64 NBR>
U  ObjectMoldData::get_value (Enum <U, MIN, NBR> T::*var, const char * var_name_0) const
{
   NameValueMap::const_iterator it = _name_value_map.find (var_name_0);
   
   assert (it != _name_value_map.end ());
   
   const Value & value = it->second;
   
   return U (value._val_i64);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_ObjectMoldData_CODEHEADER_INCLUDED

#undef ohm_flip_ObjectMoldData_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
