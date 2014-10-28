/*****************************************************************************

        MachineCode.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70517

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

#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/VirtualMachine.h"
#include "ohm/flip/private/BinaryKey.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/ValidationReportWriter.h"

#include "ohm/archi/def.h"
#include "ohm/lang/fnc.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/trace.h"

#include <cassert>

#undef OHM_FLIP_MACHINE_CODE_DEBUG_FLAG
#undef OHM_FLIP_MACHINE_CODE_DEBUG_VALIDATE_FLAG
#undef OHM_FLIP_MACHINE_CODE_DEBUG_AUTO_CORRECT_FLAG

#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_FLIP_MACHINE_CODE_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define mc_TRACE(msg_0) ohm_util_TRACE_DEBUG(msg_0)
   #define mc_TRACE1(fmt_0, arg1)   ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define mc_TRACE2(fmt_0, arg1, arg2)   ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define mc_TRACE3(fmt_0, arg1, arg2, arg3)   ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define mc_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define mc_TRACE(msg_0) (void (0))
   #define mc_TRACE1(fmt_0, arg1)   (void (0))
   #define mc_TRACE2(fmt_0, arg1, arg2)   (void (0))
   #define mc_TRACE3(fmt_0, arg1, arg2, arg3)   (void (0))
   #define mc_TRACE4(fmt_0, arg1, arg2, arg3, arg4)   (void (0))
#endif

#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
   #if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
      #define dump dump_machine_code (__FUNCDNAME__, __LINE__, pos, is_safe_tolerant ())

   #elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
      #define dump dump_machine_code (__PRETTY_FUNCTION__, __LINE__, pos, is_safe_tolerant ())

   #else
      #error Compiler not supported

   #endif

#else
   #define dump (void (0))

#endif

#define if_report if (_report_ptr != 0) _report_ptr
#define if_comp if (_report_ptr != 0) _report_ptr->print_compile_error
#define if_exec if (_report_ptr != 0) _report_ptr->print_execution_error


namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : build_bool_set
==============================================================================
*/

void  MachineCode::build_bool_set (Data & tx_data, flip::Ref this_ref, bool old_val, bool new_val)
{
   mc_TRACE3 ("BOL SET %1m16rw0X% %2% (%3%)", this_ref, new_val ? "TRUE" : "FALSE", old_val ? "TRUE" : "FALSE");
   
   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_BOOL | MachineCode::Command_SET));
   bso.write (this_ref);
   bso.write (archi::UByte (new_val ? 1 : 0));
   bso.write (archi::UByte (old_val ? 1 : 0));
}
   


/*
==============================================================================
Name : build_int64_set
==============================================================================
*/

void  MachineCode::build_int64_set (Data & tx_data, flip::Ref this_ref, archi::Int64 old_val, archi::Int64 new_val)
{
   mc_TRACE3 ("I64 SET %1m16rw0X% %2% (%3%)", this_ref, new_val, old_val);
   
   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_INT64 | MachineCode::Command_SET));
   bso.write (this_ref);
   bso.write (new_val);
   bso.write (old_val);
}
   


/*
==============================================================================
Name : build_enum_set
==============================================================================
*/

void  MachineCode::build_enum_set (Data & tx_data, flip::Ref this_ref, archi::Int64 old_val, archi::Int64 new_val)
{
   mc_TRACE3 ("ENU SET %1m16rw0X% %2% (%3%)", this_ref, new_val, old_val);
   
   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_ENUM | MachineCode::Command_SET));
   bso.write (this_ref);
   bso.write (new_val);
   bso.write (old_val);
}
   


/*
==============================================================================
Name : build_float64_set
==============================================================================
*/

void  MachineCode::build_float64_set (Data & tx_data, flip::Ref this_ref, archi::Float64 old_val, archi::Float64 new_val)
{
   mc_TRACE3 ("F64 SET %1m16rw0X% %2% (%3%)", this_ref, new_val, old_val);
   
   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_FLOAT64 | MachineCode::Command_SET));
   bso.write (this_ref);
   bso.write (new_val);
   bso.write (old_val);
}
   


/*
==============================================================================
Name : build_blob_set
==============================================================================
*/

void  MachineCode::build_blob_set (Data & tx_data, flip::Ref this_ref, const archi::UByte * old_data_ptr, archi::UInt32 old_data_size, const archi::UByte * new_data_ptr, archi::UInt32 new_data_size)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      std::string new_data_str;
      new_data_str.assign (&new_data_ptr [0], &new_data_ptr [new_data_size]);

      std::string old_data_str;
      old_data_str.assign (&old_data_ptr [0], &old_data_ptr [old_data_size]);

      mc_TRACE3 ("BLO SET %1m16rw0X% %2% (%3%)", this_ref, new_data_str.c_str (), old_data_str.c_str ());
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_BLOB | MachineCode::Command_SET));
   bso.write (this_ref);
   bso.write (new_data_size);
   bso.write (new_data_ptr, new_data_size);
   bso.write (old_data_size);
   bso.write (old_data_ptr, old_data_size);
}
   


/*
==============================================================================
Name : build_blob_subset
==============================================================================
*/

void  MachineCode::build_blob_subset (Data & tx_data, flip::Ref this_ref, archi::UInt32 data_pos, archi::UInt32 data_size, const archi::UByte * old_data_ptr, const archi::UByte * new_data_ptr)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      std::string new_data_str;
      new_data_str.assign (&new_data_ptr [0], &new_data_ptr [data_size]);

      std::string old_data_str;
      old_data_str.assign (&old_data_ptr [0], &old_data_ptr [data_size]);

      mc_TRACE4 ("BLO SET %1m16rw0X% %2% %3% (%4%)", this_ref, data_pos, new_data_str.c_str (), old_data_str.c_str ());
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_BLOB | MachineCode::Command_SUBSET));
   bso.write (this_ref);
   bso.write (data_pos);
   bso.write (data_size);
   bso.write (new_data_ptr, data_size);
   bso.write (old_data_ptr, data_size);
}
   


/*
==============================================================================
Name : build_object_ref_set
==============================================================================
*/

void  MachineCode::build_object_ref_set (Data & tx_data, flip::Ref this_ref, flip::Ref old_ref, flip::Ref new_ref)
{
   mc_TRACE3 ("REF SET %1m16rw0X% %2m16rw0X% (%3m16rw0X%)", this_ref, new_ref, old_ref);
   
   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_OBJECT_REF | MachineCode::Command_SET));
   bso.write (this_ref);
   bso.write (new_ref);
   bso.write (old_ref);
}
   


/*
==============================================================================
Name : build_cue
==============================================================================
*/

void  MachineCode::build_cue (Data & tx_data, flip::Ref this_ref, const archi::UByte * data_ptr, archi::UInt32 data_size)
{
   mc_TRACE1 ("SIG SET %1m16rw0X%", this_ref);
   
   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_CUE | MachineCode::Command_SET));
   bso.write (this_ref);
   bso.write (data_size);
   bso.write (data_ptr, data_size);
}
   


/*
==============================================================================
Name : build_array_insert
==============================================================================
*/

void  MachineCode::build_array_insert (Data & tx_data, flip::Ref this_ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      std::string key_str;
      key.write (key_str);
      mc_TRACE4 ("ARR INS %1m16rw0X% %2% %3m16rw0X% %4m8rw0X%", this_ref, key_str.c_str (), pointed_ref, class_id);
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_ARRAY | MachineCode::Command_INSERT));
   bso.write (this_ref);
   bso.write (key);
   bso.write (pointed_ref);
   bso.write (class_id);
}
   


/*
==============================================================================
Name : build_array_erase
==============================================================================
*/

void  MachineCode::build_array_erase (Data & tx_data, flip::Ref this_ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      std::string key_str;
      key.write (key_str);
      mc_TRACE4 ("ARR ERS %1m16rw0X% %2% %3m16rw0X% %4m8rw0X%", this_ref, key_str.c_str (), pointed_ref, class_id);
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_ARRAY | MachineCode::Command_ERASE));
   bso.write (this_ref);
   bso.write (key);
   bso.write (pointed_ref);
   bso.write (class_id);
}
   


/*
==============================================================================
Name : build_array_move
==============================================================================
*/

void  MachineCode::build_array_move (Data & tx_data, flip::Ref old_ref, const BinaryKey & old_key, flip::Ref new_ref, const BinaryKey & new_key)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      std::string old_key_str;
      old_key.write (old_key_str);
      std::string new_key_str;
      new_key.write (new_key_str);
      mc_TRACE4 ("ARR MOV %1m16rw0X% %2% (%3m16rw0X% %4%)", new_ref, new_key_str.c_str (), old_ref, old_key_str.c_str ());
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_ARRAY | MachineCode::Command_MOVE));
   bso.write (new_ref);
   bso.write (new_key);
   bso.write (old_ref);
   bso.write (old_key);
}
   


/*
==============================================================================
Name : build_collection_insert
==============================================================================
*/

void  MachineCode::build_collection_insert (Data & tx_data, flip::Ref this_ref, flip::Ref pointed_ref, archi::UInt32 class_id)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      mc_TRACE3 ("COL INS %1m16rw0X% %2m16rw0X% %3m8rw0X%", this_ref, pointed_ref, class_id);
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_COLLECTION | MachineCode::Command_INSERT));
   bso.write (this_ref);
   bso.write (pointed_ref);
   bso.write (class_id);
}
   


/*
==============================================================================
Name : build_collection_erase
==============================================================================
*/

void  MachineCode::build_collection_erase (Data & tx_data, flip::Ref this_ref, flip::Ref pointed_ref, archi::UInt32 class_id)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      mc_TRACE3 ("COL ERS %1m16rw0X% %2m16rw0X% %3m8rw0X%", this_ref, pointed_ref, class_id);
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_COLLECTION | MachineCode::Command_ERASE));
   bso.write (this_ref);
   bso.write (pointed_ref);
   bso.write (class_id);
}
   


/*
==============================================================================
Name : build_collection_move
==============================================================================
*/

void  MachineCode::build_collection_move (Data & tx_data, flip::Ref old_ref, flip::Ref pointed_ref, flip::Ref new_ref)
{
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
      mc_TRACE3 ("COL MOV %1m16rw0X% %2m16rw0X% (%3m16rw0X%)", pointed_ref, new_ref, old_ref);
#endif

   util::BinaryStreamOutput bso (tx_data);
   
   bso.write (archi::UByte (MachineCode::Target_COLLECTION | MachineCode::Command_MOVE));
   bso.write (new_ref);
   bso.write (old_ref);
   bso.write (pointed_ref);
}


/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

MachineCode::MachineCode (const archi::UByte * data_ptr, size_t data_size)
:  _data_ptr (data_ptr)
,  _data_size (data_size)
,  _corrected_flag (false)
,  _corrected_data_sptr ()
,  _mode (Mode_NONE)
,  _report_ptr (0)
,  _pos_arr ()
{
   assert (data_ptr != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MachineCode::~MachineCode ()
{
}



/*
==============================================================================
Name : set_mode_safe
==============================================================================
*/

void  MachineCode::set_mode_safe ()
{
   _mode = Mode_SAFE;
}



/*
==============================================================================
Name : set_mode_no_fault
==============================================================================
*/

void  MachineCode::set_mode_no_fault ()
{
   _mode = Mode_NO_FAULT;
}



/*
==============================================================================
Name : set_mode_auto_correct
==============================================================================
*/

void  MachineCode::set_mode_auto_correct ()
{
   _mode = Mode_AUTO_CORRECT;
   
   _corrected_data_sptr = DataSPtr (new Data);
   _corrected_flag = false;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  MachineCode::bind (ValidationReportWriter & report)
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

int   MachineCode::compile ()
{
   int err = 0;
   
   if (_data_size == 0)
   {
      assert (is_safe_tolerant ());
      err = Err_VM_BOOTLEG;
   }
   else
   {
      assert (_data_ptr != 0);
      
      err = split_opcodes ();
   }
   
   if (err != 0) if_comp (err);
   
   return err;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

int   MachineCode::get_nbr_opcodes () const
{
   return _pos_arr.size ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

int   MachineCode::exec (VirtualMachine & virtual_machine, int opcode_nbr, bool direction_normal_flag)
{
   bool ok_flag = true;
   ok_flag &= (opcode_nbr >= 0);
   ok_flag &= (size_t (opcode_nbr) < _pos_arr.size ());

   if (!ok_flag)
   {
      if_exec (Err_VM_INTERNAL_SYSTEM_ERROR, "MachineCode::exec");
      
      assert (false);
      return Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   size_t pos = _pos_arr [opcode_nbr];
   
   if (_report_ptr != 0)
   {
      virtual_machine.bind (*_report_ptr);
   }
   
   int err = exec_opcode (virtual_machine, pos, direction_normal_flag);

#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_VALIDATE_FLAG)  
   if (_corrected_flag)
   {
      const Data & data = *_corrected_data_sptr;
      MachineCode code (&data [0], data.size ());
      
      code.set_mode_no_fault ();
      
      int err = code.compile ();
      assert (err == 0);
      
      if (err != 0)
      {
         ohm_util_TRACE_ERROR1 ("opcode_nbr=%1%", opcode_nbr);
         
         trace_opcodes ();
      }
   }
#endif

#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_AUTO_CORRECT_FLAG)
   if (_mode == Mode_AUTO_CORRECT)
   {
      const Data & data = *_corrected_data_sptr;
      MachineCode code (&data [0], data.size ());

      ohm_util_TRACE_INFO ("Mode_AUTO_CORRECT trace");
      code.trace_opcodes ();
      ohm_util_TRACE_INFO ("");
   }
#endif
   
   if (_report_ptr != 0)
   {
      virtual_machine.unbind (*_report_ptr);
   }
   
   return err;
}



/*
==============================================================================
Name : has_corrected_tx
==============================================================================
*/

bool  MachineCode::has_corrected_tx () const
{
   assert (_mode == Mode_AUTO_CORRECT);

   return _corrected_flag;
}



/*
==============================================================================
Name : get_corrected_tx_data
==============================================================================
*/

DataSPtr MachineCode::get_corrected_tx_data () const
{
   assert (_mode == Mode_AUTO_CORRECT);
   assert (has_corrected_tx ());
   
   return _corrected_data_sptr;
}



/*
==============================================================================
Name : get_container_elem
==============================================================================
*/

int   MachineCode::get_container_elem (RefContainerElemMap & ref_map)
{
   ref_map.clear ();
   
   set_mode_no_fault ();
   
   for (size_t i = 0 ; i < _pos_arr.size () ; ++i)
   {
      size_t pos = _pos_arr [i];

      archi::UByte opcode = _data_ptr [pos];
      ++pos;
      
      int target = opcode & 0xF0;
      int command = opcode & 0x0F;
      
      if ((target == Target_ARRAY) && (command == Command_INSERT))
      {
         ContainerElem container_elem;
         flip::Ref pointed_ref = 0;
         const bool tolerant_flag = false;
      
         int err = read (container_elem._parent_ref, pos);
         if (err != Err_OK) return err;
         
         archi::UInt32 key_bytes_size = 0;
         err = read (key_bytes_size, pos);   if (err != Err_OK) return err;
         
         err = container_elem._key.read (&_data_ptr [pos], key_bytes_size, tolerant_flag);
         if (err) return err;
         
         pos += key_bytes_size;

         err = read (pointed_ref, pos);   if (err != Err_OK) return err;

         bool ok_flag = ref_map.insert (
            std::make_pair (pointed_ref, container_elem)
         ).second;
         assert (ok_flag);
      }
      else if ((target == Target_COLLECTION) && (command == Command_INSERT))
      {
         ContainerElem container_elem;
         flip::Ref pointed_ref = 0;
      
         int err = read (container_elem._parent_ref, pos);  if (err != Err_OK) return err;
         
         err = read (pointed_ref, pos);   if (err != Err_OK) return err;

         bool ok_flag = ref_map.insert (
            std::make_pair (pointed_ref, container_elem)
         ).second;
         assert (ok_flag);
      }
   }
   
   int err = get_value_elem (ref_map);
   
   return err;
}
   


/*
==============================================================================
Name : get_value_elem
Description :
   Fill value of Float64 or Int64 for ref already present in ref_map
==============================================================================
*/

int   MachineCode::get_value_elem (RefContainerElemMap & ref_map)
{
   set_mode_no_fault ();
   
   for (size_t i = 0 ; i < _pos_arr.size () ; ++i)
   {
      size_t pos = _pos_arr [i];

      archi::UByte opcode = _data_ptr [pos];
      ++pos;
      
      int target = opcode & 0xF0;
      int command = opcode & 0x0F;
      
      int err = Err_OK;
      
      if ((target == Target_INT64) && (command == Command_SET))
      {
         flip::Ref pointed_ref = 0;
      
         err = read (pointed_ref, pos);
         if (err != Err_OK) return err;
         
         RefContainerElemMap::iterator it = ref_map.find (pointed_ref);
         
         if (it != ref_map.end ())
         {
            ContainerElem & container_elem = it->second;
            
            err = read (container_elem._val_i64, pos);
            if (err != Err_OK) return err;
         }
      }
      else if ((target == Target_FLOAT64) && (command == Command_SET))
      {
         flip::Ref pointed_ref = 0;
      
         err = read (pointed_ref, pos);
         if (err != Err_OK) return err;
         
         RefContainerElemMap::iterator it = ref_map.find (pointed_ref);
         
         if (it != ref_map.end ())
         {
            ContainerElem & container_elem = it->second;
            
            err = read (container_elem._val_f64, pos);
            if (err != Err_OK) return err;
         }
      }
   }
   
   return Err_OK;
}
   


/*
==============================================================================
Name : inverse
Description :
   Inverse the current transaction and store it into 'dst'
Note :
   This is used in the 'DocumentDifferentiator', to produce a transaction
   that contains a container element erase.
==============================================================================
*/

void  MachineCode::inverse (Data & dst)
{
   if (_data_size == 0)
   {
      // inverse of null is null
      
      return;
   }
   
   set_mode_no_fault ();
   
   assert (!_pos_arr.empty ());
   
   for (size_t i = 0 ; i < _pos_arr.size () ; ++i)
   {
      size_t pos = _pos_arr [i];
      
      inverse_opcode (dst, pos);
   }
}



/*
==============================================================================
Name : trace_hex_ascii
==============================================================================
*/

void  MachineCode::trace_hex_ascii () const
{
   ohm_util_TRACE_INFO ("*** Machine Code Dump Hex ***");

   for (size_t i = 0 ; i < _data_size ; i += 16)
   {
      txt::out::Print printer;
      
      printer.print ("");
   
      size_t len = lang::min (size_t (16), _data_size - i);
      
      size_t j = 0;
      
      for (; j < len ; ++j)
      {
         archi::UByte byte = _data_ptr [i + j];
         
         printer.cat ("%1m2rw0x%").arg (byte);
         
         if (j % 4 == 3)
         {
            printer.cat (" ");
         }
      }
      
      for (; j < 16 ; ++j)
      {
         printer.cat ("  ");
         if (j % 4 == 3)
         {
            printer.cat (" ");
         }
      }
      
      printer.cat (" ");
      
      for (size_t j = 0 ; j < len ; ++j)
      {
         char c = char (_data_ptr [i + j]);
         
         if ((c >= 32) && (c <= 126))
         {
            char c_0 [2];
            c_0 [0] = c;
            c_0 [1] = '\0';
         
            printer.cat ("%1%").arg (c_0);
         }
         else
         {
            printer.cat (".");
         }
      }
      
      const std::string & str = printer;

      ohm_util_TRACE_INFO1 ("%1%", str.c_str ());
   }
}



/*
==============================================================================
Name : trace_opcodes
Description :
   Trace opcodes as long as we can
==============================================================================
*/

void  MachineCode::trace_opcodes () const
{
   size_t pos = 0;
         
   for (; pos < _data_size ;)
   {
      txt::out::Print printer;
      printer.print ("");
      
      archi::UByte opcode = _data_ptr [pos];
      ++pos;
      
      int target = opcode & 0xF0;
      int command = opcode & 0x0F;
      
      bool continue_flag = true;
      
      switch (target)
      {
      case Target_BOOL:
         printer.cat ("BOL ");
         break;

      case Target_INT64:
         printer.cat ("I64 ");
         break;

      case Target_ENUM:
         printer.cat ("ENU ");
         break;

      case Target_FLOAT64:
         printer.cat ("F64 ");
         break;

      case Target_BLOB:
         printer.cat ("BLO ");
         break;

      case Target_ARRAY:
         printer.cat ("ARR ");
         break;

      case Target_COLLECTION:
         printer.cat ("COL ");
         break;

      case Target_OBJECT_REF:
         printer.cat ("REF ");
         break;

      case Target_CUE:
         printer.cat ("CUE ");
         break;
      
      default:
         printer.cat ("??? ");
         continue_flag = false;
         break;
      }
      
      switch (command)
      {
      case Command_SET:
         printer.cat ("SET ");
         break;

      case Command_INSERT:
         printer.cat ("INS ");
         break;

      case Command_ERASE:
         printer.cat ("ERS ");
         break;

      case Command_MOVE:
         printer.cat ("MOV ");
         break;
      
      case Command_SUBSET:
         printer.cat ("SST ");
         break;
      
      default:
         printer.cat ("??? ");
         continue_flag = false;
         break;
      }
      
      const std::string & str = printer;
         
      if (!continue_flag)
      {
         ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
      
         break;
      }
      
      switch (target)
      {
      case Target_BOOL:
         pos += 8 + 1 + 1;
         break;

      case Target_INT64:
      case Target_ENUM:
      case Target_FLOAT64:
      case Target_OBJECT_REF:
         pos += 8 + 8 + 8;
         break;

      case Target_BLOB:
         if (command == Command_SET)
         {
            pos += 8;
            archi::UInt32 size = 0;
            int err = read (size, pos);
            if (err != Err_OK)
            {
               ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
               
               break;
            }
            
            pos += size;

            err = read (size, pos);
            if (err != Err_OK)
            {
               ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
               
               break;
            }
            
            pos += size;
         }
         else if (command == Command_SUBSET)
         {
            pos += 8 + 4;
            archi::UInt32 size = 0;
            int err = read (size, pos);
            if (err != Err_OK)
            {
               ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
               
               break;
            }
            
            pos += size;
            pos += size;
         }
         break;

      case Target_ARRAY:
         if (command == Command_MOVE)
         {
            pos += 8;
            archi::UInt32 size = 0;
            int err = read (size, pos);
            if (err != Err_OK)
            {
               ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
               
               break;
            }
            
            pos += size;
            pos += 8;

            err = read (size, pos);
            if (err != Err_OK)
            {
               ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
               
               break;
            }
            
            pos += size;
         }
         else
         {
            pos += 8;
            archi::UInt32 size = 0;
            int err = read (size, pos);
            if (err != Err_OK)
            {
               ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
               
               break;
            }
            
            pos += size;
            pos += 8 + 4;
         }
         break;

      case Target_COLLECTION:
         if (command == Command_MOVE)
         {
            pos += 8 + 8 + 8;
         }
         else
         {
            pos += 8 + 8 + 4;
         }
         break;

      case Target_CUE:
         {
            pos += 8;
            archi::UInt32 size = 0;
            int err = read (size, pos);
            if (err != Err_OK)
            {
               ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
               
               break;
            }
            
            pos += size;
         }
         break;
      }
      
      if (pos > _data_size)
      {
         ohm_util_TRACE_INFO1 ("%1% FAILED", str.c_str ());
         
         break;
      }
      else
      {
         ohm_util_TRACE_INFO1 ("%1% OK", str.c_str ());
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : exec_opcode
==============================================================================
*/

int   MachineCode::exec_opcode (VirtualMachine & virtual_machine, size_t pos, bool direction_normal_flag)
{
   archi::UByte opcode = _data_ptr [pos];
   ++pos;
   
   int target = opcode & 0xF0;
   int command = opcode & 0x0F;
   
   int err = Err_OK;
   
   switch (target)
   {
   case Target_BOOL:
      err = exec_opcode_bool (virtual_machine, command, pos, direction_normal_flag);
      break;
      
   case Target_INT64:
      err = exec_opcode_int64 (virtual_machine, command, pos, direction_normal_flag);
      break;
      
   case Target_ENUM:
      err = exec_opcode_enum (virtual_machine, command, pos, direction_normal_flag);
      break;
      
   case Target_FLOAT64:
      err = exec_opcode_float64 (virtual_machine, command, pos, direction_normal_flag);
      break;
      
   case Target_OBJECT_REF:
      err = exec_opcode_object_ref (virtual_machine, command, pos, direction_normal_flag);
      break;

   case Target_ARRAY:
      err = exec_opcode_array (virtual_machine, command, pos, direction_normal_flag);
      break;

   case Target_COLLECTION:
      err = exec_opcode_collection (virtual_machine, command, pos, direction_normal_flag);
      break;

   case Target_BLOB:
      err = exec_opcode_blob (virtual_machine, command, pos, direction_normal_flag);
      break;
      
   case Target_CUE:
      // direction agnostic
      err = exec_opcode_cue (virtual_machine, command, pos, direction_normal_flag);
      break;
      
   default:
      // this error should have been detected at compile stage
      if_exec (Err_VM_INTERNAL_SYSTEM_ERROR, "MachineCode::exec_opcode");
      err = Err_VM_INTERNAL_SYSTEM_ERROR;
      assert (false);
      break;
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_bool
==============================================================================
*/

int   MachineCode::exec_opcode_bool (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   // ref new old
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::UByte val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::UByte old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   if (!direction_normal_flag)
   {
      std::swap (val, old_val);
   }
   
   mc_TRACE2 ("BOL SET %1m16rw0X% %2%", ref, val);
   
   err = virtual_machine.set_bool (ref, val, old_val);
   
   if (is_auto_correct ())
   {
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         if (direction_normal_flag)
         {
            build_bool_set (*_corrected_data_sptr, ref, old_val, val);
         }
         else
         {
            Data data;
   
            build_bool_set (data, ref, val, old_val);
            
            _corrected_data_sptr->insert (
               _corrected_data_sptr->begin (), data.begin (), data.end ()
            );
         }
         
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }
   }
   else
   {
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_int64
==============================================================================
*/

int   MachineCode::exec_opcode_int64 (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   // ref new old
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::Int64 val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::Int64 old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   if (!direction_normal_flag)
   {
      std::swap (val, old_val);
   }
   
   mc_TRACE2 ("I64 SET %1m16rw0X% %2%", ref, val);
   
   err = virtual_machine.set_int64 (ref, val, old_val);
   
   if (is_auto_correct ())
   {
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         if (direction_normal_flag)
         {
            build_int64_set (*_corrected_data_sptr, ref, old_val, val);
         }
         else
         {
            Data data;
   
            build_int64_set (data, ref, val, old_val);
            
            _corrected_data_sptr->insert (
               _corrected_data_sptr->begin (), data.begin (), data.end ()
            );
         }
         
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }
   }
   else
   {
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_enum
==============================================================================
*/

int   MachineCode::exec_opcode_enum (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   // ref new old
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::Int64 val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::Int64 old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   if (!direction_normal_flag)
   {
      std::swap (val, old_val);
   }
   
   mc_TRACE2 ("ENU SET %1m16rw0X% %2%", ref, val);
   
   err = virtual_machine.set_enum (ref, val, old_val);

   if (is_auto_correct ())
   {
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         if (direction_normal_flag)
         {
            build_enum_set (*_corrected_data_sptr, ref, old_val, val);
         }
         else
         {
            Data data;
   
            build_enum_set (data, ref, val, old_val);
            
            _corrected_data_sptr->insert (
               _corrected_data_sptr->begin (), data.begin (), data.end ()
            );
         }
         
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }
   }
   else
   {
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_float64
==============================================================================
*/

int   MachineCode::exec_opcode_float64 (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   // ref new old
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::Float64 val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::Float64 old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   if (!direction_normal_flag)
   {
      std::swap (val, old_val);
   }
   
   mc_TRACE2 ("F64 SET %1m16rw0X% %2%", ref, val);
   
   err = virtual_machine.set_float64 (ref, val, old_val);
   
   if (is_auto_correct ())
   {
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         if (direction_normal_flag)
         {
            build_float64_set (*_corrected_data_sptr, ref, old_val, val);
         }
         else
         {
            Data data;
   
            build_float64_set (data, ref, val, old_val);
            
            _corrected_data_sptr->insert (
               _corrected_data_sptr->begin (), data.begin (), data.end ()
            );
         }
         
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }
   }
   else
   {
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_object_ref
==============================================================================
*/

int   MachineCode::exec_opcode_object_ref (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   // ref new_pointed old_pointed
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;
   
   flip::Ref pointed_ref = 0;
   err = read (pointed_ref, pos);   if (err != Err_OK) return err;
   
   flip::Ref old_pointed_ref = 0;
   err = read (old_pointed_ref, pos);  if (err != Err_OK) return err;
   
   if (!direction_normal_flag)
   {
      std::swap (pointed_ref, old_pointed_ref);
   }
   
   mc_TRACE2 ("REF SET %1m16rw0X% %2m16rw0X%", ref, pointed_ref);
   
   err = virtual_machine.set_object_ref (ref, pointed_ref, old_pointed_ref);
   
   if (is_auto_correct ())
   {
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         if (direction_normal_flag)
         {
            build_object_ref_set (*_corrected_data_sptr, ref, old_pointed_ref, pointed_ref);
         }
         else
         {
            Data data;
   
            build_object_ref_set (data, ref, pointed_ref, old_pointed_ref);
            
            _corrected_data_sptr->insert (
               _corrected_data_sptr->begin (), data.begin (), data.end ()
            );
         }
         
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }
   }
   else
   {
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_array
==============================================================================
*/

int   MachineCode::exec_opcode_array (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   if (command == Command_MOVE)
   {
      // new_ref new_key old_ref old_key
      
      size_t opcode_pos = pos - 1;
      
      flip::Ref new_ref = 0;
      int err = read (new_ref, pos);   if (err != Err_OK) return err;
      
      archi::UInt32 new_key_bytes_size = 0;
      err = read (new_key_bytes_size, pos);  if (err != Err_OK) return err;

      BinaryKey new_key;
      err = new_key.read (&_data_ptr [pos], new_key_bytes_size, is_safe_tolerant ());
      if (err != Err_OK) return err;
      
      pos += new_key_bytes_size;
      
      flip::Ref old_ref = 0;
      err = read (old_ref, pos); if (err != Err_OK) return err;
      
      archi::UInt32 old_key_bytes_size = 0;
      err = read (old_key_bytes_size, pos);  if (err != Err_OK) return err;

      BinaryKey old_key;
      err = old_key.read (&_data_ptr [pos], old_key_bytes_size, is_safe_tolerant ());
      if (err != Err_OK) return err;
      
      pos += old_key_bytes_size;
      
      if (direction_normal_flag)
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         std::string old_key_str;
         old_key.write (old_key_str);
         std::string new_key_str;
         new_key.write (new_key_str);
         mc_TRACE4 ("ARR MOV %1m16rw0X% %2% <- %3m16rw0X% %4%", new_ref, new_key_str.c_str (), old_ref, old_key_str.c_str ());
#endif
      
         err = virtual_machine.move_array (new_ref, new_key, old_ref, old_key);
      }
      else
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         std::string old_key_str;
         old_key.write (old_key_str);
         std::string new_key_str;
         new_key.write (new_key_str);
         mc_TRACE4 ("ARR MOV %1m16rw0X% %2% <- %3m16rw0X% %4%", old_ref, old_key_str.c_str (), new_ref, new_key_str.c_str ());
#endif
      
         err = virtual_machine.move_array (old_ref, old_key, new_ref, new_key);
      }
      
      if (is_auto_correct ())
      {
         if (err == Err_VM_CORRECT_REMOVE_OPCODE)
         {
            _corrected_flag = true;
            
            err = Err_OK;
         }
         else if (err == Err_VM_CORRECT_OLD_VAL)
         {
            assert (false);
         }
         else if (err == Err_OK)
         {
            _corrected_data_sptr->insert (
               direction_normal_flag
                  ? _corrected_data_sptr->end ()
                  : _corrected_data_sptr->begin (),
               &_data_ptr [opcode_pos], &_data_ptr [pos]
            );
         }
      }
      else
      {
         if (err != Err_OK)
         {
            mc_TRACE1 ("FAILED %1%", err);
            return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
         }
      }
   }
   else
   {
      // ref key pointed class_id
      
      size_t opcode_pos = pos - 1;
      
      bool insert_flag = (command == Command_INSERT) ^ !direction_normal_flag;

      flip::Ref ref = 0;
      int err = read (ref, pos); if (err != Err_OK) return err;

      archi::UInt32 key_bytes_size = 0;
      err = read (key_bytes_size, pos);   if (err != Err_OK) return err;

      BinaryKey key;
      err = key.read (&_data_ptr [pos], key_bytes_size, is_safe_tolerant ());
      if (err) return err;

      pos += key_bytes_size;

      flip::Ref pointed_ref = 0;
      err = read (pointed_ref, pos);   if (err != Err_OK) return err;

      archi::UInt32 class_id = 0;
      err = read (class_id, pos);   if (err != Err_OK) return err;

      if (insert_flag)
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         std::string key_str;
         key.write (key_str);
         mc_TRACE4 ("ARR INS %1m16rw0X% %2% %3m16rw0X% %4m8rw0X%", ref, key_str.c_str (), pointed_ref, class_id);
#endif

         err = virtual_machine.insert_array (ref, key, pointed_ref, class_id);
      }
      else
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         std::string key_str;
         key.write (key_str);
         mc_TRACE4 ("ARR ERS %1m16rw0X% %2% %3m16rw0X% %4m8rw0X%", ref, key_str.c_str (), pointed_ref, class_id);
#endif

         err = virtual_machine.erase_array (ref, key, pointed_ref, class_id);
      }
      
      if (is_auto_correct ())
      {
         if (err == Err_VM_CORRECT_REMOVE_OPCODE)
         {
            _corrected_flag = true;
            
            err = Err_OK;
         }
         else if (err == Err_VM_CORRECT_OLD_VAL)
         {
            assert (false);
         }
         else if (err == Err_OK)
         {
            _corrected_data_sptr->insert (
               direction_normal_flag
                  ? _corrected_data_sptr->end ()
                  : _corrected_data_sptr->begin (),
               &_data_ptr [opcode_pos], &_data_ptr [pos]
            );
         }
      }
      else
      {
         if (err != Err_OK)
         {
            mc_TRACE1 ("FAILED %1%", err);
            return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
         }
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : exec_opcode_collection
==============================================================================
*/

int   MachineCode::exec_opcode_collection (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   if (command == Command_MOVE)
   {
      // new_ref old_ref pointed_ref
      
      size_t opcode_pos = pos - 1;
      
      flip::Ref new_ref = 0;
      int err = read (new_ref, pos);   if (err != Err_OK) return err;
      
      flip::Ref old_ref = 0;
      err = read (old_ref, pos); if (err != Err_OK) return err;

      flip::Ref pointed_ref = 0;
      err = read (pointed_ref, pos);   if (err != Err_OK) return err;
      
      if (direction_normal_flag)
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         mc_TRACE3 ("COL MOV %1m16rw0X% %2m16rw0X% <- %3m16rw0X%", pointed_ref, new_ref, old_ref);
#endif
      
         err = virtual_machine.move_collection (new_ref, old_ref, pointed_ref);
      }
      else
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         mc_TRACE3 ("COL MOV %1m16rw0X% %2m16rw0X% <- %3m16rw0X%", pointed_ref, old_ref, new_ref);
#endif
      
         err = virtual_machine.move_collection (old_ref, new_ref, pointed_ref);
      }
      
      if (is_auto_correct ())
      {
         if (err == Err_VM_CORRECT_REMOVE_OPCODE)
         {
            _corrected_flag = true;
            
            err = Err_OK;
         }
         else if (err == Err_VM_CORRECT_OLD_VAL)
         {
            assert (false);
         }
         else if (err == Err_OK)
         {
            _corrected_data_sptr->insert (
               direction_normal_flag
                  ? _corrected_data_sptr->end ()
                  : _corrected_data_sptr->begin (),
               &_data_ptr [opcode_pos], &_data_ptr [pos]
            );
         }
      }
      else
      {
         if (err != Err_OK)
         {
            mc_TRACE1 ("FAILED %1%", err);
            return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
         }
      }
   }
   else
   {
      // ref pointed class_id
      
      size_t opcode_pos = pos - 1;
      
      bool insert_flag = (command == Command_INSERT) ^ !direction_normal_flag;
      
      flip::Ref ref = 0;
      int err = read (ref, pos); if (err != Err_OK) return err;
      
      flip::Ref pointed_ref = 0;
      err = read (pointed_ref, pos);   if (err != Err_OK) return err;
      
      archi::UInt32 class_id = 0;
      err = read (class_id, pos);   if (err != Err_OK) return err;
      
      if (insert_flag)
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         mc_TRACE3 ("COL INS %1m16rw0X% %2m16rw0X% %3m8rw0X%", ref, pointed_ref, class_id);
#endif

         err = virtual_machine.insert_collection (ref, pointed_ref, class_id);
      }
      else
      {
#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
         mc_TRACE3 ("COL ERS %1m16rw0X% %2m16rw0X% %3m8rw0X%", ref, pointed_ref, class_id);
#endif

         err = virtual_machine.erase_collection (ref, pointed_ref, class_id);
      }
      
      if (is_auto_correct ())
      {
         if (err == Err_VM_CORRECT_REMOVE_OPCODE)
         {
            _corrected_flag = true;
            
            err = Err_OK;
         }
         else if (err == Err_VM_CORRECT_OLD_VAL)
         {
            assert (false);
         }
         else if (err == Err_OK)
         {
            _corrected_data_sptr->insert (
               direction_normal_flag
                  ? _corrected_data_sptr->end ()
                  : _corrected_data_sptr->begin (),
               &_data_ptr [opcode_pos], &_data_ptr [pos]
            );
         }
      }
      else
      {
         if (err != Err_OK)
         {
            mc_TRACE1 ("FAILED %1%", err);
            return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
         }
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : exec_opcode_blob
==============================================================================
*/

int   MachineCode::exec_opcode_blob (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   if (command == Command_SET)
   {
      return exec_opcode_blob_set (virtual_machine, pos, direction_normal_flag);
   }
   else if (command == Command_SUBSET)
   {
      return exec_opcode_blob_subset (virtual_machine, pos, direction_normal_flag);
   }
   
   assert (false);
   return Err_VM_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : exec_opcode_blob_set
==============================================================================
*/

int   MachineCode::exec_opcode_blob_set (VirtualMachine & virtual_machine, size_t pos, bool direction_normal_flag)
{
   // ref new old
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::UInt32 size = 0;
   err = read (size, pos); if (err != Err_OK) return err;
   size_t data_pos = pos;
   pos += size;
   
   if (size > Limit_BLOB_SIZE)
   {
      // the tolerant case is one of the server case
      // that way a local transaction should harm only
      // the initiator of that transaction (therefore
      // irevelant for a malicous client)
      
      dump;
      assert (is_safe_tolerant ());
      
      if_exec (Err_VM_BLOB_SIZE_TOO_BIG, "MachineCode::exec_opcode_blob size > Limit_BLOB_SIZE");
      
      return Err_VM_BLOB_SIZE_TOO_BIG;
   }

   archi::UInt32 old_size = 0;
   err = read (old_size, pos);   if (err != Err_OK) return err;
   size_t old_data_pos = pos;
   pos += old_size;
   
   if (old_size > Limit_BLOB_SIZE)
   {
      // the tolerant case is one of the server case
      // that way a local transaction should harm only
      // the initiator of that transaction (therefore
      // irevelant for a malicous client)
      
      dump;
      assert (is_safe_tolerant ());

      if_exec (Err_VM_BLOB_SIZE_TOO_BIG, "MachineCode::exec_opcode_blob old_size > Limit_BLOB_SIZE");
      
      return Err_VM_BLOB_SIZE_TOO_BIG;
   }
   
   if (!direction_normal_flag)
   {
      std::swap (size, old_size);
      std::swap (data_pos, old_data_pos);
   }

#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
   std::string data_str;
   data_str.assign (&_data_ptr [data_pos], &_data_ptr [data_pos + size]);
   mc_TRACE3 ("BLO SET %1m16rw0X% %2m8rw0X% %3%", ref, size, data_str.c_str ());
#endif

   if (is_auto_correct ())
   {
      std::vector <archi::UByte> old_data;
      
      if (old_size > 0)
      {
         old_data.assign (
            &_data_ptr [old_data_pos],
            &_data_ptr [old_data_pos + old_size]
         );
      }
      
      err = virtual_machine.set_blob (
         ref,
         size, &_data_ptr [data_pos],
         old_data
      );
   
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         const archi::UByte * old_data_ptr = 0;
         size_t old_data_size = old_data.size ();
         
         if (old_data_size > 0)
         {
            old_data_ptr = &old_data [0];
         }
         
         const archi::UByte * data_ptr = 0;
         
         if (size > 0)
         {
            data_ptr = &_data_ptr [data_pos];
         }
         
         if (direction_normal_flag)
         {
            build_blob_set (
               *_corrected_data_sptr, ref,
               old_data_ptr, old_data_size,
               data_ptr, size
            );
         }
         else
         {
            Data data;
   
            build_blob_set (
               data, ref,
               data_ptr, size,
               old_data_ptr, old_data_size
            );
            
            _corrected_data_sptr->insert (
               _corrected_data_sptr->begin (), data.begin (), data.end ()
            );
         }
         
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }

      /*if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         size_t opcode_size = 1 + pos - start_pos;
         
         remove (start_pos - 1, opcode_size);
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         const archi::UByte * data_ptr = 0;
         
         if (old_data.size () > 0)
         {
            data_ptr = &old_data [0];
         }
         
         splice (data_ptr, old_size, old_data.size (), start_pos);
         
         err = Err_OK;
      }*/
   }
   else
   {
      err = virtual_machine.set_blob (
         ref,
         size, &_data_ptr [data_pos],
         old_size, &_data_ptr [old_data_pos]
      );
   
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_blob_subset
==============================================================================
*/

int   MachineCode::exec_opcode_blob_subset (VirtualMachine & virtual_machine, size_t pos, bool direction_normal_flag)
{
   // ref new old
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::UInt32 sub_pos = 0;
   err = read (sub_pos, pos); if (err != Err_OK) return err;

   archi::UInt32 size = 0;
   err = read (size, pos); if (err != Err_OK) return err;
   size_t data_pos = pos;
   pos += size;
   
   if (size > Limit_BLOB_SIZE)
   {
      // the tolerant case is one of the server case
      // that way a local transaction should harm only
      // the initiator of that transaction (therefore
      // irevelant for a malicous client)
      
      dump;
      assert (is_safe_tolerant ());
      
      if_exec (Err_VM_BLOB_SIZE_TOO_BIG, "MachineCode::exec_opcode_blob size > Limit_BLOB_SIZE");
      
      return Err_VM_BLOB_SIZE_TOO_BIG;
   }
   else if (size == 0)
   {
      assert (is_safe_tolerant ());
      
      if_exec (Err_VM_VOID_INSTRUCTION, "MachineCode::exec_opcode_blob size == 0");
      
      return Err_VM_VOID_INSTRUCTION;
   }

   size_t old_data_pos = pos;
   pos += size;
   
   if (!direction_normal_flag)
   {
      std::swap (data_pos, old_data_pos);
   }

#if defined (OHM_FLIP_MACHINE_CODE_DEBUG_FLAG)
   std::string data_str;
   data_str.assign (&_data_ptr [data_pos], &_data_ptr [data_pos + size]);
   mc_TRACE3 ("BLO SUBSET %1m16rw0X% %2m8rw0X% %3%", ref, size, data_str.c_str ());
#endif

   if (is_auto_correct ())
   {
      std::vector <archi::UByte> old_data;
      
      old_data.assign (
         &_data_ptr [old_data_pos],
         &_data_ptr [old_data_pos + size]
      );
      
      err = virtual_machine.subset_blob (
         ref,
         sub_pos, size,
         &_data_ptr [data_pos],
         old_data
      );
   
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         const archi::UByte * old_data_ptr = &old_data [0];
         const archi::UByte * data_ptr = &_data_ptr [data_pos];
         
         if (direction_normal_flag)
         {
            build_blob_subset (
               *_corrected_data_sptr, ref,
               sub_pos, size,
               old_data_ptr, data_ptr
            );
         }
         else
         {
            Data data;
   
            build_blob_subset (
               data, ref,
               sub_pos, size,
               data_ptr, old_data_ptr
            );
            
            _corrected_data_sptr->insert (
               _corrected_data_sptr->begin (), data.begin (), data.end ()
            );
         }
         
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }
   }
   else
   {
      err = virtual_machine.subset_blob (
         ref,
         sub_pos, size,
         &_data_ptr [data_pos], &_data_ptr [old_data_pos]
      );
   
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : exec_opcode_cue
==============================================================================
*/

int   MachineCode::exec_opcode_cue (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag)
{
   // ref data
   
   size_t opcode_pos = pos - 1;
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::UInt32 size = 0;
   err = read (size, pos); if (err != Err_OK) return err;
   size_t data_pos = pos;
   pos += size;
   
   if (size > Limit_BLOB_SIZE)
   {
      // the tolerant case is one of the server case
      // that way a local transaction should harm only
      // the initiator of that transaction (therefore
      // irevelant for a malicous client)
      
      dump;
      assert (is_safe_tolerant ());
      
      if_exec (Err_VM_BLOB_SIZE_TOO_BIG, "MachineCode::exec_opcode_cue size > Limit_BLOB_SIZE");
      
      return Err_VM_BLOB_SIZE_TOO_BIG;
   }

   err = virtual_machine.set_cue (
      ref,
      size, &_data_ptr [data_pos],
      direction_normal_flag
   );
   
   if (is_auto_correct ())
   {
      if (err == Err_VM_CORRECT_REMOVE_OPCODE)
      {
         _corrected_flag = true;
         
         err = Err_OK;
      }
      else if (err == Err_VM_CORRECT_OLD_VAL)
      {
         assert (false);
      }
      else if (err == Err_OK)
      {
         _corrected_data_sptr->insert (
            direction_normal_flag
               ? _corrected_data_sptr->end ()
               : _corrected_data_sptr->begin (),
            &_data_ptr [opcode_pos], &_data_ptr [pos]
         );
      }
   }
   else
   {
      if (err != Err_OK)
      {
         mc_TRACE1 ("FAILED %1%", err);
         return is_execution_tolerant () ? err : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : split_opcodes
==============================================================================
*/

int   MachineCode::split_opcodes ()
{
   size_t pos = 0;
   assert (_pos_arr.empty ());
   
   _pos_arr.push_back (0);
   
   for ( ; pos < _data_size ; /* nothing */ )
   {
      archi::UByte opcode = _data_ptr [pos];
      ++pos;
      
      int target = opcode & 0xF0;
      int command = opcode & 0x0F;
      
      int err = Err_OK;
      
      switch (target)
      {
      case Target_BOOL:
         err = split_opcodes_8 (command, pos);
         break;

      case Target_INT64:
      case Target_ENUM:
      case Target_FLOAT64:
      case Target_OBJECT_REF:
         err = split_opcodes_64 (command, pos);
         break;

      case Target_ARRAY:
         err = split_opcodes_array (command, pos);
         break;

      case Target_COLLECTION:
         err = split_opcodes_collection (command, pos);
         break;

      case Target_BLOB:
         if (command == Command_SET)
         {
            err = split_opcodes_blob_set (pos);
         }
         else if (command == Command_SUBSET)
         {
            err = split_opcodes_blob_subset (pos);
         }
         else
         {
            dump;
            assert (is_safe_tolerant ());
            return is_safe_tolerant () ? Err_VM_ILLEGAL_OPERAND_COMMAND : Err_VM_INTERNAL_SYSTEM_ERROR;
         }
         break;
         
      case Target_CUE:
         if (command == Command_SET)
         {
            err = split_opcodes_cue (pos);
         }
         else
         {
            dump;
            assert (is_safe_tolerant ());
            return is_safe_tolerant () ? Err_VM_ILLEGAL_OPERAND_COMMAND : Err_VM_INTERNAL_SYSTEM_ERROR;
         }
         break;
         
      default:
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_ILLEGAL_OPERAND_TARGET : Err_VM_INTERNAL_SYSTEM_ERROR;
         break;
      }
      
      if (err != Err_OK)
      {
         _pos_arr.clear ();
         
         dump;
         assert (is_safe_tolerant ());
         return err;
      }
      
      if (pos != _data_size)
      {
         _pos_arr.push_back (pos);
      }
   }
   
   if (pos != _data_size)
   {
      _pos_arr.clear ();
      
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : split_opcodes_8
==============================================================================
*/

int   MachineCode::split_opcodes_8 (int command, size_t & pos)
{
   switch (command)
   {
   case Command_SET:
      // ref new old
      pos += 8 + 1 + 1;
      if (pos > _data_size)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      break;

   default:
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_ILLEGAL_OPERAND_COMMAND : Err_VM_INTERNAL_SYSTEM_ERROR;
      break;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : split_opcodes_64
==============================================================================
*/

int   MachineCode::split_opcodes_64 (int command, size_t & pos)
{
   switch (command)
   {
   case Command_SET:
      // ref new old
      pos += 8 + 8 + 8;
      if (pos > _data_size)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      break;

   default:
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_ILLEGAL_OPERAND_COMMAND : Err_VM_INTERNAL_SYSTEM_ERROR;
      break;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : split_opcodes_array
==============================================================================
*/

int   MachineCode::split_opcodes_array (int command, size_t & pos)
{
   if ((command != Command_INSERT) && (command != Command_ERASE) && (command != Command_MOVE))
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_ILLEGAL_OPERAND_COMMAND : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   if (command == Command_MOVE)
   {
      // dst_ref dst_key (variable bits) src_ref src_key (variable bits)

      pos += 8;
      
      if (pos >= _data_size)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      
      archi::UInt32 nbr_bytes = 0;
      int err = read (nbr_bytes, pos); if (err != Err_OK) return err;
      
      if (nbr_bytes < 1)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_ARRAY_WRONG_KEY_SIZE : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      
      pos += nbr_bytes;
      
      pos += 8;
      
      if (pos >= _data_size)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      
      nbr_bytes = 0;
      err = read (nbr_bytes, pos);  if (err != Err_OK) return err;
      
      if (nbr_bytes < 1)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_ARRAY_WRONG_KEY_SIZE : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      
      pos += nbr_bytes;
      
      if (pos > _data_size)   // can reach last byte
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   else
   {
      // ref key (variable bits) pointed class_id
   
      pos += 8;
      
      if (pos >= _data_size)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
      
      archi::UInt32 nbr_bytes = 0;
      int err = read (nbr_bytes, pos); if (err != Err_OK) return err;
      
      if (nbr_bytes < 1)
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_ARRAY_WRONG_KEY_SIZE : Err_VM_INTERNAL_SYSTEM_ERROR;
      }

      // key pointed class_id
      pos += nbr_bytes + 8 + 4;
      
      if (pos > _data_size)   // can reach last byte
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : split_opcodes_collection
==============================================================================
*/

int   MachineCode::split_opcodes_collection (int command, size_t & pos)
{
   if ((command != Command_INSERT) && (command != Command_ERASE) && (command != Command_MOVE))
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_ILLEGAL_OPERAND_COMMAND : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   if (command == Command_MOVE)
   {
      // new_ref old_ref pointed_ref
      
      pos += 8 + 8 + 8;
      
      if (pos > _data_size)   // can reach last byte
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   else
   {
      // ref pointed class_id
   
      pos += 8 + 8 + 4;
      
      if (pos > _data_size)   // can reach last byte
      {
         dump;
         assert (is_safe_tolerant ());
         return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : split_opcodes_blob_set
==============================================================================
*/

int   MachineCode::split_opcodes_blob_set (size_t & pos)
{
   // ref new old
   
   pos += 8;
   
   if (pos >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   if (pos + 4 >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   archi::UInt32 size = 0;
   int err = read (size, pos);   if (err != Err_OK) return err;
   
   pos += size;
   
   if (pos >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   if (pos + 4 > _data_size)  // can reach last byte if size if 0
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   size = 0;
   err = read (size, pos); if (err != Err_OK) return err;
   
   pos += size;
   
   if (pos > _data_size)   // can reach last byte
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : split_opcodes_blob_subset
==============================================================================
*/

int   MachineCode::split_opcodes_blob_subset (size_t & pos)
{
   // ref pos size new old
   
   pos += 8;
   
   if (pos >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   pos += 4;
   
   if (pos >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   if (pos + 4 >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }

   archi::UInt32 size = 0;
   int err = read (size, pos);   if (err != Err_OK) return err;
   
   pos += size;
   
   if (pos >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   pos += size;
   
   if (pos > _data_size)   // can reach last byte
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : split_opcodes_cue
==============================================================================
*/

int   MachineCode::split_opcodes_cue (size_t & pos)
{
   // ref data
   
   pos += 8;
   
   if (pos >= _data_size)
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   if (pos + 4 > _data_size)  // can reach last byte if size if 0
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   archi::UInt32 size = 0;
   int err = read (size, pos);   if (err != Err_OK) return err;
   
   pos += size;
   
   if (pos > _data_size)   // can reach last byte
   {
      dump;
      assert (is_safe_tolerant ());
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode
==============================================================================
*/

int   MachineCode::inverse_opcode (Data & dst, size_t pos) const
{
   archi::UByte opcode = _data_ptr [pos];
   ++pos;
   
   int target = opcode & 0xF0;
   int command = opcode & 0x0F;
   
   int err = Err_OK;
   
   switch (target)
   {
   case Target_BOOL:
      err = inverse_opcode_bool (dst, command, pos);
      break;
      
   case Target_INT64:
      err = inverse_opcode_int64 (dst, command, pos);
      break;
      
   case Target_ENUM:
      err = inverse_opcode_enum (dst, command, pos);
      break;
      
   case Target_FLOAT64:
      err = inverse_opcode_float64 (dst, command, pos);
      break;
      
   case Target_OBJECT_REF:
      err = inverse_opcode_object_ref (dst, command, pos);
      break;

   case Target_ARRAY:
      err = inverse_opcode_array (dst, command, pos);
      break;

   case Target_COLLECTION:
      err = inverse_opcode_collection (dst, command, pos);
      break;

   case Target_BLOB:
      err = inverse_opcode_blob (dst, command, pos);
      break;
      
   default:
      // this error should have been detected at compile stage
      err = Err_VM_INTERNAL_SYSTEM_ERROR;
      assert (false);
      break;
   }
   
   return err;
}



/*
==============================================================================
Name : inverse_opcode_bool
==============================================================================
*/

int   MachineCode::inverse_opcode_bool (Data & dst, int command, size_t pos) const
{
   // ref new old
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::UByte val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::UByte old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   Data data;
   
   build_bool_set (data, ref, val != 0, old_val != 0);
   
   dst.insert (dst.begin (), data.begin (), data.end ());
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_int64
==============================================================================
*/

int   MachineCode::inverse_opcode_int64 (Data & dst, int command, size_t pos) const
{
   // ref new old
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::Int64 val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::Int64 old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   Data data;
   
   build_int64_set (data, ref, val, old_val);
   
   dst.insert (dst.begin (), data.begin (), data.end ());
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_enum
==============================================================================
*/

int   MachineCode::inverse_opcode_enum (Data & dst, int command, size_t pos) const
{
   // ref new old
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::Int64 val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::Int64 old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   Data data;
   
   build_enum_set (data, ref, val, old_val);
   
   dst.insert (dst.begin (), data.begin (), data.end ());
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_float64
==============================================================================
*/

int   MachineCode::inverse_opcode_float64 (Data & dst, int command, size_t pos) const
{
   // ref new old
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::Float64 val = 0;
   err = read (val, pos);  if (err != Err_OK) return err;

   archi::Float64 old_val = 0;
   err = read (old_val, pos); if (err != Err_OK) return err;
   
   Data data;
   
   build_float64_set (data, ref, val, old_val);
   
   dst.insert (dst.begin (), data.begin (), data.end ());
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_object_ref
==============================================================================
*/

int   MachineCode::inverse_opcode_object_ref (Data & dst, int command, size_t pos) const
{
   // ref new_pointed old_pointed
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;
   
   flip::Ref pointed_ref = 0;
   err = read (pointed_ref, pos);   if (err != Err_OK) return err;
   
   flip::Ref old_pointed_ref = 0;
   err = read (old_pointed_ref, pos);  if (err != Err_OK) return err;
   
   Data data;
   
   build_object_ref_set (data, ref, pointed_ref, old_pointed_ref);
   
   dst.insert (dst.begin (), data.begin (), data.end ());
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_array
==============================================================================
*/

int   MachineCode::inverse_opcode_array (Data & dst, int command, size_t pos) const
{
   const bool tolerant_flag = false;
   
   if (command == Command_MOVE)
   {
      // new_ref new_key old_ref old_key
      
      flip::Ref new_ref = 0;
      int err = read (new_ref, pos);   if (err != Err_OK) return err;
      
      archi::UInt32 new_key_bytes_size = 0;
      err = read (new_key_bytes_size, pos);  if (err != Err_OK) return err;

      BinaryKey new_key;
      err = new_key.read (&_data_ptr [pos], new_key_bytes_size, tolerant_flag);  if (err != Err_OK) return err;
      
      pos += new_key_bytes_size;
      
      flip::Ref old_ref = 0;
      err = read (old_ref, pos); if (err != Err_OK) return err;
      
      archi::UInt32 old_key_bytes_size = 0;
      err = read (old_key_bytes_size, pos);  if (err != Err_OK) return err;

      BinaryKey old_key;
      err = old_key.read (&_data_ptr [pos], old_key_bytes_size, tolerant_flag);  if (err != Err_OK) return err;
      
      Data data;
   
      build_array_move (data, new_ref, new_key, old_ref, old_key);
   
      dst.insert (dst.begin (), data.begin (), data.end ());
   }
   else
   {
      // ref key pointed class_id

      bool insert_flag = (command == Command_INSERT);

      flip::Ref ref = 0;
      int err = read (ref, pos); if (err != Err_OK) return err;

      archi::UInt32 key_bytes_size = 0;
      err = read (key_bytes_size, pos);   if (err != Err_OK) return err;

      BinaryKey key;
      err = key.read (&_data_ptr [pos], key_bytes_size, tolerant_flag); if (err) return err;

      pos += key_bytes_size;

      flip::Ref pointed_ref = 0;
      err = read (pointed_ref, pos);   if (err != Err_OK) return err;

      archi::UInt32 class_id = 0;
      err = read (class_id, pos);   if (err != Err_OK) return err;

      Data data;
   
      if (insert_flag)
      {
         build_array_erase (data, ref, key, pointed_ref, class_id);
      }
      else
      {
         build_array_insert (data, ref, key, pointed_ref, class_id);
      }
   
      dst.insert (dst.begin (), data.begin (), data.end ());
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_collection
==============================================================================
*/

int   MachineCode::inverse_opcode_collection (Data & dst, int command, size_t pos) const
{
   if (command == Command_MOVE)
   {
      // new_ref old_ref pointed_ref
      
      flip::Ref new_ref = 0;
      int err = read (new_ref, pos);   if (err != Err_OK) return err;
      
      flip::Ref old_ref = 0;
      err = read (old_ref, pos); if (err != Err_OK) return err;

      flip::Ref pointed_ref = 0;
      err = read (pointed_ref, pos);   if (err != Err_OK) return err;
      
      Data data;
   
      build_collection_move (data, new_ref, pointed_ref, old_ref);
   
      dst.insert (dst.begin (), data.begin (), data.end ());
   }
   else
   {
      // ref pointed class_id
      
      bool insert_flag = (command == Command_INSERT);
      
      flip::Ref ref = 0;
      int err = read (ref, pos); if (err != Err_OK) return err;
      
      flip::Ref pointed_ref = 0;
      err = read (pointed_ref, pos);   if (err != Err_OK) return err;
      
      archi::UInt32 class_id = 0;
      err = read (class_id, pos);   if (err != Err_OK) return err;
      
      Data data;
   
      if (insert_flag)
      {
         build_collection_erase (data, ref, pointed_ref, class_id);
      }
      else
      {
         build_collection_insert (data, ref, pointed_ref, class_id);
      }
   
      dst.insert (dst.begin (), data.begin (), data.end ());
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_blob
==============================================================================
*/

int   MachineCode::inverse_opcode_blob (Data & dst, int command, size_t pos) const
{
   const bool tolerant_flag = false;
   
   // ref new old
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::UInt32 size = 0;
   err = read (size, pos); if (err != Err_OK) return err;
   size_t data_pos = pos;
   pos += size;
   
   if (tolerant_flag && (size > Limit_BLOB_SIZE))
   {
      // the tolerant case is one of the server case
      // that way a local transaction should harm only
      // the initiator of that transaction (therefore
      // irevelant for a malicous client)
      
      return Err_VM_BLOB_SIZE_TOO_BIG;
   }

   archi::UInt32 old_size = 0;
   err = read (old_size, pos);   if (err != Err_OK) return err;
   size_t old_data_pos = pos;
   pos += old_size;
   
   if (tolerant_flag && (old_size > Limit_BLOB_SIZE))
   {
      // the tolerant case is one of the server case
      // that way a local transaction should harm only
      // the initiator of that transaction (therefore
      // irevelant for a malicous client)
      
      return Err_VM_BLOB_SIZE_TOO_BIG;
   }
   
   Data data;
   
   build_blob_set (
      data, ref,
      &_data_ptr [data_pos], size,
      &_data_ptr [old_data_pos], old_size
   );
   
   dst.insert (dst.begin (), data.begin (), data.end ());
   
   return Err_OK;
}



/*
==============================================================================
Name : inverse_opcode_cue
==============================================================================
*/

int   MachineCode::inverse_opcode_cue (Data & dst, int command, size_t pos) const
{
   const bool tolerant_flag = false;
   
   // ref data
   
   flip::Ref ref = 0;
   int err = read (ref, pos); if (err != Err_OK) return err;

   archi::UInt32 size = 0;
   err = read (size, pos); if (err != Err_OK) return err;
   size_t data_pos = pos;
   pos += size;
   
   if (tolerant_flag && (size > Limit_BLOB_SIZE))
   {
      // the tolerant case is one of the server case
      // that way a local transaction should harm only
      // the initiator of that transaction (therefore
      // irevelant for a malicous client)
      
      return Err_VM_BLOB_SIZE_TOO_BIG;
   }

   Data data;
   
   build_cue (
      data, ref,
      &_data_ptr [data_pos], size
   );
   
   dst.insert (dst.begin (), data.begin (), data.end ());
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   MachineCode::read (archi::UByte & val, size_t & pos) const
{
   if (pos + 1 > _data_size)
   {
      assert (is_safe_tolerant ());
      if_exec (Err_VM_BOOTLEG, "MachineCode::read");
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }

   val = _data_ptr [pos];
   ++pos;
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   MachineCode::read (archi::UInt32 & val, size_t & pos) const
{
   if (pos + 4 > _data_size)
   {
      assert (is_safe_tolerant ());
      if_exec (Err_VM_BOOTLEG, "MachineCode::read");
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }

   val = _data_ptr [pos] << 24;
   ++pos;
   val |= _data_ptr [pos] << 16;
   ++pos;
   val |= _data_ptr [pos] << 8;
   ++pos;
   val |= _data_ptr [pos];
   ++pos;
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   MachineCode::read (archi::Int64 & val, size_t & pos) const
{
   if (pos + 8 > _data_size)
   {
      assert (is_safe_tolerant ());
      if_exec (Err_VM_BOOTLEG, "MachineCode::read");
      return is_safe_tolerant () ? Err_VM_BOOTLEG : Err_VM_INTERNAL_SYSTEM_ERROR;
   }

   val = archi::Int64 (_data_ptr [pos]) << 56;
   ++pos;
   val |= archi::Int64 (_data_ptr [pos]) << 48;
   ++pos;
   val |= archi::Int64 (_data_ptr [pos]) << 40;
   ++pos;
   val |= archi::Int64 (_data_ptr [pos]) << 32;
   ++pos;
   val |= archi::Int64 (_data_ptr [pos]) << 24;
   ++pos;
   val |= archi::Int64 (_data_ptr [pos]) << 16;
   ++pos;
   val |= archi::Int64 (_data_ptr [pos]) << 8;
   ++pos;
   val |= archi::Int64 (_data_ptr [pos]);
   ++pos;
   
   return Err_OK;
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   MachineCode::read (archi::Float64 & val, size_t & pos) const
{
   archi::Int64 temp;
   
   int err = read (temp, pos);
   
   val = *reinterpret_cast <archi::Float64 *> (&temp);
   
   return err;
}



/*
==============================================================================
Name : is_execution_tolerant
==============================================================================
*/

bool  MachineCode::is_execution_tolerant () const
{
   assert (_mode != Mode_NONE);
   
   return _mode != Mode_NO_FAULT;
}



/*
==============================================================================
Name : is_safe_tolerant
==============================================================================
*/

bool  MachineCode::is_safe_tolerant () const
{
   assert (_mode != Mode_NONE);
   
   return _mode == Mode_SAFE;
}



/*
==============================================================================
Name : is_auto_correct
==============================================================================
*/

bool  MachineCode::is_auto_correct () const
{
   assert (_mode != Mode_NONE);
   
   return _mode == Mode_AUTO_CORRECT;
}



/*
==============================================================================
Name : dump_machine_code
==============================================================================
*/

void  MachineCode::dump_machine_code (const char * function_name_0, int line_nbr, size_t pos, bool tolerant_flag)
{
   if (!tolerant_flag)
   {
      // dump back trace
      
      //ohm_util_TRACE_BACKTRACE;

      // dump hex / ascii
      
      // ohm_util_TRACE_ERROR3 ("in %1% line %2% pos %3%", function_name_0, line_nbr, pos);

      // trace_hex_ascii ();
      
      // dump opcodes as long as we can
      
      // trace_opcodes ();
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#undef if_report
#undef if_comp
#undef if_exec
#undef dump



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
