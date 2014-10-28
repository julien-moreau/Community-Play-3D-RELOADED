/*****************************************************************************

        ClientBase.cpp
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

#include "plex/ClientBase.h"

#include "plex/Central.h"
#include "plex/shared/Configuration.h"
#include "plex/FdEventMgr.h"
#include "plex/fnc.h"

#include "ohm/archi/def.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/util/Base64.h"
#include "ohm/util/trace.h"

#include <algorithm>
#include <stdexcept>

#include <sys/wait.h>
#include <fcntl.h>
#include <resolv.h>
#include <openssl/bio.h>
#include <openssl/hmac.h>
#include <openssl/ssl.h>

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ClientBase::ClientBase (Central & central, int sd)
:  _central (central)
,  _sd (sd)
,  _cnx (central, sd, this, &ClientBase::send)
{
   set_non_blocking (_sd);

   // post

   FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
   
   evt_mgr.insert (FdEventMgr::Type_READ, _sd, this, &ClientBase::receive);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ClientBase::~ClientBase ()
{
   try
   {
      FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
      
      evt_mgr.erase (FdEventMgr::Type_READ, _sd);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : get_sd
==============================================================================
*/

int   ClientBase::get_sd () const
{
   return _sd;
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  ClientBase::receive (void * ctx_ptr)
{
   assert (ctx_ptr != 0);
   
   ClientBase * this_ptr = (ClientBase *) ctx_ptr;
   
   this_ptr->receive ();
}



/*
==============================================================================
Name : receive
==============================================================================
*/

void  ClientBase::receive ()
{
   using namespace ohm;
   
   _cnx.receive ();
   
   if (_cnx.get_action () == Connection::Action_DATA)
   {
      std::vector <archi::UByte> data;
      _cnx.pop (data);
      
      _data.insert (_data.end (), data.begin (), data.end ());
      
      process ();
   }
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  ClientBase::send (void * ctx_ptr)
{
   assert (ctx_ptr != 0);
   
   ClientBase * this_ptr = (ClientBase *) ctx_ptr;
   
   this_ptr->send ();
}



/*
==============================================================================
Name : send
==============================================================================
*/

void  ClientBase::send ()
{
   _cnx.send ();
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  ClientBase::push (std::vector <ohm::archi::UByte> & data)
{
   _cnx.push (data);
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  ClientBase::push (const ohm::archi::UByte * data_ptr, size_t data_size)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data (data_ptr, data_ptr + data_size);
   
   push (data);
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  ClientBase::push (const char * str_0)
{
   using namespace ohm;
   
   std::vector <archi::UByte> data (str_0, str_0 + strlen (str_0));
   
   push (data);
}



/*
==============================================================================
Name : terminate
==============================================================================
*/

void  ClientBase::terminate ()
{
   _cnx.terminate ();
}



/*
==============================================================================
Name : need_release
==============================================================================
*/

bool  ClientBase::need_release ()
{
   return _cnx.get_action () == Connection::Action_DELETE;
}



/*
==============================================================================
Name : get_read_ts
Description :
   Returns the last time 'this' received some data.
==============================================================================
*/

double   ClientBase::get_read_ts () const
{
   return _cnx.get_read_ts ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : pop
==============================================================================
*/

void  ClientBase::pop (size_t size)
{
   _data.erase (_data.begin (), _data.begin () + size);
}



/*
==============================================================================
Name : get_data_ptr
==============================================================================
*/

const ohm::archi::UByte *  ClientBase::get_data_ptr (size_t pos, size_t size)
{
   assert (pos < _data.size ());
   assert (pos + size <= _data.size ());
   
   return &_data [pos];
}



/*
==============================================================================
Name : find
==============================================================================
*/

size_t   ClientBase::find (const char * str_0) const
{
   using namespace ohm;
   
   size_t strlen = ::strlen (str_0);
   
   std::vector <archi::UByte>::const_iterator it = std::search (
      _data.begin (), _data.end (),
      (const archi::UByte *) str_0, (const archi::UByte *) str_0 + strlen
   );
   
   if (it == _data.end ())
   {
      return std::string::npos;
   }
   else
   {
      return size_t (it - _data.begin ());
   }
}



/*
==============================================================================
Name : trace_escaped_ascii
==============================================================================
*/

void  ClientBase::trace_escaped_ascii ()
{
   using namespace ohm;
   
   std::string str (_data.begin (), _data.end ());
   std::string esc;
   
   for (size_t i = 0 ; i < str.size () ; ++i)
   {
      char c = str [i];
      
      // from space to ~
      if ((c >= 32) && (c <= 126) && c != '\\')
      {
         esc.push_back (c);
      }
      else if (c == '\\')
      {
         esc.push_back ('\\');
         esc.push_back ('\\');
      }
      else if (c == '\n')
      {
         esc.push_back ('\\');
         esc.push_back ('n');
      }
      else if (c == '\r')
      {
         esc.push_back ('\\');
         esc.push_back ('r');
      }
      else if (c == '\t')
      {
         esc.push_back ('\\');
         esc.push_back ('t');
      }
      else
      {
         esc.push_back ('\\');
         esc.push_back ('x');
         
         archi::UByte c0 = (c >> 4) & 15;
         
         if (c0 < 10)
         {
            esc.push_back ('0' + c0);
         }
         else
         {
            esc.push_back ('a' + c0 - 10);
         }
         
         archi::UByte c1 = c & 15;
         
         if (c1 < 10)
         {
            esc.push_back ('0' + c1);
         }
         else
         {
            esc.push_back ('a' + c1 - 10);
         }
      }
   }
   
   ohm_util_TRACE_INFO1 ("%1%", esc.c_str ());
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process
==============================================================================
*/

void  ClientBase::process ()
{
   bool loop_flag = true;
   
   int err = 0;
   
   while (loop_flag)
   {
      loop_flag = do_process (err);
      
      if (err != 0)
      {
         loop_flag = false;
      }
   }
   
   if (err != 0)
   {
      _cnx.terminate ();
   }
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

