/*****************************************************************************

        ValidationReportWriter.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64072

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

#include "ohm/flip/Constants.h"
#include "ohm/flip/Err.h"
#include "ohm/flip/ValidationReportWriter.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/trace.h"

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

ValidationReportWriter::ValidationReportWriter (std::vector <archi::UByte> & dst)
:  _dst (dst)
,  _suspicious_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ValidationReportWriter::~ValidationReportWriter ()
{
}



/*
==============================================================================
Name : get
==============================================================================
*/

const std::vector <archi::UByte> &  ValidationReportWriter::get () const
{
   return _dst;
}



/*
==============================================================================
Name : is_suspicious
==============================================================================
*/

bool  ValidationReportWriter::is_suspicious () const
{
   return _suspicious_flag;
}



/*
==============================================================================
Name : has_no_error
==============================================================================
*/

bool  ValidationReportWriter::has_no_error () const
{
   return _dst.empty ();
}



/*
==============================================================================
Name : flush
==============================================================================
*/

void  ValidationReportWriter::flush ()
{
   const std::string & str = _printer;
   
   if (!str.empty ())
   {
      _printer.cat ("\n");
      
      const std::string & text = _printer;
      
      _dst.insert (_dst.end (), text.begin (), text.end ());
   }
}



/*
==============================================================================
Name : print_compile_error
Description :
   Push a compilation error. Compilation error are always suspicious.
   The error number is a flip error.
==============================================================================
*/

void  ValidationReportWriter::print_compile_error (int err)
{
   flush ();
   
   _printer.print ("Compile Error %1%\n").arg (error_str (err));
   
   _suspicious_flag = true;
}



/*
==============================================================================
Name : print_execution_error
Description :
   Push an execution error, that is an error raised by a structural error
   while executing the flip program.
   Some errors might be suspicious or not, and are taken on an individual
   basis.
   The error number is a flip error.
==============================================================================
*/

ValidationReportWriter &   ValidationReportWriter::print_execution_error (int err, const char * format_0)
{
   flush ();
   
   _printer.print ("Execution Error %1%\n\t").arg (error_str (err));
   _printer.cat (format_0);
   
   switch (err)
   {
   case Err_VM_INTERNAL_SYSTEM_ERROR:
   case Err_VM_ILLEGAL_OPERAND_TARGET:
   case Err_VM_ILLEGAL_OPERAND_COMMAND:
   case Err_VM_UNAUTHORIZED_OPERAND_COMMAND:
   case Err_VM_BOOTLEG:
   case Err_VM_ILLEGAL_INSTRUCTION:
   case Err_VM_BAD_TYPE:
   case Err_VM_BAD_VALUE:
   case Err_VM_VOID_INSTRUCTION:
   case Err_VM_ARRAY_BAD_ELEMENT_TYPE:
   case Err_VM_ARRAY_WRONG_KEY_SIZE:
   case Err_VM_ARRAY_KEY_TOO_SMALL:
   case Err_VM_ARRAY_KEY_INVALID:
   case Err_VM_ARRAY_KEY_NOT_COMPACTED:
   case Err_VM_COLLECTION_BAD_ELEMENT_TYPE:
   case Err_VM_BLOB_SIZE_TOO_BIG:
      _suspicious_flag = true;
      break;
   }
   
   return *this;
}



/*
==============================================================================
Name : print_logical_error
Description :
   (Used by model validator)
   Push a logical error, that is an error raised by a logical error while
   validating the flip program.
   Logical errors are never suspicious.
   Severity is let to the appreciation of the model validator.
==============================================================================
*/

ValidationReportWriter &   ValidationReportWriter::print_logical_error (const char * format_0)
{
   flush ();
   
   _printer.print ("Logical Error \n\t");
   _printer.cat (format_0);
   
   return *this;
}



/*
==============================================================================
Name : print_severe_error
Description :
   (Used by model validator)
   Push a severe error, that is an error raised by a logical error while
   validating the flip program.
   Severe errors are always suspicious.
   Severity is let to the appreciation of the model validator.
==============================================================================
*/

ValidationReportWriter &   ValidationReportWriter::print_severe_error (const char * format_0)
{
   flush ();
   
   _printer.print ("Severe Error \n\t");
   _printer.cat (format_0);
   
   _suspicious_flag = true;
   
   return *this;
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  ValidationReportWriter::trace ()
{
   flush ();
   
   std::string str (_dst.begin (), _dst.end ());
   
   ohm_util_TRACE_INFO1 ("ValidationReportWriter::trace: %1%", str.c_str ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
