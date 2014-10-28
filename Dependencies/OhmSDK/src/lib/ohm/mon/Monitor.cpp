/*****************************************************************************

        Monitor.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70583

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

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define  WIN32_LEAN_AND_MEAN
   #define  NOMINMAX
   #define  NOGDI
   
   #include <windows.h>
#endif

#include "ohm/mon/Monitor.h"
#include "ohm/txt/out/PrintTubeEscaper.h"
#include "ohm/util/trace.h"

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   // for backtrace, backtrace_symbols
   // MacOS X.5 or later is assumed
   #include <execinfo.h>

   #include <cxxabi.h>
   #include <mach/mach.h>
   #include <mach/mach_time.h>
   #include <mach-o/stab.h>
   #include <mach/mach_vm.h>
#endif

#include <cassert>



namespace ohm
{
namespace mon
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : thread_self
==============================================================================
*/

unsigned int   Monitor::thread_self ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   const ::DWORD     thread_id = ::GetCurrentThreadId ();
   ohm_lang_CHECK_CST (ThreadIdSize, (sizeof (thread_id) <= sizeof (unsigned int)));

   return (static_cast <unsigned int> (thread_id));

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   return mach_thread_self ();
   
#endif
}



/*
==============================================================================
Name : counter
==============================================================================
*/

Monitor::CounterType Monitor::counter ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   ::LARGE_INTEGER   now;
   ::QueryPerformanceCounter (&now);
   ohm_lang_CHECK_CST (CounterSize, (sizeof (now.QuadPart) <= sizeof (CounterType)));

   return (now.QuadPart);

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   return mach_absolute_time ();
   
#endif
}



/*
==============================================================================
Name : to_time
==============================================================================
*/

double   Monitor::to_time (CounterType counter)
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   const double      t_s = double (counter) / double (_pc_freq);

   return (t_s);

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   ::Nanoseconds nanos = ::AbsoluteToNanoseconds (*(::AbsoluteTime *)&counter);

   return nanos.hi * 4294967295e-9 + nanos.lo * 1e-9;
#endif
}



/*
==============================================================================
Name : add_rule
==============================================================================
*/

void  Monitor::add_rule (const std::string & search, const std::string & replace)
{
   assert (!search.empty ());
   
   Rule rule;
   rule._search = search;
   rule._replace = replace;
   
   _rule_arr.push_back (rule);
}



/*
==============================================================================
Name : add_rule
==============================================================================
*/

void  Monitor::add_rule (const std::string & condition, const std::string & search, const std::string & replace)
{
   assert (!condition.empty ());
   assert (!search.empty ());
   
   Rule rule;
   rule._condition = condition;
   rule._search = search;
   rule._replace = replace;
   
   _rule_arr.push_back (rule);
}



/*
==============================================================================
Name : write_na
==============================================================================
*/

void  Monitor::write_na (unsigned int thread, CounterType timestamp, const char * pretty_func_0, const char * msg_0)
{
   Msg & msg = _msg_ring_buffer.push ();
   
   msg._thread = thread;
   msg._timestamp = timestamp;
   msg._pretty_func_0 = pretty_func_0;
   msg._msg_0 = msg_0;
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  Monitor::write (unsigned int thread, CounterType timestamp, const char * pretty_func_0, const char * output_0)
{
   Msg & msg = _msg_ring_buffer.push ();
   
   msg._thread = thread;
   msg._timestamp = timestamp;
   msg._pretty_func_0 = pretty_func_0;
   msg._msg_0 = 0;
   msg._output.assign (output_0);
}



/*
==============================================================================
Name : suspend_threads
==============================================================================
*/

void  Monitor::suspend_threads ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   // to do

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   thread_act_port_array_t thread_arr;
   mach_msg_type_number_t thread_cnt = 0;
   
   kern_return_t err = task_threads (
      mach_task_self (),
      &thread_arr,
      &thread_cnt
   );
   
   if (err == KERN_SUCCESS)
   {
      mach_port_t cur_thread = mach_thread_self ();
   
      for (mach_msg_type_number_t i = 0 ; i < thread_cnt ; ++i)
      {
         mach_port_t sub_thread = thread_arr [i];
         
         if (sub_thread != cur_thread)
         {
            // ignore error
            err = thread_suspend (sub_thread);
         }
      }
   }

#endif
}



/*
==============================================================================
Name : resume_threads
==============================================================================
*/

void  Monitor::resume_threads ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   // to do

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   thread_act_port_array_t thread_arr;
   mach_msg_type_number_t thread_cnt = 0;
   
   kern_return_t err = task_threads (
      mach_task_self (),
      &thread_arr,
      &thread_cnt
   );
   
   if (err == KERN_SUCCESS)
   {
      mach_port_t cur_thread = mach_thread_self ();
   
      for (mach_msg_type_number_t i = 0 ; i < thread_cnt ; ++i)
      {
         mach_port_t sub_thread = thread_arr [i];
         
         if (sub_thread != cur_thread)
         {
            // ignore error
            err = thread_resume (sub_thread);
         }
      }
   }

#endif
}



/*
==============================================================================
Name : trace_backtrace
==============================================================================
*/

void  Monitor::trace_backtrace ()
{
   ohm_util_TRACE_INFO ("Monitor:backtrace");
   
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   // to do

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   void * callstack [1024];
   int frames = backtrace (callstack, 1024);
   char ** strs = backtrace_symbols (callstack, frames);
   
   for (int i = 1 ; i < frames ; ++i)
   {
      trace_backtrace (strs [i]);
   }

   free (strs);

#endif
}



/*
==============================================================================
Name : trace_backtrace
==============================================================================
*/

void  Monitor::trace_backtrace (const char * str_0, bool prefix_flag, bool offset_flag)
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   // to do

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   std::string str (str_0);
   std::string prefix;
   std::string offset;
   
   prefix = str.substr (0, 51 - 0);
   str = str.substr (51);
   size_t pos = str.find_first_of (" \t\n\r");
   if (pos != std::string::npos)
   {
      offset = str.substr (pos);
      str = str.substr (0, pos - 0);
   }
   
   char * demangled_0 = 0;
   int status = -1;
   size_t length = 0;
   demangled_0 = __cxxabiv1::__cxa_demangle (
      str.c_str (), demangled_0, &length, &status
   );
   
   const char * func_name_0 = str.c_str ();
      
   if ((status == 0) && (demangled_0 != 0))
   {
      func_name_0 = demangled_0;
   }

   ohm_util_TRACE_INFO3 (
      "%1%%2%%3%",
      prefix_flag ? prefix.c_str () : "",
      func_name_0,
      offset_flag ? offset.c_str () : ""
   );
   
   if (length > 0)
   {
      free (demangled_0);
      demangled_0 = 0;
   }

#endif
}



/*
==============================================================================
Name : trace_dump
Note :
   We suppose here that all threads are stopped
==============================================================================
*/

void  Monitor::trace_dump ()
{
   ohm_util_TRACE_INFO ("Monitor:dump");

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   load_map_dgbhelp ();
#endif
   
   size_t pos = 0;
   size_t size = _msg_ring_buffer.size ();
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      if (pos < size)
      {
         while (
            (_msg_ring_buffer [pos]._pretty_func_0 == 0)
            && (pos < size)
            )
         {
            ++pos;
         }
      }
      
      if (pos < size)
      {
         const Msg & msg = _msg_ring_buffer [pos];
         ++pos;
         
         assert (msg._pretty_func_0 != 0);
         
         std::string func (msg._pretty_func_0);

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
         undecorate (func);
#endif

         process_rules (func);
         
         ohm_util_TRACE_INFO4 (
            "%1m8rw0X% %2m10p2rv% %3% %4%",
            msg._thread,
            to_time (msg._timestamp),
            func.c_str (),
            (msg._msg_0 != 0) ? msg._msg_0 : msg._output.c_str ()
         );
      }
      
      loop_flag = pos < size;
   }
}



/*
==============================================================================
Name : print_dump_xml
Note :
   We suppose here that all threads are stopped
==============================================================================
*/

void  Monitor::print_dump_xml (txt::out::PrintTubeEscaper & printer)
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   load_map_dgbhelp ();
#endif

   printer.cat ("  <monitor>\n");
   
   size_t pos = 0;
   size_t size = _msg_ring_buffer.size ();
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      if (pos < size)
      {
         while (
            (_msg_ring_buffer [pos]._pretty_func_0 == 0)
            && (pos < size)
            )
         {
            ++pos;
         }
      }
      
      if (pos < size)
      {
         printer.cat ("    <item>\n");
         
         const Msg & msg = _msg_ring_buffer [pos];
         ++pos;
         
         assert (msg._pretty_func_0 != 0);
         
         std::string func (msg._pretty_func_0);

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
         undecorate (func);
#endif

         process_rules (func);
         
         printer.cat ("      <mach_port>");
         printer.esc_cat ("%1%").arg (msg._thread);
         printer.cat ("</mach_port>\n");

         printer.cat ("      <timestamp>");
         printer.esc_cat ("%1m10p2rv%").arg (to_time (msg._timestamp));
         printer.cat ("</timestamp>\n");

         printer.cat ("      <function>");
         printer.esc_cat ("%1%").arg (func.c_str ());
         printer.cat ("</function>\n");

         printer.cat ("      <message>");
         printer.esc_cat ("%1%").arg ((msg._msg_0 != 0) ? msg._msg_0 : msg._output.c_str ());
         printer.cat ("</message>\n");
         
         printer.cat ("    </item>\n");
      }
      
      loop_flag = pos < size;
   }
   
   printer.cat ("  </monitor>\n");
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Monitor::Monitor ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   if (_pc_freq == 0)
   {
      ::LARGE_INTEGER   freq;
      ::QueryPerformanceFrequency (&freq);
      _pc_freq = freq.QuadPart;
   }
#endif
   
   _msg_ring_buffer.resize (500);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : undecorate
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
void  Monitor::undecorate (std::string & func)
{
   if (_fnc_UnDecorateSymbolName != 0)
   {
      char name_0 [1024];
      name_0 [0] = '\0';
      
      bool demangled_flag = true;
      
      ::DWORD ret = _fnc_UnDecorateSymbolName (
         func.c_str (),
         name_0,
         1024,
         UNDNAME_COMPLETE
      );
      
      if (ret == 0)
      {
         ret = _fnc_UnDecorateSymbolName (
            func.c_str (),
            name_0,
            1024,
            UNDNAME_NAME_ONLY
         );
      }
      
      if (ret != 0)
      {
         func = std::string (name_0);
      }
   }
}
#endif



/*
==============================================================================
Name : load_map_dgbhelp
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
void  Monitor::load_map_dgbhelp ()
{
   if (_fnc_UnDecorateSymbolName != 0)
   {
      // already loaded
      return;
   }
   
   // Builds the dbghelp.dll full pathname.
   // We want to use our own version (up to date and working) in priority,
   // we DO NOT want the one stored in the system path in this case.

   static const wchar_t dll_name_0 [] = L"dbghelp.dll";
   const int filename_size = 31999;
   wchar_t filename_0 [filename_size + 256 + 1];   // Additional space for "dbghelp.dll"

   ::DWORD ret = ::GetModuleFileNameW (0, filename_0, filename_size);
   filename_0 [filename_size] = L'\0';

   if (ret == 0)
   {
      return;  // abort
   }
   
   wchar_t *      bslash_0 = wcsrchr (filename_0, L'\\');

   if (bslash_0 == 0)
   {
      bslash_0 = wcsrchr (filename_0, L'/');
   }
   if (bslash_0 == 0)
   {
      bslash_0 = filename_0;
   }
   else
   {
      ++ bslash_0;   // Skips the backslash
   }
   
   wcscpy (bslash_0, dll_name_0);

   // Loads dbghelp.dll

   _dbghelp_hnd = ::LoadLibraryW (filename_0);
   
   if (_dbghelp_hnd == 0)
   {
      // Not found, try the system one.
      _dbghelp_hnd = ::LoadLibraryW (dll_name_0);
   }
   
   if (_dbghelp_hnd == 0)
   {
      return;  // abort
   }
   
   _fnc_UnDecorateSymbolName
      = (PtrFncUnDecorateSymbolName) ::GetProcAddress (_dbghelp_hnd, "UnDecorateSymbolName");
}
#endif



/*
==============================================================================
Name : process_rules
==============================================================================
*/

void  Monitor::process_rules (std::string & output) const
{
   for (size_t i = 0 ; i < _rule_arr.size () ; ++i)
   {
      const Rule & rule = _rule_arr [i];
      
      process_rule (output, rule);
   }
}



/*
==============================================================================
Name : process_rule
==============================================================================
*/

void  Monitor::process_rule (std::string & output, const Rule & rule) const
{
   if (!rule._condition.empty ())
   {
      if (output.find (rule._condition) == std::string::npos)
      {
         return;
      }
   }
   
   while (true)
   {
      size_t pos = output.find (rule._search);
      
      if (pos != std::string::npos)
      {
         output.replace (pos, rule._search.size (), rule._replace);
      }
      else
      {
         break;
      }
   }
}



#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
Monitor::PtrFncUnDecorateSymbolName Monitor::_fnc_UnDecorateSymbolName = 0;
archi::Int64   Monitor::_pc_freq = 0;
#endif



}  // namespace mon
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
