/*****************************************************************************

        ViewImpl.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66308

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



#if ! defined (ohm_opal_ViewImpl_HEADER_INCLUDED)
#define  ohm_opal_ViewImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/def.h"

#if (ohm_opal_API != ohm_opal_API_OPENGL)
   #error MacOSX version of ViewImpl works only with OpenGL API
#endif

#include "ohm/opa/Rect.h"
#include "ohm/opal/Data.h"
#include "ohm/opal/opengl/ViewImplOpenGL.h"
#include "ohm/opal/private/SystemEvent.h"



namespace ohm
{
namespace opal
{



class Window;
class View;
class Data;

class ViewImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  ViewImpl (Window & window, View & view);
   virtual        ~ViewImpl ();
   
   // internal
   void           vt_init ();
   void           vt_restore ();
   void           vt_update (const opa::Size & size);
   void           vt_draw (bool wait_vbr_flag);

   HIViewRef      get_system_view () const;
   View &         use_view ();
   static ViewImpl *
                  get (HIViewRef view_ref);
   
   void           set_origin (const opa::Point & point);
   void           set_bounds (const opa::Size & size);
   opa::Size      get_bounds ();
   void           post_view_layout_changed ();

   void           show ();
   void           hide ();
   
   void           invalidate (const opa::Rect & rect);
   void           invalidate_all ();
   
   void           acquire_keyboard_focus ();
   void           release_keyboard_focus ();
   
   void           begin_mouse_capture (bool relative_hidden_flag, bool block_flag);
   void           end_mouse_capture ();
   void           end_mouse_capture (const opa::Point & view_rel_pt);
   void           begin_mouse_gesture (bool block_flag);
   void           end_mouse_gesture ();

   void           debug_toggle_show_invalid_rect ();  



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
      
   friend class WindowImpl;
   
   static OSStatus
                  handle_event_proc (EventHandlerCallRef handler_call_ref, EventRef event_ref, void * user_data);
   OSStatus       handle_event (EventRef event_ref);
   OSStatus       handle_mouse_event (EventRef event_ref);
   OSStatus       handle_control_event (EventRef event_ref);
   OSStatus       handle_raw_keyboard_event (EventRef event_ref);
   OSStatus       handle_text_input_event (EventRef event_ref);
   
   opa::Point     get_drag_location (DragRef drag_ref);
   void           process_drag (Data & data, DragRef drag_ref);
   void           process_drag_item (Data::MimeTypeDataMap & mime_type_data_map, DragRef drag_ref, DragItemRef drag_item_ref);
   std::string    to_mime_type (FlavorType flavor_type);
   
   Window &       _window;
   View &         _view;
   
   ohm::opa::Size
                  _bounds;
   
   HIViewRef      _hi_view_ref;
   EventHandlerRef
                  _hi_view_event_handler_ref;
   bool           _focus_flag;
   archi::UInt32  _modifiers;
   
   bool           _inhibit_delta_flag;

   ViewImplOpenGL _view_impl;
   
   Data           _drag_data;

   SystemEvent    _sys_event;
   
   static PropertyCreator
                  _property_creator;
   static PropertyTag
                  _property_tag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ViewImpl (const ViewImpl &other);
   ViewImpl &     operator = (const ViewImpl &other);
   bool           operator == (const ViewImpl &other);
   bool           operator != (const ViewImpl &other);

}; // class ViewImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ViewImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
