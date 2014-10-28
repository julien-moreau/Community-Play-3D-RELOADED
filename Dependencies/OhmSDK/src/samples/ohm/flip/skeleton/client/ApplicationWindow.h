/*****************************************************************************

        ApplicationWindow.h
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



#if ! defined (client_ApplicationWindow_HEADER_INCLUDED)
#define  client_ApplicationWindow_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/SkeletonGui.h"

#include "ohm/opal/Window.h"

#include <memory>



namespace ohm
{
namespace opa
{
class BundleView;
}
}



namespace client
{



class AppMsgHandler;

class ApplicationWindow
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ApplicationWindow ();
   virtual        ~ApplicationWindow ();
   
   void           run ();
   void           stop ();
   
   ohm::opa::BundleView &
                  use_view ();
   ohm::opal::Window &
                  use_window ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   // ohm::opal::Window delegate
   void           handle_event (ohm::opal::Window::Event & event);


   std::auto_ptr <ohm::opal::Window>
                  _window_aptr;
   std::auto_ptr <ohm::opa::BundleView>
                  _view_aptr;
   std::auto_ptr <SkeletonGui>
                  _gui_aptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ApplicationWindow (const ApplicationWindow &other);
   ApplicationWindow &
                  operator = (const ApplicationWindow &other);
   bool           operator == (const ApplicationWindow &other);
   bool           operator != (const ApplicationWindow &other);

}; // class ApplicationWindow



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // client_ApplicationWindow_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
