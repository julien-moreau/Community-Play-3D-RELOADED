/*****************************************************************************

        PageSessions.cpp
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

#include "plex/http/PageSessions.h"

#include "plex/http/HttpStats.h"
#include "plex/shared/Configuration.h"

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

PageSessions::PageSessions (HttpCentral & central)
:  PageBase (central, "SkeletonServer: Sessions")
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PageSessions::~PageSessions ()
{
}



/*
==============================================================================
Name : create
==============================================================================
*/

PageBase *  PageSessions::create (HttpCentral & central)
{
   return new PageSessions (central);
}




/*
==============================================================================
Name : process
==============================================================================
*/

void  PageSessions::process_body ()
{
   line ("<p class=\"section\">Sessions</p>");
   line ("<div id=\"sessions\" class=\"dsection\">");
   process_xhr_sessions ();
   line ("</div>");
}



/*
==============================================================================
Name : process_xhr
==============================================================================
*/

void  PageSessions::process_xhr ()
{
   process_xhr_sessions ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_xhr_sessions
==============================================================================
*/

void  PageSessions::process_xhr_sessions ()
{
   line ("<table border=0 cellpadding=0 cellspacing=0 width=100%>");

   line ("<tr class=\"tr_item\">");
   line ("<td class=\"tr_h\">Session</td>");
   line ("<td class=\"tr_h\">State</td>");
   line ("<td class=\"tr_h\">Storage</td>");
   line ("<td class=\"tr_h\">Memory Used</td>");
   line ("<td class=\"tr_h\">CPU Used</td>");
   line ("<td class=\"tr_h\">Nbr Tx</td>");
   line ("<td class=\"tr_h\">Nbr Clients</td>");
   line ("<td class=\"tr_h\">Clients</td>");
   line ("</tr>");
   
   const HttpStats & stats = HttpStats::use_instance ();
   const StatsData & stats_data = stats._stats_data;
   
   SessionStatsMap::const_iterator it = stats_data._session_map.begin ();
   const SessionStatsMap::const_iterator it_end = stats_data._session_map.end ();
   
   for (; it != it_end ; ++it)
   {
      const SessionStats & session = it->second;
      
      process_xhr_sessions (session);
   }
   
   line ("</table>");
   
   flush_script ("sessions");
}



/*
==============================================================================
Name : process_xhr_sessions
==============================================================================
*/

void  PageSessions::process_xhr_sessions (const SessionStats & session)
{
   line ("<tr class=\"tr_item\" bgcolor=\"#6b6b6b\" onMouseOver=\"this.bgColor='#777777';\" onMouseOut=\"this.bgColor='#6b6b6b';\">");
   
   process_xhr_sessions_session (session);
   process_xhr_sessions_state (session);
   process_xhr_sessions_storage (session);
   process_xhr_sessions_proc_stat (session);
   process_xhr_sessions_nbr_tx (session);
   process_xhr_sessions_nbr_client (session);
   process_xhr_sessions_clients (session);
   
   line ("</tr>");
}



/*
==============================================================================
Name : process_xhr_sessions
==============================================================================
*/

void  PageSessions::process_xhr_sessions_session (const SessionStats & session)
{
   line ("<td>" + session._id + "</td>");
}



/*
==============================================================================
Name : process_xhr_sessions_state
==============================================================================
*/

void  PageSessions::process_xhr_sessions_state (const SessionStats & session)
{
   std::string state;
   
   switch (session._state)
   {
   case SessionStats::State_LOAD:
      state = "LOADING";
      break;

   case SessionStats::State_RUN:
      state = "RUNNING";
      break;

   case SessionStats::State_END:
      state = "ENDING";
      break;
   }

   line ("<td>" + state + "</td>");
}



/*
==============================================================================
Name : process_xhr_sessions_storage
==============================================================================
*/

void  PageSessions::process_xhr_sessions_storage (const SessionStats & session)
{
   std::string state;
   
   switch (session._storage)
   {
   case SessionStats::Storage_IDLE:
      state = "IDLE";
      break;

   case SessionStats::Storage_READING:
      state = "LOADING ";
      state += StatsData::to_mem_str (session._storage_cur);
      break;

   case SessionStats::Storage_WRITING:
      state = "FLUSHING ";
      state += StatsData::to_mem_str (session._storage_cur);
      state += " of ";
      state += StatsData::to_mem_str (session._storage_total);
      break;

   case SessionStats::Storage_COPYING:
      state = "COPYING";
      break;
   }

   line ("<td>" + state + "</td>");
}



/*
==============================================================================
Name : process_xhr_sessions_proc_stat
==============================================================================
*/

void  PageSessions::process_xhr_sessions_proc_stat (const SessionStats & session)
{
   using namespace ohm;
   
   size_t size = session._mem_size;
   
   std::string mem_str = StatsData::to_mem_str (size);
   
   line ("<td>" + mem_str + "</td>");
   
   double cpu = double (session._cpu) / 100.0;
   
   txt::out::Print printer;
   printer.print ("<td>%1%</td>").arg (cpu);
   
   line (printer);
}



/*
==============================================================================
Name : process_xhr_sessions_nbr_tx
==============================================================================
*/

void  PageSessions::process_xhr_sessions_nbr_tx (const SessionStats & session)
{
   using namespace ohm;
   
   txt::out::Print printer;
   printer.print ("<td>%1%</td>").arg (session._nbr_tx);
   
   line (printer);
}



/*
==============================================================================
Name : process_xhr_sessions_nbr_client
==============================================================================
*/

void  PageSessions::process_xhr_sessions_nbr_client (const SessionStats & session)
{
   using namespace ohm;
   
   txt::out::Print printer;
   printer.print ("<td>%1%</td>").arg (session._client_set.size ());
   
   line (printer);
}



/*
==============================================================================
Name : process_xhr_sessions_clients
==============================================================================
*/

void  PageSessions::process_xhr_sessions_clients (const SessionStats & session)
{
   std::string str;
   
   std::set <std::string>::const_iterator it = session._client_set.begin ();
   const std::set <std::string>::const_iterator it_end = session._client_set.end ();
   
   if (it != it_end)
   {
      std::set <std::string>::const_iterator it_last = it_end;
      --it_last;
      
      for (; it != it_end ; ++it)
      {
         const std::string & username = *it;
         
         str += username;
         
         if (it != it_last)
         {
            str += ", ";
         }
      }
   }
   
   line ("<td>" + str + "</td>");
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

