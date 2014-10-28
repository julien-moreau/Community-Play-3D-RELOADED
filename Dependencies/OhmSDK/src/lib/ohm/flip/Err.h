/*****************************************************************************

        Err.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
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



#if ! defined (ohm_flip_Err_HEADER_INCLUDED)
#define  ohm_flip_Err_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace flip
{



enum /* Err */
{
   Err_OK = 0,

   Err_ZONE = -12500,

   Err_EXCEPTION = Err_ZONE,
   
   Err_VM_INTERNAL_SYSTEM_ERROR,
   Err_VM_ILLEGAL_OPERAND_TARGET,
   Err_VM_ILLEGAL_OPERAND_COMMAND,
   Err_VM_UNAUTHORIZED_OPERAND_COMMAND,
   Err_VM_BOOTLEG,                                    // -12495
   Err_VM_ILLEGAL_INSTRUCTION,
   Err_VM_BAD_ACCESS,
   Err_VM_BAD_TYPE,
   Err_VM_BAD_VALUE,
   Err_VM_VALUE_COLLISION,                            // -12490
   Err_VM_VOID_INSTRUCTION,
   Err_VM_ARRAY_BAD_ELEMENT_TYPE,
   Err_VM_ARRAY_WRONG_KEY_SIZE,
   Err_VM_ARRAY_KEY_EXISTS,
   Err_VM_ARRAY_KEY_DOES_NOT_EXIST,                   // -12485
   Err_VM_ARRAY_KEY_TOO_SMALL,
   Err_VM_ARRAY_KEY_INVALID,
   Err_VM_ARRAY_KEY_NOT_COMPACTED,
   Err_VM_ARRAY_OBJECT_CANNOT_BE_RELEASED,
   Err_VM_COLLECTION_BAD_ELEMENT_TYPE,                // -12480
   Err_VM_COLLECTION_OBJECT_EXISTS,
   Err_VM_COLLECTION_OBJECT_DOES_NOT_EXIST,
   Err_VM_COLLECTION_OBJECT_CANNOT_BE_RELEASED,
   Err_VM_BLOB_SIZE_TOO_BIG,
   Err_VM_REF_EXISTS,                                 // -12475
   Err_VM_VALIDATION_FAILED,
   Err_VM_TX_SESSION_COLLISION,
   Err_VM_CORRECT_REMOVE_OPCODE,
   Err_VM_CORRECT_OLD_VAL,

   Err_UNDO_TX_SESSION_COLLISION,                     // -12470
   Err_UNDO_TX_EMPTY,
   Err_UNDO_EMPTY_STACK,
   
   Err_DOC_INTERNAL_SYSTEM_ERROR,
   Err_DOC_BAD_HEADER,
   Err_DOC_BAD_EOF,                                   // -12465
   Err_DOC_XREF_NOT_FOUND,
   Err_DOC_BAD_USER_ID,
   Err_DOC_BAD_REFERENCE,
   Err_DOC_OBJECT_MISSING,
   Err_DOC_UNEXPECTED_KEYWORD,                        // -12460
   Err_DOC_CLASS_NOT_DECLARED,
   Err_DOC_ATTRIBUTE_NOT_DECLARED,
   Err_DOC_NO_MORE_ATTRIBUTES,
   Err_DOC_TYPE_MISMATCH,
   Err_DOC_UNEXPECTED_TYPE,                           // -12455
   Err_DOC_ITERATION_NO_MORE_ITEMS,
   Err_DOC_ARRAY_WRONG_KEY,
   Err_DOC_BLOB_EMPTY,
   
   Err_DIF_NOT_COALESCABLE,

   Err_DOC_BOOTLEG,                                   // -12450
   Err_DOC_MISSING_STARTXREF,
   Err_DOC_MISSING_TRAILER,
   Err_DOC_BAD_REVISION,
   Err_DOC_BAD_VERSION,
   
   Err_DS_STREAM_OPEN_ERROR,                          // -12445
   Err_DS_STREAM_SEEK_ERROR,
   Err_DS_STREAM_READ_ERROR,
   Err_DS_EOF,
   Err_DS_UNEXPECTED_TOKEN,
   Err_DS_UNEXPECTED_KEYWORD,                         // -12440
   Err_DS_INTEGER_OVERFLOW,
   Err_DS_INTEGER_UNDERFLOW,
   Err_DS_FLOAT_HEX_WRONG_SIZE,
   Err_DS_FLOAT_MISSING_DOT,
   
   Err_COM_DISCONNECTED,                              // -12435
   Err_COM_BAD_TYPE,
   
   Err_CODEC_INTERNAL_SYSTEM_ERROR,
   Err_CODEC_UNKNOWN,
   Err_CODEC_DEFLATE,
   Err_CODEC_BOOTLEG,                                 // -12430
   Err_CODEC_BAD_DATA,

   Err_FILE_TOO_LONG

}; // enum Err

const char *   error_str (int err);



}  // namespace flip
}  // namespace ohm



//#include "ohm/flip/Err.hpp"



#endif   // ohm_flip_Err_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
