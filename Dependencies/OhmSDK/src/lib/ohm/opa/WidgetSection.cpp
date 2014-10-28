/*****************************************************************************

        WidgetSection.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 60933

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

#include "ohm/opa/WidgetSection.h"

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

WidgetSection::~WidgetSection ()
{
   try
   {
      unset ();
      
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_aptr.reset ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_style_header
==============================================================================
*/

void  WidgetSection::set_style_header ()
{
   _prim_back_aptr->_type = PrimSection::Type_HEADER;

   _layer_widget_aptr->_label_style = AspectWidgetLabel::Style_SECTION;
   
   _margin = Margin (0, 6, 0, 0);
   
   invalidate_layout ();
}



/*
==============================================================================
Name : set_style_content
==============================================================================
*/

void  WidgetSection::set_style_content ()
{
   _prim_back_aptr->_type = PrimSection::Type_CONTENT;

   _layer_widget_aptr->_label_style = AspectWidgetLabel::Style_DEFAULT;

   _margin = Margin (0, 0, 0, 0);
   
   invalidate_layout ();
}



/*
==============================================================================
Name : set_style_footer
==============================================================================
*/

void  WidgetSection::set_style_footer ()
{
   _prim_back_aptr->_type = PrimSection::Type_FOOTER;

   _layer_widget_aptr->_label_style = AspectWidgetLabel::Style_DEFAULT;

   _margin = Margin (0, 0, 0, 6);
   
   invalidate_layout ();
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
   In particular this means that the widget may scroll horizontally.
==============================================================================
*/

void  WidgetSection::set_width (float width)
{
   _width.set (width);
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name : set_width_auto_content
Description :
   Tell the layout system that the width of 'this' is dependent on the
   actual content width.
   In particular this means that the widget won't scroll horizontally.
==============================================================================
*/

void  WidgetSection::set_width_auto_content ()
{
   _width.set_auto ();
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

void  WidgetSection::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetSection::get_width ()
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

void  WidgetSection::set_height (float height)
{
   _height.set (height);
   set_vertical_weight (0.f);
}



/*
==============================================================================
Name : set_height_auto_content
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual content height.
   In particular this means that the widget won't scroll vertically.
==============================================================================
*/

void  WidgetSection::set_height_auto_content ()
{
   _height.set_auto ();
   set_vertical_weight (0.f);
}



/*
==============================================================================
Name : set_height_auto_parent
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual parent width.
==============================================================================
*/

void  WidgetSection::set_height_auto_parent (float weight)
{
   _height.set_ext ();
   set_vertical_weight (weight);
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float WidgetSection::get_height ()
{
   if (_height.need_update ())
   {
      update_metrics ();
   }
   
   return _height.get ();
}



/*
==============================================================================
Name : unset
==============================================================================
*/

void  WidgetSection::unset ()
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

bool  WidgetSection::empty () const
{
   return (_widget_sptr.get () == 0);
}



/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void  WidgetSection::invalidate_layout ()
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

bool  WidgetSection::layout (const Size & bounds)
{
   bool retrigger_layout_flag = WidgetBase::layout (bounds);
   
   if (!empty ())
   {
      Rect sub_min_bbox;
      _widget_sptr->get_min_bbox (sub_min_bbox);
      
      Margin margin;
      _widget_sptr->get_margin (margin);
      
      lang::max_in_place (margin._top, _padding_y);
      lang::max_in_place (margin._bottom, _padding_y);

      Size sub_bounds = bounds - Size (2 * _padding_x, margin._top + margin._bottom);
      
      //Point origin (0, margin._top);
      Point origin (0, margin._top);
      
      retrigger_layout_flag |= _widget_sptr->layout (sub_bounds);
      
      _widget_sptr->set_origin (origin, get_transition_time (), get_transition_function ());
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

void  WidgetSection::spread_view (const Rect & rect)
{
   if (!empty ())
   {
      Rect sub_rect = rect;
      sub_rect -= _widget_sptr->get_origin ();
      
      Point content_origin (_padding_x, 0);
      sub_rect -= content_origin;
      
      _widget_sptr->spread_view (sub_rect);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetSection::WidgetSection ()
:  _widget_sptr ()
,  _width (*this)
,  _height (*this)
{
   _frame_aptr = new opal::Frame ("WidgetSection::_frame_aptr");
   
   Point content_origin (_padding_x, 0);
   
   _frame_content_aptr = new opal::Frame ("WidgetSection::_frame_content_aptr");
   _frame_content_aptr->set_origin (content_origin);
   _frame_aptr->insert_top (*_frame_content_aptr);
   
   _layer_widget_aptr = new LayerWidget;
   _layer_widget_aptr->set_name ("WidgetSection::base");
   
   _prim_back_aptr = new PrimSection;
   _layer_widget_aptr->push_back (*_prim_back_aptr);
   
   _layer_content_aptr = new opak::Layer;
   _layer_widget_aptr->push_back (*_layer_content_aptr);
   
   _layer_content_aptr->_origin = content_origin;
   

   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_widget_aptr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_content_aptr);
   set_slot (0, *_layer_content_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetSection::do_get_min_bbox (Rect & min_bbox)
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

void  WidgetSection::do_get_margin (Margin & margin)
{
   margin = _margin;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetSection::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);

   _layer_widget_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetSection::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
   _prim_back_aptr->_size.stop ().push (bounds, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetSection::do_enable (bool enabled_flag)
{
   _layer_widget_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetSection::do_show (bool show_flag)
{
   _layer_widget_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetSection::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetSection::update_metrics ()
{
   if (_width.need_update () || _height.need_update ())
   {
      if (_widget_sptr.get () != 0)
      {
         Rect bbox;
         _widget_sptr->get_min_bbox (bbox);

         Margin margin;
         _widget_sptr->get_margin (margin);
         
         lang::max_in_place (margin._top, _padding_y);
         lang::max_in_place (margin._bottom, _padding_y);
         
         if (_width.need_update ())
         {
            _width.update (bbox._size._width + _padding_x * 2);
         }

         if (_height.need_update ())
         {
            _height.update (bbox._size._height + margin._top + margin._bottom);
         }
      }
   }
}



float WidgetSection::_padding_x = 16.f;
float WidgetSection::_padding_y = 4.f;



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
