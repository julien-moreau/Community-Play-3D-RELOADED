/*****************************************************************************

        fnc_win32.cpp
        Copyright (c) 2011 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 71199

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#define  NOMINMAX
#define  WIN32_LEAN_AND_MEAN

#include "ohm/math/fnc.h"
#include "ohm/sys/win32/fnc_win32.h"
#include "ohm/sys/win32/LocalUseCom.h"
#include "ohm/sys/Err.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/utf16/utf16_fnc.h"
#include "ohm/txt/fnc.h"
#include "ohm/txt/String16.h"
#include "ohm/util/trace.h"

#include <windows.h>
#include <Objbase.h>
#include <shlguid.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shobjidl.h>
#include <Tlhelp32.h>

#include <cassert>



namespace ohm
{
namespace sys
{
namespace win32
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



std::string  get_csidl_path (int csidl)
{
   txt::Char16    path_w_0 [MAX_PATH + 1];

   if (::SHGetSpecialFolderPathW (0, path_w_0, csidl, 0) != TRUE)
   {
      const unsigned long  err_code = ::GetLastError ();
      ohm_util_TRACE_ERROR2 (
         "FileRefFactory_get_csidl_path(): "
         "Cannot retrieve specific directory (CSIDL 0x%1rm8w0X%). "
         "Returned Windows error code 0x%2rm8w0X%.",
         csidl,
         err_code
      );
      throw std::runtime_error (
         "ohm::sys::FileRefFactory_get_csidl_path(): "
         "Cannot retrieve special folder path."
      );
   }

   std::string    path;
   const int      ret_val = txt::conv_utf16_to_utf8 (path, path_w_0);

   if (ret_val != Err_OK)
   {
      ohm_util_TRACE_ERROR (
         "FileRefFactory_get_csidl_path(): Cannot convert path to UTF-8."
      );
      throw std::runtime_error (
         "ohm::sys::FileRefFactory_get_csidl_path(): "
         "Cannot convert path to UTF-8."
      );
   }

   return (path);
}



int   execute_process (const std::string &path, const std::string &cur_dir, int flags, bool inherit_flag, bool init_wait_flag, bool load_feedback_flag)
{
   int            ret_val = Err_OK;

   txt::String16  path_utf16;
   if (ret_val == Err_OK)
   {
      ret_val = txt::conv_utf8_to_utf16 (path_utf16, path.c_str ());
   }

   txt::String16  cur_dir_utf16;
   if (ret_val == Err_OK && ! cur_dir.empty ())
   {
      ret_val = txt::conv_utf8_to_utf16 (cur_dir_utf16, cur_dir.c_str ());
   }

   ::BOOL         res_create = 0;
   ::PROCESS_INFORMATION proc_info;
   if (ret_val == Err_OK)
   {
      ohm_util_TRACE_INFO1 (
         "execute_process(): Lauching \"%1%\"",
         path.c_str ()
      );

      ::STARTUPINFOW startup_info;
      ::memset (&startup_info, 0, sizeof (startup_info));
      startup_info.cb = sizeof (startup_info);
      if (load_feedback_flag)
      {
         startup_info.dwFlags = STARTF_FORCEONFEEDBACK;
      }
      else
      {
         startup_info.dwFlags = STARTF_FORCEOFFFEEDBACK;
      }

      res_create = ::CreateProcessW (
         path_utf16.c_str (), 
         0,                            // command line string
         0,                            // process attr
         0,                            // thread attr
         inherit_flag ? TRUE : FALSE,  // inherit handles
         flags,                        // creation flags
         0,                            // environement
         cur_dir.empty () ? 0 : cur_dir_utf16.c_str (),  // current dir
         &startup_info,    
         &proc_info
      );

      if (res_create == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "execute_process(): Cannot create process. "
            "Returned Windows error code 0x%1rm8w0X%.",
            err_code
         );
         ret_val = Err_CANNOT_CREATE_PROCESS;
      }
   }

   // CreateProcess is not synchronous.
   if (ret_val == Err_OK && init_wait_flag)
   {
      const ::DWORD  res_wait = ::WaitForInputIdle (
         proc_info.hProcess,
         INFINITE
      );
      if (res_wait != 0)
      {
         ohm_util_TRACE_ERROR ("execute_process(): wait failed.");
         assert (false);
      }
   }

   if (res_create != 0)
   {
      // We don't need the proc info
      ::CloseHandle (proc_info.hProcess);
      ::CloseHandle (proc_info.hThread);
   }

   return (ret_val);
}



// Output: pid is the process identifier (DWORD)
// Returns true if found.
bool  find_process (unsigned long &pid, const std::string &path_substring)
{
   assert (&path_substring != 0);
   assert (! path_substring.empty ());

   bool           ok_flag = false;
   pid = 0;

   txt::String16  path16_ref;
   int            ret_val =
      txt::conv_utf8_to_utf16 (path16_ref, path_substring.c_str ());

   if (ret_val != txt::Err_OK)
   {
      ohm_util_TRACE_ERROR ("find_process(): illformed UTF-8 path.");
   }

   else
   {
      // Enumerates the processes
      const ::HANDLE snapshot_process =
         ::CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
      if (snapshot_process != INVALID_HANDLE_VALUE)
      {
         ::PROCESSENTRY32  process_entry;
         process_entry.dwSize = sizeof (PROCESSENTRY32);
         if (::Process32First (snapshot_process, &process_entry))
         {
            do
            {
               ::HANDLE       p_hnd = ::OpenProcess (
                  PROCESS_QUERY_INFORMATION,
                  FALSE,
                  process_entry.th32ProcessID
               );

               if (p_hnd != 0)
               {
                  // Enumerates the modules attached to the scanned process.
                  const ::HANDLE    snapshot_module = ::CreateToolhelp32Snapshot (
                     TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
                     process_entry.th32ProcessID
                  );

                  if (snapshot_module != INVALID_HANDLE_VALUE)
                  {
                     ::MODULEENTRY32W  module_entry;
                     module_entry.dwSize = sizeof (MODULEENTRY32W);
                     if (::Module32FirstW (snapshot_module, &module_entry))
                     {
                        do
                        {
                           long        dummy_pos;
                           const int   ret_val = txt::utf16::strstr_ci (
                              module_entry.szModule,
                              path16_ref.c_str (),
                              dummy_pos
                           );
                           if (ret_val == txt::Err_OK)
                           {
                              pid = process_entry.th32ProcessID;
                              ok_flag = true;
                           }
                        }
                        while (! ok_flag && ::Module32NextW (snapshot_module, &module_entry));
                     }

                     ::CloseHandle (snapshot_module);
                  }

                  ::CloseHandle (p_hnd);
               }
            }
            while (! ok_flag && ::Process32Next (snapshot_process, &process_entry));
         }

         ::CloseHandle (snapshot_process);
      }
   }

   return (ok_flag);
}



// timeout_s special values:
// -  0: returns instantly
// - -1: waits indefinitely
// Reaching the timeout (if any) is accounted as an error.
int   kill_process_by_name (const char *name_0, double timeout_s)
{
   int            ret_val = Err_OK;

   int            timeout_ms = INFINITE;
   if (timeout_s >= 0)
   {
      timeout_ms = math::round_int (timeout_s * 1000);
   }

   assert (name_0 != 0);

   unsigned long  pid;
   while (ret_val == Err_OK && find_process (pid, name_0))
   {
      ohm_util_TRACE_INFO1 (
         "kill_process_by_name(): "
         "Remaining process \"%1%\" has been detected and must be killed.",
         name_0
      );

      ::HANDLE       phnd = ::OpenProcess (
         PROCESS_TERMINATE | SYNCHRONIZE, // dwDesiredAccess
         FALSE,                           // bInheritHandle
         pid                              // dwProcessId
      );
      if (phnd == 0)
      {
         const unsigned long  err_code = ::GetLastError ();
         ohm_util_TRACE_ERROR1 (
            "kill_process_by_name(): Cannot open the process. "
            "Returned Windows error code 0x%1rm8w0X%.",
            err_code
         );
         ret_val = Err_CANNOT_KILL_PROCESS;
      }

      if (ret_val == Err_OK)
      {
         const ::BOOL   tp_ret = ::TerminateProcess (phnd, 0);
         if (tp_ret == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "kill_process_by_name(): Cannot kill the process. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
            ret_val = Err_CANNOT_KILL_PROCESS;
         }
      }

      if (ret_val == Err_OK && timeout_ms > 0)
      {
         const ::DWORD  ret_wfso = ::WaitForSingleObject (phnd, timeout_ms);
         if (ret_wfso != WAIT_OBJECT_0)
         {
            ohm_util_TRACE_ERROR1 (
               "kill_process_by_name(): WaitForSingleObject "
               "returned 0x%1rm8w0X%.",
               ret_wfso
            );
            
            if (ret_wfso == WAIT_FAILED)
            {
               const unsigned long  err_code = ::GetLastError ();
               ohm_util_TRACE_ERROR1 (
                  "kill_process_by_name(): WaitForSingleObject "
                  "returned Windows error code 0x%1rm8w0X%.",
                  err_code
               );
            }
            
            ret_val = Err_CANNOT_KILL_PROCESS;              
         }
      }

      if (phnd != 0)
      {
         const ::BOOL   cl_ret = ::CloseHandle (phnd);
         if (cl_ret == 0)
         {
            const unsigned long  err_code = ::GetLastError ();
            ohm_util_TRACE_ERROR1 (
               "kill_process_by_name(): Cannot close the process handle. "
               "Returned Windows error code 0x%1rm8w0X%.",
               err_code
            );
         }
         phnd = 0;
      }
   }

   return (ret_val);
}



// Does not generate any log.
// One can use THREAD_ALL_ACCESS for access_flags
int   iterate_on_threads (unsigned long process_id, bool cur_thread_flag, CbThreadOp callback_ptr, void *user_data_ptr, long access_flags)
{
   assert (callback_ptr != 0);

   const ::DWORD  cur_thread_id  = ::GetCurrentThreadId ();

   const ::HANDLE snapshot_threads =
      ::CreateToolhelp32Snapshot (TH32CS_SNAPTHREAD, 0);
   if (snapshot_threads != INVALID_HANDLE_VALUE)
   {
      ::THREADENTRY32   thread_entry;
      thread_entry.dwSize = sizeof (thread_entry);
      if (::Thread32First (snapshot_threads, &thread_entry))
      {
         do
         {
            const int      ofs =
                 FIELD_OFFSET (THREADENTRY32, th32OwnerProcessID)
               + sizeof (thread_entry.th32OwnerProcessID);
            if (thread_entry.dwSize >= ofs)
            {
               const ::DWORD  tst_process_id = thread_entry.th32OwnerProcessID;
               const ::DWORD  tst_thread_id  = thread_entry.th32ThreadID;
               if (   tst_process_id == process_id
                   && (tst_thread_id != cur_thread_id || cur_thread_flag))
               {
                  ::HANDLE       thread_handle = ::OpenThread (
                     access_flags,  // dwDesiredAccess
                     TRUE,             // bInheritHandle
                     tst_thread_id     // dwThreadId
                  );
                  if (thread_handle != 0)
                  {
                     callback_ptr (user_data_ptr, thread_handle, tst_thread_id);
                     ::CloseHandle (thread_handle);
                  }
               }
            }
            thread_entry.dwSize = sizeof (thread_entry);
         }
         while (::Thread32Next (snapshot_threads, &thread_entry));
      }

      ::CloseHandle (snapshot_threads);
   }

   return (Err_OK);
}



// path_link and path_real must be set.
// path_link directory must exist.
// If work_dir ore arguments are empty, they are not specified in the link.
int   create_shortcut (const std::string &path_link, const std::string &path_real, const std::string &work_dir, const std::string &arguments)
{
   assert (&path_link != 0);
   assert (! path_link.empty ());
   assert (&path_real != 0);
   assert (! path_real.empty ());
   assert (&work_dir != 0);
   assert (&arguments != 0);

   int            ret_val = Err_OK;

   LocalUseCom    com_session (COINIT_APARTMENTTHREADED, false);
   if (! com_session.is_successfully_initialised ())
   {
      ret_val = Err_CANNOT_INITIALISE_COM;
   }

   txt::String16  path_link_utf16;
   txt::String16  path_real_utf16;
   txt::String16  work_dir_utf16;
   txt::String16  arguments_utf16;
   if (ret_val == Err_OK)
   {
      ret_val = txt::conv_utf8_to_utf16 (path_link_utf16, path_link.c_str ());
   }
   if (ret_val == Err_OK)
   {
      ret_val = txt::conv_utf8_to_utf16 (path_real_utf16, path_real.c_str ());
   }
   if (ret_val == Err_OK)
   {
      ret_val = txt::conv_utf8_to_utf16 (work_dir_utf16, work_dir.c_str ());
   }
   if (ret_val == Err_OK)
   {
      ret_val = txt::conv_utf8_to_utf16 (arguments_utf16, arguments.c_str ());
   }

   ::HRESULT      hres;

   ::IShellLinkW *   shelli_ptr = 0;
   if (ret_val == Err_OK)
   {
      hres = ::CoCreateInstance (
         ::CLSID_ShellLink,
         0,
         ::CLSCTX_INPROC_SERVER,
         IID_IShellLinkW,
         reinterpret_cast <LPVOID *> (&shelli_ptr)
      );
      if (! SUCCEEDED (hres))
      {
         ohm_util_TRACE_ERROR1 (
            "create_shortcut(): CoCreateInstance() failed. Returned code %1%.",
            hres
         );
         ret_val = Err_CANNOT_CREATE_COM_OBJ;
      }
   }

   if (ret_val == Err_OK)
   {
      hres = shelli_ptr->SetPath (path_real_utf16.c_str ());
      if (hres != S_OK)
      {
         ohm_util_TRACE_ERROR1 (
            "create_shortcut(): SetPath() failed. Returned code %1%.",
            hres
         );
         ret_val = Err_CANNOT_SET_LINK_PARAM;
      }
   }
   if (ret_val == Err_OK && ! work_dir_utf16.empty ())
   {
      hres = shelli_ptr->SetWorkingDirectory (work_dir_utf16.c_str ());
      if (hres != S_OK)
      {
         ohm_util_TRACE_ERROR1 (
            "create_shortcut(): SetWorkingDirectory() failed. Returned code %1%.",
            hres
         );
         ret_val = Err_CANNOT_SET_LINK_PARAM;
      }
   }
   if (ret_val == Err_OK && ! arguments_utf16.empty ())
   {
      hres = shelli_ptr->SetArguments (arguments_utf16.c_str ());
      if (hres != S_OK)
      {
         ohm_util_TRACE_ERROR1 (
            "create_shortcut(): SetArguments() failed. Returned code %1%.",
            hres
         );
         ret_val = Err_CANNOT_SET_LINK_PARAM;
      }
   }

   ::IPersistFile *  pf_ptr = 0;
   if (ret_val == Err_OK)
   {
      hres = shelli_ptr->QueryInterface (
         IID_IPersistFile,
         reinterpret_cast <LPVOID *> (&pf_ptr)
      );
      if (hres != S_OK)
      {
         ohm_util_TRACE_ERROR1 (
            "create_shortcut(): QueryInterface() failed. Returned code %1%.",
            hres
         );
         ret_val = Err_CANNOT_SAVE_LINK;
      }
   }
   if (ret_val == Err_OK)
   {
      hres = pf_ptr->Save (path_link_utf16.c_str (), TRUE);
      if (hres != S_OK)
      {
         ohm_util_TRACE_ERROR1 (
            "create_shortcut(): Save() failed. Returned code %1%.",
            hres
         );
         ret_val = Err_CANNOT_SAVE_LINK;
      }
   }

   if (pf_ptr != 0)
   {
      pf_ptr->Release ();
      pf_ptr = 0;
   }

   if (shelli_ptr != 0)
   {
      shelli_ptr->Release ();
      shelli_ptr = 0;
   }

   return (ret_val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace win32
}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
