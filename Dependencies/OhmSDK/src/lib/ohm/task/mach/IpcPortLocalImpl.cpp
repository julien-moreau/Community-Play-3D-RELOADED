/*****************************************************************************

        IpcPortLocalImpl.cpp
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

#include "ohm/task/mach/IpcPortLocalImpl.h"
#include "ohm/task/IpcData.h"

#include "ohm/sys/ThreadMgr.h"
#include "ohm/util/trace.h"

#include <servers/bootstrap.h>

#include <cassert>



#define OHM_TASK_IPC_PORT_LOCAL_DEBUG_FLAG

#if defined (OHM_TASK_IPC_PORT_LOCAL_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_IPC_PORT_LOCAL_DEBUG_FLAG is defined in shipping level code !
   #endif
   
   #define ipl_TRACE(msg_0)   ohm_util_TRACE_DEBUG(msg_0)
   #define ipl_TRACE1(fmt_0, arg1)  ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define ipl_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define ipl_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define ipl_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
   #define ipl_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  ohm_util_TRACE_DEBUG5(fmt_0, arg1, arg2, arg3, arg4, arg5)
#else
   #define ipl_TRACE(msg_0)   (void (0))
   #define ipl_TRACE1(fmt_0, arg1)  (void (0))
   #define ipl_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define ipl_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define ipl_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
   #define ipl_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  (void (0))
#endif



namespace ohm
{
namespace task
{


/*
==============================================================================
Documentation :
   http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/
   http://flylib.com/books.php?ln=en&n=3&p=126&c=106&p1=1&c1=1&c2=161&view=1

==============================================================================
*/

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

IpcPortLocalImpl::IpcPortLocalImpl (const std::string & name)
:  _name (name)
,  _recv_port (MACH_PORT_NULL)
{
   mach_port_t cur_task = mach_task_self ();
   
   ohm_util_TRACE_INFO2 ("Registering service %1% for task %2%", name.c_str (), cur_task);
   
   kern_return_t err = KERN_SUCCESS;
   
   if (err == KERN_SUCCESS)
   {
      // returns receive rights
      
      ohm_util_TRACE_INFO ("  Allocating port with receive rights...");
      
      err = mach_port_allocate (
         cur_task, MACH_PORT_RIGHT_RECEIVE, &_recv_port
      );
      
      ohm_util_TRACE_INFO1 ("    recv_port %1x%", _recv_port);
      
      if (err != 0)
      {
         ohm_util_TRACE_ERROR2 ("    %1% : %2%", err, bootstrap_strerror (err));
      }
   }
   
   if (err == KERN_SUCCESS)
   {
      // returns send rights
      
      ohm_util_TRACE_INFO ("  Inserting send rights for remote...");
      
      err = mach_port_insert_right (
         cur_task, _recv_port, _recv_port, MACH_MSG_TYPE_MAKE_SEND
      );
      
      if (err != 0)
      {
         ohm_util_TRACE_ERROR2 ("    %1% : %2%", err, bootstrap_strerror (err));
      }
   }
   
   if (err == KERN_SUCCESS)
   {
      ohm_util_TRACE_INFO ("  Registering name...");
      
      err = bootstrap_register (
         bootstrap_port,
         const_cast <char *> (name.c_str ()),
         _recv_port
      );
      
      if (err != 0)
      {
         ohm_util_TRACE_ERROR2 ("    %1% : %2%", err, bootstrap_strerror (err));
      }
      
      ohm_util_TRACE_INFO2 ("mach recv port %1% : %2x%", name.c_str (), _recv_port);
   }
      
   if (err != KERN_SUCCESS)
   {
      assert (false);
      _recv_port = MACH_PORT_NULL;
   }
}



/*
==============================================================================
Name : dtor
Note :
   A local port has a send & receive right
==============================================================================
*/

IpcPortLocalImpl::~IpcPortLocalImpl ()
{
   try
   {
      mach_port_t cur_task = mach_task_self ();
      
      // Remove Receive Right
      mach_port_mod_refs (cur_task, _recv_port, MACH_PORT_RIGHT_RECEIVE, -1);
      
      // Remove Send Right
      mach_port_deallocate (cur_task, _recv_port);
      
      // Actually mach_port_get_refs returns an error, but we are relying on this
      
      mach_port_urefs_t recv_cnt = 0;
      mach_port_get_refs (cur_task, _recv_port, MACH_PORT_RIGHT_RECEIVE, &recv_cnt);
      assert (recv_cnt == 0);
      
      mach_port_urefs_t send_cnt = 0;
      mach_port_get_refs (cur_task, _recv_port, MACH_PORT_RIGHT_SEND, &send_cnt);
      assert (send_cnt == 0);

      _recv_port = MACH_PORT_NULL;
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : wait_connect
==============================================================================
*/

void  IpcPortLocalImpl::wait_connect (double timeout)
{
   IpcData data;
   bool ok_flag = recv (data, timeout);
   assert (ok_flag);
   
   assert (data.size () == 0);
}



/*
==============================================================================
Name : recv
Description :
   Block until data is received.
   Returns true is data was received, false if the wait timed out.
==============================================================================
*/

bool  IpcPortLocalImpl::recv (IpcData & data, double timeout)
{
   mach_msg_timeout_t msg_timeout
      = (timeout < 0.0)
      ? MACH_MSG_TIMEOUT_NONE
      : timeout * 1000;
   
   mach_msg_t msg;
   msg._header.msgh_bits = 0;
   msg._header.msgh_size = sizeof (mach_msg_t);
   msg._header.msgh_local_port = _recv_port;
   msg._header.msgh_remote_port = MACH_PORT_NULL;
   msg._header.msgh_id = 0;
   msg._header.msgh_reserved = 0;
   
   kern_return_t err = mach_msg (
      &msg._header,
      MACH_RCV_MSG,
      0, // send size
      sizeof (mach_msg_t),
      _recv_port,
      msg_timeout,
      MACH_PORT_NULL // notification port
   );
   
   if ((err != KERN_SUCCESS) && (err != MACH_RCV_TIMED_OUT))
   {
      ohm_util_TRACE_ERROR3 ("recv mach_msg %1x% %2% : %3%", _recv_port, err, bootstrap_strerror (err));
   }
   
   assert ((err == KERN_SUCCESS) || (err == MACH_RCV_TIMED_OUT));
   
   assert (msg._ool.deallocate);
   
   bool ok_flag = err != MACH_RCV_TIMED_OUT;
   
   if (ok_flag)
   {
      data = IpcData (
         reinterpret_cast <const archi::UByte *> (msg._ool.address),
         msg._ool.size
      );
   }
   
   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
