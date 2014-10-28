/*****************************************************************************

        Connection.h
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



#if ! defined (plex_Connection_HEADER_INCLUDED)
#define  plex_Connection_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/BufferList.h"

#include "ohm/archi/types.h"

#include <list>
#include <vector>

#include <sys/socket.h>
#include <arpa/inet.h>



namespace plex
{



class Central;

class Connection
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Action
   {
                  Action_NOTHING = 0,
                  Action_DATA,
                  Action_DELETE,
   };
   
                  Connection (Central & central, int fd, void * ctx_ptr, FdEventMgr::ProcessFnc process_fnc);
                  Connection (Central & central, int fd, const ::sockaddr_in & addr);
   virtual        ~Connection ();
   
   int            get_fd () const;
   ::sockaddr_in &
                  use_addr ();
   
   double         get_read_ts () const;
   double         get_write_ts () const;
   
   void           receive ();
   Action         get_action ();
   void           pop (std::vector <ohm::archi::UByte> & data);
   
   void           push (std::vector <ohm::archi::UByte> & data);
   void           send ();
   bool           need_send () const;

   void           terminate ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   enum {         BUF_SIZE = 4096   };
   
   void           receive_socket ();
   void           send_socket ();

   const int      _fd;
   ::sockaddr_in  _addr;

   bool           _is_ok_flag;
   
   double         _read_ts;
   double         _write_ts;
   
   std::vector <ohm::archi::UByte>
                  _read_data;
   BufferList     _buffer_list;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Connection ();
                  Connection (const Connection &other);
   Connection &   operator = (const Connection &other);
   bool           operator == (const Connection &other);
   bool           operator != (const Connection &other);

}; // class Connection



}  // namespace plex



//#include  "Connection.hpp"



#endif   // plex_Connection_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

