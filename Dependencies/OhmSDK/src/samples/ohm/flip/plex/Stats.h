/*****************************************************************************

        Stats.h
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



#if ! defined (plex_Stats_HEADER_INCLUDED)
#define  plex_Stats_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/shared/StatsData.h"



namespace plex
{



class Central;

class Stats
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Stats (Central & central);
   virtual        ~Stats ();
   
   void           post_process ();
   
   void           notify_process_duration (size_t (Metrics::*metrics_var), size_t (Metric::*metric_var), double process_duration);
   
   void           increment_ping_failed ();

   void           session_add (const std::string & session);
   void           session_remove (const std::string & session);
   void           session_add_client (const std::string & session, const std::string & username);
   void           session_remove_client (const std::string & session, const std::string & username);
   void           session_increment_nbr_tx (const std::string & session);
   void           session_set_state_run (const std::string & session);
   void           session_set_state_end (const std::string & session);
   void           session_set_proc_stat (const std::string & session, size_t mem_size, size_t cpu);
   void           session_set_storage_idle (const std::string & session);
   void           session_set_storage_reading (const std::string & session, size_t cur);
   void           session_set_storage_writing (const std::string & session, size_t cur, size_t total);
   void           session_set_storage_copying (const std::string & session);

   StatsData      _stats_data;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   Central &      _central;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Stats ();
                  Stats (const Stats &other);
   Stats &        operator = (const Stats &other);
   bool           operator == (const Stats &other);
   bool           operator != (const Stats &other);

}; // class Stats



}  // namespace plex



//#include  "Stats.hpp"



#endif   // plex_Stats_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

