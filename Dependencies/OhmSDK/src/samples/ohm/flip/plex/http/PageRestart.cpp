/*****************************************************************************

        PageRestart.cpp
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

#include "plex/http/PageRestart.h"

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

PageRestart::PageRestart (HttpCentral & central)
:  PageBase (central, "SkeletonServer: Restart")
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PageRestart::~PageRestart ()
{
}



/*
==============================================================================
Name : create
==============================================================================
*/

PageBase *  PageRestart::create (HttpCentral & central)
{
   return new PageRestart (central);
}




/*
==============================================================================
Name : process_body
==============================================================================
*/

void  PageRestart::process_body ()
{
   // process_body_restart ();
   
   process_body_shutdown ();
}



/*
==============================================================================
Name : process_xhr
==============================================================================
*/

void  PageRestart::process_xhr ()
{
   line ("<table border=0 cellpadding=0 cellspacing=0 width=100%>");
   
   process_tr ("Started On:", HttpStats::use_instance ().get_start_date ());
   process_tr ("Running Since:", HttpStats::use_instance ().get_running_time ());
   
   line ("</table>");
   
   flush_script ("timings");
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_body_restart
Note :
   Not used for now.
==============================================================================
*/

void  PageRestart::process_body_restart ()
{
   const std::string url = "/restart.do";
   
   line ("<p class=\"section\">Restart</p>");
   line ("<div id=\"restart\" class=\"dsection\">");

   line ("<div id=\"timings\">");
   process_xhr ();
   line ("</div>");
   
   line ("<br/>");
   line ("<center><p>This button will flush all ongoing sessions, and restart SkeletonServer</p></center>");
   line ("<br/>");
   line ("<center><button type=\"submit\" onclick=\"signal('" + url + "')\" style=\"color:black;width:180px;height:30px;\">Restart</button></center>");
   
   line ("</div>");
}



/*
==============================================================================
Name : process_body_shutdown
==============================================================================
*/

void  PageRestart::process_body_shutdown ()
{
   const std::string url = "/shutdown.do";
   
   line ("<p class=\"section\">Shutdown</p>");
   line ("<div id=\"shutdown\" class=\"dsection\">");

   line ("<div id=\"timings\">");
   process_xhr ();
   line ("</div>");
   
   line ("<br/>");
   line ("<center><p>This button will flush all ongoing sessions, and shutdown SkeletonServer</p></center>");
   line ("<br/>");
   line ("<center><button type=\"submit\" onclick=\"signal('" + url + "')\" style=\"color:black;width:180px;height:30px;\">Shutdown</button></center>");
   
   line ("</div>");
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

