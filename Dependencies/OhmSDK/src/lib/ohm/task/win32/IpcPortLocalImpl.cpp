/*****************************************************************************

        IpcPortLocalImpl.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70203

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

#include "ohm/math/fnc.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/task/win32/IpcPortLocalImpl.h"
#include "ohm/task/IpcData.h"
#include "ohm/util/trace.h"

#include <cassert>

#undef OHM_TASK_IPLI_DEBUG_FLAG

#if defined (OHM_TASK_IPLI_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error OHM_TASK_IPLI_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define ipli_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
   #define ipli_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define ipli_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define ipli_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define ipli_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define ipli_TRACE(msg_0)  (void (0))
   #define ipli_TRACE1(fmt_0, arg1) (void (0))
   #define ipli_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define ipli_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define ipli_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
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

IpcPortLocalImpl::IpcPortLocalImpl (const char * name_0)
:  _h_pipe (INVALID_HANDLE_VALUE)
,  _ol ()
,  _name (name_0)
,  _connected_flag (false)
,  _read_pending (false)
{
   assert (name_0 != 0);
   
   std::string name (name_0);
   
   assert (name.find ('\\') == std::string::npos);
   
   std::string pipe_name = std::string ("\\\\.\\pipe\\");
   pipe_name += name;

   const ::DWORD  open_mode
      = PIPE_ACCESS_INBOUND   // client to server only
      |  FILE_FLAG_OVERLAPPED 
   ;  
   
   const ::DWORD  pipe_mode
      = PIPE_TYPE_MESSAGE     // bytes are treated as a block of bytes
      | PIPE_READMODE_MESSAGE // rather than a stream of bytes
      | PIPE_WAIT             // block
   ;
   
   _h_pipe = ::CreateNamedPipeA (
      pipe_name.c_str (),
      open_mode,
      pipe_mode,
      1,             // nbr max instances
      512,           // out buf size, advisory
      0,             // in buf size
      0,             // default time out
      0              // secure attribs
   );
   if (_h_pipe == INVALID_HANDLE_VALUE)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "ctor(): CreateNamedPipeA() failed (\"%2%\"). "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code,
         pipe_name.c_str ()
      );
      assert (false);
      throw std::runtime_error (
         "ohm::task::IpcPortLocalImpl::ctor(): CreateNamedPipe() failed."
      );
   }
   else
   {
      ohm_util_TRACE_INFO1 (
         "ctor(): Opened named pipe \"%1%\"",
         _name.c_str ()
      );
   }
   
   ::memset (&_ol, 0, sizeof (_ol));
   _ol.hEvent = ::CreateEventA (0, TRUE, FALSE, 0);
   if (_ol.hEvent == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "ctor(): CreateEventA() failed (\"%2%\"). "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code,
         _name.c_str ()
      );
      assert (false);
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

IpcPortLocalImpl::~IpcPortLocalImpl ()
{
   try
   {
      if (_h_pipe == INVALID_HANDLE_VALUE)
      {
         assert (false);
      }
      else
      {
         if (_connected_flag)
         {
            // We don't log anything upon success because we might have been
            // destructed after the log manager if we are stored as a static
            // object.
            disconnect (false);
         }
         
         if (_ol.hEvent == INVALID_HANDLE_VALUE)
         {
            assert (false);
         }
         else
         {
            ::BOOL         res = ::CloseHandle (_ol.hEvent);
            if (res == 0)
            {
               const unsigned long  err_code = ::GetLastError ();
               ohm_util_TRACE_ERROR2 (
                  "dtor(): CloseHandle() failed on the event (\"%2%\"). "
                  "Returned Windows error code 0x%1rm8w0X%.",
                  err_code,
                  _name.c_str ()
               );
               assert (false);
            }
            _ol.hEvent = INVALID_HANDLE_VALUE;
         }

         ::BOOL         res = ::CloseHandle (_h_pipe);
         if (res == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR2 (
               "dtor(): CloseHandle() failed on the pipe (\"%2%\"). "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code,
               _name.c_str ()
            );
            assert (false);
         }
         _h_pipe = INVALID_HANDLE_VALUE;
      }
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
   if (!_connected_flag)
   {
      // wait for remote to connect synchronously, and then only process messages
      
      connect_sync (timeout);
   }
}



/*
==============================================================================
Name : recv
Description :
   Block until data is received.
   timeout is in seconds.
Returns :
   True if the data was received before the timeout.
   False if no connection or no reception during the timout
   False if the pipe was broken (client disconnected)
==============================================================================
*/

bool  IpcPortLocalImpl::recv (IpcData & data, double timeout)
{
   ::DWORD     timeout_ms = INFINITE;

   if (timeout > 0)
   {
      timeout_ms = math::ceil_int (timeout * 1000);
   }

   bool        retry_flag = false;
   bool        ok_flag = false;
   do
   {
      retry_flag = false;

      if (!_connected_flag)
      {
         // wait for remote to connect synchronously, and then only process messages
         
         connect_sync (timeout);
      }
      
      ok_flag = _connected_flag;
      bool  read_flag = false;
      
      if (ok_flag && !_read_pending)
      {  
         // no read pending, try to read. 
                  
         ipli_TRACE1 ("IpcPortLocal %1% : reading...", _name.c_str ());

         ::BOOL ret_val = ::ReadFile (
            _h_pipe, 
            0,
            0,
            0,
            &_ol
         );
         ::DWORD err = ::GetLastError ();

         if (ret_val || err == ERROR_MORE_DATA)
         {
            ipli_TRACE1 ("IpcPortLocal %1% : read more data", _name.c_str ());
            read_flag = true;
         }
         else if (err == ERROR_IO_PENDING)
         {
            ipli_TRACE1 ("IpcPortLocal %1% : read pending", _name.c_str ());
            _read_pending = true;
         }              
         else if (err == ERROR_BROKEN_PIPE)
         {
            disconnect ();
            ok_flag = false;
            retry_flag = true;
         }
         else
         {
            assert (false);
         }
      }
      
      if (ok_flag && _read_pending)
      {
         // read pending, wait timout
      
         ipli_TRACE1 ("IpcPortLocal %1% : read waiting...", _name.c_str ());

         ::DWORD err = ::WaitForSingleObject (_ol.hEvent, timeout_ms);
         
         if (err == WAIT_OBJECT_0)
         {
            // go on and check the result
            ipli_TRACE1 ("IpcPortLocal %1% : read ready", _name.c_str ());
            _read_pending = false;
         }
         else if (err == WAIT_TIMEOUT)
         {
            // read timeout
            ipli_TRACE1 ("IpcPortLocal %1% : read timeout", _name.c_str ());
            ok_flag = false;
         }
         else
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "recv(): WaitForSingleObject() failed. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            ok_flag = false;
         }

         if (ok_flag)
         {
            // this is where we learn the pending operation was cancelled
            ::DWORD  read_len = 0;
            ::BOOL   ol_res = ::GetOverlappedResult (
               _h_pipe,
               &_ol,
               &read_len,
               FALSE
            );
            ::DWORD err = ::GetLastError ();
            
            if (ol_res || err == ERROR_MORE_DATA)
            {
               read_flag = true;
            }
            else if (err == ERROR_BROKEN_PIPE)
            {
               disconnect ();
               ok_flag = false;
               retry_flag = true;
            }
            else if (err == ERROR_OPERATION_ABORTED)
            {
               ok_flag = false;
               retry_flag = true;
            }
            else
            {
               assert (false);
            }
         }
      }
      
      if (ok_flag)
      {
         // data is available, read the whole message
         
         assert (read_flag);
         
         ::DWORD msg_len = 0;
         ::BOOL ret_val = ::PeekNamedPipe (
            _h_pipe,
            0,
            0,
            0,
            0,
            &msg_len
         );
         assert (ret_val);
         
         archi::UByte * data_ptr = 0;
         
         if (msg_len > 0)
         {
            data_ptr = new archi::UByte [msg_len];
         }
         
         ::DWORD read_len = 0;
         ret_val = ::ReadFile (
            _h_pipe,
            data_ptr,
            msg_len,
            &read_len,
            &_ol
         );
         
         assert (msg_len == read_len);
         assert (ret_val);

         ipli_TRACE2 ("IpcPortLocal %1% : read %2% bytes", _name.c_str (), read_len);

         data = IpcData (
            data_ptr,
            read_len
         );
      }
   }
   while (retry_flag);     

   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: connect_sync
Description : 
   Can eventually fail (timeout...)
   Then _connected_flag remains false.
==============================================================================
*/

void IpcPortLocalImpl::connect_sync (double timeout)
{
   assert (!_connected_flag);
   assert (_h_pipe != INVALID_HANDLE_VALUE);
   
   ipli_TRACE1 ("IpcPortLocal %1% : connecting...", _name.c_str ());

   ::BOOL res = ::ConnectNamedPipe (
      _h_pipe,
      &_ol
   );
   ::DWORD err = ::GetLastError ();
   
   // returns non zero only if the pipe was disconnected
   assert (res == FALSE);
   
   if (err == ERROR_PIPE_CONNECTED)
   {
      ipli_TRACE1 ("IpcPortLocal %1% : connected.", _name.c_str ());
      _connected_flag = true;
   }
   else if (err == ERROR_IO_PENDING)
   {
      ::DWORD     timeout_ms = INFINITE;

      if (timeout > 0)
      {
         timeout_ms = ::DWORD (math::ceil_int (timeout * 1000));
      }

      err = ::WaitForSingleObject (_ol.hEvent, timeout_ms);
      
      if (err == WAIT_OBJECT_0)
      {
         ipli_TRACE1 ("IpcPortLocal %1% : connected.", _name.c_str ());
         _connected_flag = true;
      }
      else if (err == WAIT_TIMEOUT)
      {
         ipli_TRACE1 ("IpcPortLocal %1% : connection timed out.", _name.c_str ());
         // connection failed
      }
      else
      {
         ohm_util_TRACE_ERROR1 (
            "connect_sync(): WaitForSingleObject failed, "
            "Windows error code 0x%1rm8w0X%",
            err
         );
         assert (false);   // unexpected error
      }
   }
   else if (err == ERROR_NO_DATA)
   {
      // In some situations this error message fills the logs at extremely
      // high rate. Try to minimise the consequences.
      static archi::Int64  error_counter = 0;
      static archi::Int64  error_limit   = 1000;

      ++ error_counter; // We don't really care if it's not atomic.
      if (    error_counter <= error_limit
          || (error_counter % error_limit) == 0)
      {
         ohm_util_TRACE_ERROR (
            "connect_sync(): ConnectNamedPipe() returned ERROR_NO_DATA."
         );
      }
      if (error_counter == error_limit)
      {
         ohm_util_TRACE_ERROR1 (
            "connect_sync(): Too many ERROR_NO_DATA, "
            "logging only every %1% errors from now.",
            error_limit
         );
      }
      else if (error_counter > error_limit * 10 - 1)
      {
         error_limit *= 10;
      }
   }
   else
   {
      ohm_util_TRACE_ERROR1 (
         "connect_sync(): ConnectNamedPipe() failed, "
         "Windows error code 0x%1rm8w0X%",
         err
      );
      assert (false);      // unexpected error
   }
   
   assert (_connected_flag);
}



/*
==============================================================================
Name: disconnect
==============================================================================
*/

void IpcPortLocalImpl::disconnect (bool log_flag)
{
   const ::BOOL      res = ::DisconnectNamedPipe (_h_pipe);
   if (res == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "disconnect(): DisconnectNamedPipe() failed (\"%2%\"). "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code,
         _name.c_str ()
      );
   }
   else
   {
      if (log_flag)
      {
         ohm_util_TRACE_INFO1 (
            "disconnect(): Closed named pipe \"%1%\"",
            _name.c_str ()
         );
      }
   }

   _connected_flag = false;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
