/*****************************************************************************

        Metrics.cpp
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

#include "plex/shared/Metrics.h"

#include "ohm/lang/fnc.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

Metrics::Metrics ()
:  _type (Type_INVALID)

,  _nbr_tx_max (0)
,  _nbr_ping_failed_max (0)
,  _nbr_user_max (0)
,  _nbr_session_max (0)

,  _proc_all_max_us (0)
,  _proc_fd_max_us (0)
,  _proc_toc_max_us (0)
,  _proc_tos_max_us (0)
,  _proc_recf_max_us (0)
,  _proc_rech_max_us (0)
,  _proc_res_max_us (0)
,  _proc_nch_max_us (0)
,  _proc_af_max_us (0)
,  _proc_ah_max_us (0)
,  _proc_bi_max_us (0)
,  _proc_ubi_max_us (0)

,  _metric_list (0)
,  _cur_time (time (0))

,  _block_s (0)
,  _nbr_metric_max (0)
,  _mark_period_s (0)
,  _date_format_0 ("")

,  _block_nbr (0)
{
   _metric_list.push_back (Metric ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Metrics::~Metrics ()
{
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  Metrics::init (Type type)
{
   _type = type;
   
   switch (type)
   {
   case Type_RL:
      init_rl ();
      break;

   case Type_DAY:
      init_day ();
      break;

   case Type_WEEK:
      init_week ();
      break;
   
   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name : post_process
==============================================================================
*/

void  Metrics::post_process (double start_time_s, size_t nbr_user, size_t nbr_session)
{
   using namespace ohm;
   
   double now = sys::TimerReliable::use_instance ().get ();
   time_t cur_time = time (0);
   
   archi::Int64 cur_block_nbr = archi::Int64 (floor (
      (now - start_time_s) / double (_block_s)
   ));
   
   while (cur_block_nbr > _block_nbr)
   {
      Metric & metric = *_metric_list.insert (_metric_list.end (), Metric ());
      _cur_time = cur_time;
      
      metric._nbr_user_max = nbr_user;
      metric._nbr_session_max = nbr_session;
      
      ++_block_nbr;
   }

   bool pop_flag = false;
   
   while (_metric_list.size () > _nbr_metric_max)
   {
      _metric_list.pop_front ();
      
      pop_flag = true;
   }
   
   if (pop_flag)
   {
      recalculate ();
   }
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  Metrics::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   using namespace ohm;
   
   bso.write (archi::UInt32 (_nbr_tx_max));
   bso.write (archi::UInt32 (_nbr_ping_failed_max));
   bso.write (archi::UInt32 (_nbr_user_max));
   bso.write (archi::UInt32 (_nbr_session_max));
   bso.write (archi::UInt32 (_proc_all_max_us));
   bso.write (archi::UInt32 (_proc_fd_max_us));
   bso.write (archi::UInt32 (_proc_toc_max_us));
   bso.write (archi::UInt32 (_proc_tos_max_us));
   bso.write (archi::UInt32 (_proc_recf_max_us));
   bso.write (archi::UInt32 (_proc_rech_max_us));
   bso.write (archi::UInt32 (_proc_res_max_us));
   bso.write (archi::UInt32 (_proc_nch_max_us));
   bso.write (archi::UInt32 (_proc_af_max_us));
   bso.write (archi::UInt32 (_proc_ah_max_us));
   bso.write (archi::UInt32 (_proc_bi_max_us));
   bso.write (archi::UInt32 (_proc_ubi_max_us));
   bso.write (_metric_list.begin (), _metric_list.end ());
   bso.write (archi::UInt32 (_cur_time));
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   Metrics::read (ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (_nbr_tx_max, "bsi:sess:stats:nbr_tx_max");
   bsi.read_uint32 (_nbr_ping_failed_max, "bsi:sess:stats:nbr_ping_failed_max");
   bsi.read_uint32 (_nbr_user_max, "bsi:sess:stats:nbr_user_max");
   bsi.read_uint32 (_nbr_session_max, "bsi:sess:stats:nbr_session_max");
   bsi.read_uint32 (_proc_all_max_us, "bsi:sess:stats:proc_all_max_us");
   bsi.read_uint32 (_proc_fd_max_us, "bsi:sess:stats:proc_fd_max_us");
   bsi.read_uint32 (_proc_toc_max_us, "bsi:sess:stats:proc_toc_max_us");
   bsi.read_uint32 (_proc_tos_max_us, "bsi:sess:stats:proc_tos_max_us");
   bsi.read_uint32 (_proc_recf_max_us, "bsi:sess:stats:proc_recf_max_us");
   bsi.read_uint32 (_proc_rech_max_us, "bsi:sess:stats:proc_rech_max_us");
   bsi.read_uint32 (_proc_res_max_us, "bsi:sess:stats:proc_res_max_us");
   bsi.read_uint32 (_proc_nch_max_us, "bsi:sess:stats:proc_nch_max_us");
   bsi.read_uint32 (_proc_af_max_us, "bsi:sess:stats:proc_af_max_us");
   bsi.read_uint32 (_proc_ah_max_us, "bsi:sess:stats:proc_ah_max_us");
   bsi.read_uint32 (_proc_bi_max_us, "bsi:sess:stats:proc_bi_max_us");
   bsi.read_uint32 (_proc_ubi_max_us, "bsi:sess:stats:proc_ubi_max_us");
   
   _metric_list.clear ();
   bsi.read (_metric_list, "bsi:sess:stats:metric_list");

   bsi.read_uint32 (_cur_time, "bsi:sess:stats:cur_time");
   
   return 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init_rl
==============================================================================
*/

void  Metrics::init_rl ()
{
   _block_s = 1;
   _nbr_metric_max = 120;
   _mark_period_s = 20;
   _date_format_0 = "%H:%M:%S";
}



/*
==============================================================================
Name : init_day
==============================================================================
*/

void  Metrics::init_day ()
{
   _block_s = 10 * 60;
   _nbr_metric_max = 144;
   _mark_period_s = 4 * 60 * 60;
   _date_format_0 = "%d %b %Hh";
}



/*
==============================================================================
Name : init_week
==============================================================================
*/

void  Metrics::init_week ()
{
   _block_s = 60 * 60;
   _nbr_metric_max = 168;
   _mark_period_s = 24 * 60 * 60;
   _date_format_0 = "%a %d %b";
}



/*
==============================================================================
Name : recalculate
==============================================================================
*/

void  Metrics::recalculate ()
{
   using namespace ohm;
   
   _nbr_tx_max = 0;
   _nbr_ping_failed_max = 0;
   _nbr_user_max = 0;
   _nbr_session_max = 0;
   _proc_all_max_us = 0;
   _proc_fd_max_us = 0;
   _proc_toc_max_us = 0;
   _proc_tos_max_us = 0;
   _proc_recf_max_us = 0;
   _proc_rech_max_us = 0;
   _proc_res_max_us = 0;
   _proc_nch_max_us = 0;
   _proc_af_max_us = 0;
   _proc_ah_max_us = 0;
   _proc_bi_max_us = 0;
   _proc_ubi_max_us = 0;
   
   MetricList::const_iterator it = _metric_list.begin ();
   const MetricList::const_iterator it_end = _metric_list.end ();
   
   for (; it != it_end ; ++it)
   {
      const Metric & metric = *it;
      
      lang::max_in_place (_nbr_tx_max, metric._nbr_tx);
      lang::max_in_place (_nbr_ping_failed_max, metric._nbr_ping_failed);
      lang::max_in_place (_nbr_user_max, metric._nbr_user_max);
      lang::max_in_place (_nbr_session_max, metric._nbr_session_max);
      lang::max_in_place (_proc_all_max_us, metric._proc_all_max_us);
      lang::max_in_place (_proc_fd_max_us, metric._proc_fd_max_us);
      lang::max_in_place (_proc_toc_max_us, metric._proc_toc_max_us);
      lang::max_in_place (_proc_tos_max_us, metric._proc_tos_max_us);
      lang::max_in_place (_proc_recf_max_us, metric._proc_recf_max_us);
      lang::max_in_place (_proc_rech_max_us, metric._proc_rech_max_us);
      lang::max_in_place (_proc_res_max_us, metric._proc_res_max_us);
      lang::max_in_place (_proc_nch_max_us, metric._proc_nch_max_us);
      lang::max_in_place (_proc_af_max_us, metric._proc_af_max_us);
      lang::max_in_place (_proc_ah_max_us, metric._proc_ah_max_us);
      lang::max_in_place (_proc_bi_max_us, metric._proc_bi_max_us);
      lang::max_in_place (_proc_ubi_max_us, metric._proc_ubi_max_us);
   }
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

