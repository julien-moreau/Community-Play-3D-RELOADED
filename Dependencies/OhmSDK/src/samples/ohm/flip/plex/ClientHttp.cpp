/*****************************************************************************

        ClientHttp.cpp
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

#include "plex/ClientHttp.h"

#include "plex/Central.h"
#include "plex/shared/Configuration.h"
#include "plex/shared/ProtocolOsHttp.h"
#include "plex/FdEventMgr.h"
#include "plex/shared/LogMgr.h"

#include "ohm/archi/def.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/Base64.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include "zlib.h"

#include <sys/wait.h>
#include <resolv.h>
#include <openssl/bio.h>
#include <openssl/hmac.h>
#include <openssl/ssl.h>

#if defined (__linux__)
   #include <mcheck.h>
#endif

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const char *   ClientHttp::_ns_log_0 = "Service Port";

/*
==============================================================================
Name : ctor
==============================================================================
*/

ClientHttp::ClientHttp (Central & central, int sd, const ::sockaddr_in & addr)
:  ClientBase (central, sd)
,  _central (central)
,  _sd (sd)
,  _process_aptr ()
,  _is_ok_flag (true)
,  _read_data ()
{
   std::string ip_str;

   const char * ip_0 = inet_ntoa (addr.sin_addr);
   
   if (ip_0 != 0)
   {
      ip_str = ip_0;
   }
   
   std::string identity = "unknown";
   
   if (ip_str == "127.0.0.1")
   {
      identity = "localhost";
   }
   else if (ip_str == "82.238.201.174")
   {
      identity = "ohmforce.paris";
   }

   TRACEo3 ("c%1%: creating http from ip %2% (%3%)", _sd, ip_str, identity);
   
   const Configuration & conf = Configuration::get ();
   
   std::list <std::string> arg_list;
   arg_list.push_back ("--http");
   conf.make (arg_list);
   
   _process_aptr = std::auto_ptr <Process> (new Process (conf._bin_path, arg_list));

   int fd_read = _process_aptr->get_read_fd ();
   int fd_write = _process_aptr->get_write_fd ();
   
   _buffer_list_aptr = std::auto_ptr <BufferList> (new BufferList (central, fd_write, this, &ClientHttp::send));
   
   FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
   
   evt_mgr.insert (FdEventMgr::Type_READ, fd_read, this, &ClientHttp::receive);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ClientHttp::~ClientHttp ()
{
   try
   {
      int fd_read = _process_aptr->get_read_fd ();
      
      FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
      
      evt_mgr.erase (FdEventMgr::Type_READ, fd_read);
      
      _process_aptr->kill ();

      pid_t pid = _process_aptr->acquire ();

      _central.add_waitpid_async (pid);

      TRACEo1 ("c%1%: http released", _sd);
   }
   catch (...)
   {
      TRACEX1 ("c%1%: exception !", _sd);
      
      assert (false);
   }
}



/*
==============================================================================
Name : use_central
==============================================================================
*/

Central &   ClientHttp::use_central ()
{
   return _central;
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  ClientHttp::receive (void * ctx_ptr)
{
   assert (ctx_ptr != 0);
   
   ClientHttp * this_ptr = (ClientHttp *) ctx_ptr;
   
   this_ptr->receive ();
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  ClientHttp::receive ()
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
         TRACEX3 ("c%1%: p%2%: read failed %3%", _sd, fd, strerror (errno));
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

void  ClientHttp::send (void * ctx_ptr)
{
   assert (ctx_ptr != 0);
   
   ClientHttp * this_ptr = (ClientHttp *) ctx_ptr;
   
   this_ptr->send ();
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  ClientHttp::send ()
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
         TRACEX3 ("c%1%: p%2%: write failed `%3%'", _sd, _process_aptr->get_write_fd (), strerror (errno));
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
Name : push_info
==============================================================================
*/

void  ClientHttp::push_info (const StatsData & stats_data, LogMgr & log_mgr)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data;
   ProtocolOsHttp::write_info (data, stats_data, log_mgr);
   
   _buffer_list_aptr->push (data);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_process
==============================================================================
*/

bool  ClientHttp::do_process (int & err)
{
#if 0
   trace_escaped_ascii ();
#endif
   
   size_t pos = find ("\r\n\r\n");
   
   bool loop_flag = pos != std::string::npos;
   
   if (loop_flag)
   {
      const char * data_ptr = (const char *) get_data_ptr (0, pos);
      
      err = process_request (data_ptr, pos);
      
      if (err != 0)
      {
         loop_flag = false;
      }
      
      pop (pos + 4);
   }
   
   return loop_flag;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : terminate_incident
==============================================================================
*/

void  ClientHttp::terminate_incident ()
{
   TRACEX2 ("c%1%: p%2% oshttp died", _sd, _process_aptr->get_write_fd ());
   
   terminate ();
   
   _is_ok_flag = false;
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  ClientHttp::process ()
{
   bool loop_flag = true;
   
   while (loop_flag)
   {
      if (_read_data.size () >= 4)
      {
         size_t packet_size = ProtocolOsHttp::get_packet_size (_read_data);
         
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

void  ClientHttp::process_packet (const ohm::archi::UByte * data_ptr, size_t packet_size)
{
   using namespace ohm;
   
   util::BinaryStreamInput bsi (data_ptr, packet_size);
   
   // skip packet size
   bsi.advance (4);
   
   ProtocolOsHttp::Plex type = ProtocolOsHttp::Plex_INVALID;
   ProtocolOsHttp::read_type (type, bsi);
   
   switch (type)
   {
   case ProtocolOsHttp::Plex_HTTP_RESPONSE:
      process_http_response (bsi);
      break;

   case ProtocolOsHttp::Plex_RESTART_OSPLEX:
      process_restart_osplex (bsi);
      break;

   case ProtocolOsHttp::Plex_SHUTDOWN_OSPLEX:
      process_shutdown_osplex (bsi);
      break;

   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : process_http_response
==============================================================================
*/

void  ClientHttp::process_http_response (ohm::util::BinaryStreamInput & bsi)
{
   using namespace ohm;
   
   const archi::UByte * data_ptr = 0;
   size_t data_size = 0;
   ProtocolOsHttp::read_http_response (data_ptr, data_size, bsi);
   
   std::vector <archi::UByte> data (data_ptr, data_ptr + data_size);
   
   push (data);
}



/*
==============================================================================
Name : process_restart_osplex
==============================================================================
*/

void  ClientHttp::process_restart_osplex (ohm::util::BinaryStreamInput & bsi)
{
   exit (0);
}



/*
==============================================================================
Name : process_shutdown_osplex
==============================================================================
*/

void  ClientHttp::process_shutdown_osplex (ohm::util::BinaryStreamInput & bsi)
{
   TRACEo1 ("c%1%: shutting down", _sd);
   
   _central.req_shutdown ();
}



/*
==============================================================================
Name : process_request
==============================================================================
*/

int   ClientHttp::process_request (const char * char_arr, size_t size)
{
   int err = 0;
   
   std::string method;
   std::string resource;
   std::string version;
   
   if (err == 0)
   {
      err = process_request_line (method, resource, version, char_arr, size);
   }
   
   if (err == 0)
   {
      if (version != "HTTP/1.1")
      {
         err = -1;
      }
   }
   
   if (err == 0)
   {
      err = dispatch_request (method, resource);
   }
   
   return err;
}



/*
==============================================================================
Name : process_request_line
==============================================================================
*/

int   ClientHttp::process_request_line (std::string & method, std::string & resource, std::string & version, const char * char_arr, size_t size)
{
   /*
   METHOD resource HTTP/1.1\r\n
   Host: server\r\n
   \r\n
   */
   
   enum ReqLine
   {
      ReqLine_METHOD = 0,
      ReqLine_METHOD_SP,
      ReqLine_RESOURCE,
      ReqLine_RESOURCE_SP,
      ReqLine_VERSION,
   };
   
   int err = 0;
   
   ReqLine rl = ReqLine_METHOD;
   
   size_t base = 0;

   for (size_t i = 0 ; i < size ; ++i)
   {
      char c = char_arr [i];
      
      if (c == ' ')
      {
         switch (rl)
         {
         case ReqLine_METHOD:
            method.assign (char_arr + base, i - base);
            rl = ReqLine_METHOD_SP;
            break;

         case ReqLine_RESOURCE:
            resource.assign (char_arr + base, i - base);
            rl = ReqLine_RESOURCE_SP;
            break;
         
         default:
            return -1;
            break;
         }
      }
      else if ((c == '\r') || (c == '\n'))
      {
         if (rl == ReqLine_VERSION)
         {
            version.assign (char_arr + base, i - base);
            break;
         }
         else
         {
            return -1;
         }
      }
      else
      {
         switch (rl)
         {
         case ReqLine_METHOD_SP:
            base = i;
            rl = ReqLine_RESOURCE;
            break;

         case ReqLine_RESOURCE_SP:
            base = i;
            rl = ReqLine_VERSION;
            break;
         
         default:
            //nothing
            break;
         }
      }
   }
   
   return err;
}



/*
==============================================================================
Name : dispatch_request
==============================================================================
*/

int   ClientHttp::dispatch_request (const std::string & method, const std::string & resource)
{
   if (method != "GET")
   {
      return -1;
   }
   
   const Configuration & conf = Configuration::get ();
   
   std::string secret = "/";
   
   int c = resource.compare (0, secret.size (), secret);
   
   if (c != 0)
   {
      return -1;
   }
   
   std::string rel_url = resource.substr (secret.size ());
   
   dispatch_resource (rel_url);
   
   return 0;
}



/*
==============================================================================
Name : dispatch_resource
==============================================================================
*/

int   ClientHttp::dispatch_resource (const std::string & resource)
{
   using namespace ohm;
   
   std::string action = resource;
   std::map <std::string, std::string> args;
   
   size_t pos = resource.find ("?");
   
   if (pos != std::string::npos)
   {
      action = resource.substr (0, pos);
      
      bool loop_flag = true;
      
      while (loop_flag)
      {
         pos += 1;
         
         size_t pos2 = resource.find ("=", pos);
         
         if (pos2 == std::string::npos)
         {
            return -1;
         }
         
         std::string key = resource.substr (pos, pos2 - pos);
         
         pos = pos2 + 1;
         
         pos2 = resource.find ("&", pos);
         
         std::string value;
         
         if (pos2 == std::string::npos)
         {
            value = resource.substr (pos);
            loop_flag = false;
         }
         else
         {
            value = resource.substr (pos, pos2 - pos);
         }
         
         args [key] = value;
      }
   }
   
   std::vector <archi::UByte> data;
   ProtocolOsHttp::write_http_request (data, action, args);
   
   _buffer_list_aptr->push (data);
   
   return 0;
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

