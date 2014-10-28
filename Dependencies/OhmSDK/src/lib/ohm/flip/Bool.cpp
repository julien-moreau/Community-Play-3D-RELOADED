/*****************************************************************************

        Bool.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/flip/Bool.h"
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

void  Bool::declare ()
{
   ClassDescription <Bool>::use ().set_name (BasicTypesName::_bool_0);
   
   ClassDescManager::declare (ClassDescription <Bool>::use ());
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Bool::Bool (DocumentBase & document)
:  BasicType (document)
,  _val (false)
,  _old_val (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Bool::~Bool ()
{
   // destruction (undefine) is handled in base class Object
   
   assert (_val == false);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Bool::ctor (ohm::flip::Args & args)
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

void  Bool::ctor (bool val)
{
   make_transaction_and_set (val);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Bool &   Bool::operator = (bool val)
{
   make_transaction_and_set (val);
   
   return *this;
}



/*
==============================================================================
Name : operator bool
==============================================================================
*/

Bool::operator bool () const
{
   return _val;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Bool::operator == (bool val) const
{
   return (_val == val);
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Bool::operator != (bool val) const
{
   return (_val != val);
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Bool::operator == (const Bool &other) const
{
   return operator == (bool (other));
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Bool::operator != (const Bool &other) const
{
   return operator != (bool (other));
}



/*
==============================================================================
Name : did_value_change
==============================================================================
*/

bool  Bool::did_value_change () const
{
   return _val != _old_val;
}



/*
==============================================================================
Name : get_old_value
==============================================================================
*/

bool  Bool::get_old_value () const
{
   return _old_val;
}



/*
==============================================================================
Name : backup
==============================================================================
*/

void  Bool::backup ()
{
   make_transaction_and_set (false);
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  Bool::synchronize ()
{
   _old_val = _val;
   
   Object::synchronize ();
}



/*
==============================================================================
Name : check_synchronized
==============================================================================
*/

void  Bool::check_synchronized () const
{
   assert (_old_val == _val);
   
   Object::check_synchronized ();
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

void  Bool::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (_old_val != _val)
   {
      ohm_util_TRACE_DEBUG ("  BOL");
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

void  Bool::trace ()
{
   Object::trace ();
   
   ohm_util_TRACE_INFO2 (
      "BOL %1% -> %2%",
      _old_val ? "1" : "0",
      _val ? "1" : "0"
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
==============================================================================
*/

void  Bool::set (bool val)
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

void  Bool::make_transaction_and_set (bool val)
{
   if (_val == val)
   {
      return;
   }
   
   Data & transaction = use_document ().use_transaction ();

   Ref this_ref = get_ref ();
   assert (this_ref != 0);
   
   MachineCode::build_bool_set (transaction, this_ref, _val, val);
   
   set (val);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
