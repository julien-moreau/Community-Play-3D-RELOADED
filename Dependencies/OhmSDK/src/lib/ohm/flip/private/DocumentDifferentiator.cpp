/*****************************************************************************

        DocumentDifferentiator.cpp
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

#include "ohm/flip/private/DocumentDifferentiator.h"
#include "ohm/flip/private/ObjectManager.h"
#include "ohm/flip/Float64.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/ObjectRef.h"
#include "ohm/flip/Array.h"
#include "ohm/flip/Collection.h"
#include "ohm/flip/EnumDescManager.h"
#include "ohm/flip/private/DocumentKeywords.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/private/DocumentReader.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/util/trace.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <cassert>

#undef OHM_FLIP_DOCUMENT_DIFFERENTIATOR_DEBUG_FLAG

#if defined (OHM_FLIP_DOCUMENT_DIFFERENTIATOR_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_DOCUMENT_DIFFERENTIATOR_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define dd_break assert (false)
   #define dd_TRACE(msg_0) ohm_util_TRACE_INFO(msg_0)
   #define dd_TRACE1(fmt_0, arg1)   ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define dd_TRACE2(fmt_0, arg1, arg2)   ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define dd_TRACE3(fmt_0, arg1, arg2, arg3)   ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define dd_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)

#else
   #define dd_break ((void) 0)
   #define dd_TRACE(msg_0) (void (0))
   #define dd_TRACE1(fmt_0, arg1)   (void (0))
   #define dd_TRACE2(fmt_0, arg1, arg2)   (void (0))
   #define dd_TRACE3(fmt_0, arg1, arg2, arg3)   (void (0))
   #define dd_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   (void (0))

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

DocumentDifferentiator::DocumentDifferentiator ()
:  _src_ds ()
,  _src_dp (_src_ds)
,  _dst_ds ()
,  _dst_dp (_dst_ds)
,  _tx_data_sptr ()
,  _excluded_ref_set ()
,  _exclude_cnt (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentDifferentiator::~DocumentDifferentiator ()
{
   
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentDifferentiator::load_src (const char * path_0)
{
   int err = _src_ds.open (path_0);
   if (err != Err_OK) return err;
   
   err = _src_dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentDifferentiator::load_src_no_copy (const archi::UByte * data_ptr, int data_size)
{
   _src_ds.assign_no_copy (data_ptr, data_size);
   
   int err = _src_dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentDifferentiator::load_src_no_copy (const std::string & document_str)
{
   _src_ds.assign_no_copy ((const archi::UByte *) document_str.c_str (), document_str.size ());
   
   int err = _src_dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentDifferentiator::load_dst (const char * path_0)
{
   int err = _dst_ds.open (path_0);
   if (err != Err_OK) return err;
   
   err = _dst_dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentDifferentiator::load_dst_no_copy (const archi::UByte * data_ptr, int data_size)
{
   _dst_ds.assign_no_copy (data_ptr, data_size);
   
   int err = _dst_dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
==============================================================================
*/

int   DocumentDifferentiator::load_dst_no_copy (const std::string & document_str)
{
   _dst_ds.assign_no_copy ((const archi::UByte *) document_str.c_str (), document_str.size ());
   
   int err = _dst_dp.init ();
   
   return err;
}



/*
==============================================================================
Name : load
Description :
   Exclude a flip object and its subtree from the differentation.
   (it will keep source and ignore destination)
==============================================================================
*/

void  DocumentDifferentiator::exclude (Object & object)
{
   assert (_exclude_cnt == 0);
   
   _excluded_ref_set.insert (object.get_ref ());
}



/*
==============================================================================
Name : differentiate
==============================================================================
*/

int   DocumentDifferentiator::differentiate ()
{
   dd_TRACE ("DIF");
   
   _tx_data_sptr = DataSPtr (new Data);
   _exclude_cnt = 0;
   
   flip::Ref src_root_ref = _src_dp.get_root_ref ();
   flip::Ref dst_root_ref = _dst_dp.get_root_ref ();
   
   if ((src_root_ref == 0) || (dst_root_ref == 0))
   {
      return Err_DOC_OBJECT_MISSING;
   }
   
   if (src_root_ref != dst_root_ref)
   {
      return Err_DIF_NOT_COALESCABLE;
   }
   
   // get actual ref for src & dst
   
   dd_TRACE ("DIF SRC");
   
   {
      Data tx_data;
      
      DocumentReader::read_object_ref (tx_data, _src_dp, src_root_ref);
      
      if (!tx_data.empty ())
      {
         MachineCode code (&tx_data [0], tx_data.size ());
      
         code.set_mode_no_fault ();
         int err = code.compile ();
         assert (err == Err_OK);

         code.get_container_elem (_src_container_ref_map);
      }
   }

   dd_TRACE ("DIF DST");
   
   {
      Data tx_data;

      DocumentReader::read_object_ref (tx_data, _dst_dp, dst_root_ref);
      
      if (!tx_data.empty ())
      {
         MachineCode code (&tx_data [0], tx_data.size ());
      
         code.set_mode_no_fault ();
         int err = code.compile ();
         assert (err == Err_OK);
         
         code.get_container_elem (_dst_container_ref_map);
      }
   }
   
   dd_TRACE ("DIF DIF");
   
   int err = differentiate_object_ref (src_root_ref);
   
   assert (_exclude_cnt == 0);
   
   return err;
}



/*
==============================================================================
Name : get_tx_data
==============================================================================
*/

DataSPtr DocumentDifferentiator::get_tx_data ()
{
   return _tx_data_sptr;
}



/*
==============================================================================
Name : exec
==============================================================================
*/

int   DocumentDifferentiator::exec (DocumentBase & document)
{
   dd_TRACE ("DIF EXEC");
   
   assert (&document != 0);
   
   int err = Err_OK;
   
   if (!_tx_data_sptr->empty ())
   {
      VirtualMachine & virtual_machine = document.use_virtual_machine ();
      
      Data & tx_data = *_tx_data_sptr;
      
      MachineCode code (&tx_data [0], tx_data.size ());
      
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



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : differentiate_object_ref
Description :
   Parse recursively the objects and fill '_transaction_sptr'
==============================================================================
*/

int   DocumentDifferentiator::differentiate_object_ref (flip::Ref ref)
{
   DocumentParser::Object src_object;
   int err = _src_dp.seek_object (src_object, ref);
   if (err != Err_OK) return err;

   DocumentParser::Object dst_object;
   err = _dst_dp.seek_object (dst_object, ref);
   if (err != Err_OK) return err;
   
   std::string src_class_name;
   err = src_object.read_class_name (src_class_name);
   if (err != Err_OK) return err;

   std::string dst_class_name;
   err = dst_object.read_class_name (dst_class_name);
   if (err != Err_OK) return err;
   
   if (src_class_name != dst_class_name)
   {
      dd_break;
      return Err_DIF_NOT_COALESCABLE;
   }
   
   ClassDescriptionMaster * cdm_ptr = ClassDescManager::use_instance ().find (src_class_name);
   
   if (cdm_ptr == 0)
   {
      dd_break;
      return Err_DOC_CLASS_NOT_DECLARED;
   }

   bool loop_flag = true;
   
   bool exclude_flag = _excluded_ref_set.find (ref) != _excluded_ref_set.end ();
   
   if (exclude_flag)
   {
      ++_exclude_cnt;
   }
   
   while (loop_flag)
   {
      err = differentiate_attribute (src_object, dst_object, ref, cdm_ptr);
      
      if (err == Err_DOC_NO_MORE_ATTRIBUTES)
      {
         loop_flag = false;
      }
      else
      {
         if (err != Err_OK) return err;
      }
   }
   
   if (exclude_flag)
   {
      assert (_exclude_cnt > 0);
      --_exclude_cnt;
   }

   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, ClassDescriptionMaster * cdm_ptr)
{
   std::string src_attribute_name;
   int err = src_object.read_attribute_name (src_attribute_name);
   if (err != Err_OK) return err;

   std::string dst_attribute_name;
   err = dst_object.read_attribute_name (dst_attribute_name);
   if (err != Err_OK) return err;
   
   if (src_attribute_name != dst_attribute_name)
   {
      dd_break;
      return Err_DIF_NOT_COALESCABLE;
   }
   
   const ClassDescriptionMaster::VarInfo * var_info_ptr
      = cdm_ptr->get_var_info (src_attribute_name);
   
   if (var_info_ptr == 0)
   {
      dd_break;
      return Err_DOC_ATTRIBUTE_NOT_DECLARED;
   }
   
   DocumentParser::Object::Type src_type;
   err = src_object.read_type (src_type);
   if (err != Err_OK) return err;

   DocumentParser::Object::Type dst_type;
   err = dst_object.read_type (dst_type);
   if (err != Err_OK) return err;
   
   if (src_type != dst_type)
   {
      dd_break;
      return Err_DIF_NOT_COALESCABLE;
   }
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   bool exclude_flag = _excluded_ref_set.find (this_ref) != _excluded_ref_set.end ();
   
   if (exclude_flag)
   {
      ++_exclude_cnt;
   }
   
   switch (src_type)
   {
   case DocumentParser::Object::Type_OBJECT:
      err = differentiate_attribute_object (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_BOOL:
      err = differentiate_attribute_bool (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_INT64:
      err = differentiate_attribute_int64 (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_ENUM:
      err = differentiate_attribute_enum (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_FLOAT64:
      err = differentiate_attribute_float64 (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_BLOB:
      err = differentiate_attribute_blob (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_CUE:
      err = differentiate_attribute_cue (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_ARRAY:
      err = differentiate_attribute_array (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_COLLECTION:
      err = differentiate_attribute_collection (src_object, dst_object, parent_ref, var_info_ptr);
      break;

   case DocumentParser::Object::Type_OBJECT_REF:
      err = differentiate_attribute_object_ref (src_object, dst_object, parent_ref, var_info_ptr);
      break;
   
   default:
      err = Err_DOC_INTERNAL_SYSTEM_ERROR;
      break;
   };
   
   if (exclude_flag)
   {
      assert (_exclude_cnt > 0);
      --_exclude_cnt;
   }
   
   return err;
}



/*
==============================================================================
Name : differentiate_attribute_object
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_object (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id < ClassDescManager::NBR_BasicTypeId_TOTAL)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   flip::Ref src_sub_ref = 0;
   int err = src_object.read_ref (src_sub_ref);
   if (err != Err_OK) return err;

   if (src_sub_ref != parent_ref + var_info_ptr->_var_offset)
   {
      dd_break;
      return Err_DOC_BAD_REFERENCE;
   }
   
   flip::Ref dst_sub_ref = 0;
   err = dst_object.read_ref (dst_sub_ref);
   if (err != Err_OK) return err;
   
   if (dst_sub_ref != parent_ref + var_info_ptr->_var_offset)
   {
      dd_break;
      return Err_DOC_BAD_REFERENCE;
   }
   
   if (src_sub_ref != dst_sub_ref)
   {
      dd_break;
      return Err_DOC_INTERNAL_SYSTEM_ERROR;
   }
   
   err = differentiate_object_ref (src_sub_ref);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_bool
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_bool (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_BOOL)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   bool src_val = false;
   int err = src_object.read_bool (src_val);
   if (err != Err_OK) return err;

   bool dst_val = false;
   err = dst_object.read_bool (dst_val);
   if (err != Err_OK) return err;
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (src_val != dst_val)
   {
      if (_exclude_cnt == 0)
      {
         MachineCode::build_bool_set (*_tx_data_sptr, this_ref, src_val, dst_val);
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_int64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_int64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_INT64)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   archi::Int64 src_val = 0LL;
   int err = src_object.read_int64 (src_val);
   if (err != Err_OK) return err;

   archi::Int64 dst_val = 0LL;
   err = dst_object.read_int64 (dst_val);
   if (err != Err_OK) return err;
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (src_val != dst_val)
   {
      if (_exclude_cnt == 0)
      {
         MachineCode::build_int64_set (*_tx_data_sptr, this_ref, src_val, dst_val);
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_enum
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_enum (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_ENUM)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   std::string src_enum_name;
   int err = src_object.read_sub_enum_name (src_enum_name);
   if (err != Err_OK) return err;

   EnumDescriptionMaster * src_edm_ptr = EnumDescManager::use_instance ().find (src_enum_name);
   if (src_edm_ptr == 0)
   {
      dd_break;
      return Err_DOC_UNEXPECTED_TYPE;
   }
   
   std::string dst_enum_name;
   err = dst_object.read_sub_enum_name (dst_enum_name);
   if (err != Err_OK) return err;
   
   EnumDescriptionMaster * dst_edm_ptr = EnumDescManager::use_instance ().find (dst_enum_name);
   if (dst_edm_ptr == 0)
   {
      dd_break;
      return Err_DOC_UNEXPECTED_TYPE;
   }
   
   if (src_edm_ptr != dst_edm_ptr)
   {
      dd_break;
      return Err_DIF_NOT_COALESCABLE;
   }
   
   assert (src_edm_ptr != 0);
   EnumDescriptionMaster & edm = *src_edm_ptr;
   
   archi::Int64 src_val = 0LL;
   err = src_object.read_enum (src_val, edm);
   if (err != Err_OK) return err;

   archi::Int64 dst_val = 0LL;
   err = dst_object.read_enum (dst_val, edm);
   if (err != Err_OK) return err;
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (src_val != dst_val)
   {
      if (_exclude_cnt == 0)
      {
         MachineCode::build_enum_set (*_tx_data_sptr, this_ref, src_val, dst_val);
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_float64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_float64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_FLOAT64)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   archi::Float64 src_val = 0.0;
   int err = src_object.read_float64 (src_val);
   if (err != Err_OK) return err;

   archi::Float64 dst_val = 0.0;
   err = dst_object.read_float64 (dst_val);
   if (err != Err_OK) return err;
   
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (src_val != dst_val)
   {
      if (_exclude_cnt == 0)
      {
         MachineCode::build_float64_set (*_tx_data_sptr, this_ref, src_val, dst_val);
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_blob
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_blob (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_BLOB)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   flip::Ref src_sub_ref = 0;
   int err = src_object.read_ref (src_sub_ref);
   
   bool src_empty_flag = false;
   
   if (err == Err_DOC_BLOB_EMPTY)
   {
      src_empty_flag = true;
   }
   else if (err != Err_OK) return err;
   
   if (!src_empty_flag)
   {
      if (src_sub_ref != parent_ref + var_info_ptr->_var_offset)
      {
         dd_break;
         return Err_DOC_BAD_REFERENCE;
      }
   }

   flip::Ref dst_sub_ref = 0;
   err = dst_object.read_ref (dst_sub_ref);
   
   bool dst_empty_flag = false;
   
   if (err == Err_DOC_BLOB_EMPTY)
   {
      dst_empty_flag = true;
   }
   else if (err != Err_OK) return err;
   
   if (!dst_empty_flag)
   {
      if (dst_sub_ref != parent_ref + var_info_ptr->_var_offset)
      {
         dd_break;
         return Err_DOC_BAD_REFERENCE;
      }
   }
   
   if ((!src_empty_flag) && (!dst_empty_flag))
   {
      if (src_sub_ref != dst_sub_ref)
      {
         dd_break;
         return Err_DOC_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   std::vector <archi::UByte> src_data;
   
   if (!src_empty_flag)
   {
      err = DocumentReader::read_blob_data_ref (src_data, _src_dp, src_sub_ref);
      if (err != Err_OK) return err;
   }

   std::vector <archi::UByte> dst_data;
   
   if (!dst_empty_flag)
   {
      err = DocumentReader::read_blob_data_ref (dst_data, _dst_dp, dst_sub_ref);
      if (err != Err_OK) return err;
   }
   
   if (src_data != dst_data)
   {
      size_t src_size = src_data.size ();
      archi::UByte * src_data_ptr = 0;
      if (src_size)
      {
         src_data_ptr = &src_data [0];
      }
      
      size_t dst_size = dst_data.size ();
      archi::UByte * dst_data_ptr = 0;
      if (dst_size)
      {
         dst_data_ptr = &dst_data [0];
      }
      
      if (_exclude_cnt == 0)
      {
         MachineCode::build_blob_set (
            *_tx_data_sptr, src_sub_ref,
            src_data_ptr, src_size,
            dst_data_ptr, dst_size
         );
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_cue
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_cue (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_CUE)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
      
   // nothing to differentiate
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_array
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_array (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_ARRAY)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   DocumentParser::Object::Type src_sub_type = DocumentParser::Object::Type_VOID;
   int err = src_object.read_sub_type (src_sub_type);
   if (err != Err_OK) return err;

   DocumentParser::Object::Type dst_sub_type = DocumentParser::Object::Type_VOID;
   err = dst_object.read_sub_type (dst_sub_type);
   if (err != Err_OK) return err;
   
   if (src_sub_type != dst_sub_type)
   {
      dd_break;
      return Err_DIF_NOT_COALESCABLE;
   }
   
   switch (src_sub_type)
   {
   case DocumentParser::Object::Type_OBJECT:
      err = differentiate_attribute_array_object (src_object, dst_object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_INT64:
      err = differentiate_attribute_array_int64 (src_object, dst_object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_FLOAT64:
      err = differentiate_attribute_array_float64 (src_object, dst_object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;
   
   default:
      dd_break;
      return Err_DOC_UNEXPECTED_TYPE;
      break;
   }
      
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_array_object
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_array_object (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   ContainerObjectInfoList src_info_list;
   int err = read_attribute_array_object (src_info_list, src_object);
   if (err != Err_OK) return err;
   
   ContainerObjectInfoList dst_info_list;
   err = read_attribute_array_object (dst_info_list, dst_object);
   if (err != Err_OK) return err;
   
   ContainerObjectItPairList it_pair_list;
   
   // merge ordered among keys
   
   merge_ordered (it_pair_list, src_info_list, dst_info_list);
   
   // differentiate
   
   ActorArrayObject actor (*this, this_ref);

   err = differentiate <ActorArrayObject> (actor, it_pair_list, src_info_list, dst_info_list);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_array_int64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_array_int64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   ContainerObjectInfoList src_info_list;
   int err = read_attribute_array_int64 (src_info_list, src_object);
   if (err != Err_OK) return err;
   
   ContainerObjectInfoList dst_info_list;
   err = read_attribute_array_int64 (dst_info_list, dst_object);
   if (err != Err_OK) return err;
   
   ContainerObjectItPairList it_pair_list;
   
   // merge ordered among keys
   
   merge_ordered (it_pair_list, src_info_list, dst_info_list);
   
   // differentiate
   
   ActorArrayInt64 actor (*this, this_ref);

   err = differentiate <ActorArrayInt64> (actor, it_pair_list, src_info_list, dst_info_list);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_array_float64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_array_float64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   ContainerObjectInfoList src_info_list;
   int err = read_attribute_array_float64 (src_info_list, src_object);
   if (err != Err_OK) return err;
   
   ContainerObjectInfoList dst_info_list;
   err = read_attribute_array_float64 (dst_info_list, dst_object);
   if (err != Err_OK) return err;
   
   ContainerObjectItPairList it_pair_list;
   
   // merge ordered among keys
   
   merge_ordered (it_pair_list, src_info_list, dst_info_list);
   
   // differentiate
   
   ActorArrayFloat64 actor (*this, this_ref);

   err = differentiate <ActorArrayFloat64> (actor, it_pair_list, src_info_list, dst_info_list);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_collection
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_collection (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_COLLECTION)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }
   
   DocumentParser::Object::Type src_sub_type = DocumentParser::Object::Type_VOID;
   int err = src_object.read_sub_type (src_sub_type);
   if (err != Err_OK) return err;

   DocumentParser::Object::Type dst_sub_type = DocumentParser::Object::Type_VOID;
   err = dst_object.read_sub_type (dst_sub_type);
   if (err != Err_OK) return err;
   
   if (src_sub_type != dst_sub_type)
   {
      dd_break;
      return Err_DIF_NOT_COALESCABLE;
   }
   
   switch (src_sub_type)
   {
   case DocumentParser::Object::Type_OBJECT:
      err = differentiate_attribute_collection_object (src_object, dst_object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_INT64:
      err = differentiate_attribute_collection_int64 (src_object, dst_object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;

   case DocumentParser::Object::Type_FLOAT64:
      err = differentiate_attribute_collection_float64 (src_object, dst_object, parent_ref, var_info_ptr);
      if (err != Err_OK) return err;
      break;
   
   default:
      dd_break;
      return Err_DOC_UNEXPECTED_TYPE;
      break;
   }
      
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_collection_object
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_collection_object (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   ContainerObjectInfoList src_info_list;
   int err = read_attribute_collection_object (src_info_list, src_object);
   if (err != Err_OK) return err;
   
   ContainerObjectInfoList dst_info_list;
   err = read_attribute_collection_object (dst_info_list, dst_object);
   if (err != Err_OK) return err;
   
   ContainerObjectItPairList it_pair_list;
   
   // merge ordered among keys
   
   merge_ordered (it_pair_list, src_info_list, dst_info_list);
   
   // differentiate
   
   ActorCollectionObject actor (*this, this_ref);

   err = differentiate <ActorCollectionObject> (actor, it_pair_list, src_info_list, dst_info_list);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_collection_int64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_collection_int64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   ContainerObjectInfoList src_info_list;
   int err = read_attribute_collection_int64 (src_info_list, src_object);
   if (err != Err_OK) return err;
   
   ContainerObjectInfoList dst_info_list;
   err = read_attribute_collection_int64 (dst_info_list, dst_object);
   if (err != Err_OK) return err;
   
   ContainerObjectItPairList it_pair_list;
   
   // merge ordered among keys
   
   merge_ordered (it_pair_list, src_info_list, dst_info_list);
   
   // differentiate
   
   ActorCollectionInt64 actor (*this, this_ref);

   err = differentiate <ActorCollectionInt64> (actor, it_pair_list, src_info_list, dst_info_list);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_collection_float64
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_collection_float64 (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   ContainerObjectInfoList src_info_list;
   int err = read_attribute_collection_float64 (src_info_list, src_object);
   if (err != Err_OK) return err;
   
   ContainerObjectInfoList dst_info_list;
   err = read_attribute_collection_float64 (dst_info_list, dst_object);
   if (err != Err_OK) return err;
   
   ContainerObjectItPairList it_pair_list;
   
   // merge ordered among keys
   
   merge_ordered (it_pair_list, src_info_list, dst_info_list);
   
   // differentiate
   
   ActorCollectionFloat64 actor (*this, this_ref);

   err = differentiate <ActorCollectionFloat64> (actor, it_pair_list, src_info_list, dst_info_list);
   if (err != Err_OK) return err;
   
   return Err_OK;
}



/*
==============================================================================
Name : differentiate_attribute_object_ref
Description :
   Read an attribute of an object
==============================================================================
*/

int   DocumentDifferentiator::differentiate_attribute_object_ref (DocumentParser::Object & src_object, DocumentParser::Object & dst_object, flip::Ref parent_ref, const ClassDescriptionMaster::VarInfo * var_info_ptr)
{
   if (var_info_ptr->_type_id != ClassDescManager::BasicTypeId_OBJECT_REF)
   {
      dd_break;
      return Err_DOC_TYPE_MISMATCH;
   }

   flip::Ref src_pointed_ref = 0;
   int err = src_object.read_ref (src_pointed_ref);
   if (err != Err_OK) return err;
      
   flip::Ref dst_pointed_ref = 0;
   err = dst_object.read_ref (dst_pointed_ref);
   if (err != Err_OK) return err;
      
   flip::Ref this_ref = parent_ref + var_info_ptr->_var_offset;
   
   if (src_pointed_ref != dst_pointed_ref)
   {
      if (_exclude_cnt == 0)
      {
         MachineCode::build_object_ref_set (
            *_tx_data_sptr, this_ref,
            src_pointed_ref, dst_pointed_ref
         );
      }
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

int   DocumentDifferentiator::read_attribute_array_object (ContainerObjectInfoList & info_list, DocumentParser::Object & object)
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      ContainerObjectInfo info;
      info._array_flag = true;
      
      int err = object.read_binary_key (info._key);
      
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
         err = object.read_ref (info._ref);
         if (err != Err_OK) return err;
         
         if (info._ref == 0)
         {
            dd_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         info_list.push_back (info);
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

int   DocumentDifferentiator::read_attribute_array_int64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object)
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      ContainerObjectInfo info;
      info._array_flag = true;
      
      int err = object.read_binary_key (info._key);
      
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
         err = object.read_ref (info._ref);
         if (err != Err_OK) return err;
         
         err = object.read_int64 (info._val_i64);
         if (err != Err_OK) return err;
         
         if (info._ref == 0)
         {
            dd_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         info_list.push_back (info);
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

int   DocumentDifferentiator::read_attribute_array_float64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object)
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      ContainerObjectInfo info;
      info._array_flag = true;
      
      int err = object.read_binary_key (info._key);
      
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
         err = object.read_ref (info._ref);
         if (err != Err_OK) return err;

         err = object.read_float64 (info._val_f64);
         if (err != Err_OK) return err;
         
         if (info._ref == 0)
         {
            dd_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         info_list.push_back (info);
      }
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

int   DocumentDifferentiator::read_attribute_collection_object (ContainerObjectInfoList & info_list, DocumentParser::Object & object)
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      ContainerObjectInfo info;
      info._array_flag = false;
      
      int err = object.read_ref (info._ref);
      
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
         if (info._ref == 0)
         {
            dd_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         info_list.push_back (info);
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

int   DocumentDifferentiator::read_attribute_collection_int64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object)
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      ContainerObjectInfo info;
      info._array_flag = false;
      
      int err = object.read_ref (info._ref);
      
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
         err = object.read_int64 (info._val_i64);
         if (err != Err_OK) return err;
         
         if (info._ref == 0)
         {
            dd_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         info_list.push_back (info);
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

int   DocumentDifferentiator::read_attribute_collection_float64 (ContainerObjectInfoList & info_list, DocumentParser::Object & object)
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      ContainerObjectInfo info;
      info._array_flag = false;
      
      int err = object.read_ref (info._ref);
      
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
         err = object.read_float64 (info._val_f64);
         if (err != Err_OK) return err;
         
         if (info._ref == 0)
         {
            dd_break;
            return Err_DOC_BAD_REFERENCE;
         }
         
         info_list.push_back (info);
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : merge_ordered
==============================================================================
*/

void  DocumentDifferentiator::merge_ordered (ContainerObjectItPairList & it_pair_list, ContainerObjectInfoList & src_info_list, ContainerObjectInfoList & dst_info_list)
{
   ContainerObjectInfoList::iterator src_it = src_info_list.begin ();
   const ContainerObjectInfoList::iterator src_it_end = src_info_list.end ();

   ContainerObjectInfoList::iterator dst_it = dst_info_list.begin ();
   const ContainerObjectInfoList::iterator dst_it_end = dst_info_list.end ();
   
   for (;;)
   {
      bool equal_flag = false;
      bool src_first_flag = false;
      bool dst_first_flag = false;
      
      if (src_it != src_it_end)
      {
         if (dst_it != dst_it_end)
         {
            equal_flag = src_it->is_equal (*dst_it);
            
            if (!equal_flag)
            {
               src_first_flag = src_it->is_less (*dst_it);
               
               if (!src_first_flag)
               {
                  dst_first_flag = true;
               }
            }
         }
         else
         {
            src_first_flag = true;
         }
      }
      else
      {
         if (dst_it != dst_it_end)
         {
            dst_first_flag = true;
         }
         else
         {
            // stop
         }
      }
      
      if (equal_flag)
      {
         it_pair_list.push_back (std::make_pair (src_it, dst_it));
         
         ++src_it;
         ++dst_it;
      }
      else if (src_first_flag)
      {
         it_pair_list.push_back (std::make_pair (src_it, dst_it_end));
         
         ++src_it;
      }
      else if (dst_first_flag)
      {
         it_pair_list.push_back (std::make_pair (src_it_end, dst_it));
         
         ++dst_it;
      }
      else
      {
         break;
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#undef dd_break



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
