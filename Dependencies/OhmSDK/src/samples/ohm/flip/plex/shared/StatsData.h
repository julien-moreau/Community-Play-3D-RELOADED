/*****************************************************************************

        StatsData.h
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



#if ! defined (plex_StatsData_HEADER_INCLUDED)
#define  plex_StatsData_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/shared/Metrics.h"
#include "plex/shared/SessionStats.h"



namespace plex
{



class StatsData
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  StatsData ();
   virtual        ~StatsData ();
   
   void           post_process ();

   void           write (ohm::util::BinaryStreamOutputBase & bso) const;
   int            read (ohm::util::BinaryStreamInput & bsi);
   
   static size_t  get_proc_mem_used ();
   static size_t  get_proc_cpu_used ();
   static std::string
                  to_mem_str (size_t size);
   
   time_t         _start_time;

   SessionStatsMap
                  _session_map;
   Metrics        _metrics_arr [Metrics::Type_NBR_ELT];
   
   size_t         _nbr_user;
   size_t         _nbr_session;
   
   size_t         _osplex_mem;
   size_t         _osplex_cpu;   // per 10000



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   double         _start_time_s;
   double         _last_sample_time_s;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  StatsData (const StatsData &other);
   StatsData &    operator = (const StatsData &other);
   bool           operator == (const StatsData &other);
   bool           operator != (const StatsData &other);

}; // class StatsData



}  // namespace plex



//#include  "StatsData.hpp"



#endif   // plex_StatsData_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

