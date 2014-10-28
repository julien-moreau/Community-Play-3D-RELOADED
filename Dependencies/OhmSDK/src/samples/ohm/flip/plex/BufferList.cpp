/*****************************************************************************

        BufferList.cpp
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

#include "plex/BufferList.h"

#include "plex/Central.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

BufferList::BufferList (Central & central, int fd, void * ctx_ptr, FdEventMgr::ProcessFnc process_fnc)
:  _central (central)
,  _ctx_ptr (ctx_ptr)
,  _process_fnc (process_fnc)
,  _fd (fd)

,  _write_reg_flag (false)
,  _list ()
{
   assert (_process_fnc != 0);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

BufferList::BufferList (Central & central, int fd)
:  _central (central)
,  _ctx_ptr (0)
,  _process_fnc (0)
,  _fd (fd)

,  _write_reg_flag (false)
,  _list ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BufferList::~BufferList ()
{
   try
   {
      if (_write_reg_flag)
      {
         FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
         
         evt_mgr.erase (FdEventMgr::Type_WRITE, _fd);
         
         _write_reg_flag = false;
      }
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : push
Description :
   Push more data to send over the connection.
   On return, 'data' will be empty (it swapped internally)
==============================================================================
*/

void  BufferList::push (std::vector <ohm::archi::UByte> & data)
{
   Buffer & buffer = *_list.insert (_list.end (), Buffer ());
   buffer._pos = 0;
   buffer._data.swap (data);

   if (!_write_reg_flag)
   {
      FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
      
      evt_mgr.insert (FdEventMgr::Type_WRITE, _fd, _ctx_ptr, _process_fnc);
      
      _write_reg_flag = true;
   }
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  BufferList::empty () const
{
   return _list.empty ();
}



/*
==============================================================================
Name : get_data_ptr
==============================================================================
*/

const ohm::archi::UByte *  BufferList::get_data_ptr () const
{
   using namespace ohm;
   
   assert (!empty ());
   
   const Buffer & buffer = _list.front ();
   const std::vector <archi::UByte> & data = buffer._data;
   
   assert (buffer._pos < data.size ());
   
   return &data [buffer._pos];
}



/*
==============================================================================
Name : get_data_size
==============================================================================
*/

size_t   BufferList::get_data_size () const
{
   using namespace ohm;
   
   assert (!empty ());
   
   const Buffer & buffer = _list.front ();
   const std::vector <archi::UByte> & data = buffer._data;
   
   assert (buffer._pos < data.size ());
   
   return data.size () - buffer._pos;
}



/*
==============================================================================
Name : pop
==============================================================================
*/

void  BufferList::pop (size_t size)
{
   using namespace ohm;
   
   assert (!empty ());
   
   Buffer & buffer = _list.front ();
   std::vector <archi::UByte> & data = buffer._data;
   
   buffer._pos += size;
      
   assert (buffer._pos <= data.size ());
   
   if (buffer._pos == data.size ())
   {
      _list.pop_front ();
   }

   if (_write_reg_flag && _list.empty ())
   {
      FdEventMgr & evt_mgr = _central.use_fd_event_mgr ();
   
      evt_mgr.erase (FdEventMgr::Type_WRITE, _fd);
      
      _write_reg_flag = false;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

