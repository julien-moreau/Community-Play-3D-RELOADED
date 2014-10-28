/*****************************************************************************

        EnumDescription.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if defined (ohm_flip_EnumDescription_CURRENT_CODEHEADER)
   #error Recursive inclusion of EnumDescription code header.
#endif
#define  ohm_flip_EnumDescription_CURRENT_CODEHEADER

#if ! defined (ohm_flip_EnumDescription_CODEHEADER_INCLUDED)
#define  ohm_flip_EnumDescription_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/EnumDescManager.h"
#include "ohm/lang/fnc.h"

#include <cassert>
#include <cstring>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
EnumDescription <T>::~EnumDescription ()
{
}



/*
==============================================================================
Name : set_name
==============================================================================
*/

template <class T>
EnumDescription <T> &   EnumDescription <T>::use ()
{
   static EnumDescription instance;
   
   return instance;
}



/*
==============================================================================
Name : get_enum_id
==============================================================================
*/

template <class T>
int   EnumDescription <T>::get_enum_id () const
{
   return _enum_id;
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

template <class T>
const char *   EnumDescription <T>::get_name () const
{
   return _name_0;
}



/*
==============================================================================
Name : get_min
==============================================================================
*/

template <class T>
archi::Int64   EnumDescription <T>::get_min () const
{
   return _min;
}



/*
==============================================================================
Name : get_nbr_total
==============================================================================
*/

template <class T>
archi::Int64   EnumDescription <T>::get_nbr_total () const
{
   return _enum_info_arr.size ();
}



/*
==============================================================================
Name : get_var_info_arr
==============================================================================
*/

template <class T>
const typename EnumDescription <T>::EnumInfoArr &  EnumDescription <T>::get_enum_info_arr () const
{
   assert (!_enum_info_arr.empty ());
   
   return _enum_info_arr;
}



/*
==============================================================================
Name : get_value_for_name
==============================================================================
*/

template <class T>
int   EnumDescription <T>::get_value_for_name (archi::Int64 & val, const std::string & name) const
{
   typename EnumInfoArr::const_iterator it = _enum_info_arr.begin ();
   const typename EnumInfoArr::const_iterator it_end = _enum_info_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const EnumInfo & enum_info = *it;
      
      if (std::string (enum_info._name_0) == name)
      {
         val = enum_info._val;

         assert (val >= _min);
         assert (val <= _max);
         
         return Err_OK;
      }
   }
   
   return Err_VM_BAD_VALUE;
}



/*
==============================================================================
Name : get_name_for_value
==============================================================================
*/

template <class T>
int   EnumDescription <T>::get_name_for_value (const char * & name_0, archi::Int64 val) const
{
   typename EnumInfoArr::const_iterator it = _enum_info_arr.begin ();
   const typename EnumInfoArr::const_iterator it_end = _enum_info_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const EnumInfo & enum_info = *it;
      
      if (enum_info._val == val)
      {
         name_0 = enum_info._name_0;
         
         return Err_OK;
      }
   }
   
   name_0 = 0;
   
   return Err_VM_BAD_VALUE;
}



/*
==============================================================================
Name : set_enum_id
==============================================================================
*/

template <class T>
void  EnumDescription <T>::set_enum_id (int enum_id)
{
   assert (enum_id >= 1);
   
   _enum_id = enum_id;
}



/*
==============================================================================
Name : post_check
==============================================================================
*/

template <class T>
void  EnumDescription <T>::post_check ()
{
   assert (_enum_id >= 1);
   assert (_name_0 != 0);
   
   // cannot be empty
   assert (_max > _min);
   assert (!_enum_info_arr.empty ());

   // no holes in enum are permitted
   assert (_max - _min + 1 == _enum_info_arr.size ());
}



/*
==============================================================================
Name : set_name
==============================================================================
*/

template <class T>
void  EnumDescription <T>::set_name (const char * name_0)
{
   assert (name_0 != 0);
   
#if ! defined (NDEBUG)  
   std::string name (name_0);
   
   for (size_t i = 0 ; i < name.size () ; ++i)
   {
      char c = name [i];
      
      bool ok_flag = false;
      ok_flag |= ((c >= 'a') && (c <= 'z'));
      ok_flag |= ((c >= 'A') && (c <= 'Z'));
      ok_flag |= ((c >= '0') && (c <= '9'));
      ok_flag |= (c == '_');
      ok_flag |= (c == '.');
      
      assert (ok_flag);
   }
#endif
   
   _name_0 = name_0;
}



/*
==============================================================================
Name : inherit
==============================================================================
*/

template <class T>
template <class U>
void  EnumDescription <T>::inherit ()
{
   EnumDescriptionMaster * super_edm_ptr
      = &EnumDescription <U>::use ();
   
   const EnumInfoArr & enum_info_arr
      = super_edm_ptr->get_enum_info_arr ();

   // did you forget to declare base enum inheritance before declaring
   // this enum ?
   
   assert (!enum_info_arr.empty ());
   
   // we don't support multiple inheritance, and inheritage must
   // be declared before any class member
   assert (_enum_info_arr.empty ());
   
   // add the enums of the base enum
   
   _enum_info_arr.insert (
      _enum_info_arr.end (),
      enum_info_arr.begin (),
      enum_info_arr.end ()
   );
}



/*
==============================================================================
Name : push_enum_desc
==============================================================================
*/

template <class T>
void  EnumDescription <T>::push_enum_desc (T val, const char * val_name_0)
{
   assert (val_name_0 != 0);

#if ! defined (NDEBUG)  
   std::string val_name (val_name_0);
   
   for (size_t i = 0 ; i < val_name.size () ; ++i)
   {
      char c = val_name [i];
      
      bool ok_flag = false;
      ok_flag |= ((c >= 'a') && (c <= 'z'));
      ok_flag |= ((c >= 'A') && (c <= 'Z'));
      ok_flag |= ((c >= '0') && (c <= '9'));
      ok_flag |= (c == '_');
      
      assert (ok_flag);
   }
#endif
   
   EnumInfo enum_info;
   enum_info._val = archi::Int64 (val);
   enum_info._name_0 = val_name_0;
   
   _enum_info_arr.push_back (enum_info);
   
   lang::min_in_place (_min, enum_info._val);
   lang::max_in_place (_max, enum_info._val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
EnumDescription <T>::EnumDescription ()
:  _enum_id (0)
,  _name_0 (0)
,  _enum_info_arr ()
,  _min (archi::INT64_MAX_VAL)
,  _max (archi::INT64_MIN_VAL)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_EnumDescription_CODEHEADER_INCLUDED

#undef ohm_flip_EnumDescription_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
