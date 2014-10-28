/*****************************************************************************

        ComLayerClientNet.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71432

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



#if ! defined (client_net_ComLayerClientNet_HEADER_INCLUDED)
#define  client_net_ComLayerClientNet_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#include "ohm/flip/ComLayerClient.h"
#include "ohm/sys/ThreadSync.h"
#include "ohm/task/Observer.h"
#include "ohm/task/Port.h"
#include "ohm/task/Timer.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define  WIN32_LEAN_AND_MEAN
   #define  NOMINMAX
   //#define   NOGDI
   #include <winsock2.h>   // Must be included before windows.h
   #include <windows.h>

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   // nothing
   
#else
   #error Unsupported Operating System
   
#endif

#include <list>



namespace client
{
namespace net
{



class ComLayerClientNet
:  public ohm::flip::ComLayerClient
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ComLayerClientNet (const std::string & host, int port, const std::string & session, const std::string & username, const std::string & secret);
   virtual        ~ComLayerClientNet ();
   
   void           run ();
   void           stop ();
   
   // ohm::flip::ComLayerClient
   virtual void   bind (ohm::flip::ComLayerClientListener & listener);
   virtual void   unbind ();
   
   virtual void   connect ();
   virtual void   disconnect ();
   virtual void   send (const ohm::flip::DataMaster & data);


   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         BUF_SIZE = 4096   };
   
   class Buffer
   {
   public:
      size_t      _pos;
      ohm::flip::DataMaster
                  _data;
   };
   
   typedef std::list <Buffer> BufferList;
   
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   typedef ::SOCKET  Socket;

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   typedef int       Socket;
   enum {         INVALID_SOCKET    };

#endif
   
   int            resolve_host ();
   int            connect_socket ();
   void           disconnect_socket ();
   
   void           push_cxrq ();
   void           push (const ohm::archi::UByte * data_ptr, size_t data_size);
   void           push (const ohm::flip::DataMaster & data);
   
   void           ping_socket ();

   void           run_socket ();
   bool           send_socket ();
   int            send_socket (const ohm::archi::UByte * data_ptr, size_t data_size);
   void           receive_socket ();
   int            receive_socket (ohm::archi::UByte * data_ptr, size_t data_size);
   
   void           process_receive ();
   bool           process_packet ();
   void           process_packet (const ohm::archi::UByte * data_ptr, size_t data_size);
   
   static void    write_uint32 (ohm::archi::UByte * & data_ptr, ohm::archi::UInt32 x);
   static void    write_str (ohm::archi::UByte * & data_ptr, const std::string & str);
   
   void           trace_socket_error (const char * scope_0);
   
   const std::string
                  _host;
   const int      _port;
   const std::string
                  _session;
   const std::string
                  _username;
   const std::string
                  _secret;
   
   ohm::flip::ComLayerClientListener *
                  _listener_ptr;
   bool           _connected_flag;

   ohm::task::Timer
                  _timer_run;
   ohm::task::Timer
                  _timer_ping;
   
   ohm::archi::UInt32
                  _ip;
   
   Socket         _socket;
   
   BufferList     _buffer_list;
   std::vector <ohm::archi::UByte>
                  _read_data;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ComLayerClientNet ();
                  ComLayerClientNet (const ComLayerClientNet &other);
   ComLayerClientNet &
                  operator = (const ComLayerClientNet &other);
   bool           operator == (const ComLayerClientNet &other);
   bool           operator != (const ComLayerClientNet &other);

}; // class ComLayerClientNet



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace net
}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/net/ComLayerClientNet.hpp"



#endif   // client_net_ComLayerClientNet_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
