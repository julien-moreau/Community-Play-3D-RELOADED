/*****************************************************************************

        Frame.h
        Copyright (c) 2004 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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



#if ! defined (ohm_opal_Frame_HEADER_INCLUDED)
#define  ohm_opal_Frame_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"
#include "ohm/opa/Margin.h"
#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opal/CursorLink.h"
#include "ohm/opal/def.h"
#include "ohm/opal/base/EventTypeCounter.h"
#include "ohm/opal/private/Frame_iterator.h"
#include "ohm/opal/private/FramePtrOrder.h"
#include "ohm/opal/private/MouseFrameRoundTripManager.h"
#include "ohm/opal/StandardMouseCursor.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/Event.h"
#include "ohm/util/Delegate.h"

#include <list>



namespace ohm
{
namespace opak
{
class Primitive;
}



namespace opal
{



class EventDispatcher;
class Cursor;
class View;

class Frame
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:  
   typedef Frame_iterator <Frame, FramePtrOrderList::iterator> iterator;
   typedef Frame_iterator <const Frame, FramePtrOrderList::const_iterator> const_iterator;
   typedef Frame_iterator <Frame, FramePtrOrderList::reverse_iterator> reverse_iterator;
   typedef Frame_iterator <const Frame, FramePtrOrderList::const_reverse_iterator> const_reverse_iterator;
   
   class PropagationClip
   {
   public:
                  PropagationClip (Frame & frame, const opa::Rect & clip_rect);
      virtual     ~PropagationClip ();
   
   private:
      Frame &     _frame;
   };
   
                  Frame (const char * name_0 = 0);
   virtual        ~Frame ();
   
   const char *   get_debug_name () const;
   
   void           enable_debug ();
   
   // Dimensions
   void           set_origin (const opa::Point & point);
   void           set_origin_x (float x);
   void           set_origin_y (float y);
   const opa::Point &
                  get_origin () const;
   void           set_relative_origin (const opa::Point & point);
   const opa::Point &
                  get_relative_origin () const;
   void           set_bounds (const opa::Size & size);
   void           set_bounds_width (float width);
   void           set_bounds_height (float height);
   const opa::Size &
                  get_bounds () const;
   void           set_no_clip ();
   void           set_margin (const opa::Margin & margin);
   const opa::Margin &
                  get_margin () const;

   void           activate (bool activate_flag = true);
   void           deactivate ();
   bool           is_active () const;
   
   // Tree
   void           insert_top (Frame & frame);
   void           insert_bottom (Frame & frame);
   void           remove (Frame & frame);
   void           remove_from_parents ();
   
   void           clear ();
   void           erase (iterator it);
   
   iterator       find (Frame & frame);
   
   iterator       begin ();
   const_iterator begin () const;
   iterator       end ();
   const_iterator end () const;
   reverse_iterator
                  rbegin ();
   const_reverse_iterator
                  rbegin () const;
   reverse_iterator
                  rend ();
   const_reverse_iterator
                  rend () const;
   
   const std::list <Frame *> &
                  get_parent_frame_ptr_arr () const;
   
   void           mark (archi::UInt32 mark_id);
   
   // Order
   void           set_order_nbr (Frame & frame, size_t order);
   void           set_order_nbr (size_t order);
   void           sort ();
   
   // Timers
   template <class DerivedType, void (DerivedType::*Method)(Event &)>
   void           install_regular_timer ();
   template <class DerivedType, void (DerivedType::*Method)(Event &)>
   void           install_one_shot_timer (double delay);
   template <class DerivedType, void (DerivedType::*Method)(Event &)>
   void           remove_timer ();
   
   // Mouse & Keyboard Capture
   void           acquire_mouse_capture ();
   void           release_mouse_capture ();
   void           take_mouse_capture ();
   
   void           acquire_keyboard_focus ();
   void           take_keyboard_focus ();
   void           release_keyboard_focus ();

   void           bind_next_keyboard_focus (Frame & frame);
   void           unbind_keyboard_focus ();
   void           next_keyboard_focus ();
   void           previous_keyboard_focus ();
   
   // Cursor
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   void           set_cursor_depth (int depth);
#endif
   void           show_cursor (bool show_flag = true);
   void           hide_cursor ();
   void           obscure_cursor ();
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
   void           set_cursor_default ();
   void           set_cursor (StandardMouseCursor cursor);
   void           set_cursor (Cursor & cursor);
   void           set_cursor (const std::string & path, const opa::Point & hot_point);
   void           force_cursor_default ();
   void           force_cursor (StandardMouseCursor cursor);
   void           force_cursor (Cursor & cursor);
   void           force_cursor (const std::string & path, const opa::Point & hot_point);
   void           release_cursor ();
#endif
   void           reposition_cursor (const opa::Point & location);
   
   // Post Event
   template <class T>
   void           post_conduct_changed_event (T & conduct, Event::Direction direction = Event::Direction_TOP_BOTTOM);
   template <class T>
   void           post (const T & event, Event::Direction direction = Event::Direction_TOP_BOTTOM);
   void           post_update_cursor ();
   
   // Event propagation
   void           add (EventDispatcher & dispatcher);
   void           remove (EventDispatcher & dispatcher);
   virtual void   propagate_children (Event & event);
   void           reset_children_propagation ();
   void           block_children_propagation ();
   void           handle_event (Event & event);
   virtual void   dispatch (Event & event);
   
   // To implement
   
   virtual void   layout_changed ();
   
   virtual void   mouse_entered (Event & event);
   virtual void   mouse_exited (Event & event);
   bool           is_mouse_in_bounds (Event & event);

   virtual void   mouse_target_lost (const Event & event);
   virtual void   keyboard_focus_lost (const Event & event);
   


/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // mouse moved/enter/exit handling
   void           increment_mouse_in ();
   void           decrement_mouse_in ();

   // called by AutoFramePtr
   void           set_destroyable ();
   
   // called by this class
   bool           is_destroyed () const;
   
   // called by FrameBin
   void           post_synchronize ();
   
   // called by PropagationClip
   void           enable_clip_propagation (const opa::Rect & clip_rect);
   void           disable_clip_propagation ();
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   void           increment (EventTypeCounter & counter);
   void           decrement (EventTypeCounter & counter);
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   friend class View;
   typedef std::list <Frame *>   FramePtrList;
   
   typedef std::list <EventDispatcher *> EventDispatcherPtrArr;
   
   class TimerCmd
   {
   public:
      enum Action
      {
                  Action_INSTALL_REGULAR_TIMER = 0,
                  Action_INSTALL_ONE_SHOT_TIMER,
                  Action_REMOVE_TIMER,
      };
      util::Delegate
                  _delegate;
      Action      _action;
      double      _delay;
   };

   void           bind (Frame & parent);
   void           unbind (Frame & parent);
   void           unbind_mfrt (Frame & parent, Frame & frame);

   void           bind_root (View & view);
   void           unbind_root ();
   
   void           bind (View & view, size_t view_cnt);
   void           unbind_view (size_t view_cnt);
   
   bool           is_in_parent_arr (Frame & frame);
   bool           is_in_child_arr (Frame & frame);
   FramePtrOrderList::iterator
                  find_child (Frame & frame);

   void           propagate_children_top_bottom (Event & event);
   void           propagate_children_bottom_top (Event & event);
   bool           propagate_children (Event & event, Frame & child, const opa::Point & location, bool spatial_flag, bool parent_in_flag);
   
   bool           dispatch_auto (Event & event);
   
   void           forward_latent_timer_cmd ();
   
   void           notify_layout_changed ();

   void           handle_event_debug (Event & event);
   
   bool           _resident_flag;
   
   View *         _view_ptr;
   size_t         _view_cnt;
   FramePtrList   _parent_frame_ptr_arr;
   bool           _root_flag;
   const char *   _name_0;
      
   FramePtrOrderList
                  _frame_ptr_arr;
   
   opa::Point     _origin;
   opa::Point     _rel_origin;
   opa::Size      _bounds;
   opa::Margin    _margin;
   bool           _clip_flag;
   bool           _active_flag;
   
   bool           _marked_flag;
   archi::UInt32  _mark_id;
   
   EventTypeCounter
                  _event_type_counter;

   std::vector <TimerCmd>
                  _latent_timer_cmd_arr;
   
   size_t         _mouse_in_cnt;
   MouseFrameRoundTripManager
                  _mouse_in_mfrt_manager;
   
   EventDispatcherPtrArr
                  _dispatcher_ptr_arr;
   bool           _iteration_guard_flag;
   bool           _abort_iteration_flag;
   bool           _children_propagated_flag;
   
   bool           _propagation_clip_flag;
   opa::Rect      _propagation_clip_rect;
   
   Frame *        _next_keyboard_focus_ptr;
   Frame *        _prev_keyboard_focus_ptr;

   opal::CursorLink
                  _cursor_link;
   
   bool           _debug_flag;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Frame (const Frame &other);
   Frame &        operator = (const Frame &other);
   bool           operator == (const Frame &other);
   bool           operator != (const Frame &other);

}; // class Frame



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Frame.hpp"



#endif   // ohm_opal_Frame_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
