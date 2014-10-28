/*****************************************************************************

        FdEventMgr.cpp
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

#include "plex/FdEventMgr.h"

#include "plex/Connection.h"
#include "plex/shared/LogMgr.h"

#include "ohm/math/fnc.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <sys/wait.h>
#include <resolv.h>

#include <cassert>

#undef SKELETON_SERVER_FEM_DEBUG_FLAG

#if defined (SKELETON_SERVER_FEM_DEBUG_FLAG)
   #if defined (OHM_SHIPPING_LEVEL_CODE)
      #error SKELETON_SERVER_FEM_DEBUG_FLAG is defined in shipping level code !
   #endif

   #define fem_TRACE(msg_0)   ohm_util_TRACE_INFO(msg_0)
   #define fem_TRACE1(fmt_0, arg1)  ohm_util_TRACE_INFO1(fmt_0, arg1)
   #define fem_TRACE2(fmt_0, arg1, arg2)  ohm_util_TRACE_INFO2(fmt_0, arg1, arg2)
   #define fem_TRACE3(fmt_0, arg1, arg2, arg3)  ohm_util_TRACE_INFO3(fmt_0, arg1, arg2, arg3)
   #define fem_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  ohm_util_TRACE_INFO4(fmt_0, arg1, arg2, arg3, arg4)
#else
   #define fem_TRACE(msg_0)   (void (0))
   #define fem_TRACE1(fmt_0, arg1)  (void (0))
   #define fem_TRACE2(fmt_0, arg1, arg2)  (void (0))
   #define fem_TRACE3(fmt_0, arg1, arg2, arg3)  (void (0))
   #define fem_TRACE4(fmt_0, arg1, arg2, arg3, arg4)  (void (0))
#endif



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

const char *   FdEventMgr::_ns_log_0 = "FdEventMgr";

/*
==============================================================================
Name : ctor
==============================================================================
*/

FdEventMgr::FdEventMgr ()
:  _ctx_timeout_ptr (0)
,  _process_timeout (0)
,  _select_max_fd (-1)
{
   for (int i = 0 ; i < Type_NBR_ELT ; ++i)
   {
      FD_ZERO (&_select_base_arr [i]);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

FdEventMgr::FdEventMgr (void * ctx_ptr, ProcessFnc process_timeout)
:  _ctx_timeout_ptr (ctx_ptr)
,  _process_timeout (process_timeout)
,  _select_max_fd (-1)
{
   for (int i = 0 ; i < Type_NBR_ELT ; ++i)
   {
      FD_ZERO (&_select_base_arr [i]);
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

FdEventMgr::~FdEventMgr ()
{
}



/*
==============================================================================
Name : insert
==============================================================================
*/

void  FdEventMgr::insert (Type type, int fd, void * ctx_ptr, ProcessFnc process_fnc)
{
   fem_TRACE2 ("FdEventMgr::insert: %1% %2%", (type == Type_READ) ? "READ" : "WRITE", fd);
   
   //assert (fcntl (fd, F_GETFL) != -1 || errno != EBADF);
   
   assert (type >= 0);
   assert (type < Type_NBR_ELT);
   
   assert (fd >= 0);
   assert (fd < FD_SETSIZE);
   assert (process_fnc != 0);
   
   FdInfoMap & fd_info_map = _fd_info_map_arr [type];
   fd_set & select_base = _select_base_arr [type];
   
   assert (fd_info_map.find (fd) == fd_info_map.end ());
   
   FD_SET (fd, &select_base);
      
   if (fd > _select_max_fd)
   {
      _select_max_fd = fd;
   }

   Info & info = fd_info_map [fd];
   info._ctx_ptr = ctx_ptr;
   info._process_fnc = process_fnc;
}



/*
==============================================================================
Name : erase
==============================================================================
*/

void  FdEventMgr::erase (Type type, int fd)
{
   fem_TRACE2 ("FdEventMgr::erase: %1% %2%", (type == Type_READ) ? "READ" : "WRITE", fd);
   
   assert (type >= 0);
   assert (type < Type_NBR_ELT);
   
   assert (fd >= 0);
   assert (fd < FD_SETSIZE);
   
   FdInfoMap & fd_info_map = _fd_info_map_arr [type];
   fd_set & select_base = _select_base_arr [type];

   FdInfoMap::iterator it = fd_info_map.find (fd);
   
   assert (it != fd_info_map.end ());

   FD_CLR (fd, &select_base);
   
   fd_info_map.erase (it);
}



/*
==============================================================================
Name : wait
==============================================================================
*/

void  FdEventMgr::wait (double timeout)
{
   using namespace ohm;
   
   assert (timeout > 0);
   assert (_select_max_fd >= 0);
   
   // timeout
   
   struct timeval tv;
   int timeout_ms = math::floor_int (timeout * 1000.0);
   tv.tv_sec = timeout_ms / 1000;
   tv.tv_usec = (timeout_ms % 1000) * 1000;
   
   // copy fd sets
   
   fd_set select_read_cur = _select_base_arr [Type_READ];
   fd_set select_write_cur = _select_base_arr [Type_WRITE];
   
   int select_max_fd = _select_max_fd;
   FdInfoMap fd_info_map_read = _fd_info_map_arr [Type_READ];
   FdInfoMap fd_info_map_write = _fd_info_map_arr [Type_WRITE];
   
   // select
   
   int nbr_fd = ::select (select_max_fd + 1, &select_read_cur, &select_write_cur, 0, &tv);
   
   const double start_time = sys::TimerReliable::use_instance ().get ();
   
   if (nbr_fd == -1)
   {
      if (errno != EINTR)
      {
         TRACEX1 ("'select' failed with error `%1%'", strerror (errno));
         assert (false);
         
         //throw std::runtime_error ("FdEventMgr::wait: select failed");
      }
   }
   else if (nbr_fd == 0)
   {
      if (_process_timeout != 0)
      {
         _process_timeout (_ctx_timeout_ptr);
      }
   }
   else
   {
      for (int fd = 0 ; fd <= select_max_fd ; ++fd)
      {
         if (FD_ISSET (fd, &select_read_cur))
         {
            signal (fd_info_map_read, fd);
         }

         if (FD_ISSET (fd, &select_write_cur))
         {
            signal (fd_info_map_write, fd);
         }
      }
   }
   
   const double end_time = sys::TimerReliable::use_instance ().get ();
   
   _process_time = end_time - start_time;
}



/*
==============================================================================
Name : get_process_time
==============================================================================
*/

double   FdEventMgr::get_process_time () const
{
   return _process_time;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : signal
==============================================================================
*/

void  FdEventMgr::signal (FdInfoMap & fd_info_map, int fd)
{
   FdInfoMap::iterator it = fd_info_map.find (fd);
   assert (it != fd_info_map.end ());
   
   Info & info = it->second;
   
   ProcessFnc process_fnc = info._process_fnc;
   assert (process_fnc != 0);
   
   process_fnc (info._ctx_ptr);
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

