/*****************************************************************************

        Delegate.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 44887

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

#include "ohm/util/Delegate.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Delegate::Delegate ()
:  _receiver_ptr (0)
,  _param_type_info_ptr (0)
,  _stub (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Delegate::Delegate (const Delegate &other)
:  _receiver_ptr (other._receiver_ptr)
,  _param_type_info_ptr (other._param_type_info_ptr)
,  _stub (other._stub)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Delegate::~Delegate ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

Delegate &  Delegate::operator = (const Delegate &other)
{
   _receiver_ptr = other._receiver_ptr;
   _param_type_info_ptr = other._param_type_info_ptr;
   _stub = other._stub;
   
   return *this;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

bool  Delegate::operator == (const Delegate &other) const
{
   return (
      (_receiver_ptr == other._receiver_ptr)
      && (_stub == other._stub)
   );
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

bool  Delegate::operator != (const Delegate &other) const
{
   return (
      (_receiver_ptr != other._receiver_ptr)
      || (_stub != other._stub)
   );
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

bool  Delegate::operator < (const Delegate &other) const
{
   int c = lang::cmp (_receiver_ptr, other._receiver_ptr);
   if (c != 0) return c < 0;

   c = lang::cmp (_stub, other._stub);
   return c < 0;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

void  Delegate::reset ()
{
   _receiver_ptr = 0;
   _param_type_info_ptr = 0;
   _stub = 0;
}



/*
==============================================================================
Name : is_set
==============================================================================
*/

bool  Delegate::is_set () const
{
   return (_stub != 0);
}



/*
==============================================================================
Name : get_object_ptr
==============================================================================
*/

void *   Delegate::get_receiver_ptr () const
{
   return _receiver_ptr;
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  Delegate::signal () const
{
   if (_stub != 0)
   {
      assert (_param_type_info_ptr == 0);
      
      // _receiver_ptr might be null
      _stub (_receiver_ptr, 0);
   }
}



/*
==============================================================================
Name : cast_signal
==============================================================================
*/

void  Delegate::cast_signal (void * param_ptr) const
{
   assert (param_ptr != 0);
   
   if (_stub != 0)
   {
      assert (_param_type_info_ptr != 0);
      
      // _receiver_ptr might be null
      _stub (_receiver_ptr, param_ptr);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
