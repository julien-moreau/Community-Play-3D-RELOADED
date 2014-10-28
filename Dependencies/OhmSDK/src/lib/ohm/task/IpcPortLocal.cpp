/*****************************************************************************

        IpcPortLocal.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66420

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

#include "ohm/conc/AioAdd.h"
#include "ohm/conc/AtomicIntOp.h"
#include "ohm/task/IpcPortLocal.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/task/win32/IpcPortLocalImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/task/mach/IpcPortLocalImpl.h"

#else
   #error Operation System not supported

#endif

#include <cassert>



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   if 'exclusive_flag' is true, then 'recv' call is exclusive, that is no
   concurrent recv are allowed.
==============================================================================
*/

IpcPortLocal::IpcPortLocal (const char * name_0, bool exclusive_flag)
:  _impl_aptr ()
,  _exclusive_flag (exclusive_flag)
,  _recv_cnt (0)
{
   _impl_aptr = std::auto_ptr <IpcPortLocalImpl> (
      new IpcPortLocalImpl (name_0)
   );
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

IpcPortLocal::~IpcPortLocal ()
{
}



/*
==============================================================================
Name : wait_connect
==============================================================================
*/

void  IpcPortLocal::wait_connect (double timeout)
{
   _impl_aptr->wait_connect (timeout);
}



/*
==============================================================================
Name : recv
Description :
   Block until data is received.
   Returns true is data was received, false if the wait timed out.
==============================================================================
*/

bool  IpcPortLocal::recv (IpcData & data, double timeout)
{
   // quick fix: crash under windows until we fix the quit problem
   
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define ohm_task_IpcPortLocal_CHECK_CALL_MULTIPLE_CALL
#elif !defined NDEBUG
   #define ohm_task_IpcPortLocal_CHECK_CALL_MULTIPLE_CALL
#else
   #undef ohm_task_IpcPortLocal_CHECK_CALL_MULTIPLE_CALL
#endif

#if defined ohm_task_IpcPortLocal_CHECK_CALL_MULTIPLE_CALL
   /*if (_exclusive_flag)*/
   {
   
      const conc::AioAdd <long> ftor_inc (1);
      int recv_nbr = conc::AtomicIntOp_exec_new (_recv_cnt, ftor_inc);

#                 if 0
      assert (recv_nbr == 1);
      
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
      if (recv_nbr != 1)
      {
         ohm_lang_CRASH ();
      }
#endif
#                 endif
   }
#endif

   bool ok_flag = _impl_aptr->recv (data, timeout);
   
#if defined ohm_task_IpcPortLocal_CHECK_CALL_MULTIPLE_CALL
   /*if (_exclusive_flag)*/
   {
      const conc::AioAdd <long> ftor_inc (-1);
      conc::AtomicIntOp_exec (_recv_cnt, ftor_inc);
   }
#endif   

   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
