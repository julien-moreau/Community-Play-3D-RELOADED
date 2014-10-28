/*****************************************************************************

        WidgetEmbedder.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67991

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

#include "ohm/opa/WidgetEmbedder.h"
#include "ohm/math/fnc.h"

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

WidgetEmbedder::~WidgetEmbedder ()
{
   try
   {
      unset ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_width
==============================================================================
*/

void  WidgetEmbedder::set_width (float width)
{
   assert (math::is_finite (width));
   
   _width = width;
   
   invalidate_layout ();
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetEmbedder::get_width () const
{
   return _width;
}



/*
==============================================================================
Name : set_height
==============================================================================
*/

void  WidgetEmbedder::set_height (float height)
{
   assert (math::is_finite (height));
   
   _height = height;
   
   invalidate_layout ();
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float WidgetEmbedder::get_height () const
{
   return _height;
}



/*
==============================================================================
Name : unset
==============================================================================
*/

void  WidgetEmbedder::unset ()
{
   assert (_widget_sptr.get () != 0);
   
   if (_widget_sptr.get () != 0)
   {
      unembed (*_widget_sptr);
      
      _widget_sptr.destroy ();
   }
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  WidgetEmbedder::empty () const
{
   return (_widget_sptr.get () == 0);
}



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetEmbedder::layout (const Size & bounds)
{
   assert (math::is_finite (bounds._width));
   assert (math::is_finite (bounds._height));
   
   bool retrigger_layout_flag = WidgetBase::layout (bounds);
   
   if (_widget_sptr.get () != 0)
   {
      retrigger_layout_flag |= _widget_sptr->layout (bounds);
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

void  WidgetEmbedder::spread_view (const Rect & rect)
{
   assert (math::is_finite (rect._origin._x));
   assert (math::is_finite (rect._origin._y));
   assert (math::is_finite (rect._size._width));
   assert (math::is_finite (rect._size._height));

   if (_widget_sptr.get () != 0)
   {
      // origin of sub widget is zero
      
      _widget_sptr->spread_view (rect);
   }  
}



/*
==============================================================================
Name: update_layout
==============================================================================
*/

void WidgetEmbedder::update_layout ()
{
   if ((_width == 0.f) || (_height == 0.f))
   {
      // abort
      return;
   }
   
   Rect min_bbox;
   
   for (int i = 0 ; i < 2 ; ++i)
   {
      get_min_bbox (min_bbox);
      
      Size bounds = min_bbox._size;
      
      if (_widget_sptr.get () != 0)
      {
         if (_widget_sptr->get_horizontal_weight () > 0.f)
         {
            bounds._width = _width;
         }
         
         if (_widget_sptr->get_vertical_weight () > 0.f)
         {
            bounds._height = _height;
         }
      }
      
      bool retrigger_layout_flag = false;
      
      retrigger_layout_flag |= layout (bounds);
      
      if (!retrigger_layout_flag)
      {
         break;
      }
   }
   
   // once the whole layout has been updated, spread the visible portion
   // of the widget tree
   
   Rect view_rect (0, 0, _width, _height);
   
   spread_view (view_rect);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetEmbedder::WidgetEmbedder ()
:  _width (0.f)
,  _height (0.f)
{
   _frame_aptr = new opal::Frame ("WidgetEmbedder::frame");
   _frame_aptr->set_bounds (opa::Size (100000.f, 100000.f));
   set_embeddable (*_frame_aptr);
   
   _layer_aptr = new LayerWidget;
   set_embeddable (*_layer_aptr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_aptr);
   set_slot (0, *_layer_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetEmbedder::do_get_min_bbox (Rect & min_bbox)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->get_min_bbox (min_bbox);
   }
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetEmbedder::do_get_margin (Margin & margin)
{
   margin = opa::Margin::_zero;
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetEmbedder::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetEmbedder::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetEmbedder::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetEmbedder::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetEmbedder::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
