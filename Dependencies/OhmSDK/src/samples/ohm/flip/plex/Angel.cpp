/*****************************************************************************

        Angel.cpp
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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/Angel.h"

#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : run
Description :
   This is only call when '--angel' configuration option is present.
   For the angel, this function will run indefinitely and will call exit when
   a shutdown of the whole system is required.
   For osplex, this function will return.
   Internally, this function will 'fork' to create osplex, and will
   automatically handle osplex termination.
==============================================================================
*/

void  Angel::run ()
{
   init_sigaction ();
   
   while (true)
   {
      if (_start_process)
      {
         _osplex_pid = fork ();
         
         if (_osplex_pid == 0)
         {
            // this is child, osplex
            
            // from sigaction man page :
            // "A child created via fork(2) inherits a copy of its parent's signal
            // dispositions."
            
            reset_sigaction ();
            
            // return and continue execution
            
            return;
         }
         else if (_osplex_pid == -1)
         {
            exit (-1);
         }
         
         // this is angel

         _start_process = 0;
      }
      
      int exit_code = 0;
      pid_t pid = waitpid (_osplex_pid, &exit_code, 0);
      
      if (pid == -1)
      {
         switch (errno)
         {
         case EINTR:
            // nothing
            break;
         
         case ECHILD:
            if (!_start_process)
            {
               exit (0);
            }
            break;
         
         default:
            break;
         }
      }
      else
      {
         if (WIFEXITED (exit_code))
         {
            // normal shutdown
            exit (0);
         }
         else if (WIFSIGNALED (exit_code))
         {
            // signaled, restart
            
            _start_process = 1;
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init_sigaction
==============================================================================
*/

void  Angel::init_sigaction ()
{
   struct sigaction act;
   
   ::memset (&act, 0, sizeof (act));
   
   // ignore SIGPIPE & SIGUSR1

   act.sa_handler = SIG_IGN;
   sigaction (SIGPIPE, &act, 0);
   sigaction (SIGUSR1, &act, 0);
   
   // handle SIGINT, SIGTERM, SIGHUP, SIGALRM, SIGCHLD

   act.sa_sigaction = sigaction_handler;
   sigemptyset (&act.sa_mask);
   act.sa_flags = SA_SIGINFO;

   sigaction (SIGINT,  &act, 0);
   sigaction (SIGTERM, &act, 0);
   sigaction (SIGHUP,  &act, 0);
   sigaction (SIGALRM, &act, 0);
   sigaction (SIGCHLD, &act, 0);
}



/*
==============================================================================
Name : reset_sigaction
==============================================================================
*/

void  Angel::reset_sigaction ()
{
   struct sigaction act;
   
   ::memset (&act, 0, sizeof (act));
   
   // reset to defaults

   act.sa_handler = SIG_DFL;
   sigaction (SIGPIPE, &act, 0);
   sigaction (SIGUSR1, &act, 0);
   sigaction (SIGINT,  &act, 0);
   sigaction (SIGTERM, &act, 0);
   sigaction (SIGHUP,  &act, 0);
   sigaction (SIGALRM, &act, 0);
   sigaction (SIGCHLD, &act, 0);
}



/*
==============================================================================
Name : sigaction_handler
==============================================================================
*/

void  Angel::sigaction_handler (int sig, siginfo_t * siginfo_ptr, void *)
{
   switch (sig)
   {
   case SIGINT:
   case SIGTERM:
      sigaction_handler_term (siginfo_ptr);
      break;
   }
}



/*
==============================================================================
Name : sigaction_handler_term
==============================================================================
*/

void  Angel::sigaction_handler_term (siginfo_t * siginfo_ptr)
{
}



volatile sig_atomic_t   Angel::_start_process (1);
volatile pid_t Angel::_osplex_pid (-1);



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

