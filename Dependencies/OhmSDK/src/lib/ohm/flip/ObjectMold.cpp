/*****************************************************************************

        ObjectMold.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70255

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

#include "ohm/flip/ObjectMold.h"
#include "ohm/flip/Cue.h"

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

ObjectMold::ObjectMold (Object & object)
:  _ref (object.get_ref ())
,  _cdm (object.get_cdm ())
,  _data_ptr_list ()
,  _instruction_ptr_list ()
{
   // Add predestination to element
   
   Object * object_ptr = &object;

   bool loop_flag = true;
   
   while (loop_flag)
   {
      ClassDescriptionMaster & cdm = object_ptr->get_cdm ();
      
      _data_ptr_list.push_back (new ObjectMoldData (object_ptr->get_ref (), cdm.get_class_id ()));
      ObjectMoldData & data = *_data_ptr_list.back ();
      
      parse (data, *object_ptr, "");
      
      object_ptr = object_ptr->get_parent_ptr ();
      
      loop_flag = object_ptr != 0;
   }
   
   // Add subtree to element
   // start with properties of first element
   
   parse_properties (object);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ObjectMold::~ObjectMold ()
{
   {
      ObjectMoldDataPtrList::iterator it = _data_ptr_list.begin ();
      const ObjectMoldDataPtrList::iterator it_end = _data_ptr_list.end ();
      
      for (; it != it_end ; ++it)
      {
         ObjectMoldData * data_ptr = *it;
         
         delete data_ptr;
      }
      
      _data_ptr_list.clear ();
   }
   
   {
      InstructionPtrList::iterator it = _instruction_ptr_list.begin ();
      const InstructionPtrList::iterator it_end = _instruction_ptr_list.end ();
      
      for (; it != it_end ; ++it)
      {
         Instruction * inst_ptr = *it;
         
         delete inst_ptr;
      }
      
      _instruction_ptr_list.clear ();
   }
}



/*
==============================================================================
Name : get_cdm
==============================================================================
*/

ClassDescriptionMaster &   ObjectMold::get_cdm ()
{
   return _cdm;
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::execute (DocumentBase & document, flip::Ref ref)
{
   RefMapping ref_mapping;
   
   ref_mapping [_ref] = ref;
   
   InstructionPtrList::iterator it = _instruction_ptr_list.begin ();
   InstructionPtrList::iterator it_end = _instruction_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      Instruction * inst_ptr = *it;
      assert (inst_ptr != 0);
      
      inst_ptr->execute (document, ref_mapping);
   }
   
   it = _instruction_ptr_list.begin ();
   
   for (; it != it_end ; ++it)
   {
      Instruction * inst_ptr = *it;
      assert (inst_ptr != 0);
      
      inst_ptr->post_execute (document, ref_mapping);
   }
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::execute (DocumentBase & document, flip::Ref ref, RefRefArr & ref_ref_arr)
{
   RefMapping ref_mapping;
   
   ref_mapping [_ref] = ref;
   
   InstructionPtrList::iterator it = _instruction_ptr_list.begin ();
   InstructionPtrList::iterator it_end = _instruction_ptr_list.end ();
   
   for (; it != it_end ; ++it)
   {
      Instruction * inst_ptr = *it;
      assert (inst_ptr != 0);
      
      inst_ptr->execute (document, ref_mapping);
   }
   
   it = _instruction_ptr_list.begin ();
   
   for (; it != it_end ; ++it)
   {
      Instruction * inst_ptr = *it;
      assert (inst_ptr != 0);
      
      inst_ptr->post_execute (document, ref_mapping);
   }
   
   ref_ref_arr.push_back (std::make_pair (_ref, ref));
   
   it = _instruction_ptr_list.begin ();
   
   for (; it != it_end ; ++it)
   {
      Instruction * inst_ptr = *it;
      assert (inst_ptr != 0);
      
      inst_ptr->push_translation (ref_ref_arr, ref_mapping);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::Instruction::make_mapping (RefMapping & ref_mapping, flip::Ref src_ref, flip::Ref dst_ref)
{
   bool ok_flag = ref_mapping.insert (std::make_pair (src_ref, dst_ref)).second;
   
   assert (ok_flag);
}



/*
==============================================================================
Name : translate
Description :
   Translate from the original ref to our new created ref.
   If the original ref is not in the ref_mapping, consider it as a non copied
   external reference.
==============================================================================
*/

flip::Ref   ObjectMold::Instruction::translate (const RefMapping & ref_mapping, const RefOffset & ref_offset)
{
   RefMapping::const_iterator it = ref_mapping.find (ref_offset.first);
   
   flip::Ref ref = 0;
   
   if (it != ref_mapping.end ())
   {
      ref = it->second;
   }
   else
   {
      // not in the copied ref. consider it as a non copied
      // external reference
      
      ref = ref_offset.first;
   }
   
   ref += ref_offset.second;
   
   return ref;
}



/*
==============================================================================
Name : push_translation
==============================================================================
*/

void  ObjectMold::Instruction::push_translation (RefRefArr & ref_ref_arr, RefMapping & ref_mapping)
{
   flip::Ref sub_ref = translate (ref_mapping, _ref_offset);
   
   ref_ref_arr.push_back (std::make_pair (
      _ref_offset.first + _ref_offset.second,
      sub_ref
   ));
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionCollectionInsert::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   flip::Ref ref = translate (ref_mapping, _ref_offset);
   assert (ref != 0);
   
   Object * obj_ptr = document.get_object (ref);
   assert (obj_ptr != 0);
   
   CollectionMaster * cast_ptr = dynamic_cast <CollectionMaster *> (obj_ptr);
   assert (cast_ptr != 0);
   
   assert (_cdm_ptr != 0);
   _new_sub_ref = cast_ptr->insert (*_cdm_ptr);
   
   make_mapping (ref_mapping, _sub_ref, _new_sub_ref);
}



/*
==============================================================================
Name : push_translation
==============================================================================
*/

void  ObjectMold::InstructionCollectionInsert::push_translation (RefRefArr & ref_ref_arr, RefMapping & ref_mapping)
{
   ref_ref_arr.push_back (std::make_pair (_sub_ref, _new_sub_ref));
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionArrayInsert::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   flip::Ref ref = translate (ref_mapping, _ref_offset);
   assert (ref != 0);
   
   Object * obj_ptr = document.get_object (ref);
   assert (obj_ptr != 0);
   
   ArrayMaster * cast_ptr = dynamic_cast <ArrayMaster *> (obj_ptr);
   assert (cast_ptr != 0);
   
   assert (_cdm_ptr != 0);
   _new_sub_ref = cast_ptr->push_back (*_cdm_ptr);
   
   make_mapping (ref_mapping, _sub_ref, _new_sub_ref);
}



/*
==============================================================================
Name : push_translation
==============================================================================
*/

void  ObjectMold::InstructionArrayInsert::push_translation (RefRefArr & ref_ref_arr, RefMapping & ref_mapping)
{
   ref_ref_arr.push_back (std::make_pair (_sub_ref, _new_sub_ref));
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionSetBool::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   flip::Ref ref = translate (ref_mapping, _ref_offset);
   assert (ref != 0);
   
   Object * obj_ptr = document.get_object (ref);
   assert (obj_ptr != 0);
   
   Bool * cast_ptr = dynamic_cast <Bool *> (obj_ptr);
   assert (cast_ptr != 0);
   
   cast_ptr->operator = (_val);
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionSetInt64::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   flip::Ref ref = translate (ref_mapping, _ref_offset);
   assert (ref != 0);
   
   Object * obj_ptr = document.get_object (ref);
   assert (obj_ptr != 0);
   
   Int64 * cast_ptr = dynamic_cast <Int64 *> (obj_ptr);
   assert (cast_ptr != 0);
   
   cast_ptr->operator = (_val);
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionSetEnum::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   flip::Ref ref = translate (ref_mapping, _ref_offset);
   assert (ref != 0);
   
   Object * obj_ptr = document.get_object (ref);
   assert (obj_ptr != 0);
   
   EnumMaster * cast_ptr = dynamic_cast <EnumMaster *> (obj_ptr);
   assert (cast_ptr != 0);
   
   cast_ptr->ctor (_val);
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionSetFloat64::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   flip::Ref ref = translate (ref_mapping, _ref_offset);
   assert (ref != 0);
   
   Object * obj_ptr = document.get_object (ref);
   assert (obj_ptr != 0);
   
   Float64 * cast_ptr = dynamic_cast <Float64 *> (obj_ptr);
   assert (cast_ptr != 0);
   
   cast_ptr->operator = (_val);
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionSetObjectRef::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   // nothing
}



/*
==============================================================================
Name : post_execute
==============================================================================
*/

void  ObjectMold::InstructionSetObjectRef::post_execute (DocumentBase & document, RefMapping & ref_mapping)
{
   if (_pointed_ref != 0)
   {
      flip::Ref ref = translate (ref_mapping, _ref_offset);
      assert (ref != 0);
      
      Object * obj_ptr = document.get_object (ref);
      assert (obj_ptr != 0);
      
      flip::Ref pointed_ref = translate (ref_mapping, std::make_pair (_pointed_ref, 0));
      assert (pointed_ref != 0);

      ObjectRefMaster * cast_ptr = dynamic_cast <ObjectRefMaster *> (obj_ptr);
      assert (cast_ptr != 0);
      
      cast_ptr->set (pointed_ref);
   }
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionSetBlob::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   if (!_val.empty ())
   {
      flip::Ref ref = translate (ref_mapping, _ref_offset);
      assert (ref != 0);
      
      Object * obj_ptr = document.get_object (ref);
      assert (obj_ptr != 0);
      
      Blob * cast_ptr = dynamic_cast <Blob *> (obj_ptr);
      assert (cast_ptr != 0);
      
      cast_ptr->set (&_val [0], _val.size ());
   }
}



/*
==============================================================================
Name : execute
==============================================================================
*/

void  ObjectMold::InstructionSetCue::execute (DocumentBase & document, RefMapping & ref_mapping)
{
   flip::Ref ref = translate (ref_mapping, _ref_offset);
   assert (ref != 0);
   
   Object * obj_ptr = document.get_object (ref);
   assert (obj_ptr != 0);
   
   Cue * cast_ptr = dynamic_cast <Cue *> (obj_ptr);
   assert (cast_ptr != 0);
   
   // nothing
}



/*
==============================================================================
Name : parse
==============================================================================
*/

void  ObjectMold::parse (ObjectMoldData & data, Object & object, const std::string & prefix)
{
   ClassDescriptionMaster & cdm = object.get_cdm ();
   
   const ClassDescriptionMaster::VarInfoArr & var_info_arr = cdm.get_var_info_arr ();
   
   flip::Ref ref = object.get_ref ();
   ref += 1LL;
   
   for (size_t i = 0 ; i < var_info_arr.size () ; ++i)
   {
      const ClassDescriptionMaster::VarInfo & var_info = var_info_arr [i];
      
      const char * name_0 = var_info._name_0;
      int type_id = var_info._type_id;
      ClassDescriptionMaster * sub_cdm_ptr = var_info._cdm_ptr;
      
      Object * sub_object_ptr = object.use_document ().get_object (ref);
      assert (sub_object_ptr != 0);
         
      if ((type_id > 0) && (type_id < ClassDescManager::NBR_BasicTypeId_TOTAL))
      {
         // pointer comparison is ok
         
         std::string var_name = prefix + name_0;
         
         if (type_id == ClassDescManager::BasicTypeId_OBJECT_REF)
         {
            ObjectRefMaster * cast_ptr = dynamic_cast <ObjectRefMaster *> (sub_object_ptr);
            assert (cast_ptr != 0);
            
            data.add_value (var_name, type_id, cast_ptr->get ());
         }
         else if (type_id == ClassDescManager::BasicTypeId_ARRAY)
         {
            // don't follow arrays
         }
         else if (type_id == ClassDescManager::BasicTypeId_COLLECTION)
         {
            // don't follow collections
         }
         else if (type_id == ClassDescManager::BasicTypeId_BOOL)
         {
            Bool * cast_ptr = dynamic_cast <Bool *> (sub_object_ptr);
            assert (cast_ptr != 0);
            
            data.add_value (var_name, type_id, cast_ptr->operator bool () ? 1LL : 0LL);
         }
         else if (type_id == ClassDescManager::BasicTypeId_INT64)
         {
            Int64 * cast_ptr = dynamic_cast <Int64 *> (sub_object_ptr);
            assert (cast_ptr != 0);
            
            data.add_value (var_name, type_id, cast_ptr->operator archi::Int64 ());
         }
         else if (type_id == ClassDescManager::BasicTypeId_ENUM)
         {
            EnumMaster * cast_ptr = dynamic_cast <EnumMaster *> (sub_object_ptr);
            assert (cast_ptr != 0);
            
            data.add_value (var_name, type_id, cast_ptr->get ());
         }
         else if (type_id == ClassDescManager::BasicTypeId_FLOAT64)
         {
            Float64 * cast_ptr = dynamic_cast <Float64 *> (sub_object_ptr);
            assert (cast_ptr != 0);
            
            data.add_value (var_name, type_id, cast_ptr->operator archi::Float64 ());
         }
         else if (type_id == ClassDescManager::BasicTypeId_BLOB)
         {
            Blob * cast_ptr = dynamic_cast <Blob *> (sub_object_ptr);
            assert (cast_ptr != 0);
            
            data.add_value (var_name, type_id, cast_ptr->get ());
         }
         else if (type_id == ClassDescManager::BasicTypeId_CUE)
         {
            Cue * cast_ptr = dynamic_cast <Cue *> (sub_object_ptr);
            assert (cast_ptr != 0);
            
            // nothing
         }
         else
         {
            assert (false);
         }
      }
      else
      {
         std::string sub_prefix = cdm.get_name ();
         
         parse (data, *sub_object_ptr, prefix + name_0 + ".");
      }
      
      ref += sub_cdm_ptr->get_nbr_ref ();
   }
}



/*
==============================================================================
Name : parse_properties
Description :
   Parse each ref starting from ref + 1 and create the instruction to set
   its value
   (main - not compound - version)
==============================================================================
*/

void  ObjectMold::parse_properties (Object & object)
{
   ClassDescriptionMaster & cdm = object.get_cdm ();
   
   if (BasicTypesName::is_basic_type (cdm.get_name ()))
   {
      flip::Ref base_ref = object.get_ref ();
      
      parse_basic_type (object, base_ref, 0);
   }
   else
   {
      const ClassDescriptionMaster::VarInfoArr & var_info_arr = cdm.get_var_info_arr ();
      
      flip::Ref base_ref = object.get_ref ();
      flip::Ref ref = base_ref + 1LL;
      
      for (size_t i = 0 ; i < var_info_arr.size () ; ++i)
      {
         const ClassDescriptionMaster::VarInfo & var_info = var_info_arr [i];
         
         ClassDescriptionMaster * sub_cdm_ptr = var_info._cdm_ptr;
         
         Object * sub_object_ptr = object.use_document ().get_object (ref);
         assert (sub_object_ptr != 0);
            
         if (BasicTypesName::is_basic_type (sub_cdm_ptr->get_name ()))
         {
            parse_basic_type (*sub_object_ptr, base_ref, ref - base_ref);
         }
         else
         {
            parse_properties (*sub_object_ptr, base_ref, ref - base_ref);
         }
         
         ref += sub_cdm_ptr->get_nbr_ref ();
      }
   }
}



/*
==============================================================================
Name : parse_properties
Description :
   Parse each ref starting from ref + 1 and create the instruction to set
   its value
   (compound version)
==============================================================================
*/

void  ObjectMold::parse_properties (Object & object, flip::Ref base_ref, archi::Int64 offset)
{
   ClassDescriptionMaster & cdm = object.get_cdm ();
   
   if (BasicTypesName::is_basic_type (cdm.get_name ()))
   {
      flip::Ref ref = base_ref + offset;
      
      parse_basic_type (object, base_ref, ref - base_ref);
   }
   else
   {
      const ClassDescriptionMaster::VarInfoArr & var_info_arr = cdm.get_var_info_arr ();
      
      flip::Ref ref = base_ref + offset + 1LL;
      
      for (size_t i = 0 ; i < var_info_arr.size () ; ++i)
      {
         const ClassDescriptionMaster::VarInfo & var_info = var_info_arr [i];
         
         ClassDescriptionMaster * sub_cdm_ptr = var_info._cdm_ptr;
         
         Object * sub_object_ptr = object.use_document ().get_object (ref);
         assert (sub_object_ptr != 0);
            
         if (BasicTypesName::is_basic_type (sub_cdm_ptr->get_name ()))
         {
            parse_basic_type (*sub_object_ptr, base_ref, ref - base_ref);
         }
         else
         {
            parse_properties (*sub_object_ptr, base_ref, ref - base_ref);
         }
         
         ref += sub_cdm_ptr->get_nbr_ref ();
      }
   }
}



/*
==============================================================================
Name : parse_basic_type
Description :
   Parse each ref starting from ref + 1 and create the instruction to set
   its value
==============================================================================
*/

void  ObjectMold::parse_basic_type (Object & object, flip::Ref base_ref, archi::Int64 offset)
{
   ClassDescriptionMaster & cdm = object.get_cdm ();
   
   // pointer comparison is ok
   
   if (&ClassDescription <ObjectRefMaster>::use () == &cdm)
   {
      ObjectRefMaster * cast_ptr = dynamic_cast <ObjectRefMaster *> (&object);
      assert (cast_ptr != 0);
      
      InstructionSetObjectRef * inst_ptr = new InstructionSetObjectRef;
      inst_ptr->_pointed_ref = cast_ptr->get ();
      
      inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
      _instruction_ptr_list.push_back (inst_ptr);
   }
   else if (&ClassDescription <ArrayMaster>::use () == &cdm)
   {
      ArrayMaster * cast_ptr = dynamic_cast <ArrayMaster *> (&object);
      assert (cast_ptr != 0);
      
      size_t array_size = cast_ptr->size ();
      
      for (size_t i = 0 ; i < array_size ; ++i)
      {
         flip::Ref sub_ref = cast_ptr->get_element_ref (i);
         
         DocumentBase & document = object.use_document ();
         
         Object * sub_object_ptr = document.get_object (sub_ref);
         assert (sub_object_ptr != 0);
         
         InstructionArrayInsert * inst_ptr = new InstructionArrayInsert;
         inst_ptr->_sub_ref = sub_ref;
         inst_ptr->_cdm_ptr = &sub_object_ptr->get_cdm ();
         
         inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
         _instruction_ptr_list.push_back (inst_ptr);
         
         parse_properties (*sub_object_ptr);
      }
   }
   else if (&ClassDescription <CollectionMaster>::use () == &cdm)
   {
      CollectionMaster * cast_ptr = dynamic_cast <CollectionMaster *> (&object);
      assert (cast_ptr != 0);
      
      size_t array_size = cast_ptr->size ();
      
      for (size_t i = 0 ; i < array_size ; ++i)
      {
         flip::Ref sub_ref = cast_ptr->get_element_ref (i);
         
         DocumentBase & document = object.use_document ();
         
         Object * sub_object_ptr = document.get_object (sub_ref);
         assert (sub_object_ptr != 0);
         
         InstructionCollectionInsert * inst_ptr = new InstructionCollectionInsert;
         inst_ptr->_sub_ref = sub_ref;
         inst_ptr->_cdm_ptr = &sub_object_ptr->get_cdm ();
         
         inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
         _instruction_ptr_list.push_back (inst_ptr);
         
         parse_properties (*sub_object_ptr);
      }
   }
   else if (&ClassDescription <Bool>::use () == &cdm)
   {
      Bool * cast_ptr = dynamic_cast <Bool *> (&object);
      assert (cast_ptr != 0);
      
      InstructionSetBool * inst_ptr = new InstructionSetBool;
      inst_ptr->_val = cast_ptr->operator bool ();
      
      inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
      _instruction_ptr_list.push_back (inst_ptr);
   }
   else if (&ClassDescription <Int64>::use () == &cdm)
   {
      Int64 * cast_ptr = dynamic_cast <Int64 *> (&object);
      assert (cast_ptr != 0);
      
      InstructionSetInt64 * inst_ptr = new InstructionSetInt64;
      inst_ptr->_val = cast_ptr->operator archi::Int64 ();
      
      inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
      _instruction_ptr_list.push_back (inst_ptr);
   }
   else if (&ClassDescription <EnumMaster>::use () == &cdm)
   {
      EnumMaster * cast_ptr = dynamic_cast <EnumMaster *> (&object);
      assert (cast_ptr != 0);
      
      InstructionSetEnum * inst_ptr = new InstructionSetEnum;
      inst_ptr->_val = cast_ptr->get ();
      
      inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
      _instruction_ptr_list.push_back (inst_ptr);
   }
   else if (&ClassDescription <Float64>::use () == &cdm)
   {
      Float64 * cast_ptr = dynamic_cast <Float64 *> (&object);
      assert (cast_ptr != 0);
      
      InstructionSetFloat64 * inst_ptr = new InstructionSetFloat64;
      inst_ptr->_val = cast_ptr->operator archi::Float64 ();
      
      inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
      _instruction_ptr_list.push_back (inst_ptr);
   }
   else if (&ClassDescription <Blob>::use () == &cdm)
   {
      Blob * cast_ptr = dynamic_cast <Blob *> (&object);
      assert (cast_ptr != 0);
      
      InstructionSetBlob * inst_ptr = new InstructionSetBlob;
      inst_ptr->_val = cast_ptr->get ();
      
      inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
      _instruction_ptr_list.push_back (inst_ptr);
   }
   else if (&ClassDescription <Cue>::use () == &cdm)
   {
      Cue * cast_ptr = dynamic_cast <Cue *> (&object);
      assert (cast_ptr != 0);
      
      InstructionSetCue * inst_ptr = new InstructionSetCue;
      // nothing
      
      inst_ptr->_ref_offset = std::make_pair (base_ref, offset);
      _instruction_ptr_list.push_back (inst_ptr);
   }
   else
   {
      assert (false);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
