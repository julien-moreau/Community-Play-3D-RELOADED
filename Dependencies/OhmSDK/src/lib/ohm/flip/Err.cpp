/*****************************************************************************

        Err.cpp
        Copyright (c) 2011 Ohm Force

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

#include "ohm/flip/Err.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : error_str
==============================================================================
*/

const char *   error_str (int err)
{
   const char * ret_val = "flip::<Unknown Error>";
   
#define deferr(x) case x: ret_val = #x; break

   switch (err)
   {
   deferr (Err_EXCEPTION);

   deferr (Err_VM_INTERNAL_SYSTEM_ERROR);
   deferr (Err_VM_ILLEGAL_OPERAND_TARGET);
   deferr (Err_VM_ILLEGAL_OPERAND_COMMAND);
   deferr (Err_VM_UNAUTHORIZED_OPERAND_COMMAND);
   deferr (Err_VM_ILLEGAL_INSTRUCTION);
   deferr (Err_VM_BAD_ACCESS);
   deferr (Err_VM_BAD_TYPE);
   deferr (Err_VM_BAD_VALUE);
   deferr (Err_VM_VALUE_COLLISION);
   deferr (Err_VM_VOID_INSTRUCTION);
   deferr (Err_VM_ARRAY_BAD_ELEMENT_TYPE);
   deferr (Err_VM_ARRAY_WRONG_KEY_SIZE);
   deferr (Err_VM_ARRAY_KEY_EXISTS);
   deferr (Err_VM_ARRAY_KEY_DOES_NOT_EXIST);
   deferr (Err_VM_ARRAY_KEY_TOO_SMALL);
   deferr (Err_VM_ARRAY_KEY_INVALID);
   deferr (Err_VM_ARRAY_KEY_NOT_COMPACTED);
   deferr (Err_VM_ARRAY_OBJECT_CANNOT_BE_RELEASED);
   deferr (Err_VM_COLLECTION_BAD_ELEMENT_TYPE);
   deferr (Err_VM_COLLECTION_OBJECT_EXISTS);
   deferr (Err_VM_COLLECTION_OBJECT_DOES_NOT_EXIST);
   deferr (Err_VM_COLLECTION_OBJECT_CANNOT_BE_RELEASED);
   deferr (Err_VM_BLOB_SIZE_TOO_BIG);
   deferr (Err_VM_REF_EXISTS);
   deferr (Err_VM_VALIDATION_FAILED);
   deferr (Err_VM_TX_SESSION_COLLISION);
   deferr (Err_VM_CORRECT_REMOVE_OPCODE);
   deferr (Err_VM_CORRECT_OLD_VAL);

   deferr (Err_UNDO_TX_SESSION_COLLISION);
   deferr (Err_UNDO_TX_EMPTY);
   deferr (Err_UNDO_EMPTY_STACK);

   deferr (Err_DOC_INTERNAL_SYSTEM_ERROR);
   deferr (Err_DOC_BAD_HEADER);
   deferr (Err_DOC_BAD_EOF);
   deferr (Err_DOC_XREF_NOT_FOUND);
   deferr (Err_DOC_BAD_USER_ID);
   deferr (Err_DOC_BAD_REFERENCE);
   deferr (Err_DOC_OBJECT_MISSING);
   deferr (Err_DOC_UNEXPECTED_KEYWORD);
   deferr (Err_DOC_CLASS_NOT_DECLARED);
   deferr (Err_DOC_ATTRIBUTE_NOT_DECLARED);
   deferr (Err_DOC_NO_MORE_ATTRIBUTES);
   deferr (Err_DOC_TYPE_MISMATCH);
   deferr (Err_DOC_UNEXPECTED_TYPE);
   deferr (Err_DOC_ITERATION_NO_MORE_ITEMS);
   deferr (Err_DOC_ARRAY_WRONG_KEY);
   deferr (Err_DOC_BLOB_EMPTY);

   deferr (Err_DIF_NOT_COALESCABLE);

   deferr (Err_DOC_BOOTLEG);
   deferr (Err_DOC_MISSING_STARTXREF);
   deferr (Err_DOC_MISSING_TRAILER);
   deferr (Err_DOC_BAD_REVISION);
   deferr (Err_DOC_BAD_VERSION);

   deferr (Err_DS_STREAM_OPEN_ERROR);
   deferr (Err_DS_STREAM_SEEK_ERROR);
   deferr (Err_DS_STREAM_READ_ERROR);
   deferr (Err_DS_EOF);
   deferr (Err_DS_UNEXPECTED_TOKEN);
   deferr (Err_DS_UNEXPECTED_KEYWORD);
   deferr (Err_DS_INTEGER_OVERFLOW);
   deferr (Err_DS_INTEGER_UNDERFLOW);
   deferr (Err_DS_FLOAT_HEX_WRONG_SIZE);
   deferr (Err_DS_FLOAT_MISSING_DOT);

   deferr (Err_COM_DISCONNECTED);
   deferr (Err_COM_BAD_TYPE);

   deferr (Err_CODEC_INTERNAL_SYSTEM_ERROR);
   deferr (Err_CODEC_UNKNOWN);
   deferr (Err_CODEC_DEFLATE);
   deferr (Err_CODEC_BOOTLEG);
   deferr (Err_CODEC_BAD_DATA);

   deferr (Err_FILE_TOO_LONG);
   }

#undef deferr

   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
