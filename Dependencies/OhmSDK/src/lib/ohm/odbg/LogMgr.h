/*****************************************************************************

        LogMgr.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70250

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



#if defined (ohm_odbg_LogMgr_CURRENT_HEADER)
   #error Recursive inclusion of ohm/odbg/LogMgr header.
#endif
#define  ohm_odbg_LogMgr_CURRENT_HEADER

#if ! defined (ohm_odbg_LogMgr_HEADER_INCLUDED)
#define  ohm_odbg_LogMgr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/odbg/Debugger.h"
#include "ohm/archi/def.h"
#include "ohm/txt/out/PrintInterface.h"
//#include "ohm/sys/RemoteMessagePort.h"
#include "ohm/sys/Mutex.h"

#include <map>
#include <memory>
#include <string>

#include <cstdio>



namespace ohm
{
namespace odbg
{



class LogMgr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         NBR_BUF_LINES  = 64  };
   
   class Config
   {
   public:
                  Config ();
                  
      bool        _display_level_flag;
      bool        _display_scope_flag;
      bool        _display_func_flag;
      bool        _display_line_nbr_flag;
      bool        _display_ns_flag;
      bool        _use_thread_indentation_flag;
      int         _thread_indentation;
   };

                  ~LogMgr ();

   void           write (Debugger::Level level, const char * msg_0, const char * scope_0 = "", const char * func_0 = "", long line_nbr = -1);
   void           write_cache (Debugger::Level level, const char * msg_0, const char * scope_0 = "", const char * func_0 = "", long line_nbr = -1);
   void           write_dump_abort (Debugger::Level level, const char * msg_0, const char * scope_0 = "", const char * func_0 = "", long line_nbr = -1);
   void           indent (int cnt);
   
   static void    set_scope_crop (const char * scope_pattern_0);
   
   static void    create_as_util (Config config = Config ());
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   static void    create_as_util_library_logs (const char * manufacturer_0, const char * log_name_0, Config config = Config ());
#endif
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   static void    create_as_util_app_data_logs (const char * manufacturer_0, const char * application_0, const char * log_name_0, Config config = Config ());
#endif
   static void    create_as_syslog (const char * namespace_0);
   static void    create_as_printf (Config config = Config ());
   /*static void     create_as_profiler ();
   static void    create_as_analog_util ();
   static void    create_as_analog_printf ();
   static void    create_as_analog_ipc (const char * src_name_0);*/
   static void    create_as_win32_debugger (Config config = Config ());
   static void    create_as_plugin (const char * namespace_0, Debugger::FncPtr fnc_ptr);
   
   static LogMgr &use_instance ();

   static archi::Int32
                  execute_command (archi::Int32 command, void * data_ptr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum Type
   {
      Type_NONE = -1,
      Type_UTIL = 0,
      Type_SYSLOG,
      Type_PRINTF,
      /*Type_PROFILER,
      Type_ANALOG_UTIL,
      Type_ANALOG_PRINTF,
      Type_ANALOG_IPC,*/
      Type_WIN32_DEBUGGER,
      Type_PLUGIN,

      Type_NBR_ELT
   };

                  LogMgr ();
   
   void           write_util (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   void           write_util_cache (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   void           write_syslog (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   void           write_printf (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   /*void            write_profiler (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   void           write_analog_util (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   void           write_analog_printf (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   void           write_analog_ipc (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);
   void           write_analog_str (txt::out::PrintInterface & txt, Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);*/
   void           escape_slash (std::string & txt);
   void           write_win32_debugger (Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);

   void           write_str (txt::out::PrintInterface & txt, bool trailing_cr_flag, Debugger::Level level, const char * msg_0, const char * scope_0, const char * func_0, long line_nbr, const char * ns_0);

   static void    create_empty_if_not_existing ();

   Type           _type;
   Config         _config;
   char           _namespace_0 [257];
   char           _src_name_0 [257];
   int            _indent;
   size_t         _scope_crop;

   Debugger::FncPtr
                  _to_host_fnc_ptr;
   
   /*ohm::sys::RemoteMessagePort *
                  _remote_msg_port_ptr;*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   sys::Mutex     _mutex;
   int            _nbr_thread;
   std::map <pthread_t, int>
                  _thread_indent_map;
   
#endif

   static std::auto_ptr <LogMgr>
                  _instance_aptr;

   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  LogMgr (const LogMgr &other);
   LogMgr &       operator = (const LogMgr &other);
   bool           operator == (const LogMgr &other);
   bool           operator != (const LogMgr &other);

}; // class LogMgr



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace odbg
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_odbg_LogMgr_HEADER_INCLUDED

#undef ohm_odbg_LogMgr_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
