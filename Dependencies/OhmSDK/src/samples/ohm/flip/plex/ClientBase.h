/*****************************************************************************

        ClientBase.h
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



#if ! defined (plex_ClientBase_HEADER_INCLUDED)
#define  plex_ClientBase_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/Connection.h"

#include "ohm/archi/types.h"

#include <vector>

#include <sys/socket.h>
#include <arpa/inet.h>



namespace plex
{



class Central;
class FdEventMgr;

class ClientBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ClientBase (Central & central, int sd);
   virtual        ~ClientBase ();
   
   int            get_sd () const;
   
   static void    receive (void * ctx_ptr);
   void           receive ();

   static void    send (void * ctx_ptr);
   void           send ();
   
   void           push (std::vector <ohm::archi::UByte> & data);
   void           push (const ohm::archi::UByte * data_ptr, size_t data_size);
   void           push (const char * str_0);
   
   void           terminate ();
   bool           need_release ();

   double         get_read_ts () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual bool   do_process (int & err) = 0;
   
   inline size_t  size () const {return _data.size ();}
   inline ohm::archi::UByte
                  get_byte (size_t pos) const {assert (pos < size ()); return _data [pos];}
   void           pop (size_t size);
   const ohm::archi::UByte *
                  get_data_ptr (size_t pos, size_t size);
   size_t         find (const char * str_0) const;
   
   void           trace_escaped_ascii ();
   
   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           process ();
   
   Central &      _central;
   const int      _sd;
   Connection     _cnx;
   
   std::vector <ohm::archi::UByte>
                  _data;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClientBase ();
                  ClientBase (const ClientBase &other);
   ClientBase &         operator = (const ClientBase &other);
   bool           operator == (const ClientBase &other);
   bool           operator != (const ClientBase &other);

}; // class ClientBase



}  // namespace plex



//#include  "ClientBase.hpp"



#endif   // plex_ClientBase_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

