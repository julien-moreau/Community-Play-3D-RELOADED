/*****************************************************************************

        ThreadBase.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 55115

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

#include "ohm/sys/ThreadBase.h"
#include "ohm/sys/macho/ThreadBaseImpl.h"

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ThreadBase::ThreadBase ()
:  _impl_ptr (0)
{
   _impl_ptr = new ThreadBaseImpl (this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ThreadBase::~ThreadBase ()
{
   // Here the task has been run until it returns
   // approximatively, that is we are at least at
   // the point the _wait_mutex has been unlocked
   
   kill ();
   
   delete _impl_ptr;
   _impl_ptr = 0;
}



/*
==============================================================================
Name : run
==============================================================================
*/

int   ThreadBase::run ()
{
   assert (_impl_ptr != 0);
   
   return _impl_ptr->run ();
}



/*
==============================================================================
Name : wait_for_death
==============================================================================
*/

bool  ThreadBase::wait_for_death (double timeout)
{
   assert (_impl_ptr != 0);
   
   bool ret_val = _impl_ptr->wait_for_death (timeout);
   
   return ret_val;
}



/*
==============================================================================
Name : kill
==============================================================================
*/

void  ThreadBase::kill ()
{
   assert (_impl_ptr != 0);
   
   _impl_ptr->kill ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
