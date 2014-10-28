/*****************************************************************************

        PageLogs.h
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



#if ! defined (plex_PageLogs_HEADER_INCLUDED)
#define  plex_PageLogs_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/http/PageBase.h"
#include "plex/http/HttpStats.h"



namespace plex
{



class HttpCentral;
class LogNamespace;

class PageLogs
:  public PageBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PageLogs (HttpCentral & central);
   virtual        ~PageLogs ();
   
   static PageBase *
                  create (HttpCentral & central);
   
   // PageBase
   virtual void   process_body ();
   virtual void   process_xhr ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class LogBnd
   {
   public:
      bool        operator < (const LogBnd & other) const {return _order < other._order;}
      int         _order;
      const LogNamespace *
                  _log_ns_ptr;
   };
   
   typedef std::list <LogBnd> LogBndList;
   
   typedef std::map <std::string, int> NsOrderMap;
   
   void           process_body (const LogNamespace & log_ns);
   void           process_xhr (const LogNamespace & log_ns);
   
   NsOrderMap     _ns_order_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PageLogs ();
                  PageLogs (const PageLogs &other);
   PageLogs &     operator = (const PageLogs &other);
   bool           operator == (const PageLogs &other);
   bool           operator != (const PageLogs &other);

}; // class PageLogs



}  // namespace plex



//#include  "PageLogs.hpp"



#endif   // plex_PageLogs_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

