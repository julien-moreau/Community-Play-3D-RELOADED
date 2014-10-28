/*****************************************************************************

        Int64.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49394

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

#include "ohm/flip/Int64.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/flip/private/BasicTypesName.h"
#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/DocumentBase.h"

#include "ohm/util/trace.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

void  Int64::declare ()
{
   ClassDescription <Int64>::use ().set_name (BasicTypesName::_int64_0);
   
   ClassDescManager::declare (ClassDescription <Int64>::use ());
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Int64::Int64 (DocumentBase & document)
:  BasicType (document)
,  _val (0)
,  _old_val (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Int64::~Int64 ()
{
   // destruction (undefine) is handled in base class Object
   
   assert (_val == 0);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Int64::ctor (ohm::flip::Args & args)
{
   if (!args.empty ())
   {
      args.pop (*this);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Int64::ctor (archi::Int64 val)
{
   make_transaction_and_set (val);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Int64 &  Int64::operator = (archi::Int64 val)
{
   make_transaction_and_set (val);
   
   return *this;
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

Int64 &  Int64::operator += (archi::Int64 val)
{
   make_transaction_and_set (_val + val);
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

Int64 &  Int64::operator -= (archi::Int64 val)
{
   make_transaction_and_set (_val - val);
   
   return *this;
}



/*
==============================================================================
Name : operator *=
==============================================================================
*/

Int64 &  Int64::operator *= (archi::Int64 val)
{
   make_transaction_and_set (_val * val);
   
   return *this;
}



/*
==============================================================================
Name : operator /=
==============================================================================
*/

Int64 &  Int64::operator /= (archi::Int64 val)
{
   make_transaction_and_set (_val / val);
   
   return *this;
}



/*
==============================================================================
Name : operator archi::Int64
==============================================================================
*/

Int64::operator archi::Int64 () const
{
   return _val;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Int64::operator == (archi::Int64 val) const
{
   return (_val == val);
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Int64::operator != (archi::Int64 val) const
{
   return (_val != val);
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Int64::operator == (const Int64 &other) const
{
   return operator == (archi::Int64 (other));
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Int64::operator != (const Int64 &other) const
{
   return operator != (archi::Int64 (other));
}



/*
==============================================================================
Name : did_value_change
==============================================================================
*/

bool  Int64::did_value_change () const
{
   return _val != _old_val;
}



/*
==============================================================================
Name : get_old_value
==============================================================================
*/

archi::Int64   Int64::get_old_value () const
{
   return _old_val;
}



/*
==============================================================================
Name : backup
==============================================================================
*/

void  Int64::backup ()
{
   make_transaction_and_set (0);
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  Int64::synchronize ()
{
   _old_val = _val;
   
   Object::synchronize ();
}



/*
==============================================================================
Name : check_synchronized
==============================================================================
*/

void  Int64::check_synchronized () const
{
   assert (_old_val == _val);
   
   Object::check_synchronized ();
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

void  Int64::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (_old_val != _val)
   {
      ohm_util_TRACE_DEBUG ("  I64");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   Object::check_inner_invalidation_cnt (invalid_cnt);
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  Int64::trace ()
{
   Object::trace ();
   
   ohm_util_TRACE_INFO2 (
      "I64 %1% -> %2%",
      _old_val,
      _val
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
==============================================================================
*/

void  Int64::set (archi::Int64 val)
{
   assert (_val != val);
   
   if (_old_val == _val)
   {
      _val = val;
      
      invalidate ();
   }
   else if (_old_val == val)
   {
      _val = val;
      
      validate ();
   }
   else
   {
      _val = val;
   }
}



/*
==============================================================================
Name : make_transaction_and_set
==============================================================================
*/

void  Int64::make_transaction_and_set (archi::Int64 val)
{
   if (_val == val)
   {
      return;
   }
   
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);
   
   MachineCode::build_int64_set (transaction, this_ref, _val, val);
   
   set (val);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
