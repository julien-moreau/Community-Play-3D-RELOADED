/*****************************************************************************

        IpcPortBiDir.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67796

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

#include "ohm/task/IpcPortBiDir.h"
#include "ohm/task/IpcData.h"
#include "ohm/util/trace.h"

#include <string>

#include <cassert>



#define OHM_TASK_IPC_PORT_BIDIR_DEBUG_FLAG

#if defined (OHM_TASK_IPC_PORT_BIDIR_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_IPC_PORT_BIDIR_DEBUG_FLAG is defined in shipping level code !
   #endif
   
   #define ipb_TRACE(msg_0)   ohm_util_TRACE_DEBUG(msg_0)
   #define ipb_TRACE1(fmt_0, arg1)  ohm_util_TRACE_DEBUG1(fmt_0, arg1)
   #define ipb_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_DEBUG2(fmt_0, arg1, arg2)
   #define ipb_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_DEBUG3(fmt_0, arg1, arg2, arg3)
   #define ipb_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_DEBUG4(fmt_0, arg1, arg2, arg3, arg4)
   #define ipb_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  ohm_util_TRACE_DEBUG5(fmt_0, arg1, arg2, arg3, arg4, arg5)
#else
   #define ipb_TRACE(msg_0)   (void (0))
   #define ipb_TRACE1(fmt_0, arg1)  (void (0))
   #define ipb_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define ipb_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define ipb_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
   #define ipb_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5)  (void (0))
#endif



namespace ohm
{
namespace task
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Note :
   host must be created before guest.
==============================================================================
*/

IpcPortBiDir::IpcPortBiDir (const char * name_0, bool host_flag)
:  _host_flag (host_flag)
,  _name (name_0)
,  _log_flag (true)
,  _port_local_aptr ()
,  _port_remote_aptr ()
{
   if (host_flag)
   {
      ohm_util_TRACE_INFO1 ("Creating Bidir host %1%", name_0);
      
      _port_local_aptr = std::auto_ptr <IpcPortLocal> (
         new IpcPortLocal ((std::string (name_0) + ".host").c_str ())
      );
   }
   else
   {
      ohm_util_TRACE_INFO1 ("Creating Bidir guest %1%", name_0);
      
      _port_local_aptr = std::auto_ptr <IpcPortLocal> (
         new IpcPortLocal ((std::string (name_0) + ".guest").c_str ())
      );
      
      _port_remote_aptr = std::auto_ptr <IpcPortRemote> (
         new IpcPortRemote ((std::string (name_0) + ".host").c_str ())
      );
      
      ohm_util_TRACE_INFO ("  hand shaking");
      
      _port_remote_aptr->hand_shake ();
   }
}



/*
==============================================================================
Name : dtor
Note :
   guest must be released before host.
==============================================================================
*/

IpcPortBiDir::~IpcPortBiDir ()
{
   try
   {
      ohm_util_TRACE_INFO2 ("Releasing Bidir %1% %2%", _host_flag ? "host" : "guest", _name.c_str ());
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : wait_hand_shake
Note :
   host only : wait until guest hand shake
   we might get an old 'goodbye' in the queue : just ignore it
==============================================================================
*/

void  IpcPortBiDir::wait_hand_shake ()
{
   assert (_host_flag);
   
   _port_remote_aptr.reset ();
   
   ohm_util_TRACE_INFO1 ("Waiting Bidir hand shake %1%", _name.c_str ());

   _port_local_aptr->wait_connect ();
      
   _port_remote_aptr = std::auto_ptr <IpcPortRemote> (
      new IpcPortRemote ((_name + ".guest").c_str ())
   );
   
   ipb_TRACE ("  done");
}



/*
==============================================================================
Name : send
Description :
   Returns true if data could be delivered, false if "other side" local port
   was closed.
==============================================================================
*/

bool  IpcPortBiDir::send (const archi::UByte * data_ptr, size_t data_size)
{
   bool ok_flag = _port_remote_aptr->send (data_ptr, data_size);
   
   if ((!ok_flag) && _log_flag)
   {
      // log the problem once
      _log_flag = false;

      ohm_util_TRACE_ERROR2 ("Bidir %1% %2% send (MSG) failed (logged once)", _host_flag ? "host" : "guest", _name.c_str ());
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : recv
Description :
   Block until data is received.
   Returns true is data was received, false if the wait timed out.
==============================================================================
*/

IpcPortBiDir::Reply  IpcPortBiDir::recv (IpcData & data, double timeout)
{
   bool ok_flag = _port_local_aptr->recv (data);
   
   if (!ok_flag)
   {
      ipb_TRACE2 ("Bidir %1% %2% Timeout (MSG)", _host_flag ? "host" : "guest", _name.c_str ());
   }
   
   return ok_flag ? Reply_DATA_AVAILABLE : Reply_TIME_OUT;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
