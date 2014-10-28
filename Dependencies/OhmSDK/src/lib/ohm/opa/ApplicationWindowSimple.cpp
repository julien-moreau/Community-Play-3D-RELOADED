/*****************************************************************************

        ApplicationWindowSimple.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71404

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

#include "ohm/opa/ApplicationWindowSimple.h"

#include "ohm/archi/def.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opa/private/WidgetLayoutManager.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/MenuRoot.h"
#include "ohm/opal/MenuBar.h"
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/MenuBarImpl.h"
#endif
#include "ohm/opal/Screens.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/task/Clock.h"
#include "ohm/task/RunLoopGui.h"
#include "ohm/task/RunLoopManager.h"
#include "ohm/task/private/EventProvider.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ApplicationWindowSimple::ApplicationWindowSimple (int argc, const char * const * argv)
{
   init (argc, argv, true, opa::Size::_zero);
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ApplicationWindowSimple::ApplicationWindowSimple (int argc, const char * const * argv, const opa::Size & bounds)
{
   init (argc, argv, false, bounds);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ApplicationWindowSimple::~ApplicationWindowSimple ()
{
   try
   {
      _window_aptr->hide ();
      
      _bundle_view_aptr->remove (*this);  // opal::PostActionListener
      
      _bundle_view_aptr->stop ();
      _bundle_view_aptr.reset ();
      _window_aptr.reset ();
      
      //
      
      opal::VideoThread::use_instance ().stop ();
      
      //
      
      task::RunLoopGui & run_loop_gui = task::RunLoopGui::use_instance ();
      
      WidgetLayoutManager::use_instance ().restore ();
      task::RunLoopManager::use_instance ().pop_current (run_loop_gui);

      task::Clock::use_instance ().release ();
      
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
      opal::MenuBarImpl::use_instance ().release ();
#endif
      run_loop_gui.release ();
      
      task::EventProvider::use_instance ().release ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  ApplicationWindowSimple::run ()
{
   opal::Application::use_instance ().run ();
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  ApplicationWindowSimple::set (const std::string & key, const std::string & value)
{
   _key_value_map [key] = value;
}



/*
==============================================================================
Name: reset
==============================================================================
*/

void  ApplicationWindowSimple::reset (const std::string & key)
{
   _key_value_map.erase (key);
}



/*
==============================================================================
Name : has
==============================================================================
*/

bool  ApplicationWindowSimple::has (const std::string & key) const
{
   return _key_value_map.find (key) != _key_value_map.end ();
}



/*
==============================================================================
Name : get
==============================================================================
*/

const std::string &  ApplicationWindowSimple::get (const std::string & key) const
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
Name : use_view
==============================================================================
*/

BundleView &   ApplicationWindowSimple::use_view ()
{
   assert (_bundle_view_aptr.get () != 0);
   
   return *_bundle_view_aptr;
}



/*
==============================================================================
Name : notify_pre_action
==============================================================================
*/

void  ApplicationWindowSimple::notify_pre_action ()
{
   _delegate_pre.signal ();
}



/*
==============================================================================
Name : notify_post_action
==============================================================================
*/

void  ApplicationWindowSimple::notify_post_action ()
{
   _delegate_post.signal ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init
==============================================================================
*/

void  ApplicationWindowSimple::init (int argc, const char * const * argv, bool default_flag, const opa::Size & bounds)
{
   parse_arguments (argc, argv);
   
   task::RunLoopGui & run_loop_gui = task::RunLoopGui::use_instance ();
      
   task::RunLoopManager::use_instance ().push_current (run_loop_gui);
   WidgetLayoutManager::use_instance ().init ();
   
   //
   
   opal::Application::use_instance ();
   
   //
   
   opal::VideoThread::use_instance ().run ();
   
   //
   
   _window_aptr = std::auto_ptr <opal::Window> (
      ohm::opal::Window::create_document_window ("")
   );

   _window_aptr->bind_menu_bar ();
   _window_aptr->bind <
      ApplicationWindowSimple,
      &ApplicationWindowSimple::handle_event
   > (*this);
   
   if (default_flag)
   {
      _window_aptr->resize_default_main_screen ();
   }
   else
   {
      const opa::Rect & rect = opal::Screens::use_instance ().get_main_screen_without_decorations ();
      
      opa::Point origin (
         rect._origin._x + (rect._size._width - bounds._width) * .5f,
         rect._origin._y + (rect._size._height - bounds._height) * .5f
      );
      
      _window_aptr->set_origin (origin);
      _window_aptr->set_bounds (bounds);
   }
   
   _window_aptr->set_limit_bounds (Size (640, 480), Size::_infinity);

   _bundle_view_aptr = std::auto_ptr <BundleView> (
      new BundleView (*_window_aptr)
   );
   
   _bundle_view_aptr->run ();
   
   _bundle_view_aptr->add (*this);  // opal::PostActionListener

   opak::TransactionMaster::use_instance ().send_wait_vbr ();
   
   _window_aptr->show ();
   
   //
   
   opal::MenuRoot & menu_root = opal::MenuBar::use_instance ().use_root ();
   
   menu_root.bind_pre <
      ApplicationWindowSimple,
      &ApplicationWindowSimple::notify_pre_action
   > (*this);
   
   menu_root.bind_post <
      ApplicationWindowSimple,
      &ApplicationWindowSimple::notify_post_action
   > (*this);
}



/*
==============================================================================
Name : parse_arguments
==============================================================================
*/

void  ApplicationWindowSimple::parse_arguments (int argc, const char * const * argv)
{
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   // on MacOS the second argument when launched from Finder is the psn
   // in that case we have two arguments, which we ignore
   
   if (argc == 2)
   {
      return;
   }

   assert ((argc - 1) % 2 == 0);
   
   for (int i = 1 ; i < argc ; i += 2)
   {
      std::string key (argv [i]);
      std::string value (argv [i + 1]);
      
      set (key, value);
   }

#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)
   assert (argc % 2 == 0);
   
   for (int i = 0 ; i < argc ; i += 2)
   {
      std::string key (argv [i]);
      std::string value (argv [i + 1]);
      
      set (key, value);
   }
#endif
   
}



/*
==============================================================================
Name : handle_event
==============================================================================
*/

void  ApplicationWindowSimple::handle_event (opal::Window::Event & event)
{
   if (event._type == opal::Window::Event::Type_CLOSING) 
   {
      event._close_flag = false;

      task::RunLoopGui::use_instance ().quit ();
   }
   else if (event._type == opal::Window::Event::Type_CLOSED)
   {
      // nothing
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
