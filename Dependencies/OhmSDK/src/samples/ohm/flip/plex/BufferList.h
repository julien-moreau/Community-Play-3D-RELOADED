/*****************************************************************************

        BufferList.h
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



#if ! defined (plex_BufferList_HEADER_INCLUDED)
#define  plex_BufferList_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/FdEventMgr.h"

#include "ohm/archi/types.h"

#include <list>
#include <vector>



namespace plex
{



class Central;

class BufferList
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BufferList (Central & central, int fd, void * ctx_ptr, FdEventMgr::ProcessFnc process_fnc);
                  BufferList (Central & central, int fd);
   virtual        ~BufferList ();
   
   void           push (std::vector <ohm::archi::UByte> & data);
   
   bool           empty () const;
   const ohm::archi::UByte *
                  get_data_ptr () const;
   size_t         get_data_size () const;
   void           pop (size_t size);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class Buffer
   {
   public:
      size_t      _pos;
      std::vector <ohm::archi::UByte>
                  _data;
   };
   
   Central &      _central;
   void * const   _ctx_ptr;
   const FdEventMgr::ProcessFnc
                  _process_fnc;
   const int      _fd;

   bool           _write_reg_flag;
   std::list <Buffer>
                  _list;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BufferList ();
                  BufferList (const BufferList &other);
   BufferList &   operator = (const BufferList &other);
   bool           operator == (const BufferList &other);
   bool           operator != (const BufferList &other);

}; // class BufferList



}  // namespace plex



//#include  "BufferList.hpp"



#endif   // plex_BufferList_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

