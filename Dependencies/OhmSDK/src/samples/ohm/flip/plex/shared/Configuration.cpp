/*****************************************************************************

        Configuration.cpp
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

#include "plex/shared/Configuration.h"

#include "ohm/txt/ascii/ascii_fnc.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/txt/out/Print.h"
#include "ohm/util/trace.h"

#include <cassert>

#define conf_TRACE(msg_0)  ohm_util_TRACE_INFO(msg_0)
#define conf_TRACE1(fmt_0, arg1) ohm_util_TRACE_INFO1(fmt_0, arg1)
#define conf_TRACE5(fmt_0, arg1, arg2, arg3, arg4, arg5) ohm_util_TRACE_INFO5(fmt_0, arg1, arg2, arg3, arg4, arg5)



namespace plex
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use
==============================================================================
*/

Configuration &   Configuration::use ()
{
   Configuration & instance = use_instance ();
   
   instance._timestamp = time (0);
   
   return instance;
}



/*
==============================================================================
Name : get
==============================================================================
*/

const Configuration &   Configuration::get ()
{
   Configuration & instance = use_instance ();
   
   return instance;
}



/*
==============================================================================
Name : set
Description :
   Returns true iff conf was ok.
==============================================================================
*/

bool  Configuration::set (int argc, const char * const * argv)
{
   using namespace ohm;
   
   if (argc < 1)
   {
      usage ();
      
      return false;
   }
   
   _daemon_flag = false;
   
   // verb
   
   bool ok_flag = true;
   
   ok_flag = argc >= 1;
   
   // options
   
   if (ok_flag)
   {
      for (int i = 1 ; i < argc ; ++i)
      {
         std::string arg (argv [i]);
         
         bool value_flag
            = (arg == _opt_port_0)
            || (arg == _opt_session_0)
            || (arg == _opt_flush_0)
            || (arg == _opt_ping_period_0)
            || (arg == _opt_recv_0)
            || (arg == _opt_srvp_0)
            || (arg == _opt_bin_path_0);
         
         std::string value;
         
         if (value_flag)
         {
            ++i;
            ok_flag = i < argc;
            
            if (ok_flag)
            {
               value = argv [i];
            }
         }
         
         if (ok_flag)
         {
            if (arg == _opt_port_0)
            {
               archi::Int64 val = 0;

               ok_flag &= txt::utf8::ConvNum::conv_str_to_int64 (val, value.c_str ());
               ok_flag &= val >= 0;
               ok_flag &= val <= 65535;
               
               if (ok_flag)
               {
                  _port = val;
               }
            }
            else if (arg == _opt_session_0)
            {
               _session = value;
            }
            else if (arg == _opt_flush_0)
            {
               double val = 60.0;
               
               ok_flag &= txt::ascii::conv_str_to_double (val, value.c_str ());
               ok_flag &= val >= 0.0;
               
               if (ok_flag)
               {
                  _snapshot_flush_period = val;
               }
            }
            else if (arg == _opt_ping_period_0)
            {
               double val = 1.0;
               
               ok_flag &= txt::ascii::conv_str_to_double (val, value.c_str ());
               ok_flag &= val >= 0.0;
               
               if (ok_flag)
               {
                  _ping_period = val;
               }
            }
            else if (arg == _opt_recv_0)
            {
               double val = 2.0;
               
               ok_flag &= txt::ascii::conv_str_to_double (val, value.c_str ());
               ok_flag &= val >= 0.0;
               
               if (ok_flag)
               {
                  _recv_timeout = val;
               }
            }
            else if (arg == _opt_srvp_0)
            {
               archi::Int64 val = 0;

               ok_flag &= txt::utf8::ConvNum::conv_str_to_int64 (val, value.c_str ());
               ok_flag &= val >= 0;
               ok_flag &= val <= 65535;
               
               if (ok_flag)
               {
                  _service_port = val;
               }
            }
            else if (arg == _opt_daemon_0)
            {
               _daemon_flag = true;
            }
            else if (arg == _opt_no_auth_0)
            {
               _no_auth_flag = true;
            }
            else if (arg == _opt_angel_0)
            {
               _angel_flag = true;
            }
            else if (arg == _opt_http_0)
            {
               _http_flag = true;
            }
            else if (arg == _opt_sess_0)
            {
               _sess_flag = true;
            }
            else if (arg == _opt_comp_0)
            {
               _comp_flag = true;
            }
            else if (arg == _opt_timings_0)
            {
               _timings_flag = true;
            }
            else if (arg == _opt_bin_path_0)
            {
               _bin_path = value;
            }
            else
            {
               ohm_util_TRACE_ERROR1 ("unknown option `%1%'", arg.c_str ());
               
               ok_flag = false;
            }
         }
         
         if (!ok_flag)
         {
            break;
         }
      }
   }
   
   if (!ok_flag)
   {
      usage ();
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  Configuration::make (std::list <std::string> & arg_list) const
{
   make (arg_list, _opt_port_0, _port);
   make (arg_list, _opt_session_0, _session);
   make (arg_list, _opt_flush_0, _snapshot_flush_period);
   make (arg_list, _opt_ping_period_0, _ping_period);
   make (arg_list, _opt_recv_0, _recv_timeout);
   make (arg_list, _opt_srvp_0, _service_port);
   make (arg_list, _opt_no_auth_0, _no_auth_flag);
   make (arg_list, _opt_timings_0, _timings_flag);
   make (arg_list, _opt_bin_path_0, _bin_path);
   make (arg_list, _opt_sess_0, _comp_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use_instance
==============================================================================
*/

Configuration &   Configuration::use_instance ()
{
   static Configuration instance;
   
   return instance;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Configuration::Configuration ()
:  _port (9222)
,  _bin_path ()
,  _snapshot_flush_period (60.0)
,  _ping_period (2.0)
,  _recv_timeout (10.0)
,  _service_port (9223)
,  _daemon_flag (false)
,  _timings_flag (false)
,  _no_auth_flag (false)
,  _angel_flag (false)
,  _http_flag (false)
,  _sess_flag (false)
,  _comp_flag (false)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : usage
==============================================================================
*/

void  Configuration::usage ()
{
   conf_TRACE ("usage: osplex <options>\n");
   
   conf_TRACE ("options:");
   conf_TRACE1 ("%1% port", _opt_port_0);
   conf_TRACE1 ("%1% path", _opt_bin_path_0);
   conf_TRACE1 ("%1% seconds", _opt_flush_0);
   conf_TRACE1 ("%1% seconds", _opt_ping_period_0);
   conf_TRACE1 ("%1% seconds", _opt_recv_0);
   conf_TRACE1 ("%1% port", _opt_srvp_0);
   conf_TRACE1 ("%1%", _opt_daemon_0);
   conf_TRACE1 ("%1%", _opt_no_auth_0);
   conf_TRACE1 ("%1%", _opt_angel_0);
   conf_TRACE1 ("%1%\n", _opt_timings_0);
   
   conf_TRACE ("example:");
   conf_TRACE1 ("osplex %1% 9222", _opt_port_0);
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  Configuration::make (std::list <std::string> & arg_list, const char * opt_0, bool val) const
{
   if (val)
   {
      arg_list.push_back (opt_0);
   }
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  Configuration::make (std::list <std::string> & arg_list, const char * opt_0, int val) const
{
   ohm::txt::out::Print printer;
   printer.print ("%1%").arg (val);
   
   arg_list.push_back (opt_0);
   arg_list.push_back (printer);
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  Configuration::make (std::list <std::string> & arg_list, const char * opt_0, double val) const
{
   ohm::txt::out::Print printer;
   printer.print ("%1%").arg (val);
   
   arg_list.push_back (opt_0);
   arg_list.push_back (printer);
}



/*
==============================================================================
Name : make
==============================================================================
*/

void  Configuration::make (std::list <std::string> & arg_list, const char * opt_0, const std::string & val) const
{
   if (!val.empty ())
   {
      arg_list.push_back (opt_0);
      arg_list.push_back (val);
   }
}



// options
const char *   Configuration::_opt_port_0 = "--port";
const char *   Configuration::_opt_session_0 = "--session";
const char *   Configuration::_opt_flush_0 = "--snapshot_flush_period";
const char *   Configuration::_opt_ping_period_0 = "--ping_period";
const char *   Configuration::_opt_recv_0 = "--recv_timeout";
const char *   Configuration::_opt_srvp_0 = "--service_port";
const char *   Configuration::_opt_daemon_0 = "--daemon";
const char *   Configuration::_opt_no_auth_0 = "--no_auth";
const char *   Configuration::_opt_timings_0 = "--timings";
const char *   Configuration::_opt_bin_path_0 = "--bin_sess_path";
const char *   Configuration::_opt_angel_0 = "--angel";
const char *   Configuration::_opt_http_0 = "--http";
const char *   Configuration::_opt_sess_0 = "--sess";
const char *   Configuration::_opt_comp_0 = "--compress";



}  // namespace plex



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

