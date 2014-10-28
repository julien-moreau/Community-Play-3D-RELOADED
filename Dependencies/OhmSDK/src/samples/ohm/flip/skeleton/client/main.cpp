/*****************************************************************************

        main.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71422

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
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define  NOMINMAX
   #define  WIN32_LEAN_AND_MEAN

   #include <windows.h>
#endif

#include "client/Application.h"
#include "ohm/odbg/LogMgr.h"

#include "ohm/opa/private/WidgetLayoutManager.h"
#include "ohm/opa/Debugger.h"
#include "ohm/task/Clock.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/task/private/EventProvider.h"
#include "ohm/txt/fnc.h"
#include "ohm/util/ArgcArgvParser.h"
#include "ohm/util/LogMgr.h"
#include "ohm/util/trace.h"



#if defined (OHM_STUDIO_WEB)
   #include <curl/curl.h>
#endif      

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/ApplicationImpl.h"
   #include "ohm/opal/win32/MenuBarImpl.h"
   #include "client/Constants.h"

   #if defined (_MSC_VER)
   #include <crtdbg.h>
   #include <new.h>
   #endif   // _MSC_VER

   #include <new>

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)

#endif   // (ohm_archi_SYS == ohm_archi_SYS_WIN)

#include <cassert>




/*\\\ CLASS DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

#if defined (_MSC_VER)
static int __cdecl   OHMF_new_handler_cb (size_t dummy)
{
   throw std::bad_alloc ();
   return (0);
}
#endif   // _MSC_VER



#if defined (_MSC_VER) && ! defined (NDEBUG)
static int  __cdecl  OHMF_debug_alloc_hook_cb (int alloc_type, void *user_data_ptr, size_t size, int block_type, long request_nbr, const unsigned char *filename_0, int line_nbr)
{
   if (block_type != _CRT_BLOCK) // Ignore CRT blocks to prevent infinite recursion
   {
      switch (alloc_type)
      {
      case  _HOOK_ALLOC:
      case  _HOOK_REALLOC:
      case  _HOOK_FREE:

         // Put some debug code here

         break;

      default:
         assert (false);   // Undefined allocation type
         break;
      }
   }

   return (1);
}
#endif



#if defined (_MSC_VER) && ! defined (NDEBUG)
static int  __cdecl  OHMF_debug_report_hook_cb (int report_type, char *user_msg_0, int *ret_val_ptr)
{
   // see http://msdn.microsoft.com/en-us/library/74kabxyx(VS.80).aspx
   *ret_val_ptr = 0; // A value of zero continues execution, a value of 1 starts the debugger

   switch (report_type)
   {
   case  _CRT_WARN:
   case  _CRT_ERROR:
   case  _CRT_ASSERT:

// Put some debug code here

      break;
   }

   return (0); // If the hook handles the message in question completely, so that no further reporting is required, it should return TRUE. If it returns FALSE, _CrtDbgReport will report the message normally
}
#endif



static void OHMF_main_prog_init (::HINSTANCE instance)
{
#if defined (_MSC_VER)
   ::_set_new_handler (::OHMF_new_handler_cb);
#endif   // _MSC_VER

#if defined (_MSC_VER) && ! defined (NDEBUG) && defined (_DEBUG)
   {
      const int   mode =   (1 * _CRTDBG_MODE_DEBUG)
                         | (0 * _CRTDBG_MODE_WNDW);
      ::_CrtSetReportMode (_CRT_WARN, mode);
      ::_CrtSetReportMode (_CRT_ERROR, mode);
      ::_CrtSetReportMode (_CRT_ASSERT, mode);

      const int   old_flags = ::_CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
      ::_CrtSetDbgFlag (  old_flags
                        | (1 * _CRTDBG_LEAK_CHECK_DF)
                        | (0 * _CRTDBG_CHECK_ALWAYS_DF));
      ::_CrtSetBreakAlloc (-1);  // Specify here a memory bloc number
      ::_CrtSetAllocHook (OHMF_debug_alloc_hook_cb);
      ::_CrtSetReportHook (OHMF_debug_report_hook_cb);
   }
#endif   // _MSC_VER, NDEBUG

   ohm::opal::ApplicationImpl::use_instance ().set_hinst (instance);
}



static void OHMF_main_prog_end ()
{  
#if defined (_MSC_VER) && ! defined (NDEBUG) && defined (_DEBUG)
   {
      const int   mode =   (1 * _CRTDBG_MODE_DEBUG)
                         | (0 * _CRTDBG_MODE_WNDW);
      ::_CrtSetReportMode (_CRT_WARN, mode);
      ::_CrtSetReportMode (_CRT_ERROR, mode);
      ::_CrtSetReportMode (_CRT_ASSERT, mode);

      ::_CrtMemState mem_state;
      ::_CrtMemCheckpoint (&mem_state);
      ::_CrtMemDumpStatistics (&mem_state);
   }
#endif   // _MSC_VER, NDEBUG
}
#endif   // (ohm_archi_SYS == ohm_archi_SYS_WIN)



void  parse_arguments (int argc, const char * const * argv)
{
   for (int i = 1 ; i < argc ; ++i)
   {
      std::string arg (argv [i]);
      
      if (arg.size () > 2)
      {
         if ((arg [0] == '-') && (arg.find ('=') != std::string::npos))
         {
            size_t pos = arg.find ('=');
            
            std::string key = arg.substr (1, pos - 1);
            std::string value = arg.substr (pos + 1);
            
            client::Application::use_instance ().set (key, value);
         }
      }
   }
}



#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

int WINAPI  WinMain (::HINSTANCE instance, ::HINSTANCE prev_instance, ::LPSTR cmd_line_0, int cmd_show)
{
   OHMF_main_prog_init (instance);

   ohm::odbg::LogMgr::Config config;
   config._display_level_flag = true;
   config._display_scope_flag = false;
   config._display_func_flag = false;
   config._display_line_nbr_flag = false;
   config._display_ns_flag = false;
   
   ohm::odbg::LogMgr::create_as_win32_debugger (config);

   ohm::util::ArgcArgvParser parser;
   
   parser.parse_line (cmd_line_0, false);
   
   int argc = 0;
   const char * const * argv = 0;

   parser.get_argc_argv (argc, argv);
   
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)

int main (int argc, char* argv [])
{
   ohm::odbg::LogMgr::Config config;
   config._use_thread_indentation_flag = true;
   config._thread_indentation = 20;
   
   config._display_level_flag = false;
   config._display_scope_flag = false;
   config._display_func_flag = false;
   config._display_line_nbr_flag = false;
   config._display_ns_flag = false;

   ohm::odbg::LogMgr::create_as_printf (config);
   
   
#endif // ohm_archi_SYS
   
   parse_arguments (argc, argv);
   
   try
   {
      using namespace ohm;

      // test
      
      task::RunLoopGui & run_loop_gui = task::RunLoopGui::use_instance ();
      
      task::RunLoopManager::use_instance ().push_current (run_loop_gui);
      opa::WidgetLayoutManager::use_instance ().init ();
      
//    opa::Debugger::enable_events_trace (true);
//    opa::Debugger::trace (opal::Event::Type_CUSTOM_SENT);
//    opa::Debugger::trace (opal::Event::Type_CONDUCT_CHANGED);
//    opa::Debugger::trace (opal::Event::Type_MOUSE_BUTTON_PRESSED);
//    opa::Debugger::trace (opal::Event::Type_MOUSE_MOVED);
//    opa::Debugger::trace (opal::Event::Type_MOUSE_BUTTON_RELEASED);
//    opa::Debugger::trace (opal::Event::Type_KEY_PRESSED);
//    opa::Debugger::trace (opal::Event::Type_KEY_RELEASED);
//    opa::Debugger::enable_frame_trace ();
//    opa::Debugger::enable_frame_break ();
//    opa::Debugger::enable_prim_trace ();
//    opa::Debugger::trace_prim_display ();
//    opa::Debugger::enable_prim_break ();
      
      client::Application::use_instance ().run ();
      
      opa::WidgetLayoutManager::use_instance ().restore ();
      task::RunLoopManager::use_instance ().pop_current (run_loop_gui);

      task::Clock::use_instance ().release ();
      
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
      opal::MenuBarImpl::use_instance ().release ();
#endif
      run_loop_gui.release ();
      
      task::EventProvider::use_instance ().release ();
   }
   catch (...)
   {
      assert (false);
   }


   
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

   OHMF_main_prog_end ();

#endif   // ohm_archi_SYS
   
   // sleep (10000000);

   return 0;
}


