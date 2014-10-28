/*****************************************************************************

        boot.hpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71405

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



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   // First the usual define for the Windows headers.
   #define  NOMINMAX
   #define  WIN32_LEAN_AND_MEAN

   // Then this before including any other Windows headers,
   // because of the ExceptionHandler things.
   #include <ntstatus.h>
   #define  WIN32_NO_STATUS

   // And now these ones, they must be included before windows.h
   #include <winsock2.h>
   #include <windows.h>

   #if defined (_MSC_VER)
   #include <crtdbg.h>
   #include <new.h>
   #endif   // _MSC_VER

   #include <new>

   #include "ohm/opal/win32/ApplicationImpl.h"
#endif



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



namespace ohm
{
namespace util
{


class Boot
{
public:
   Boot (::HINSTANCE instance, ::LPSTR cmd_line_0, int argc, const char * const * argv)
   {
      OHMF_main_prog_init (instance);
      
      ohm::util::ArgcArgvParser parser;
      parser.parse_line (cmd_line_0, false);
      parser.get_argc_argv (argc, argv);
   }
   
   ~Boot () // DON'T INHERIT FROM util::Boot
   {
      OHMF_main_prog_end ();
   }

private:
   Boot ();
};


}  // namespace util
}  // namespace ohm



#endif   // (ohm_archi_SYS == ohm_archi_SYS_WIN)



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
