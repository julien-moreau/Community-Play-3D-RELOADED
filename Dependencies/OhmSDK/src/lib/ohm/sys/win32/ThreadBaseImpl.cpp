/*****************************************************************************

        ThreadBaseImpl.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 68831

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
#include "ohm/sys/win32/ThreadBaseImpl.h"



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ThreadBaseImpl::ThreadBaseImpl ()
:  _handle (0)
,  _id (0)
,  _run_flag (false)
,  _term_flag (false)
{
}



   #if (ohm_sys_ThreadBase_CRT == ohm_sys_ThreadBase_CRT_USE)
unsigned int __stdcall  ThreadBaseImpl::dispatch_and_run (void *thread_ptr)
   #else    // ohm_sys_ThreadBase_CRT
::DWORD WINAPI ThreadBaseImpl::dispatch_and_run (::LPVOID thread_ptr)
   #endif   // ohm_sys_ThreadBase_CRT
{
   ThreadBase *   real_thread_ptr =
      reinterpret_cast <ThreadBase *> (thread_ptr);

   real_thread_ptr->_impl_ptr->_run_flag = true;
   real_thread_ptr->do_run ();
   real_thread_ptr->_impl_ptr->_run_flag = false;
   real_thread_ptr->_impl_ptr->_term_flag = true;

   return (0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
