/*****************************************************************************

        PageLogs.cpp
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

#include "plex/http/PageLogs.h"

#include "plex/http/HttpCentral.h"
#include "plex/shared/Configuration.h"
#include "plex/shared/LogMgr.h"

#include "ohm/txt/out/Print.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PageLogs::PageLogs (HttpCentral & central)
:  PageBase (central, "SkeletonServer: Logs")
{
   int order = 0;
   
   _ns_order_map ["Incidents"] = order ++;
   _ns_order_map ["Central"] = order ++;
   _ns_order_map ["ClientFlip"] = order ++;
   _ns_order_map ["Session"] = order ++;
   _ns_order_map ["Process"] = order ++;
   _ns_order_map ["FdEventMgr"] = order ++;
   _ns_order_map ["Service Port"] = order ++;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PageLogs::~PageLogs ()
{
}



/*
==============================================================================
Name : create
==============================================================================
*/

PageBase *  PageLogs::create (HttpCentral & central)
{
   return new PageLogs (central);
}




/*
==============================================================================
Name : process
==============================================================================
*/

void  PageLogs::process_body ()
{
   LogBndList log_bnd_list;
   
   {
      const LogNamespaceMap & log_ns_map = LogMgr::use ().get_log_ns_map ();
      
      LogNamespaceMap::const_iterator it = log_ns_map.begin ();
      const LogNamespaceMap::const_iterator it_end = log_ns_map.end ();
      
      for (; it != it_end ; ++it)
      {
         const LogNamespace & log_ns = it->second;
         const std::string & ns = log_ns._namespace;
         
         LogBnd & log_bnd = *log_bnd_list.insert (log_bnd_list.end (), LogBnd ());
         log_bnd._order = _ns_order_map [ns];
         log_bnd._log_ns_ptr = &log_ns;
      }
      
      log_bnd_list.sort ();
   }

   {
      LogBndList::const_iterator it = log_bnd_list.begin ();
      const LogBndList::const_iterator it_end = log_bnd_list.end ();
      
      for (; it != it_end ; ++it)
      {
         const LogBnd & log_bnd = *it;
         const LogNamespace & log_ns = *log_bnd._log_ns_ptr;
         
         process_body (log_ns);
      }
   }
}



/*
==============================================================================
Name : process_xhr
==============================================================================
*/

void  PageLogs::process_xhr ()
{
   const LogNamespaceMap & log_ns_map = LogMgr::use ().get_log_ns_map ();
   
   LogNamespaceMap::const_iterator it = log_ns_map.begin ();
   const LogNamespaceMap::const_iterator it_end = log_ns_map.end ();
   
   for (; it != it_end ; ++it)
   {
      const LogNamespace & log_ns = it->second;
      
      process_xhr (log_ns);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_body
==============================================================================
*/

void  PageLogs::process_body (const LogNamespace & log_ns)
{
   const std::string namespace_str (log_ns._namespace);
   
   if (namespace_str == LogMgr::_ns_incident_0)
   {
      line ("<p class=\"sectionr\">" + namespace_str + "</p>");
      line ("<div class=\"dsectionr\">");
   }
   else
   {
      line ("<p class=\"section\">" + namespace_str + "</p>");
      line ("<div class=\"dsection\">");
   }

   line ("<div id=\"Ns" + namespace_str + "\">");

   process_xhr (log_ns);

   line ("</div>");
   
   line ("</div>");
}



/*
==============================================================================
Name : process_xhr
==============================================================================
*/

void  PageLogs::process_xhr (const LogNamespace & log_ns)
{
   if (!need_refresh (log_ns._timestamp))
   {
      return;
   }
   
   const std::string namespace_str (log_ns._namespace);
   
   std::string html;

   log_ns.make_html (html);
   
   line (html);
   
   flush_script ("Ns" + namespace_str);
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

