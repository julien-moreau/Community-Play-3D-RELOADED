/*****************************************************************************

        ProtocolOsSess.cpp
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

#include "plex/shared/ProtocolOsSess.h"
#include "plex/shared/LogLine.h"

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

size_t   ProtocolOsSess::get_packet_size (const std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   if (data.size () < 4)
   {
      return 0;
   }
   
   util::BinaryStreamInput bsi (data);
   
   size_t packet_size = 0;
   bsi.read_uint32 (packet_size, "bsi:sess:packet_size");
   
   return packet_size;
}



/*
==============================================================================
Name : read_type
==============================================================================
*/

void  ProtocolOsSess::read_type (Sess & type, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_ubyte (type, "bsi:sess:type");
}



/*
==============================================================================
Name : read_tx
==============================================================================
*/

void  ProtocolOsSess::read_tx (std::string & username, const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (username, "bsi:sess:usename");
   bsi.read_uint32 (data_size, "bsi:sess:data_size");
   bsi.read (data_ptr, data_size, "bsi:sess:data_ptr");
}



/*
==============================================================================
Name : read_disconnected
==============================================================================
*/

void  ProtocolOsSess::read_disconnected (std::string & username, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (username, "bsi:sess:usename");
}



/*
==============================================================================
Name : write_tx
==============================================================================
*/

void  ProtocolOsSess::write_tx (std::vector <ohm::archi::UByte> & data, const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Sess_TX));
   bso.write (username);
   bso.write (archi::UInt32 (data_size));
   bso.write (data_ptr, data_size);
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_disconnected
==============================================================================
*/

void  ProtocolOsSess::write_disconnected (std::vector <ohm::archi::UByte> & data, const std::string & username)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Sess_DISCONNECTED));
   bso.write (username);
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_flush
==============================================================================
*/

void  ProtocolOsSess::write_flush (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Sess_FLUSH));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_stop
==============================================================================
*/

void  ProtocolOsSess::write_stop (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Sess_STOP));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : read_type
==============================================================================
*/

void  ProtocolOsSess::read_type (Plex & type, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_ubyte (type, "bsi:plex:type");
}



/*
==============================================================================
Name : read_send
==============================================================================
*/

void  ProtocolOsSess::read_send (std::string & username, const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (username, "bsi:plex:username");
   bsi.read_uint32 (data_size, "bsi:plex:data_size");
   bsi.read (data_ptr, data_size, "bsi:plex:data_ptr");
}



/*
==============================================================================
Name : read_broadcast
==============================================================================
*/

void  ProtocolOsSess::read_broadcast (const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (data_size, "bsi:plex:data_size");
   bsi.read (data_ptr, data_size, "bsi:plex:data_ptr");
}



/*
==============================================================================
Name : read_broadcast_except
==============================================================================
*/

void  ProtocolOsSess::read_broadcast_except (std::string & username, const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (username, "bsi:plex:username");
   bsi.read_uint32 (data_size, "bsi:plex:data_size");
   bsi.read (data_ptr, data_size, "bsi:plex:data_ptr");
}



/*
==============================================================================
Name : read_log_line
==============================================================================
*/

void  ProtocolOsSess::read_log_line (std::string & ns, LogLine & log_line, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read (ns, "bsi:plex:ns");
   bsi.read (log_line, "bsi:plex:log_line");
}



/*
==============================================================================
Name : read_proc_stat
==============================================================================
*/

void  ProtocolOsSess::read_proc_stat (size_t & mem_size, size_t & cpu, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (mem_size, "bsi:plex:mem_size");
   bsi.read_uint32 (cpu, "bsi:plex:cpu");
}



/*
==============================================================================
Name : read_storage_reading
==============================================================================
*/

void  ProtocolOsSess::read_storage_reading (size_t & cur, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (cur, "bsi:plex:cur");
}



/*
==============================================================================
Name : read_storage_writing
==============================================================================
*/

void  ProtocolOsSess::read_storage_writing (size_t & cur, size_t & total, ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (cur, "bsi:plex:cur");
   bsi.read_uint32 (total, "bsi:plex:total");
}



/*
==============================================================================
Name : write_send
==============================================================================
*/

void  ProtocolOsSess::write_send (std::vector <ohm::archi::UByte> & data, const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_SEND));
   bso.write (username);
   bso.write (archi::UInt32 (data_size));
   bso.write (data_ptr, data_size);
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_broadcast
==============================================================================
*/

void  ProtocolOsSess::write_broadcast (std::vector <ohm::archi::UByte> & data, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_BROADCAST));
   bso.write (archi::UInt32 (data_size));
   bso.write (data_ptr, data_size);
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_broadcast_except
==============================================================================
*/

void  ProtocolOsSess::write_broadcast_except (std::vector <ohm::archi::UByte> & data, const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_BROADCAST_EXCEPT));
   bso.write (username);
   bso.write (archi::UInt32 (data_size));
   bso.write (data_ptr, data_size);
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_stopping
==============================================================================
*/

void  ProtocolOsSess::write_stopping (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_STOPPING));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_log_line
==============================================================================
*/

void  ProtocolOsSess::write_log_line (std::vector <ohm::archi::UByte> & data, const std::string & ns, const LogLine & log_line)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_LOG_LINE));
   bso.write (ns);
   bso.write (log_line);
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_proc_stat
==============================================================================
*/

void  ProtocolOsSess::write_proc_stat (std::vector <ohm::archi::UByte> & data, size_t mem_size, size_t cpu)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_PROC_STAT));
   bso.write (archi::UInt32 (mem_size));
   bso.write (archi::UInt32 (cpu));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_storage_idle
==============================================================================
*/

void  ProtocolOsSess::write_storage_idle (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_STORAGE_IDLE));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_storage_reading
==============================================================================
*/

void  ProtocolOsSess::write_storage_reading (std::vector <ohm::archi::UByte> & data, size_t cur)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_STORAGE_READING));
   bso.write (archi::UInt32 (cur));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_storage_writing
==============================================================================
*/

void  ProtocolOsSess::write_storage_writing (std::vector <ohm::archi::UByte> & data, size_t cur, size_t total)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_STORAGE_WRITING));
   bso.write (archi::UInt32 (cur));
   bso.write (archi::UInt32 (total));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_storage_copying
==============================================================================
*/

void  ProtocolOsSess::write_storage_copying (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_STORAGE_COPYING));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*
==============================================================================
Name : write_kick_all
==============================================================================
*/

void  ProtocolOsSess::write_kick_all (std::vector <ohm::archi::UByte> & data)
{
   using namespace ohm;
   
   util::BinaryStreamOutput bso (data);
   
   bso.write (archi::UInt32 (0));   // place holder for packet size
   
   bso.write (archi::UByte (Plex_KICK_ALL));
   
   bso.overwrite (archi::UInt32 (data.size ()), 0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

