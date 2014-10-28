/*****************************************************************************

        MachineCode.h
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



#if ! defined (ohm_flip_MachineCode_HEADER_INCLUDED)
#define  ohm_flip_MachineCode_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/mem/SharedPtr.h"
#include "ohm/flip/Data.h"
#include "ohm/flip/Err.h"
#include "ohm/flip/Ref.h"
#include "ohm/flip/private/BinaryKey.h"

#include <vector>
#include <set>
#include <map>



namespace ohm
{
namespace flip
{



class TransactionBase;
class VirtualMachine;
class ValidationReportWriter;

class MachineCode
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   class ContainerElem
   {
   public:
      flip::Ref   _parent_ref;
      BinaryKey   _key;
      union {
         archi::Int64
                  _val_i64;
         archi::Float64
                  _val_f64;
      };
   };
   
   typedef std::map <flip::Ref, ContainerElem> RefContainerElemMap;
   
   static void    build_bool_set (Data & tx_data, flip::Ref this_ref, bool old_val, bool new_val);
   static void    build_int64_set (Data & tx_data, flip::Ref this_ref, archi::Int64 old_val, archi::Int64 new_val);
   static void    build_enum_set (Data & tx_data, flip::Ref this_ref, archi::Int64 old_val, archi::Int64 new_val);
   static void    build_float64_set (Data & tx_data, flip::Ref this_ref, archi::Float64 old_val, archi::Float64 new_val);
   static void    build_blob_set (Data & tx_data, flip::Ref this_ref, const archi::UByte * old_data_ptr, archi::UInt32 old_data_size, const archi::UByte * new_data_ptr, archi::UInt32 new_data_size);
   static void    build_blob_subset (Data & tx_data, flip::Ref this_ref, archi::UInt32 data_pos, archi::UInt32 data_size, const archi::UByte * old_data_ptr, const archi::UByte * new_data_ptr);
   static void    build_object_ref_set (Data & tx_data, flip::Ref this_ref, flip::Ref old_ref, flip::Ref new_ref);
   static void    build_cue (Data & tx_data, flip::Ref this_ref, const archi::UByte * data_ptr, archi::UInt32 data_size);
   static void    build_array_insert (Data & tx_data, flip::Ref this_ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id);
   static void    build_array_erase (Data & tx_data, flip::Ref this_ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id);
   static void    build_array_move (Data & tx_data, flip::Ref old_ref, const BinaryKey & old_key, flip::Ref new_ref, const BinaryKey & new_key);
   static void    build_collection_insert (Data & tx_data, flip::Ref this_ref, flip::Ref pointed_ref, archi::UInt32 class_id);
   static void    build_collection_erase (Data & tx_data, flip::Ref this_ref, flip::Ref pointed_ref, archi::UInt32 class_id);
   static void    build_collection_move (Data & tx_data, flip::Ref old_ref, flip::Ref pointed_ref, flip::Ref new_ref);
   
                  MachineCode (const archi::UByte * data_ptr, size_t data_size);
   virtual        ~MachineCode ();

   void           set_mode_safe ();
   void           set_mode_no_fault ();
   void           set_mode_auto_correct ();
      
   void           bind (ValidationReportWriter & report);
   
   int            compile ();

   int            get_nbr_opcodes () const;
   int            exec (VirtualMachine & virtual_machine, int opcode_nbr, bool direction_normal_flag);
   
   bool           has_corrected_tx () const;
   DataSPtr       get_corrected_tx_data () const;
   
   int            get_container_elem (RefContainerElemMap & ref_map);
   int            get_value_elem (RefContainerElemMap & ref_map);
   
   void           inverse (Data & dst);

   void           trace_hex_ascii () const;
   void           trace_opcodes () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum Target
   {
      Target_BOOL          = 0x00,
      Target_INT64         = 0x10,
      Target_ENUM          = 0x20,
      Target_FLOAT64       = 0x30,
      Target_BLOB          = 0x40,
      Target_ARRAY         = 0x50,
      Target_COLLECTION    = 0x60,
      Target_OBJECT_REF    = 0x70,
      Target_CUE           = 0x80,
   };
   
   enum Command
   {
      Command_SET          = 0x01,
      Command_INSERT       = 0x02,
      Command_ERASE        = 0x03,
      Command_MOVE         = 0x04,
      Command_SUBSET       = 0x05,
   };
   
   enum Limit
   {
      Limit_BLOB_SIZE      = 10L * 1024L * 1024L,  // 10MB
   };
   
   enum Mode
   {
                  Mode_NONE = 0,
                  Mode_SAFE,
                  Mode_NO_FAULT,
                  Mode_AUTO_CORRECT,
   };
   
   int            exec_opcode (VirtualMachine & virtual_machine, size_t pos, bool direction_normal_flag);
   int            exec_opcode_bool (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_int64 (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_enum (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_float64 (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_object_ref (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_array (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_collection (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_blob (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   int            exec_opcode_blob_set (VirtualMachine & virtual_machine, size_t pos, bool direction_normal_flag);
   int            exec_opcode_blob_subset (VirtualMachine & virtual_machine, size_t pos, bool direction_normal_flag);
   int            exec_opcode_cue (VirtualMachine & virtual_machine, int command, size_t pos, bool direction_normal_flag);
   
   int            split_opcodes ();
   int            split_opcodes_8 (int command, size_t & pos);
   int            split_opcodes_64 (int command, size_t & pos);
   int            split_opcodes_array (int command, size_t & pos);
   int            split_opcodes_collection (int command, size_t & pos);
   int            split_opcodes_blob_set (size_t & pos);
   int            split_opcodes_blob_subset (size_t & pos);
   int            split_opcodes_cue (size_t & pos);
   
   int            inverse_opcode (Data & dst, size_t pos) const;
   int            inverse_opcode_bool (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_int64 (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_enum (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_float64 (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_object_ref (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_array (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_collection (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_blob (Data & dst, int command, size_t pos) const;
   int            inverse_opcode_cue (Data & dst, int command, size_t pos) const;
   
   int            read (archi::UByte & val, size_t & pos) const;
   int            read (archi::UInt32 & val, size_t & pos) const;
   int            read (archi::Int64 & val, size_t & pos) const;
   int            read (archi::Float64 & val, size_t & pos) const;
   
   bool           is_execution_tolerant () const;
   bool           is_safe_tolerant () const;
   bool           is_auto_correct () const;
   
   void           dump_machine_code (const char * function_name_0, int line_nbr, size_t pos, bool tolerant_flag);
   
   const archi::UByte * const
                  _data_ptr;
   const size_t   _data_size;
   Mode           _mode;
   
   ValidationReportWriter *
                  _report_ptr;
   
   std::vector <size_t>
                  _pos_arr;
   
   bool           _corrected_flag;
   DataSPtr       _corrected_data_sptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MachineCode ();
                  MachineCode (const MachineCode &other);
   MachineCode &  operator = (const MachineCode &other);
   bool           operator == (const MachineCode &other);
   bool           operator != (const MachineCode &other);

}; // class MachineCode



typedef  mem::SharedPtr <MachineCode>  MachineCodeSPtr;



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/MachineCode.hpp"



#endif   // ohm_flip_MachineCode_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
