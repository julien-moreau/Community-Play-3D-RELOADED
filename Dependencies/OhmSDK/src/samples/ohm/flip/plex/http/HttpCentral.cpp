/*****************************************************************************

        HttpCentral.cpp
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

#include "plex/http/HttpCentral.h"

#include "plex/http/PageState.h"
#include "plex/http/PageLogs.h"
#include "plex/http/PageSessions.h"
#include "plex/http/PageRestart.h"
#include "plex/http/HttpStats.h"
#include "plex/shared/Configuration.h"
#include "plex/shared/ProtocolOsHttp.h"
#include "plex/shared/LogMgr.h"

#include "ohm/sys/TimerReliable.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"
#include "ohm/util/BinaryStreamOutputNa.h"
#include "ohm/util/trace.h"

#include "zlib.h"

#include <stdexcept>

#include <sys/wait.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

HttpCentral::HttpCentral ()
{
   // ignore SIGPIPE : write will return EPIPE
   ::signal (SIGPIPE, SIG_IGN);
   
   add ("status", PageState::create);
   add ("logs", PageLogs::create);
   add ("sessions", PageSessions::create);
   add ("restart", PageRestart::create);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

HttpCentral::~HttpCentral ()
{
   try
   {
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : run
Description :
   Start listening socket for connections. Each time a connection is made,
   the connection is checked and a new 'Client' is builded.
==============================================================================
*/

void  HttpCentral::run ()
{
   // run loop
   
   _run_flag = true;
   
   while (_run_flag)
   {
      process ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : add
==============================================================================
*/

void  HttpCentral::add (const std::string & action, CreateProc create_proc)
{
   assert (!action.empty ());
   assert (create_proc != 0);
   
   _action_html_map [action] = create_proc;
   _action_xhr_map [action + ".xhr"] = create_proc;
}



/*
==============================================================================
Name : send_msg
==============================================================================
*/

void  HttpCentral::send_msg (const std::vector <ohm::archi::UByte> & data)
{
   bool ok_flag = write (&data [0], data.size ());
   
   if (!ok_flag)
   {
      _run_flag = false;
   }
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  HttpCentral::process ()
{
   bool ok_flag = process_command ();
   
   if (!ok_flag)
   {
      _run_flag = false;
   }
}



/*
==============================================================================
Name : process_command
==============================================================================
*/

bool  HttpCentral::process_command ()
{
   using namespace ohm;
   
   archi::UByte buf [4];
   
   bool ok_flag = true;
   
   if (ok_flag)
   {
      ok_flag = read (buf, 4);
   }
   
   archi::UInt32 packet_size = 0;
   
   if (ok_flag)
   {
      util::BinaryStreamInput bsi (buf, 4);
      
      bsi.read (packet_size);
      
      ok_flag &= packet_size >= 4;
   }

   std::vector <archi::UByte> data;

   if (ok_flag)
   {
      size_t size = packet_size - 4;
      
      data.resize (size);
         
      ok_flag = read (&data [0], size);
   }
   
   if (ok_flag && !data.empty ())
   {
      util::BinaryStreamInput bsi (&data [0], data.size ());
      
      ProtocolOsHttp::Http type = ProtocolOsHttp::Http_INVALID;
      ProtocolOsHttp::read_type (type, bsi);
      
      switch (type)
      {
      case ProtocolOsHttp::Http_HTTP_REQUEST:
         process_http_request (bsi);
         break;

      case ProtocolOsHttp::Http_INFO:
         process_info (bsi);
         break;

      default:
         assert (false);
         break;
      }
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : process_http_request
==============================================================================
*/

void  HttpCentral::process_http_request (ohm::util::BinaryStreamInput & bsi)
{
   std::string action;
   std::map <std::string, std::string> args;
   ProtocolOsHttp::read_http_request (action, args, bsi);
   
   bool continue_flag = true;
   
   process_http_request_html (continue_flag, action, args);
   process_http_request_xhr (continue_flag, action, args);
   process_http_request_restart (continue_flag, action, args);
   process_http_request_shutdown (continue_flag, action, args);
   
   if (continue_flag)
   {
      send_404 ();
   }
}



/*
==============================================================================
Name : process_info
==============================================================================
*/

void  HttpCentral::process_info (ohm::util::BinaryStreamInput & bsi)
{
   LogMgr & log_mgr = LogMgr::use ();
   StatsData & stats_data = HttpStats::use_instance ()._stats_data;

   ProtocolOsHttp::read_info (stats_data, log_mgr, bsi);
}



/*
==============================================================================
Name : process_http_request_html
==============================================================================
*/

void  HttpCentral::process_http_request_html (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args)
{
   if (!continue_flag) return;
   
   ActionCreateProcMap::iterator it = _action_html_map.find (action);
   
   if (it != _action_html_map.end ())
   {
      CreateProc create_proc = it->second;
      
      process_html (action, create_proc, args);
      
      continue_flag = false;
   }
}



/*
==============================================================================
Name : process_http_request_xhr
==============================================================================
*/

void  HttpCentral::process_http_request_xhr (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args)
{
   if (!continue_flag) return;
   
   ActionCreateProcMap::iterator it = _action_xhr_map.find (action);
   
   if (it != _action_xhr_map.end ())
   {
      CreateProc create_proc = it->second;
      
      process_xhr (action, create_proc, args);
      
      continue_flag = false;
   }
}



/*
==============================================================================
Name : process_http_request_restart
==============================================================================
*/

void  HttpCentral::process_http_request_restart (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args)
{
   using namespace ohm;
   
   if (!continue_flag) return;
   if (action != "restart.do") return;
   continue_flag = false;
   
   std::vector <archi::UByte> data;
   ProtocolOsHttp::write_restart_osplex (data);
   
   send_msg (data);
   
   static const std::string body ("restarting");

   send_compress (body.c_str (), body.size ());
}



/*
==============================================================================
Name : process_http_request_shutdown
==============================================================================
*/

void  HttpCentral::process_http_request_shutdown (bool & continue_flag, const std::string & action, const std::map <std::string, std::string> & args)
{
   using namespace ohm;
   
   if (!continue_flag) return;
   if (action != "shutdown.do") return;
   continue_flag = false;
   
   std::vector <archi::UByte> data;
   ProtocolOsHttp::write_shutdown_osplex (data);
   
   send_msg (data);
   
   static const std::string body ("shutting down");

   send_compress (body.c_str (), body.size ());
}



/*
==============================================================================
Name : process_html
==============================================================================
*/

int   HttpCentral::process_html (const std::string & action, CreateProc create_proc, const std::map <std::string, std::string> & args)
{
   std::auto_ptr <PageBase> page_aptr (create_proc (*this));
         
   page_aptr->init (action, args);
   page_aptr->process_html ();
   
   const std::string & txt = page_aptr->get_html ();
   
   send_compress (txt.c_str (), txt.size ());
   
   return 0;
}



/*
==============================================================================
Name : process_xhr
==============================================================================
*/

int   HttpCentral::process_xhr (const std::string & action, CreateProc create_proc, const std::map <std::string, std::string> & args)
{
   using namespace ohm;
   
   std::auto_ptr <PageBase> page_aptr (create_proc (*this));
         
   page_aptr->init (action, args);
   page_aptr->process_xhr ();
   
   time_t now = time (0);
   txt::out::Print printer;
   printer.print ("%1%").arg (now);
   const std::string & ts_str = printer;
   
   std::string txt = page_aptr->get_js ();
   txt += "timestamp=" + ts_str + ";";
   
   send_compress (txt.c_str (), txt.size ());
   
   return 0;
}



/*
==============================================================================
Name : send_compress
==============================================================================
*/

void  HttpCentral::send_compress (const char * str, size_t size)
{
   using namespace ohm;
   
   std::vector <archi::UByte> comp_data;
   
   comp_data.resize (::compressBound (size));
   ::uLongf dest_len = comp_data.size ();
   
   int err = ::compress2 (
      &comp_data [0], &dest_len,
      (const archi::UByte *) str, size,
      9
   );
   assert (err == Z_OK);
   
   comp_data.resize (dest_len);
   
   txt::out::Print printer;
   printer.print (
      "HTTP/1.1 200 OK\r\n"
      "Server: osplex\r\n"
      "Content-Length: %1%\r\n"
      "Content-Type: text/html; charset=utf-8\r\n"
      "Content-Encoding: deflate\r\n"
      "Connection: keep-alive\r\n"
      "\r\n"
   ).arg (dest_len);
   
   const std::string & header (printer);
   
   std::vector <archi::UByte> response;
   response.insert (response.end (), header.begin (), header.end ());
   response.insert (response.end (), comp_data.begin (), comp_data.end ());
   
   std::vector <archi::UByte> data;
   ProtocolOsHttp::write_http_response (data, response);
   
   send_msg (data);
}



/*
==============================================================================
Name : send_404
==============================================================================
*/

void  HttpCentral::send_404 ()
{
   using namespace ohm;
   
   static const std::string body (
      "<!DOCTYPE html>\r\n"
      "<html lang=en>\r\n"
      "<meta charset=utf-8>\r\n"
      "<title>Error 404 Not Found</title>\r\n"
      "<h1>404 Not Found</h1>\r\n"
      "<p>The requested URL was not found on this server.</p>\r\n"
   );
   
   std::vector <archi::UByte> comp_data;
   
   comp_data.resize (::compressBound (body.size ()));
   ::uLongf dest_len = comp_data.size ();
   
   int err = ::compress2 (
      &comp_data [0], &dest_len,
      (const archi::UByte *) body.c_str (), body.size (),
      9
   );
   assert (err == Z_OK);
   
   comp_data.resize (dest_len);
   
   txt::out::Print printer;
   printer.print (
      "HTTP/1.1 404 Not Found\r\n"
      "Server: osplex\r\n"
      "Content-Length: %1%\r\n"
      "Content-Type: text/html; charset=utf-8\r\n"
      "Content-Encoding: deflate\r\n"
      "Connection: close\r\n"
      "\r\n"
   ).arg (dest_len);
   
   const std::string & header (printer);
   
   std::vector <archi::UByte> response;
   response.insert (response.end (), header.begin (), header.end ());
   response.insert (response.end (), comp_data.begin (), comp_data.end ());
   
   std::vector <archi::UByte> data;
   ProtocolOsHttp::write_http_response (data, response);
   
   send_msg (data);
}



/*
==============================================================================
Name : read
==============================================================================
*/

bool  HttpCentral::read (ohm::archi::UByte * data_ptr, size_t data_size)
{
   size_t pos = 0;
   
   do 
   {
      int nbr_bytes = ::read (STDIN_FILENO, &data_ptr [pos], data_size - pos);
      
      if (nbr_bytes < 0)
      {
         if (errno == EINTR)
         {
            nbr_bytes == 0;
         }
         else
         {
            return false;
         }
      }
      else if (nbr_bytes == 0)
      {
         return false;
      }
      
      pos += nbr_bytes;
   } 
   while (pos < data_size);
   
   return true;
}



/*
==============================================================================
Name : write
==============================================================================
*/

bool  HttpCentral::write (const std::vector <ohm::archi::UByte> & data)
{
   if (data.empty ())
   {
      return true;
   }
   
   return write (&data [0], data.size ());
}



/*
==============================================================================
Name : write
==============================================================================
*/

bool  HttpCentral::write (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   size_t pos = 0;
   
   do 
   {
      int nbr_bytes = ::write (STDOUT_FILENO, &data_ptr [pos], data_size - pos);
      
      if (nbr_bytes < 0)
      {
         if (errno == EINTR)
         {
            nbr_bytes == 0;
         }
         else
         {
            return false;
         }
      }
      else if (nbr_bytes == 0)
      {
         return false;
      }
      
      pos += nbr_bytes;
   } 
   while (pos < data_size);
   
   return true;
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

