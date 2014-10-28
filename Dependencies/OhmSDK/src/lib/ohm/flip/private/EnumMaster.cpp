/*****************************************************************************

        EnumMaster.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/flip/private/EnumMaster.h"
#include "ohm/flip/private/BinaryKey.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*
==============================================================================
Note :
   Pure virtual classes are not supported in the flip C++ translation model.
   This is so because ClassDescription template will create the function
   'create' which inherently allocate an 'CollectionMaster' object.
   We cannot therefore detect pure virtual concepts at compilation time,
   but only at runtime.
==============================================================================
*/

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

EnumMaster::EnumMaster (DocumentBase & document)
:  BasicType (document)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

EnumMaster::~EnumMaster ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  EnumMaster::ctor (ohm::flip::Args & args)
{
   // see note
   assert (false);
}



/*
==============================================================================
Name : get
==============================================================================
*/

archi::Int64   EnumMaster::get () const
{
   // see note
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : get_min_val
==============================================================================
*/

archi::Int64   EnumMaster::get_min_val () const
{
   // see note
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : get_nbr_val
==============================================================================
*/

archi::Int64   EnumMaster::get_nbr_val () const
{
   // see note
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : get_name_for_val
==============================================================================
*/

int   EnumMaster::get_name_for_val (const char * & name_0, archi::Int64 val) const
{
   // see note
   assert (false);
   
   return Err_VM_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : get_val_for_name
==============================================================================
*/

int   EnumMaster::get_val_for_name (archi::Int64 & val, const std::string & name) const
{
   // see note
   assert (false);
   
   return Err_VM_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : get_enum_type_name
==============================================================================
*/

const char *   EnumMaster::get_enum_type_name () const
{
   // see note
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  EnumMaster::ctor (archi::Int64 val)
{
   // see note
   assert (false);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
==============================================================================
*/

void  EnumMaster::set (archi::Int64 val)
{
   // see note
   assert (false);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
