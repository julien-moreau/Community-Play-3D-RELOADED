/*****************************************************************************

        IpcPortRemoteImpl.cpp
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

#include "ohm/task/mach/IpcPortRemoteImpl.h"

#include "ohm/sys/ThreadMgr.h"
#include "ohm/util/trace.h"

#include <servers/bootstrap.h>

#include <cassert>



#define OHM_TASK_IPC_PORT_REMOTE_DEBUG_FLAG

#if defined (OHM_TASK_IPC_PORT_REMOTE_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_IPC_PORT_REMOTE_DEBUG_FLAG is defined in shipping level code !
   #endif
   
   #define ipr_TRACE(msg_0)   ohm_util_TRACE_DEBUG(msg_0)
   #define ipr_TRACE1(fmt_0, arg1)  ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define ipr_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define ipr_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define ipr_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
   #define ipr_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  ohm_util_TRACE_DEBUG5(fmt_0, arg1, arg2, arg3, arg4, arg5)
#else
   #define ipr_TRACE(msg_0)   (void (0))
   #define ipr_TRACE1(fmt_0, arg1)  (void (0))
   #define ipr_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define ipr_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define ipr_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
   #define ipr_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  (void (0))
#endif



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

IpcPortRemoteImpl::IpcPortRemoteImpl (const std::string & name)
:  _name (name)
,  _send_port (MACH_PORT_NULL)
{
   ohm_util_TRACE_INFO1 ("Looking up %1%", name.c_str ());
   
   bool retry_flag = true;
   
   while (retry_flag)
   {
      // returns send right
      
      kern_return_t err = bootstrap_look_up (
         bootstrap_port,
         const_cast <char *> (name.c_str ()),
         &_send_port
      );
      
      // it this fails, ensure that the local port is created before
      // the remote port
      assert ((err == KERN_SUCCESS) || (err == BOOTSTRAP_UNKNOWN_SERVICE));
      
      retry_flag = err == BOOTSTRAP_UNKNOWN_SERVICE;
      
      if (retry_flag)
      {
         ohm_util_TRACE_INFO ("  Retrying...");
         
         sys::ThreadMgr::sleep (0.01);
      }
   }
   
   ohm_util_TRACE_INFO1 ("  send_port %1x%", _send_port);
   ohm_util_TRACE_INFO2 ("mach send port %1% : %2x%", name.c_str (), _send_port);
   
   assert (_send_port != MACH_PORT_NULL);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

IpcPortRemoteImpl::~IpcPortRemoteImpl ()
{
   try
   {
      mach_port_t cur_task = mach_task_self ();
      kern_return_t err = mach_port_deallocate (cur_task, _send_port);
      assert (err == KERN_SUCCESS);
      
      // Actually mach_port_get_refs returns an error, but we are relying on this
      
      mach_port_urefs_t send_cnt = 0;
      mach_port_get_refs (cur_task, _send_port, MACH_PORT_RIGHT_SEND, &send_cnt);
      assert (send_cnt == 0);

      _send_port = MACH_PORT_NULL;
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : hand_shake
==============================================================================
*/

void  IpcPortRemoteImpl::hand_shake ()
{
   bool ok_flag = send (0, 0);
   assert (ok_flag);
}



/*
==============================================================================
Name : send
Note :
   impl :
   http://www.opensource.apple.com/source/xnu/xnu-1456.1.26/libsyscall/mach/mach_msg.c
   man :
   http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/
==============================================================================
*/

bool  IpcPortRemoteImpl::send (const archi::UByte * data_ptr, size_t data_size)
{
   assert (_send_port != MACH_PORT_NULL);
   
   // MACH_MSGH_BITS_COMPLEX is specified as the msg body
   // has one additional out-of-line memory region
   
   mach_msg_t msg;
   msg._header.msgh_bits = MACH_MSGH_BITS (MACH_MSG_TYPE_COPY_SEND, 0);
   msg._header.msgh_bits |= MACH_MSGH_BITS_COMPLEX;
   msg._header.msgh_size = sizeof (mach_msg_t);
   msg._header.msgh_local_port = MACH_PORT_NULL;
   msg._header.msgh_remote_port = _send_port;
   msg._header.msgh_id = 0;
   msg._header.msgh_reserved = 0;
   
   // number of additional descriptor in the msg
   msg._body.msgh_descriptor_count = 1;
   
   // actual additional descriptor (out-of-line memory region)
   // MACH_MSG_VIRTUAL_COPY tells the kernel to use whatever
   // suitable mechanism to transmit the data 
   
   const void * address = data_ptr;
   
   msg._ool.type = MACH_MSG_OOL_DESCRIPTOR;
   msg._ool.address = const_cast <void *> (address);
   msg._ool.size = data_size;
   msg._ool.deallocate = false;
   msg._ool.copy = MACH_MSG_VIRTUAL_COPY;
   
   // send
   
   kern_return_t err = mach_msg (
      &msg._header,
      MACH_SEND_MSG,
      sizeof (mach_msg_t),
      0, MACH_PORT_NULL, // receive limit/name
      MACH_MSG_TIMEOUT_NONE,
      MACH_PORT_NULL // notification port
   );
   
   assert ((err == KERN_SUCCESS) || (err == MACH_SEND_INVALID_DEST));
   
   bool ok_flag = (err == KERN_SUCCESS);
   
   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
