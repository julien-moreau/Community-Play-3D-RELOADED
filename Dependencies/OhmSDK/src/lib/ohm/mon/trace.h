/*****************************************************************************

        trace.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67519

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



#if ! defined (ohm_mon_trace_HEADER_INCLUDED)
#define  ohm_mon_trace_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#include "ohm/mon/Monitor.h"
#include "ohm/util/trace.h"

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   // for backtrace, backtrace_symbols
   // MacOS X.5 or later is assumed
   #include <execinfo.h>

#endif



#if defined (ohm_mon_DISABLE_TRACE_FLAG)

   #define ohm_mon_TRACE_NO_ARG_MACRO(msg_0) ((void) 0)
   #define ohm_mon_TRACE_ARG_MACRO(fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8) ((void) 0)
   #define ohm_mon_DUMP ((void) 0)
   #define ohm_mon_BACKTRACE ((void) 0)
   
#else // defined (ohm_mon_DISABLE_TRACE_FLAG)

   #if (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_VC)
      #define ohm_mon_PRETTY_FUNCTION __FUNCDNAME__
      
   #elif (ohm_archi_COMPILER_CLASS == ohm_archi_COMPILER_CLASS_GCC)
      #define ohm_mon_PRETTY_FUNCTION __PRETTY_FUNCTION__
      
   #else
      #error Compiler not supported

   #endif

   // When no argument are used, they are fast functions as only
   // 'msg_0' *pointer* is copied.

   #define ohm_mon_TRACE_NO_ARG_MACRO(msg_0)                \
      do { ohm::mon::Monitor::use_instance ().write_na (    \
         ohm::mon::Monitor::thread_self (),                 \
         ohm::mon::Monitor::counter (),                     \
         ohm_mon_PRETTY_FUNCTION,                           \
         "" msg_0 ""                                        \
      ); } while (false)


   // When arguments are used, a txt::out::PrintNa class is used
   // to evaluate, and the a string is put into memory

   #define ohm_mon_TRACE_ARG_MACRO(fmt_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8)  \
      do {                                                                                               \
         char ohm_mon_output_0______ [1024];                                                             \
         ohm::txt::out::PrintNa ohm_mon_txt______ (ohm_mon_output_0______, 1023);                        \
         ohm_mon_txt______.print (fmt_0)                                                                 \
            .arg (arg_1).arg (arg_2).arg (arg_3).arg (arg_4)                                             \
            .arg (arg_5).arg (arg_6).arg (arg_7).arg (arg_8);                                            \
         ohm::mon::Monitor::use_instance ().write (                                                      \
            ohm::mon::Monitor::thread_self (),                                                           \
            ohm::mon::Monitor::counter (),                                                               \
            ohm_mon_PRETTY_FUNCTION,                                                                     \
            ohm_mon_output_0______                                                                       \
         );                                                                                              \
      } while (false)

   #define ohm_mon_DUMP                                           \
      do {                                                        \
         ohm::mon::Monitor::use_instance ().suspend_threads ();   \
         ohm::mon::Monitor::use_instance ().trace_dump ();        \
         ohm::mon::Monitor::use_instance ().resume_threads ();    \
      } while (false)

   #define ohm_mon_BACKTRACE                                         \
         do {                                                        \
            ohm::mon::Monitor::use_instance ().trace_backtrace ();   \
         } while (false)

#endif   // defined (ohm_mon_DISABLE_TRACE_FLAG)



// A ";" is needed after these macros

// LOW frequency : user actions, around 1/2 second or less

#define ohm_mon_TRACE(msg_0)                    \
   ohm_mon_TRACE_NO_ARG_MACRO (msg_0)

#define ohm_mon_TRACE1(format_0, arg_1)            \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, 0, 0, 0, 0, 0, 0, 0)

#define ohm_mon_TRACE2(format_0, arg_1, arg_2)           \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, arg_2, 0, 0, 0, 0, 0, 0)

#define ohm_mon_TRACE3(format_0, arg_1, arg_2, arg_3)          \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)

#define ohm_mon_TRACE4(format_0, arg_1, arg_2, arg_3, arg_4)            \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)

#define ohm_mon_TRACE5(format_0, arg_1, arg_2, arg_3, arg_4, arg_5)           \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

#define ohm_mon_TRACE6(format_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6)          \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, 0, 0)

#define ohm_mon_TRACE7(format_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7)            \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, 0)

#define ohm_mon_TRACE8(format_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8)           \
   ohm_mon_TRACE_ARG_MACRO (format_0, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8)

// Value version

#define ohm_mon_TRACEV1(arg_1)            \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% ", arg_1, 0, 0, 0, 0, 0, 0, 0)

#define ohm_mon_TRACEV2(arg_1, arg_2)           \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% " #arg_2 "=%2% ", arg_1, arg_2, 0, 0, 0, 0, 0, 0)

#define ohm_mon_TRACEV3(arg_1, arg_2, arg_3)          \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% " #arg_2 "=%2% " #arg_3 "=%3% ", arg_1, arg_2, arg_3, 0, 0, 0, 0, 0)

#define ohm_mon_TRACEV4(arg_1, arg_2, arg_3, arg_4)            \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% " #arg_2 "=%2% " #arg_3 "=%3% " #arg_4 "=%4% ", arg_1, arg_2, arg_3, arg_4, 0, 0, 0, 0)

#define ohm_mon_TRACEV5(arg_1, arg_2, arg_3, arg_4, arg_5)           \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% " #arg_2 "=%2% " #arg_3 "=%3% " #arg_4 "=%4%" #arg_5 "=%5% ", arg_1, arg_2, arg_3, arg_4, arg_5, 0, 0, 0)

#define ohm_mon_TRACEV6(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6)          \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% " #arg_2 "=%2% " #arg_3 "=%3% " #arg_4 "=%4%" #arg_5 "=%5% " #arg_6 "=%6% ", arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, 0, 0)

#define ohm_mon_TRACEV7(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7)            \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% " #arg_2 "=%2% " #arg_3 "=%3% " #arg_4 "=%4%" #arg_5 "=%5% " #arg_6 "=%6% " #arg_7 "=%7% ", arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, 0)

#define ohm_mon_TRACEV8(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8)           \
   ohm_mon_TRACE_ARG_MACRO (#arg_1 "=%1% " #arg_2 "=%2% " #arg_3 "=%3% " #arg_4 "=%4%" #arg_5 "=%5% " #arg_6 "=%6% " #arg_7 "=%7% " #arg_8 "=%8% ", arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7, arg_8)



namespace ohm
{
namespace mon
{



}  // namespace mon
}  // namespace ohm



//#include  "ohm/mon/trace.hpp"



#endif   // ohm_mon_trace_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
