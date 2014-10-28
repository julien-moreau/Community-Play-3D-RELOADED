/*****************************************************************************

        ProtocolOsSess.h
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



#if ! defined (plex_ProtocolOsSess_HEADER_INCLUDED)
#define  plex_ProtocolOsSess_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/shared/Metrics.h"

#include "ohm/archi/types.h"

#include <vector>



namespace plex
{



class LogLine;

class ProtocolOsSess
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Sess      // to ossess
   {
                  Sess_INVALID = -1,
                  
                  Sess_TX = 0,
                  // std::string : username
                  // archi::UInt32 : data_size
                  // const archi::UByte * : data
                  
                  Sess_DISCONNECTED,
                  // std::string : username
                  
                  Sess_FLUSH,
                  // nothing

                  Sess_STOP,
                  // nothing
   };
   
   enum Plex      // to osplex
   {
                  Plex_INVALID = -1,
                  
                  Plex_SEND = 0,
                  // std::string : username
                  // archi::UInt32 : data_size
                  // const archi::UByte * : data

                  Plex_BROADCAST,
                  // archi::UInt32 : data_size
                  // const archi::UByte * : data

                  Plex_BROADCAST_EXCEPT,
                  // std::string : username
                  // archi::UInt32 : data_size
                  // const archi::UByte * : data

                  Plex_STOPPING,
                  // nothing
                  
                  Plex_LOG_LINE,
                  // std::string : namespace
                  // LogLine : log_line
                  
                  Plex_PROC_STAT,
                  // archi::UInt32 : mem_size
                  // archi::UInt32 : cpu

                  Plex_STORAGE_IDLE,
                  // nothing

                  Plex_STORAGE_READING,
                  // archi::UInt32 : cur

                  Plex_STORAGE_WRITING,
                  // archi::UInt32 : cur
                  // archi::UInt32 : total
                  
                  Plex_STORAGE_COPYING,
                  // nothing

                  Plex_KICK_ALL,
                  // nothing
   };

   virtual        ~ProtocolOsSess () {}
   
   static size_t  get_packet_size (const std::vector <ohm::archi::UByte> & data);

   static void    read_type (Sess & type, ohm::util::BinaryStreamInput & bsi);
   static void    read_tx (std::string & username, const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi);
   static void    read_disconnected (std::string & username, ohm::util::BinaryStreamInput & bsi);
   
   static void    write_tx (std::vector <ohm::archi::UByte> & data, const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size);
   static void    write_disconnected (std::vector <ohm::archi::UByte> & data, const std::string & username);
   static void    write_flush (std::vector <ohm::archi::UByte> & data);
   static void    write_stop (std::vector <ohm::archi::UByte> & data);

   static void    read_type (Plex & type, ohm::util::BinaryStreamInput & bsi);
   static void    read_send (std::string & username, const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi);
   static void    read_broadcast (const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi);
   static void    read_broadcast_except (std::string & username, const ohm::archi::UByte * & data_ptr, size_t & data_size, ohm::util::BinaryStreamInput & bsi);
   static void    read_log_line (std::string & ns, LogLine & log_line, ohm::util::BinaryStreamInput & bsi);
   static void    read_proc_stat (size_t & mem_size, size_t & cpu, ohm::util::BinaryStreamInput & bsi);
   static void    read_storage_reading (size_t & cur, ohm::util::BinaryStreamInput & bsi);
   static void    read_storage_writing (size_t & cur, size_t & total, ohm::util::BinaryStreamInput & bsi);

   static void    write_send (std::vector <ohm::archi::UByte> & data, const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size);
   static void    write_broadcast (std::vector <ohm::archi::UByte> & data, const ohm::archi::UByte * data_ptr, size_t data_size);
   static void    write_broadcast_except (std::vector <ohm::archi::UByte> & data, const std::string & username, const ohm::archi::UByte * data_ptr, size_t data_size);
   static void    write_stopping (std::vector <ohm::archi::UByte> & data);
   static void    write_log_line (std::vector <ohm::archi::UByte> & data, const std::string & ns, const LogLine & log_line);
   static void    write_proc_stat (std::vector <ohm::archi::UByte> & data, size_t mem_size, size_t cpu);
   static void    write_storage_idle (std::vector <ohm::archi::UByte> & data);
   static void    write_storage_reading (std::vector <ohm::archi::UByte> & data, size_t cur);
   static void    write_storage_writing (std::vector <ohm::archi::UByte> & data, size_t cur, size_t total);
   static void    write_storage_copying (std::vector <ohm::archi::UByte> & data);
   static void    write_kick_all (std::vector <ohm::archi::UByte> & data);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ProtocolOsSess ();
                  ProtocolOsSess (const ProtocolOsSess &other);
   ProtocolOsSess &
                  operator = (const ProtocolOsSess &other);
   bool           operator == (const ProtocolOsSess &other);
   bool           operator != (const ProtocolOsSess &other);

}; // class ProtocolOsSess



}  // namespace plex



//#include  "ProtocolOsSess.hpp"



#endif   // plex_ProtocolOsSess_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

