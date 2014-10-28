/*****************************************************************************

        StatsData.cpp
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

#include "plex/shared/StatsData.h"

#include "ohm/math/fnc.h"
#include "ohm/sys/TimerReliable.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/BinaryStreamInput.h"
#include "ohm/util/BinaryStreamOutputBase.h"

#if defined (__linux__)
   #include <sys/sysinfo.h>
   #include <linux/limits.h>
   #include <sys/times.h>
   #include <unistd.h>

#elif defined (__APPLE__)
   #include <mach/task.h>
   #include <mach/mach_init.h>
#endif



#include <cassert>



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

StatsData::StatsData ()
:  _start_time (time (0))

,  _session_map ()
,  _metrics_arr ()

,  _nbr_user (0)
,  _nbr_session (0)

,  _start_time_s (ohm::sys::TimerReliable::use_instance ().get ())
,  _last_sample_time_s (ohm::sys::TimerReliable::use_instance ().get ())
{
   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      _metrics_arr [i].init (Metrics::Type (i));
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

StatsData::~StatsData ()
{
}



/*
==============================================================================
Name : post_process
==============================================================================
*/

void  StatsData::post_process ()
{
   using namespace ohm;
   
   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      Metrics & metrics = _metrics_arr [i];

      metrics.post_process (_start_time_s, _nbr_user, _nbr_session);
   }
   
   _osplex_mem = get_proc_mem_used ();

   double now = sys::TimerReliable::use_instance ().get ();
   
   if (now > _last_sample_time_s + 0.25)
   {
      _last_sample_time_s = now;
      
      _osplex_cpu = get_proc_cpu_used ();
   }
}



/*
==============================================================================
Name : write
==============================================================================
*/

void  StatsData::write (ohm::util::BinaryStreamOutputBase & bso) const
{
   using namespace ohm;
   
   bso.write (archi::UInt32 (_start_time));
   bso.write (_session_map.begin (), _session_map.end ());

   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      bso.write (_metrics_arr [i]);
   }
   
   bso.write (archi::UInt32 (_osplex_mem));
   bso.write (archi::UInt32 (_osplex_cpu));
}



/*
==============================================================================
Name : read
==============================================================================
*/

int   StatsData::read (ohm::util::BinaryStreamInput & bsi)
{
   bsi.read_uint32 (_start_time, "bsi:stats:start_time");

   _session_map.clear ();
   bsi.read (_session_map, "bsi:stats:session_map");

   for (int i = 0 ; i < Metrics::Type_NBR_ELT ; ++i)
   {
      bsi.read (_metrics_arr [i], "bsi:stats:metrics_arr");
   }
   
   bsi.read_uint32 (_osplex_mem, "bsi:stats:osplex_mem");
   bsi.read_uint32 (_osplex_cpu, "bsi:stats:osplex_cpu");
   
   return 0;
}



/*
==============================================================================
Name : get_proc_mem_used
==============================================================================
*/

size_t   StatsData::get_proc_mem_used ()
{
   size_t size = 0;
   
#if defined (__linux__)
   FILE * file_ptr = fopen ("/proc/self/statm", "r");
   if (file_ptr != 0)
   {
      unsigned long virt = 0;
      fscanf (file_ptr, "%ul", &virt);

      unsigned long res = 0;
      fscanf (file_ptr, "%ul", &res);

      fclose (file_ptr);
      size = (size_t) res * getpagesize ();
   }
   
#elif defined (__APPLE__)
   struct task_basic_info t_info;
   mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
   task_info (current_task (), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count);
   size = t_info.resident_size;

#endif
   
   return size;
}



/*
==============================================================================
Name : get_proc_cpu_used
Reference :
   man 5 proc
==============================================================================
*/

size_t   StatsData::get_proc_cpu_used ()
{
   size_t cpu = 0;
   
#if defined (__linux__)

   static size_t proc_time_old = 0;
   static size_t total_time_old = 0;

   FILE * file_ptr = fopen ("/proc/self/stat", "r");
   
   size_t proc_time = 0;
   
   if (file_ptr != 0)
   {
      int pid = 0;
      char tcomm [2048];
      char state;
      int ppid = 0;
      int pgrp = 0;
      int session = 0;
      int tty_nr = 0;
      int tpgid = 0;
      unsigned int flags = 0;
      unsigned long minflt = 0;
      unsigned long cminflt = 0;
      unsigned long majflt = 0;
      unsigned long cmajflt = 0;
      unsigned long utime = 0;
      unsigned long stime = 0;

      fscanf (
         file_ptr,
         "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu",
         &pid, tcomm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid,
         &flags, &minflt, &cminflt, &majflt, &cmajflt,
         &utime, &stime
      );
      
      fclose (file_ptr);
      
      proc_time = utime + stime;
   }
   
   file_ptr = fopen ("/proc/stat", "r");
   
   size_t total_time = 0;
   
   if (file_ptr != 0)
   {
      char field [2048];
      unsigned long t [9];
      
      fscanf (
         file_ptr,
         "%s %lu %lu %lu %lu %lu %lu %lu %lu %lu",
         field, &t[0], &t[1], &t[2], &t[3], &t[4], &t[5], &t[6], &t[7], &t[8]
      );
      
      fclose (file_ptr);
      
      for (int i = 0 ; i < 9 ; ++i)
      {
         total_time += t [i];
      }
   }
   
   size_t proc_diff = proc_time - proc_time_old;
   size_t total_diff = total_time - total_time_old;
   
   proc_time_old = proc_time;
   total_time_old = total_time;
   
   if (total_diff > 0)
   {
      cpu = size_t (10000.0 * double (proc_diff) / double (total_diff));
   }
   
#elif defined (__APPLE__)
   // nothing
   
#endif
   
   return cpu;
}



/*
==============================================================================
Name : to_mem_str
==============================================================================
*/

std::string StatsData::to_mem_str (size_t size)
{
   using namespace ohm;
   
   txt::out::Print printer;
   
   if (size > 1000000000)
   {
      printer.print ("%1%G").arg (size / 1000000000);
   }
   if (size > 10000000)
   {
      printer.print ("%1%M").arg (size / 1000000);
   }
   else if (size > 10000)
   {
      printer.print ("%1%K").arg (size / 1000);
   }
   else
   {
      printer.print ("%1%B").arg (size);
   }
   
   return std::string (printer);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

