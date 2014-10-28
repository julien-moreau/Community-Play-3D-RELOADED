/*****************************************************************************

        ProtocolOsHttp.cpp
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

#include "plex/shared/ProtocolOsHttp.h"
#include "plex/shared/LogMgr.h"
#include "plex/shared/LogLine.h"
#include "plex/shared/StatsData.h"

#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : get_packet_size
==============================================================================
*/

size_t   ProtocolOsHttp::get_packet_size (const std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   if (data.size () < 4)
   {
      return 0;
   }
   
   util::BinaryStreamInput bsi (data);
   
   size_t packet_size = 0;
   bsi.read_uint32 (packet_size, "bsi:http:packet_size");
   
   return packet_size;
}



/*
==============================================================================
Name : read_type
==============================================================================
*/

void  ProtocolOsHttp::read_type (Http & type, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_ubyte (type, "bsi:http:type");
}



/*
==============================================================================
Name : read_http_request
==============================================================================
*/

void  ProtocolOsHttp::read_http_request (std::string & action, std::map <std::string, std::string> & args, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (action, "bsi:http:action");
   bsi.read (args, "bsi:http:args");
}



/*
==============================================================================
Name : read_info
==============================================================================
*/

void  ProtocolOsHttp::read_info (StatsData & stats_data, LogMgr & log_mgr, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (stats_data, "bsi:http:stats_data");
   bsi.read (log_mgr, "bsi:http:log_mgr");
}



/*
==============================================================================
Name : write_http_request
==============================================================================
*/

void  ProtocolOsHttp::write_http_request (std::vector <ohm::archi::UByte> & data, const std::string & action, const std::map <std::string, std::string> & args)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Http_HTTP_REQUEST));
   bso.write (action);
   bso.write (args.begin (), args.end ());
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_info
==============================================================================
*/

void  ProtocolOsHttp::write_info (std::vector <ohm::archi::UByte> & data, const StatsData & stats_data, const LogMgr & log_mgr)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Http_INFO));
   bso.write (stats_data);
   bso.write (log_mgr);
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : read_type
==============================================================================
*/

void  ProtocolOsHttp::read_type (Plex & type, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_ubyte (type, "bsi:plex:type");
}



/*
==============================================================================
Name : read_http_response
==============================================================================
*/

void  ProtocolOsHttp::read_http_response (const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (data_size, "bsi:http:data_size");
   bsi.read (data_ptr, data_size, "bsi:http:data_ptr");
}



/*
==============================================================================
Name : write_http_response
==============================================================================
*/

void  ProtocolOsHttp::write_http_response (std::vector <ohm::archi::UByte> & data, const std::vector <ohm::archi::UByte> & page)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_HTTP_RESPONSE));
   bso.write (archi::UInt32 (page.size ()));
   bso.write (&page [0], page.size ());
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_restart_osplex
==============================================================================
*/

void  ProtocolOsHttp::write_restart_osplex (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_RESTART_OSPLEX));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_shutdown_osplex
==============================================================================
*/

void  ProtocolOsHttp::write_shutdown_osplex (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_SHUTDOWN_OSPLEX));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

