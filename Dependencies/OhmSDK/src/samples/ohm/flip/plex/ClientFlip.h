/*****************************************************************************

        ClientFlip.h
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



#if ! defined (plex_ClientFlip_HEADER_INCLUDED)
#define  plex_ClientFlip_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/ClientBase.h"

#include "ohm/archi/types.h"

#include <vector>

#include <sys/socket.h>
#include <arpa/inet.h>



namespace plex
{



class Central;
class Session;
class Stats;

class ClientFlip
:  public ClientBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    init_static ();
   
                  ClientFlip (Central & central, int sd, const ::sockaddr_in & addr);
   virtual        ~ClientFlip ();
   
   void           bind (Session & session);
   
   void           process_ping ();
   
   int            get_sd () const;
   const std::string &
                  get_username () const;
   const std::string &
                  get_session () const;
   const std::string &
                  get_format_version () const;

   static const char *
                  _ns_log_0;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   // ClientBase
   virtual bool   do_process (int & err);

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum State
   {
                  State_CONNECTED = 0,
                  State_AUTHENTIFIED,
   };
   
   int            process_packet (const ohm::archi::UByte * data_ptr, size_t packet_size);
   
   int            process_cxrq (const ohm::archi::UByte * data_ptr, size_t packet_size);
   int            process_cxrq ();

   int            process_tx (const ohm::archi::UByte * data_ptr, size_t packet_size);
   
   int            process_check_cxrq (const ohm::archi::UByte * & data_ptr, size_t & packet_size);
   int            process_check_serv (const ohm::archi::UByte * & data_ptr, size_t & packet_size);
   int            process_check_vers (const ohm::archi::UByte * & data_ptr, size_t & packet_size);

   bool           check_secret ();
   
   static void    push_uint32 (std::vector <ohm::archi::UByte> & data, ohm::archi::UInt32 x);
   
   Central &      _central;
   Stats &        _stats;
   const int      _sd;
   
   State          _state;
   
   std::string    _session;
   std::string    _username;
   std::string    _secret;
   std::string    _format_version;
   
   Session *      _session_ptr;
   
   double         _to_client_ping_due_time;
   
   std::list <std::vector <ohm::archi::UByte> >
                  _latent_tx_data_list;
   
   static std::vector <ohm::archi::UByte>
                  _ping_data;
   static std::vector <ohm::archi::UByte>
                  _cxak_data;
   static std::vector <ohm::archi::UByte>
                  _cxrf_data;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClientFlip ();
                  ClientFlip (const ClientFlip &other);
   ClientFlip &         operator = (const ClientFlip &other);
   bool           operator == (const ClientFlip &other);
   bool           operator != (const ClientFlip &other);

}; // class ClientFlip



}  // namespace plex



//#include  "ClientFlip.hpp"



#endif   // plex_ClientFlip_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

