/*****************************************************************************

        ApplicationWindowSimple.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70419

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



#if ! defined (ohm_opa_ApplicationWindowSimple_HEADER_INCLUDED)
#define  ohm_opa_ApplicationWindowSimple_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/BundleView.h"

#include "ohm/opal/Window.h"
#include "ohm/opal/private/PostActionListener.h"

#include <memory>



namespace ohm
{
namespace opa
{



class ApplicationWindowSimple
:  public opal::PostActionListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ApplicationWindowSimple (int argc, const char * const * argv);
                  ApplicationWindowSimple (int argc, const char * const * argv, const opa::Size & bounds);
   virtual        ~ApplicationWindowSimple ();
   
   void           run ();

   void           set (const std::string & key, const std::string & value);
   void           reset (const std::string & key);
   bool           has (const std::string & key) const;
   const std::string &
                  get (const std::string & key) const;
   
   BundleView &   use_view ();
   
   template <class T, void (T::*notify_pre_proc) (), void (T::*notify_post_proc) ()>
   void           bind_pre_post_ui (T & receiver);

   // opal::PostActionListener
   virtual void   notify_pre_action ();
   virtual void   notify_post_action ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::map <std::string, std::string> KeyValueMap;
   
   void           init (int argc, const char * const * argv, bool default_flag, const opa::Size & bounds);
   void           parse_arguments (int argc, const char * const * argv);
   
   // opal::Window delegate
   void           handle_event (opal::Window::Event & event);


   KeyValueMap    _key_value_map;

   std::auto_ptr <opal::Window>
                  _window_aptr;
   std::auto_ptr <BundleView>
                  _bundle_view_aptr;
   
   util::Delegate _delegate_pre;
   util::Delegate _delegate_post;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ApplicationWindowSimple ();
                  ApplicationWindowSimple (const ApplicationWindowSimple &other);
   ApplicationWindowSimple &
                  operator = (const ApplicationWindowSimple &other);
   bool           operator == (const ApplicationWindowSimple &other);
   bool           operator != (const ApplicationWindowSimple &other);

}; // class ApplicationWindowSimple



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opa/ApplicationWindowSimple.hpp"



#endif   // ohm_opa_ApplicationWindowSimple_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
