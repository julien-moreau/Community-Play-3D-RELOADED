/*****************************************************************************

        Window.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66185

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

#include "ohm/opal/Window.h"
#include "ohm/opa/Rect.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : create_document_window
Description :
   Create a document window.
   A document window has a title bar, a close box, a zoom box, a collapse box,
   and is resizable.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The window is created in hidden state.
==============================================================================
*/

Window * Window::create_document_window (const std::string & name)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_document ();
   window_ptr->_window_impl.set_properties (true, true, true, true);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_owned_window
==============================================================================
*/

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
Window * Window::create_owned_window (const std::string & name, bool close_box_flag, bool resizable_flag, archi::Int64 owner_window_data)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_owned (owner_window_data);
   window_ptr->_window_impl.set_properties (close_box_flag, false, false, resizable_flag);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}
#endif



/*
==============================================================================
Name : create_floating_window
Description :
   Create a floating window. A floating window floats over every other
   window class of the application.
   A floating window may or may not have a close box, and may or may not be
   resizable. It has also a title bar. The decoration should be smaller than
   the document window class.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The window is created in hidden state.
==============================================================================
*/

Window * Window::create_floating_window (const std::string & name, bool close_box_flag, bool resizable_flag)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_floating ();
   window_ptr->_window_impl.set_properties (close_box_flag, false, false, resizable_flag);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_floating_window_overlay_group
==============================================================================
*/

Window * Window::create_floating_window_overlay_group (const std::string & name, bool close_box_flag, bool resizable_flag)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_floating_overlay_group ();
   window_ptr->_window_impl.set_properties (close_box_flag, false, false, resizable_flag);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_floating_no_decoration_window
Description :
   Create a floating window with no decoration. A floating window floats over
   every other window class of the application.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The window is created in hidden state.
==============================================================================
*/

Window * Window::create_floating_no_decoration_window (const std::string & name)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_floating_no_decoration ();
   window_ptr->_window_impl.set_properties (false, false, false, false);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_no_decoration_window
Description :
   Create a window with no decoration. It has no boxes, cannot be resized,
   and does not have a title. It has a shadow on MacOS X.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The window is created in hidden state.
==============================================================================
*/

Window * Window::create_no_decoration_window (const std::string & name)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_no_decoration ();
   window_ptr->_window_impl.set_properties (false, false, false, false);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_no_decoration_window_overlay_group
Description :
   Create a window with no decoration. It has no boxes, cannot be resized,
   and does not have a title. It has a shadow an is placed over all other
   windows.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The window is created in hidden state.
==============================================================================
*/

Window * Window::create_no_decoration_window_overlay_group (const std::string & name)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_no_decoration ();
   window_ptr->_window_impl.set_overlay_group ();
   window_ptr->_window_impl.set_properties (false, false, false, false);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_no_decoration_no_shadow_window_overlay_group
Description :
   Create a window with no decoration. It has no boxes, cannot be resized,
   and does not have a title. It has a shadow an is placed over all other
   windows.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The window is created in hidden state.
==============================================================================
*/

Window * Window::create_no_decoration_no_shadow_window_overlay_group (const std::string & name)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_no_decoration ();
   window_ptr->_window_impl.set_overlay_group ();
   window_ptr->_window_impl.set_no_shadow ();
   window_ptr->_window_impl.set_properties (false, false, false, false);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_overlay_window
Description :
   Create a window with no decoration that will set upon all over windows.
   It has no boxes, has no resize box, and does not have a title.
   It has no shadow.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The window is created in hidden state.
==============================================================================
*/

Window * Window::create_overlay_window (const std::string & name)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_overlay ();
   window_ptr->_window_impl.set_properties (false, false, false, false);
   window_ptr->_window_impl.create ();
   
   return window_ptr;
}



/*
==============================================================================
Name : create_dialog
Description :
   Create a dialog. A dialog is modal relative to the other windows.
   A dialog does not have any boxes and cannot be resized.
   Until origin is explicitely set, it is placed to be in dialog position,
   which is centered horizontally, 1/3 vertically from the top in the passed
   parent window relative coords. 
   Each change of bounds reset it in dialog position.
   'name' is NOT the title of the window (use set_title for this). It is only
   meant as a way to access it through a path.
   The dialog is created in hidden state.
==============================================================================
*/

Window * Window::create_dialog (Window & parent_window, const std::string & name)
{
   check_name (name);
   
   Window * window_ptr = new Window;
   
   window_ptr->_window_impl.set_class_dialog ();
   window_ptr->_window_impl.set_properties (false, false, false, false);
   window_ptr->_window_impl.create ();
   
   window_ptr->_window_impl.reposition_alert (&parent_window, true);
   
   return window_ptr;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Window::~Window ()
{
}



/*
==============================================================================
Name : bind_menu_bar
Description: one window can have the menu bar. Windows platform only.
==============================================================================
*/

void  Window::bind_menu_bar ()
{
#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   _window_impl.bind_menu_bar ();
#endif
}



/*
==============================================================================
Name : show
==============================================================================
*/

void  Window::show ()
{
   _window_impl.show ();
}



/*
==============================================================================
Name : hide
==============================================================================
*/

void  Window::hide ()
{
   _window_impl.hide ();
}  



/*
==============================================================================
Name : collapse
==============================================================================
*/

void  Window::collapse ()
{
   _window_impl.collapse ();
}  



/*
==============================================================================
Name : set_origin
Description :
   Set the origin of the window. Origin is expressed in global coordinates
   of the whole computer desktop, with 0,0 being the top-left corner of
   the "main" screen.
   This sets the origin of the window with its decoration, (not only the
   content of the window). In the case where there is no decoration, it
   will set the content origin (incidentally)
==============================================================================
*/

void  Window::set_origin (const opa::Point & origin)
{
   _window_impl.set_origin (origin);
}



/*
==============================================================================
Name : get_origin
==============================================================================
*/

opa::Point  Window::get_origin ()
{
   return _window_impl.get_origin ();
}



/*
==============================================================================
Name : get_content_origin
Description :
   Returns the origin of the client part of the window (that is the window
   without decoration) in global coordinates of the whole computer desktop,
   with 0,0 being the top-left corner of the "main" screen.
==============================================================================
*/

opa::Point  Window::get_content_origin ()
{
   return _window_impl.get_content_origin ();
}



/*
==============================================================================
Name : set_bounds
Description :
   Set the bounds of the window
==============================================================================
*/

void  Window::set_bounds (const opa::Size & bounds)
{
   _window_impl.set_bounds (bounds);
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

opa::Size   Window::get_bounds ()
{
   return _window_impl.get_bounds ();
}



/*
==============================================================================
Name : set_limit_bounds
Description :
   Set the bounds in which the window bounds must be included.
   This is generally used to constraint the window bounds when resizing the
   window.
==============================================================================
*/

void  Window::set_limit_bounds (const opa::Size & min_bounds, const opa::Size & max_bounds)
{
   _window_impl.set_limit_bounds (min_bounds, max_bounds);
}



/*
==============================================================================
Name : resize_default_main_screen
Description :
   Resize the window so that it as "acceptable" size and origin on main
   screen.
   On Windows, it will take the largest aera into the application window.
   On MacOS X, it will take all the main screen, minus the menu bar on top,
   some pixels on the left and the right, and will go fully to the bottom
==============================================================================
*/

void  Window::resize_default_main_screen ()
{
   _window_impl.resize_default_main_screen ();
}



/*
==============================================================================
Name : is_document
==============================================================================
*/

bool  Window::is_document () const
{
   return _window_impl.is_class_document ();
}



/*
==============================================================================
Name : set_document
==============================================================================
*/

void  Window::set_document ()
{
   _window_impl.set_class_document ();
}



/*
==============================================================================
Name : set_no_decoration
==============================================================================
*/

void  Window::set_no_decoration ()
{
   _window_impl.set_class_no_decoration ();
}



/*
==============================================================================
Name : is_active
Description :
   tell if the window has the focus
==============================================================================
*/

bool  Window::is_active ()
{
   return _window_impl.is_active ();
}



/*
==============================================================================
Name : set_title
Description :
   Set the title of the window that will appear in the title bar, if the
   window has a title bar.
==============================================================================
*/

void  Window::set_title (const std::string & title)
{
   _window_impl.set_title (title);
}



/*
==============================================================================
Name : set_alpha
Description :
   Set the global alpha of the window when the window manager of the operating
   system support it.
==============================================================================
*/

void  Window::set_alpha (float alpha)
{
   _window_impl.set_alpha (alpha);
}



/*
==============================================================================
Name : set_modified
Description :
   Notify the window that its content was modified, and it should update
   its decoration to notify the user about this.
   On MacOS X, this will put a circle in the close box.
   On Windows, no standard behavior is defined.
   The implementation of this can be safely ignored.
==============================================================================
*/

void  Window::set_modified (bool modified_flag)
{
   _window_impl.set_modified (modified_flag);
}



/*
==============================================================================
Name : set_modified
Description :
   Associate the window to a path on disk. On MacOS X, this let the user
   quickly browse the tree containing the file, and will also gray out
   the icon when the window content is modified via 'set_modified'
==============================================================================
*/

void  Window::set_proxy_path (const std::string & proxy_path)
{
   _window_impl.set_proxy_path (proxy_path);
}


   
/*
==============================================================================
Name : window_bounds_changed
Description :
   This message is only sent by the window implementation when the bounds
   have changed (in particular to a response to set_bounds)
   It is then propagated to any views contained in the window.
   This message will be resent to the window implementation that will
   send it to its views
==============================================================================
*/

void  Window::window_bounds_changed ()
{
   _window_impl.window_bounds_changed ();
}



/*
==============================================================================
Name : get_system_window
==============================================================================
*/

SystemWindow   Window::get_system_window () const
{
   return _window_impl.get_system_window ();
}


   
/*
==============================================================================
Name : use_window_impl
Description :
   Returns window implementation.
   This is generally used for derived class to construct the window.
Throws : ?
==============================================================================
*/

WindowImpl &   Window::use_window_impl ()
{
   return (_window_impl);
}
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Window::Window ()
:  _window_impl (*this)
{
}



/*
==============================================================================
Name : check_name
Description :
   Make sure that the given frame name is valid.
   The only condition is that it does not include a delimitor '/'
Throws : ?
==============================================================================
*/

void  Window::check_name (const std::string & name)
{
   for (int i = 0; i < int (name.size ()); ++i)
   {
      assert (name [i] != '/');
   }
}



/*
==============================================================================
Name : req_close
==============================================================================
*/

bool  Window::req_close ()
{
   Event event;
   event._type = Event::Type_CLOSING;
   event._close_flag = true;
   
   _delegate.param_ref_signal (event);
   
   bool close_flag = event._close_flag;
   
   if (close_flag)
   {
      event._type = Event::Type_CLOSED;
      
      _delegate.param_ref_signal (event);
      
      hide ();
   }
   
   return close_flag;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
