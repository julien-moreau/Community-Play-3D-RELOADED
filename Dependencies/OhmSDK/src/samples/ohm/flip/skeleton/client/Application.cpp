/*****************************************************************************

        Application.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71433

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/Application.h"

#include "client/ApplicationWindow.h"
#include "client/Constants.h"

#include "model/fnc.h"

#include "ohm/opal/Application.h"
#include "ohm/opal/VideoThread.h"

#include <cassert>
#include <ctime>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Application::~Application ()
{
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Application::set (const std::string & key, const std::string & value)
{
   _key_value_map [key] = value;
}



/*
==============================================================================
Name: reset
==============================================================================
*/

void Application::reset (const std::string & key)
{
   KeyValueMap::iterator it = _key_value_map.find (key);
   
   if (it != _key_value_map.end ())
   {
      _key_value_map.erase (it);
   }
}



/*
==============================================================================
Name : has
==============================================================================
*/

bool  Application::has (const std::string & key) const
{
   return _key_value_map.find (key) != _key_value_map.end ();
}



/*
==============================================================================
Name : get
==============================================================================
*/

const std::string &  Application::get (const std::string & key) const
{
   KeyValueMap::const_iterator it = _key_value_map.find (key);
   
   if (it != _key_value_map.end ())
   {
      return it->second;
   }
   else
   {
      static const std::string empty_sptr = "";
      
      return empty_sptr;
   }
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  Application::run ()
{
   using namespace ohm;
   
   opal::VideoThread::use_instance ().run ();
   
   // application window
   
   assert (_application_window_aptr.get () == 0);
   
   _application_window_aptr = std::auto_ptr <ApplicationWindow> (new ApplicationWindow);
   
   _application_window_aptr->run ();

   // run application
   
   opal::Application::use_instance ().run ();


   // application window
   
   _application_window_aptr->stop ();
   
   _application_window_aptr.reset ();

   opal::VideoThread::use_instance ().stop ();
}



/*
==============================================================================
Name : use_window
==============================================================================
*/

ApplicationWindow &  Application::use_application_window ()
{
   assert (_application_window_aptr.get () != 0);
   
   return *_application_window_aptr;
}



const char *   Application::_key_host_0 = "host";
const char *   Application::_key_port_0 = "port";
const char *   Application::_key_session_0 = "session";
const char *   Application::_key_username_0 = "username";
const char *   Application::_key_secret_0 = "secret";



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Application::Application ()
:  _application_window_aptr ()
{
   using namespace ohm;
   
   opal::Application::use_instance ();
   
   model::declare ();
   
   set (_key_host_0, "localhost");
   set (_key_port_0, "9222");
   set (_key_session_0, "mysession");
   set (_key_username_0, "me");
   set (_key_secret_0, "secret");
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
