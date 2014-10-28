/*****************************************************************************

        PageState.h
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



#if ! defined (plex_PageState_HEADER_INCLUDED)
#define  plex_PageState_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "plex/http/PageBase.h"
#include "plex/http/HttpStats.h"
#include "plex/shared/Metric.h"



namespace plex
{



class HttpCentral;

class PageState
:  public PageBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PageState (HttpCentral & central);
   virtual        ~PageState ();
   
   static PageBase *
                  create (HttpCentral & central);
   
   // PageBase
   virtual void   process_body ();
   virtual void   process_xhr ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   

   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   class ProcData
   {
   public:
                  ProcData (const std::string & name, size_t max, size_t cur) : _name (name), _max (max), _cur (cur) {}

      std::string _name;
      size_t      _max;
      size_t      _cur;
   };
   
   typedef std::vector <ProcData>   ProcDataArr;
   
   void           process_xhr_conf ();
   void           process_xhr_conf_format_version_set ();
   void           process_xhr_conf_emergency_path ();

   void           process_xhr_status ();

   void           process_xhr_rl_metrics ();
   void           process_xhr_day_metrics ();
   void           process_xhr_week_metrics ();
   void           process_xhr_metrics (const Metrics & metrics);
   void           process_xhr_metrics (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name);
   void           process_xhr_metrics_tr (const ProcDataArr & proc_data_arr);
   void           process_xhr_metrics_axis (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name);
   void           process_xhr_metrics_caption (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name);
   void           process_xhr_metrics_metrics (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name);
   void           process_xhr_metrics_time (const Metrics & metrics, int height, float yo, size_t max, size_t (Metric::*var), const std::string & name);
   
   void           poly_begin (std::string & svg);
   void           poly_point (std::string & svg, float x, float y);
   void           poly_end (std::string & svg, const std::string & color);
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PageState ();
                  PageState (const PageState &other);
   PageState &    operator = (const PageState &other);
   bool           operator == (const PageState &other);
   bool           operator != (const PageState &other);

}; // class PageState



}  // namespace plex



//#include  "PageState.hpp"



#endif   // plex_PageState_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

