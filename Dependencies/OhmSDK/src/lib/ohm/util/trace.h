/*****************************************************************************

        trace.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 60553

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



#if ! defined (ohm_util_trace_HEADER_INCLUDED)
#define  ohm_util_trace_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if defined (ohm_util_TRACE_USE_UNIX_SYSLOG_NO_DEPENDENCIES)
   #include <syslog.h>

#else // defined (ohm_util_TRACE_USE_UNIX_SYSLOG_NO_DEPENDENCIES)
   #include "ohm/odbg/LogMgr.h"

#endif // defined (ohm_util_TRACE_USE_UNIX_SYSLOG_NO_DEPENDENCIES)

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
      #if (ohm_archi_COMPILER >= ohm_archi_COMPILER_VC2005)
         // for __debugbreak
         #include <intrin.h>
      #endif
   #endif

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)

   #include <AvailabilityMacros.h>
   
   #if (MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4)
      // for backtrace, backtrace_symbols
      #include <execinfo.h>
   #endif
   
#endif // ohm_archi_SYS

#include "ohm/odbg/Debugger.h"
#include "ohm/util/LogLine.h"
#include "ohm/txt/out/PrintNa.h"

#include <cassert>



// A ";" is needed after these macros

#if defined (ohm_util_TRACE_USE_UNIX_SYSLOG_NO_DEPENDENCIES)

   #define  ohm_util_TRACE_WITH_CTX( lvl, msg_0)   \
      do {  \
         switch (lvl)   \
         {  \
         case ohm::odbg::Debugger::Level_PANIC: \
            syslog (LOG_EMERG, "%s", msg_0); \
            break;   \
   \
         case ohm::odbg::Debugger::Level_ERROR: \
            syslog (LOG_ERR, "%s", msg_0);   \
            break;   \
   \
         case ohm::odbg::Debugger::Level_WARNING:  \
            syslog (LOG_WARNING, "%s", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_INFO:  \
            syslog (LOG_INFO, "%s", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_DEBUG: \
            syslog (LOG_DEBUG, "%s", msg_0); \
            break;   \
         }  \
      } while (false)

   #define  ohm_util_TRACE_FAST_WITH_CTX( lvl, msg_0) \
      do {  \
         switch (lvl)   \
         {  \
         case ohm::odbg::Debugger::Level_PANIC: \
            syslog (LOG_EMERG, "%s", msg_0); \
            break;   \
   \
         case ohm::odbg::Debugger::Level_ERROR: \
            syslog (LOG_ERR, "%s", msg_0);   \
            break;   \
   \
         case ohm::odbg::Debugger::Level_WARNING:  \
            syslog (LOG_WARNING, "%s", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_INFO:  \
            syslog (LOG_INFO, "%s", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_DEBUG: \
            syslog (LOG_DEBUG, "%s", msg_0); \
            break;   \
         }  \
      } while (false)

#elif defined (ohm_util_TRACE_USE_UNIX_PRINTF_NO_DEPENDENCIES)

   #define  ohm_util_TRACE_WITH_CTX( lvl, msg_0)   \
      do {  \
         switch (lvl)   \
         {  \
         case ohm::odbg::Debugger::Level_PANIC: \
            printf ("EMERG %s\n", msg_0); \
            break;   \
   \
         case ohm::odbg::Debugger::Level_ERROR: \
            printf ("ERROR %s\n", msg_0); \
            break;   \
   \
         case ohm::odbg::Debugger::Level_WARNING:  \
            printf ("WARNING %s\n", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_INFO:  \
            printf ("INFO %s\n", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_DEBUG: \
            printf ("DEBUG %s\n", msg_0); \
            break;   \
         }  \
      } while (false)

   #define  ohm_util_TRACE_FAST_WITH_CTX( lvl, msg_0) \
      do {  \
         switch (lvl)   \
         {  \
         case ohm::odbg::Debugger::Level_PANIC: \
            printf ("EMERG %s\n", msg_0); \
            break;   \
   \
         case ohm::odbg::Debugger::Level_ERROR: \
            printf ("ERROR %s\n", msg_0); \
            break;   \
   \
         case ohm::odbg::Debugger::Level_WARNING:  \
            printf ("WARNING %s\n", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_INFO:  \
            printf ("INFO %s\n", msg_0);  \
            break;   \
   \
         case ohm::odbg::Debugger::Level_DEBUG: \
            printf ("DEBUG %s\n", msg_0); \
            break;   \
         }  \
      } while (false)

#elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)

   #define  ohm_util_TRACE_WITH_CTX( lvl, msg_0)   \
      do {  \
         ohm::odbg::LogMgr::use_instance ().write (   \
            lvl,     \
            msg_0,   \
            __FILE__ ", " __DATE__, \
            __PRETTY_FUNCTION__, \
            __LINE__ \
         ); \
      } while (false)

   #define  ohm_util_TRACE_FAST_WITH_CTX( lvl, msg_0) \
      do {  \
         ohm::odbg::LogMgr::use_instance ().write_cache (   \
            lvl,     \
            msg_0,   \
            __FILE__ ", " __DATE__, \
            __PRETTY_FUNCTION__, \
            __LINE__ \
         ); \
      } while (false)

#else // ohm_archi_COMPILER_CLASS

   #define  ohm_util_TRACE_WITH_CTX( lvl, msg_0)   \
      do {  \
         ohm::odbg::LogMgr::use_instance ().write (   \
            lvl,     \
            msg_0,   \
            __FILE__ ", " __DATE__, \
            "",   \
            __LINE__ \
         ); \
      } while (false)

   #define  ohm_util_TRACE_FAST_WITH_CTX( lvl, msg_0) \
      do {  \
         ohm::odbg::LogMgr::use_instance ().write_cache (   \
            lvl,     \
            msg_0,   \
            __FILE__ ", " __DATE__, \
            "",   \
            __LINE__ \
         ); \
      } while (false)

#endif   // ohm_archi_COMPILER_CLASS


#define  ohm_util_TRACE_MACRO( trace_method, lvl, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8)  \
   do {  \
      const long     max_len______ = ohm::util::LogLine::MAX_STR_SIZE;  \
      char           output_0______ [max_len______ + 1]; \
      ohm::txt::out::PrintNa  txt______ (output_0______, max_len______);   \
      txt______.print (fmt_0).arg (arg_1).arg (arg_2).arg (arg_3).arg (arg_4).arg (arg_5).arg (arg_6).arg (arg_7).arg (arg_8);   \
      trace_method (lvl, output_0______); \
   } while (false)



#define  ohm_util_DO_NOTHING  do { } while (false)


#define ohm_util_TRACE_INDENT(cnt)  \
   odbg::LogMgr::use_instance ().indent (cnt)



// ohm_util_TRACE_DEBUGGER

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #define ohm_util_TRACE_DEBUGGER  \
      Debugger

#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
      #if (ohm_archi_COMPILER >= ohm_archi_COMPILER_VC2005)
         #define ohm_util_TRACE_DEBUGGER  \
            __debugbreak
      #else
         #define ohm_util_TRACE_DEBUGGER  \
            do {              \
               __asm int 3    \
            } while (false);

      #endif

   #else
      #error To do for this compiler

   #endif

#elif (ohm_archi_SYS == ohm_archi_SYS_LINUX)
   #define ohm_util_TRACE_DEBUGGER  \
      do {} while (true);

#else // ohm_archi_SYS
   #error Unsupported Architecture

#endif // ohm_archi_SYS



// ohm_util_TRACE_BACKTRACE

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)

   #if (MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_4)
   
      #define ohm_util_TRACE_BACKTRACE                                  \
            do {                                                        \
               void * callstack [1024];                                 \
               int frames = backtrace (callstack, 1024);                \
               char ** strs = backtrace_symbols (callstack, frames);    \
               for (int i = 0 ; i < frames ; ++i)                       \
               {                                                        \
                  ohm_util_TRACE_MACRO (                                \
                     ohm_util_TRACE_WITH_CTX,                           \
                     ohm::odbg::Debugger::Level_ERROR,                  \
                     strs [i],                                          \
                     0, 0, 0, 0, 0, 0, 0, 0                             \
                  );                                                    \
               }                                                        \
               free (strs);                                             \
            } while (false);
   #else
   
      #define ohm_util_TRACE_BACKTRACE \
         (void (0))
         
   #endif

   
#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define ohm_util_TRACE_BACKTRACE \
      (void (0))

#elif (ohm_archi_SYS == ohm_archi_SYS_LINUX)
   #define ohm_util_TRACE_BACKTRACE \
      (void (0))

#else // ohm_archi_SYS
   #error Unsupported Architecture

#endif // ohm_archi_SYS




// These macro require definition of ohm::txt::out::PrintNa

#define  ohm_util_TRACE_PANIC( msg_0)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC1( fmt_0, arg_1)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC2( fmt_0, arg_1, arg_2)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC3( fmt_0, arg_1, arg_2, arg_3) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC4( fmt_0, arg_1, arg_2, arg_3, arg_4)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC5( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

#define  ohm_util_TRACE_PANIC_FAST( msg_0)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC1_FAST( fmt_0, arg_1) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC2_FAST( fmt_0, arg_1, arg_2)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC3_FAST( fmt_0, arg_1, arg_2, arg_3)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC4_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
#define  ohm_util_TRACE_PANIC5_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_PANIC, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)



#define  ohm_util_TRACE_ERROR( msg_0)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR1( fmt_0, arg_1)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR2( fmt_0, arg_1, arg_2)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR3( fmt_0, arg_1, arg_2, arg_3) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR4( fmt_0, arg_1, arg_2, arg_3, arg_4)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR5( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

#define  ohm_util_TRACE_ERROR_FAST( msg_0)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR1_FAST( fmt_0, arg_1) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR2_FAST( fmt_0, arg_1, arg_2)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR3_FAST( fmt_0, arg_1, arg_2, arg_3)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR4_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
#define  ohm_util_TRACE_ERROR5_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_ERROR, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)



#define  ohm_util_TRACE_WARNING( msg_0)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING1( fmt_0, arg_1) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING2( fmt_0, arg_1, arg_2)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING3( fmt_0, arg_1, arg_2, arg_3)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING4( fmt_0, arg_1, arg_2, arg_3, arg_4) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING5( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

#define  ohm_util_TRACE_WARNING_FAST( msg_0) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING1_FAST( fmt_0, arg_1)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING2_FAST( fmt_0, arg_1, arg_2) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING3_FAST( fmt_0, arg_1, arg_2, arg_3)   \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING4_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4)  \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
#define  ohm_util_TRACE_WARNING5_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5) \
   ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_WARNING, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)


#if defined (ohm_util_TRACE_NOINFO_FLAG)

   #define  ohm_util_TRACE_INFO( msg_0)   \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO1( fmt_0, arg_1) \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO2( fmt_0, arg_1, arg_2)   \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO3( fmt_0, arg_1, arg_2, arg_3)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO4( fmt_0, arg_1, arg_2, arg_3, arg4)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO5( fmt_0, arg_1, arg_2, arg_3, arg4, arg5)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO6( fmt_0, arg_1, arg_2, arg_3, arg4, arg5)  \
      ohm_util_DO_NOTHING

   #define  ohm_util_TRACE_INFO_FAST( msg_0) \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO1_FAST( fmt_0, arg_1)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO2_FAST( fmt_0, arg_1, arg_2) \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO3_FAST( fmt_0, arg_1, arg_2, arg_3)   \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO4_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_INFO5_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5) \
      ohm_util_DO_NOTHING

#else // defined (ohm_util_TRACE_NOINFO_FLAG)

   #define  ohm_util_TRACE_INFO( msg_0)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_INFO, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO1( fmt_0, arg_1) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO2( fmt_0, arg_1, arg_2)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO3( fmt_0, arg_1, arg_2, arg_3)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO4( fmt_0, arg_1, arg_2, arg_3, arg_4) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO5( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)
   #define  ohm_util_TRACE_INFO6( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, 0, 0)

   #define  ohm_util_TRACE_INFO_FAST( msg_0) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_INFO, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO1_FAST( fmt_0, arg_1)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO2_FAST( fmt_0, arg_1, arg_2) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO3_FAST( fmt_0, arg_1, arg_2, arg_3)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO4_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
   #define  ohm_util_TRACE_INFO5_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_INFO, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

#endif   // defined (ohm_util_TRACE_INFO_FLAG)



#if defined (ohm_util_TRACE_DEBUG_FLAG)

   #define  ohm_util_TRACE_DEBUG( msg_0)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG1( fmt_0, arg_1)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG2( fmt_0, arg_1, arg_2)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG3( fmt_0, arg_1, arg_2, arg_3) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG4( fmt_0, arg_1, arg_2, arg_3, arg_4)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG5( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

   #define  ohm_util_TRACE_DEBUG_FAST( msg_0)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, msg_0, 0, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG1_FAST( fmt_0, arg_1) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, 0, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG2_FAST( fmt_0, arg_1, arg_2)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG3_FAST( fmt_0, arg_1, arg_2, arg_3)  \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG4_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4) \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)
   #define  ohm_util_TRACE_DEBUG5_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)   \
      ohm_util_TRACE_MACRO (ohm_util_TRACE_FAST_WITH_CTX, ohm::odbg::Debugger::Level_DEBUG, fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

   #define  ohm_util_TRACE_ASSERT( cond, msg_0) \
      do { if (! (cond)) { ohm_util_TRACE_ERROR (msg_0); } assert (cond); } while (false)
   #define  ohm_util_TRACE_ASSERT_FAST( cond, msg_0)  \
      do { if (! (cond)) { ohm_util_TRACE_ERROR_FAST (msg_0); } assert (cond); } while (false)

#else // defined (ohm_util_TRACE_DEBUG_FLAG)

   #define  ohm_util_TRACE_DEBUG( msg_0)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG1( fmt_0, arg_1)   \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG2( fmt_0, arg_1, arg_2)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG3( fmt_0, arg_1, arg_2, arg_3) \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG4( fmt_0, arg_1, arg_2, arg_3, arg4) \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG5( fmt_0, arg_1, arg_2, arg_3, arg4, arg5) \
      ohm_util_DO_NOTHING

   #define  ohm_util_TRACE_DEBUG_FAST( msg_0)   \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG1_FAST( fmt_0, arg_1) \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG2_FAST( fmt_0, arg_1, arg_2)   \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG3_FAST( fmt_0, arg_1, arg_2, arg_3)  \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG4_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4) \
      ohm_util_DO_NOTHING
   #define  ohm_util_TRACE_DEBUG5_FAST( fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5)   \
      ohm_util_DO_NOTHING

   #define  ohm_util_TRACE_ASSERT( cond, msg_0) \
      do { assert (cond); } while (false)
   #define  ohm_util_TRACE_ASSERT_FAST( cond, msg_0)  \
      do { assert (cond); } while (false)

#endif   // defined (ohm_util_TRACE_DEBUG_FLAG)

// . . . End of ohm::txt::out::PrintNa requirement



namespace ohm
{
namespace util
{



}  // namespace util
}  // namespace ohm



//#include  "ohm/util/trace.hpp"



#endif   // ohm_util_trace_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
