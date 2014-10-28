/*****************************************************************************

        PageState.cpp
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

#include "plex/http/PageState.h"

#include "plex/http/HttpCentral.h"
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

PageState::PageState (HttpCentral & central)
:  PageBase (central, "SkeletonServer: Status")
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

PageState::~PageState ()
{
}



/*
==============================================================================
Name : create
==============================================================================
*/

PageBase *  PageState::create (HttpCentral & central)
{
   return new PageState (central);
}




/*
==============================================================================
Name : process
==============================================================================
*/

void  PageState::process_body ()
{
   line ("<p class=\"section\">Configuration</p>");
   line ("<div id=\"conf\" class=\"dsection\">");
   process_xhr_conf ();
   line ("</div>");
   
   line ("<p class=\"section\">Status</p>");
   line ("<div id=\"status\" class=\"dsection\">");
   process_xhr_status ();
   line ("</div>");

   line ("<p class=\"section\">Realtime Metrics</p>");
   line ("<div id=\"rlmet\" class=\"dsection\">");
   process_xhr_rl_metrics ();
   line ("</div>");

   line ("<p class=\"section\">Day Metrics</p>");
   line ("<div id=\"daymet\" class=\"dsection\">");
   process_xhr_day_metrics ();
   line ("</div>");

   line ("<p class=\"section\">Week Metrics</p>");
   line ("<div id=\"weekmet\" class=\"dsection\">");
   process_xhr_week_metrics ();
   line ("</div>");
}



/*
==============================================================================
Name : process_xhr
==============================================================================
*/

void  PageState::process_xhr ()
{
   process_xhr_conf ();
   process_xhr_status ();
   process_xhr_rl_metrics ();
   process_xhr_day_metrics ();
   process_xhr_week_metrics ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_xhr_conf
==============================================================================
*/

void  PageState::process_xhr_conf ()
{
   const Configuration & conf = Configuration::get ();
   
   if (!need_refresh (conf._timestamp))
   {
      return;
   }

   line ("<table border=0 cellpadding=0 cellspacing=0 width=100%>");
   
   process_tr ("Binary Path", conf._bin_path);
   process_xhr_conf_format_version_set ();
   process_tr ("Port:", conf._port);
   process_tr ("Authentification", conf._no_auth_flag ? "NO" : "YES");
   process_xhr_conf_emergency_path ();
   process_tr ("Snapshot Flush Period", conf._snapshot_flush_period);
   process_tr ("Ping Period", conf._ping_period);
   process_tr ("Receive Timeout", conf._recv_timeout);
   
   line ("</table>");
   
   flush_script ("conf");
}



/*
==============================================================================
Name : process_xhr_conf_format_version_set
==============================================================================
*/

void  PageState::process_xhr_conf_format_version_set ()
{
   /*Central & central = use_client ().use_central ();
   
   const std::set <std::string> & format_version_set
      = central.use_install_mgr ().get_format_version_set ();
   
   std::set <std::string>::const_iterator it = format_version_set.begin ();
   const std::set <std::string>::const_iterator it_end = format_version_set.end ();
   
   for (; it != it_end ; ++it)
   {
      const std::string & format_version = *it;
      
      if (it == format_version_set.begin ())
      {
         process_tr ("Format Versions:", format_version);
      }
      else
      {
         process_tr ("", format_version);
      }
   }*/
}



/*
==============================================================================
Name : process_xhr_conf_emergency_path
==============================================================================
*/

void  PageState::process_xhr_conf_emergency_path ()
{
   /*const Configuration & conf = Configuration::get ();
   
   std::string value = conf._emergency_path + " ";
   
   if (conf._emergency_path_ok_flag)
   {
      value += "<span style=\"color:#0f0;\">OK</span>";
   }
   else
   {
      value += "<span style=\"color:#f00;\">FAILED</span>";
   }
   
   process_tr ("Emergency Path", value);*/
}



/*
==============================================================================
Name : process_xhr_status
==============================================================================
*/

void  PageState::process_xhr_status ()
{
   line ("<table border=0 cellpadding=0 cellspacing=0 width=100%>");
   
   size_t mem_size = HttpStats::use_instance ()._stats_data._osplex_mem;
   size_t cpu = HttpStats::use_instance ()._stats_data._osplex_cpu;
   double cpu_f = double (cpu) / 100.0;
   
   process_tr ("Started On:", HttpStats::use_instance ().get_start_date ());
   process_tr ("Running Since:", HttpStats::use_instance ().get_running_time ());
   process_tr ("Memory:", StatsData::to_mem_str (mem_size));
   process_tr ("CPU:", cpu_f);
   
   line ("</table>");
   
   flush_script ("status");
}



/*
==============================================================================
Name : process_xhr_rl_metrics
==============================================================================
*/

void  PageState::process_xhr_rl_metrics ()
{
   const HttpStats & stats = HttpStats::use_instance ();
   const StatsData & stats_data = stats._stats_data;
   
   const Metrics & metrics = stats_data._metrics_arr [Metrics::Type_RL];
   
   process_xhr_metrics (metrics);
   
   flush_script ("rlmet");
}



/*
==============================================================================
Name : process_xhr_day_metrics
==============================================================================
*/

void  PageState::process_xhr_day_metrics ()
{
   const HttpStats & stats = HttpStats::use_instance ();
   const StatsData & stats_data = stats._stats_data;
   
   const Metrics & metrics = stats_data._metrics_arr [Metrics::Type_DAY];
   
   process_xhr_metrics (metrics);
   
   flush_script ("daymet");
}



/*
==============================================================================
Name : process_xhr_week_metrics
==============================================================================
*/

void  PageState::process_xhr_week_metrics ()
{
   const HttpStats & stats = HttpStats::use_instance ();
   const StatsData & stats_data = stats._stats_data;
   
   const Metrics & metrics = stats_data._metrics_arr [Metrics::Type_WEEK];
   
   process_xhr_metrics (metrics);
   
   flush_script ("weekmet");
}



/*
==============================================================================
Name : process_xhr_metrics
==============================================================================
*/

void  PageState::process_xhr_metrics (const Metrics & metrics)
{
   line ("<center><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000px\" height=\"600px\" version=\"1.1\">");
   
   float y = 100.f;

   process_xhr_metrics (metrics, 100, y, metrics._nbr_tx_max, &Metric::_nbr_tx, "Nbr Tx");
   y += 120.f;

   process_xhr_metrics (metrics, 100, y, metrics._nbr_ping_failed_max, &Metric::_nbr_ping_failed, "Ping Err");
   y += 120.f;

   process_xhr_metrics (metrics, 100, y, metrics._nbr_user_max, &Metric::_nbr_user_max, "User Max");
   y += 120.f;

   process_xhr_metrics (metrics, 100, y, metrics._nbr_session_max, &Metric::_nbr_session_max, "Session Max");
   y += 120.f;
   
   process_xhr_metrics (metrics, 100, y, metrics._proc_all_max_us, &Metric::_proc_all_max_us, "Proc Max \xc2\xb5s");
   y += 120.f;

   line ("</svg></center>");
   
   const Metric & metric = metrics._metric_list.back ();
   
   line ("<center><table border=0 cellpadding=0 cellspacing=0 width=1000px>");
   
   {
      ProcDataArr proc_data_arr;
      proc_data_arr.push_back (ProcData ("Proc Timeout Client", metrics._proc_toc_max_us, metric._proc_toc_max_us));
      proc_data_arr.push_back (ProcData ("Proc Timeout Server", metrics._proc_tos_max_us, metric._proc_tos_max_us));
      proc_data_arr.push_back (ProcData ("Proc Release Client Flip", metrics._proc_recf_max_us, metric._proc_recf_max_us));
      proc_data_arr.push_back (ProcData ("Proc Release Client Http", metrics._proc_rech_max_us, metric._proc_rech_max_us));
      
      process_xhr_metrics_tr (proc_data_arr);
   }
   
   {
      ProcDataArr proc_data_arr;
      proc_data_arr.push_back (ProcData ("Proc Release Session", metrics._proc_res_max_us, metric._proc_res_max_us));
      proc_data_arr.push_back (ProcData ("Proc Notify Client Http", metrics._proc_nch_max_us, metric._proc_nch_max_us));
      proc_data_arr.push_back (ProcData ("Proc Accept Flip", metrics._proc_af_max_us, metric._proc_af_max_us));
      proc_data_arr.push_back (ProcData ("Proc Accept Http", metrics._proc_ah_max_us, metric._proc_ah_max_us));
      
      process_xhr_metrics_tr (proc_data_arr);
   }
   
   {
      ProcDataArr proc_data_arr;
      proc_data_arr.push_back (ProcData ("Proc Bind Client Flip", metrics._proc_bi_max_us, metric._proc_bi_max_us));
      proc_data_arr.push_back (ProcData ("Proc Unbind Client Flip", metrics._proc_ubi_max_us, metric._proc_ubi_max_us));
      proc_data_arr.push_back (ProcData ("Proc FdEvent", metrics._proc_fd_max_us, metric._proc_fd_max_us));
      
      process_xhr_metrics_tr (proc_data_arr);
   }
   
   line ("</table></center>");
}



/*
==============================================================================
Name : process_xhr_metrics_tr
==============================================================================
*/

void  PageState::process_xhr_metrics_tr (const ProcDataArr & proc_data_arr)
{
   using namespace ohm;
   
   line ("<tr class=\"tr_item\">");

   for (size_t i = 0 ; i < proc_data_arr.size () ; ++i)
   {
      const ProcData & proc_data = proc_data_arr [i];
      
      line ("<td class=\"tr_h\" width=250px>" + proc_data._name + " \xc2\xb5s</td>");
   }

   line ("</tr>");

   line ("<tr class=\"tr_item\" bgcolor=\"#6b6b6b\" onMouseOver=\"this.bgColor='#777777';\" onMouseOut=\"this.bgColor='#6b6b6b';\">");

   for (size_t i = 0 ; i < proc_data_arr.size () ; ++i)
   {
      const ProcData & proc_data = proc_data_arr [i];
      
      txt::out::Print printer;
      printer.print ("<td width=250px><span style=\"width:125px; float:left;\">%1%</span> <span style=\"color:#999;\">%2%</span></td>").arg (proc_data._max).arg (proc_data._cur);
      
      line (printer);
   }

   line ("</tr>");
}



/*
==============================================================================
Name : process_xhr_metrics
==============================================================================
*/

void  PageState::process_xhr_metrics (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name)
{
   process_xhr_metrics_axis (metrics, height, yo, max, var, name);
   process_xhr_metrics_time (metrics, height, yo, max, var, name);
   process_xhr_metrics_caption (metrics, height, yo, max, var, name);
   process_xhr_metrics_metrics (metrics, height, yo, max, var, name);
}



/*
==============================================================================
Name : process_xhr_metrics_axis
==============================================================================
*/

void  PageState::process_xhr_metrics_axis (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name)
{
   std::string svg;
   
   // bottom axis
   
   poly_begin (svg);
   poly_point (svg, 100, yo);
   poly_point (svg, 1000, yo);
   poly_end (svg, "#888");
   
   line (svg);
   
   // top axis
   
   if (max > 0)
   {
      poly_begin (svg);
      poly_point (svg, 100, yo - height);
      poly_point (svg, 1000, yo - height);
      poly_end (svg, "#888");
      
      line (svg);
   }
}



/*
==============================================================================
Name : process_xhr_metrics_caption
==============================================================================
*/

void  PageState::process_xhr_metrics_caption (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name)
{
   using namespace ohm;
   
   txt::out::Print printer;
   std::string svg;
   
   {
      printer.print ("%1%.5").arg (int (yo));
      const std::string & y_str = printer;
      
      line ("<text x=\"0\" y=\"" + y_str + "\" font-family=\"Arial\" font-size=\"13\" fill=\"white\">" + name + "</text>");
   }

   if (max > 0)
   {
      printer.print ("%1%.5").arg (int (yo - height + 9));
      std::string y_str = printer;

      printer.print ("%1%").arg (max);
      std::string txt = printer;
      
      line ("<text x=\"96\" y=\"" + y_str + "\" font-family=\"Arial\" font-size=\"13\" fill=\"#999\" text-anchor=\"end\">" + txt + "</text>");
   }
}



/*
==============================================================================
Name : process_xhr_metrics_metrics
==============================================================================
*/

void  PageState::process_xhr_metrics_metrics (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name)
{
   std::string svg;

   poly_begin (svg);
   
   const float xs = 900.f / float (metrics._nbr_metric_max);
   const float ys = (max > 0) ? float (height) / float (max) : 0.f;
   
   int i = 0;

   const MetricList & metric_list = metrics._metric_list;
   
   MetricList::const_reverse_iterator it = metric_list.rbegin ();
   const MetricList::const_reverse_iterator it_end = metric_list.rend ();

   for (; it != it_end ; ++it, ++i)
   {
      const Metric & metric = *it;
      
      float x = metrics._nbr_metric_max - i;
      x *= xs;
      
      float y = metric.*var;
      y = yo - ys * y;
      
      poly_point (svg, 100 + x, y);
   }
   
   poly_end (svg, "white");
   
   line (svg);
}



/*
==============================================================================
Name : process_xhr_metrics_time
==============================================================================
*/

void  PageState::process_xhr_metrics_time (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name)
{
   using namespace ohm;
   
   size_t period_s = metrics._mark_period_s;
   assert (period_s > 0);
   
   time_t cur_time = metrics._cur_time;
   time_t start_time = cur_time - metrics._nbr_metric_max * metrics._block_s;
   time_t display_time = ((start_time + period_s - 1) / period_s) * period_s;
   size_t nbr_display = (cur_time + period_s - 1 - display_time) / period_s;
   
   // find the first to display, where, and number to display
   
   const float xs = 900.f / float (metrics._nbr_metric_max);
   
   // start_time -> 0
   // cur_time -> metrics._nbr_metric_max
   
   float x
      = float (metrics._nbr_metric_max)
      * float (display_time - start_time)
      / float (cur_time - start_time);
   
   float xt
      = float (metrics._nbr_metric_max)
      * float (period_s)
      / float (cur_time - start_time);
   
   std::string svg;
   
   for (size_t i = 0 ; i < nbr_display ; ++i, x += xt, display_time += period_s)
   {
      float x2 = x * xs;
      
      poly_begin (svg);
      poly_point (svg, 100 + x2, yo);
      poly_point (svg, 100 + x2, yo - height);
      poly_end (svg, "#888");
      
      txt::out::Print printer;

      tm tm_time;
      gmtime_r (&display_time, &tm_time);

      char date_0 [100];
      strftime (date_0, sizeof (date_0), metrics._date_format_0, &tm_time);
      
      printer
         .print ("<text x=\"%1%.5\" y=\"%2%.5\" font-family=\"Arial\" font-size=\"13\" fill=\"#999\">%3%</text>")
         .arg (int (104 + x2))
         .arg (int (yo - 4))
         .arg (date_0);
      
      svg += std::string (printer);
   }
   
   line (svg);
}



/*
==============================================================================
Name : poly_begin
==============================================================================
*/

void  PageState::poly_begin (std::string & svg)
{
   svg += "<polyline points=\"";
}



/*
==============================================================================
Name : poly_point
==============================================================================
*/

void  PageState::poly_point (std::string & svg, float x, float y)
{
   using namespace ohm;
   
   txt::out::Print printer;
   
   printer.print ("%1%.5,%2%.5 ").arg (int (x)).arg (int (y));
   svg += printer;
}



/*
==============================================================================
Name : poly_end
==============================================================================
*/

void  PageState::poly_end (std::string & svg, const std::string & color)
{
   svg += "\" style=\"fill:none;stroke:" + color + ";stroke-width:1px\" />";
}



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

