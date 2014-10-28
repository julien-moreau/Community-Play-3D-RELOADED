/*****************************************************************************

        DocumentReader.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64771

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

#include "ohm/flip/private/DocumentReader.h"
#include "ohm/flip/private/ObjectManager.h"
#include "ohm/flip/EnumDescManager.h"
#include "ohm/flip/Float64.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/ObjectRef.h"
#include "ohm/flip/Array.h"
#include "ohm/flip/Collection.h"
#include "ohm/flip/private/DocumentKeywords.h"
#include "ohm/flip/private/EnumDescriptionMaster.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/private/UserManager.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/util/trace.h"

#if ! defined (ohm_flip_SERVER)
   #include "ohm/sys/TimerReliable.h"
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <cassert>

#undef OHM_FLIP_DOCUMENT_READER_DEBUG_FLAG

#if defined (OHM_FLIP_DOCUMENT_READER_DEBUG_FLAG)

   #if defined (ohm_flip_SERVER)
      #error reader will print to stdout : not compatible with server mode
   #endif

   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DOCUMENT_READER_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define dr_break assert (false)
#else
   #define dr_break ((void) 0)
#endif



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

DocumentReader::DocumentReader ()
:  _ds ()
,  _dp (_ds)
,  _tx_data ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentReader::~DocumentReader ()
{
   
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentReader::load (const char * path_0)
{
   int err = _ds.open (path_0);
   if (err != Err_OK) return err;
   
   err = _dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentReader::load_no_copy (const archi::UByte * data_ptr, size_t data_size)
{
   _ds.assign_no_copy (data_ptr, data_size);
   
   int err = _dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentReader::load_no_copy (const std::string & document_str)
{
   _ds.assign_no_copy ((const archi::UByte *) document_str.c_str (), document_str.size ());
   
   int err = _dp.init ();
   
   return err;
}



/*
==============================================================================
Name : get_rev_id
==============================================================================
*/

archi::Int64   DocumentReader::get_rev_id () const
{
   return _dp.get_rev_id ();
}



/*
==============================================================================
Name : get_format_version
==============================================================================
*/

const std::string &  DocumentReader::get_format_version () const
{
   return _dp.get_format_version ();
}



/*
==============================================================================
Name : get_users
==============================================================================
*/

void  DocumentReader::get_users (UserManager & user_manager)
{
   const DocumentParser::UserNameIdMap & user_name_id_map
      = _dp.get_user_name_id_map ();
   
   user_manager.assign (user_name_id_map);
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   DocumentReader::read ()
{
#if ! defined (ohm_flip_SERVER)
   sys::TimerReliable & tr = sys::TimerReliable::use_instance ();

   ohm_util_TRACE_INFO ("DocumentReader::read started");
#endif

#if defined (OHM_FLIP_DOCUMENT_READER_DEBUG_FLAG)
   _dp.print ();
#endif
   
#if ! defined (ohm_flip_SERVER)
   double now = tr.get ();
#endif
   
   flip::Ref root_ref = _dp.get_root_ref ();
   
   if (root_ref == 0)
   {
      return Err_DOC_OBJECT_MISSING;
   }

   int err = read_object_ref (_tx_data, _dp, root_ref);

#if ! defined (ohm_flip_SERVER)  
   ohm_util_TRACE_INFO1 ("DocumentReader::read took %1% ms", (tr.get () - now) * 1000.0);
   
   ohm_util_TRACE_INFO1 ("DocumentReader::read returned %1%", err);
#endif
   
   return err;
}



/*
==============================================================================
Name : exec
Note :
   the root object must already be present in 'document'
==============================================================================
*/

int   DocumentReader::exec (DocumentBase & document)
{
   assert (&document != 0);
   
   int err = Err_OK;
   
   if (!_tx_data.empty ())
   {
      VirtualMachine & virtual_machine = document.use_virtual_machine ();
      
      MachineCode code (&_tx_data [0], _tx_data.size ());
      
      virtual_machine.run ();
      virtual_machine.set_mode_no_fault ();
      virtual_machine.set_mode_direction (1);
      
      err = virtual_machine.compile (code);

      if (err == Err_OK)
      {
         err = virtual_machine.execute (code);
      }

      virtual_machine.stop ();
   }
   
   return err;
}



/*
==============================================================================
Name : read_object_ref
Description :
   Parse recursively the objects and fill '_transaction_sptr'
==============================================================================
*/

int   DocumentReader::read_object_ref (Data & tx_data, DocumentParser & dp, flip::Ref ref)
{
   DocumentParser::Object object;
   int err = dp.seek_object (object, ref);
   if (err != Err_OK) return err;
   
   std::string class_name;
   err = object.read_class_name (class_name);
   if (err != Err_OK) return err;
   
   ClassDescriptionMaster * cdm_ptr = ClassDescManager::use_instance ().find (class_name);
   
   if (cdm_ptr == 0)
   {
      dr_break;
      return Err_DOC_CLASS_NOT_DECLARED;
   }

   bool loop_flag = true;
   
   while (loop_flag)
   {
      err = read_attribute (tx_data, object, ref, cdm_ptr);
      
      if (err == Err_DOC_NO_MORE_ATTRIBUTES)
      {
         loop_flag = false;
      }
      else
      {
         if (err != Err_OK) return err;
      }
   }

   return Err_OK;
}



/*
==============================================================================
Name : read_object_ref_cdm
==============================================================================
*/

int   DocumentReader::read_object_ref_cdm (ClassDescriptionMaster * & cdm_ptr, DocumentParser & dp, flip::Ref ref)
{
   DocumentParser::Object object;
   int err = dp.seek_object (object, ref);
   if (err != Err_OK) return err;
   
   std::string class_name;
   err = object.read_class_name (class_name);
   if (err != Err_OK) return err;
   
   cdm_ptr = ClassDescManager::use_instance ().find (class_name);
   
   if (cdm_ptr == 0)
   {
      dr_break;
      return Err_DOC_CLASS_NOT_DECLARED;
   }

   return Err_OK;
}
   


/*
==============================================================================
Name : read_attribute
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, ClassDescriptionMaster * cdm_ptr)
{
   std::string attribute_name;
   int err = object.read_attribute_name (attribute_name);
   if (err != Err_OK) return err;
   
   const ClassDescriptionMaster::VarInfo * var_info_ptr
      = cdm_ptr->get_var_info (attribute_name);
   
   if (var_info_ptr == 0)
   {
      dr_break;
      return Err_DOC_ATTRIBUTE_NOT_DECLARED;
   }
   
   DocumentParser::Object::Type type;
   err = object.read_type (type);
   if (err != Err_OK) return err;
   
   switch (type)
   {
   case DocumentParser::Object::Type_OBJECT:
      err = read_attribute_object (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_BOOL:
      err = read_attribute_bool (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_INT64:
      err = read_attribute_int64 (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_ENUM:
      err = read_attribute_enum (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_FLOAT64:
      err = read_attribute_float64 (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_BLOB:
      err = read_attribute_blob (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_CUE:
      err = read_attribute_cue (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_ARRAY:
      err = read_attribute_array (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_COLLECTION:
      err = read_attribute_collection (tx_data, object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_OBJECT_REF:
      err = read_attribute_object_ref (tx_data, object, parent_ref, var_info_ptr);
      break;
   
   default:
      err = Err_DOC_INTERNAL_SYSTEM_ERROR;
      break;
   };
   
   return err;
}



/*
==============================================================================
Name : read_attribute_object
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_object (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id < ClassDescManager::NBR_BasicTypeId_TOTAL)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   flip::Ref sub_ref = 0;
   int err = object.read_ref (sub_ref);
   if (err != Err_OK) return err;
   
   if (sub_ref != parent_ref + var_info_ptr->_var_offset)
   {
      dr_break;
      return Err_DOC_BAD_REFERENCE;
   }
   
   DocumentParser & dp = object.get_document_parser ();
   
   err = read_object_ref (tx_data, dp, sub_ref);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_bool
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_bool (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_BOOL)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   bool val = 0;
   int err = object.read_bool (val);
   if (err != Err_OK) return err;
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (val != false)
   {
      MachineCode::build_bool_set (tx_data, this_ref, false, val);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_int64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_int64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_INT64)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   archi::Int64 val = 0;
   int err = object.read_int64 (val);
   if (err != Err_OK) return err;
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (val != 0LL)
   {
      MachineCode::build_int64_set (tx_data, this_ref, 0LL, val);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_enum
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_enum (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_ENUM)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   std::string enum_name;
   int err = object.read_sub_enum_name (enum_name);
   if (err != Err_OK) return err;
   
   EnumDescriptionMaster * edm_ptr = EnumDescManager::use_instance ().find (enum_name);
   if (edm_ptr == 0)
   {
      dr_break;
      return Err_DOC_UNEXPECTED_TYPE;
   }
   
   archi::Int64 val = 0;
   err = object.read_enum (val, *edm_ptr);
   if (err != Err_OK) return err;
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (val != edm_ptr->get_min ())
   {
      MachineCode::build_enum_set (tx_data, this_ref, edm_ptr->get_min (), val);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_float64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_float64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_FLOAT64)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   archi::Float64 val = 0;
   int err = object.read_float64 (val);
   if (err != Err_OK) return err;
      
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (val != 0.0)
   {
      MachineCode::build_float64_set (tx_data, this_ref, 0.0, val);
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_blob
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_blob (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_BLOB)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   flip::Ref sub_ref = 0;
   bool empty_flag = false;
   bool inline_flag = false;
   std::vector <Codec::Encoder> codec_arr;
   archi::UByte * data_ptr = 0;
   size_t data_length = 0;
   
   int err = object.read_blob (inline_flag, empty_flag, sub_ref, codec_arr, data_ptr, data_length);
   if (err != Err_OK) return err;
   
   if (empty_flag)
   {
      return Err_OK;
   }
   
   if (inline_flag)
   {
      if ((data_ptr == 0) || (data_length == 0))
      {
         return Err_DOC_INTERNAL_SYSTEM_ERROR;
      }
      
      flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;

      if (codec_arr.size () > 0)
      {
         std::vector <archi::UByte> data;
         
         Codec::decode (data, data_ptr, data_length, codec_arr);
         
         if (data.size () > 0)
         {
            MachineCode::build_blob_set (
               tx_data, this_ref,
               0, 0, &data [0], data.size ()
            );
         }
      }
      else
      {
         MachineCode::build_blob_set (
            tx_data, this_ref,
            0, 0, data_ptr, data_length
         );
      }
   }
   else
   {
      if (sub_ref != parent_ref + var_info_ptr->_var_offset)
      {
         dr_break;
         return Err_DOC_BAD_REFERENCE;
      }
   
      DocumentParser & dp = object.get_document_parser ();
      
      err = read_blob_ref (tx_data, dp, sub_ref);
      if (err != Err_OK) return err;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_signal
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_cue (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_CUE)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_array
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_array (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_ARRAY)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   DocumentParser::Object::Type sub_type = DocumentParser::Object::Type_VOID;
   int err = object.read_sub_type (sub_type);
   if (err != Err_OK) return err;
   
   switch (sub_type)
   {
   case DocumentParser::Object::Type_OBJECT:
      err = read_attribute_array_object (tx_data, object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_INT64:
      err = read_attribute_array_int64 (tx_data, object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_FLOAT64:
      err = read_attribute_array_float64 (tx_data, object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;
   
   default:
      dr_break;
      return Err_DOC_UNEXPECTED_TYPE;
      break;
   }
      
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_array_object
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_array_object (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      BinaryKey key;
      int err = object.read_binary_key (key);
      
      if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
      {
         loop_flag = false;
      }
      else if (err != Err_OK)
      {
         return err;
      }
      
      if (loop_flag)
      {
         flip::Ref ref = 0;
         err = object.read_ref (ref);
         if (err != Err_OK) return err;
         
         if (ref == 0)
         {
            dr_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         DocumentParser & dp = object.get_document_parser ();
         
         ClassDescriptionMaster * cdm_ptr = 0;
            
         err = read_object_ref_cdm (cdm_ptr, dp, ref);
         if (err != Err_OK) return err;
         
         MachineCode::build_array_insert (
            tx_data,
            this_ref, key, ref, cdm_ptr->get_class_id ()
         );
         
         err = read_object_ref (tx_data, dp, ref);
         if (err != Err_OK) return err;
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_array_int64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_array_int64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      BinaryKey key;
      int err = object.read_binary_key (key);
      
      if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
      {
         loop_flag = false;
      }
      else if (err != Err_OK)
      {
         return err;
      }
      
      if (loop_flag)
      {
         flip::Ref ref = 0;
         err = object.read_ref (ref);
         if (err != Err_OK) return err;
         
         if (ref == 0)
         {
            dr_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         MachineCode::build_array_insert (
            tx_data,
            this_ref, key, ref, ClassDescManager::BasicTypeId_INT64
         );
         
         archi::Int64 val = 0LL;
         err = object.read_int64 (val);
         if (err != Err_OK) return err;
         
         if (val != 0LL)
         {
            MachineCode::build_int64_set (
               tx_data,
               ref,
               0LL,
               val
            );
         }
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_array_float64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_array_float64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      BinaryKey key;
      int err = object.read_binary_key (key);
      
      if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
      {
         loop_flag = false;
      }
      else if (err != Err_OK)
      {
         return err;
      }
      
      if (loop_flag)
      {
         flip::Ref ref = 0;
         err = object.read_ref (ref);
         if (err != Err_OK) return err;
         
         if (ref == 0)
         {
            dr_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         MachineCode::build_array_insert (
            tx_data,
            this_ref, key, ref, ClassDescManager::BasicTypeId_FLOAT64
         );
         
         archi::Float64 val = 0.0;
         err = object.read_float64 (val);
         if (err != Err_OK) return err;
         
         if (val != 0.0)
         {
            MachineCode::build_float64_set (
               tx_data,
               ref,
               0.0,
               val
            );
         }
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_collection
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_collection (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_COLLECTION)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }

   DocumentParser::Object::Type sub_type = DocumentParser::Object::Type_VOID;
   int err = object.read_sub_type (sub_type);
   if (err != Err_OK) return err;
   
   switch (sub_type)
   {
   case DocumentParser::Object::Type_OBJECT:
      err = read_attribute_collection_object (tx_data, object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_INT64:
      err = read_attribute_collection_int64 (tx_data, object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_FLOAT64:
      err = read_attribute_collection_float64 (tx_data, object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;
   
   default:
      dr_break;
      return Err_DOC_UNEXPECTED_TYPE;
      break;
   }
      
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_collection_object
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_collection_object (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      flip::Ref ref = 0;
      int err = object.read_ref (ref);

      if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
      {
         loop_flag = false;
      }
      else if (err != Err_OK)
      {
         return err;
      }
      
      if (loop_flag)
      {
         if (ref == 0)
         {
            dr_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         DocumentParser & dp = object.get_document_parser ();
         
         ClassDescriptionMaster * cdm_ptr = 0;
         
         err = read_object_ref_cdm (cdm_ptr, dp, ref);
         if (err != Err_OK) return err;
         
         MachineCode::build_collection_insert (
            tx_data,
            this_ref, ref, cdm_ptr->get_class_id ()
         );
         
         err = read_object_ref (tx_data, dp, ref);
         if (err != Err_OK) return err;
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_collection_int64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_collection_int64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      flip::Ref ref = 0;
      int err = object.read_ref (ref);

      if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
      {
         loop_flag = false;
      }
      else if (err != Err_OK)
      {
         return err;
      }
      
      if (loop_flag)
      {
         if (ref == 0)
         {
            dr_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         MachineCode::build_collection_insert (
            tx_data,
            this_ref, ref, ClassDescManager::BasicTypeId_INT64
         );
         
         archi::Int64 val = 0LL;
         int err = object.read_int64 (val);
         if (err != Err_OK) return err;

         if (val != 0LL)
         {
            MachineCode::build_int64_set (
               tx_data,
               ref,
               0LL,
               val
            );
         }
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_collection_float64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_collection_float64 (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      flip::Ref ref = 0;
      int err = object.read_ref (ref);

      if (err == Err_DOC_ITERATION_NO_MORE_ITEMS)
      {
         loop_flag = false;
      }
      else if (err != Err_OK)
      {
         return err;
      }
      
      if (loop_flag)
      {
         if (ref == 0)
         {
            dr_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         MachineCode::build_collection_insert (
            tx_data,
            this_ref, ref, ClassDescManager::BasicTypeId_FLOAT64
         );
         
         archi::Float64 val = 0.0;
         int err = object.read_float64 (val);
         if (err != Err_OK) return err;

         if (val != 0.0)
         {
            MachineCode::build_float64_set (
               tx_data,
               ref,
               0.0,
               val
            );
         }
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : read_attribute_object_ref
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_attribute_object_ref (Data & tx_data, DocumentParser::Object & object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_OBJECT_REF)
   {
      dr_break;
      return Err_DOC_TYPE_MISMATCH;
   }

   flip::Ref pointed_ref = 0;
   int err = object.read_ref (pointed_ref);
   if (err != Err_OK) return err;
      
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (pointed_ref != 0)
   {
      MachineCode::build_object_ref_set (tx_data, this_ref, 0, pointed_ref);
   }

   return Err_OK;
}


   
/*
==============================================================================
Name : read_blob_data_ref
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_blob_data_ref (std::vector <archi::UByte> & data, DocumentParser & dp, flip::Ref ref)
{
   int err = dp.read_blob (data, ref);
   if (err != Err_OK) return err;
      
   return Err_OK;
}



/*
==============================================================================
Name : read_blob_ref
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentReader::read_blob_ref (Data & tx_data, DocumentParser & dp, flip::Ref ref)
{
   std::vector <archi::UByte> data;
   int err = dp.read_blob (data, ref);
   if (err != Err_OK) return err;

   if (data.size () > 0)
   {
      MachineCode::build_blob_set (
         tx_data, ref,
         0, 0, &data [0], data.size ()
      );
   }  
      
   return Err_OK;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#undef dr_break



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
