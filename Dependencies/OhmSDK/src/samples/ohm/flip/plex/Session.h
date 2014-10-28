/*****************************************************************************

        Session.h
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



#if ! defined (plex_Session_HEADER_INCLUDED)
#define  plex_Session_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/BufferList.h"
#include "plex/Process.h"

#include <set>

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
class ClientFlip;
class Stats;

class Session
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Session (Central & central, const std::string & bin_path, const std::string & session);
   virtual        ~Session ();
   
   const std::string &
                  get_session () const;
   
   void           bind (ClientFlip & client);
   void           unbind (ClientFlip & client);
   bool           empty () const;
   void           kick_all ();

   static void    receive (void * ctx_ptr);
   void           receive ();

   static void    send (void * ctx_ptr);
   void           send ();

   void           push_tx (const std::string & username, std::vector <ohm::archi::UByte> & data);
   void           push_flush ();
   void           push_stop ();
   
   bool           need_release () const;
   
   static const char *
                  _ns_log_0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         BUF_SIZE = 4096   };
   enum {         TX_HEADER_SIZE = 20  };
   
   typedef std::map <std::string, ClientFlip *> ClientMap;
   
   void           terminate_incident ();
   
   void           make_args (std::list <std::string> & arg_list, const std::string & session);
   
   void           push_disconnected (const std::string & username);
   void           push (std::vector <ohm::archi::UByte> & data);
   void           process ();
   void           process_packet (const ohm::archi::UByte * data_ptr, size_t packet_size);
   void           process_send (ohm::util::BinaryStreamInput & bsi);
   void           process_broadcast (ohm::util::BinaryStreamInput & bsi);
   void           process_broadcast_except (ohm::util::BinaryStreamInput & bsi);
   void           process_stopping (ohm::util::BinaryStreamInput & bsi);
   void           process_log_line (ohm::util::BinaryStreamInput & bsi);
   void           process_proc_stat (ohm::util::BinaryStreamInput & bsi);
   void           process_storage_idle (ohm::util::BinaryStreamInput & bsi);
   void           process_storage_reading (ohm::util::BinaryStreamInput & bsi);
   void           process_storage_writing (ohm::util::BinaryStreamInput & bsi);
   void           process_storage_copying (ohm::util::BinaryStreamInput & bsi);
   void           process_kick_all (ohm::util::BinaryStreamInput & bsi);

   void           make_tx (std::vector <ohm::archi::UByte> & data);
   void           make_tx_header (ohm::archi::UByte * data_ptr, ohm::archi::UInt32 data_size);
   
   Central &      _central;
   Stats &        _stats;
   const std::string
                  _session;
   const std::string
                  _bin;

   std::auto_ptr <Process>
                  _process_aptr;
   
   ClientMap      _client_map;
   
   bool           _is_ok_flag;
   std::vector <ohm::archi::UByte>
                  _read_data;
   std::auto_ptr <BufferList>
                  _buffer_list_aptr;
   
   bool           _loaded_flag;
   
   static std::vector <ohm::archi::UByte>
                  _buf;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Session ();
                  Session (const Session &other);
   Session &      operator = (const Session &other);
   bool           operator == (const Session &other);
   bool           operator != (const Session &other);

}; // class Session



}  // namespace plex



//#include  "Session.hpp"



#endif   // plex_Session_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

