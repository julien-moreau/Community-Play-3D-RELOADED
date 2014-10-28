/*****************************************************************************

        Connection.cpp
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

#include "plex/Connection.h"

#include "plex/Central.h"

#include "ohm/sys/TimerReliable.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <sys/wait.h>
#include <resolv.h>

#include <openssl/err.h>

#include <cassert>



#undef SKELETON_SERVER_CONNECTION_DEBUG_FLAG

#if defined (SKELETON_SERVER_CONNECTION_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error SKELETON_SERVER_CONNECTION_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define cnx_TRACE(msg_0)   ohm_util_TRACE_INFO(msg_0)
   #define cnx_TRACE1(fmt_0, arg1)  ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define cnx_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define cnx_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define cnx_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define cnx_TRACE(msg_0)   (void (0))
   #define cnx_TRACE1(fmt_0, arg1)  (void (0))
   #define cnx_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define cnx_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define cnx_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
#endif



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Connection::Connection (Central & central, int fd, void * ctx_ptr, FdEventMgr::ProcessFnc process_fnc)
:  _fd (fd)
,  _addr ()

,  _is_ok_flag (true)

,  _read_ts (ohm::sys::TimerReliable::use_instance ().get ())
,  _write_ts (-1)

,  _read_data ()
,  _buffer_list (central, fd, ctx_ptr, process_fnc)
{
   ::memset (&_addr, 0, sizeof (_addr));
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Connection::Connection (Central & central, int fd, const ::sockaddr_in & addr)
:  _fd (fd)
,  _addr (addr)

,  _is_ok_flag (true)

,  _read_ts (-1)
,  _write_ts (-1)

,  _read_data ()
,  _buffer_list (central, fd)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Connection::~Connection ()
{
   try
   {
      terminate ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

int   Connection::get_fd () const
{
   return _fd;
}



/*
==============================================================================
Name : use_addr
==============================================================================
*/

::sockaddr_in &   Connection::use_addr ()
{
   return _addr;
}



/*
==============================================================================
Name : get_read_ts
==============================================================================
*/

double   Connection::get_read_ts () const
{
   return _read_ts;
}



/*
==============================================================================
Name : get_write_ts
==============================================================================
*/

double   Connection::get_write_ts () const
{
   return _write_ts;
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  Connection::receive ()
{
   if (!_is_ok_flag)
   {
      return;  // abort
   }
   
   receive_socket ();
}



/*
==============================================================================
Name : get_action
==============================================================================
*/

Connection::Action   Connection::get_action ()
{
   if (_is_ok_flag)
   {
      return _read_data.empty () ? Action_NOTHING : Action_DATA;
   }
   else
   {
      return Action_DELETE;
   }
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  Connection::pop (std::vector <ohm::archi::UByte> & data)
{
   assert (get_action () == Action_DATA);
   
   _read_data.swap (data);
}



/*
==============================================================================
Name : push
Description :
   Push more data to send over the connection.
   On return, 'data' will be empty (it swapped internally)
==============================================================================
*/

void  Connection::push (std::vector <ohm::archi::UByte> & data)
{
   _buffer_list.push (data);
}



/*
==============================================================================
Name : send
Description :
   If possible, send accumulated data over the connection. Just try one
   'write' at most.
==============================================================================
*/

void  Connection::send ()
{
   if (!_is_ok_flag)
   {
      return;  // abort
   }
   
   if (_buffer_list.empty ())
   {
      return;  // nothing to write
   }
   
   send_socket ();
}



/*
==============================================================================
Name : need_send
==============================================================================
*/

bool  Connection::need_send () const
{
   return !_buffer_list.empty ();
}



/*
==============================================================================
Name : terminate
==============================================================================
*/

void  Connection::terminate ()
{
   if (!_is_ok_flag)
   {
      return;  // abort
   }
   
   int err = ::shutdown (_fd, SHUT_WR);
   
   if (err == -1)
   {
      ohm_util_TRACE_WARNING2 ("c%1%: shutdown failed `%2%'", _fd, strerror (errno));
   }
   
   err = ::close (_fd);
   
   if (err == -1)
   {
      ohm_util_TRACE_WARNING2 ("c%1%: close failed `%2%'", _fd, strerror (errno));
   }
   
   _is_ok_flag = false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : receive_socket
==============================================================================
*/

void  Connection::receive_socket ()
{
   using namespace ohm;
   
   archi::UByte buf [BUF_SIZE];
   
   ssize_t nbr_read = ::read (_fd, buf, BUF_SIZE);
   
   bool terminate_flag = false;
   
   if (nbr_read < 0)
   {
      switch (errno)
      {
      case EAGAIN:
      case EINTR:
         // with EINTR we might read again, but we will wait for the next 'select'
         break;

      case ECONNRESET:
         terminate_flag = true;
         break;

      default:
         ohm_util_TRACE_ERROR2 ("c%1%: read failed `%2%'", _fd, strerror (errno));
         terminate_flag = true;
         assert (false);
         break;
      }
   }
   else if (nbr_read == 0)
   {
      // remote client closed connection
      
      terminate_flag = true;
   }
   else
   {
      cnx_TRACE2 ("c%1%: recv %2%", _fd, nbr_read);
      
      _read_ts = sys::TimerReliable::use_instance ().get ();
      
      _read_data.insert (_read_data.end (), buf, buf + nbr_read);
   }
   
   if (terminate_flag)
   {
      terminate ();
   }
}



/*
==============================================================================
Name : send_socket
==============================================================================
*/

void  Connection::send_socket ()
{
   using namespace ohm;
   
   const archi::UByte * data_ptr = _buffer_list.get_data_ptr ();
   size_t data_size = _buffer_list.get_data_size ();
   
   ssize_t nbr_write = ::write (_fd, data_ptr, data_size);
   
   bool terminate_flag = false;
   
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
         terminate_flag = true;
         break;
      
      default:
         assert (false);
         ohm_util_TRACE_ERROR2 ("c%1%: write failed `%2%'", _fd, strerror (errno));
         terminate_flag = true;
         break;
      }
   }
   else
   {
      cnx_TRACE2 ("c%1%: send %2%", _fd, nbr_write);
      
      _write_ts = sys::TimerReliable::use_instance ().get ();
      
      _buffer_list.pop (nbr_write);
   }
   
   if (terminate_flag)
   {
      terminate ();
   }
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

