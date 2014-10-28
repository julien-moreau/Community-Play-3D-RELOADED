/*****************************************************************************

        Session.cpp
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

#include "plex/Session.h"

#include "plex/Central.h"
#include "plex/ClientFlip.h"
#include "plex/shared/Configuration.h"
#include "plex/ProtocolFlip.h"
#include "plex/shared/ProtocolOsSess.h"
#include "plex/Stats.h"
#include "plex/fnc.h"
#include "plex/shared/LogMgr.h"

#include "ohm/sys/TimerReliable.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <sys/wait.h>
#include <resolv.h>

#include <cassert>



#define SKELETON_SERVER_SESSION_DEBUG_FLAG

#if defined (SKELETON_SERVER_SESSION_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error SKELETON_SERVER_SESSION_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define sess_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
   #define sess_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define sess_TRACE2(fmt_0, arg1, arg2) ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define sess_TRACE3(fmt_0, arg1, arg2, arg3) ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define sess_TRACE4(fmt_0, arg1, arg2, arg3, arg4) ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define sess_TRACE(msg_0)  (void (0))
   #define sess_TRACE1(fmt_0, arg1) (void (0))
   #define sess_TRACE2(fmt_0, arg1, arg2) (void (0))
   #define sess_TRACE3(fmt_0, arg1, arg2, arg3) (void (0))
   #define sess_TRACE4(fmt_0, arg1, arg2, arg3, arg4) (void (0))
#endif



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const char *   Session::_ns_log_0 = "Session";

/*
==============================================================================
Name : ctor
==============================================================================
*/

Session::Session (Central & central, const std::string & bin_path, const std::string & session)
:  _central (central)
,  _stats (central.use_stats ())
,  _session (session)
,  _bin (bin_path.substr (bin_path.rfind ("/") + 1))
,  _process_aptr ()
,  _client_map ()
,  _is_ok_flag (true)
,  _read_data ()
,  _buffer_list_aptr ()
,  _loaded_flag (false)
{
   TRACEo2 ("s%1%: connecting %2%", _session, _bin);
   
   _stats.session_add (_session);
   
   std::list <std::string> arg_list;
   make_args (arg_list, session);
   
   _process_aptr = std::auto_ptr <Process> (new Process (bin_path, arg_list));
   
   int fd_read = _process_aptr->get_read_fd ();
   int fd_write = _process_aptr->get_write_fd ();
   
   _buffer_list_aptr = std::auto_ptr <BufferList> (new BufferList (central, fd_write, this, &Session::send));
   
   FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
   
   evt_mgr.insert (FdEventMgr::Type_READ, fd_read, this, &Session::receive);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Session::~Session ()
{
   try
   {
      _stats.session_remove (_session);
      
      int fd_read = _process_aptr->get_read_fd ();
      
      FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
      
      evt_mgr.erase (FdEventMgr::Type_READ, fd_read);
      
      pid_t pid = _process_aptr->acquire ();

      _central.add_waitpid_async (pid);

      TRACEo1 ("s%1%: disconnected", _session);
   }
   catch (...)
   {
      TRACEX1 ("s%1%: exception !", _session);
      
      assert (false);
   }
}



/*
==============================================================================
Name : get_session
==============================================================================
*/

const std::string &  Session::get_session () const
{
   return _session;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  Session::bind (ClientFlip & client)
{
   const std::string & username = client.get_username ();
   
   TRACEo2 ("s%1%: %2% binded", _session, username);
   
   assert (&client != 0);
   assert (_client_map.find (username) == _client_map.end ());
   
   _client_map [username] = &client;
   
   _stats.session_add_client (_session, username);
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  Session::unbind (ClientFlip & client)
{
   const std::string username = client.get_username ();
   
   TRACEo2 ("s%1%: %2% unbinded", _session, username);
   
   assert (&client != 0);
   
   ClientMap::iterator it = _client_map.find (username);
   
   assert (it != _client_map.end ());
   
   _client_map.erase (it);
   
   push_disconnected (username);
   
   _stats.session_remove_client (_session, username);
   
   if (empty ())
   {
      if (_is_ok_flag)
      {
         push_stop ();
      }
      else
      {
         _central.req_terminate (*this);
      }
   }
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  Session::empty () const
{
   return _client_map.empty ();
}



/*
==============================================================================
Name : kick_all
==============================================================================
*/

void  Session::kick_all ()
{
   ClientMap::iterator it = _client_map.begin ();
   const ClientMap::iterator it_end = _client_map.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientFlip * client_ptr = it->second;
      
      TRACEI2 ("s%1%: %2% kicked", _session.c_str (), client_ptr->get_username ());
      
      client_ptr->terminate ();
   }
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  Session::receive (void * ctx_ptr)
{
   assert (ctx_ptr != 0);
   
   Session * this_ptr = (Session *) ctx_ptr;
   
   this_ptr->receive ();
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  Session::receive ()
{
   using namespace ohm;
   
   if (!_is_ok_flag)
   {
      return;  // abort
   }
   
   archi::UByte buf [BUF_SIZE];
   
   int fd = _process_aptr->get_read_fd ();
   
   ssize_t nbr_read = ::read (fd, buf, BUF_SIZE);

   bool terminate_flag = false;
   
   if (nbr_read < 0)
   {
      switch (errno)
      {
      case EAGAIN:
      case EINTR:
         // nothing
         break;
      
      default:
         TRACEX3 ("s%1%: p%2%: read failed %3%", _session.c_str (), fd, strerror (errno));
         terminate_flag = true;
         break;
      }
   }
   else if (nbr_read == 0)
   {
      terminate_flag = true;
   }
   else
   {
      _read_data.insert (_read_data.end (), buf, buf + nbr_read);
      
      process ();
   }
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  Session::send (void * ctx_ptr)
{
   assert (ctx_ptr != 0);
   
   Session * this_ptr = (Session *) ctx_ptr;
   
   this_ptr->send ();
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  Session::send ()
{
   using namespace ohm;
   
   if (!_is_ok_flag)
   {
      return;  // abort
   }
   
   if (_buffer_list_aptr->empty ())
   {
      return;  // nothing to write
   }
   
   const archi::UByte * data_ptr = _buffer_list_aptr->get_data_ptr ();
   size_t data_size = _buffer_list_aptr->get_data_size ();
   
   ssize_t nbr_write = _process_aptr->write (data_ptr, data_size);
   
   bool terminate_flag = false;
   
   if (nbr_write < 0)
   {
      switch (errno)
      {
      case EAGAIN:
      case EINTR:
         // nothing
         break;

      case EPIPE:
         // process died
         terminate_flag = true;
         break;
      
      default:
         TRACEX3 ("s%1%: p%2%: write failed `%3%'", _session.c_str (), _process_aptr->get_write_fd (), strerror (errno));
         terminate_flag = true;
         break;
      }
   }
   else
   {
      _buffer_list_aptr->pop (nbr_write);
   }
   
   if (terminate_flag)
   {
      terminate_incident ();
   }
}



/*
==============================================================================
Name : push_tx
==============================================================================
*/

void  Session::push_tx (const std::string & username, std::vector <ohm::archi::UByte> & data)
{
   //TRACEo2 ("s%1%: push_tx %2%", _session, username);
   
   if (data.empty ())
   {
      return;  // abort
   }
   
   _stats.session_increment_nbr_tx (_session);
   
   _buf.clear ();
   ProtocolOsSess::write_tx (_buf, username, &data [0], data.size ());
   push (_buf);
}



/*
==============================================================================
Name : push_flush
==============================================================================
*/

void  Session::push_flush ()
{
   TRACEo1 ("s%1%: flush", _session);
   
   _buf.clear ();
   ProtocolOsSess::write_flush (_buf);
   push (_buf);
}



/*
==============================================================================
Name : push_stop
==============================================================================
*/

void  Session::push_stop ()
{
   TRACEo1 ("s%1%: stop", _session);
   
   _stats.session_set_state_end (_session);
   
   _buf.clear ();
   ProtocolOsSess::write_stop (_buf);
   push (_buf);
}



/*
==============================================================================
Name : need_release
==============================================================================
*/

bool  Session::need_release () const
{
   return !_is_ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : terminate_incident
==============================================================================
*/

void  Session::terminate_incident ()
{
   TRACEX3 ("s%1%: p%2% %3% died", _session.c_str (), _process_aptr->get_write_fd (), _bin.c_str ());
      
   kick_all ();
   
   _is_ok_flag = false;
}



/*
==============================================================================
Name : make_args
==============================================================================
*/

void  Session::make_args (std::list <std::string> & arg_list, const std::string & session)
{
   const Configuration & conf = Configuration::get ();
   
   arg_list.push_back ("--sess");
   arg_list.push_back ("--session");
   arg_list.push_back (session);
   
   conf.make (arg_list);
}



/*
==============================================================================
Name : push_disconnected
==============================================================================
*/

void  Session::push_disconnected (const std::string & username)
{
   TRACEo2 ("s%1%: %2% disconnected", _session, username);
   
   _buf.clear ();
   ProtocolOsSess::write_disconnected (_buf, username);
   push (_buf);
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  Session::push (std::vector <ohm::archi::UByte> & data)
{
   _buffer_list_aptr->push (data);
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  Session::process ()
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      if (_read_data.size () >= 4)
      {
         size_t packet_size = ProtocolOsSess::get_packet_size (_read_data);
         
         if (_read_data.size () >= packet_size)
         {
            // packet is ready
            
            process_packet (&_read_data [0], packet_size);
            
            _read_data.erase (_read_data.begin (), _read_data.begin () + packet_size);
         }
         else
         {
            // not enough data for now

            loop_flag = false;
         }
      }
      else
      {
         loop_flag = false;
      }
   }
}



/*
==============================================================================
Name : process_packet
==============================================================================
*/

void  Session::process_packet (const ohm::archi::UByte * data_ptr, size_t packet_size)
{
   using namespace ohm;
   
   util::BinaryStreamInput bsi (data_ptr, packet_size);
   
   // skip packet size
   bsi.advance (4);
   
   ProtocolOsSess::Plex type = ProtocolOsSess::Plex_INVALID;
   ProtocolOsSess::read_type (type, bsi);
   
   switch (type)
   {
   case ProtocolOsSess::Plex_SEND:
      process_send (bsi);
      break;

   case ProtocolOsSess::Plex_BROADCAST:
      process_broadcast (bsi);
      break;

   case ProtocolOsSess::Plex_BROADCAST_EXCEPT:
      process_broadcast_except (bsi);
      break;
   
   case ProtocolOsSess::Plex_STOPPING:
      process_stopping (bsi);
      break;
   
   case ProtocolOsSess::Plex_LOG_LINE:
      process_log_line (bsi);
      break;
   
   case ProtocolOsSess::Plex_PROC_STAT:
      process_proc_stat (bsi);
      break;
   
   case ProtocolOsSess::Plex_STORAGE_IDLE:
      process_storage_idle (bsi);
      break;
   
   case ProtocolOsSess::Plex_STORAGE_READING:
      process_storage_reading (bsi);
      break;
   
   case ProtocolOsSess::Plex_STORAGE_WRITING:
      process_storage_writing (bsi);
      break;
   
   case ProtocolOsSess::Plex_STORAGE_COPYING:
      process_storage_copying (bsi);
      break;
   
   case ProtocolOsSess::Plex_KICK_ALL:
      process_kick_all (bsi);
      break;
   
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : process_send
==============================================================================
*/

void  Session::process_send (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   if (!_loaded_flag)
   {
      _stats.session_set_state_run (_session);
      
      _loaded_flag = true;
   }
   
   std::string username;
   size_t data_size = 0;
   const archi::UByte * data_ptr = 0;
   
   ProtocolOsSess::read_send (username, data_ptr, data_size, bsi);
   
   std::vector <archi::UByte> data (data_ptr, data_ptr + data_size);

   make_tx (data);
   
   ClientMap::iterator it = _client_map.find (username);
   
   if (it != _client_map.end ())
   {
      ClientFlip * client_ptr = it->second;
      
      client_ptr->push (data);
   }
}



/*
==============================================================================
Name : process_broadcast
==============================================================================
*/

void  Session::process_broadcast (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   size_t data_size = 0;
   const archi::UByte * data_ptr = 0;
   
   ProtocolOsSess::read_broadcast (data_ptr, data_size, bsi);
   
   std::vector <archi::UByte> data (data_ptr, data_ptr + data_size);
   
   make_tx (data);
   
   ClientMap::iterator it = _client_map.begin ();
   const ClientMap::iterator it_end = _client_map.end ();
   
   for (; it != it_end ; ++it)
   {
      ClientFlip * client_ptr = it->second;
      
      std::vector <archi::UByte> sub_data (data);
      
      client_ptr->push (sub_data);
   }
}



/*
==============================================================================
Name : process_broadcast_except
==============================================================================
*/

void  Session::process_broadcast_except (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   std::string username;
   size_t data_size = 0;
   const archi::UByte * data_ptr = 0;
   
   ProtocolOsSess::read_broadcast_except (username, data_ptr, data_size, bsi);
   
   std::vector <archi::UByte> data (data_ptr, data_ptr + data_size);
   
   make_tx (data);
   
   ClientMap::iterator it = _client_map.begin ();
   const ClientMap::iterator it_end = _client_map.end ();
   
   for (; it != it_end ; ++it)
   {
      const std::string & sub_username = it->first;
      ClientFlip * client_ptr = it->second;
      
      if (sub_username != username)
      {
         std::vector <archi::UByte> sub_data (data);
         
         client_ptr->push (sub_data);
      }
   }
}



/*
==============================================================================
Name : process_stopping
==============================================================================
*/

void  Session::process_stopping (ohm::util::BinaryStreamInput & bsi)
{
   _central.req_terminate (*this);
}



/*
==============================================================================
Name : process_log_line
==============================================================================
*/

void  Session::process_log_line (ohm::util::BinaryStreamInput & bsi)
{
   std::string ns;
   LogLine log_line;
   
   ProtocolOsSess::read_log_line (ns, log_line, bsi);
   
   //#error to do
}



/*
==============================================================================
Name : process_proc_stat
==============================================================================
*/

void  Session::process_proc_stat (ohm::util::BinaryStreamInput & bsi)
{
   size_t mem_size = 0;
   size_t cpu = 0;
   
   ProtocolOsSess::read_proc_stat (mem_size, cpu, bsi);
   
   _stats.session_set_proc_stat (_session, mem_size, cpu);
}



/*
==============================================================================
Name : process_storage_idle
==============================================================================
*/

void  Session::process_storage_idle (ohm::util::BinaryStreamInput & bsi)
{
   _stats.session_set_storage_idle (_session);
}



/*
==============================================================================
Name : process_storage_reading
==============================================================================
*/

void  Session::process_storage_reading (ohm::util::BinaryStreamInput & bsi)
{
   size_t cur = 0;
   
   ProtocolOsSess::read_storage_reading (cur, bsi);
   
   _stats.session_set_storage_reading (_session, cur);
}



/*
==============================================================================
Name : process_storage_writing
==============================================================================
*/

void  Session::process_storage_writing (ohm::util::BinaryStreamInput & bsi)
{
   size_t cur = 0;
   size_t total = 0;
   
   ProtocolOsSess::read_storage_writing (cur, total, bsi);
   
   _stats.session_set_storage_writing (_session, cur, total);
}



/*
==============================================================================
Name : process_storage_copying
==============================================================================
*/

void  Session::process_storage_copying (ohm::util::BinaryStreamInput & bsi)
{
   _stats.session_set_storage_copying (_session);
}



/*
==============================================================================
Name : process_kick_all
==============================================================================
*/

void  Session::process_kick_all (ohm::util::BinaryStreamInput & bsi)
{
   kick_all ();
}



/*
==============================================================================
Name : make_tx
==============================================================================
*/

void  Session::make_tx (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   archi::UByte buf [TX_HEADER_SIZE];
   
   make_tx_header (buf, data.size ());
   
   data.insert (data.begin (), buf, buf + TX_HEADER_SIZE);
}



/*
==============================================================================
Name : make_tx_header
==============================================================================
*/

void  Session::make_tx_header (ohm::archi::UByte * data_ptr, ohm::archi::UInt32 data_size)
{
   // TX_HEADER_SIZE = 20, calculated from here. If needed change header
   
   ProtocolFlip::write_uint32 (&data_ptr [0], data_size + 16);
   ProtocolFlip::write_uint32 (&data_ptr [4], ProtocolFlip::Fcc_TX);
   ProtocolFlip::write_uint32 (&data_ptr [8], data_size + 16);
   ProtocolFlip::write_uint32 (&data_ptr [12], ProtocolFlip::Fcc_DATA);
   ProtocolFlip::write_uint32 (&data_ptr [16], data_size + 8);
}



std::vector <ohm::archi::UByte>  Session::_buf;



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

