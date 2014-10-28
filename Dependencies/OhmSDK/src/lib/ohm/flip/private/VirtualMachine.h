/*****************************************************************************

        VirtualMachine.h
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



#if ! defined (ohm_flip_VirtualMachine_HEADER_INCLUDED)
#define  ohm_flip_VirtualMachine_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/flip/Err.h"
#include "ohm/flip/Object.h"
#include "ohm/flip/private/BinaryKey.h"



#include <list>

namespace ohm
{
namespace flip
{



class MachineCode;
class DocumentBase;
class ObjectManager;
class ValidationReportWriter;

class VirtualMachine
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  VirtualMachine (DocumentBase & document, ObjectManager & object_manager, bool master_flag);
   virtual        ~VirtualMachine ();
   
   void           stop ();
   void           run ();
   
   void           set_mode_safe ();
   void           set_mode_no_fault ();
   void           set_mode_auto_correct ();
   void           set_mode_conflict_marking ();
   
   void           set_mode_direction (archi::Int64 direction);
   
   void           bind (ValidationReportWriter & report);
   void           unbind (ValidationReportWriter & report);
   
   int            compile (MachineCode & code);
   int            execute (MachineCode & code);
   
   // MachineCode access
   int            set_bool (flip::Ref ref, archi::UByte val, archi::UByte & old_val);
   int            set_int64 (flip::Ref ref, archi::Int64 val, archi::Int64 & old_val);
   int            set_enum (flip::Ref ref, archi::Int64 val, archi::Int64 & old_val);
   int            set_float64 (flip::Ref ref, archi::Float64 val, archi::Float64 & old_val);
   int            set_object_ref (flip::Ref ref, flip::Ref pointed_ref, flip::Ref & old_pointed_ref);
   int            insert_array (flip::Ref ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id);
   int            erase_array (flip::Ref ref, const BinaryKey & key, flip::Ref pointed_ref, archi::UInt32 class_id);
   int            move_array (flip::Ref dst_ref, const BinaryKey & dst_key, flip::Ref src_ref, const BinaryKey & src_key);
   int            insert_collection (flip::Ref ref, flip::Ref pointed_ref, archi::UInt32 class_id);
   int            erase_collection (flip::Ref ref, flip::Ref pointed_ref, archi::UInt32 class_id);
   int            move_collection (flip::Ref new_ref, flip::Ref old_ref, flip::Ref pointed_ref);
   int            set_blob (flip::Ref ref, archi::UInt32 data_size, const archi::UByte * data_ptr, archi::UInt32 old_data_size, const archi::UByte * old_data_ptr);
   int            set_blob (flip::Ref ref, archi::UInt32 data_size, const archi::UByte * data_ptr, std::vector <archi::UByte> & old_data);
   int            subset_blob (flip::Ref ref, archi::UInt32 data_pos, archi::UInt32 data_size, const archi::UByte * data_ptr, const archi::UByte * old_data_ptr);
   int            subset_blob (flip::Ref ref, archi::UInt32 data_pos, archi::UInt32 data_size, const archi::UByte * data_ptr, std::vector <archi::UByte> & old_data);
   int            set_cue (flip::Ref ref, archi::UInt32 data_size, const archi::UByte * data_ptr, bool direction_normal_flag);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum Mode
   {
                  Mode_NONE = 0,
                  Mode_SAFE,
                  Mode_NO_FAULT,
                  Mode_AUTO_CORRECT,
                  Mode_CONFLICT_MARKING,
   };

   bool           is_tolerant () const;
   bool           is_auto_correct () const;
   bool           is_conflict_marking () const;
   
   int            execute_normal (MachineCode & code);
   int            execute_reverse (MachineCode & code);
   
   DocumentBase & _document;
   ObjectManager &
                  _object_manager;
   const bool     _master_flag;
   
   bool           _run_flag;
   Mode           _mode;
   bool           _direction_normal_flag;
   
   ValidationReportWriter *
                  _report_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  VirtualMachine ();
                  VirtualMachine (const VirtualMachine &other);
   VirtualMachine &
                  operator = (const VirtualMachine &other);
   bool           operator == (const VirtualMachine &other);
   bool           operator != (const VirtualMachine &other);

}; // class VirtualMachine



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/VirtualMachine.hpp"



#endif   // ohm_flip_VirtualMachine_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
