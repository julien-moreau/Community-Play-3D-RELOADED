/*****************************************************************************

        ProtocolOsHttp.h
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



#if ! defined (plex_ProtocolOsHttp_HEADER_INCLUDED)
#define  plex_ProtocolOsHttp_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/shared/Metrics.h"

#include "ohm/archi/types.h"

#include <vector>
#include <map>
#include <string>



namespace plex
{



class LogMgr;
class LogLine;
class StatsData;

class ProtocolOsHttp
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Http      // to oshttp
   {
                  Http_INVALID = -1,
                  
                  Http_HTTP_REQUEST = 0,
                  
                  Http_INFO,
                  // StatsData
                  // LogMgr
   };
   
   enum Plex      // to osplex
   {
                  Plex_INVALID = -1,
                  
                  Plex_HTTP_RESPONSE = 0,
                  
                  Plex_RESTART_OSPLEX,
                  // nothing

                  Plex_SHUTDOWN_OSPLEX,
                  // nothing
   };

   virtual        ~ProtocolOsHttp () {}

   static size_t  get_packet_size (const std::vector <ohm::archi::UByte> & data);

   static void    read_type (Http & type, ohm::util::BinaryStreamInput & bsi);
   static void    read_http_request (std::string & action, std::map <std::string, std::string> & args, ohm::util::BinaryStreamInput & bsi);
   static void    read_info (StatsData & stats_data, LogMgr & log_mgr, ohm::util::BinaryStreamInput & bsi);
   
   static void    write_http_request (std::vector <ohm::archi::UByte> & data, const std::string & action, const std::map <std::string, std::string> & args);
   static void    write_info (std::vector <ohm::archi::UByte> & data, const StatsData & stats_data, const LogMgr & log_mgr);

   static void    read_type (Plex & type, ohm::util::BinaryStreamInput & bsi);
   static void    read_http_response (const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi);

   static void    write_http_response (std::vector <ohm::archi::UByte> & data, const std::vector <ohm::archi::UByte> & page);
   static void    write_restart_osplex (std::vector <ohm::archi::UByte> & data);
   static void    write_shutdown_osplex (std::vector <ohm::archi::UByte> & data);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ProtocolOsHttp ();
                  ProtocolOsHttp (const ProtocolOsHttp &other);
   ProtocolOsHttp &
                  operator = (const ProtocolOsHttp &other);
   bool           operator == (const ProtocolOsHttp &other);
   bool           operator != (const ProtocolOsHttp &other);

}; // class ProtocolOsHttp



}  // namespace plex



//#include  "ProtocolOsHttp.hpp"



#endif   // plex_ProtocolOsHttp_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

