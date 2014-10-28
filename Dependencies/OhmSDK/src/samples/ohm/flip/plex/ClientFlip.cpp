/*****************************************************************************

        ClientFlip.cpp
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

#include "plex/ClientFlip.h"

#include "plex/Central.h"
#include "plex/shared/Configuration.h"
#include "plex/FdEventMgr.h"
#include "plex/shared/LogMgr.h"
#include "plex/ProtocolFlip.h"
#include "plex/Session.h"
#include "plex/Stats.h"

#include "ohm/archi/def.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/util/Base64.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <sys/wait.h>
#include <resolv.h>
#include <openssl/bio.h>
#include <openssl/hmac.h>
#include <openssl/ssl.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const char *   ClientFlip::_ns_log_0 = "ClientFlip";

/*
==============================================================================
Name : init_static
==============================================================================
*/

void  ClientFlip::init_static ()
{
   push_uint32 (_ping_data, 0);

   push_uint32 (_cxak_data, 8);
   push_uint32 (_cxak_data, ProtocolFlip::Fcc_CNX_ACK);
   push_uint32 (_cxak_data, 8);

   push_uint32 (_cxrf_data, 8);
   push_uint32 (_cxrf_data, ProtocolFlip::Fcc_CNX_REFUSED);
   push_uint32 (_cxrf_data, 8);
}
   


/*
==============================================================================
Name : ctor
==============================================================================
*/

ClientFlip::ClientFlip (Central & central, int sd, const ::sockaddr_in & addr)
:  ClientBase (central, sd)
,  _central (central)
,  _stats (central.use_stats ())
,  _sd (sd)
,  _state (State_CONNECTED)
,  _session_ptr (0)
,  _to_client_ping_due_time (-1)
{
   std::string ip_str;

   const char * ip_0 = inet_ntoa (addr.sin_addr);
   
   if (ip_0 != 0)
   {
      ip_str = ip_0;
   }

   TRACEo2 ("c%1%: connecting from ip %2%", _sd, ip_str);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ClientFlip::~ClientFlip ()
{
   try
   {
      if (_session_ptr != 0)
      {
         _central.unbind (*_session_ptr, *this);
         
         _session_ptr = 0;
      }
      
      TRACEo1 ("c%1%: disconnected", _sd);
   }
   catch (...)
   {
      TRACEX1 ("c%1%: exception !", _sd);
      
      assert (false);
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ClientFlip::bind (Session & session)
{
   using namespace ohm;
   
   assert (_session_ptr == 0);
   assert (&session != 0);
   
   _session_ptr = &session;
   
   if ((_session_ptr != 0) && (!_latent_tx_data_list.empty ()))
   {
      size_t tx_size = 0;
      
      std::list <std::vector <archi::UByte> >::iterator it = _latent_tx_data_list.begin ();
      const std::list <std::vector <archi::UByte> >::iterator it_end = _latent_tx_data_list.end ();
      
      for (; it != it_end ; ++it)
      {
         std::vector <archi::UByte> & data = *it;
         
         tx_size += data.size ();
         
         _session_ptr->push_tx (_username, data);
      }
      
      TRACEI2 ("c%1%: forwarded %2% bytes of latent data", _sd, tx_size);
      
      _latent_tx_data_list.clear ();
   }
}



/*
==============================================================================
Name : process_ping
==============================================================================
*/

void  ClientFlip::process_ping ()
{
   using namespace ohm;
   
   if (_state == State_AUTHENTIFIED)
   {
      double now = sys::TimerReliable::use_instance ().get ();
      
      if (now >= _to_client_ping_due_time)
      {
         assert (!_ping_data.empty ());
         
         // make a copy, as it will swapped in 'push'
         std::vector <archi::UByte> ping_data (_ping_data);
         
         //TRACEo1 ("c%1%: push_ping", _sd);
         
         push (ping_data);
         
         const Configuration & conf = Configuration::get ();
      
         _to_client_ping_due_time = now + conf._ping_period;
      }
   }
}



/*
==============================================================================
Name : get_sd
==============================================================================
*/

int   ClientFlip::get_sd () const
{
   return _sd;
}



/*
==============================================================================
Name : get_username
==============================================================================
*/

const std::string &  ClientFlip::get_username () const
{
   return _username;
}



/*
==============================================================================
Name : get_session
==============================================================================
*/

const std::string &  ClientFlip::get_session () const
{
   return _session;
}



/*
==============================================================================
Name : get_format_version
==============================================================================
*/

const std::string &  ClientFlip::get_format_version () const
{
   return _format_version;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_process
==============================================================================
*/

bool  ClientFlip::do_process (int & err)
{
   using namespace ohm;
   
   bool loop_flag = true;
   
   if (size () >= 4)
   {
      size_t packet_size = 0;
      
      packet_size |= get_byte (0) << 24;
      packet_size |= get_byte (1) << 16;
      packet_size |= get_byte (2) << 8;
      packet_size |= get_byte (3);
      
      // maximum possible size : 32 MB
      size_t max_packet_size = 32L * 1024L * 1024L;
      
      if (packet_size == 0)
      {
         // this is ping
         
         //TRACEo1 ("c%1%: ping", _sd);
         
         pop (4);
      }
      else if ((packet_size >= max_packet_size) || (packet_size < 16))
      {
         TRACEX2 ("c%1%: wrong packet size (%2%)", _sd, packet_size);
         
         err = -1;
      }
      else if (size () >= 4 + packet_size)
      {
         // packet is ready
         
         const archi::UByte * data_ptr = get_data_ptr (4, packet_size);
         
         err = process_packet (data_ptr, packet_size);
         
         pop (4 + packet_size);
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
   
   return loop_flag;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_packet
==============================================================================
*/

int   ClientFlip::process_packet (const ohm::archi::UByte * data_ptr, size_t packet_size)
{
   int err = 0;
   
   if (_state == State_CONNECTED)
   {
      err = process_cxrq (data_ptr, packet_size);
   }
   else if (_state == State_AUTHENTIFIED)
   {
      err = process_tx (data_ptr, packet_size);
   }
   else
   {
      assert (false);

      err = -1;
   }
   
   return err;
}



/*
==============================================================================
Name : process_cxrq
==============================================================================
*/

int   ClientFlip::process_cxrq (const ohm::archi::UByte * data_ptr, size_t packet_size)
{
   int err = 0;
   
   if (err == 0)
   {
      err = process_check_cxrq (data_ptr, packet_size);
   }
   
   if (err == 0)
   {
      err = process_check_serv (data_ptr, packet_size);
   }
   
   if (err == 0)
   {
      err = process_check_vers (data_ptr, packet_size);
   }
   
   if (err == 0)
   {
      err = ProtocolFlip::read_fcc_string (_session, ProtocolFlip::Fcc_SESSION, data_ptr, packet_size);
   }
   
   if (err == 0)
   {
      err = ProtocolFlip::read_fcc_string (_username, ProtocolFlip::Fcc_USERNAME, data_ptr, packet_size);
   }
   
   if (err == 0)
   {
      err = ProtocolFlip::read_fcc_string (_secret, ProtocolFlip::Fcc_SECRET, data_ptr, packet_size);
   }
   
   if (err == 0)
   {
      err = process_cxrq ();
   }
   
   return err;
}



/*
==============================================================================
Name : process_cxrq
==============================================================================
*/

int   ClientFlip::process_cxrq ()
{
   using namespace ohm;
   
   int err = 0;
   
   const Configuration & conf = Configuration::get ();
      
   if (err == 0)
   {
      TRACEo2 ("c%1%:   \"%2%\"", _sd, _username);
      
      if (!conf._no_auth_flag)
      {
         bool ok_flag = check_secret ();
         
         if (!ok_flag)
         {
            TRACEX1 ("c%1%:   *** AUTH FAILED ***", _sd);
            
            err = -1;
         }
      }
   }
   
   if (err == 0)
   {
      bool ok_flag = _central.bind (*this);
      
      if (!ok_flag)
      {
         TRACEX2 ("c%1%:   cannot bind session %2%", _sd, _session.c_str ());
         
         err = -1;
      }
   }
   
   if (err == 0)
   {
      TRACEo1 ("c%1%: sending cxak", _sd);
      
      assert (!_cxak_data.empty ());
      
      // make a copy, as it will swapped in 'push'
      std::vector <archi::UByte> cxak_data (_cxak_data);
      
      push (cxak_data);
      
      _state = State_AUTHENTIFIED;

      _to_client_ping_due_time = sys::TimerReliable::use_instance ().get ();
      _to_client_ping_due_time += conf._ping_period;
   }
   else
   {
      TRACEo1 ("c%1%: sending cxrf", _sd);
      
      assert (!_cxrf_data.empty ());

      // make a copy, as it will swapped in 'push'
      std::vector <archi::UByte> cxrf_data (_cxrf_data);
      
      push (cxrf_data);
      
      err = -1;
   }
   
   return err;
}



/*
==============================================================================
Name : process_tx
Note :
   The caller guarantees that the packet is at least 16 bytes
==============================================================================
*/

int   ClientFlip::process_tx (const ohm::archi::UByte * data_ptr, size_t packet_size)
{
   using namespace ohm;
   
   int err = 0;
   
   archi::UInt32 fcc_size = 0;
   
   if (err == 0)
   {
      archi::UInt32 fcc = ProtocolFlip::read_uint32 (&data_ptr [0]);
      fcc_size = ProtocolFlip::read_uint32 (&data_ptr [4]);
      
      if (fcc == ProtocolFlip::Fcc_TX)
      {
         // ok
      }
      else
      {
         TRACEX1 ("handle_other: Fcc_* unknown (%1%)", fcc);
         err = -1;
      }
   }

   if (err == 0)
   {
      archi::UInt32 sub_fcc = ProtocolFlip::read_uint32 (&data_ptr [8]);
      archi::UInt32 sub_fcc_size = ProtocolFlip::read_uint32 (&data_ptr [12]);
      
      // erlang envelop was used, so that mixed with iff, this gives
      // the 0-3 bytes the same as 8-11 bytes
      
      if (fcc_size != packet_size)
      {
         TRACEX3 ("c%1%: size mismatch (%2% %3%)", _sd, fcc_size, packet_size);
         err = -1;
      }

      if (sub_fcc != ProtocolFlip::Fcc_DATA)
      {
         TRACEX2 ("c%1%: Fcc_DATA mismatch (%2%)", _sd, sub_fcc);
         err = -1;
      }
      
      if (fcc_size != sub_fcc_size + 8)
      {
         TRACEX3 ("c%1%: sub size mismatch (%2% %3%)", _sd, fcc_size, sub_fcc_size + 8);
         err = -1;
      }
   }
   
   if (err == 0)
   {
      data_ptr += 16;
      packet_size -= 16;
      
      std::vector <archi::UByte> data (data_ptr, data_ptr + packet_size);
         
      if (_session_ptr != 0)
      {
         _session_ptr->push_tx (_username, data);
      }
      else
      {
         std::vector <archi::UByte> & tx_data = *_latent_tx_data_list.insert (
            _latent_tx_data_list.end (), std::vector <archi::UByte> ()
         );

         tx_data.swap (data);
         
         TRACEI2 ("c%1%: not binded, storing %2% bytes of latent data", _sd, tx_data.size ());
      }
   }
   
   return err;
}



/*
==============================================================================
Name : process_check_cxrq
==============================================================================
*/

int   ClientFlip::process_check_cxrq (const ohm::archi::UByte * & data_ptr, size_t & packet_size)
{
   using namespace ohm;
   
   archi::UInt32 fcc;
   size_t fcc_size;
   
   size_t old_packet_size = packet_size;
   
   int err = ProtocolFlip::read_iff_header (fcc, fcc_size, data_ptr, packet_size);
   
   if (err == 0)
   {
      bool ok_flag = fcc == ProtocolFlip::Fcc_CNX_REQ;
      ok_flag &= fcc_size == old_packet_size;
      
      if (!ok_flag)
      {
         TRACEX1 ("c%1%: handle_cxrq: wrong cxrq", _sd);
         err = -1;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : process_check_serv
==============================================================================
*/

int   ClientFlip::process_check_serv (const ohm::archi::UByte * & data_ptr, size_t & packet_size)
{
   std::string str;
   int err = ProtocolFlip::read_fcc_string (str, ProtocolFlip::Fcc_SERVICE, data_ptr, packet_size);
   
   if (err == 0)
   {
      static const std::string service ("com.ohmstudio.daw");
      
      if (str != service)
      {
         TRACEX1 ("c%1%: handle_cxrq: wrong service", _sd);
         err = -1;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : process_check_vers
==============================================================================
*/

int   ClientFlip::process_check_vers (const ohm::archi::UByte * & data_ptr, size_t & packet_size)
{
   int err = ProtocolFlip::read_fcc_string (_format_version, ProtocolFlip::Fcc_VERSION, data_ptr, packet_size);
   
   if (err == 0)
   {
      // this is where you check that the version is available, typically :
      // bool ok_flag = _central.is_format_version_available (_format_version);
      
      bool ok_flag = true;
      
      if (!ok_flag)
      {
         TRACEX1 ("c%1%: handle_cxrq: wrong version", _sd);
         err = -1;
      }
   }
   
   return err;
}



/*
==============================================================================
Name : check_secret
Description :
   Typically this is where you verify that a client cannot connect to the
   server.
   Here we just check that it is "secret"
==============================================================================
*/

bool  ClientFlip::check_secret ()
{
   using namespace ohm;
   
   return _secret == "secret";
}



/*
==============================================================================
Name : push_uint32
==============================================================================
*/

void  ClientFlip::push_uint32 (std::vector <ohm::archi::UByte> & data, ohm::archi::UInt32 x)
{
   data.push_back ((x >> 24) & 255);
   data.push_back ((x >> 16) & 255);
   data.push_back ((x >>  8) & 255);
   data.push_back ((x >>  0) & 255);
}



std::vector <ohm::archi::UByte>  ClientFlip::_ping_data;
std::vector <ohm::archi::UByte>  ClientFlip::_cxak_data;
std::vector <ohm::archi::UByte>  ClientFlip::_cxrf_data;



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

