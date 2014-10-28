/*****************************************************************************

        ComLayerClientNet.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71434

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#include "client/net/ComLayerClientNet.h"

#include "plex/ProtocolFlip.h"

#include "ohm/archi/endian.h"
#include "ohm/flip/ClassDescManager.h"
#include "ohm/opal/Application.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/util/trace.h"

#include <cassert>

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include <netdb.h>
#endif



namespace client
{
namespace net
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ComLayerClientNet::ComLayerClientNet (const std::string & host, int port, const std::string & session, const std::string & username, const std::string & secret)
:  ohm::flip::ComLayerClient ()
,  _host (host)
,  _port (port)
,  _session (session)
,  _username (username)
,  _secret (secret)
,  _listener_ptr (0)
,  _connected_flag (false)
,  _timer_run ("client.net.ComLayerClientNet.run")
,  _timer_ping ("client.net.ComLayerClientNet.ping")
,  _ip (0)
,  _socket (INVALID_SOCKET)
{
   _timer_run.bind_process <
      ComLayerClientNet,
      &ComLayerClientNet::run_socket
   > (*this);
   
   _timer_run.set_periodic (1.0 / 10.0);

   _timer_ping.bind_process <
      ComLayerClientNet,
      &ComLayerClientNet::ping_socket
   > (*this);
   
   _timer_ping.set_periodic (1.0);

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   const ::WORD ws_version = MAKEWORD (1, 1);
   ::WSADATA ws_data;
   int err = ::WSAStartup (ws_version, &ws_data);
   assert (err == 0);
#endif
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ComLayerClientNet::~ComLayerClientNet ()
{
   try
   {
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
      ::WSACleanup ();
#endif
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  ComLayerClientNet::run ()
{
   using namespace ohm;
   
   task::RunLoopGui::use_instance ().add (_timer_run);
   task::RunLoopGui::use_instance ().add (_timer_ping);
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  ComLayerClientNet::stop ()
{
   using namespace ohm;
   
   task::RunLoopGui::use_instance ().remove (_timer_ping);
   task::RunLoopGui::use_instance ().remove (_timer_run);
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  ComLayerClientNet::bind (ohm::flip::ComLayerClientListener & listener)
{
   assert (_listener_ptr == 0);
   assert (&listener != 0);
   
   _listener_ptr = &listener;
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  ComLayerClientNet::unbind ()
{
   assert (_listener_ptr != 0);
   
   _listener_ptr = 0;
}



/*
==============================================================================
Name : connect
==============================================================================
*/

void  ComLayerClientNet::connect ()
{
   assert (_listener_ptr != 0);
   assert (!_connected_flag);
   
   _listener_ptr->notify_connecting ();
   
   int err = 0;
   
   if (err == 0)
   {
      err = resolve_host ();
   }
   
   if (err == 0)
   {
      err = connect_socket ();
   }
   
   if (err == 0)
   {
      push_cxrq ();
   }
   
   if (err == 0)
   {
      _listener_ptr->notify_connected ();

      _connected_flag = true;
   }
   else
   {
      _listener_ptr->notify_disconnected ();
   }
}



/*
==============================================================================
Name : disconnect
==============================================================================
*/

void  ComLayerClientNet::disconnect ()
{
   assert (_listener_ptr != 0);
   assert (_connected_flag);
   
   disconnect_socket ();
   
   _listener_ptr->notify_disconnected ();
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  ComLayerClientNet::send (const ohm::flip::DataMaster & data)
{
   using namespace ohm;
   
   if (!_connected_flag)
   {
      // drop silently
      return;
   }

   size_t packet_size = data.size () + 8 + 8;
   
   archi::UByte header [20];
   
   plex::ProtocolFlip::write_uint32 (header, packet_size);
   plex::ProtocolFlip::write_uint32 (header + 4, plex::ProtocolFlip::Fcc_TX);
   plex::ProtocolFlip::write_uint32 (header + 8, packet_size);
   plex::ProtocolFlip::write_uint32 (header + 12, plex::ProtocolFlip::Fcc_DATA);
   plex::ProtocolFlip::write_uint32 (header + 16, packet_size - 8);
   
   push (header, 20);
   push (data);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : resolve_host
==============================================================================
*/

int   ComLayerClientNet::resolve_host ()
{
   using namespace ohm;
   
   _ip = 0;

   const struct hostent * host_ptr = ::gethostbyname (_host.c_str ());
   // do not free nor modify 'host_ptr'
   
   int err = 0;
   
   if (host_ptr != 0)
   {
      assert (host_ptr->h_addrtype == AF_INET);
      
      _ip = *reinterpret_cast <archi::UInt32BE *> (host_ptr->h_addr_list [0]);
   }
   else
   {
      ohm_util_TRACE_ERROR1 ("Cannot resolve %1%", _host.c_str ());
      
      err = -1;
   }
   
   return err;
}



/*
==============================================================================
Name : connect_socket
==============================================================================
*/

int   ComLayerClientNet::connect_socket ()
{
   using namespace ohm;
   
   disconnect_socket ();
   
   // create socket
   
   Socket socket = ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
   if (socket == INVALID_SOCKET)
   {
      trace_socket_error ("::socket");
      
      return INVALID_SOCKET;
   }
   
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   {
      int err = 0;
      int val = 1;

      err = ::setsockopt (
         socket, SOL_SOCKET,
         SO_NOSIGPIPE, (const char *) (&val), sizeof (val)
      );
      assert (err == 0);
   }
#endif
   
   // connect socket
   
   struct sockaddr_in server;
   ::memset (&server, 0, sizeof (server));

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   server.sin_len = sizeof (server);
#endif
   server.sin_family = AF_INET;
   server.sin_port = htons ((archi::UInt16) _port);
   server.sin_addr.s_addr = htonl ((archi::UInt32) _ip);
   
   int err = ::connect (socket, (struct sockaddr *) &server, sizeof (server));
   
   if (err == 0)
   {
      _socket = socket;
   }
   else
   {
      ohm_util_TRACE_ERROR3 (
         "socket connection error `%1%' for %2%:%3%",
         strerror (errno), _host.c_str (), _port
      );

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
      ::closesocket (_socket);
      
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
      ::close (socket);

#endif
      
      return -1;
   }
   
   // set non blocking
   
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   unsigned long argp = 1;
   err = ::ioctlsocket (_socket, FIONBIO, &argp);
   assert (err == 0);
   
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   int flag = ::fcntl (_socket, F_GETFL, 0);
   assert (flag != -1);
   
   err = ::fcntl (_socket, F_SETFL, flag | O_NONBLOCK);
   assert (err == 0);
#endif

   return 0;
}



/*
==============================================================================
Name : disconnect_socket
==============================================================================
*/

void  ComLayerClientNet::disconnect_socket ()
{
   if (_socket != INVALID_SOCKET)
   {
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
      ::closesocket (_socket);

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
      ::close (_socket);
      
#endif

      _socket = INVALID_SOCKET;
   }
}



/*
==============================================================================
Name : push_cxrq
==============================================================================
*/

void  ComLayerClientNet::push_cxrq ()
{
   using namespace ohm;
   
   static std::string service ("com.ohmstudio.daw");
   const std::string & version
      = flip::ClassDescManager::use_instance ().get_format_version ();
   
   size_t packet_size = 8;
   packet_size += 8 + service.size ();
   packet_size += 8 + version.size ();
   packet_size += 8 + _session.size ();
   packet_size += 8 + _username.size ();
   packet_size += 8 + _secret.size ();
   
   std::vector <archi::UByte> header;
   header.resize (packet_size + 4);
   
   archi::UByte * header_ptr = &header [0];
   
   write_uint32 (header_ptr, packet_size);
   write_uint32 (header_ptr, plex::ProtocolFlip::Fcc_CNX_REQ);
   write_uint32 (header_ptr, packet_size);

   write_uint32 (header_ptr, plex::ProtocolFlip::Fcc_SERVICE);
   write_uint32 (header_ptr, service.size () + 8);
   write_str (header_ptr, service);

   write_uint32 (header_ptr, plex::ProtocolFlip::Fcc_VERSION);
   write_uint32 (header_ptr, version.size () + 8);
   write_str (header_ptr, version);

   write_uint32 (header_ptr, plex::ProtocolFlip::Fcc_SESSION);
   write_uint32 (header_ptr, _session.size () + 8);
   write_str (header_ptr, _session);

   write_uint32 (header_ptr, plex::ProtocolFlip::Fcc_USERNAME);
   write_uint32 (header_ptr, _username.size () + 8);
   write_str (header_ptr, _username);

   write_uint32 (header_ptr, plex::ProtocolFlip::Fcc_SECRET);
   write_uint32 (header_ptr, _secret.size () + 8);
   write_str (header_ptr, _secret);
   
   push (&header [0], header.size ());
}



/*
==============================================================================
Name : disconnect_socket
==============================================================================
*/

void  ComLayerClientNet::push (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   assert (data_size > 0);

   flip::DataSPtr data_sptr (new flip::Data (data_ptr, data_ptr + data_size));
   
   flip::DataMaster data;
   data.assign (data_sptr);
   data.lock ();
   
   push (data);
}



/*
==============================================================================
Name : disconnect_socket
==============================================================================
*/

void  ComLayerClientNet::push (const ohm::flip::DataMaster & data)
{
   assert (!data.empty ());
   
   Buffer & buffer = *_buffer_list.insert (_buffer_list.end (), Buffer ());
   buffer._pos = 0;
   buffer._data = data;
}



/*
==============================================================================
Name : ping_socket
==============================================================================
*/

void  ComLayerClientNet::ping_socket ()
{
   using namespace ohm;
   
   archi::UByte header [4];
   
   plex::ProtocolFlip::write_uint32 (header, 0);
   
   push (header, 4);
}



/*
==============================================================================
Name : run_socket
==============================================================================
*/

void  ComLayerClientNet::run_socket ()
{
   if (_socket == INVALID_SOCKET)
   {
      return;
   }
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      loop_flag = false;
      
      ::fd_set read_set;
      FD_ZERO (&read_set);
      FD_SET (_socket, &read_set);

      ::fd_set write_set;
      FD_ZERO (&write_set);
      FD_SET (_socket, &write_set);
      
      int nbr_fd = ::select (_socket + 1, &read_set, &write_set, 0, 0);
      
      if (nbr_fd == -1)
      {
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
         if (errno != EINTR)
#endif
         {
            disconnect_socket ();
         }
      }
      else if (nbr_fd > 0)
      {
         Socket socket = _socket;
         
         if (FD_ISSET (socket, &read_set))
         {
            loop_flag = true;
            
            receive_socket ();
         }
         
         if (FD_ISSET (socket, &write_set))
         {
            loop_flag = send_socket ();
         }
      }
   }
}



/*
==============================================================================
Name : send_socket
==============================================================================
*/

bool  ComLayerClientNet::send_socket ()
{
   using namespace ohm;
   
   if (_socket == INVALID_SOCKET)
   {
      return false;
   }

   if (_buffer_list.empty ())
   {
      return false;
   }
   
   Buffer & buffer = _buffer_list.front ();
   
   assert (buffer._pos < buffer._data.size ());
   
   const archi::UByte * data_ptr = buffer._data.data () + buffer._pos;
   size_t data_size = buffer._data.size () - buffer._pos;
   
   int nbr_write = send_socket (data_ptr, data_size);
   
   if (nbr_write < 0)
   {
      return false;
   }
   
   buffer._pos += nbr_write;
   
   if (buffer._pos == buffer._data.size ())
   {
      _buffer_list.pop_front ();
   }
   
   return !_buffer_list.empty ();
}



/*
==============================================================================
Name : send_socket
Note :
   Windows version
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
int   ComLayerClientNet::send_socket (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   int nbr_write = ::send (_socket, (const char *) data_ptr, data_size, 0);
   
   if (nbr_write == SOCKET_ERROR)
   {
      switch (::WSAGetLastError ())
      {
      case WSAEWOULDBLOCK:
         // nothing
         break;
      
      default:
         trace_socket_error ("::send");
         disconnect_socket ();
         break;
      }
      
      return -1;
   }
   
   return nbr_write;
}
#endif



/*
==============================================================================
Name : send_socket
Note :
   MacOS version
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
int   ComLayerClientNet::send_socket (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   ssize_t nbr_write = ::write (_socket, data_ptr, data_size);
   
   if (nbr_write < 0)
   {
      switch (errno)
      {
      case EAGAIN:
      case EINTR:
         // nothing
         break;
         
      case EPIPE:
      case ECONNRESET:
         disconnect_socket ();
         break;
      
      default:
         trace_socket_error ("::write");
         disconnect_socket ();
         break;
      }
      
      return -1;
   }
   
   return int (nbr_write);
}
#endif



/*
==============================================================================
Name : receive_socket
==============================================================================
*/

void  ComLayerClientNet::receive_socket ()
{
   using namespace ohm;
   
   if (_socket == INVALID_SOCKET)
   {
      return;
   }

   archi::UByte buf [BUF_SIZE];
   
   int nbr_read = receive_socket (buf, BUF_SIZE);
   
   if (nbr_read == 0)
   {
      // remote client closed connection
      
      disconnect_socket ();
   }
   else if (nbr_read > 0)
   {
      _read_data.insert (_read_data.end (), buf, buf + nbr_read);
      
      process_receive ();
   }
}



/*
==============================================================================
Name : receive_socket
Note :
   Windows version
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
int   ComLayerClientNet::receive_socket (ohm::archi::UByte * data_ptr, size_t data_size)
{
   int nbr_read = ::recv (_socket, (char *) data_ptr, data_size, 0);
   
   if (nbr_read == SOCKET_ERROR)
   {
      switch (::WSAGetLastError ())
      {
      case WSAEWOULDBLOCK:
         break;

      default:
         trace_socket_error ("::recv");
         disconnect_socket ();
         break;
      }

      return -1;
   }
   
   return nbr_read;
}
#endif



/*
==============================================================================
Name : receive_socket
Note :
   MacOS version
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
int   ComLayerClientNet::receive_socket (ohm::archi::UByte * data_ptr, size_t data_size)
{
   ssize_t nbr_read = ::read (_socket, data_ptr, data_size);
   
   if (nbr_read < 0)
   {
      switch (errno)
      {
      case EAGAIN:
      case EINTR:
         break;

      case ECONNRESET:
         disconnect_socket ();
         break;

      default:
         trace_socket_error ("::read");
         disconnect_socket ();
         break;
      }

      return -1;
   }
   
   return nbr_read;
}
#endif



/*
==============================================================================
Name : process_receive
==============================================================================
*/

void  ComLayerClientNet::process_receive ()
{
   using namespace ohm;
   
   bool loop_flag = true;
   
   while (loop_flag)
   {
      loop_flag = process_packet ();
   }
}



/*
==============================================================================
Name : process_receive
==============================================================================
*/

bool  ComLayerClientNet::process_packet ()
{
   using namespace ohm;
   
   bool loop_flag = true;
   
   if (_read_data.size () >= 4)
   {
      size_t packet_size = plex::ProtocolFlip::read_uint32 (&_read_data [0]);
      
      if (packet_size == 0)
      {
         // ping
         
         _read_data.erase (_read_data.begin (), _read_data.begin () + 4);
      }
      else if (_read_data.size () >= 4 + packet_size)
      {
         // packet is ready
         
         const archi::UByte * data_ptr = &_read_data [4];
         
         process_packet (data_ptr, packet_size);
         
         _read_data.erase (_read_data.begin (), _read_data.begin () + 4 + packet_size);
      }
      else
      {
         loop_flag = false;
      }
   }
   else
   {
      loop_flag = false;
   }
   
   return loop_flag;
}



/*
==============================================================================
Name : process_packet
==============================================================================
*/

void  ComLayerClientNet::process_packet (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   archi::UInt32 fcc = plex::ProtocolFlip::read_uint32 (data_ptr);
   archi::UInt32 size = plex::ProtocolFlip::read_uint32 (data_ptr + 4);
   data_ptr += 8;
   assert (data_size >= 8);
   data_size -= 8;
   
   if (fcc == plex::ProtocolFlip::Fcc_CNX_ACK)
   {
      // nothing
   }
   else if (fcc == plex::ProtocolFlip::Fcc_CNX_REFUSED)
   {
      disconnect_socket ();
   }
   else if (fcc == plex::ProtocolFlip::Fcc_TX)
   {
      assert (_listener_ptr != 0);
   
      archi::UInt32 fcc = plex::ProtocolFlip::read_uint32 (data_ptr);
      archi::UInt32 size = plex::ProtocolFlip::read_uint32 (data_ptr + 4);
      data_ptr += 8;
      assert (data_size >= 8);
      data_size -= 8;
      
      assert (fcc == plex::ProtocolFlip::Fcc_DATA);
      
      flip::DataSPtr data_sptr (new flip::Data (data_ptr, data_ptr + data_size));
      
      flip::DataMaster data;
      data.assign (data_sptr);
      data.lock ();
      
      if (_connected_flag)
      {
         _listener_ptr->receive (data);
      }
   }
}



/*
==============================================================================
Name : write_uint32
==============================================================================
*/

void  ComLayerClientNet::write_uint32 (ohm::archi::UByte * & data_ptr, ohm::archi::UInt32 x)
{
   plex::ProtocolFlip::write_uint32 (data_ptr, x);
   data_ptr += 4;
}



/*
==============================================================================
Name : write_str
==============================================================================
*/

void  ComLayerClientNet::write_str (ohm::archi::UByte * & data_ptr, const std::string & str)
{
   using namespace std;
   
   memcpy (data_ptr, str.c_str (), str.size ());
   data_ptr += str.size ();
}



/*
==============================================================================
Name : trace_socket_error
==============================================================================
*/

void  ComLayerClientNet::trace_socket_error (const char * scope_0)
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   ohm_util_TRACE_ERROR2 ("%1% error #%2%", scope_0, ::WSAGetLastError ());
   
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   ohm_util_TRACE_ERROR3 ("%1% error #%2% `%3%'", scope_0, errno, strerror (errno));

#endif
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace net
}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
