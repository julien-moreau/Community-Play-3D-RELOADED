/*****************************************************************************

        Configuration.h
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



#if ! defined (plex_Configuration_HEADER_INCLUDED)
#define  plex_Configuration_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <vector>
#include <string>



namespace plex
{



class Configuration
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Command
   {
                  Command_START
   };
   
   static Configuration &
                  use ();
   static const Configuration &
                  get ();
   
   virtual        ~Configuration () {}
   
   bool           set (int argc, const char * const * argv);
   void           make (std::list <std::string> & arg_list) const;
   
   int            _port;
   std::string    _session;
   std::string    _bin_path;
   double         _snapshot_flush_period;
   double         _ping_period;
   double         _recv_timeout;
   int            _service_port;
   bool           _daemon_flag;
   bool           _timings_flag;
   bool           _no_auth_flag;
   bool           _angel_flag;
   bool           _http_flag;
   bool           _sess_flag;
   bool           _comp_flag;
   
   time_t         _timestamp;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  Configuration ();


   
   
/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static Configuration &
                  use_instance ();
   void           usage ();
   
   void           make (std::list <std::string> & arg_list, const char * opt_0, bool val) const;
   void           make (std::list <std::string> & arg_list, const char * opt_0, int val) const;
   void           make (std::list <std::string> & arg_list, const char * opt_0, double val) const;
   void           make (std::list <std::string> & arg_list, const char * opt_0, const std::string & val) const;

   // options
   static const char *
                  _opt_port_0;
   static const char *
                  _opt_session_0;
   static const char *
                  _opt_flush_0;
   static const char *
                  _opt_ping_period_0;
   static const char *
                  _opt_recv_0;
   static const char *
                  _opt_srvp_0;
   static const char *
                  _opt_daemon_0;
   static const char *
                  _opt_no_auth_0;
   static const char *
                  _opt_timings_0;
   static const char *
                  _opt_bin_path_0;
   static const char *
                  _opt_angel_0;
   static const char *
                  _opt_http_0;
   static const char *
                  _opt_sess_0;
   static const char *
                  _opt_comp_0;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Configuration (const Configuration &other);
   Configuration &
                  operator = (const Configuration &other);
   bool           operator == (const Configuration &other);
   bool           operator != (const Configuration &other);

}; // class Configuration



}  // namespace plex



//#include  "Configuration.hpp"



#endif   // plex_Configuration_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

