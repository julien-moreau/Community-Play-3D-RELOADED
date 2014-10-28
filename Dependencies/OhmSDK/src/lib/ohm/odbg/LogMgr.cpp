/*****************************************************************************

        LogMgr.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70452

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

#include "ohm/archi/def.h"
#include "ohm/odbg/LogMgr.h"
#include "ohm/pibase/Err.h"
#include "ohm/sys/ThreadMgr.h"
#include "ohm/txt/fnc.h"
#include "ohm/txt/Char16.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/LogMgr.h"

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include <syslog.h>
   #if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC) && (TARGET_API_MAC_CARBON)
//    include  "ohm/odbg/ProfilerClient.h"
   #endif

#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define  NOMINMAX
   #define  WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <Shlobj.h>

#endif

#include <vector>
#include <cassert>



namespace ohm
{
namespace odbg
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

LogMgr::Config::Config ()
:  _display_level_flag (true)
,  _display_scope_flag (true)
,  _display_func_flag (true)
,  _display_line_nbr_flag (true)
,  _display_ns_flag (true)
,  _use_thread_indentation_flag (false)
,  _thread_indentation (0)
{
}



LogMgr::~LogMgr ()
{
   // Nothing
}



void  LogMgr::write (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr)
{
   switch (_type)
   {
   case Type_NONE:
      // nothing
      break;

   case Type_UTIL:
      write_util (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_SYSLOG:
      write_syslog (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_PRINTF:
      write_printf (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   /*case Type_PROFILER:
      write_profiler (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_UTIL:
      write_analog_util (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_PRINTF:
      write_analog_printf (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_IPC:
      write_analog_ipc (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   case Type_WIN32_DEBUGGER:
      write_win32_debugger (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_PLUGIN:
      {
         assert (_to_host_fnc_ptr != 0);
         
         Debugger::CmdHost::WriteLogParam write_log_param;
         write_log_param._ns_0 = _namespace_0;
         write_log_param._level = level;
         write_log_param._msg_0 = msg_0;
         write_log_param._scope_0 = scope_0 + _scope_crop;
         write_log_param._func_0 = func_0;
         write_log_param._line_nbr = line_nbr;
         write_log_param._fast_flag = 0;
         
         _to_host_fnc_ptr (Debugger::CmdHost::WRITE_LOG, &write_log_param);
      }
      break;
      
   case Type_NBR_ELT:
   default:
      assert (false);
      break;
   }
}



void  LogMgr::write_cache (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr)
{
   switch (_type)
   {
   case Type_NONE:
      // nothing
      break;

   case Type_UTIL:
      write_util_cache (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_SYSLOG:
      write_syslog (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_PRINTF:
      write_printf (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   /*case Type_PROFILER:
      write_profiler (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_UTIL:
      write_analog_util (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_PRINTF:
      write_analog_printf (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_IPC:
      write_analog_ipc (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   case Type_WIN32_DEBUGGER:
      write_win32_debugger (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_PLUGIN:
      {
         assert (_to_host_fnc_ptr != 0);
         
         Debugger::CmdHost::WriteLogParam write_log_param;
         write_log_param._ns_0 = _namespace_0;
         write_log_param._level = level;
         write_log_param._msg_0 = msg_0;
         write_log_param._scope_0 = scope_0 + _scope_crop;
         write_log_param._func_0 = func_0;
         write_log_param._line_nbr = line_nbr;
         write_log_param._fast_flag = 1;
         
         _to_host_fnc_ptr (Debugger::CmdHost::WRITE_LOG, &write_log_param);
      }
      break;
      
   case Type_NBR_ELT:
   default:
      assert (false);
      break;
   }
}



void  LogMgr::write_dump_abort (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr)
{
   switch (_type)
   {
   case Type_NONE:
      // nothing
      break;

   case Type_UTIL:
      write_util (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_SYSLOG:
      write_syslog (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_PRINTF:
      write_printf (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   /*case Type_PROFILER:
      write_profiler (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_UTIL:
      write_analog_util (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_PRINTF:
      write_analog_printf (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   /*case Type_ANALOG_IPC:
      write_analog_ipc (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;*/

   case Type_WIN32_DEBUGGER:
      write_win32_debugger (level, msg_0, scope_0 + _scope_crop, func_0, line_nbr, _instance_aptr->_namespace_0);
      break;

   case Type_PLUGIN:
      {
         assert (_to_host_fnc_ptr != 0);
         
         Debugger::CmdHost::WriteLogParam write_log_param;
         write_log_param._ns_0 = _namespace_0;
         write_log_param._level = level;
         write_log_param._msg_0 = msg_0;
         write_log_param._scope_0 = scope_0 + _scope_crop;
         write_log_param._func_0 = func_0;
         write_log_param._line_nbr = line_nbr;
         write_log_param._fast_flag = 0;
         
         _to_host_fnc_ptr (Debugger::CmdHost::WRITE_LOG, &write_log_param);
      }
      break;
      
   case Type_NBR_ELT:
   default:
      assert (false);
      break;
   }
   
//#warning call the dumpers
   
   // let the IPC collect the msgs
   sys::ThreadMgr::sleep (1);
   
   abort ();
}



void  LogMgr::indent (int cnt)
{
   _indent += cnt;
}



void  LogMgr::set_scope_crop (const char * scope_pattern_0)
{
   if (scope_pattern_0 != 0)
   {
      create_empty_if_not_existing ();
      
      std::string file (__FILE__);
      std::string scope_pattern (scope_pattern_0);
      
      size_t pos = file.find (scope_pattern);
      
      if (pos != std::string::npos)
      {
         _instance_aptr->_scope_crop = pos + scope_pattern.size ();
      }
   }
}



void  LogMgr::create_as_util (Config config)
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_UTIL))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_UTIL;
         _instance_aptr->_config = config;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
      }
      catch (...)
      {
         assert (false);
      }
   }
}



#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
void  LogMgr::create_as_util_library_logs (const char * manufacturer_0, const char * log_name_0, Config config)
{
   create_as_util (config);
   
   FSRef fs_ref;
   
   OSStatus err = 0;
   
   if (err == 0)
   {
      err = ::FSFindFolder (
         kUserDomain,
         kLogsFolderType,
         false,
         &fs_ref
      );
      
      if (err != 0)
      {
         printf ("FSFindFolder kUserDomain/kLogsFolderType failed with error %ld", err);
      }
   }
   
   char log_path_0 [4096 + 1];
   
   if (err == 0)
   {
      err = ::FSRefMakePath (
         &fs_ref,
         reinterpret_cast < ::UInt8 *> (log_path_0),
         4096
      );
      
      if (err != 0)
      {
         printf ("FSRefMakePath failed with error %ld", err);
      }
   }
   
   std::string log_path (log_path_0);
   
   txt::out::Print printer;

   printer.print ("mkdir \"%1%/%2%\"").arg (log_path_0).arg (manufacturer_0);
   system (std::string (printer).c_str ());

   printer.print ("%1%/%2%/%3%").arg (log_path_0).arg (manufacturer_0).arg (log_name_0);
   
   const std::string log_file = printer;
      
   util::LogMgr::use_instance ().open_file (log_file.c_str ());
}
#endif



#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
/*
==============================================================================
Name: create_as_util_app_data_logs
==============================================================================
*/

void LogMgr::create_as_util_app_data_logs (const char * manufacturer_0, const char * application_0, const char * log_name_0, Config config)
{
   txt::Char16 path_utf16_0 [MAX_PATH + 1024 + 1];
   
   const size_t buf_len_path_utf16 
      = sizeof (path_utf16_0) / sizeof (path_utf16_0 [0]);

   {
      ::BOOL ok_flag = ::SHGetSpecialFolderPathW (0, path_utf16_0, CSIDL_APPDATA, 0);
      assert (ok_flag == TRUE);
      
      txt::Char16 manufacturer_16_0 [256];

      {
         int len = ::MultiByteToWideChar (
            CP_UTF8,
            0,
            manufacturer_0,
            int (txt::get_strlen (manufacturer_0)),
            manufacturer_16_0,
            sizeof (manufacturer_16_0) / sizeof (manufacturer_0 [0])
         );
         manufacturer_16_0 [len] = '\0';
      }

      txt::Char16 application_16_0 [256];

      {
         int len = ::MultiByteToWideChar (
            CP_UTF8,
            0,
            application_0,
            int (txt::get_strlen (application_0)),
            application_16_0,
            sizeof (application_16_0) / sizeof (application_16_0 [0])
         );
         application_16_0 [len] = '\0';
      }

      txt::strncat_0 (path_utf16_0, L"\\", buf_len_path_utf16);
      txt::strncat_0 (path_utf16_0, manufacturer_16_0, buf_len_path_utf16);
      ok_flag = ::CreateDirectoryW (path_utf16_0, 0);
      assert ((ok_flag == TRUE) || (::GetLastError () == ERROR_ALREADY_EXISTS));
      
      txt::strncat_0 (path_utf16_0, L"\\", buf_len_path_utf16);
      txt::strncat_0 (path_utf16_0, application_16_0, buf_len_path_utf16);
      ok_flag = ::CreateDirectoryW (path_utf16_0, 0);
      assert ((ok_flag == TRUE) || (::GetLastError () == ERROR_ALREADY_EXISTS));
   }
      
   const long len_utf16 = long (txt::get_strlen (path_utf16_0));
   char log_path_0 [MAX_PATH + 1024 + 1];
   
   const int len = ::WideCharToMultiByte (
      CP_UTF8,
      0,
      path_utf16_0,
      len_utf16,
      log_path_0,
      int (sizeof (log_path_0)),
      0,
      0
   );
   log_path_0 [len] = '\0';

   
   txt::strncat_0 (log_path_0, "\\", sizeof (log_path_0));
   txt::strncat_0 (log_path_0, log_name_0, sizeof (log_path_0));
   
   ::printf ("opening log %s", log_path_0);
   util::LogMgr::use_instance ().open_file (log_path_0);
}



#endif



void  LogMgr::create_as_syslog (const char * namespace_0)
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_SYSLOG))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_SYSLOG;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
         
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
         openlog (namespace_0, LOG_PID | LOG_NDELAY, LOG_USER);
         setlogmask(LOG_UPTO(LOG_DEBUG));
#endif
      }
      catch (...)
      {
         assert (false);
      }
   }
}



void  LogMgr::create_as_printf (Config config)
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_PRINTF))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_PRINTF;
         _instance_aptr->_config = config;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
      }
      catch (...)
      {
         assert (false);
      }
   }
}



/*void   LogMgr::create_as_profiler ()
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_PROFILER))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_PROFILER;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
      }
      catch (...)
      {
         assert (false);
      }
   }
}*/



/*void   LogMgr::create_as_analog_util ()
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_ANALOG_UTIL))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_ANALOG_UTIL;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
      }
      catch (...)
      {
         assert (false);
      }
   }
}*/



/*void   LogMgr::create_as_analog_printf ()
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_ANALOG_PRINTF))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_ANALOG_PRINTF;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
      }
      catch (...)
      {
         assert (false);
      }
   }
}*/



/*void   LogMgr::create_as_analog_ipc (const char * src_name_0)
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_ANALOG_IPC))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_ANALOG_IPC;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
         txt::strncpy_0 (_instance_aptr->_src_name_0, src_name_0, 256);
         
         try
         {
            _instance_aptr->_remote_msg_port_ptr = new ohm::sys::RemoteMessagePort ("com.ohmforce.analog");
         }
         catch (...)
         {
            printf ("RemoveMessagePort com.ohmforce.analog unavailable - logging deactivated\n");
            _instance_aptr->_remote_msg_port_ptr = 0;
         }
      }
      catch (...)
      {
         assert (false);
      }
   }
}*/



void  LogMgr::create_as_win32_debugger (Config config)
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_WIN32_DEBUGGER))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_WIN32_DEBUGGER;
         _instance_aptr->_config = config;
         txt::strncpy_0 (_instance_aptr->_namespace_0, "main", 256);
      }
      catch (...)
      {
         assert (false);
      }
   }
}



void  LogMgr::create_as_plugin (const char * namespace_0, Debugger::FncPtr fnc_ptr)
{
   if ((_instance_aptr.get () == 0) || (_instance_aptr->_type != Type_PLUGIN))
   {
      try
      {
         create_empty_if_not_existing ();
         
         _instance_aptr->_type = Type_PLUGIN;
         txt::strncpy_0 (_instance_aptr->_namespace_0, namespace_0, 256);
         
         _instance_aptr->_to_host_fnc_ptr = fnc_ptr;
      }
      catch (...)
      {
         assert (false);
      }
   }
}



LogMgr & LogMgr::use_instance ()
{
   if (_instance_aptr.get () == 0)
   {
      create_as_util ();
   }

   return (*_instance_aptr);
}



archi::Int32   LogMgr::execute_command (archi::Int32 command, void * data_ptr)
{
   int            ret_val = pibase::Err_NOT_IMPLEMENTED;

   switch (command)
   {
   case Debugger::CmdHost::WRITE_LOG:
      {
         Debugger::CmdHost::WriteLogParam * write_log_param_ptr =
            reinterpret_cast <Debugger::CmdHost::WriteLogParam *> (data_ptr);

         LogMgr &       mgr = use_instance ();

         switch (mgr._type)
         {
         case Type_NONE:
            // Nothing
            ret_val = pibase::Err_OK;
            break;

         case Type_UTIL:
            if (write_log_param_ptr->_fast_flag != 0)
            {
               mgr.write_util_cache (
                  static_cast <Debugger::Level> (write_log_param_ptr->_level),
                  write_log_param_ptr->_msg_0,
                  write_log_param_ptr->_scope_0,
                  write_log_param_ptr->_func_0,
                  write_log_param_ptr->_line_nbr,
                  write_log_param_ptr->_ns_0
               );
            }
            else
            {
               mgr.write_util (
                  static_cast <Debugger::Level> (write_log_param_ptr->_level),
                  write_log_param_ptr->_msg_0,
                  write_log_param_ptr->_scope_0,
                  write_log_param_ptr->_func_0,
                  write_log_param_ptr->_line_nbr,
                  write_log_param_ptr->_ns_0
               );
            }
            ret_val = pibase::Err_OK;
            break;

         case Type_SYSLOG:
            mgr.write_syslog (
               static_cast <Debugger::Level> (write_log_param_ptr->_level),
               write_log_param_ptr->_msg_0,
               write_log_param_ptr->_scope_0,
               write_log_param_ptr->_func_0,
               write_log_param_ptr->_line_nbr,
               write_log_param_ptr->_ns_0
            );
            ret_val = pibase::Err_OK;
            break;

         case Type_PRINTF:
            mgr.write_printf (
               static_cast <Debugger::Level> (write_log_param_ptr->_level),
               write_log_param_ptr->_msg_0,
               write_log_param_ptr->_scope_0,
               write_log_param_ptr->_func_0,
               write_log_param_ptr->_line_nbr,
               write_log_param_ptr->_ns_0
            );
            ret_val = pibase::Err_OK;
            break;

         /*case Type_PROFILER:
            mgr.write_profiler (
               static_cast <Debugger::Level> (write_log_param_ptr->_level),
               write_log_param_ptr->_msg_0,
               write_log_param_ptr->_scope_0,
               write_log_param_ptr->_func_0,
               write_log_param_ptr->_line_nbr,
               write_log_param_ptr->_ns_0
            );
            ret_val = pibase::Err_OK;
            break;*/

         /*case Type_ANALOG_UTIL:
            mgr.write_analog_util (
               static_cast <Debugger::Level> (write_log_param_ptr->_level),
               write_log_param_ptr->_msg_0,
               write_log_param_ptr->_scope_0,
               write_log_param_ptr->_func_0,
               write_log_param_ptr->_line_nbr,
               write_log_param_ptr->_ns_0
            );
            ret_val = pibase::Err_OK;
            break;*/

         /*case Type_ANALOG_PRINTF:
            mgr.write_analog_printf (
               static_cast <Debugger::Level> (write_log_param_ptr->_level),
               write_log_param_ptr->_msg_0,
               write_log_param_ptr->_scope_0,
               write_log_param_ptr->_func_0,
               write_log_param_ptr->_line_nbr,
               write_log_param_ptr->_ns_0
            );
            ret_val = pibase::Err_OK;
            break;*/

         /*case Type_ANALOG_IPC:
            mgr.write_analog_ipc (
               static_cast <Debugger::Level> (write_log_param_ptr->_level),
               write_log_param_ptr->_msg_0,
               write_log_param_ptr->_scope_0,
               write_log_param_ptr->_func_0,
               write_log_param_ptr->_line_nbr,
               write_log_param_ptr->_ns_0
            );
            ret_val = pibase::Err_OK;
            break;*/

         case Type_WIN32_DEBUGGER:
            mgr.write_win32_debugger (
               static_cast <Debugger::Level> (write_log_param_ptr->_level),
               write_log_param_ptr->_msg_0,
               write_log_param_ptr->_scope_0,
               write_log_param_ptr->_func_0,
               write_log_param_ptr->_line_nbr,
               write_log_param_ptr->_ns_0
            );
            ret_val = pibase::Err_OK;
            break;

         case  Type_PLUGIN:
            assert (mgr._to_host_fnc_ptr != 0);
            ret_val = (mgr._to_host_fnc_ptr) (command, data_ptr);
            break;

         default:
            assert (false);
            ret_val = pibase::Err_GENERIC_FAILURE;
            break;
         }
      }
      
      break;
   
   default:
      assert (false);
      break;
   }
   
   return (ret_val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



LogMgr::LogMgr ()
:  _type (Type_NONE)
,  _indent (0)
,  _scope_crop (0)
,  _to_host_fnc_ptr (0)
/*,   _remote_msg_port_ptr (0)*/
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
,  _nbr_thread (0)
#endif
{
   // Nothing
}



void  LogMgr::write_util (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   const int max_len = 1024;
   char output_0 [max_len + 1];
   txt::out::PrintNa txt (output_0, max_len);
   
   write_str (txt, false, level, msg_0, scope_0, func_0, line_nbr, ns_0);
   
   util::LogMgr::use_instance ().write (output_0);
}



void  LogMgr::write_util_cache (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   const int max_len = 1024;
   char output_0 [max_len + 1];
   txt::out::PrintNa txt (output_0, max_len);
   
   write_str (txt, false, level, msg_0, scope_0, func_0, line_nbr, ns_0);
   
   util::LogMgr::use_instance ().write_cache (output_0);
}



void  LogMgr::write_syslog (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   switch (level)
   {
   case Debugger::Level_PANIC:
      syslog (LOG_EMERG, "%s %s %s %s %ld", ns_0, msg_0, scope_0, func_0, line_nbr);
      break;

   case Debugger::Level_ERROR:
      syslog (LOG_ERR, "%s %s %s %s %ld", ns_0, msg_0, scope_0, func_0, line_nbr);
      break;

   case Debugger::Level_WARNING:
      syslog (LOG_WARNING, "%s %s %s %s %ld", ns_0, msg_0, scope_0, func_0, line_nbr);
      break;

   case Debugger::Level_INFO:
      syslog (LOG_INFO, "%s %s %s %s %ld", ns_0, msg_0, scope_0, func_0, line_nbr);
      break;

   case Debugger::Level_DEBUG:
      syslog (LOG_DEBUG, "%s %s %s %s %ld", ns_0, msg_0, scope_0, func_0, line_nbr);
      break;
   
   case Debugger::Level_ASSERT:
      syslog (LOG_DEBUG, "%s %s %s %s %ld", ns_0, msg_0, scope_0, func_0, line_nbr);
      break;
   
   default:
      assert (false);
      break;
   }
#endif
}



void  LogMgr::write_printf (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   const int max_len = 1024;
   char output_0 [max_len + 1];
   txt::out::PrintNa txt (output_0, max_len);
   
   write_str (txt, true, level, msg_0, scope_0, func_0, line_nbr, ns_0);
   printf ("%s", output_0);
}



/*void   LogMgr::write_profiler (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{

#if    (ohm_archi_SYS == ohm_archi_SYS_MACOS)   \
    && (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC) \
    && (TARGET_API_MAC_CARBON)

   ProfilerClient::use_instance ().write_cache_expand (
      level,
      msg_0,
      scope_0,
      func_0,
      line_nbr,
      ns_0,
      ""
   );

#endif

}*/



/*void   LogMgr::write_analog_util (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   txt::out::Print   txt;
   
   write_analog_str (txt, level, msg_0, scope_0, func_0, line_nbr, ns_0);
   
   const std::string & txt_str = txt;
   
   util::LogMgr::use_instance ().write (txt_str.c_str ());
}*/



/*void   LogMgr::write_analog_printf (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   txt::out::Print   txt;
   
   write_analog_str (txt, level, msg_0, scope_0, func_0, line_nbr, ns_0);
   
   const std::string & txt_str = txt;
   
   printf ("%s\n", txt_str.c_str ());
}*/



/*void   LogMgr::write_analog_ipc (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   if (_remote_msg_port_ptr != 0)
   {
      size_t         i;
      txt::out::Print   txt;
      
      write_analog_str (txt, level, msg_0, scope_0, func_0, line_nbr, ns_0);
      
      const std::string & txt_str = txt;
      std::string::size_type  txt_str_size = txt_str.size ();
      
      size_t         src_name_length = strlen (_src_name_0);
      std::vector <archi::UByte> data;
      data.resize (4 + src_name_length + 4 + txt_str_size);
      
      size_t         pos = 0;
      data [pos + 0] = archi::UByte ((src_name_length >> 24) & 255);
      data [pos + 1] = archi::UByte ((src_name_length >> 16) & 255);
      data [pos + 2] = archi::UByte ((src_name_length >>  8) & 255);
      data [pos + 3] = archi::UByte ((src_name_length      ) & 255);
      
      pos += 4;
      
      for (i = 0 ; i < src_name_length; ++i)
      {
         data [pos] = _src_name_0 [i];
         ++pos;
      }
      
      data [pos + 0] = archi::UByte ((txt_str_size >> 24) & 255);
      data [pos + 1] = archi::UByte ((txt_str_size >> 16) & 255);
      data [pos + 2] = archi::UByte ((txt_str_size >>  8) & 255);
      data [pos + 3] = archi::UByte ((txt_str_size      ) & 255);
      
      pos += 4;
      
      for (i = 0 ; i < txt_str_size; ++i)
      {
         data [pos] = txt_str [i];
         ++pos;
      }
      
      assert (_remote_msg_port_ptr != 0);
      assert (data.size () <= LONG_MAX);
      _remote_msg_port_ptr->send (&data [0], long (data.size ()));
   }
}*/



/*void   LogMgr::write_analog_str (txt::out::PrintInterface & txt, Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   const char *      level_0 = "OOPS";
   switch (level)
   {
   case Debugger::Level_PANIC:
      level_0 = "PANIC";
      break;

   case Debugger::Level_ERROR:
      level_0 = "ERROR";
      break;

   case Debugger::Level_WARNING:
      level_0 = "WARNING";
      break;

   case Debugger::Level_INFO:
      level_0 = "INFO";
      break;

   case Debugger::Level_DEBUG:
      level_0 = "DEBUG";
      break;

   case Debugger::Level_ASSERT:
      level_0 = "ASSERT";
      break;

   default:
      assert (false);
      break;
   }
   
   std::string ns (ns_0);
   escape_slash (ns);

   std::string msg (msg_0);
   escape_slash (msg);

   std::string scope (scope_0);
   escape_slash (scope);

   txt.print ("%1%/;%2%/;%3%/;%4%/;%5%/;%6%")
      .arg (ns.c_str ())
      .arg (level_0)
      .arg (msg.c_str ())
      .arg (scope.c_str ())
      .arg (func_0)
      .arg (line_nbr);
}*/



void  LogMgr::escape_slash (std::string & txt)
{
   for (int i = 0 ; i < int (txt.size ()) ; ++i)
   {
      char c = txt [i];
      
      if (c == '/')
      {
         txt.insert (i, "/");
         ++i;
      }
   }
}



void  LogMgr::write_win32_debugger (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

   const int max_len = 1024;
   char output_0 [max_len + 1];
   txt::out::PrintNa txt (output_0, max_len);
   
   write_str (txt, true, level, msg_0, scope_0, func_0, line_nbr, ns_0);
   ::OutputDebugString (output_0);

#endif
}



void  LogMgr::write_str (txt::out::PrintInterface & txt, bool trailing_cr_flag, Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0)
{
   const char *      level_0 = "OOPS";
   switch (level)
   {
   case Debugger::Level_VOID:
      level_0 = "";
      break;

   case Debugger::Level_PANIC:
      level_0 = "PANIC";
      break;

   case Debugger::Level_ERROR:
      level_0 = "ERROR";
      break;

   case Debugger::Level_WARNING:
      level_0 = "WARNING";
      break;

   case Debugger::Level_INFO:
      level_0 = "INFO";
      break;

   case Debugger::Level_DEBUG:
      level_0 = "DEBUG";
      break;

   case Debugger::Level_ASSERT:
      level_0 = "ASSERT";
      break;

   default:
      assert (false);
      break;
   }
   
   txt.print ("");
   
#if    (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   if (_config._use_thread_indentation_flag)
   {
      pthread_t thread_id = pthread_self ();
      
      _mutex.lock ();
      
      std::map <pthread_t, int>::iterator it = _thread_indent_map.find (thread_id);
      
      if (it == _thread_indent_map.end ())
      {
         _thread_indent_map [thread_id] = _nbr_thread;
         ++_nbr_thread;
      }
      
      int indent = _thread_indent_map [thread_id];
      
      _mutex.unlock ();
      
      for (int j = 0 ; j < indent ; ++j)
      {
         for (int i = 0 ; i < _config._thread_indentation ; ++i)
         {
            txt.cat (" ");
         }
      }
   }
#endif

   for (int i = 0 ; i < _indent ; ++i)
   {
      txt.cat (" ");
   }

   if (_config._display_ns_flag)
   {
      txt.cat ("%1%\t").arg (ns_0);
   }

   if (_config._display_level_flag)
   {
      txt.cat ("%1%\t").arg (level_0);
   }
   
   txt.cat ("%1%\t").arg (msg_0);

   if (_config._display_scope_flag)
   {
      txt.cat ("%1%\t").arg (scope_0);
   }

   if (_config._display_func_flag)
   {
      txt.cat ("%1%\t").arg (func_0);
   }

   if (_config._display_line_nbr_flag)
   {
      txt.cat ("%1%\t").arg (line_nbr);
   }
   
   if (trailing_cr_flag)
   {
      txt.cat ("\n");
   }
}



void  LogMgr::create_empty_if_not_existing ()
{
   if (_instance_aptr.get () == 0)
   {
      _instance_aptr = std::auto_ptr <LogMgr> (new LogMgr);
   }
}



std::auto_ptr <LogMgr>  LogMgr::_instance_aptr (0);



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace odbg
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
