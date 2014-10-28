/*****************************************************************************

        RunLoopGuiImpl.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70989

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

#include "ohm/sys/win32/ExceptionHandler.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/task/win32/AcceleratorInterface.h"
#include "ohm/task/win32/RunLoopGuiImpl.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/util/trace.h"

#include <cassert>



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

RunLoopGuiImpl::RunLoopGuiImpl (RunLoopGui & parent)
:  _parent (parent)
,  _custom_msg_index (::RegisterWindowMessageW (L"ohm.task.RunLoopGuiImpl"))
,  _stop_flag (false)
,  _accelerator_ptr (0)
,  _custom_msg_count (0)
,  _hwnd (0)
,  _current_processed_msg (WM_NULL)
,  _crumb_proc (0)
,  _crumb_user_data (0)
{
   assert (&parent != 0);
   assert (_custom_msg_index != 0);

   // Force the message queue to be created.
   ::MSG          msg;
   ::PeekMessageW (&msg, 0, WM_USER, WM_USER, PM_NOREMOVE);

   int            ret_val = 0;

   // Registers the window class
   if (ret_val == 0)
   {
      WNDCLASSEXW    wcex;

      // Creates the parent window class
      wcex.cbSize        = sizeof (WNDCLASSEX); 
      wcex.style         = 0;
      wcex.lpfnWndProc   = reinterpret_cast <WNDPROC> (win_proc_static);
      wcex.cbClsExtra    = 0;
      wcex.cbWndExtra    = 0;
      wcex.hInstance     = 0; // hinstance should not be 0 ???
      wcex.hIcon         = 0;
      wcex.hCursor       = 0;
      wcex.hbrBackground = 0;
      wcex.lpszMenuName  = 0;
      wcex.lpszClassName = _window_class_name;
      wcex.hIconSm       = 0;
      const ::ATOM   win_ret_val = ::RegisterClassExW (&wcex);
      if (win_ret_val == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         if (err_code != ERROR_CLASS_ALREADY_EXISTS)
         {
            ohm_util_TRACE_ERROR1 (
               "ctor(): Cannot register the window class. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
         }
      }
   }

   // Creates the window
   if (ret_val == 0)
   {
      _hwnd = ::CreateWindowExW (
         0,
         _window_class_name,
         L"Something very ugly",
         0,
         0, 0,
         0, 0,
         HWND_MESSAGE,
         0,
         ::GetModuleHandle (0),
         0
      );
      if (_hwnd == 0)
      {
         ohm_util_TRACE_ERROR ("open_window(): Window creation failed.");
         ret_val = -1;
      }
   }

   if (ret_val == 0)
   {
      ::SetLastError (0);
      LONG_PTR    win_ret_val = ::SetWindowLongPtrW (
         _hwnd,
         GWLP_USERDATA,
         (::LONG_PTR) (this)
      );
      if (win_ret_val == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         if (err_code != 0)
         {
            ohm_util_TRACE_ERROR1 (
               "open_window(): Cannot set the pointer. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            ret_val = -1;
         }
      }
   }

   if (ret_val != 0)
   {
      ohm_util_TRACE_ERROR ("ctor(): Window opening failed.");
      throw std::runtime_error (
         "ohm::task::RunLoopGuiImpl::RunLoopGuiImpl(): "
         "Window opening failed."
      );
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

RunLoopGuiImpl::~RunLoopGuiImpl ()
{
   try
   {
      // nothing
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  RunLoopGuiImpl::bind (AcceleratorInterface & accelerator)
{
   assert (&accelerator != 0);
   assert (_accelerator_ptr == 0);
   
   _accelerator_ptr = &accelerator;
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  RunLoopGuiImpl::unbind (AcceleratorInterface & accelerator)
{
   assert (_accelerator_ptr == &accelerator);
   
   _accelerator_ptr = 0;
}



/*
==============================================================================
Name : bind_crumb
==============================================================================
*/

void  RunLoopGuiImpl::bind_crumb (CrumbProc crumb_proc, void * user_data)
{
   _crumb_proc = crumb_proc;
   _crumb_user_data = user_data;
}



/*
==============================================================================
Name : run
Reference :
   "About Messages and Message Queues" MSDN
   http://msdn.microsoft.com/en-us/library/windows/desktop/ms644927(v=vs.85).aspx
==============================================================================
*/

void  RunLoopGuiImpl::run ()
{
   _stop_flag = false;
   bool           loop_flag = true;

   do
   {
      const int      timeout_ms = 50;

      ::DWORD        result = ::MsgWaitForMultipleObjectsEx (
         0,
         NULL,
         timeout_ms,
         QS_ALLINPUT,
         MWMO_INPUTAVAILABLE
      );

      if (_custom_msg_count > 0)
      {
         conc::AioAdd <int>   op (-1);
         const int      new_count =
            conc::AtomicIntOp_exec_new (_custom_msg_count, op);
#if 0
         if (new_count != 0)
         {
            ohm_util_TRACE_ERROR3 (
               "#### %1%: receiving message %2%, "
               "%3% signal messages still in the queue",
               int (_hwnd),
               _custom_msg_index,
               new_count
            );
         }
#endif

         _parent.req_process ();
      }

      if (result == WAIT_OBJECT_0 + 0) // + nCount
      {
         ::MSG          msg;
         const ::BOOL   gm_ret_val = ::GetMessageW (&msg, 0, 0, 0);
         _current_processed_msg = msg.message;

#if 0
         ohm_util_TRACE_INFO1 ("**** %1%: receiving message %2%", int (msg.hwnd), msg.message);
#endif

         bool accel_flag = false;

         if (_accelerator_ptr != 0)
         {
            accel_flag = _accelerator_ptr->translate (msg);
         }

         if (! accel_flag)
         {
            const bool     internal_msg_flag = (msg.message == _custom_msg_index);
            const bool     user_input_flag = ! internal_msg_flag;

            if (gm_ret_val == 0) // Intercepts the WM_QUIT message
            {
               _stop_flag = true;
            }
#if 0
            else if (internal_msg_flag)
            {
               ohm_util_TRACE_INFO2 ("**** %1%: message %2% processed in the runloop.", int (msg.hwnd), msg.message);
               _parent.req_process ();
            }
#endif
            else
            {
               ::TranslateMessage (&msg);
               LRESULT ret = ::DispatchMessageW (&msg);
               
               handle_crumb_event (ret, msg);
            }
            
            if (_stop_flag)
            {
               loop_flag = false;
            }
         }

         _current_processed_msg = WM_NULL;
      }
   }
   while (loop_flag);

   _stop_flag = false;
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  RunLoopGuiImpl::stop ()
{
   ::PostQuitMessage (0);
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  RunLoopGuiImpl::signal ()
{
   conc::AioAdd <int>   op (1);
   const int      old_count =
      conc::AtomicIntOp_exec_old (_custom_msg_count, op);

#if 0 // Debug stuff
   if (old_count != 0)
   {
      ohm_util_TRACE_ERROR3 (
         "#### %1%: sending message %2%, "
         "already %3% signal messages in the queue",
         int (_hwnd),
         _custom_msg_index,
         old_count
      );
   }
   else
   {
      ohm_util_TRACE_INFO2 (
         "#### %1%: sending message %2%",
         int (_hwnd),
         _custom_msg_index
      );
   }
#endif

   const ::BOOL      win_ret =
      ::PostMessageW (_hwnd, _custom_msg_index, 0, 0);
   
   if (win_ret == 0)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR1 (
         "signal(): PostMessageW() failed. "
         "Returned Windows error code 0x%1rm8w0X%.",
         err_code
      );
      assert (false);
   }
}



/*
==============================================================================
Name : get_time
==============================================================================
*/

double   RunLoopGuiImpl::get_time ()
{
   sys::TimerReliable &    tf = sys::TimerReliable::use_instance ();
   const double   timestamp = tf.get ();

   return (timestamp);
}



/*
==============================================================================
Name : is_processing_timer_message
==============================================================================
*/

bool  RunLoopGuiImpl::is_processing_timer_message () const
{
   return (_current_processed_msg == WM_TIMER);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : handle_crumb_event
==============================================================================
*/

void  RunLoopGuiImpl::handle_crumb_event (::LRESULT dispatch_ret, const ::MSG & msg)
{
   if (_crumb_proc == 0)
   {
      return;  // abort
   }
   
   switch (msg.message)
   {
   case WM_KEYDOWN:
   case WM_SYSKEYDOWN:
   case WM_KEYUP:
   case WM_SYSKEYUP:
   case WM_CHAR:
   case WM_UNICHAR:
      if (dispatch_ret == 0)
      {
         // message was not processed
         
         _crumb_proc (msg, _crumb_user_data);
      }
      break;
   }
}



/*
==============================================================================
Name : win_proc_static
==============================================================================
*/

::LRESULT CALLBACK   RunLoopGuiImpl::win_proc_static (::HWND hwnd, ::UINT msg, ::WPARAM wparam, ::LPARAM lparam)
{
   __try
   {
      // Nothing
   }
   __except (sys::win32::ExceptionHandler::exec (GetExceptionInformation ()))
   {
      assert (false);
   }

   return (::DefWindowProcW (hwnd, msg, wparam, lparam));
}



const wchar_t  RunLoopGuiImpl::_window_class_name [] = L"drunk_cat_urinating_on_my_pillow";



}  // namespace task
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
