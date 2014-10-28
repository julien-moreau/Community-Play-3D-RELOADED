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

#include "ohm/task/win32/IpcPortRemoteImpl.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/util/trace.h"

#include <string>

#include <cassert>

#undef OHM_TASK_IPRI_DEBUG_FLAG

#if defined (OHM_TASK_IPRI_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_IPRI_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define ipri_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
   #define ipri_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define ipri_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define ipri_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define ipri_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define ipri_TRACE(msg_0)  (void (0))
   #define ipri_TRACE1(fmt_0, arg1) (void (0))
   #define ipri_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define ipri_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define ipri_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
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

IpcPortRemoteImpl::IpcPortRemoteImpl (const char * name_0)
:  _pipe_handle (INVALID_HANDLE_VALUE)
{
   assert (name_0 != 0);
   

   std::string name (name_0);
   assert (name.find ('\\') == std::string::npos);
   
   _name = name;

   std::string pipe_name = "\\\\.\\pipe\\" + name;
   
   // look up for local port receiving msgs
   ipri_TRACE1 ("IpcPortLocal %1% : ctor", _name.c_str ());
   
   while (_pipe_handle == INVALID_HANDLE_VALUE)
   {
      _pipe_handle = ::CreateFile (
         pipe_name.c_str (),
         GENERIC_WRITE,
         0,
         0,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
         0
      );
      
      ::DWORD err = ::GetLastError ();
      
      if (_pipe_handle != INVALID_HANDLE_VALUE)
      {
         // nothing
      }
      else
      {
         if (err == ERROR_FILE_NOT_FOUND)
         {
            sys::ThreadMgr::sleep (0.01);
         }
         else if (err == ERROR_PIPE_BUSY)
         {
            BOOL wait_res = ::WaitNamedPipe (
               pipe_name.c_str (),
               NMPWAIT_WAIT_FOREVER
            );
            assert (wait_res);   // no timeout
         }
         else
         {
            assert (false);
            throw (std::runtime_error ("error creating the remote port"));
            break;
         }
      }
   }
   
   assert (_pipe_handle != INVALID_HANDLE_VALUE);
   
   ::DWORD pipe_mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;

   BOOL res = ::SetNamedPipeHandleState (
      _pipe_handle,
      &pipe_mode,
      0,
      0
   );
   assert (res);

   ::memset (&_ol, 0, sizeof (_ol));
   _ol.hEvent = ::CreateEvent (0, TRUE, FALSE, 0);
   assert (_ol.hEvent != 0);

   ipri_TRACE1 ("IpcPortLocal %1% : ctor ended", _name.c_str ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

IpcPortRemoteImpl::~IpcPortRemoteImpl ()
{
   BOOL ret_val = ::CloseHandle (_pipe_handle);
   assert (ret_val);

   _pipe_handle = INVALID_HANDLE_VALUE;

   ret_val = ::CloseHandle (_ol.hEvent);
   assert (ret_val);
}



/*
==============================================================================
Name : hand_shake
==============================================================================
*/

void  IpcPortRemoteImpl::hand_shake ()
{
   // nothing
}



/*
==============================================================================
Name : send
==============================================================================
*/

bool  IpcPortRemoteImpl::send (const archi::UByte * data_ptr, size_t data_size)
{
   bool ret_val = false;

   assert (_pipe_handle != INVALID_HANDLE_VALUE);
   
   ::DWORD written_byte_count = 0;
   
   ipri_TRACE2 ("IpcPortLocal %1% : sending %2% bytes...", _name.c_str (), data_size);

   ::BOOL ok_flag = ::WriteFile (
      _pipe_handle,
      data_ptr,
      data_size,
      &written_byte_count,
      &_ol
   );

   ::DWORD err = ::GetLastError ();
   
   if (ok_flag)
   {
      assert (written_byte_count == data_size);
      ipri_TRACE2 ("IpcPortLocal %1% : sent %2% bytes", _name.c_str (), written_byte_count);
      ret_val = true;
   }
   else if (err == ERROR_IO_PENDING)
   {
      ipri_TRACE1 ("IpcPortLocal %1% : write pending", _name.c_str ());
      ret_val = true;
   }              
   else if (err == ERROR_BROKEN_PIPE)
   {
      ohm_util_TRACE_ERROR1 ("IpcPortLocal %1% : broken pipe", _name.c_str ());
   }
   else if (err == ERROR_PIPE_NOT_CONNECTED || err == ERROR_NO_DATA)
   {
      ohm_util_TRACE_ERROR1 ("IpcPortLocal %1% : pipe not connected", _name.c_str ());
   }
   else if (err == ERROR_INVALID_USER_BUFFER || err == ERROR_NOT_ENOUGH_MEMORY)
   {
      ohm_util_TRACE_ERROR1 ("IpcPortLocal %1% : too much write", _name.c_str ());
   }
   else
   {
      assert (false);
   }
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
