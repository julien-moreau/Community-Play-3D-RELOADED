/*****************************************************************************

        FdEventMgr.h
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



#if ! defined (plex_FdEventMgr_HEADER_INCLUDED)
#define  plex_FdEventMgr_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <sys/socket.h>
#include <arpa/inet.h>

#include <set>



namespace plex
{



class FdEventMgr
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Type
   {
                  Type_READ = 0,
                  Type_WRITE,
                  
                  Type_NBR_ELT
   };
   
   typedef void   (*ProcessFnc) (void * ctx_ptr);
   
                  FdEventMgr ();
                  FdEventMgr (void * ctx_ptr, ProcessFnc process_timeout);
   virtual        ~FdEventMgr ();
   
   void           insert (Type type, int fd, void * ctx_ptr, ProcessFnc process_fnc);
   void           erase (Type type, int fd);
   
   void           wait (double timeout);
   
   double         get_process_time () const;
   
   static const char *
                  _ns_log_0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class Info
   {
   public:
      void *      _ctx_ptr;
      ProcessFnc  _process_fnc;
   };
   
   typedef std::map <int, Info> FdInfoMap;
   
   void           signal (FdInfoMap & fd_info_map, int fd);
   
   void * const   _ctx_timeout_ptr;
   ProcessFnc     _process_timeout;
   
   FdInfoMap      _fd_info_map_arr [Type_NBR_ELT];
   
   int            _select_max_fd;
   fd_set         _select_base_arr [Type_NBR_ELT];
   
   double         _process_time;
      


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  FdEventMgr (const FdEventMgr &other);
   FdEventMgr &   operator = (const FdEventMgr &other);
   bool           operator == (const FdEventMgr &other);
   bool           operator != (const FdEventMgr &other);

}; // class FdEventMgr



}  // namespace plex



//#include  "FdEventMgr.hpp"



#endif   // plex_FdEventMgr_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

