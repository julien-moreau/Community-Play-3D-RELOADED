/*****************************************************************************

        Event.h
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



#if ! defined (ohm_opal_Event_HEADER_INCLUDED)
#define  ohm_opal_Event_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/FramePath.h"
#include "ohm/opal/ConductChange.h"
#include "ohm/opal/Custom.h"
#include "ohm/opal/Data.h"
#include "ohm/opal/Mouse.h"
#include "ohm/opal/Key.h"
#include "ohm/opal/KeyboardCatchFocus.h"
#include "ohm/opal/Timer.h"
#include "ohm/opal/private/MarkStack.h"
#include "ohm/opal/private/ConductStack.h"
#include "ohm/opa/AffineTransform.h"

#include <string>
#include <vector>
#include <map>
#include <set>



namespace ohm
{
namespace opal
{



class Frame;
class Data;
class View;
class ConductBase;
   
class Event
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Type
   {
                  Type_NONE = -1,
                  Type_MOUSE_MOVED = 0,
                  Type_MOUSE_BUTTON_PRESSED,
                  Type_MOUSE_BUTTON_RELEASED,
                  Type_MOUSE_WHEEL_MOVED,
                  Type_MOUSE_CATCH_CAPTURE,
                  Type_MOUSE_CURSOR,

                  Type_KEY_PRESSED,
                  Type_KEY_REPEATED,
                  Type_KEY_RELEASED,
                  Type_CHARACTERS_TYPED,
                  Type_KEYBOARD_CATCH_FOCUS,

                  Type_USER_INPUT_LOST,
                  
                  Type_CONDUCT_CHANGED,
                  Type_CUSTOM_SENT,

                  Type_DATA_DROPPED,

                  Type_DEBUG,
                  
                  Type_NBR_ELT
   };
   
   enum Direction
   {
                  Direction_TOP_BOTTOM = 0,
                  Direction_BOTTOM_TOP,
   };

   enum Pass
   {
                  Pass_FOCUS = 0,
                  Pass_SPATIAL,
                  Pass_CRUMB,
                  
                  Pass_NBR_ELT
   };
   
   class MultiPropagationState
   {
   public:
                  MultiPropagationState (Event & event);
      virtual     ~MultiPropagationState ();
      void        prepare ();
      
   private:
      Event &     _event;
      size_t      _loop_nbr;
      
      const size_t
                  _old_loop_nbr;
      const bool  _old_mouse_in_flag;
   };
   
   typedef std::vector <opa::Quad>  QuadArr;
   
                  Event ();
   virtual        ~Event ();
   
   void           block ();
   bool           is_blocked () const;
   void           skip_passes ();
   bool           are_passes_skipped () const;
   void           deflect (bool deflect_flag = true);
   bool           is_deflected () const;
   
   bool           is_target_frame_path () const;
   
   void           reset ();
   void           save ();
   void           restore ();
   
   void           translate (float tx, float ty);
   void           scale (float sx, float sy);
   void           rotate (float rot);
   
   void           apply_transform (opa::Point & pt) const;
   void           apply_transform (opa::Size & sz) const;

   void           apply_inverse_transform (opa::Point & pt) const;
   void           apply_inverse_transform (opa::Size & sz) const;
   
   template <class FrameType>
   bool           has_mark (archi::UInt32 mark_id = 0) const;
   bool           has_mark (archi::UInt32 mark_id) const;

   template <class FrameType>
   void           convert_to_mark (opa::Point & pt, archi::UInt32 mark_id = 0) const;
   void           convert_to_mark (opa::Point & pt, archi::UInt32 mark_id) const;
   template <class FrameType>
   void           convert_to_mark (opa::Size & sz, archi::UInt32 mark_id = 0) const;
   void           convert_to_mark (opa::Size & sz, archi::UInt32 mark_id) const;

   template <class FrameType>
   void           convert_from_mark (opa::Point & pt, archi::UInt32 mark_id = 0) const;
   void           convert_from_mark (opa::Point & pt, archi::UInt32 mark_id) const;
   template <class FrameType>
   void           convert_from_mark (opa::Size & sz, archi::UInt32 mark_id = 0) const;
   void           convert_from_mark (opa::Size & sz, archi::UInt32 mark_id) const;
   
   template <class FrameType>
   void           mark (archi::UInt32 mark_id);
   void           mark (const std::type_info *info_ptr, archi::UInt32 mark_id);
   
   template <class ConductType>
   void           set (ConductType * conduct_ptr);
   template <class ConductType>
   ConductType *  get ();
   
   bool           is_spatial () const;
   
   Type           _type;
   Direction      _direction;
   size_t         _loop_nbr;
   FramePath      _target_frame_path;
   Pass           _pass;
   Mouse          _mouse;
   bool           _mouse_in_flag;
   Frame *        _mouse_capture_gifted_frame_ptr;
   Key            _key;
   std::string    _str;
   KeyboardCatchFocus
                  _keyboard_catch_focus;
   Timer          _timer;
   Custom         _custom;
   ConductChange
                  _conduct_change;
   Data           _data;
   
   QuadArr        _debug_quad_arr;
   

   
/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   class State
   {
   public:
                  State ();
                  
      void        translate (float tx, float ty);
      void        scale (float sx, float sy);
      void        rotate (float rot);
      
      opa::AffineTransform
                  _affine_transform;
   };
   
   size_t         push (Frame & frame);
   size_t         pop ();
   const FramePath &
                  get_frame_path () const;
   const opa::AffineTransform &
                  get_affine_transform () const;
   size_t         get_state_stack_size ();
   
   void           debug_print ();
   void           trace_frame_path () const;
   void           trace_target_frame_path () const;
   void           trace (const FramePath & frame_path) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   typedef std::vector <State>   StateArr;
   
   friend class View;
   friend class Frame;
   
   // called by Frame & View
   bool           is_propagation_stopped () const;
   bool           is_mapping_changed () const;
   
   bool           _blocked_flag;
   bool           _skip_passes_flag;
   bool           _deflected_flag;
   StateArr       _state_arr;
   ConductStack   _conduct_stack;
   MarkStack      _mark_stack;
   FramePath      _frame_path;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Event &other);
   bool           operator != (const Event &other);

}; // class Event



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Event.hpp"



#endif   // ohm_opal_Event_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
