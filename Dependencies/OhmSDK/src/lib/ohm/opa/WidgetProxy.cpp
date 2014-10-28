/*****************************************************************************

        WidgetProxy.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59147

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

#include "ohm/opa/WidgetProxy.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetProxy::~WidgetProxy ()
{
   try
   {
      if (_widget_sptr.get () != 0)
      {
         if (_widget_sptr->has_parent ())
         {
            Bundle & parent = _widget_sptr->get_parent ();
            
            parent.unembed (*_widget_sptr);
         }
         
         _widget_sptr.destroy ();
      }
      
      unset_all ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
   In particular this means that the widget may scroll horizontally.
==============================================================================
*/

void  WidgetProxy::set_width (float width)
{
   _width.set (width);
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name : set_width_auto_parent
Description :
   Tell the layout system that the width of 'this' is dependent on the
   actual parent width.
==============================================================================
*/

void  WidgetProxy::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetProxy::get_width ()
{
   if (_width.need_update ())
   {
      update_metrics ();
   }
   
   return _width.get ();
}



/*
==============================================================================
Name : set_height
Description :
   Fix the height of 'this', independent of actual content height.
   In particular this means that the widget may scroll vertically.
==============================================================================
*/

void  WidgetProxy::set_height (float height)
{
   _height.set (height);
}



/*
==============================================================================
Name : set_height_auto_parent
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual parent width.
==============================================================================
*/

void  WidgetProxy::set_height_auto_parent (float weight)
{
   _height.set_ext ();
   set_vertical_weight (weight);
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float WidgetProxy::get_height ()
{
   if (_height.need_update ())
   {
      update_metrics ();
   }
   
   return _height.get ();
}



/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void WidgetProxy::invalidate_layout ()
{
   _width.invalidate ();
   _height.invalidate ();

   WidgetBase::invalidate_layout ();
}



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetProxy::layout (const Size & bounds)
{
   bool retrigger_layout_flag = WidgetBase::layout (bounds);
   
   if (_widget_sptr.get () != 0)
   {
      for (int i = 0 ; i < 2 ; ++i)
      {
         Size sub_bounds (1e8f, 1e8f);
         
         {
            float weight = _widget_sptr->get_horizontal_weight ();
            
            if (weight > 0.f)
            {
               sub_bounds._width = bounds._width;
            }
         }
         
         {
            float weight = _widget_sptr->get_vertical_weight ();
            
            if (weight > 0.f)
            {
               sub_bounds._height = bounds._height;
            }
         }
         
         retrigger_layout_flag |= _widget_sptr->layout (sub_bounds);
         
         if (!retrigger_layout_flag)
         {
            break;
         }
      }
   }
   
   return retrigger_layout_flag;
}



/*
==============================================================================
Name : spread_view
Description :
   Propagate the visible portion in a widget tree. 'rect' is expressed
   relative to 'this' origin (not accounting relative origin).
==============================================================================
*/

void  WidgetProxy::spread_view (const Rect & rect)
{
   bool show_flag = false;
   
   if (is_visible ())
   {
      if (!rect.is_empty ())
      {
         Rect sub_rect (Point::_zero, get_bounds ());
         
         if (rect.is_intersecting (sub_rect))
         {
            show_flag = true;
         }
      }
   }
   
   show_actual (show_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetProxy::WidgetProxy ()
:  _width (*this)
,  _height (*this)
,  _widget_sptr ()
{
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetProxy::do_get_min_bbox (Rect & min_bbox)
{
   if (_width.need_update () || _height.need_update ())
   {
      update_metrics ();
   }
   
   min_bbox = Rect::_zero;
   min_bbox._size._width = _width.get ();
   min_bbox._size._height = _height.get ();
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetProxy::do_get_margin (Margin & margin)
{
   margin = Margin::_zero;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetProxy::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->set_origin (origin, tt, tf);
   }
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetProxy::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->set_bounds (bounds, tt, tf);
   }
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetProxy::do_enable (bool enabled_flag)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->do_enable (enabled_flag);
   }
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetProxy::do_show (bool show_flag)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->do_show (show_flag);
   }
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetProxy::do_activate (bool activate_flag)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->do_activate (activate_flag);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetProxy::update_metrics ()
{
   if (_width.need_update () || _height.need_update ())
   {
      if (_width.need_update ())
      {
         _width.update (0.f);
      }

      if (_height.need_update ())
      {
         _height.update (0.f);
      }
   }
}



/*
==============================================================================
Name : show_actual
Description :
   Tells if the represented widget of the proxy is actually visible or not.
   The actual creation/release of the represented widget is done here.
Note :
   This does not trigger invalidation.
==============================================================================
*/

void  WidgetProxy::show_actual (bool show_flag)
{
   bool update_flag = false;
   
   if (show_flag && (_widget_sptr.get () == 0))
   {
      update_flag = true;
   }
   else if ((!show_flag) && (_widget_sptr.get () != 0))
   {
      update_flag = true;
   }
   
   if (update_flag)
   {
      Bundle & parent = get_parent ();
      
      if (!show_flag)
      {
         parent.unembed (*_widget_sptr);
      }
      
      Event widget_event (*this, _widget_sptr);
      
      widget_event._type = show_flag ? Event::Type_CREATE : Event::Type_RELEASE;
      
      _delegate.param_ref_signal (widget_event);
      
      // even in Type_RELEASE, delegate is not meant to destroy the widget
      assert (_widget_sptr.get () != 0);
         
      // do not trigger invalidation
      
      if (show_flag)
      {
         parent.embed (*_widget_sptr);
         
         _widget_sptr->layout (get_bounds ());
         _widget_sptr->set_rect (get_rect ());
      }
      else
      {
         _widget_sptr.destroy ();
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
