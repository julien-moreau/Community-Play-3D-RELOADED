/*****************************************************************************

        Metrics.h
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



#if ! defined (plex_Metrics_HEADER_INCLUDED)
#define  plex_Metrics_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/shared/Metric.h"

#include "ohm/archi/types.h"



namespace ohm
{
namespace util
{
class BinaryStreamOutputBase;
class BinaryStreamInput;
}
}


namespace plex
{



class Metrics
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Type
   {
                  Type_INVALID = -1,
                  
                  Type_RL = 0,
                  Type_DAY,
                  Type_WEEK,
                  
                  Type_NBR_ELT
   };
   
                  Metrics ();
   virtual        ~Metrics ();
   
   void           init (Type type);
   void           post_process (double start_time_s, size_t nbr_user, size_t nbr_session);
   
   void           write (ohm::util::BinaryStreamOutputBase & bso) const;
   int            read (ohm::util::BinaryStreamInput & bsi);
   
   Type           _type;
   
   size_t         _nbr_tx_max;
   size_t         _nbr_ping_failed_max;
   size_t         _nbr_user_max;
   size_t         _nbr_session_max;

   size_t         _proc_all_max_us;
   size_t         _proc_fd_max_us;
   size_t         _proc_toc_max_us;
   size_t         _proc_tos_max_us;
   size_t         _proc_recf_max_us;
   size_t         _proc_rech_max_us;
   size_t         _proc_res_max_us;
   size_t         _proc_nch_max_us;
   size_t         _proc_af_max_us;
   size_t         _proc_ah_max_us;
   size_t         _proc_bi_max_us;
   size_t         _proc_ubi_max_us;
   
   MetricList     _metric_list;
   time_t         _cur_time;

   size_t         _block_s;
   size_t         _nbr_metric_max;
   size_t         _mark_period_s;
   const char *   _date_format_0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           init_rl ();
   void           init_day ();
   void           init_week ();
   
   void           recalculate ();

   ohm::archi::Int64
                  _block_nbr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Metrics (const Metrics &other);
   Metrics &      operator = (const Metrics &other);
   bool           operator == (const Metrics &other);
   bool           operator != (const Metrics &other);

}; // class Metrics

typedef std::list <Metrics>   MetricsList;



}  // namespace plex



//#include  "Metrics.hpp"



#endif   // plex_Metrics_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

