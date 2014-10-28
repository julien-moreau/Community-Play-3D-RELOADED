/*****************************************************************************

        PageSessions.h
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



#if ! defined (plex_PageSessions_HEADER_INCLUDED)
#define  plex_PageSessions_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/http/PageBase.h"



namespace plex
{



class HttpCentral;
class SessionStats;

class PageSessions
:  public PageBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PageSessions (HttpCentral & central);
   virtual        ~PageSessions ();
   
   static PageBase *
                  create (HttpCentral & central);
   
   // PageBase
   virtual void   process_body ();
   virtual void   process_xhr ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           process_xhr_sessions ();
   void           process_xhr_sessions (const SessionStats & session);
   void           process_xhr_sessions_session (const SessionStats & session);
   void           process_xhr_sessions_state (const SessionStats & session);
   void           process_xhr_sessions_storage (const SessionStats & session);
   void           process_xhr_sessions_proc_stat (const SessionStats & session);
   void           process_xhr_sessions_nbr_tx (const SessionStats & session);
   void           process_xhr_sessions_nbr_client (const SessionStats & session);
   void           process_xhr_sessions_clients (const SessionStats & session);
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PageSessions ();
                  PageSessions (const PageSessions &other);
   PageSessions & operator = (const PageSessions &other);
   bool           operator == (const PageSessions &other);
   bool           operator != (const PageSessions &other);

}; // class PageSessions



}  // namespace plex



//#include  "PageSessions.hpp"



#endif   // plex_PageSessions_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

