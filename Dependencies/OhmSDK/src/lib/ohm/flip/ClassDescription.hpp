/*****************************************************************************

        ClassDescription.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 48045

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



#if defined (ohm_flip_ClassDescription_CURRENT_CODEHEADER)
   #error Recursive inclusion of ClassDescription code header.
#endif
#define  ohm_flip_ClassDescription_CURRENT_CODEHEADER

#if ! defined (ohm_flip_ClassDescription_CODEHEADER_INCLUDED)
#define  ohm_flip_ClassDescription_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/EnumMaster.h"
#include "ohm/flip/private/ArrayMaster.h"
#include "ohm/flip/private/CollectionMaster.h"

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
ClassDescription <T>::~ClassDescription ()
{
}



/*
==============================================================================
Name : set_name
==============================================================================
*/

template <class T>
ClassDescription <T> &  ClassDescription <T>::use ()
{
   static ClassDescription instance;
   
   return instance;
}



/*
==============================================================================
Name : get_class_id
==============================================================================
*/

template <class T>
int   ClassDescription <T>::get_class_id () const
{
   return _class_id;
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

template <class T>
const char *   ClassDescription <T>::get_name () const
{
   return _name_0;
}



/*
==============================================================================
Name : get_name
==============================================================================
*/

template <class T>
ClassDescriptionMaster *   ClassDescription <T>::get_super_cdm_ptr () const
{
   return _super_cdm_ptr;
}



/*
==============================================================================
Name : get_size_of
==============================================================================
*/

template <class T>
int   ClassDescription <T>::get_size_of () const
{
   assert (_size_of > 0);
   
   return _size_of;
}



/*
==============================================================================
Name : get_var_info_arr
==============================================================================
*/

template <class T>
const typename ClassDescription <T>::VarInfoArr &  ClassDescription <T>::get_var_info_arr () const
{
   // the returned list might be empty (case of basic type for example)
   
   return _var_info_arr;
}



/*
==============================================================================
Name : get_var_info
==============================================================================
*/

template <class T>
const typename ClassDescription <T>::VarInfo *  ClassDescription <T>::get_var_info (const std::string & var_name) const
{
   typename VarInfoArr::const_iterator it = _var_info_arr.begin ();
   const typename VarInfoArr::const_iterator it_end = _var_info_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const VarInfo & var_info = *it;
      
      if (std::string (var_info._name_0) == var_name)
      {
         return &var_info;
      }
   }
   
   return 0;
}



/*
==============================================================================
Name : set_name
==============================================================================
*/

template <class T>
void  ClassDescription <T>::set_name (const char * name_0)
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
void  ClassDescription <T>::inherit ()
{
   _super_cdm_ptr = &ClassDescription <U>::use ();

   // did you forget to declare base class inheritance before declaring
   // this class members ?

   // we don't support multiple inheritance, and inheritage must
   // be declared before any class member
   assert (_var_info_arr.empty ());
   
   // add the members of the base class
   
   const VarInfoArr & var_info_arr
      = _super_cdm_ptr->get_var_info_arr ();
   
   _var_info_arr.insert (
      _var_info_arr.end (),
      var_info_arr.begin (),
      var_info_arr.end ()
   );
   
   _nbr_ref = _super_cdm_ptr->get_nbr_ref ();
}



/*
==============================================================================
Name : push_var_desc
==============================================================================
*/

template <class T>
template <class U>
void  ClassDescription <T>::push_var_desc (U T::*var, const char * var_name_0)
{
   assert (var_name_0 != 0);

#if ! defined (NDEBUG)  
   std::string var_name (var_name_0);
   
   for (size_t i = 0 ; i < var_name.size () ; ++i)
   {
      char c = var_name [i];
      
      bool ok_flag = false;
      ok_flag |= ((c >= 'a') && (c <= 'z'));
      ok_flag |= ((c >= 'A') && (c <= 'Z'));
      ok_flag |= ((c >= '0') && (c <= '9'));
      ok_flag |= (c =='_');
      
      assert (ok_flag);
   }
#endif
   
   ClassDescriptionMaster & cdm = ClassDescription <U>::use ();

   int type_id = cdm.get_class_id ();
   assert (type_id > 0);   // did you forget to declare the class ?
   
   VarInfo var_info;
   var_info._type_id = type_id;
   var_info._cdm_ptr = &cdm;
   var_info._name_0 = var_name_0;
   var_info._var_offset = _nbr_ref;
   
   _nbr_ref += cdm.get_nbr_ref ();
   
   _var_info_arr.push_back (var_info);
}



/*
==============================================================================
Name : push_var_desc
==============================================================================
*/

template <class T>
template <class U, archi::Int64 MIN, archi::Int64 NBR>
void  ClassDescription <T>::push_var_desc (Enum <U, MIN, NBR> T::*var, const char * var_name_0)
{
   assert (var_name_0 != 0);
   
   ClassDescriptionMaster & cdm = ClassDescription <EnumMaster>::use ();

   int type_id = cdm.get_class_id ();
   assert (type_id > 0);
   
   VarInfo var_info;
   var_info._type_id = type_id;
   var_info._cdm_ptr = &cdm;
   var_info._name_0 = var_name_0;
   var_info._var_offset = _nbr_ref;
   
   _nbr_ref += 1;
   
   _var_info_arr.push_back (var_info);
}



/*
==============================================================================
Name : push_var_desc
==============================================================================
*/

template <class T>
template <class U>
void  ClassDescription <T>::push_var_desc (ObjectRef <U> T::*var, const char * var_name_0)
{
   assert (var_name_0 != 0);
   
   ClassDescriptionMaster & cdm = ClassDescription <ObjectRefMaster>::use ();

   int type_id = cdm.get_class_id ();
   assert (type_id > 0);
   
   VarInfo var_info;
   var_info._type_id = type_id;
   var_info._cdm_ptr = &cdm;
   var_info._name_0 = var_name_0;
   var_info._var_offset = _nbr_ref;
   
   _nbr_ref += 1;
   
   _var_info_arr.push_back (var_info);
}



/*
==============================================================================
Name : push_var_desc
==============================================================================
*/

template <class T>
template <class U>
void  ClassDescription <T>::push_var_desc (Array <U> T::*var, const char * var_name_0)
{
   assert (var_name_0 != 0);

   ClassDescriptionMaster & cdm = ClassDescription <ArrayMaster>::use ();

   int type_id = cdm.get_class_id ();
   assert (type_id > 0);
   
   VarInfo var_info;
   var_info._type_id = type_id;
   var_info._cdm_ptr = &cdm;
   var_info._name_0 = var_name_0;
   var_info._var_offset = _nbr_ref;
   
   _nbr_ref += 1;
   
   _var_info_arr.push_back (var_info);
}



/*
==============================================================================
Name : push_var_desc
==============================================================================
*/

template <class T>
template <class U>
void  ClassDescription <T>::push_var_desc (Collection <U> T::*var, const char * var_name_0)
{
   assert (var_name_0 != 0);

   ClassDescriptionMaster & cdm = ClassDescription <CollectionMaster>::use ();

   int type_id = cdm.get_class_id ();
   assert (type_id > 0);
   
   VarInfo var_info;
   var_info._type_id = type_id;
   var_info._cdm_ptr = &cdm;
   var_info._name_0 = var_name_0;
   var_info._var_offset = _nbr_ref;
   
   _nbr_ref += 1;
   
   _var_info_arr.push_back (var_info);
}



/*
==============================================================================
Name : enable_root
==============================================================================
*/

template <class T>
void  ClassDescription <T>::enable_root (const std::string & format_version_str)
{
   _root_flag = true;

   // check format string, we don't allow spaces (' ')
   
   assert (format_version_str.find (' ') == std::string::npos);

   _format_version_str = format_version_str;
   
}



/*
==============================================================================
Name : is_root
==============================================================================
*/

template <class T>
bool  ClassDescription <T>::is_root () const
{
   return _root_flag;
}



/*
==============================================================================
Name : get_format_version
==============================================================================
*/

template <class T>
const std::string &  ClassDescription <T>::get_format_version () const
{
   assert (_format_version_str != "");
   
   return _format_version_str;
}



/*
==============================================================================
Name : enable_only_write_in_full_state
==============================================================================
*/

template <class T>
void  ClassDescription <T>::forbid_instantiation ()
{
   _forbid_instantiation_flag = true;
}



/*
==============================================================================
Name : get_nbr_ref
Description :
   count the nbr of refs contains in this object and child object tree (not
   including this ref) and return that number.
==============================================================================
*/

template <class T>
archi::Int64   ClassDescription <T>::get_nbr_ref () const
{
   return _nbr_ref;
}



/*
==============================================================================
Name : set_class_id
==============================================================================
*/

template <class T>
void  ClassDescription <T>::set_class_id (int class_id)
{
   assert (class_id >= 1);
   
   _class_id = class_id;
}



/*
==============================================================================
Name : post_check
==============================================================================
*/

template <class T>
void  ClassDescription <T>::post_check ()
{
#if !defined (NDEBUG)
   assert (_class_id >= 1);
   assert (_name_0 != 0);
   
   typename VarInfoArr::const_iterator it = _var_info_arr.begin ();
   typename VarInfoArr::const_iterator it_end = _var_info_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const VarInfo & var_info = *it;
      
      int type_id = var_info._type_id;
      assert (type_id > 0);
   }
#endif
}



/*
==============================================================================
Name : create
==============================================================================
*/

template <class T>
Object * ClassDescription <T>::create (void * ptr, DocumentBase & document)
{
   assert (!_forbid_instantiation_flag);
   
   ohm::flip::Object * this_ptr = new (ptr) T (document);
   
   return this_ptr;
}



/*
==============================================================================
Name : create
==============================================================================
*/

template <class T>
Object * ClassDescription <T>::create (void * ptr, DocumentBase & document, Args & args)
{
   assert (!_forbid_instantiation_flag);
   
   ohm::flip::Object * this_ptr = new (ptr) T (document);
   
   ctor (this_ptr, args);
   args.end ();
   
   return this_ptr;
}



/*
==============================================================================
Name : release
==============================================================================
*/

template <class T>
void  ClassDescription <T>::release (Object * obj_ptr)
{
   T * cast_ptr = dynamic_cast <T *> (obj_ptr);
   
   assert (cast_ptr != 0);
   
   cast_ptr->~T ();
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
void  ClassDescription <T>::ctor (Object * obj_ptr, Args & args)
{
   T * cast_ptr = dynamic_cast <T *> (obj_ptr);
   
   assert (cast_ptr != 0);
   
   if (_super_cdm_ptr != 0)
   {
      _super_cdm_ptr->ctor (obj_ptr, args);
   }
   
   cast_ptr->T::ctor (args);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
ClassDescription <T>::ClassDescription ()
:  _class_id (0)
,  _name_0 (0)
,  _super_cdm_ptr (0)
,  _size_of (sizeof (T))
,  _var_info_arr ()
,  _root_flag (false)
,  _format_version_str ()
,  _forbid_instantiation_flag (false)
,  _nbr_ref (1)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_ClassDescription_CODEHEADER_INCLUDED

#undef ohm_flip_ClassDescription_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
