/*****************************************************************************

        Central.h
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



#if ! defined (plex_Central_HEADER_INCLUDED)
#define  plex_Central_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/FdEventMgr.h"
#include "plex/Stats.h"

#include "ohm/sys/TimerReliable.h"

#include <set>

#include <sys/socket.h>
#include <arpa/inet.h>



namespace plex
{



class ClientFlip;
class ClientHttp;
class Connection;
class Session;

class Central
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Central ();
   virtual        ~Central ();
   
   void           run ();
   
   FdEventMgr &   use_fd_event_mgr ();
   Stats &        use_stats ();
   
   bool           bind (ClientFlip & client);
   void           unbind (Session & session, ClientFlip & client);
   
   void           add_waitpid_async (pid_t pid);
   
   void           req_terminate (Session & session);
   void           req_shutdown ();
   
   static const char *
                  _ns_log_0;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class SessionInfo
   {
   public:
      enum State
      {
                  State_RUNNING = 0,
                  State_STOPPING,
                  State_RELEASING,
      };
      
      Session *   _session_ptr;
      std::string _format_version;
      State       _state;
   };
   
   typedef std::map <std::string, SessionInfo> SessionInfoMap;
   
   typedef std::set <ClientFlip *>  ClientFlipSet;
   typedef std::map <ClientFlip *, double>   ClientFlipTsMap;
   typedef std::set <ClientHttp *>  ClientHttpSet;
   typedef std::set <std::string>   UsernameSet;
   typedef std::set <pid_t>   PidSet;
   
   void           run_flip ();
   void           run_http ();
   void           run_loop ();
   void           run_exit ();
   
   void           process ();
   
   void           process_timeout_client ();
   void           process_timeout_server ();
   void           process_release_client_flip ();
   void           process_queued_client_flip ();
   void           process_release_client_http ();
   void           process_release_session ();
   void           process_notify_client_http ();
   void           process_waitpid_async ();

   static void    process_accept_flip (void * ctx_ptr);
   void           process_accept_flip ();

   static void    process_accept_http (void * ctx_ptr);
   void           process_accept_http ();

   ohm::sys::TimerReliable &
                  _tr;
   
   std::auto_ptr <Connection>
                  _flip_cnx_aptr;
   std::auto_ptr <Connection>
                  _http_cnx_aptr;
   
   FdEventMgr     _fd_event_mgr;
   ClientFlipSet  _client_flip_set;
   ClientFlipTsMap
                  _queued_client_flip_map;
   ClientHttpSet  _client_http_set;
   UsernameSet    _username_set;
   SessionInfoMap _session_info_map;
   
   double         _stats_ts;
   Stats          _stats;
   
   PidSet         _pid_set;
   
   bool           _shutdown_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Central (const Central &other);
   Central &      operator = (const Central &other);
   bool           operator == (const Central &other);
   bool           operator != (const Central &other);

}; // class Central



}  // namespace plex



//#include  "Central.hpp"



#endif   // plex_Central_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

