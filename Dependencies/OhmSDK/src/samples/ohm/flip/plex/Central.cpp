/*****************************************************************************

        Central.cpp
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

#include "plex/Central.h"

#include "plex/ClientFlip.h"
#include "plex/ClientHttp.h"
#include "plex/shared/Configuration.h"
#include "plex/Connection.h"
#include "plex/Session.h"
#include "plex/fnc.h"
#include "plex/shared/LogMgr.h"

#include "ohm/util/trace.h"

#include <stdexcept>

#include <dirent.h>
#include <sys/wait.h>
#include <resolv.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const char *   Central::_ns_log_0 = "Central";

/*
==============================================================================
Name : ctor
==============================================================================
*/

Central::Central ()
:  _tr (_tr)
,  _flip_cnx_aptr ()
,  _http_cnx_aptr ()
,  _fd_event_mgr ()
,  _client_flip_set ()
,  _client_http_set ()
,  _username_set ()
,  _session_info_map ()
,  _stats_ts (_tr.get ())
,  _stats (*this)
,  _shutdown_flag (false)
{
   TRACEo ("Starting");

   // ignore SIGPIPE : write will return EPIPE
   
   ::signal (SIGPIPE, SIG_IGN);
   
   ClientFlip::init_static ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Central::~Central ()
{
   TRACEo ("Ending");
}



/*
==============================================================================
Name : run
Description :
   Start listening socket for connections. Each time a connection is made,
   the connection is checked and a new 'Client' is builded.
==============================================================================
*/

void  Central::run ()
{
   run_flip ();
   run_http ();

   run_loop ();

   run_exit ();
}



/*
==============================================================================
Name : use_fd_event_mgr
==============================================================================
*/

FdEventMgr &   Central::use_fd_event_mgr ()
{
   return _fd_event_mgr;
}



/*
==============================================================================
Name : use_stats
==============================================================================
*/

Stats &  Central::use_stats ()
{
   return _stats;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

bool  Central::bind (ClientFlip & client)
{
   const double now = _tr.get ();
   
   const int sd = client.get_sd ();
   const std::string & username = client.get_username ();
   const std::string & session = client.get_session ();
   const std::string & format_version = client.get_format_version ();
   
   if (_username_set.find (username) != _username_set.end ())
   {
      TRACEX1 ("c%1%: already binded", sd);
      
      return false;
   }
   
   _username_set.insert (username);
   
   SessionInfoMap::iterator it = _session_info_map.find (session);
   
   Session * session_ptr = 0;
   
   if (it == _session_info_map.end ())
   {
      SessionInfo & info = _session_info_map [session];
      
      const Configuration & conf = Configuration::get ();
      
      info._session_ptr = new Session (*this, conf._bin_path, session);
      info._format_version = format_version;
      info._state = SessionInfo::State_RUNNING;
      
      session_ptr = info._session_ptr;
   }
   else
   {
      SessionInfo & info = it->second;
      
      if (info._format_version != format_version)
      {
         TRACEX2 ("c%1%: wrong format version for %2%", sd, session.c_str ());
         
         _username_set.erase (username);
         
         return false;
      }
      else if (
         (info._state == SessionInfo::State_STOPPING)
         || (info._state == SessionInfo::State_RELEASING)
         )
      {
         _username_set.erase (username);
         
         ClientFlipTsMap::iterator it = _queued_client_flip_map.find (&client);
         
         if (it == _queued_client_flip_map.end ())
         {
            _queued_client_flip_map [&client] = _tr.get ();
            
            TRACEI2 ("c%1%: queueing for %2%", sd, session.c_str ());
         }
      }
      else
      {
         session_ptr = info._session_ptr;
      }
   }
   
   if (session_ptr != 0)
   {
      ClientFlipTsMap::iterator it = _queued_client_flip_map.find (&client);
      
      if (it != _queued_client_flip_map.end ())
      {
         TRACEI3 (
            "c%1%: entered %2% (queued for %3% ms)",
            sd, session.c_str (), (now - it->second) * 1000.0
         );
         
         _queued_client_flip_map.erase (&client);
      }
      
      session_ptr->bind (client);
      client.bind (*session_ptr);
   }
   
   _stats.notify_process_duration (
      &Metrics::_proc_bi_max_us, &Metric::_proc_bi_max_us, _tr.get () - now
   );
   
   return true;
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  Central::unbind (Session & session, ClientFlip & client)
{
   const double now = _tr.get ();
   
   const std::string & username = client.get_username ();
   
   _username_set.erase (username);

   session.unbind (client);
   
   if (session.empty ())
   {
      // the session is currently quitting, we must not accept any clients
      // anymore
      
      const std::string & session_id = session.get_session ();
      
      SessionInfoMap::iterator it = _session_info_map.find (session_id);
      const SessionInfoMap::iterator it_end = _session_info_map.end ();
      assert (it != it_end);
      
      if (it != it_end)
      {
         SessionInfo & info = it->second;
         assert (&session == info._session_ptr);
         
         info._state = SessionInfo::State_STOPPING;
      }
   }
   
   _stats.notify_process_duration (
      &Metrics::_proc_ubi_max_us, &Metric::_proc_ubi_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : add_waitpid_async
==============================================================================
*/

void  Central::add_waitpid_async (pid_t pid)
{
   _pid_set.insert (pid);
}



/*
==============================================================================
Name : req_terminate
==============================================================================
*/

void  Central::req_terminate (Session & session)
{
   assert (session.empty ());
   const std::string & session_id = session.get_session ();
   
   SessionInfoMap::iterator it = _session_info_map.find (session_id);
   const SessionInfoMap::iterator it_end = _session_info_map.end ();
   assert (it != it_end);
   
   if (it != it_end)
   {
      SessionInfo & info = it->second;
      assert (&session == info._session_ptr);
      
      info._state = SessionInfo::State_RELEASING;
   }
}



/*
==============================================================================
Name : req_shutdown
==============================================================================
*/

void  Central::req_shutdown ()
{
   _shutdown_flag = true;
   
   SessionInfoMap::iterator it = _session_info_map.begin ();
   const SessionInfoMap::iterator it_end = _session_info_map.end ();
   
   for (; it != it_end ; ++it)
   {
      SessionInfo & info = it->second;
      Session * session_ptr = info._session_ptr;
      
      session_ptr->kick_all ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : run_flip
==============================================================================
*/

void  Central::run_flip ()
{
   TRACEo ("Setting up flip listen port");
   
   const Configuration & conf = Configuration::get ();
   
   Connection * cnx_ptr = create_listen (*this, conf._port);
   _flip_cnx_aptr = std::auto_ptr <Connection> (cnx_ptr);
   
   int fd_main = cnx_ptr->get_fd ();
   
   _fd_event_mgr.insert (
      FdEventMgr::Type_READ, fd_main, this, &Central::process_accept_flip
   );
}



/*
==============================================================================
Name : run_http
==============================================================================
*/

void  Central::run_http ()
{
   TRACEo ("Setting up http listen port");
   
   const Configuration & conf = Configuration::get ();
   
   Connection * cnx_ptr = create_listen (*this, conf._service_port);
   _http_cnx_aptr = std::auto_ptr <Connection> (cnx_ptr);
   
   int fd_service = cnx_ptr->get_fd ();
   
   _fd_event_mgr.insert (
      FdEventMgr::Type_READ, fd_service, this, &Central::process_accept_http
   );
}



/*
==============================================================================
Name : run_loop
==============================================================================
*/

void  Central::run_loop ()
{
   TRACEo ("Ready");
   
   // run loop
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      process ();
   }
}



/*
==============================================================================
Name : run_exit
==============================================================================
*/

void  Central::run_exit ()
{
   TRACEo ("Exiting process loop");
   
   int fd_flip = _flip_cnx_aptr->get_fd ();
   _fd_event_mgr.erase (FdEventMgr::Type_READ, fd_flip);

   int fd_http = _http_cnx_aptr->get_fd ();
   _fd_event_mgr.erase (FdEventMgr::Type_READ, fd_http);
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  Central::process ()
{
   double timeout = 0.5;
   
   // read, write fd or timeout
   
   _fd_event_mgr.wait (timeout);
   
   double t0 = _tr.get ();
      
   // at this stage clients have processed packets if any
   
   assert (_flip_cnx_aptr->get_action () != Connection::Action_DELETE);
   
   // terminate clients that have timeout
   
   process_timeout_client ();

   // let the server send its ping
   
   process_timeout_server ();

   // some clients might be in an error state
   
   process_release_client_flip ();
   process_release_client_http ();
   
   // release session which are ready to quit (waitpid should be fast)
   
   process_release_session ();
   process_queued_client_flip ();

   process_waitpid_async ();
   
   //
      
   double process_duration = _tr.get () - t0 + _fd_event_mgr.get_process_time ();
   
   _stats.notify_process_duration (
      &Metrics::_proc_all_max_us, &Metric::_proc_all_max_us, process_duration
   );
   
   _stats.notify_process_duration (
      &Metrics::_proc_fd_max_us, &Metric::_proc_fd_max_us, _fd_event_mgr.get_process_time ()
   );
   
   _stats.post_process ();
   
   process_notify_client_http ();
}



/*
==============================================================================
Name : process_timeout_client
==============================================================================
*/

void  Central::process_timeout_client ()
{
   const double now = _tr.get ();
   const Configuration & conf = Configuration::get ();
   
   ClientFlipSet::iterator it = _client_flip_set.begin ();
   const ClientFlipSet::iterator it_end = _client_flip_set.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientFlip * client_ptr = *it;
      
      double read_ts = client_ptr->get_read_ts ();
      
      if (read_ts + conf._recv_timeout < now)
      {
         _stats.increment_ping_failed ();
         TRACEI1 ("c%1%: ping failed", client_ptr->get_sd ());
         
         client_ptr->terminate ();
      }
   }
   
   _stats.notify_process_duration (
      &Metrics::_proc_toc_max_us, &Metric::_proc_toc_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : process_timeout_server
==============================================================================
*/

void  Central::process_timeout_server ()
{
   const double now = _tr.get ();

   ClientFlipSet::iterator it = _client_flip_set.begin ();
   const ClientFlipSet::iterator it_end = _client_flip_set.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientFlip * client_ptr = *it;
      
      client_ptr->process_ping ();
   }

   _stats.notify_process_duration (
      &Metrics::_proc_tos_max_us, &Metric::_proc_tos_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : process_release_client_flip
==============================================================================
*/

void  Central::process_release_client_flip ()
{
   const double now = _tr.get ();

   ClientFlipSet::iterator it = _client_flip_set.begin ();
   const ClientFlipSet::iterator it_end = _client_flip_set.end ();
   
   for (; it != it_end ;)
   {
      ClientFlipSet::iterator it_next = it;
      ++it_next;
      
      ClientFlip * client_ptr = *it;
      
      if (client_ptr->need_release ())
      {
         {
            ClientFlipTsMap::iterator it = _queued_client_flip_map.find (client_ptr);
         
            if (it != _queued_client_flip_map.end ())
            {
               const std::string & username = client_ptr->get_username ();
               const std::string & session = client_ptr->get_session ();

               TRACEI2 (
                  "%1% unqueued from %2% (disconnected)",
                  username.c_str (), session.c_str ()
               );
               
               _queued_client_flip_map.erase (client_ptr);
            }
         }
         
         delete client_ptr;
         client_ptr = 0;
         
         _client_flip_set.erase (it);
      }
      
      it = it_next;
   }

   _stats.notify_process_duration (
      &Metrics::_proc_recf_max_us, &Metric::_proc_recf_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : process_queued_client_flip
==============================================================================
*/

void  Central::process_queued_client_flip ()
{
   ClientFlipTsMap queued_client_flip_map = _queued_client_flip_map;
   
   ClientFlipTsMap::iterator it = queued_client_flip_map.begin ();
   const ClientFlipTsMap::iterator it_end = queued_client_flip_map.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientFlip * client_ptr = it->first;
      
      bool ok_flag = bind (*client_ptr);

      if (!ok_flag)
      {
         client_ptr->terminate ();
      }
   }
}



/*
==============================================================================
Name : process_release_client_http
==============================================================================
*/

void  Central::process_release_client_http ()
{
   const double now = _tr.get ();

   ClientHttpSet::iterator it = _client_http_set.begin ();
   const ClientHttpSet::iterator it_end = _client_http_set.end ();
   
   for (; it != it_end ;)
   {
      ClientHttpSet::iterator it_next = it;
      ++it_next;
      
      ClientHttp * client_ptr = *it;
      
      if (client_ptr->need_release ())
      {
         delete client_ptr;
         client_ptr = 0;
         
         _client_http_set.erase (it);
      }
      
      it = it_next;
   }

   _stats.notify_process_duration (
      &Metrics::_proc_rech_max_us, &Metric::_proc_rech_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : process_release_session
==============================================================================
*/

void  Central::process_release_session ()
{
   const double now = _tr.get ();

   SessionInfoMap::iterator it = _session_info_map.begin ();
   const SessionInfoMap::iterator it_end = _session_info_map.end ();
   
   for (; it != it_end ;)
   {
      SessionInfoMap::iterator it_next = it;
      ++it_next;
      
      SessionInfo & info = it->second;
      
      if (info._state == SessionInfo::State_RELEASING)
      {
         assert (info._session_ptr->empty ());
         
         delete info._session_ptr;
         
         _session_info_map.erase (it);
      }
      
      it = it_next;
   }
   
   if (_session_info_map.empty () && _shutdown_flag)
   {
      exit (0);
   }

   _stats.notify_process_duration (
      &Metrics::_proc_res_max_us, &Metric::_proc_res_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : process_notify_client_http
==============================================================================
*/

void  Central::process_notify_client_http ()
{
   double now = _tr.get ();
   
   if (now > _stats_ts + 0.25)
   {
      _stats_ts = now;
      
      ClientHttpSet::iterator it = _client_http_set.begin ();
      const ClientHttpSet::iterator it_end = _client_http_set.end ();
      
      for (; it != it_end ; ++it)
      {
         ClientHttp * client_ptr = *it;
         
         client_ptr->push_info (_stats._stats_data, LogMgr::use ());
      }
   }

   _stats.notify_process_duration (
      &Metrics::_proc_nch_max_us, &Metric::_proc_nch_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : process_waitpid_async
==============================================================================
*/

void  Central::process_waitpid_async ()
{
   PidSet::iterator it = _pid_set.begin ();
   const PidSet::iterator it_end = _pid_set.end ();
   
   for (; it != it_end ;)
   {
      PidSet::iterator it_next = it;
      ++it_next;
      
      pid_t pid = *it;
      
      int child = 0;
      int status = 0;
      
      do
      {
         child = waitpid (pid, &status, WNOHANG);
      }
      while ((child == -1) && (errno == EINTR));
      
      if (child != 0)
      {
         assert (child == pid);
         
         _pid_set.erase (it);
      }
      
      it = it_next;
   }
}



/*
==============================================================================
Name : process_accept_flip
==============================================================================
*/

void  Central::process_accept_flip (void * ctx_ptr)
{
   Central * this_ptr = (Central *) ctx_ptr;
   
   this_ptr->process_accept_flip ();
}



/*
==============================================================================
Name : process_accept_flip
==============================================================================
*/

void  Central::process_accept_flip ()
{
   double now = _tr.get ();
   
   assert (_flip_cnx_aptr.get () != 0);
   
   int sd = _flip_cnx_aptr->get_fd ();
   ::sockaddr_in & addr = _flip_cnx_aptr->use_addr ();
   ::socklen_t addr_size = sizeof (addr);

   int asd = ::accept (sd, (::sockaddr *) &addr, &addr_size);
   
   if ((asd >= 0) && (asd < FD_SETSIZE))
   {
      if (_shutdown_flag)
      {
         ::close (asd);
      }
      else
      {
         ClientFlip * client_ptr = new ClientFlip (*this, asd, addr);
         
         _client_flip_set.insert (client_ptr);
      }
   }
   else if (asd >= FD_SETSIZE)
   {
      TRACEX1 ("'accept' returned fd > FD_SETSIZE %1%", asd);
   }
   else
   {
      TRACEX1 ("'accept' failed with error `%1%'", strerror (errno));
   }

   _stats.notify_process_duration (
      &Metrics::_proc_af_max_us, &Metric::_proc_af_max_us, _tr.get () - now
   );
}



/*
==============================================================================
Name : process_accept_http
==============================================================================
*/

void  Central::process_accept_http (void * ctx_ptr)
{
   Central * this_ptr = (Central *) ctx_ptr;
   
   this_ptr->process_accept_http ();
}



/*
==============================================================================
Name : process_accept_http
==============================================================================
*/

void  Central::process_accept_http ()
{
   double now = _tr.get ();
   
   assert (_http_cnx_aptr.get () != 0);
   
   int sd = _http_cnx_aptr->get_fd ();
   ::sockaddr_in & addr = _http_cnx_aptr->use_addr ();
   ::socklen_t addr_size = sizeof (addr);

   int asd = ::accept (sd, (::sockaddr *) &addr, &addr_size);
   
   if ((asd >= 0) && (asd < FD_SETSIZE))
   {
      ClientHttp * client_ptr = new ClientHttp (*this, asd, addr);
      
      _client_http_set.insert (client_ptr);
      
      client_ptr->push_info (_stats._stats_data, LogMgr::use ());
   }
   else if (asd >= FD_SETSIZE)
   {
      TRACEX1 ("'accept' returned fd > FD_SETSIZE %1%", asd);
   }
   else
   {
      TRACEX1 ("'accept' failed with error `%1%'", strerror (errno));
   }

   _stats.notify_process_duration (
      &Metrics::_proc_ah_max_us, &Metric::_proc_ah_max_us, _tr.get () - now
   );
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

