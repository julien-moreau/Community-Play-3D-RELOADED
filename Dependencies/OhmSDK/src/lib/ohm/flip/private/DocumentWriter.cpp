/*****************************************************************************

        DocumentWriter.cpp
        Copyright (c) 2007 Ohm Force

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

#include "ohm/flip/private/DocumentWriter.h"
#include "ohm/flip/private/ObjectManager.h"
#include "ohm/flip/Bool.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Enum.h"
#include "ohm/flip/Float64.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/Cue.h"
#include "ohm/flip/ObjectRef.h"
#include "ohm/flip/Array.h"
#include "ohm/flip/Collection.h"
#include "ohm/flip/private/DocumentKeywords.h"
#include "ohm/flip/private/DocumentFormatter.h"
#include "ohm/flip/private/DataStreamOutput.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/flip/private/Codec.h"
#include "ohm/txt/Err.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/txt/out/Print.h"
#include "ohm/lang/fnc.h"
#include "ohm/math/FloatMultiPrecPrint.h"

#if ! defined (ohm_flip_SERVER)
   #include "ohm/sys/TimerReliable.h"
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <cassert>
#include <cstdarg>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   'full_state_flag' tells if all object are to be written, since some objects
   must not be stored in the document.
==============================================================================
*/

DocumentWriter::DocumentWriter (DocumentBase & document, bool float64x_flag)
:  _document (document)
,  _object_manager (document.use_object_manager ())
,  _float64x_flag (float64x_flag)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

DocumentWriter::~DocumentWriter ()
{
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  DocumentWriter::write (DataStreamOutput & dst)
{
#if ! defined (ohm_flip_SERVER)
   sys::TimerReliable & tr = sys::TimerReliable::use_instance ();
   
   double now = tr.get ();
#endif
   
   write_dst (dst);

#if ! defined (ohm_flip_SERVER)
   ohm_util_TRACE_INFO1 ("DocumentWriter::write took %1% ms", (tr.get () - now) * 1000.0);
#endif
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : write_dst
==============================================================================
*/

void  DocumentWriter::write_dst (DataStreamOutput & dst)
{
   DocumentFormatter::write_header (dst);
   
   std::set <size_t> obj_offset_set;
   
   ObjectManager::iterator it = _object_manager.begin ();
   ObjectManager::iterator it_end = _object_manager.end ();
   
   for (; it != it_end ; ++it)
   {
      Object * object_ptr = it.get_object_ptr ();
      
      if (object_ptr != 0)
      {
         assert (object_ptr != 0);
         assert (object_ptr->is_inited ());
         
         flip::Ref object_ref = it.get_ref ();
         assert (object_ref != 0);
         
         bool write_flag = true;
         write_flag &= (dynamic_cast <const Bool *> (object_ptr) == 0);
         write_flag &= (dynamic_cast <const Float64 *> (object_ptr) == 0);
         write_flag &= (dynamic_cast <const Int64 *> (object_ptr) == 0);
         write_flag &= (dynamic_cast <const EnumMaster *> (object_ptr) == 0);
         write_flag &= (dynamic_cast <const ObjectRefMaster *> (object_ptr) == 0);
         write_flag &= (dynamic_cast <const ArrayMaster *> (object_ptr) == 0);
         write_flag &= (dynamic_cast <const CollectionMaster *> (object_ptr) == 0);
         write_flag &= (dynamic_cast <const Cue *> (object_ptr) == 0);
         
         if (dynamic_cast <const Blob *> (object_ptr) != 0)
         {
            write_flag &= dynamic_cast <Blob &> (*object_ptr).size () > 0;
            write_flag &= !dynamic_cast <Blob &> (*object_ptr).is_inline ();
         }
         
         if (write_flag)
         {
            obj_offset_set.insert (dst.tell ());
            
            write (dst, *object_ptr);
         }
      }
   }
   
   size_t startxref = 0;
   
   DocumentFormatter::write_xref (dst, startxref, obj_offset_set);
   
   Object * root_ptr = _object_manager.get_root ();
   assert (root_ptr != 0);
   
   DocumentFormatter::write_trailer (
      dst,
      root_ptr->get_ref (),
      _document.get_rev_id (),
      _document.get_format_version (),
      _document.use_user_manager ().get_user_name_id_map ()
   );
   
   DocumentFormatter::write_start_xref (dst, startxref);
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  DocumentWriter::write (DataStreamOutput & dst, Object & object)
{
   if (dynamic_cast <Blob *> (&object) != 0)
   {
      write_blob (dst, dynamic_cast <Blob &> (object));
   }
   else
   {
      write_object (dst, object);
   }
}



/*
==============================================================================
Name : write_object
==============================================================================
*/

void  DocumentWriter::write_object (DataStreamOutput & dst, Object & object)
{
   flip::Ref ref = object.get_ref ();
   
   ClassDescriptionMaster & cdm = object.get_cdm ();
   
   DocumentFormatter::write_begin_obj (dst, ref, cdm.get_name ());
   
   if (cdm.get_var_info_arr ().size () > 0)
   {
      ClassDescriptionMaster::VarInfoArr::const_iterator it
         = cdm.get_var_info_arr ().begin ();
      const ClassDescriptionMaster::VarInfoArr::const_iterator it_end
         = cdm.get_var_info_arr ().end ();
      
      // ref of the first aggregate if it exists
      flip::Ref sub_ref = ref + 1;
      
      for (; it != it_end ; ++it)
      {
         int type_id = it->_type_id;
         assert (type_id > 0);
         
         const char * var_name_0 = it->_name_0;
         
         ClassDescriptionMaster & var_class_desc = *it->_cdm_ptr;
         
         DocumentFormatter::write_member (dst, var_name_0);
         
         Object * member_ptr = _object_manager.get_object (sub_ref);
         assert (member_ptr != 0);
         
         if (dynamic_cast <Bool *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_BOOL);
            
            DocumentFormatter::write_member_bool (
               dst,
               dynamic_cast <Bool *> (member_ptr)->operator bool ()
            );
         }
         else if (dynamic_cast <Int64 *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_INT64);
            
            DocumentFormatter::write_member_int64 (
               dst,
               dynamic_cast <Int64 *> (member_ptr)->operator archi::Int64 ()
            );
         }
         else if (dynamic_cast <EnumMaster *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_ENUM);
            
            EnumMaster & enum_master = *dynamic_cast <EnumMaster *> (member_ptr);
            
            enum_master.get_enum_type_name ();
            
            const char * name_0 = 0;
            int sub_err = enum_master.get_name_for_val (name_0, enum_master.get ());
            assert (sub_err == Err_OK);
            
            DocumentFormatter::write_member_enum (
               dst,
               enum_master.get_enum_type_name (),
               name_0
            );
         }
         else if (dynamic_cast <Float64 *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_FLOAT64);
            
            if (_float64x_flag)
            {
               DocumentFormatter::write_member_float64x (
                  dst,
                  dynamic_cast <Float64 *> (member_ptr)->operator archi::Float64 ()
               );
            }
            else
            {
               DocumentFormatter::write_member_float64 (
                  dst,
                  dynamic_cast <Float64 *> (member_ptr)->operator archi::Float64 ()
               );
            }
         }
         else if (dynamic_cast <Blob *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_BLOB);
            
            Blob & blob = *dynamic_cast <Blob *> (member_ptr);
            
            if (blob.empty ())
            {
               DocumentFormatter::write_member_blob_empty (dst, blob.is_inline ());
            }
            else if (blob.is_inline ())
            {
               Codec::Data data;
               const Blob::Data & blob_data = blob.get ();
               
               int err = Codec::encode (data, &blob_data [0], blob_data.size (), blob.get_encoder_arr ());
               assert (err == Err_OK);

               DocumentFormatter::write_member_blob_inline (dst, data, blob.get_encoder_arr ());
            }
            else
            {
               DocumentFormatter::write_member_blob (dst, member_ptr->get_ref ());
            }
         }
         else if (dynamic_cast <ObjectRefMaster *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_OBJECT_REF);
            
            DocumentFormatter::write_member_object_ref (
               dst,
               dynamic_cast <ObjectRefMaster *> (member_ptr)->get ()
            );
         }
         else if (dynamic_cast <ArrayMaster *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_ARRAY);
            
            write_member_array (dst, *dynamic_cast <ArrayMaster *> (member_ptr));
         }
         else if (dynamic_cast <CollectionMaster *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_COLLECTION);
            
            write_member_collection (dst, *dynamic_cast <CollectionMaster *> (member_ptr));
         }
         else if (dynamic_cast <Cue *> (member_ptr) != 0)
         {
            assert (type_id == ClassDescManager::BasicTypeId_CUE);
            
            DocumentFormatter::write_member_cue (dst);
         }
         else if (member_ptr != 0)
         {
            DocumentFormatter::write_member_object (dst, member_ptr->get_ref ());
         }
         else
         {
            ohm_util_TRACE_PANIC ("Document write corrupted : object exists but member not");
         }
         
         archi::Int64 nbr_ref = var_class_desc.get_nbr_ref ();
         
         sub_ref += nbr_ref;
      }
   }
   
   DocumentFormatter::write_end_obj (dst);
}



/*
==============================================================================
Name : write_blob
==============================================================================
*/

void  DocumentWriter::write_blob (DataStreamOutput & dst, Blob & blob)
{
   flip::Ref ref = blob.get_ref ();
   
   Codec::Data data;
   const Blob::Data & blob_data = blob.get ();
   
   assert (blob_data.size () > 0);
   
   int err = Codec::encode (data, &blob_data [0], blob_data.size (), blob.get_encoder_arr ());
   assert (err == Err_OK);
   
   DocumentFormatter::write_begin_blob (dst, ref, data, blob.get_encoder_arr ());
   
   DocumentFormatter::write_blob (dst, data);
   
   DocumentFormatter::write_end_blob (dst);
}



/*
==============================================================================
Name : write_member_array
==============================================================================
*/

void  DocumentWriter::write_member_array (DataStreamOutput & dst, ArrayMaster & member)
{
   int elem_class_id = member.get_element_class_id ();
   assert (elem_class_id > 0);
   
   if (elem_class_id == ClassDescManager::BasicTypeId_BOOL)
   {
      DocumentFormatter::write_member_array_bool_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_INT64)
   {
      DocumentFormatter::write_member_array_int64_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_ENUM)
   {
      // not supported
      assert (false);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_FLOAT64)
   {
      DocumentFormatter::write_member_array_float64_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_BLOB)
   {
      DocumentFormatter::write_member_array_blob_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_OBJECT_REF)
   {
      DocumentFormatter::write_member_array_object_ref_begin (dst);
   }
   else
   {
      DocumentFormatter::write_member_array_object_begin (dst);
   }
   
   for (size_t i = 0 ; i < member.size () ; ++i)
   {
      const BinaryKey & key = member.get_element_key (i);
      
      std::string key_str;
      key.write (key_str);
      
      flip::Ref ref = member.get_element_ref (i);
      
      DocumentFormatter::write_member_array_key_ref (dst, key, ref);
      
      Object * elem_ptr = _object_manager.get_object (ref);
      assert (elem_ptr != 0);
      
      if (dynamic_cast <const Bool *> (elem_ptr) != 0)
      {
         bool val = bool (*dynamic_cast <const Bool *> (elem_ptr));
         
         DocumentFormatter::write_member_container_bool (dst, val);
      }
      else if (dynamic_cast <const Int64 *> (elem_ptr) != 0)
      {
         archi::Int64 val = archi::Int64 (*dynamic_cast <const Int64 *> (elem_ptr));
         
         DocumentFormatter::write_member_container_int64 (dst, val);
      }
      else if (dynamic_cast <const EnumMaster *> (elem_ptr) != 0)
      {
         // const EnumMaster & enum_master = *dynamic_cast <const EnumMaster *> (elem_ptr);
         
         assert (false);
      }
      else if (dynamic_cast <const Float64 *> (elem_ptr) != 0)
      {
         archi::Float64 val = archi::Float64 (*dynamic_cast <const Float64 *> (elem_ptr));
         
         if (_float64x_flag)
         {
            DocumentFormatter::write_member_container_float64x (dst, val);
         }
         else
         {
            DocumentFormatter::write_member_container_float64 (dst, val);
         }
      }
      else if (dynamic_cast <const Blob *> (elem_ptr) != 0)
      {
         // nothing else
      }
      else if (dynamic_cast <const ObjectRefMaster *> (elem_ptr) != 0)
      {
         archi::Int64 pointed_ref = dynamic_cast <const ObjectRefMaster *> (elem_ptr)->get ();
         
         DocumentFormatter::write_member_container_object_ref (dst, pointed_ref);
      }
      else if (dynamic_cast <const ArrayMaster *> (elem_ptr) != 0)
      {
         // not supported
         assert (false);
      }
      else if (dynamic_cast <const CollectionMaster *> (elem_ptr) != 0)
      {
         // not supported
         assert (false);
      }
      else if (dynamic_cast <const Cue *> (elem_ptr) != 0)
      {
         // not supported
         assert (false);
      }
      else
      {
         // nothing else
      }
   }
   
   DocumentFormatter::write_member_array_end (dst);
}



/*
==============================================================================
Name : write_member_collection
==============================================================================
*/

void  DocumentWriter::write_member_collection (DataStreamOutput & dst, CollectionMaster & member)
{
   int elem_class_id = member.get_element_class_id ();
   assert (elem_class_id > 0);
   
   if (elem_class_id == ClassDescManager::BasicTypeId_BOOL)
   {
      DocumentFormatter::write_member_collection_bool_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_INT64)
   {
      DocumentFormatter::write_member_collection_int64_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_ENUM)
   {
      // not supported
      assert (false);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_FLOAT64)
   {
      DocumentFormatter::write_member_collection_float64_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_BLOB)
   {
      DocumentFormatter::write_member_collection_blob_begin (dst);
   }
   else if (elem_class_id == ClassDescManager::BasicTypeId_OBJECT_REF)
   {
      DocumentFormatter::write_member_collection_object_ref_begin (dst);
   }
   else
   {
      DocumentFormatter::write_member_collection_object_begin (dst);
   }
   
   // write do document in ref orders
   
   std::set <flip::Ref> ref_set;
   
   for (size_t i = 0 ; i < member.size () ; ++i)
   {
      flip::Ref ref = member.get_element_ref (i);
      
      // problem here, it should have been detected earlier
      assert (ref_set.find (ref) == ref_set.end ());
      
      ref_set.insert (ref);
   }
   
   std::set <flip::Ref>::iterator it = ref_set.begin ();
   const std::set <flip::Ref>::iterator it_end = ref_set.end ();
   
   for (; it != it_end ; ++it)
   {
      flip::Ref ref = *it;
      
      DocumentFormatter::write_member_collection_ref (dst, ref);
      
      Object * elem_ptr = _object_manager.get_object (ref);
      assert (elem_ptr != 0);
      
      if (dynamic_cast <const Bool *> (elem_ptr) != 0)
      {
         bool val = bool (*dynamic_cast <const Bool *> (elem_ptr));
         
         DocumentFormatter::write_member_container_bool (dst, val);
      }
      else if (dynamic_cast <const Int64 *> (elem_ptr) != 0)
      {
         archi::Int64 val = archi::Int64 (*dynamic_cast <const Int64 *> (elem_ptr));
         
         DocumentFormatter::write_member_container_int64 (dst, val);
      }
      else if (dynamic_cast <const EnumMaster *> (elem_ptr) != 0)
      {
         // const EnumMaster & enum_master = *dynamic_cast <const EnumMaster *> (elem_ptr);
         
         assert (false);
      }
      else if (dynamic_cast <const Float64 *> (elem_ptr) != 0)
      {
         archi::Float64 val = archi::Float64 (*dynamic_cast <const Float64 *> (elem_ptr));
         
         if (_float64x_flag)
         {
            DocumentFormatter::write_member_container_float64x (dst, val);
         }
         else
         {
            DocumentFormatter::write_member_container_float64 (dst, val);
         }
      }
      else if (dynamic_cast <const Blob *> (elem_ptr) != 0)
      {
         // nothing else
      }
      else if (dynamic_cast <const ObjectRefMaster *> (elem_ptr) != 0)
      {
         archi::Int64 pointed_ref = dynamic_cast <const ObjectRefMaster *> (elem_ptr)->get ();
         
         DocumentFormatter::write_member_container_object_ref (dst, pointed_ref);
      }
      else if (dynamic_cast <const ArrayMaster *> (elem_ptr) != 0)
      {
         // not supported
         assert (false);
      }
      else if (dynamic_cast <const CollectionMaster *> (elem_ptr) != 0)
      {
         // not supported
         assert (false);
      }
      else if (dynamic_cast <const Cue *> (elem_ptr) != 0)
      {
         // not supported
         assert (false);
      }
      else
      {
         // nothing else
      }
   }
   
   DocumentFormatter::write_member_collection_end (dst);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
