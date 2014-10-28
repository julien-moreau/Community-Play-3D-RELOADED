/*****************************************************************************

        ClientHttp.h
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



#if ! defined (plex_ClientHttp_HEADER_INCLUDED)
#define  plex_ClientHttp_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/ClientBase.h"
#include "plex/Process.h"

#include "ohm/archi/types.h"

#include <vector>

#include <sys/socket.h>
#include <arpa/inet.h>



namespace ohm
{
namespace util
{
class BinaryStreamInput;
}
}


namespace plex
{



class Central;
class FdEventMgr;
class PageBase;
class LogMgr;
class StatsData;

class ClientHttp
:  public ClientBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef PageBase * (*CreateProc) (ClientHttp &);
   
                  ClientHttp (Central & central, int sd, const ::sockaddr_in & addr);
   virtual        ~ClientHttp ();
   
   Central &      use_central ();
   
   static void    receive (void * ctx_ptr);
   void           receive ();

   static void    send (void * ctx_ptr);
   void           send ();

   void           push_info (const StatsData & stats_data, LogMgr & log_mgr);
   
   static const char *
                  _ns_log_0;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // ClientBase
   virtual bool   do_process (int & err);

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         BUF_SIZE = 4096   };

   typedef std::map <std::string, CreateProc>   ActionCreateProcMap;
   
   void           terminate_incident ();

   void           process ();
   void           process_packet (const ohm::archi::UByte * data_ptr, size_t packet_size);
   void           process_http_response (ohm::util::BinaryStreamInput & bsi);
   void           process_restart_osplex (ohm::util::BinaryStreamInput & bsi);
   void           process_shutdown_osplex (ohm::util::BinaryStreamInput & bsi);
   
   int            process_request (const char * char_arr, size_t size);
   int            process_request_line (std::string & method, std::string & resource, std::string & version, const char * char_arr, size_t size);

   int            dispatch_request (const std::string & method, const std::string & resource);
   int            dispatch_resource (const std::string & resource);
   
   Central &      _central;
   const int      _sd;
   
   std::auto_ptr <Process>
                  _process_aptr;
   bool           _is_ok_flag;
   std::vector <ohm::archi::UByte>
                  _read_data;
   std::auto_ptr <BufferList>
                  _buffer_list_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClientHttp ();
                  ClientHttp (const ClientHttp &other);
   ClientHttp &   operator = (const ClientHttp &other);
   bool           operator == (const ClientHttp &other);
   bool           operator != (const ClientHttp &other);

}; // class ClientHttp



}  // namespace plex



//#include  "ClientHttp.hpp"



#endif   // plex_ClientHttp_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

