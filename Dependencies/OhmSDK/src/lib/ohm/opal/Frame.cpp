/*****************************************************************************

        Frame.cpp
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

#include "ohm/archi/def.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/base/EventDispatcher.h"
#include "ohm/opal/private/FrameBin.h"
#include "ohm/util/trace.h"

#if defined (OHM_OPA_DEBUGGER)
   #include "ohm/opa/Debugger.h"
#endif

#if ! defined (NDEBUG)
   #define ohm_opal_CUR_FRAME_NAME const char * __frame_name_0__ = _name_0; __frame_name_0__
#else
   #define ohm_opal_CUR_FRAME_NAME ((void)(0))
#endif

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : PropagationClip::ctor
Description :
   See 'enable_clip_propagation'
   Generally used with 'Event::MultiPropagationState' to restrain the bounds
   of the frame, as seens by the frame system.
==============================================================================
*/

Frame::PropagationClip::PropagationClip (Frame & frame, const opa::Rect & clip_rect)
:  _frame (frame)
{
   _frame.enable_clip_propagation (clip_rect);
}



/*
==============================================================================
Name : PropagationClip::dtor
==============================================================================
*/

Frame::PropagationClip::~PropagationClip ()
{
   _frame.disable_clip_propagation ();
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Frame::Frame (const char * name_0)
:  _resident_flag (true)
,  _view_ptr (0)
,  _view_cnt (0)
,  _parent_frame_ptr_arr ()
,  _root_flag (false)
,  _name_0 (name_0)
,  _frame_ptr_arr ()
,  _origin (opa::Point::_zero)
,  _rel_origin (opa::Point::_zero)
,  _bounds (opa::Size::_infinity)
,  _margin (opa::Margin::_zero)
,  _clip_flag (true)
,  _active_flag (true)
,  _marked_flag (false)
,  _mark_id (0)

,  _event_type_counter ()
,  _mouse_in_cnt (0)
,  _mouse_in_mfrt_manager (*this)
,  _dispatcher_ptr_arr ()
,  _iteration_guard_flag (false)
,  _abort_iteration_flag (false)
,  _children_propagated_flag (false)

,  _propagation_clip_flag (false)
,  _propagation_clip_rect ()

,  _next_keyboard_focus_ptr (0)
,  _prev_keyboard_focus_ptr (0)

,  _debug_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Frame::~Frame ()
{
   try
   {
      unbind_keyboard_focus ();
      
      // every parent must have been detached at this point
      assert (_parent_frame_ptr_arr.empty ());
      assert (_view_ptr == 0);
      assert (_view_cnt == 0);
      
      {
         // children might be there, we just detach them
         FramePtrOrderList::iterator it = _frame_ptr_arr.begin ();
         const FramePtrOrderList::iterator it_end = _frame_ptr_arr.end ();
         
         for (; it != it_end ;)
         {
            FramePtrOrderList::iterator it_next = it;
            ++it_next;
            
            FramePtrOrder & elem = *it;
            
            if (!elem.was_removed ())
            {
               Frame * frame_ptr = elem.get_frame_ptr ();
               assert (frame_ptr != 0);
               
               // do not remove as this would postpone a post_synchronize
               // of a frame that won't exist anymore
               // just unbind our child.
               
               frame_ptr->unbind (*this);
               
               elem.remove ();
            }
            
            it = it_next;
         }
      }
   }
   catch (...)
   {
      ohm_util_TRACE_ERROR ("Uncatched Exception");
      assert (false);
   }
}



/*
==============================================================================
Name : get_debug_name
==============================================================================
*/

const char *   Frame::get_debug_name () const
{
   if (_name_0 != 0)
   {
      return _name_0;
   }
   else
   {
      return "<unnamed>";
   }
}



/*
==============================================================================
Name : enable_debug
==============================================================================
*/

void  Frame::enable_debug ()
{
   _debug_flag = true;
   
   EventTypeCounter event_type_counter;
   event_type_counter.add_debug ();
   
   increment (event_type_counter);
}



/*
==============================================================================
Name : set_origin
Description :
   Set origin of the frame relative to parent origin.
   The value is not necesseraly in pixel, and the parent may affect
   explicitely the topology of its children.
==============================================================================
*/

void  Frame::set_origin (const opa::Point & point)
{
   if (_origin != point)
   {
      _origin = point;
      
      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : set_origin_x
Description :
   Set x of the frame relative to parent origin.
   The value is not necesseraly in pixel, and the parent may affect
   explicitely the topology of its children.
==============================================================================
*/

void  Frame::set_origin_x (float x)
{
   if (_origin._x != x)
   {
      _origin._x = x;
      
      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : set_origin_y
Description :
   Set x of the frame relative to parent origin.
   The value is not necesseraly in pixel, and the parent may affect
   explicitely the topology of its children.
==============================================================================
*/

void  Frame::set_origin_y (float y)
{
   if (_origin._y != y)
   {
      _origin._y = y;
      
      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : get_origin
==============================================================================
*/

const opa::Point &   Frame::get_origin () const
{
   return _origin;
}



/*
==============================================================================
Name : set_relative_origin
Description :
   Set relative origin of the frame relative to its origin.
   The value is not necesseraly in pixel, and the parent may affect
   explicitely the topology of its children.
==============================================================================
*/

void  Frame::set_relative_origin (const opa::Point & point)
{
   if (_rel_origin != point)
   {
      _rel_origin = point;
      
      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : get_relative_origin
==============================================================================
*/

const opa::Point &   Frame::get_relative_origin () const
{
   return _rel_origin;
}



/*
==============================================================================
Name : set_bounds
Description :
   Set size of the frame.
   The value is not necesseraly in pixel, and the parent may affect
   explicitely the topology of its children.
==============================================================================
*/

void  Frame::set_bounds (const opa::Size & size)
{
   if (size != _bounds)
   {
      _bounds = size;

      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : set_bounds_width
Description :
   Set width of the frame.
   The value is not necesseraly in pixel, and the parent may affect
   explicitely the topology of its children.
==============================================================================
*/

void  Frame::set_bounds_width (float width)
{
   if (width != _bounds._width)
   {
      _bounds._width = width;

      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : set_bounds_height
Description :
   Set height of the frame.
   The value is not necesseraly in pixel, and the parent may affect
   explicitely the topology of its children.
==============================================================================
*/

void  Frame::set_bounds_height (float height)
{
   if (height != _bounds._height)
   {
      _bounds._height = height;

      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

const opa::Size & Frame::get_bounds () const
{
   return _bounds;
}



/*
==============================================================================
Name : set_no_clip
Description :
   Explicitely tell the frame to consider spatial events that are outside
   it bounds.
==============================================================================
*/

void  Frame::set_no_clip ()
{
   if (_clip_flag)
   {
      _clip_flag = false;
      
      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : set_margin
Description :
   Set the margin of the frame.
   The margin is always expressed in pixels.
==============================================================================
*/

void  Frame::set_margin (const opa::Margin & margin)
{
   if (margin != _margin)
   {
      _margin = margin;

      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : get_margin
==============================================================================
*/

const opa::Margin &  Frame::get_margin () const
{
   return _margin;
}



/*
==============================================================================
Name : activate
==============================================================================
*/

void  Frame::activate (bool activate_flag)
{
   if (activate_flag)
   {
      if (!_active_flag)
      {
         _active_flag = true;
         
         notify_layout_changed ();
      }
   }
   else
   {
      deactivate ();
   }
}



/*
==============================================================================
Name : deactivate
==============================================================================
*/

void  Frame::deactivate ()
{
   if (_active_flag)
   {
      _active_flag = false;
      
      notify_layout_changed ();
   }
}



/*
==============================================================================
Name : is_active
==============================================================================
*/

bool  Frame::is_active () const
{
   return _active_flag;
}



/*
==============================================================================
Name : insert_top
==============================================================================
*/

void  Frame::insert_top (Frame & frame)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (frame, &Frame::insert_top))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   assert (&frame != 0);
   assert (!is_in_child_arr (frame));
   
   FramePtrOrderList::iterator it
      = _frame_ptr_arr.insert (_frame_ptr_arr.end (), FramePtrOrder ());
   
   FramePtrOrder & elem = *it;
   elem.set_frame (frame);
   
   frame.bind (*this);
}



/*
==============================================================================
Name : insert_bottom
==============================================================================
*/

void  Frame::insert_bottom (Frame & frame)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (frame, &Frame::insert_bottom))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   assert (&frame != 0);
   assert (!is_in_child_arr (frame));
   
   FramePtrOrderList::iterator it
      = _frame_ptr_arr.insert (_frame_ptr_arr.begin (), FramePtrOrder ());
   
   FramePtrOrder & elem = *it;
   elem.set_frame (frame);
   
   frame.bind (*this);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  Frame::remove (Frame & frame)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (frame, &Frame::remove))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   FramePtrOrderList::iterator it = find_child (frame);
   assert (it != _frame_ptr_arr.end ());
   assert (!it->was_removed ());
   
   erase (iterator (it, _frame_ptr_arr.end ()));
}



/*
==============================================================================
Name : remove_from_parents
==============================================================================
*/

void  Frame::remove_from_parents ()
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*this, &Frame::remove_from_parents))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   FramePtrList::iterator it = _parent_frame_ptr_arr.begin ();
   const FramePtrList::iterator it_end = _parent_frame_ptr_arr.end ();
   
   while (!_parent_frame_ptr_arr.empty ())
   {
      FramePtrList::iterator it = _parent_frame_ptr_arr.begin ();

      Frame * parent_ptr = *it;
      assert (parent_ptr != 0);
      
      // will remove the iterator via a call to this->unbind (*parent_ptr)
      parent_ptr->remove (*this);
   }
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  Frame::clear ()
{
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ;)
   {
      iterator it_next = it;
      ++it_next;
      
      erase (it);
      
      it = it_next;
   }
}



/*
==============================================================================
Name : erase
Note :
   This will mark the iterator as removed, the pointed frame is not yet
   destroyed. After this call, the iterator is not accessible anymore using
   the public interface (begin (), end (), etc.)
==============================================================================
*/

void  Frame::erase (iterator it)
{
   Frame & frame = *it;
   
   frame.unbind (*this);
   
   FramePtrOrder & elem = *it._it;
   
   elem.remove ();
   
   FrameBin::use_instance ().require_post_synchronize (*this);
   
   // don't erase the iterator yet
}



/*
==============================================================================
Name : find
==============================================================================
*/

Frame::iterator   Frame::find (Frame & frame)
{
   return iterator (find_child (frame), _frame_ptr_arr.end ());
}



/*
==============================================================================
Name : begin
==============================================================================
*/

Frame::iterator   Frame::begin ()
{
   FramePtrOrderList::iterator it = _frame_ptr_arr.begin ();
   const FramePtrOrderList::iterator it_end = _frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      FramePtrOrder & elem = *it;
      
      if (!elem.was_removed ())
      {
         break;
      }
   }
   
   return iterator (it, _frame_ptr_arr.end ());
}



/*
==============================================================================
Name : begin
==============================================================================
*/

Frame::const_iterator   Frame::begin () const
{
   FramePtrOrderList::const_iterator it = _frame_ptr_arr.begin ();
   const FramePtrOrderList::const_iterator it_end = _frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      const FramePtrOrder & elem = *it;
      
      if (!elem.was_removed ())
      {
         break;
      }
   }
   
   return const_iterator (it, _frame_ptr_arr.end ());
}



/*
==============================================================================
Name : end
==============================================================================
*/

Frame::iterator   Frame::end ()
{
   return iterator (_frame_ptr_arr.end (), _frame_ptr_arr.end ());
}



/*
==============================================================================
Name : end
==============================================================================
*/

Frame::const_iterator   Frame::end () const
{
   return const_iterator (_frame_ptr_arr.end (), _frame_ptr_arr.end ());
}



/*
==============================================================================
Name : rbegin
==============================================================================
*/

Frame::reverse_iterator Frame::rbegin ()
{
   FramePtrOrderList::reverse_iterator it = _frame_ptr_arr.rbegin ();
   const FramePtrOrderList::reverse_iterator it_end = _frame_ptr_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      FramePtrOrder & elem = *it;
      
      if (!elem.was_removed ())
      {
         break;
      }
   }
   
   return reverse_iterator (it, _frame_ptr_arr.rend ());
}



/*
==============================================================================
Name : rbegin
==============================================================================
*/

Frame::const_reverse_iterator Frame::rbegin () const
{
   FramePtrOrderList::const_reverse_iterator it = _frame_ptr_arr.rbegin ();
   const FramePtrOrderList::const_reverse_iterator it_end = _frame_ptr_arr.rend ();
   
   for (; it != it_end ; ++it)
   {
      const FramePtrOrder & elem = *it;
      
      if (!elem.was_removed ())
      {
         break;
      }
   }
   
   return const_reverse_iterator (it, _frame_ptr_arr.rend ());
}



/*
==============================================================================
Name : rend
==============================================================================
*/

Frame::reverse_iterator Frame::rend ()
{
   return reverse_iterator (_frame_ptr_arr.rend (), _frame_ptr_arr.rend ());
}



/*
==============================================================================
Name : rend
==============================================================================
*/

Frame::const_reverse_iterator Frame::rend () const
{
   return const_reverse_iterator (_frame_ptr_arr.rend (), _frame_ptr_arr.rend ());
}



/*
==============================================================================
Name : get_parent_frame_ptr_arr
==============================================================================
*/

const std::list <Frame *> &   Frame::get_parent_frame_ptr_arr () const
{
   return _parent_frame_ptr_arr;
}



/*
==============================================================================
Name : set_order_nbr
==============================================================================
*/

void  Frame::set_order_nbr (Frame & frame, size_t order)
{
   FramePtrOrderList::iterator it = find_child (frame);
   assert (it != _frame_ptr_arr.end ());
   assert (!it->was_removed ());
   
   it->set_order (order);
}



/*
==============================================================================
Name : mark
==============================================================================
*/

void  Frame::mark (archi::UInt32 mark_id)
{
   assert (!_marked_flag);
   
   _marked_flag = true;
   _mark_id = mark_id;
}



/*
==============================================================================
Name : set_order_nbr
==============================================================================
*/

void  Frame::set_order_nbr (size_t order)
{
   FramePtrList::iterator it = _parent_frame_ptr_arr.begin ();
   const FramePtrList::iterator it_end = _parent_frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Frame * parent_ptr = *it;
      assert (parent_ptr != 0);
      
      // will remove the iterator via a call to this->unbind (*parent_ptr)
      parent_ptr->set_order_nbr (*this, order);
   }
}



/*
==============================================================================
Name : sort
==============================================================================
*/

void  Frame::sort ()
{
   _frame_ptr_arr.sort ();
   
   if (_iteration_guard_flag)
   {
      _abort_iteration_flag = true;
   }
}



/*
==============================================================================
Name : acquire_mouse_capture
==============================================================================
*/

void  Frame::acquire_mouse_capture ()
{
   assert (_view_ptr != 0);
   
   if (_view_ptr != 0)
   {
      _view_ptr->acquire_mouse_capture (*this);
   }
}



/*
==============================================================================
Name : release_mouse_capture
==============================================================================
*/

void  Frame::release_mouse_capture ()
{
   assert (_view_ptr != 0);
   
   if (_view_ptr != 0)
   {
      if (_view_ptr->get_mouse_capture_frame_ptr () == this)
      {
         _view_ptr->release_mouse_capture (*this);
      }
   }
}



/*
==============================================================================
Name : take_mouse_capture
Description :
   Request to acquire mouse capture, but in the case where the frame which
   want mouse capture is not the current target of the event.
==============================================================================
*/

void  Frame::take_mouse_capture ()
{
   assert (_view_ptr != 0);
   
   if (_view_ptr != 0)
   {
      _view_ptr->give_mouse_capture (*this);
   }
}



/*
==============================================================================
Name : acquire_keyboard_focus
Description :
   Request to acquire keyboard focus.
Note :
   In the case where the frame which want mouse capture is not the current
   target of the event, see 'take_keyboard_focus'
==============================================================================
*/

void  Frame::acquire_keyboard_focus ()
{
   assert (_view_ptr != 0);
   
   if (_view_ptr != 0)
   {
      _view_ptr->acquire_keyboard_focus (*this);
   }
}



/*
==============================================================================
Name : take_keyboard_focus
Description :
   Request to acquire keyboard focus, but in the case where the frame which
   want mouse capture is not the current target of the event.
==============================================================================
*/

void  Frame::take_keyboard_focus ()
{
   assert (_view_ptr != 0);
   
   if (_view_ptr != 0)
   {
      _view_ptr->give_keyboard_focus (*this);
   }
}



/*
==============================================================================
Name : release_keyboard_focus
==============================================================================
*/

void  Frame::release_keyboard_focus ()
{
   assert (_view_ptr != 0);
   
   if (_view_ptr != 0)
   {
      if (_view_ptr->get_keyboard_focus_frame_ptr () == this)
      {
         _view_ptr->release_keyboard_focus (*this);
      }
   }
}



/*
==============================================================================
Name : bind_next_keyboard_focus
Description :
   bind 'frame' as the next frame to be focused when 'this' decides to pass
   focus to the next frame.
   The inverse operation, 'previous_keyboard_focus', is deduced from this
   chain link.
   'this' must not already be binded (ie. use unbind_keyboard_focus if
   necessary)
==============================================================================
*/

void  Frame::bind_next_keyboard_focus (Frame & frame)
{
   assert (_next_keyboard_focus_ptr == 0);
   assert (frame._prev_keyboard_focus_ptr == 0);
   
   _next_keyboard_focus_ptr = &frame;
   frame._prev_keyboard_focus_ptr = this;
}



/*
==============================================================================
Name : unbind_keyboard_focus
Description :
   unbind 'this' from the keyboard focus chain, and relinks focus siblings
   if necessary.
==============================================================================
*/

void  Frame::unbind_keyboard_focus ()
{
   if (_next_keyboard_focus_ptr != 0)
   {
      assert (_next_keyboard_focus_ptr->_prev_keyboard_focus_ptr == this);
      
      _next_keyboard_focus_ptr->_prev_keyboard_focus_ptr = _prev_keyboard_focus_ptr;
   }
   
   if (_prev_keyboard_focus_ptr != 0)
   {
      assert (_prev_keyboard_focus_ptr->_next_keyboard_focus_ptr == this);
      
      _prev_keyboard_focus_ptr->_next_keyboard_focus_ptr = _next_keyboard_focus_ptr;
   }
}



/*
==============================================================================
Name : next_keyboard_focus
==============================================================================
*/

void  Frame::next_keyboard_focus ()
{
   assert (_view_ptr != 0);
   
   if ((_view_ptr != 0) && (_next_keyboard_focus_ptr != 0))
   {
      _view_ptr->give_keyboard_focus (*_next_keyboard_focus_ptr);
   }
}



/*
==============================================================================
Name : previous_keyboard_focus
==============================================================================
*/

void  Frame::previous_keyboard_focus ()
{
   assert (_view_ptr != 0);
   
   if ((_view_ptr != 0) && (_prev_keyboard_focus_ptr != 0))
   {
      _view_ptr->give_keyboard_focus (*_prev_keyboard_focus_ptr);
   }
}



/*
==============================================================================
Name : set_cursor_depth
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::set_cursor_depth (int depth)
{
   _cursor_link.set_depth (depth);
}
#endif



/*
==============================================================================
Name : show_cursor
==============================================================================
*/

void  Frame::show_cursor (bool show_flag)
{
   _cursor_link.show (show_flag);
}



/*
==============================================================================
Name : hide_cursor
==============================================================================
*/

void  Frame::hide_cursor ()
{
   show_cursor (false);
}



/*
==============================================================================
Name : obscure_cursor
Description :
   Hide the cursor, and makes it automatically reappear as the user moves
   the mouse.
==============================================================================
*/

void  Frame::obscure_cursor ()
{
   _cursor_link.obscure ();
}



/*
==============================================================================
Name : set_cursor_default
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::set_cursor_default ()
{
   _cursor_link.set_default ();
}
#endif



/*
==============================================================================
Name : set_cursor
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::set_cursor (StandardMouseCursor cursor)
{
   _cursor_link.set (cursor);
}
#endif



/*
==============================================================================
Name : set_cursor
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::set_cursor (Cursor & cursor)
{
   _cursor_link.set (cursor);
}
#endif



/*
==============================================================================
Name : set_cursor
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::set_cursor (const std::string & path, const opa::Point & hot_point)
{
   _cursor_link.set (path, hot_point);
}
#endif



/*
==============================================================================
Name : force_cursor_default
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::force_cursor_default ()
{
   _cursor_link.force_default ();
}
#endif



/*
==============================================================================
Name : force_cursor
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::force_cursor (StandardMouseCursor cursor)
{
   _cursor_link.force (cursor);
}
#endif



/*
==============================================================================
Name : force_cursor
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::force_cursor (Cursor & cursor)
{
   _cursor_link.force (cursor);
}
#endif



/*
==============================================================================
Name : force_cursor
Description :
   Change the mouse cursor shape.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::force_cursor (const std::string & path, const opa::Point & hot_point)
{
   _cursor_link.force (path, hot_point);
}
#endif



/*
==============================================================================
Name : release_cursor
Description :
   Restore the mouse cursor shape to the last shape on the stack.
   The system is stack based to ensure that leaving a frame to its parent
   will automatically reset the cursor to the previous shape.
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  Frame::release_cursor ()
{
   _cursor_link.unset ();
}
#endif



/*
==============================================================================
Name : reposition_cursor
==============================================================================
*/

void  Frame::reposition_cursor (const opa::Point & location)
{
   // to do
}



/*
==============================================================================
Name : post_update_cursor
==============================================================================
*/

void  Frame::post_update_cursor ()
{
   if (_view_ptr != 0)
   {
      _view_ptr->post_update_cursor ();
   }
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  Frame::add (EventDispatcher & dispatcher)
{
   assert (&dispatcher != 0);
   
#if ! defined (NDEBUG)
   {
      EventDispatcherPtrArr::iterator it = _dispatcher_ptr_arr.begin ();
      const EventDispatcherPtrArr::iterator it_end = _dispatcher_ptr_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         assert (*it != &dispatcher);
      }
   }
#endif
   
   _dispatcher_ptr_arr.push_back (&dispatcher);
   
   EventTypeCounter event_type_counter;
   
   dispatcher.get_cnt (event_type_counter);
   
   increment (event_type_counter);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  Frame::remove (EventDispatcher & dispatcher)
{
   assert (&dispatcher != 0);
   
#if ! defined (NDEBUG)
   {
      EventDispatcherPtrArr::iterator it = _dispatcher_ptr_arr.begin ();
      const EventDispatcherPtrArr::iterator it_end = _dispatcher_ptr_arr.end ();
      
      for (; it != it_end ; ++it)
      {
         if (*it == &dispatcher)
         {
            break;
         }
      }
      
      assert (it != it_end);
   }
#endif
   
   _dispatcher_ptr_arr.remove (&dispatcher);
   
   EventTypeCounter event_type_counter;
   
   dispatcher.get_cnt (event_type_counter);
   
   decrement (event_type_counter);
}



/*
==============================================================================
Name : propagate_children
==============================================================================
*/

void  Frame::propagate_children (Event & event)
{
   ohm_opal_CUR_FRAME_NAME;
   
#if defined (OHM_OPA_DEBUGGER)
   if (opa::Debugger::use_instance ().is_tracing (event._type))
   {
      ohm_util_TRACE_INDENT (2);
   }
#endif
   
   if (!_children_propagated_flag)
   {
      _children_propagated_flag = true;
      
      FramePath target_frame_path;
      bool target_frame_path_changed_flag = false;
      
      if (event.is_target_frame_path ())
      {
         // the target frame is explicitely propagating the event
         // allow its child dispatcher to dispatch them the event
         
         target_frame_path = event._target_frame_path;
         
         event._target_frame_path.clear ();
         target_frame_path_changed_flag = true;
      }
      
      _iteration_guard_flag = true;
      _abort_iteration_flag = false;
      
      if (_marked_flag)
      {
         event.mark (&typeid (*this), _mark_id);
      }
               
      switch (event._direction)
      {
      case Event::Direction_TOP_BOTTOM:
         propagate_children_top_bottom (event);
         break;

      case Event::Direction_BOTTOM_TOP:
         propagate_children_bottom_top (event);
         break;
      
      default:
         assert (false);
         break;
      }
      
      _iteration_guard_flag = false;
      _abort_iteration_flag = false;
      
      if (target_frame_path_changed_flag)
      {
         event._target_frame_path = target_frame_path;
         target_frame_path_changed_flag = false;
      }
   }

#if defined (OHM_OPA_DEBUGGER)
   if (opa::Debugger::use_instance ().is_tracing (event._type))
   {
      ohm_util_TRACE_INDENT (-2);
   }
#endif
}



/*
==============================================================================
Name : reset_children_propagation
Description :
   Reset the current children propagation state.
   This allows to propagate the same event multiple times to the same frame.
   This is generally used with 'Event::MultiPropagationState'
==============================================================================
*/

void  Frame::reset_children_propagation ()
{
   _children_propagated_flag = false;
}



/*
==============================================================================
Name : block_children_propagation
Description :
   Tells explicitely that the event should not be propagated to children.
==============================================================================
*/

void  Frame::block_children_propagation ()
{
   _children_propagated_flag = true;
}



/*
==============================================================================
Name : handle_event
Description :
==============================================================================
*/

void  Frame::handle_event (Event & event)
{
   ohm_opal_CUR_FRAME_NAME;
   
#if defined (OHM_OPA_DEBUGGER)
   if (opa::Debugger::use_instance ().is_tracing (event._type))
   {
      ohm_util_TRACE_INFO1 ("%1%", _name_0);
   }
#endif

#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (*this, &Frame::handle_event))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif
   
   _children_propagated_flag = false;
   
   assert (event._type != Event::Type_NONE);
   assert (int (event._type) < Event::Type_NBR_ELT);
   
   event._loop_nbr = 0;
   
   if (event._type == Event::Type_MOUSE_MOVED)
   {
      if (event._target_frame_path.empty ())
      {
         bool cur_mouse_in_flag = event._mouse_in_flag && !event.is_mapping_changed ();
         
         assert (_view_ptr != 0);
         
         bool can_notify_flag
            = (event._pass == Event::Pass_SPATIAL)
            && (_view_ptr->get_mouse_capture_frame_ptr () != this);
         
         if (cur_mouse_in_flag && can_notify_flag)
         {
            bool entered_flag = _mouse_in_mfrt_manager.notify_mouse_in (event);
            
            if (entered_flag)
            {
               mouse_entered (event);
            }
         }
         
         bool propagate_flag = true;
         
         bool dispatch_flag = cur_mouse_in_flag || (event._pass == Event::Pass_CRUMB);
         
         if (_event_type_counter.does_support (event) && dispatch_flag)
         {
            propagate_flag = dispatch_auto (event);
         }
         
         if (propagate_flag)
         {
            propagate_children (event);
         }
         
         if ((!cur_mouse_in_flag) && can_notify_flag)
         {
            bool exited_flag = _mouse_in_mfrt_manager.notify_mouse_out (event);
            
            if (exited_flag)
            {
               mouse_exited (event);
            }
         }
      }
      else if (event.is_target_frame_path ())
      {
         if (_event_type_counter.does_support (event))
         {
            dispatch (event);
         }
      }
      else
      {
         // don't dispatch to this frame
         // (but propagate)
         
         propagate_children (event);
      }
   }
   else
   {
      bool propagate_flag = true;
      
      if (_event_type_counter.does_support (event))
      {
         if (event._type == Event::Type_DEBUG)
         {
            if (_debug_flag)
            {
               handle_event_debug (event);
            }
         }
         else if (event._target_frame_path.empty ())
         {
            propagate_flag = dispatch_auto (event);
         }
         else if (event.is_target_frame_path ())
         {
            dispatch (event);
            
            propagate_flag = false;
         }
         else
         {
            // don't dispatch to this frame
         }
      }
      
      if (propagate_flag)
      {
         propagate_children (event);
      }
   }
}



/*
==============================================================================
Name : dispatch
==============================================================================
*/

void  Frame::dispatch (Event & event)
{
   EventDispatcherPtrArr::iterator it = _dispatcher_ptr_arr.begin ();
   const EventDispatcherPtrArr::iterator it_end = _dispatcher_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      EventDispatcher * dispatcher_ptr = *it;
      assert (dispatcher_ptr != 0);
      
      dispatcher_ptr->dispatch (event);
      
      if (event.is_deflected ())
      {
         break;
      }
   }
   
#if defined (OHM_OPA_DEBUGGER)
   if (event.is_deflected () && (opa::Debugger::use_instance ().is_tracing (event._type)))
   {
      ohm_util_TRACE_INFO ("propagation stopped");
   }
#endif

   // reset deflection
   event.deflect (false);
}



/*
==============================================================================
Name : layout_changed
Description :
   Notification that this frame layout changed (either its origin or bounds)
   Typical usage for a frame is to relayout its child when it receive this
   message
==============================================================================
*/

void  Frame::layout_changed ()
{
   // default implementation : nothing
}



/*
==============================================================================
Name : mouse_entered
==============================================================================
*/

void  Frame::mouse_entered (Event & event)
{
   // default implementation : nothing
}



/*
==============================================================================
Name : mouse_exited
==============================================================================
*/

void  Frame::mouse_exited (Event & event)
{
   // default implementation : nothing
}



/*
==============================================================================
Name : is_mouse_in_bounds
Description :
   Returns true if the mouse location relative to the current event
   propagation is in the frame bounds.
==============================================================================
*/

bool  Frame::is_mouse_in_bounds (Event & event)
{
   bool ret_val = false;
   
   if (_mouse_in_cnt > 0)
   {
      // the event is translated in frame coords
      // (but not in child frame transform)
      
      opa::Point mouse_location = event._mouse._location;
      
      opa::Rect rect (get_relative_origin (), get_bounds ());
      
      ret_val = rect.contains (mouse_location);
   }
   
   return ret_val;
}



/*
==============================================================================
Name : mouse_target_lost
==============================================================================
*/

void  Frame::mouse_target_lost (const Event & event)
{
   // default implementation : nothing
}



/*
==============================================================================
Name : keyboard_focus_lost
==============================================================================
*/

void  Frame::keyboard_focus_lost (const Event & event)
{
   // default implementation : nothing
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : increment_mouse_in
==============================================================================
*/

void  Frame::increment_mouse_in ()
{
   ++_mouse_in_cnt;
}



/*
==============================================================================
Name : decrement_mouse_in
==============================================================================
*/

void  Frame::decrement_mouse_in ()
{
   assert (_mouse_in_cnt > 0);
   
   if (_mouse_in_cnt)
   {
      --_mouse_in_cnt;
   }
}



/*
==============================================================================
Name : set_destroyable
Description :
   Mark this frame as ready for deletion.
==============================================================================
*/

void  Frame::set_destroyable ()
{
   _resident_flag = false;
   
   FrameBin::use_instance ().set_destroyable (*this);
}



/*
==============================================================================
Name : is_destroyed
==============================================================================
*/

bool  Frame::is_destroyed () const
{
   return !_resident_flag;
}



/*
==============================================================================
Name : post_synchronize
Description :
   Clean removed iterators
==============================================================================
*/

void  Frame::post_synchronize ()
{
   FramePtrOrderList::iterator it = _frame_ptr_arr.begin ();
   const FramePtrOrderList::iterator it_end = _frame_ptr_arr.end ();
   
   for (; it != it_end ;)
   {
      FramePtrOrderList::iterator it_next = it;
      ++it_next;
      
      FramePtrOrder & elem = *it;
      
      if (elem.was_removed ())
      {
         _frame_ptr_arr.erase (it);
      }
      
      it = it_next;
   }
}



/*
==============================================================================
Name : enable_clip_propagation
Description :
   Used by 'PropagationClip' to simulate a frame that would have temporaly
   a rectangle which is less than the frame rectangle.
   'clip_rect' is relative to the frame (that is its origin modulated by
   its relative origin)
   This is used in 'propagate_children_top_bottom'
   and 'propagate_children_bottom_top'
==============================================================================
*/

void  Frame::enable_clip_propagation (const opa::Rect & clip_rect)
{
   _propagation_clip_flag = true;
   _propagation_clip_rect = clip_rect;
}



/*
==============================================================================
Name : enable_clip_propagation
Description :
   Used by 'PropagationClip' to simulate a frame that would have temporaly
   a rectangle which is less than the frame rectangle.
==============================================================================
*/

void  Frame::disable_clip_propagation ()
{
   _propagation_clip_flag = false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : increment
==============================================================================
*/

void  Frame::increment (EventTypeCounter & counter)
{
   _event_type_counter.increment (counter);
   
   FramePtrList::iterator it = _parent_frame_ptr_arr.begin ();
   FramePtrList::iterator it_end = _parent_frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Frame * parent_ptr = *it;
      
      parent_ptr->increment (counter);
   }
}



/*
==============================================================================
Name : decrement
==============================================================================
*/

void  Frame::decrement (EventTypeCounter & counter)
{
   _event_type_counter.decrement (counter);
   
   FramePtrList::iterator it = _parent_frame_ptr_arr.begin ();
   FramePtrList::iterator it_end = _parent_frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Frame * parent_ptr = *it;
      
      parent_ptr->decrement (counter);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : bind
Description :
   Bind 'this' to 'parent'
Note :
   'parent' may be binded to the view or not
==============================================================================
*/

void  Frame::bind (Frame & parent)
{
   assert (&parent != 0);
   assert (!is_in_parent_arr (parent));
   
   _parent_frame_ptr_arr.push_back (&parent);
   _root_flag = false;
   
   if (parent._view_ptr != 0)
   {
      assert (parent._view_cnt > 0);
      
      bind (*parent._view_ptr, parent._view_cnt);
   }
   else
   {
      assert (parent._view_cnt == 0);
   }
   
   assert (_view_cnt >= parent._view_cnt);
   
   if (_parent_frame_ptr_arr.size () == 1)
   {
      assert (_view_cnt == parent._view_cnt);
   }
   
   parent.increment (_event_type_counter);
   
   notify_layout_changed ();
}



/*
==============================================================================
Name : unbind
Description :
   Unbind 'this' frame from its parent frame.
==============================================================================
*/

void  Frame::unbind (Frame & parent)
{
   assert (&parent != 0);
   assert (is_in_parent_arr (parent));
   
   parent.decrement (_event_type_counter);
   _mouse_in_mfrt_manager.unbind (parent);
   unbind_mfrt (parent, *this);
   
   _parent_frame_ptr_arr.remove (&parent);
   
   notify_layout_changed ();
   
   assert (_view_cnt >= parent._view_cnt);
   
   // parent._view_cnt might be 0
   unbind_view (parent._view_cnt);
   
   if (_view_cnt > 0)
   {
      assert (_view_cnt >= parent._view_cnt);
   }
   
   if (_parent_frame_ptr_arr.size () == 1)
   {
      Frame & rem_parent = *_parent_frame_ptr_arr.front ();
      
      assert (_view_cnt == rem_parent._view_cnt);
   }
   
   if (_parent_frame_ptr_arr.empty ())
   {
      assert (_view_cnt == 0);
   }
}



/*
==============================================================================
Name : unbind_mfrt
Description :
   Notify & propagate all mfrt that every frame paths containing '&parent'
   followed by '&frame' (whatever the loop nbr) are going to get invalid.
==============================================================================
*/

void  Frame::unbind_mfrt (Frame & parent, Frame & frame)
{
   _mouse_in_mfrt_manager.unbind (parent, frame);
   
   // propagate
   
   FramePtrOrderList::iterator it = _frame_ptr_arr.begin ();
   const FramePtrOrderList::iterator it_end = _frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      FramePtrOrder & elem = *it;
      
      if (!elem.was_removed ())
      {
         Frame * frame_ptr = elem.get_frame_ptr ();
         assert (frame_ptr != 0);
         
         frame_ptr->unbind_mfrt (parent, frame);
      }
   }
}



/*
==============================================================================
Name : bind_root
Note :
   Only for use to the root frame
==============================================================================
*/

void  Frame::bind_root (View & view)
{
   assert (_view_ptr == 0);
   assert (&view != 0);
   assert (_parent_frame_ptr_arr.empty ());
   
   _root_flag = true;

   bind (view, 1);
   
   notify_layout_changed ();
}



/*
==============================================================================
Name : unbind_root
Note :
   Only for use to the root frame
==============================================================================
*/

void  Frame::unbind_root ()
{
   assert (_view_ptr != 0);
   assert (_parent_frame_ptr_arr.empty ());
   assert (_root_flag);

   notify_layout_changed ();

   unbind_view (1);
}



/*
==============================================================================
Name : bind
Description :
   Bind to the view.
Note :
   A frame might be binded to a parent frame but not to the view, for example
   in the case where the parent frame has been removed from its parent frame
==============================================================================
*/

void  Frame::bind (View & view, size_t view_cnt)
{
   assert (&view != 0);
   assert (view_cnt > 0);
   
   if (_view_ptr == 0)
   {
      assert (_view_cnt == 0);
      
      _view_ptr = &view;
      _view_ptr->bind (*this);
      
      forward_latent_timer_cmd ();
   }
   else
   {
      assert (_view_cnt > 0);
   }

   _view_cnt += view_cnt;

   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ; ++it)
   {
      Frame & child = *it;
      assert (&child != 0);
      
      child.bind (view, view_cnt);
   }
}



/*
==============================================================================
Name : unbind_view
Description :
   Unbind from the view.
Note :
   A frame might be binded to a parent frame but not to the view, for example
   in the case where the parent frame has been removed from its parent frame.
Note :
   A frame may have also multiple parents, so that it must be unbinded from
   the view iff when no path exist to the view.
   In the case where the view has no parent, we can unbind. However a child
   frame may have a parent frame from another path which is still attached
   to a view.
   This is all handled by counting the number of path we have to the view.
==============================================================================
*/

void  Frame::unbind_view (size_t view_cnt)
{
   assert (_view_cnt >= view_cnt);
   
   // 'view_cnt' might be 0 in the case where the
   // parent was already unbinded from the view
   
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ; ++it)
   {
      Frame & child = *it;
      assert (&child != 0);
      
      child.unbind_view (view_cnt);
   }
   
   _view_cnt -= view_cnt;
   
   if (_view_cnt == 0)
   {
      if (_view_ptr != 0)
      {
         _view_ptr->unbind (*this);
         _view_ptr = 0;
      }
      else
      {
         assert (view_cnt == 0);
      }
   }
}



/*
==============================================================================
Name : is_in_parent_arr
==============================================================================
*/

bool  Frame::is_in_parent_arr (Frame & frame)
{
   FramePtrList::iterator it = _parent_frame_ptr_arr.begin ();
   FramePtrList::iterator it_end = _parent_frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      Frame * parent_ptr = *it;
      
      if (parent_ptr == &frame)
      {
         break;
      }
   }
   
   bool is_in_flag = (it != it_end);
   
   return is_in_flag;
}



/*
==============================================================================
Name : is_in_child_arr
Note :
   It also checks for frame that are in 'removed' state.
==============================================================================
*/

bool  Frame::is_in_child_arr (Frame & frame)
{
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ; ++it)
   {
      Frame * frame_ptr = &*it;
      
      if (frame_ptr == &frame)
      {
         break;
      }
   }
   
   bool is_in_flag = (it != it_end);
   
   return is_in_flag;
}



/*
==============================================================================
Name : find_child
==============================================================================
*/

FramePtrOrderList::iterator   Frame::find_child (Frame & frame)
{
   FramePtrOrderList::iterator it = _frame_ptr_arr.begin ();
   const FramePtrOrderList::iterator it_end = _frame_ptr_arr.end ();
   
   for (; it != it_end ; ++it)
   {
      FramePtrOrder & elem = *it;
      
      if (!elem.was_removed ())
      {
         if (elem.get_frame_ptr () == &frame)
         {
            break;
         }
      }
   }
   
   return it;
}



/*
==============================================================================
Name : propagate_children_top_bottom
==============================================================================
*/

void  Frame::propagate_children_top_bottom (Event & event)
{
   bool spatial_flag = event.is_spatial ();
   
   opa::Point location;
   
   if (spatial_flag)
   {
      location = event._mouse._location;
   }
   
   bool parent_in_flag = event._mouse_in_flag;
   
   reverse_iterator it = rbegin ();
   const reverse_iterator it_end = rend ();
   
   for (; it != it_end ;)
   {
      reverse_iterator next_it = it;
      ++next_it;
      
      if (it._it->was_removed ())
      {
         continue;
      }
      
      Frame & child = *it;
      
      bool loop_flag = propagate_children (event, child, location, spatial_flag, parent_in_flag);
      
      if (!loop_flag)
      {
         break;
      }
      
      it = next_it;
   }
}



/*
==============================================================================
Name : propagate_children_bottom_top
==============================================================================
*/

void  Frame::propagate_children_bottom_top (Event & event)
{
   bool spatial_flag = event.is_spatial ();
   
   opa::Point location;
   
   if (spatial_flag)
   {
      location = event._mouse._location;
   }
   
   bool parent_in_flag = event._mouse_in_flag;
   
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ;)
   {
      iterator next_it = it;
      ++next_it;
      
      if (it._it->was_removed ())
      {
         continue;
      }
      
      Frame & child = *it;
      
      bool loop_flag = propagate_children (event, child, location, spatial_flag, parent_in_flag);
      
      if (!loop_flag)
      {
         break;
      }
      
      it = next_it;
   }
}



/*
==============================================================================
Name : propagate_children
==============================================================================
*/

bool  Frame::propagate_children (Event & event, Frame & child, const opa::Point & location, bool spatial_flag, bool parent_in_flag)
{
#if defined (OHM_OPA_DEBUGGER)
#if (ohm_archi_SYS != ohm_archi_SYS_WIN)
   if (opa::Debugger::use_instance ().has_break (child, &Frame::propagate_children))
   {
      ohm_util_TRACE_DEBUGGER ();
   }
#endif
#endif

   if (child._event_type_counter.does_support (event))
   {
      bool propagate_flag = true;
      
      bool in_flag = true;
      
      opa::Point child_origin = child.get_origin ();
      
      if (child._clip_flag)
      {
         opa::Point child_rel_origin = child.get_relative_origin ();
         opa::Rect child_rect (child_origin + child_rel_origin, child.get_bounds ());
         
         opa::Margin child_margin = child.get_margin ();
         bool margin_flag = child_margin != opa::Margin::_zero;
         
         if (margin_flag)
         {
            opa::Size vec_x (1.f, 0.f);
            opa::Size vec_y (0.f, 1.f);
            
            event.apply_transform (vec_x);
            event.apply_transform (vec_y);
            
            child_rect._origin._x -= vec_x._width * child_margin._left;
            child_rect._origin._x -= vec_y._width * child_margin._left;
            child_rect._origin._y += vec_x._height * child_margin._top;
            child_rect._origin._y -= vec_y._height * child_margin._top;

            child_rect._size._width += vec_x._width * (child_margin._left + child_margin._right);
            child_rect._size._width += vec_y._width * (child_margin._left + child_margin._right);
            child_rect._size._height -= vec_x._height * (child_margin._top + child_margin._bottom);
            child_rect._size._height += vec_y._height * (child_margin._top + child_margin._bottom);
         }
         
         in_flag = child_rect.contains (location);
         
         if (_propagation_clip_flag)
         {
            opa::Rect clip_rect (
               child_origin + child_rel_origin + _propagation_clip_rect._origin,
               _propagation_clip_rect._size
            );
            
            in_flag &= clip_rect.contains (location);
         }
      }
      
      if (spatial_flag)
      {
         propagate_flag &= in_flag;
      }
      
      if (event._type == Event::Type_MOUSE_MOVED)
      {
         propagate_flag = true;
      }
      
      if (!event._target_frame_path.empty ())
      {
         propagate_flag = true;
      }
      
      event._mouse_in_flag = parent_in_flag && in_flag;
      
      if (propagate_flag)
      {
         size_t el = event.push (child);
         event.save ();
         
         // note : we don't translate with relative origin
         event.translate (child_origin._x, child_origin._y);
         
         if (child._active_flag)
         {
            child.handle_event (event);
         }

         event.restore ();
         size_t el2 = event.pop ();
         
         assert (el == el2);
      }
      
      if (event.is_propagation_stopped () || _abort_iteration_flag)
      {
         return false;
      }
   }
   
   return true;
}



/*
==============================================================================
Name : dispatch_auto
Description :
   Dispatch the event and detects if the dispatcher actually propagated the
   event.
   Returns true iff the event should be propagated to children.
==============================================================================
*/

bool  Frame::dispatch_auto (Event & event)
{
   dispatch (event);
   
   bool propagate_flag = (!event.is_blocked ()) && (!_children_propagated_flag);
   
   return propagate_flag;
}



/*
==============================================================================
Name : forward_latent_timer_cmd
==============================================================================
*/

void  Frame::forward_latent_timer_cmd ()
{
   assert (_view_ptr != 0);
   
   if (!_latent_timer_cmd_arr.empty ())
   {
      for (size_t i = 0 ; i < _latent_timer_cmd_arr.size () ; ++i)
      {
         TimerCmd & timer_cmd = _latent_timer_cmd_arr [i];
         
         switch (timer_cmd._action)
         {
         case TimerCmd::Action_INSTALL_REGULAR_TIMER:
            _view_ptr->install_regular_timer (timer_cmd._delegate);
            break;

         case TimerCmd::Action_INSTALL_ONE_SHOT_TIMER:
            _view_ptr->install_one_shot_timer (timer_cmd._delegate, timer_cmd._delay);
            break;

         case TimerCmd::Action_REMOVE_TIMER:
            _view_ptr->remove_timer (timer_cmd._delegate);
            break;
         
         default:
            assert (false);
            break;
         }
      }
      
      std::vector <TimerCmd> ().swap (_latent_timer_cmd_arr);
   }
}



/*
==============================================================================
Name : notify_layout_changed
==============================================================================
*/

void  Frame::notify_layout_changed ()
{
   layout_changed ();
   
   if (_view_ptr != 0)
   {
      _view_ptr->notify_layout_changed ();
   }
}



/*
==============================================================================
Name : handle_event_debug
==============================================================================
*/

void  Frame::handle_event_debug (Event & event)
{
   opa::Rect rect (get_relative_origin (), get_bounds ());
   
   opa::Margin margin = get_margin ();
   bool margin_flag = margin != opa::Margin::_zero;
   
   if (margin_flag)
   {
      opa::Size vec_x (1.f, 0.f);
      opa::Size vec_y (0.f, 1.f);
      
      event.apply_transform (vec_x);
      event.apply_transform (vec_y);
      
      rect._origin._x -= vec_x._width * margin._left;
      rect._origin._x -= vec_y._width * margin._left;
      rect._origin._y += vec_x._height * margin._top;
      rect._origin._y -= vec_y._height * margin._top;

      rect._size._width += vec_x._width * (margin._left + margin._right);
      rect._size._width += vec_y._width * (margin._left + margin._right);
      rect._size._height -= vec_x._height * (margin._top + margin._bottom);
      rect._size._height += vec_y._height * (margin._top + margin._bottom);
   }
   
   opa::Point pt1 (rect.get_left (), rect.get_top ());
   opa::Point pt2 (rect.get_right (), rect.get_top ());
   opa::Point pt3 (rect.get_right (), rect.get_bottom ());
   opa::Point pt4 (rect.get_left (), rect.get_bottom ());
   
   event.apply_inverse_transform (pt1);
   event.apply_inverse_transform (pt2);
   event.apply_inverse_transform (pt3);
   event.apply_inverse_transform (pt4);
   
   event._debug_quad_arr.push_back (opa::Quad (pt1, pt2, pt3, pt4));
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
