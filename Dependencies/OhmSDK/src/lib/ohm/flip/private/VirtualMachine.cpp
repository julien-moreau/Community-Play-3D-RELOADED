/*****************************************************************************

        VirtualMachine.cpp
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

#include "ohm/flip/private/VirtualMachine.h"
#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/ObjectTravelAgency.h"
#include "ohm/flip/private/TravelerObjectInit.h"
#include "ohm/flip/private/TravelerObjectRestore.h"
#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/ValidationReportWriter.h"

#include "ohm/flip/Bool.h"
#include "ohm/flip/Int64.h"
#include "ohm/flip/Enum.h"
#include "ohm/flip/Float64.h"
#include "ohm/flip/Blob.h"
#include "ohm/flip/Array.h"
#include "ohm/flip/Collection.h"
#include "ohm/flip/ObjectRef.h"
#include "ohm/flip/Cue.h"

#include "ohm/math/fnc.h"

#include "ohm/util/trace.h"

#include <cassert>



#undef OHM_FLIP_VM_DEBUG_FLAG

#if defined (OHM_FLIP_VM_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_VM_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define vm_TRACE(msg_0) ohm_util_TRACE_DEBUG(msg_0)
   #define vm_TRACE1(fmt_0, arg1)   ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define vm_TRACE2(fmt_0, arg1, arg2)   ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define vm_TRACE3(fmt_0, arg1, arg2, arg3)   ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define vm_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define vm_TRACE(msg_0) (void (0))
   #define vm_TRACE1(fmt_0, arg1)   (void (0))
   #define vm_TRACE2(fmt_0, arg1, arg2)   (void (0))
   #define vm_TRACE3(fmt_0, arg1, arg2, arg3)   (void (0))
   #define vm_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   (void (0))
#endif



#define if_report if (_report_ptr != 0) _report_ptr
#define if_comp if (_report_ptr != 0) _report_ptr->print_compile_error
#define if_exec if (_report_ptr != 0) _report_ptr->print_execution_error
#define cdm_name get_cdm ().get_name ()
#define arg_obj(x) arg (x->who ().c_str ())
#define arg_class(x) arg (ClassDescManager::use_instance ().who (x))



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

VirtualMachine::VirtualMachine (DocumentBase & document, ObjectManager & object_manager, bool master_flag)
:  _document (document)
,  _object_manager (object_manager)
,  _master_flag (master_flag)
,  _run_flag (false)
,  _mode (Mode_NONE)
,  _direction_normal_flag (true)
,  _report_ptr (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

VirtualMachine::~VirtualMachine ()
{
}


   
/*
==============================================================================
Name : stop
==============================================================================
*/

void  VirtualMachine::stop ()
{
   assert (_run_flag);
   
   _run_flag = false;
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  VirtualMachine::run ()
{
   assert (!_run_flag);
   
   _run_flag = true;
}



/*
==============================================================================
Name : set_mode_safe
Description :
   Set the virtual machine in a mode where exceptions are permitted.
   This is the mode to run code when the code is either not trusted
   or can be the source of exceptions.
==============================================================================
*/

void  VirtualMachine::set_mode_safe ()
{
   _mode = Mode_SAFE;
}



/*
==============================================================================
Name : set_mode_no_fault
Description :
   Set the virtual machine in a mode where no exception is permitted.
   This is the mode to run code when the code is trusted.
   (Already executed and known to execute well, or already validated
   by the server)
==============================================================================
*/

void  VirtualMachine::set_mode_no_fault ()
{
   _mode = Mode_NO_FAULT;
}



/*
==============================================================================
Name : set_mode_auto_correct
Description :
   Set the virtual machine in a mode where exception are permitted, but
   where illegal operand are removed from the code.
   This is the mode to run code when the code can be the source of exceptions,
   but where we want to retrieve as much informations as possible, even if
   is going to fail logical validation from the server.
==============================================================================
*/

void  VirtualMachine::set_mode_auto_correct ()
{
   _mode = Mode_AUTO_CORRECT;
}



/*
==============================================================================
Name : set_mode_conflict_marking
Description :
   Set the virtual machine in a mode where the code is not executed, but
   referenced object are all marked as potentially being the source of
   a vm exception.
   Even if one operation would not generate a vm exception, it is still marked
   to notify that the whole transaction was refused, and therefore the
   operation was not executed.
==============================================================================
*/

void  VirtualMachine::set_mode_conflict_marking ()
{
   _mode = Mode_CONFLICT_MARKING;
}



/*
==============================================================================
Name : set_mode_direction
Description :
   Normal if direction > 0
   Reverse if direction < 0
   an archi::Int64 is used because we use the sign of transaction ids to
   avoid an unnecessary explicit transaction inversion
==============================================================================
*/

void  VirtualMachine::set_mode_direction (archi::Int64 direction)
{
   if (direction > 0)
   {
      _direction_normal_flag = true;
   }
   else if (direction < 0)
   {
      _direction_normal_flag = false;
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : compile
==============================================================================
*/

void  VirtualMachine::bind (ValidationReportWriter & report)
{
   assert (&report != 0);
   assert (_report_ptr == 0);
   
   _report_ptr = &report;
}



/*
==============================================================================
Name : compile
==============================================================================
*/

void  VirtualMachine::unbind (ValidationReportWriter & report)
{
   assert (&report != 0);
   assert (_report_ptr == &report);
   
   _report_ptr = 0;
}



/*
==============================================================================
Name : compile
==============================================================================
*/

int   VirtualMachine::compile (MachineCode & code)
{
   assert (_run_flag);
   
   switch (_mode)
   {
   case Mode_SAFE:
      code.set_mode_safe ();
      break;

   case Mode_NO_FAULT:
      code.set_mode_no_fault ();
      break;

   case Mode_AUTO_CORRECT:
      code.set_mode_auto_correct ();
      break;
   
   case Mode_CONFLICT_MARKING:
      // accept no error at the code level
      code.set_mode_no_fault ();
      break;
   
   default:
      assert (false);
      break;
   }
   
   int err = code.compile ();
   
   assert ((err == Err_OK) || is_tolerant ());
   
   return err;
}



/*
==============================================================================
Name : execute
==============================================================================
*/

int   VirtualMachine::execute (MachineCode & code)
{
   assert (_document.get_controller () != DocumentBase::Controller_NONE);
   assert (_run_flag);
   
   int err = Err_OK;
   
   switch (_mode)
   {
   case Mode_SAFE:
      code.set_mode_safe ();
      break;

   case Mode_NO_FAULT:
      code.set_mode_no_fault ();
      break;

   case Mode_AUTO_CORRECT:
      code.set_mode_auto_correct ();
      break;
   
   case Mode_CONFLICT_MARKING:
      // accept no error at the code level
      code.set_mode_no_fault ();
      break;
   
   default:
      assert (false);
      break;
   }
   
   if (_direction_normal_flag)
   {
      err = execute_normal (code);
   }
   else
   {
      err = execute_reverse (code);
   }
   
   assert ((err == Err_OK) || is_tolerant ());
   
   return err;
}



/*
==============================================================================
Name : set_bool
==============================================================================
*/

int   VirtualMachine::set_bool (flip::Ref ref, archi::UByte val, archi::UByte & old_val)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   
   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_bool: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      Bool * bool_ptr = dynamic_cast <Bool *> (obj_ptr);

      if (bool_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_bool: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      if (val > 1)
      {
         if_exec (Err_VM_BAD_VALUE, "set_bool: %1% val %2%").arg_obj (obj_ptr).arg (val);
         vm_TRACE1 ("BAD VALUE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_VALUE;
      }
      
      if (old_val > 1)
      {
         if_exec (Err_VM_BAD_VALUE, "set_bool: %1% old_val %2%").arg_obj (obj_ptr).arg (val);
         vm_TRACE1 ("BAD VALUE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_VALUE;
      }
      
      int err = Err_OK;
      
      if (bool (*bool_ptr) != (old_val != 0))
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_bool: %1% excepted %2% got %3%")
            .arg_obj (obj_ptr).arg (bool (*bool_ptr)).arg (old_val);
         vm_TRACE3 (
            "VALUE COLLISION %1m16rw0X% found %2% should be %3%", ref,
            bool (*bool_ptr), old_val
         );
            
         old_val = bool (*bool_ptr);
         
         assert (is_tolerant ());
         
         if (!is_auto_correct ())
         {
            return Err_VM_VALUE_COLLISION;
         }
         
         err = Err_VM_CORRECT_OLD_VAL;
      }

      if (val == old_val)
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_bool: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
      }
      
      bool_ptr->set (val);
      
      return err;
   }
}



/*
==============================================================================
Name : set_int64
==============================================================================
*/

int   VirtualMachine::set_int64 (flip::Ref ref, archi::Int64 val, archi::Int64 & old_val)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   
   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_int64: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      Int64 * int64_ptr = dynamic_cast <Int64 *> (obj_ptr);

      if (int64_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_int64: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int err = Err_OK;
      
      if (archi::Int64 (*int64_ptr) != old_val)
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_int64: %1% excepted %2% got %3%")
            .arg_obj (obj_ptr).arg (archi::Int64 (*int64_ptr)).arg (old_val);
         vm_TRACE3 (
            "VALUE COLLISION %1m16rw0X% found %2% should be %3%", ref,
            archi::Int64 (*int64_ptr), old_val
         );
         
         old_val = archi::Int64 (*int64_ptr);
         
         assert (is_tolerant ());
         
         if (!is_auto_correct ())
         {
            return Err_VM_VALUE_COLLISION;
         }
         
         err = Err_VM_CORRECT_OLD_VAL;
      }

      if (val == old_val)
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_int64: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
      }
      
      int64_ptr->set (val);
      
      return err;
   }
}



/*
==============================================================================
Name : set_enum
==============================================================================
*/

int   VirtualMachine::set_enum (flip::Ref ref, archi::Int64 val, archi::Int64 & old_val)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   
   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_enum: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      EnumMaster * enum_ptr = dynamic_cast <EnumMaster *> (obj_ptr);

      if (enum_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_enum: %1% excepted got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      archi::Int64 min_val = enum_ptr->get_min_val ();
      archi::Int64 nbr_val = enum_ptr->get_nbr_val ();
      
      if ((val < min_val) || (val >= min_val + nbr_val))
      {
         if_exec (Err_VM_BAD_VALUE, "set_enum: %1% val %2%").arg_obj (obj_ptr).arg (val);
         vm_TRACE1 ("BAD VALUE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_VALUE;
      }
      
      if ((old_val < min_val) || (old_val >= min_val + nbr_val))
      {
         if_exec (Err_VM_BAD_VALUE, "set_enum: %1% old_val %2%").arg_obj (obj_ptr).arg (val);
         vm_TRACE1 ("BAD VALUE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_VALUE;
      }
      
      int err = Err_OK;
      
      if (enum_ptr->get () != old_val)
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_enum: %1% excepted %2% got %3%")
            .arg_obj (obj_ptr).arg (enum_ptr->get ()).arg (old_val);
         vm_TRACE3 (
            "VALUE COLLISION %1m16rw0X% found %2% should be %3%", ref,
            enum_ptr->get (), old_val
         );
         
         old_val = enum_ptr->get ();
         
         assert (is_tolerant ());
         
         if (!is_auto_correct ())
         {
            return Err_VM_VALUE_COLLISION;
         }
         
         err = Err_VM_CORRECT_OLD_VAL;
      }

      if (val == old_val)
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_enum: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
      }
      
      enum_ptr->set (val);
      
      return err;
   }
}



/*
==============================================================================
Name : set_float64
==============================================================================
*/

int   VirtualMachine::set_float64 (flip::Ref ref, archi::Float64 val, archi::Float64 & old_val)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   
   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_float64: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      Float64 * float64_ptr = dynamic_cast <Float64 *> (obj_ptr);

      if (float64_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_float64: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      if (! math::is_finite (val))
      {
         if_exec (Err_VM_BAD_VALUE, "set_float64: %1% val not finite").arg_obj (obj_ptr);
         vm_TRACE1 ("BAD VALUE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_VALUE;
      }
      
      if (! math::is_finite (old_val))
      {
         if_exec (Err_VM_BAD_VALUE, "set_float64: %1% old_val not finite").arg_obj (obj_ptr);
         vm_TRACE1 ("BAD VALUE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_VALUE;
      }
      
      int err = Err_OK;
      
      if (archi::Float64 (*float64_ptr) != old_val)
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_float64: %1% excepted %2% got %3%")
            .arg_obj (obj_ptr).arg (archi::Float64 (*float64_ptr)).arg (old_val);
         vm_TRACE3 (
            "VALUE COLLISION %1m16rw0X% found %2% should be %3%", ref,
            archi::Float64 (*float64_ptr), old_val
         );
         
         old_val = archi::Float64 (*float64_ptr);
         
         assert (is_tolerant ());
         
         if (!is_auto_correct ())
         {
            return Err_VM_VALUE_COLLISION;
         }
         
         err = Err_VM_CORRECT_OLD_VAL;
      }

      if (val == old_val)
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_float64: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
      }
      
      float64_ptr->set (val);
      
      return err;
   }
}



/*
==============================================================================
Name : set_object_ref
==============================================================================
*/

int   VirtualMachine::set_object_ref (flip::Ref ref, flip::Ref pointed_ref, flip::Ref & old_pointed_ref)
{
   // don't test pointed_ref : having null ref is perfectly
   // valid
   
   Object * obj_ptr = _object_manager.get_object (ref);

   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_object_ref: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      ObjectRefMaster * object_ref_ptr = dynamic_cast <ObjectRefMaster *> (obj_ptr);

      if (object_ref_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_object_ref: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int err = Err_OK;
      
      if (object_ref_ptr->get () != old_pointed_ref)
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_object_ref: %1% excepted %2% got %3%")
            .arg_obj (obj_ptr).arg (object_ref_ptr->get ()).arg (old_pointed_ref);
         vm_TRACE3 (
            "VALUE COLLISION %1m16rw0X% found %2m16rw0X% should be %3m16rw0X%", ref,
            object_ref_ptr->get (), old_pointed_ref
         );
         
         old_pointed_ref = object_ref_ptr->get ();
         
         assert (is_tolerant ());
         
         if (!is_auto_correct ())
         {
            return Err_VM_VALUE_COLLISION;
         }
         
         err = Err_VM_CORRECT_OLD_VAL;
      }

      if (pointed_ref == old_pointed_ref)
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_object_ref: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
      }
      
      object_ref_ptr->set_ref (pointed_ref);
      
      return err;
   }
}



/*
==============================================================================
Name : insert_array
==============================================================================
*/

int   VirtualMachine::insert_array (flip::Ref ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id)
{
   Object * obj_ptr = _object_manager.get_object (ref);
   
   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "insert_array: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      ArrayMaster * array_ptr = dynamic_cast <ArrayMaster *> (obj_ptr);

      if (array_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "insert_array: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int base_class_id = array_ptr->get_element_class_id ();
      
      if (!ClassDescManager::use_instance ().is_class_id_inheriting_from_class_id (
         class_id, base_class_id
      ))
      {
         if_exec (Err_VM_ARRAY_BAD_ELEMENT_TYPE, "insert_array: %1% %2% not a base of %3%")
            .arg_obj (obj_ptr).arg_class (class_id).arg_class (base_class_id);
         vm_TRACE1 ("BAD ELEMENT TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_ARRAY_BAD_ELEMENT_TYPE;
      }
      
      int err = array_ptr->insert_no_notification (key, pointed_ref, class_id, is_tolerant ());
      
      if (err != Err_OK)
      {
         if_exec (err, "insert_array: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("INSERT FAILED %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : err;
      }
      
      Object * pointed_ptr = _object_manager.get_object (pointed_ref);

      if (pointed_ptr == 0)
      {
         if_exec (Err_VM_INTERNAL_SYSTEM_ERROR, "insert_array: %1% pointed").arg_obj (obj_ptr);
         vm_TRACE1 ("INTERNAL SYSTEM ERROR %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : erase_array
==============================================================================
*/

int   VirtualMachine::erase_array (flip::Ref ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id)
{
   Object * obj_ptr = _object_manager.get_object (ref);

   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "erase_array: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      ArrayMaster * array_ptr = dynamic_cast <ArrayMaster *> (obj_ptr);

      if (array_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "erase_array: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int base_class_id = array_ptr->get_element_class_id ();
      
      if (!ClassDescManager::use_instance ().is_class_id_inheriting_from_class_id (
         class_id, base_class_id
      ))
      {
         if_exec (Err_VM_ARRAY_BAD_ELEMENT_TYPE, "erase_array: %1% %2% not a base of %3%")
            .arg_obj (obj_ptr).arg_class (class_id).arg_class (base_class_id);
         vm_TRACE1 ("BAD ELEMENT TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_ARRAY_BAD_ELEMENT_TYPE;
      }
      
      int err = array_ptr->erase_no_notification (key, pointed_ref, is_tolerant ());
      
      if (err != Err_OK)
      {
         if_exec (err, "erase_array: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("ERASE FAILED %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : err;
      }
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : move_array
==============================================================================
*/

int   VirtualMachine::move_array (flip::Ref dst_ref, const BinaryKey & dst_key, flip::Ref src_ref, const BinaryKey & src_key)
{
   Object * dst_obj_ptr = _object_manager.get_object (dst_ref);

   if (is_conflict_marking ())
   {
      if (dst_obj_ptr != 0)
      {
         dst_obj_ptr->set_conflict ();
      }
      
      Object * src_obj_ptr = _object_manager.get_object (src_ref);
      
      if (src_obj_ptr != 0)
      {
         src_obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (dst_obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "move_array: %1m16rw0X% dst").arg (dst_ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", dst_ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      ArrayMaster * dst_array_ptr = dynamic_cast <ArrayMaster *> (dst_obj_ptr);

      if (dst_array_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "move_array: %1% got %2% dst")
            .arg_obj (dst_obj_ptr).arg (dst_obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", dst_ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int dst_class_id = dst_array_ptr->get_element_class_id ();
      
      Object * src_obj_ptr = _object_manager.get_object (src_ref);

      if (src_obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "move_array: %1m16rw0X% src").arg (src_ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", src_ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      ArrayMaster * src_array_ptr = dynamic_cast <ArrayMaster *> (src_obj_ptr);

      if (src_array_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "move_array: %1% got %2% src")
            .arg_obj (src_obj_ptr).arg (src_obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", src_ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int src_class_id = src_array_ptr->get_element_class_id ();
      
      if (src_class_id != dst_class_id)
      {
         if_exec (Err_VM_ARRAY_BAD_ELEMENT_TYPE, "move_array: %1% %2% != %3%")
            .arg_obj (dst_obj_ptr).arg_class (src_class_id).arg_class (dst_class_id);
         vm_TRACE2 ("BAD ELEMENT TYPE %1m16rw0X% %2m16rw0X%", src_ref, dst_ref);
         assert (is_tolerant ());
         return Err_VM_ARRAY_BAD_ELEMENT_TYPE;
      }
      
      int err = src_array_ptr->move_no_notification (dst_array_ptr, dst_key, src_key, is_tolerant ());
      
      if (err != Err_OK)
      {
         if_exec (err, "move_array: %1%").arg_obj (dst_obj_ptr);
         vm_TRACE2 ("MOVE FAILED %1m16rw0X% %2m16rw0X%", src_ref, dst_ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : err;
      }
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : insert_collection
==============================================================================
*/

int   VirtualMachine::insert_collection (flip::Ref ref, flip::Ref pointed_ref, archi::UInt32 class_id)
{
   Object * obj_ptr = _object_manager.get_object (ref);

   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "insert_collection: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      CollectionMaster * collection_ptr = dynamic_cast <CollectionMaster *> (obj_ptr);

      if (collection_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "insert_collection: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int base_class_id = collection_ptr->get_element_class_id ();
      
      if (!ClassDescManager::use_instance ().is_class_id_inheriting_from_class_id (
         class_id, base_class_id
      ))
      {
         if_exec (Err_VM_ARRAY_BAD_ELEMENT_TYPE, "insert_collection: %1% %2% not a base of %3%")
            .arg_obj (obj_ptr).arg_class (class_id).arg_class (base_class_id);
         vm_TRACE1 ("BAD ELEMENT TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_COLLECTION_BAD_ELEMENT_TYPE;
      }
      
      int err = collection_ptr->insert_no_notification (pointed_ref, class_id, is_tolerant ());
      
      if (err != Err_OK)
      {
         if_exec (err, "insert_collection: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("INSERT FAILED %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : err;
      }
      
      Object * pointed_ptr = _object_manager.get_object (pointed_ref);

      if (pointed_ptr == 0)
      {
         vm_TRACE1 ("INTERNAL SYSTEM ERROR %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : erase_collection
==============================================================================
*/

int   VirtualMachine::erase_collection (flip::Ref ref, flip::Ref pointed_ref, archi::UInt32 class_id)
{
   Object * obj_ptr = _object_manager.get_object (ref);

   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "erase_collection: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      CollectionMaster * collection_ptr = dynamic_cast <CollectionMaster *> (obj_ptr);

      if (collection_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "erase_collection: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int base_class_id = collection_ptr->get_element_class_id ();
      
      if (!ClassDescManager::use_instance ().is_class_id_inheriting_from_class_id (
         class_id, base_class_id
      ))
      {
         if_exec (Err_VM_ARRAY_BAD_ELEMENT_TYPE, "erase_collection: %1% %2% not a base of %3%")
            .arg_obj (obj_ptr).arg_class (class_id).arg_class (base_class_id);
         vm_TRACE1 ("BAD ELEMENT TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_COLLECTION_BAD_ELEMENT_TYPE;
      }
      
      int err = collection_ptr->erase_no_notification (pointed_ref, is_tolerant ());
      
      if (err != Err_OK)
      {
         if_exec (err, "erase_collection: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("ERASE FAILED %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : err;
      }
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : move_collection
==============================================================================
*/

int   VirtualMachine::move_collection (flip::Ref new_ref, flip::Ref old_ref, flip::Ref pointed_ref)
{
   Object * src_obj_ptr = _object_manager.get_object (old_ref);

   if (is_conflict_marking ())
   {
      if (src_obj_ptr != 0)
      {
         src_obj_ptr->set_conflict ();
      }
      
      Object * dst_obj_ptr = _object_manager.get_object (new_ref);
      
      if (dst_obj_ptr != 0)
      {
         dst_obj_ptr->set_conflict ();
      }
      
      Object * obj_ptr = _object_manager.get_object (pointed_ref);
      
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (src_obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "move_collection: %1m16rw0X% src").arg (old_ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", old_ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      CollectionMaster * src_collection_ptr = dynamic_cast <CollectionMaster *> (src_obj_ptr);

      if (src_collection_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "move_collection: %1% got %2% src")
            .arg_obj (src_obj_ptr).arg (src_obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", old_ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int src_class_id = src_collection_ptr->get_element_class_id ();
      
      Object * dst_obj_ptr = _object_manager.get_object (new_ref);

      if (dst_obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "move_collection: %1m16rw0X% dst").arg (new_ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", new_ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      CollectionMaster * dst_collection_ptr = dynamic_cast <CollectionMaster *> (dst_obj_ptr);

      if (dst_collection_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "move_collection: %1% got %2% dst")
            .arg_obj (dst_obj_ptr).arg (dst_obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", new_ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      int dst_class_id = dst_collection_ptr->get_element_class_id ();
      
      if (src_class_id != dst_class_id)
      {
         if_exec (Err_VM_ARRAY_BAD_ELEMENT_TYPE, "move_collection: %1% %2% != %3%")
            .arg_obj (dst_obj_ptr).arg_class (src_class_id).arg_class (dst_class_id);
         vm_TRACE2 ("BAD ELEMENT TYPE %1m16rw0X% %2m16rw0X%", old_ref, new_ref);
         assert (is_tolerant ());
         return Err_VM_COLLECTION_BAD_ELEMENT_TYPE;
      }
      
      Object * obj_ptr = _object_manager.get_object (pointed_ref);

      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "move_collection: %1m16rw0X% pointed").arg (pointed_ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", pointed_ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      int class_id = obj_ptr->get_cdm ().get_class_id ();
      
      if (!ClassDescManager::use_instance ().is_class_id_inheriting_from_class_id (
         class_id, dst_class_id
      ))
      {
         if_exec (Err_VM_ARRAY_BAD_ELEMENT_TYPE, "move_collection: %1% %2% not a base of %3%")
            .arg_obj (obj_ptr).arg_class (class_id).arg_class (dst_class_id);
         vm_TRACE1 ("BAD ELEMENT TYPE %1m16rw0X%", pointed_ref);
         assert (is_tolerant ());
         return Err_VM_COLLECTION_BAD_ELEMENT_TYPE;
      }
      
      int err = src_collection_ptr->move_no_notification (dst_collection_ptr, obj_ptr, is_tolerant ());
      
      if (err != Err_OK)
      {
         if_exec (err, "move_collection: %1%").arg_obj (obj_ptr);
         vm_TRACE2 ("MOVE FAILED %1m16rw0X% %2m16rw0X%", old_ref, new_ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : err;
      }
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : set_blob
==============================================================================
*/

int   VirtualMachine::set_blob (flip::Ref ref, archi::UInt32 data_size, const archi::UByte * data_ptr, archi::UInt32 old_data_size, const archi::UByte * old_data_ptr)
{
   assert (!is_auto_correct ());
   
   Object * obj_ptr = _object_manager.get_object (ref);

   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_blob: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_ACCESS;
      }
      
      Blob * blob_ptr = dynamic_cast <Blob *> (obj_ptr);

      if (blob_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_blob: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      if (data_size == old_data_size)
      {
         if (std::equal (data_ptr, data_ptr + data_size, old_data_ptr))
         {
            if_exec (Err_VM_VOID_INSTRUCTION, "set_blob: %1%").arg_obj (obj_ptr);
            vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
            assert (is_tolerant ());
            return Err_VM_VOID_INSTRUCTION;
         }
      }
      
      if (blob_ptr->size () != old_data_size)
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_blob: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VALUE COLLISION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_VALUE_COLLISION;
      }
      
      if (!std::equal (blob_ptr->get ().begin (), blob_ptr->get ().end (), old_data_ptr))
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_blob: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VALUE COLLISION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_VALUE_COLLISION;
      }
      
      blob_ptr->set_blob (data_ptr, data_size);
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : set_blob
==============================================================================
*/

int   VirtualMachine::set_blob (flip::Ref ref, archi::UInt32 data_size, const archi::UByte * data_ptr, std::vector <archi::UByte> & old_data)
{
   assert (is_auto_correct ());
   
   Object * obj_ptr = _object_manager.get_object (ref);

   if (obj_ptr == 0)
   {
      if_exec (Err_VM_BAD_ACCESS, "set_blob: %1m16rw0X%").arg (ref);
      vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
      // bad access
      return Err_VM_CORRECT_REMOVE_OPCODE;
   }
   
   Blob * blob_ptr = dynamic_cast <Blob *> (obj_ptr);

   if (blob_ptr == 0)
   {
      if_exec (Err_VM_BAD_TYPE, "set_blob: %1% got %2%")
         .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
      vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
      return Err_VM_BAD_TYPE;
   }
   
   int err = Err_OK;
   
   if (blob_ptr->size () != old_data.size ())
   {
      if_exec (Err_VM_VALUE_COLLISION, "set_blob: %1%").arg_obj (obj_ptr);
      vm_TRACE1 ("VALUE COLLISION %1m16rw0X%", ref);
      // value collision
      err = Err_VM_CORRECT_OLD_VAL;
   }
   
   if (err == Err_OK)
   {
      if (old_data.size () != 0)
      {
         if (!std::equal (blob_ptr->get ().begin (), blob_ptr->get ().end (), &old_data [0]))
         {
            if_exec (Err_VM_VALUE_COLLISION, "set_blob: %1%").arg_obj (obj_ptr);
            vm_TRACE1 ("VALUE COLLISION %1m16rw0X%", ref);
            // value collision
            err = Err_VM_CORRECT_OLD_VAL;
         }
      }
   }
   
   if (err == Err_VM_CORRECT_OLD_VAL)
   {
      old_data = blob_ptr->get ();
   }
   
   if (data_size == old_data.size ())
   {
      if (data_size == 0)
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_blob: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
      }
      else
      {
         if (std::equal (data_ptr, data_ptr + data_size, &old_data [0]))
         {
            if_exec (Err_VM_VOID_INSTRUCTION, "set_blob: %1%").arg_obj (obj_ptr);
            vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
            
            return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
         }
      }
   }
   
   blob_ptr->set_blob (data_ptr, data_size);
   
   return err;
}



/*
==============================================================================
Name : subset_blob
==============================================================================
*/

int   VirtualMachine::subset_blob (flip::Ref ref, archi::UInt32 data_pos, archi::UInt32 data_size, const archi::UByte * data_ptr, const archi::UByte * old_data_ptr)
{
   assert (!is_auto_correct ());
   
   Object * obj_ptr = _object_manager.get_object (ref);

   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_blob: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_ACCESS;
      }
      
      Blob * blob_ptr = dynamic_cast <Blob *> (obj_ptr);

      if (blob_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_blob: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      if (std::equal (data_ptr, data_ptr + data_size, old_data_ptr))
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_blob: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_VOID_INSTRUCTION;
      }
      
      if (!std::equal (
         blob_ptr->get ().begin () + data_pos,
         blob_ptr->get ().begin () + data_pos + data_size,
         old_data_ptr
      ))
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_blob: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VALUE COLLISION %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_VALUE_COLLISION;
      }
      
      blob_ptr->subset_blob (data_pos, data_ptr, data_size);
      
      return Err_OK;
   }
}



/*
==============================================================================
Name : subset_blob
==============================================================================
*/

int   VirtualMachine::subset_blob (flip::Ref ref, archi::UInt32 data_pos, archi::UInt32 data_size, const archi::UByte * data_ptr, std::vector <archi::UByte> & old_data)
{
   assert (is_auto_correct ());
   
   Object * obj_ptr = _object_manager.get_object (ref);

   if (obj_ptr == 0)
   {
      if_exec (Err_VM_BAD_ACCESS, "set_blob: %1m16rw0X%").arg (ref);
      vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
      // bad access
      return Err_VM_CORRECT_REMOVE_OPCODE;
   }
   
   Blob * blob_ptr = dynamic_cast <Blob *> (obj_ptr);

   if (blob_ptr == 0)
   {
      if_exec (Err_VM_BAD_TYPE, "set_blob: %1% got %2%")
         .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
      vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
      return Err_VM_BAD_TYPE;
   }
   
   int err = Err_OK;
   
   if (err == Err_OK)
   {
      if (!std::equal (
         blob_ptr->get ().begin () + data_pos,
         blob_ptr->get ().begin () + data_pos + data_size,
         &old_data [0]
      ))
      {
         if_exec (Err_VM_VALUE_COLLISION, "set_blob: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VALUE COLLISION %1m16rw0X%", ref);
         // value collision
         err = Err_VM_CORRECT_OLD_VAL;
      }
   }
   
   if (err == Err_VM_CORRECT_OLD_VAL)
   {
      old_data.assign (
         blob_ptr->get ().begin () + data_pos,
         blob_ptr->get ().begin () + data_pos + data_size
      );
   }
   
   if (data_size == 0)
   {
      if_exec (Err_VM_VOID_INSTRUCTION, "set_blob: %1%").arg_obj (obj_ptr);
      vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
      
      return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
   }
   else
   {
      if (std::equal (data_ptr, data_ptr + data_size, &old_data [0]))
      {
         if_exec (Err_VM_VOID_INSTRUCTION, "set_blob: %1%").arg_obj (obj_ptr);
         vm_TRACE1 ("VOID INSTRUCTION %1m16rw0X%", ref);
         
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_VOID_INSTRUCTION;
      }
   }
   
   blob_ptr->subset_blob (data_pos, data_ptr, data_size);
   
   return err;
}



/*
==============================================================================
Name : set_cue
==============================================================================
*/

int   VirtualMachine::set_cue (flip::Ref ref, archi::UInt32 data_size, const archi::UByte * data_ptr, bool direction_normal_flag)
{
   Object * obj_ptr = _object_manager.get_object (ref);

   if (is_conflict_marking ())
   {
      if (obj_ptr != 0)
      {
         obj_ptr->set_conflict ();
      }
      
      return Err_OK;
   }
   else
   {
      if (obj_ptr == 0)
      {
         if_exec (Err_VM_BAD_ACCESS, "set_cue: %1m16rw0X%").arg (ref);
         vm_TRACE1 ("BAD ACCESS %1m16rw0X%", ref);
         assert (is_tolerant ());
         return is_auto_correct () ? Err_VM_CORRECT_REMOVE_OPCODE : Err_VM_BAD_ACCESS;
      }
      
      Cue * cue_ptr = dynamic_cast <Cue *> (obj_ptr);

      if (cue_ptr == 0)
      {
         if_exec (Err_VM_BAD_TYPE, "set_cue: %1% got %2%")
            .arg_obj (obj_ptr).arg (obj_ptr->cdm_name);
         vm_TRACE1 ("BAD TYPE %1m16rw0X%", ref);
         assert (is_tolerant ());
         return Err_VM_BAD_TYPE;
      }
      
      cue_ptr->set_cue (data_ptr, data_size, direction_normal_flag);
      
      return Err_OK;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : is_tolerant
==============================================================================
*/

bool  VirtualMachine::is_tolerant () const
{
   return _mode != Mode_NO_FAULT;
}



/*
==============================================================================
Name : is_auto_correct
==============================================================================
*/

bool  VirtualMachine::is_auto_correct () const
{
   return _mode == Mode_AUTO_CORRECT;
}



/*
==============================================================================
Name : is_conflict_marking
==============================================================================
*/

bool  VirtualMachine::is_conflict_marking () const
{
   return _mode == Mode_CONFLICT_MARKING;
}



/*
==============================================================================
Name : execute_normal
==============================================================================
*/

int   VirtualMachine::execute_normal (MachineCode & code)
{
   if (code.get_nbr_opcodes () == 0)
   {
      assert (false);
      return Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   int bad_opcode_nbr = 0;
   
   int err = Err_OK;
   
   for (int i = 0 ; i < code.get_nbr_opcodes () ;)
   {
      err = code.exec (*this, i, true);
      
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         /*// don't advance as the opcode has been
         // removed*/
         
         ++i;

         err = Err_OK;
      }
      else if (err != Err_OK)
      {
         assert (is_tolerant ());
         
         bad_opcode_nbr = i;
         break;
      }
      else
      {
         ++i;
      }
   }
   
   if (err != Err_OK)
   {
      if (bad_opcode_nbr > 0)
      {
         --bad_opcode_nbr;
         
         for (int i = bad_opcode_nbr ; i >= 0 ; --i)
         {
            int err_b = code.exec (*this, i, false);
            
            if (err_b != Err_OK)
            {
               assert (false);
               return Err_VM_INTERNAL_SYSTEM_ERROR;
            }
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : execute_reverse
==============================================================================
*/

int   VirtualMachine::execute_reverse (MachineCode & code)
{
   int nbr_opcodes = code.get_nbr_opcodes ();
   
   if (nbr_opcodes == 0)
   {
      assert (false);
      return Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   int bad_opcode_nbr = 0;
   
   int err = Err_OK;
   
   for (int i = nbr_opcodes - 1 ; i >= 0 ; --i)
   {
      err = code.exec (*this, i, false);
      
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         /*// don't advance, but the opcode nbr
         // is ok*/
         
         err = Err_OK;
      }
      else if (err != Err_OK)
      {
         assert (is_tolerant ());
         
         bad_opcode_nbr = i;
         break;
      }
   }
   
   if (err != Err_OK)
   {
      if (bad_opcode_nbr < nbr_opcodes - 1)
      {
         ++bad_opcode_nbr;
         
         for (int i = bad_opcode_nbr ; i < nbr_opcodes ; ++i)
         {
            int err_b = code.exec (*this, i, true);
            
            if (err_b != Err_OK)
            {
               assert (false);
               return Err_VM_INTERNAL_SYSTEM_ERROR;
            }
         }
      }
   }
   
   return err;
}



#undef if_report
#undef if_comp
#undef if_exec



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
