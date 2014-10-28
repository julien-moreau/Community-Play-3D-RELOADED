/*****************************************************************************

        ApplicationWindow.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70462

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

#include "client/ApplicationWindow.h"

#include "ohm/archi/def.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opal/Screens.h"
#include "ohm/opal/Application.h"
#include "ohm/task/RunLoopGui.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ApplicationWindow::ApplicationWindow ()
:  _window_aptr ()
,  _view_aptr ()
,  _gui_aptr ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ApplicationWindow::~ApplicationWindow ()
{
}


   
/*
==============================================================================
Name : use_window
==============================================================================
*/

ohm::opal::Window &  ApplicationWindow::use_window ()
{
   assert (_window_aptr.get () != 0);
   
   return *_window_aptr;
}



/*
==============================================================================
Name : use_bundle_view
==============================================================================
*/

ohm::opa::BundleView &  ApplicationWindow::use_view ()
{
   assert (_view_aptr.get () != 0);
   
   return *_view_aptr;
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  ApplicationWindow::run ()
{
   using namespace ohm;
   
   // windows
   
   assert (_window_aptr.get () == 0);
   
   _window_aptr = std::auto_ptr <opal::Window> (
      ohm::opal::Window::create_document_window ("")
   );

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   _window_aptr->set_title ("Skeleton");
#endif

   std::vector <ohm::opa::Rect> rect_arr = ohm::opal::Screens::use_instance ().get_layout ();      
   
   _window_aptr->bind_menu_bar ();
   _window_aptr->bind <
      ApplicationWindow,
      &ApplicationWindow::handle_event
   > (*this);

   _window_aptr->resize_default_main_screen ();
   
   _window_aptr->set_limit_bounds (opa::Size (640, 480), opa::Size::_infinity);

   // bundle view
   
   _view_aptr = std::auto_ptr <opa::BundleView> (
      new ohm::opa::BundleView (*_window_aptr)
   );
   
   _view_aptr->run ();
   
   // ohmstudio gui
   
   _gui_aptr = std::auto_ptr <SkeletonGui> (new SkeletonGui (*_view_aptr));
   
   opak::TransactionMaster::use_instance ().send_wait_vbr ();
   
   _window_aptr->show ();  
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  ApplicationWindow::stop ()
{
   // ohmstudio gui
   
   _window_aptr->hide ();
   
   _gui_aptr.reset ();

   // bundle view
   
   _view_aptr->stop ();
   _view_aptr.reset ();
   
   // window
   
   _window_aptr.reset ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : handle_event
==============================================================================
*/

void  ApplicationWindow::handle_event (ohm::opal::Window::Event & event)
{
   using namespace ohm;
   
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

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
