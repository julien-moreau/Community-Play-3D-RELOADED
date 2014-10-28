/*****************************************************************************

        Stats.cpp
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

#include "plex/Stats.h"

#include "plex/Central.h"

#include "ohm/lang/fnc.h"
#include "ohm/math/fnc.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Stats::Stats (Central & central)
:  _central (central)
,  _stats_data ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Stats::~Stats ()
{
}



/*
==============================================================================
Name : post_process
==============================================================================
*/

void  Stats::post_process ()
{
   _stats_data.post_process ();
}



/*
==============================================================================
Name : notify_process_duration
==============================================================================
*/

void  Stats::notify_process_duration (size_t (Metrics::*metrics_var), size_t (Metric::*metric_var), double process_duration)
{
   using namespace ohm;
   
   size_t reply_time_ms = math::floor_int (process_duration * 1000000.0);
   
   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      Metrics & metrics = _stats_data._metrics_arr [i];
      Metric & metric = metrics._metric_list.back ();
      size_t & reply_time_max_ms = metric.*metric_var;
      
      lang::max_in_place (reply_time_max_ms, reply_time_ms);
      lang::max_in_place (metrics.*metrics_var, reply_time_ms);
   }
}



/*
==============================================================================
Name : increment_ping_failed
==============================================================================
*/

void  Stats::increment_ping_failed ()
{
   using namespace ohm;
   
   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      Metrics & metrics = _stats_data._metrics_arr [i];
      size_t & nbr_ping_failed = metrics._metric_list.back ()._nbr_ping_failed;
      
      nbr_ping_failed += 1;
      
      lang::max_in_place (metrics._nbr_ping_failed_max, nbr_ping_failed);
   }
}



/*
==============================================================================
Name : session_add
==============================================================================
*/

void  Stats::session_add (const std::string & session)
{
   using namespace ohm;
   
   _stats_data._nbr_session += 1;

   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      Metrics & metrics = _stats_data._metrics_arr [i];
      size_t & nbr_session_max = metrics._metric_list.back ()._nbr_session_max;
      
      lang::max_in_place (nbr_session_max, _stats_data._nbr_session);
      lang::max_in_place (metrics._nbr_session_max, _stats_data._nbr_session);
   }
   
   _stats_data._session_map [session]._id = session;
}



/*
==============================================================================
Name : session_remove
==============================================================================
*/

void  Stats::session_remove (const std::string & session)
{
   assert (_stats_data._nbr_session > 0);

   _stats_data._nbr_session -= 1;
   
   _stats_data._session_map.erase (session);
}



/*
==============================================================================
Name : session_add_client
==============================================================================
*/

void  Stats::session_add_client (const std::string & session, const std::string & username)
{
   using namespace ohm;
   
   _stats_data._nbr_user += 1;
   
   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      Metrics & metrics = _stats_data._metrics_arr [i];
      size_t & nbr_user_max = metrics._metric_list.back ()._nbr_user_max;
      
      lang::max_in_place (nbr_user_max, _stats_data._nbr_user);
      lang::max_in_place (metrics._nbr_user_max, _stats_data._nbr_user);
   }

   _stats_data._session_map [session]._client_set.insert (username);
}



/*
==============================================================================
Name : session_remove_client
==============================================================================
*/

void  Stats::session_remove_client (const std::string & session, const std::string & username)
{
   assert (_stats_data._nbr_user > 0);

   _stats_data._nbr_user -= 1;
   
   _stats_data._session_map [session]._client_set.erase (username);
}



/*
==============================================================================
Name : session_increment_nbr_tx
==============================================================================
*/

void  Stats::session_increment_nbr_tx (const std::string & session)
{
   using namespace ohm;
   
   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      Metrics & metrics = _stats_data._metrics_arr [i];
      size_t & nbr_tx = metrics._metric_list.back ()._nbr_tx;
      
      nbr_tx += 1;
      
      lang::max_in_place (metrics._nbr_tx_max, nbr_tx);     
   }
   
   _stats_data._session_map [session]._nbr_tx += 1;
}



/*
==============================================================================
Name : session_set_state_run
==============================================================================
*/

void  Stats::session_set_state_run (const std::string & session)
{
   _stats_data._session_map [session]._state = SessionStats::State_RUN;
}



/*
==============================================================================
Name : session_set_state_end
==============================================================================
*/

void  Stats::session_set_state_end (const std::string & session)
{
   _stats_data._session_map [session]._state = SessionStats::State_END;
}



/*
==============================================================================
Name : session_set_proc_stat
==============================================================================
*/

void  Stats::session_set_proc_stat (const std::string & session, size_t mem_size, size_t cpu)
{
   _stats_data._session_map [session]._mem_size = mem_size;
   _stats_data._session_map [session]._cpu = cpu;
}



/*
==============================================================================
Name : session_set_storage_idle
==============================================================================
*/

void  Stats::session_set_storage_idle (const std::string & session)
{
   _stats_data._session_map [session]._storage = SessionStats::Storage_IDLE;
   _stats_data._session_map [session]._storage_cur = 0;
   _stats_data._session_map [session]._storage_total = 0;
}



/*
==============================================================================
Name : session_set_storage_reading
==============================================================================
*/

void  Stats::session_set_storage_reading (const std::string & session, size_t cur)
{
   _stats_data._session_map [session]._storage = SessionStats::Storage_READING;
   _stats_data._session_map [session]._storage_cur = cur;
   _stats_data._session_map [session]._storage_total = cur;
}



/*
==============================================================================
Name : session_set_storage_writing
==============================================================================
*/

void  Stats::session_set_storage_writing (const std::string & session, size_t cur, size_t total)
{
   _stats_data._session_map [session]._storage = SessionStats::Storage_WRITING;
   _stats_data._session_map [session]._storage_cur = cur;
   _stats_data._session_map [session]._storage_total = total;
}



/*
==============================================================================
Name : session_set_storage_copying
==============================================================================
*/

void  Stats::session_set_storage_copying (const std::string & session)
{
   _stats_data._session_map [session]._storage = SessionStats::Storage_COPYING;
   _stats_data._session_map [session]._storage_cur = 0;
   _stats_data._session_map [session]._storage_total = 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

