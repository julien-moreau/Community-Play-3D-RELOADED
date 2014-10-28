/*****************************************************************************

        Event.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/opal/Event.h"
#include "ohm/opal/FrameTransform.h"
#include "ohm/opal/base/ConductBase.h"
#include "ohm/util/trace.h"
#include "ohm/opa/Debugger.h"
#include "ohm/txt/out/Print.h"

#include <algorithm>

#include <cassert>

#undef OHM_OPAL_DEBUG_EVENT



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : MultiPropagationState::ctor
Description :
   This class is used in the case where a frame is going to propagate
   multiple times the same event to the same frame.
   Client of the library will :
   - instantiate an instance of this class
   - call 'MultiPropagationState::prepare' before each call of
     'propagate_children'
==============================================================================
*/

Event::MultiPropagationState::MultiPropagationState (Event & event)
:  _event (event)
,  _loop_nbr (0)
,  _old_loop_nbr (event._loop_nbr)
,  _old_mouse_in_flag (event._mouse_in_flag)
{
}



/*
==============================================================================
Name : MultiPropagationState::dtor
Description :
   Put back the event in the original state, for the members we changed.
==============================================================================
*/

Event::MultiPropagationState::~MultiPropagationState ()
{
   _event._loop_nbr = _old_loop_nbr;
   _event._mouse_in_flag = _old_mouse_in_flag;
}



/*
==============================================================================
Name : MultiPropagationState::prepare
Description :
   See 'MultiPropagationState::ctor'
==============================================================================
*/

void  Event::MultiPropagationState::prepare ()
{
   _event._loop_nbr = _loop_nbr;
   ++_loop_nbr;

   _event._mouse_in_flag = _old_mouse_in_flag;
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Event::Event ()
:  _type (Type_NONE)
,  _direction (Direction_TOP_BOTTOM)
,  _loop_nbr (0)
,  _target_frame_path ()
,  _pass (Pass_CRUMB)
,  _mouse (*this)
,  _mouse_in_flag (true)
,  _mouse_capture_gifted_frame_ptr (0)
,  _key ()
,  _str ()
,  _keyboard_catch_focus (*this)
,  _timer ()
,  _data ()

,  _blocked_flag (false)
,  _skip_passes_flag (false)
,  _deflected_flag (false)
,  _state_arr ()
,  _frame_path ()
{
   _state_arr.push_back (State ());
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Event::~Event ()
{
   assert (_state_arr.size () == 1);
}



/*
==============================================================================
Name : block
Description :
   Block the propagation of the event between frames.
==============================================================================
*/

void  Event::block ()
{
#if defined (OHM_OPAL_DEBUG_EVENT)
   
   Frame * frame_ptr = 0;
   
   if (!_frame_path.empty ())
   {
      frame_ptr = _frame_path.back ().first; 
   }
   
   if (frame_ptr != 0) 
   {
      if (frame_ptr->get_debug_name () != 0)
      {
         ohm_util_TRACE_DEBUG1 ("Event blocked by %1%", frame_ptr->get_debug_name ());
      }
      else
      {
         ohm_util_TRACE_DEBUG ("Event blocked by <Unknown>");
      }
   }
   else 
   {
      ohm_util_TRACE_DEBUG ("Event blocked, but not by a frame");
   }
   
   trace_frame_path ();
   
#endif
   
   assert (!_blocked_flag);
   assert (_type != Type_USER_INPUT_LOST);
   assert (_type != Type_DEBUG);
   
   if ((_type != Type_USER_INPUT_LOST) && (_type != Type_DEBUG))
   {
      _blocked_flag = true;
   }
}



/*
==============================================================================
Name : is_blocked
==============================================================================
*/

bool  Event::is_blocked () const
{
   return _blocked_flag;
}



/*
==============================================================================
Name : skip_passes
==============================================================================
*/

void  Event::skip_passes ()
{
   _skip_passes_flag = true;
}



/*
==============================================================================
Name : are_passes_skipped
==============================================================================
*/

bool  Event::are_passes_skipped () const
{
   return _skip_passes_flag;
}



/*
==============================================================================
Name : deflect
Description :
   if 'deflect_flag' is true, tell that subsequent dispatchers of the frame
   should be skipped.
==============================================================================
*/

void  Event::deflect (bool deflect_flag)
{
   _deflected_flag = deflect_flag;
}



/*
==============================================================================
Name : is_deflected
==============================================================================
*/

bool  Event::is_deflected () const
{
   return _deflected_flag;
}



/*
==============================================================================
Name : is_target_frame_path
==============================================================================
*/

bool  Event::is_target_frame_path () const
{
   if (_target_frame_path.size () == _frame_path.size ())
   {
      // do it in reverse for speed
      
      FramePath::const_reverse_iterator it = _frame_path.rbegin ();
      FramePath::const_reverse_iterator it_end = _frame_path.rend ();

      FramePath::const_reverse_iterator it2 = _target_frame_path.rbegin ();
      
      return std::equal (it, it_end, it2);
   }
   else
   {
      return false;
   }
}



/*
==============================================================================
Name : reset
==============================================================================
*/

void  Event::reset ()
{
   _type = Type_NONE;
   _direction = Direction_TOP_BOTTOM;
   _loop_nbr = 0;
   _target_frame_path.clear ();
   _pass = Pass_CRUMB;

   _mouse._view_relative_previous_location = _mouse._view_relative_location;
   _mouse._previous_location.invalidate ();

   _mouse._view_relative_wheel_delta = opa::Point::_zero;
   _mouse._wheel_delta.invalidate ();
   _mouse_in_flag = true;
   
   // don't clear data
   
   _mouse_capture_gifted_frame_ptr = 0;
   
   _str.clear ();
   
   _keyboard_catch_focus.reset ();

   _blocked_flag = false;
   _skip_passes_flag = false;
}



/*
==============================================================================
Name : save
==============================================================================
*/

void  Event::save ()
{
   assert (_state_arr.size () > 0);
   
   _state_arr.push_back (_state_arr.back ());
   _conduct_stack.save ();
   _mark_stack.save ();
}



/*
==============================================================================
Name : restore
==============================================================================
*/

void  Event::restore ()
{
   assert (_state_arr.size () > 1);
   
   _state_arr.pop_back ();
   _conduct_stack.restore ();
   _mark_stack.restore ();

   _mouse.invalidate ();
}



/*
==============================================================================
Name : translate
==============================================================================
*/

void  Event::translate (float tx, float ty)
{
   assert (_state_arr.size () > 0);
   
   State & state = _state_arr.back ();
   
   state.translate (tx, ty);

   _conduct_stack.invalidate ();
   
   _mouse.invalidate ();
}



/*
==============================================================================
Name : scale
==============================================================================
*/

void  Event::scale (float sx, float sy)
{
   assert (_state_arr.size () > 0);
   
   State & state = _state_arr.back ();
   
   state.scale (sx, sy);

   _conduct_stack.invalidate ();
   
   _mouse.invalidate ();
}



/*
==============================================================================
Name : rotate
==============================================================================
*/

void  Event::rotate (float rot)
{
   assert (_state_arr.size () > 0);
   
   State & state = _state_arr.back ();
   
   state.rotate (rot);

   _conduct_stack.invalidate (); 
   
   _mouse.invalidate ();
}



/*
==============================================================================
Name : apply_transform
==============================================================================
*/

void  Event::apply_transform (opa::Point & pt) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   const opa::AffineTransform & affine_transform = state._affine_transform;
   
   affine_transform.apply_inverse (pt);
}



/*
==============================================================================
Name : apply_transform
==============================================================================
*/

void  Event::apply_transform (opa::Size & sz) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   const opa::AffineTransform & affine_transform = state._affine_transform;
   
   affine_transform.apply_inverse (sz);
}



/*
==============================================================================
Name : apply_inverse_transform
==============================================================================
*/

void  Event::apply_inverse_transform (opa::Point & pt) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   const opa::AffineTransform & affine_transform = state._affine_transform;
   
   affine_transform.apply (pt);
}



/*
==============================================================================
Name : apply_inverse_transform
==============================================================================
*/

void  Event::apply_inverse_transform (opa::Size & sz) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   const opa::AffineTransform & affine_transform = state._affine_transform;
   
   affine_transform.apply (sz);
}



/*
==============================================================================
Name : has_mark
==============================================================================
*/

bool  Event::has_mark (archi::UInt32 mark_id) const
{
   return _mark_stack.has (mark_id);
}



/*
==============================================================================
Name : convert_to_mark
==============================================================================
*/

void  Event::convert_to_mark (opa::Point & pt, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply (pt);
   
   size_t pos = _mark_stack.find (mark_id);
   
   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply_inverse (pt);
}



/*
==============================================================================
Name : convert_to_mark
==============================================================================
*/

void  Event::convert_to_mark (opa::Size & sz, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply (sz);
   
   size_t pos = _mark_stack.find (mark_id);
   
   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply_inverse (sz);
}



/*
==============================================================================
Name : convert_from_mark
==============================================================================
*/

void  Event::convert_from_mark (opa::Point & pt, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   size_t pos = _mark_stack.find (mark_id);

   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply (pt);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply_inverse (pt);
}



/*
==============================================================================
Name : convert_from_mark
==============================================================================
*/

void  Event::convert_from_mark (opa::Size & sz, archi::UInt32 mark_id) const
{
   assert (_state_arr.size () > 0);
   
   size_t pos = _mark_stack.find (mark_id);

   const State & mark_state = _state_arr [pos];
   mark_state._affine_transform.apply (sz);
   
   const State & state = _state_arr.back ();
   
   state._affine_transform.apply_inverse (sz);
}



/*
==============================================================================
Name : mark
==============================================================================
*/

void  Event::mark (const std::type_info *info_ptr, archi::UInt32 mark_id)
{
   _mark_stack.set (info_ptr, mark_id);
}



/*
==============================================================================
Name : is_spatial
Description :
   Returns true if the msg has a notion of spaciality, and this msg will not
   be sent if it does not meet the frame clipping criteria, except for
   PASS_FOCUS
Note :
   'Type_MOUSE_MOVED' is not spatial clipped, since it is used to detect
   when the mouse exit the frame.
==============================================================================
*/

bool  Event::is_spatial () const
{
   int type_d = static_cast <int> (_type);
   
   if ((type_d >= Type_MOUSE_MOVED) && (type_d <= Type_MOUSE_WHEEL_MOVED))
   {
      return true;
   }
   else if (type_d == Type_MOUSE_CURSOR)
   {
      return true;
   }
   else if ((type_d >= Type_KEY_PRESSED) && (type_d <= Type_CHARACTERS_TYPED))
   {
      return _pass == Pass_SPATIAL;
   }
   else if ((type_d == Type_CONDUCT_CHANGED) || (type_d == Type_CUSTOM_SENT))
   {
      return _pass == Pass_SPATIAL;
   }
   else if (type_d == Type_DATA_DROPPED)
   {
      return true;
   }
   else
   {
      return false;
   }
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : push
Description :
   Push a frame into the frame chain
==============================================================================
*/

size_t   Event::push (Frame & frame)
{
   assert (&frame != 0);
   
   size_t ret_val = _frame_path.size ();
   
   _frame_path.push_back (std::make_pair (&frame, _loop_nbr));
   
   return ret_val;
}



/*
==============================================================================
Name : pop
Description :
   Pop a frame from the frame chain
==============================================================================
*/

size_t   Event::pop ()
{
   // memory is not deallocated : this is what we want
   
   assert (_frame_path.size () > 0);
   
   _frame_path.pop_back ();
   
   return _frame_path.size ();
}



/*
==============================================================================
Name : get_frame_path
==============================================================================
*/

const FramePath & Event::get_frame_path () const
{
   return _frame_path;
}


/*
==============================================================================
Name : get_affine_transform
==============================================================================
*/

const opa::AffineTransform &  Event::get_affine_transform () const
{
   assert (_state_arr.size () > 0);
   
   const State & state = _state_arr.back ();
   
   return state._affine_transform;
}



/*
==============================================================================
Name : get_state_stack_size
==============================================================================
*/

size_t   Event::get_state_stack_size ()
{
   return _state_arr.size ();
}



/*
==============================================================================
Name : debug_print
==============================================================================
*/

void  Event::debug_print ()
{

#if defined (OHM_OPA_DEBUGGER)
   if (
      opa::Debugger::use_instance ().is_tracing (_type)
      || opa::Debugger::use_instance ().is_tracing_events ()
      )
   {
      ohm_util_TRACE_INFO ("==============================================================================");
      
      static const char * _type_name_0_arr [] = {
         "Type_MOUSE_MOVED          ",
         "Type_MOUSE_BUTTON_PRESSED ",
         "Type_MOUSE_BUTTON_RELEASED",
         "Type_MOUSE_WHEEL_MOVED    ",
         "Type_MOUSE_CATCH_CAPTURE  ",
         "Type_MOUSE_CURSOR         ",

         "Type_KEY_PRESSED          ",
         "Type_KEY_REPEATED         ",
         "Type_KEY_RELEASED         ",
         "Type_CHARACTERS_TYPED     ",
         "Type_KEYBOARD_CATCH_FOCUS ",

         "Type_USER_INPUT_LOST      ",
         
         "Type_CONDUCT_CHANGED      ",
         "Type_CUSTOM_SENT          ",

         "Type_DATA_DROPPED         ",

         "Type_DEBUG      ",
      };
      
      const char * type_name_0 = _type_name_0_arr [_type];
      
      const char * in_0 = _mouse_in_flag ? "IN " : "OUT";
      
      const char * dir_0 = (_direction == Direction_TOP_BOTTOM) ? "v" : "^";
      
      const char * pass_0 = 0;
      switch (_pass)
      {
      case Pass_FOCUS:
         pass_0 = "FOCUS  ";
         break;

      case Pass_SPATIAL:
         pass_0 = "SPATIAL";
         break;

      case Pass_CRUMB:
         pass_0 = "CRUMB  ";
         break;
      }
      
      ohm_util_TRACE_INFO5 (
         "%1% %2% %3% %4% %5%",
         type_name_0, in_0, dir_0, pass_0, _loop_nbr
      );
   }
#endif
}  



/*
==============================================================================
Name : trace_frame_path
==============================================================================
*/

void  Event::trace_frame_path () const
{
   trace (_frame_path);
}



/*
==============================================================================
Name : trace_target_frame_path
==============================================================================
*/

void  Event::trace_target_frame_path () const
{
   trace (_target_frame_path);
}



/*
==============================================================================
Name : trace
==============================================================================
*/

void  Event::trace (const FramePath & frame_path) const
{
   txt::out::Print printer;
   printer.print ("");
   
   FramePath::const_iterator it = frame_path.begin ();
   const FramePath::const_iterator it_end = frame_path.end ();
   
   for (; it != it_end ; ++it)
   {
      const std::pair <Frame *, size_t> & frame_loop_nbr = *it;
   
      printer.cat ("%1%:%2% ")
         .arg (frame_loop_nbr.first->get_debug_name ())
         .arg (frame_loop_nbr.second);
   }
   
   const std::string & str = printer;
   
   ohm_util_TRACE_INFO1 ("%1%", str.c_str ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : State::ctor
==============================================================================
*/

Event::State::State ()
:  _affine_transform (true)
{
}




/*
==============================================================================
Name : State::translate
==============================================================================
*/

void  Event::State::translate (float tx, float ty)
{
   _affine_transform.translate (tx, ty);
}



/*
==============================================================================
Name : State::scale
==============================================================================
*/

void  Event::State::scale (float sx, float sy)
{
   _affine_transform.scale (sx, sy);
}



/*
==============================================================================
Name : State::rotate
==============================================================================
*/

void  Event::State::rotate (float rot)
{
   _affine_transform.rotate (rot);
}



/*
==============================================================================
Name : is_propagation_stopped
==============================================================================
*/

bool  Event::is_propagation_stopped () const
{
   switch (_type)
   {
   case Type_MOUSE_MOVED:
      if (!_target_frame_path.empty ())
      {
         return _blocked_flag;
      }
      else
      {
         return false;
      }
      break;

   case Type_USER_INPUT_LOST:
   case Type_DEBUG:
   case Type_CONDUCT_CHANGED:
      return false;
      break;

   case Type_MOUSE_BUTTON_PRESSED:
   case Type_MOUSE_BUTTON_RELEASED:
   case Type_MOUSE_WHEEL_MOVED:
   case Type_MOUSE_CATCH_CAPTURE:
   case Type_MOUSE_CURSOR:
   case Type_KEY_PRESSED:
   case Type_KEY_REPEATED:
   case Type_KEY_RELEASED:
   case Type_CHARACTERS_TYPED:
   case Type_KEYBOARD_CATCH_FOCUS:
   case Type_DATA_DROPPED:
   case Type_CUSTOM_SENT:
      return _blocked_flag || _skip_passes_flag;
      break;
   
   default:
      assert (false);
      return _blocked_flag || _skip_passes_flag;
      break;
   }
}



/*
==============================================================================
Name : is_mapping_changed
==============================================================================
*/

bool  Event::is_mapping_changed () const
{
   assert (_type == Type_MOUSE_MOVED);
   
   return _blocked_flag;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
