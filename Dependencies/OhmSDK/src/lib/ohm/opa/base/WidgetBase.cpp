/*****************************************************************************

        WidgetBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67334

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

#include "ohm/opa/base/WidgetBase.h"
#include "ohm/opa/private/WidgetLayoutManager.h"

#include "ohm/math/fnc.h"
#include "ohm/opak/base/PrimitiveContainer.h"

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

WidgetBase::WidgetBase ()
:  Bundle (0)
,  _visible_flag (true)
,  _enabled_flag (true)
,  _horizontal_weight (0.f)
,  _vertical_weight (0.f)
,  _margin ()
,  _rel_origin ()
,  _transition_time (0.0)
,  _transition_function (opak::step)

,  _data ()
,  _data_type_info_ptr (0)
,  _dtor_fnc (0)

#if defined (OHM_OPA_WIDGET_DEBUG)
,  _attached_flag (false)
#endif
{
#if defined (OHM_OPA_WIDGET_DEBUG)
   _frame_debug_aptr = new FrameWidgetBaseDebug (*this);
   _prim_debug_aptr = new PrimWidgetBaseDebug;
   _prim_debug_aptr->_visibility = false;

#endif   // OHM_OPA_WIDGET_DEBUG
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetBase::~WidgetBase ()
{
   release_data ();
}



/*
==============================================================================
Name : enable
Description :
   Enable/disable the widget. The widget should no longer be sensible to
   user inputs, and its aspect should reflect this state.
==============================================================================
*/

void  WidgetBase::enable (bool enabled_flag)
{
   if (_enabled_flag != enabled_flag)
   {
      _enabled_flag = enabled_flag;
      
      do_enable (enabled_flag);
      
      do_activate (enabled_flag && is_visible ());
   }
}



/*
==============================================================================
Name : disable
Description :
   See 'enable'
==============================================================================
*/

void  WidgetBase::disable ()
{
   enable (false);
}


   
/*
==============================================================================
Name: is_enabled
==============================================================================
*/

bool WidgetBase::is_enabled () const
{
   return _enabled_flag;
}



/*
==============================================================================
Name : show
Description :
   Show/hide the widget. The widget should no longer be sensible to user
   inputs, and should not be displayed.
   In particular, widget containers should take into account this property
   as if the widget was no longer there.
==============================================================================
*/

void  WidgetBase::show (bool show_flag)
{
   if (_visible_flag != show_flag)
   {
      _visible_flag = show_flag;
      
      do_show (show_flag);
      
      do_activate (show_flag && is_enabled ());
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name : hide
Description :
   See 'show'
==============================================================================
*/

void  WidgetBase::hide ()
{
   show (false);
}


   
/*
==============================================================================
Name: is_visible
==============================================================================
*/

bool WidgetBase::is_visible () const
{
   return _visible_flag;
}



/*
==============================================================================
Name : set_horizontal_weight
Description :
   Set the 'weight' of the widget.
   Example : when producing a layout for a line, the total width of all
   widget on the line might be smaller than the actual width they could take.
   In that case the layout system takes weights into account so that each
   widget get more width so that the total width is equal to the actual
   width they could take.
   
   A weight of 0 means that the widget size is not sensible to weight, which
   means that when layouting a line, the layout system won't give more width
   to that widget.
   
   If widgets have non 0 weight, a simple barycenter problem is solved.

Example :
   To align a widget on the right in a WidgetStack with horizontal orientation
   WidgetSpacer weight = 1 | Widget weigth = 0
   To center a widget in a WidgetStack with horizontal orientation
   WidgetSpacer weight = 1 | Widget weigth = 0 | WidgetSpacer weight = 1
   
   Weight also works in the negative way :
   If the total width (min_bbox) of all widgets on the line is greater than
   the actual width they could take, using weight (yet positive) for widget
   can force them to take less width that they could actually normally take,
   if the widget has a sensible way to interpret it.
   Example : a WidgetLabel may display an ellipsis if its label is cropped,
   making it probably still significant.

==============================================================================
*/

void  WidgetBase::set_horizontal_weight (float weight)
{
   assert (weight >= 0.f);
   
   if (_horizontal_weight != weight)
   {
      _horizontal_weight = weight;
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name: get_horizontal_weight
Description :
   Default implementation.
   Some widgets might need to specialize this member, for example Single
   widget container that needs to propagate the weight of its widget.
==============================================================================
*/

float WidgetBase::get_horizontal_weight () const
{
   return _horizontal_weight;
}



/*
==============================================================================
Name : set_vertical_weight
Description :
   See 'set_horizontal_weight'
==============================================================================
*/

void  WidgetBase::set_vertical_weight (float weight)
{
   assert (weight >= 0.f);
   
   if (_vertical_weight != weight)
   {
      _vertical_weight = weight;
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name: get_vertical_weight
Description :
   Default implementation.
   Some widgets might need to specialize this member, for example Single
   widget container that needs to propagate the weight of its widget.
==============================================================================
*/

float WidgetBase::get_vertical_weight () const
{
   return _vertical_weight;
}



/*
==============================================================================
Name : set_extra_margin
Description :
   Add a margin to the widget to make visual adjustments.
   This is a convenient function to avoid using a WidgetSpacer.
   This extra margin is added to the internal widget margin.
==============================================================================
*/

void  WidgetBase::set_extra_margin (const Margin & margin)
{
   assert (math::is_finite (margin._left));
   assert (math::is_finite (margin._top));
   assert (math::is_finite (margin._right));
   assert (math::is_finite (margin._bottom));
   
   if (_margin != margin)
   {
      _margin = margin;
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name : get_margin
Description :
   Returns the margin of the widget. The margins have the same meaning as in
   CSS, and two consecutive margins will collapse as in CSS.
   The margin of the widget takes into account the base margin dictated by
   the widget through 'do_get_margin' as well as a supplementary margin
   set through 'add_margin' to make small visual adjustment.
   
   Margin nesting (that is when a widget container contains a widget that
   has margin) is subject to the widget interpretation.
   For example :
   - WidgetStack will propagate margins of its content
   - WidgetDialog will ignore the margins of its content. (dialogs have
     content padding, but it is assumed that the content padding will be
     always greater than the content margin)
==============================================================================
*/

void  WidgetBase::get_margin (Margin & margin)
{
   do_get_margin (margin);
   
   assert (math::is_finite (margin._left));
   assert (math::is_finite (margin._top));
   assert (math::is_finite (margin._right));
   assert (math::is_finite (margin._bottom));

   margin += _margin;
}



/*
==============================================================================
Name : set_relative_origin
Description :
   Add an offset to the widget origin to make visual adjustments.
==============================================================================
*/

void  WidgetBase::set_relative_origin (const Point & rel_origin)
{
   assert (math::is_finite (rel_origin._x));
   assert (math::is_finite (rel_origin._y));
   
   if (_rel_origin != rel_origin)
   {
      _rel_origin = rel_origin;
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name : get_min_bbox
Description :
   Returns the minimal bbox under which the widget aspect or conduct might
   not be optimal.
   The coordinate system places 0, 0 at the horizontal and vertical baseline.
   
   Widgets (like a multiple line text field) that cannot have a consistent
   way to know they minimal bbox can reply a 0 bounds bbox, and then use
   layout retrigerring to adjust their bbox (see 'layout')
==============================================================================
*/

void  WidgetBase::get_min_bbox (Rect & min_bbox)
{
   do_get_min_bbox (min_bbox);

   assert (math::is_finite (min_bbox._origin._x));
   assert (math::is_finite (min_bbox._origin._y));
   assert (math::is_finite (min_bbox._size._width));
   assert (math::is_finite (min_bbox._size._height));
}



/*
==============================================================================
Name : set_transition
Description :
   Set the transition state for subsequent widget manipulation.
   The animation itself interpretation is bound to the subclass
   interpretation.
   Generally the transition apply to the widget properties except its origin,
   which is controlled by its parent.
==============================================================================
*/

void  WidgetBase::set_transition (double tt, opak::TransitionFunction tf)
{
   assert (math::is_finite (tt));
   
   _transition_time = tt;
   _transition_function = tf;
}



/*
==============================================================================
Name : set_transition_default
==============================================================================
*/

void  WidgetBase::set_transition_default ()
{
   set_transition (0.25, opak::linear);
}



/*
==============================================================================
Name : set_transition_fast
==============================================================================
*/

void  WidgetBase::set_transition_fast ()
{
   set_transition (0.125, opak::linear);
}



/*
==============================================================================
Name : set_transition_none
==============================================================================
*/

void  WidgetBase::set_transition_none ()
{
   set_transition (0.0, opak::step);
}



/*
==============================================================================
Name : get_transition_time
==============================================================================
*/

double   WidgetBase::get_transition_time () const
{
   return _transition_time;
}



/*
==============================================================================
Name : get_transition_function
==============================================================================
*/

opak::TransitionFunction   WidgetBase::get_transition_function () const
{
   return _transition_function;
}



/*
==============================================================================
Name : set_rect
Description :
   Set the 'rect' of the widget. The widget must accept this 'rect' passively,
   that is try to put up with the rectangle even if the rectangle might
   be smaller than its min bbox.
   This function is called from 'layout' implementations, or directly from
   the library client when the latter uses a widget without auto layouting.
   
   The 'rect' defines the top/left right/bottom bbox of the widget. In
   particular it does not take into account baseline, for example.

Note :
   A widget must NEVER call 'invalidate_layout' from either 'do_set_rect',
   'do_set_origin' or 'do_set_bounds'
==============================================================================
*/

void  WidgetBase::set_rect (const Rect & rect, double tt, opak::TransitionFunction tf)
{
   assert (math::is_finite (rect._origin._x));
   assert (math::is_finite (rect._origin._y));
   assert (math::is_finite (rect._size._width));
   assert (math::is_finite (rect._size._height));

   _rect = rect;
   
   do_set_rect (rect + _rel_origin, tt, tf);

#if defined (OHM_OPA_WIDGET_DEBUG)
   init_debug ();
   
   _frame_debug_aptr->set_origin ((rect + _rel_origin)._origin);
   _prim_debug_aptr->_origin = (rect + _rel_origin)._origin;
   _frame_debug_aptr->set_bounds (rect._size);
   _prim_debug_aptr->_size = rect._size;

#endif   // OHM_OPA_WIDGET_DEBUG
}



/*
==============================================================================
Name : set_origin
Description :
   See 'set_rect'
==============================================================================
*/

void  WidgetBase::set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   assert (math::is_finite (origin._x));
   assert (math::is_finite (origin._y));

   _rect._origin = origin;
   
   do_set_origin (origin + _rel_origin, tt, tf);

#if defined (OHM_OPA_WIDGET_DEBUG)
   init_debug ();

   _frame_debug_aptr->set_origin (origin + _rel_origin);
   _prim_debug_aptr->_origin = origin + _rel_origin;

#endif   // OHM_OPA_WIDGET_DEBUG
}



/*
==============================================================================
Name : set_bounds
Description :
   See 'set_rect'
==============================================================================
*/

void  WidgetBase::set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   assert (math::is_finite (bounds._width));
   assert (math::is_finite (bounds._height));

   _rect._size = bounds;
   
   do_set_bounds (bounds, tt, tf);

#if defined (OHM_OPA_WIDGET_DEBUG)
   init_debug ();

   _frame_debug_aptr->set_bounds (bounds);
   _prim_debug_aptr->_size = bounds;

#endif   // OHM_OPA_WIDGET_DEBUG
}



/*
==============================================================================
Name : get_rect
==============================================================================
*/

const Rect &   WidgetBase::get_rect () const
{
   return _rect;
}



/*
==============================================================================
Name : get_origin
==============================================================================
*/

const Point &  WidgetBase::get_origin () const
{
   return _rect._origin;
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

const Size &   WidgetBase::get_bounds () const
{
   return _rect._size;
}



/*
==============================================================================
Name : invalidate_layout
Description :
   Propagate the fact that the general widget layout must be updated.
==============================================================================
*/

void  WidgetBase::invalidate_layout ()
{
   WidgetBase * widget_ptr = get_ancestor_ptr <WidgetBase> ();
   
   if (widget_ptr != 0)
   {
      // propagate
      
      widget_ptr->invalidate_layout ();
   }
   else
   {
      // two cases :
      // ok : the widget is not yet embedded into its parent (WidgetStack)
      // error : the library client forgot to include the widget in a widget
      // that support interface embedding
   }
}



/*
==============================================================================
Name : layout
Description :
   Prepare & apply the layout operation. If bounds are modified, this tells
   the layout system that it should adapt (and maybe rettriger) its layout.
   Returns true if the layout operation must be called again, false
   otherwise.
   
   Example : a multi line text field receive a new width. To be able to
   be displayed correctly, shortening the width of a multi line text field
   might enlarge its height. Returning true in that case tells to the layout
   system that displaying the widget in the current state won't be optimal,
   and that it should trigger a new layout operation to take this change
   into account.

Note :
   Generally, only widget containers will want to reimplement this function.
==============================================================================
*/

bool  WidgetBase::layout (const Size & bounds)
{
   assert (math::is_finite (bounds._width));
   assert (math::is_finite (bounds._height));
   
   set_bounds (bounds, _transition_time, _transition_function);
   
#if defined (OHM_OPA_WIDGET_DEBUG)
   init_debug ();

   Rect min_bbox;
   get_min_bbox (min_bbox);
   
   _prim_debug_aptr->_min_bbox = min_bbox;
   
   Margin margin;
   get_margin (margin);
   
   _prim_debug_aptr->_margin = margin;

#endif   // OHM_OPA_WIDGET_DEBUG

   return false;
}



/*
==============================================================================
Name : spread_view
Description :
   Propagate the visible portion in a widget tree. 'rect' is expressed
   relative to 'this' origin (not accounting relative origin).
Note :
   Generally :
   - widget containers will want to reimplement this function to account for
     sub widgets offset in the container
   - widget that crop the visible portion will want to reimplement this
     function (eg. WidgetScroll)
   - widget that take particular actions on their visible portion will want
     to reimplement this function (eg. WidgetProxy)
==============================================================================
*/

void  WidgetBase::spread_view (const Rect & rect)
{
   // default implementation : nothing
}



/*
==============================================================================
Name : req_debug_entered
==============================================================================
*/

void  WidgetBase::req_debug_entered ()
{
#if defined (OHM_OPA_WIDGET_DEBUG)
   _prim_debug_aptr->_visibility = true;

#endif   // OHM_OPA_WIDGET_DEBUG
}



/*
==============================================================================
Name : req_debug_exited
==============================================================================
*/

void  WidgetBase::req_debug_exited ()
{
#if defined (OHM_OPA_WIDGET_DEBUG)
   _prim_debug_aptr->_visibility = false;

#endif   // OHM_OPA_WIDGET_DEBUG
}
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_set_rect
Description :
   Default implementation
==============================================================================
*/

void  WidgetBase::do_set_rect (const Rect & rect, double tt, opak::TransitionFunction tf)
{
   assert (math::is_finite (rect._origin._x));
   assert (math::is_finite (rect._origin._y));
   assert (math::is_finite (rect._size._width));
   assert (math::is_finite (rect._size._height));

   do_set_origin (rect._origin, tt, tf);
   do_set_bounds (rect._size, tt, tf);
}



/*
==============================================================================
Name : do_enable
Description :
   Set the widget enable state (and its sub widgets)
   'do_enable' only applies to primitives, NOT to frames
   Widget using a layer to have a sub widgets must use a LayerWidget
==============================================================================
*/

/*
==============================================================================
Name : do_show
Description :
   Set the widget visual state (and its sub widgets)
   'do_show' only applies to primitives, NOT to frames
==============================================================================
*/

/*
==============================================================================
Name : do_activate
Description :
   Set the widget activity state (and its sub widgets)
   'do_activate' only applies to frames, NOT to primitives
==============================================================================
*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : release_data
==============================================================================
*/

void  WidgetBase::release_data ()
{
   if (!_data.empty ())
   {
      assert (_dtor_fnc != 0);
      
      void * obj_ptr = &_data [0];
      
      _dtor_fnc (obj_ptr);
      
      _data.clear ();
      _data_type_info_ptr = 0;
      _dtor_fnc = 0;
   }
}



/*
==============================================================================
Name : init_debug
Description :
   Create the debug frame & primitive if needed, and possible
==============================================================================
*/

void  WidgetBase::init_debug ()
{
#if defined (OHM_OPA_WIDGET_DEBUG)
   if (!_attached_flag)
   {
      if (has_parent ())
      {
         Bundle & parent = get_parent ();
         
         // don't check parent has a slot, it will assert if not
         opal::Frame & parent_frame = parent.get_slot_frame ();
         opak::PrimitiveContainer & parent_layer = parent.get_slot_layer ();
         
         parent_frame.insert_top (*_frame_debug_aptr);
         parent_layer.push_back (*_prim_debug_aptr);
         
         _attached_flag = true;
      }
   }

#endif   // OHM_OPA_WIDGET_DEBUG
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
