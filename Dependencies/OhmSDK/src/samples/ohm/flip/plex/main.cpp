/*****************************************************************************

        main.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71330

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

#include "ohm/archi/types.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/util/trace.h"
#include "ohm/odbg/LogMgr.h"

#include "plex/Angel.h"
#include "plex/Central.h"
#include "plex/http/HttpCentral.h"
#include "plex/sess/SessCentral.h"
#include "plex/shared/Configuration.h"
#include "plex/fnc.h"

// warning : the following #include depends on inclusion rules
#include "model/fnc.h"

#include <string>

#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>
#include <cassert>




/*\\\ CLASS DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : usage
==============================================================================
*/

#define m_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
#define m_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
#define m_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5) ohm_util_TRACE_INFO5(fmt_0, arg1, arg2, arg3, arg4, arg5)

/*
==============================================================================
Name : main
==============================================================================
*/

int main (int argc, char * argv [])
{
   using namespace ohm;
   using namespace plex;

#if 1
   ohm::odbg::LogMgr::Config config;
   config._display_level_flag = false;
   config._display_scope_flag = false;
   config._display_func_flag = false;
   config._display_line_nbr_flag = false;
   config._display_ns_flag = false;

   odbg::LogMgr::create_as_printf (config);

#elif 0
   odbg::LogMgr::create_as_syslog ("SkeletonServer");
   
   openlog ("SkeletonServer", LOG_PID | LOG_NDELAY | LOG_CONS, LOG_USER);
   setlogmask (LOG_UPTO (LOG_DEBUG));

#endif

   int ret_val = EXIT_SUCCESS;
   
   try
   {
      // specific init for the malloc debug tool not to mark too much
      
      {
         // used by 'strftime'
         tzset ();
         
         // instantiate singleton
         ohm::sys::TimerReliable::use_instance ();
      }
      
      Configuration & conf = Configuration::use ();

      conf._bin_path = argv [0];
      
      bool ok_flag = conf.set (argc, argv);
      
      if (!ok_flag)
      {
         return EXIT_FAILURE;
      }
      
      if (conf._daemon_flag)
      {
         daemonize ();
      }
      
      if (conf._angel_flag)
      {
         Angel::run ();
      }
      
      if (conf._http_flag)
      {
         std::auto_ptr <HttpCentral> central_aptr (new HttpCentral);
         
         central_aptr->run ();
      }
      else if (conf._sess_flag)
      {
         model::declare ();
         
         std::auto_ptr <SessCentral> central_aptr (new SessCentral (conf._session));
         
         central_aptr->run ();
      }
      else
      {
         std::auto_ptr <Central> central_aptr (new Central);
         
         central_aptr->run ();
      }
   }
   catch (...)
   {
      ohm_util_TRACE_PANIC ("exception thrown!");
      ret_val = EXIT_FAILURE;
   }

   return ret_val;
}


