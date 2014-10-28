/*****************************************************************************

        ArrayMaster.cpp
        Copyright (c) 2008 Ohm Force

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

#include "ohm/flip/private/ArrayMaster.h"
#include "ohm/flip/private/BinaryKey.h"
#include "ohm/flip/private/DocumentBase.h"

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
   'create' which inherently allocate an 'ArrayMaster' object.
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

ArrayMaster::ArrayMaster (DocumentBase & document)
:  Object (document)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ArrayMaster::~ArrayMaster ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  ArrayMaster::ctor (ohm::flip::Args & args)
{
   // see note
   assert (false);
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  ArrayMaster::init ()
{
   // this is needed because the init call chain must not
   // fallback on Object::init () which was called just before
   // this call
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  ArrayMaster::restore ()
{
   // this is needed because the init call chain must not
   // fallback on Object::init () which was called just before
   // this call
}



/*
==============================================================================
Name : size
==============================================================================
*/

size_t   ArrayMaster::size (FindMask find_mask) const
{
   // see note
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  ArrayMaster::clear ()
{
   // see note
   assert (false);
}



/*
==============================================================================
Name : erase
==============================================================================
*/

void  ArrayMaster::erase (Ref ref)
{
   // see note
   assert (false);
}



/*
==============================================================================
Name : push_back
==============================================================================
*/

Ref   ArrayMaster::push_back (ClassDescriptionMaster & cdm)
{
   // see note
   assert (false);
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : insert_no_notification
==============================================================================
*/

int   ArrayMaster::insert_no_notification (const BinaryKey & key, Ref ref, int class_id, bool tolerant_flag)
{
   // see note
   assert (false);

   return Err_VM_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : erase_no_notification
==============================================================================
*/

int   ArrayMaster::erase_no_notification (const BinaryKey & key, Ref ref, bool tolerant_flag)
{
   // see note
   assert (false);
   
   return Err_VM_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : move_no_notification
==============================================================================
*/

int   ArrayMaster::move_no_notification (ArrayMaster * dst_ptr, const BinaryKey & dst_key, const BinaryKey & src_key, bool tolerant_flag)
{
   // see note
   assert (false);
   
   return Err_VM_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : broadcast_order_change
==============================================================================
*/

int   ArrayMaster::broadcast_order_change ()
{
   // see note
   assert (false);
   
   return Err_VM_INTERNAL_SYSTEM_ERROR;
}



/*
==============================================================================
Name : get_element_ref
==============================================================================
*/

Ref   ArrayMaster::get_element_ref (size_t i) const
{
   // see note
   assert (false);
   
   return 0;
}



/*
==============================================================================
Name : get_element_key
==============================================================================
*/

const BinaryKey & ArrayMaster::get_element_key (size_t i) const
{
   // see note
   assert (false);
   
   static BinaryKey zero;
   
   return zero;
}



/*
==============================================================================
Name : get_element_class_id
==============================================================================
*/

int   ArrayMaster::get_element_class_id () const
{
   // see note
   assert (false);
   
   return 0;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
