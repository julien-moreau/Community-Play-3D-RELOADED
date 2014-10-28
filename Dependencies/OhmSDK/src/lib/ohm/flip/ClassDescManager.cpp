/*****************************************************************************

        ClassDescManager.cpp
        Copyright (c) 2008 Ohm Force

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

#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/Bool.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Enum.h"
#include "ohm/flip/Float64.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/Array.h"
#include "ohm/flip/ObjectRef.h"
#include "ohm/flip/Cue.h"
#include "ohm/util/trace.h"

#include <cassert>



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

ClassDescManager::~ClassDescManager ()
{
}



/*
==============================================================================
Name : declare
==============================================================================
*/

void  ClassDescManager::declare (ClassDescriptionMaster & class_description)
{
   use_instance ().do_declare (class_description);
}



/*
==============================================================================
Name : use_instance
==============================================================================
*/

ClassDescManager &   ClassDescManager::use_instance ()
{
   static ClassDescManager instance;
   
   return instance;
}



/*
==============================================================================
Name : declare_basic_types
==============================================================================
*/

void  ClassDescManager::declare_basic_types ()
{
   Bool::declare ();
   Int64::declare ();

   // fake Enum declaration
   {
      ClassDescription <EnumMaster>::use ().set_name (BasicTypesName::_enum_0);
      
      do_declare (ClassDescription <EnumMaster>::use ());
   }
   

   Float64::declare ();
   Blob::declare ();
   
   // fake Array declaration
   {
      ClassDescription <ArrayMaster>::use ().set_name (BasicTypesName::_array_0);
      
      do_declare (ClassDescription <ArrayMaster>::use ());
   }
   
   // fake Collection declaration
   {
      ClassDescription <CollectionMaster>::use ().set_name (BasicTypesName::_collection_0);
      
      do_declare (ClassDescription <CollectionMaster>::use ());
   }

   // fake ObjectRef declaration
   {
      ClassDescription <ObjectRefMaster>::use ().set_name (BasicTypesName::_object_ref_0);
      
      do_declare (ClassDescription <ObjectRefMaster>::use ());
   }

   Cue::declare ();
}



/*
==============================================================================
Name : get_format_version
==============================================================================
*/

const std::string &  ClassDescManager::get_format_version () const
{
   assert (_format_version_str != "");
   
   return _format_version_str;
}



/*
==============================================================================
Name : get
==============================================================================
*/

ClassDescriptionMaster &   ClassDescManager::get (int class_id)
{
   assert (class_id > 0);
   --class_id;
   assert (size_t (class_id) < _class_id_class_desc_arr.size ());
   
   return *_class_id_class_desc_arr [class_id];
}



/*
==============================================================================
Name : get
==============================================================================
*/

const char *   ClassDescManager::who (int class_id)
{
   assert (class_id > 0);
   --class_id;
   if (size_t (class_id) < _class_id_class_desc_arr.size ())
   {
      return _class_id_class_desc_arr [class_id]->get_name ();
   }
   else
   {
      return "<undefined>";
   }
}



/*
==============================================================================
Name : find
Note :
   We don't need a fast function, since this is only used to resolve
   class names from stored files to class desc
==============================================================================
*/

ClassDescriptionMaster *   ClassDescManager::find (const std::string & class_name)
{
   for (size_t i = 0 ; i < _class_id_class_desc_arr.size () ; ++i)
   {
      ClassDescriptionMaster * cdm_ptr = _class_id_class_desc_arr [i];
      
      if (cdm_ptr->get_name () == class_name)
      {
         return cdm_ptr;
      }
   }
   
   return 0;
}



/*
==============================================================================
Name : find_root
Description :
   Returns the class description of the unique class marked with the
   root feature.
   In particular, if multiple root classes exist, it will assert and return
   0.
Note :
   We don't need a fast function, since this is only used to resolve
   root when creating skeleton files.
==============================================================================
*/

ClassDescriptionMaster *   ClassDescManager::find_root ()
{
   ClassDescriptionMaster * ret_cdm_ptr = 0;
   
   for (size_t i = 0 ; i < _class_id_class_desc_arr.size () ; ++i)
   {
      ClassDescriptionMaster * cdm_ptr = _class_id_class_desc_arr [i];
      
      if (cdm_ptr->is_root ())
      {
         // check root class unicity
         if (ret_cdm_ptr != 0)
         {
            ohm_util_TRACE_PANIC ("Multiple root classes exist");
         }
         
         assert (ret_cdm_ptr == 0);
         
         ret_cdm_ptr = cdm_ptr;
      }
   }
   
   // root class must exist
   if (ret_cdm_ptr == 0)
   {
      ohm_util_TRACE_PANIC ("No root class exist");
   }
   
   assert (ret_cdm_ptr != 0);
   
   return ret_cdm_ptr;
}



/*
==============================================================================
Name : is_class_id_inheriting_from_class_id
==============================================================================
*/

bool  ClassDescManager::is_class_id_inheriting_from_class_id (int class_id, int supposed_base_class_id)
{
   if (class_id == supposed_base_class_id)
   {
      return true;
   }
   
   ClassDescriptionMaster & cdm = get (class_id);
   
   ClassDescriptionMaster * super_cdm_ptr = cdm.get_super_cdm_ptr ();
   
   bool ret_val = false;
   
   while (super_cdm_ptr != 0)
   {
      int super_class_id = super_cdm_ptr->get_class_id ();
      
      if (super_class_id == supposed_base_class_id)
      {
         ret_val = true;
         break;
      }
      
      super_cdm_ptr = super_cdm_ptr->get_super_cdm_ptr ();
   }
   
   return ret_val;
}



/*
==============================================================================
Name : post_check
==============================================================================
*/

void  ClassDescManager::post_check ()
{
   assert (ClassDescription <Bool>::use ().get_class_id () == BasicTypeId_BOOL);
   assert (ClassDescription <Int64>::use ().get_class_id () == BasicTypeId_INT64);
   assert (ClassDescription <EnumMaster>::use ().get_class_id () == BasicTypeId_ENUM);
   assert (ClassDescription <Float64>::use ().get_class_id () == BasicTypeId_FLOAT64);
   assert (ClassDescription <Blob>::use ().get_class_id () == BasicTypeId_BLOB);
   assert (ClassDescription <ArrayMaster>::use ().get_class_id () == BasicTypeId_ARRAY);
   assert (ClassDescription <CollectionMaster>::use ().get_class_id () == BasicTypeId_COLLECTION);
   assert (ClassDescription <ObjectRefMaster>::use ().get_class_id () == BasicTypeId_OBJECT_REF);
   assert (ClassDescription <Cue>::use ().get_class_id () == BasicTypeId_CUE);
   
   for (size_t i = 0 ; i < _class_id_class_desc_arr.size () ; ++i)
   {
      ClassDescriptionMaster * cdm_ptr = _class_id_class_desc_arr [i];
      
      cdm_ptr->post_check ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ClassDescManager::ClassDescManager ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_declare
==============================================================================
*/

void  ClassDescManager::do_declare (ClassDescriptionMaster & class_description)
{
   class_description.set_class_id (_class_id_class_desc_arr.size () + 1);
   
   _class_id_class_desc_arr.push_back (&class_description);
   
   if (class_description.is_root ())
   {
      _root_cdm_ptr = &class_description;
      _format_version_str = class_description.get_format_version ();
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
