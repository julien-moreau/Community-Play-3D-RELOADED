/*****************************************************************************

        fnc.cpp
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

#include "plex/fnc.h"

#include "plex/shared/Configuration.h"
#include "plex/Connection.h"

#include "ohm/util/trace.h"

#include <stdexcept>

#include <sys/wait.h>
#include <resolv.h>
#include <sys/stat.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : create_listen
Description :
   Create, bind and listen to the socket that will accept clients on 'port'
==============================================================================
*/

Connection *   create_listen (Central & central, int port)
{
   // create the socket
   
   int sd = ::socket (PF_INET, SOCK_STREAM, 0);
   
   if (sd < 0)
   {
      assert (false);
      ohm_util_TRACE_PANIC1 ("'socket' failed with error `%1%'", strerror (errno));
      
      throw std::runtime_error ("fnc::init_listen_sd: socket failed");
   }
   
   // reuse port if binded (after a server crash)
   
   {
      int val = 1;
      int err = ::setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, (const char *) (&val), sizeof (val));
      
      if (err != 0)
      {
         assert (false);
         ohm_util_TRACE_PANIC1 ("'setsockopt SO_REUSEADDR' error `%1%'", strerror (errno));
         
         throw std::runtime_error ("fnc::init_listen_sd: setsockopt failed");
      }
   }
   
   // bind the socket
   
   ohm_util_TRACE_INFO1 ("Listening on port %1%", port);
   
   ::sockaddr_in addr;
   
   addr.sin_family = AF_INET;
   addr.sin_port = htons (port);
   addr.sin_addr.s_addr = INADDR_ANY;
   
   int err = ::bind (sd, (::sockaddr *) &addr, sizeof (addr));
   
   if (err != 0)
   {
      ohm_util_TRACE_PANIC1 ("'bind' failed with error `%1%'", strerror (errno));
      assert (false);
      
      throw std::runtime_error ("fnc::init_listen_sd: bind failed");
   }
   
   // listen
   
   const int max_pending_cnxs = 128;
   
   err = ::listen (sd, max_pending_cnxs);
   
   if (err != 0)
   {
      ohm_util_TRACE_PANIC1 ("'listen' failed with error `%1%'", strerror (errno));
      assert (false);
      
      throw std::runtime_error ("fnc::init_listen_sd: listen failed");
   }
   
   return new Connection (central, sd, addr);
}



/*
==============================================================================
Name : daemonize
==============================================================================
*/

void  daemonize ()
{
   /* http://www.enderunix.org/docs/eng/daemon.php */
   
   pid_t pid = ::fork ();
   
   if (pid < 0)
   {
      ohm_util_TRACE_PANIC1 ("'fork' failed with error `%1%'", strerror (errno));
      assert (false);
      
      exit (EXIT_FAILURE);
   }
   else if (pid > 0)
   {
      exit (EXIT_SUCCESS);
   }
   
   umask (0);
   pid_t sid = setsid ();
   
   if (sid < 0)
   {
      exit (EXIT_FAILURE);
   }
   
   close (STDIN_FILENO);
   close (STDOUT_FILENO);
   close (STDERR_FILENO);
}



/*
==============================================================================
Name : set_non_blocking
==============================================================================
*/

void  set_non_blocking (int fd)
{
   int flag = ::fcntl (fd, F_GETFL, 0);
   assert (flag != -1);
   
   int err = ::fcntl (fd, F_SETFL, flag | O_NONBLOCK);
   assert (err == 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

