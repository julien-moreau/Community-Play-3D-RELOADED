/*****************************************************************************

        WidgetComboBox.cpp
        Copyright (c) 2009 Ohm Force

        Author: Charles Francoise <charles.francoise@ohmforce.com>
        Revision: 61006

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/WidgetComboBox.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opa/prim/PrimComboBox.h"
#include "ohm/opal/private/Typesetter.h"
#include "ohm/opal/Frame.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~WidgetComboBox
==============================================================================
*/

WidgetComboBox::~WidgetComboBox ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_aptr.reset ();
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
==============================================================================
*/

void  WidgetComboBox::set_width (float width)
{
   // not enough room to display any label !
   assert (width > _inner_hori_margin * 2.f);
   
   if (
      (_widget_label_sptr->get_width () != width)
      || (_widget_label_sptr->is_width_auto () != false)
      )
   {
      _widget_label_sptr->set_width (width - _inner_hori_margin * 2.f);
      set_horizontal_weight (0.f);
   }
}



/*
==============================================================================
Name : set_width_auto_content
Description :
   Tell the layout system that the width of 'this' is dependent on the
   actual content width.
==============================================================================
*/

void  WidgetComboBox::set_width_auto_content ()
{
   _widget_label_sptr->set_width_auto_content ();
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

void  WidgetComboBox::set_width_auto_parent (float weight)
{
   _widget_label_sptr->set_width_auto_parent (1.f);
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetComboBox::get_width ()
{
   return _widget_label_sptr->get_width () + _inner_hori_margin * 2.f;
}



/*
==============================================================================
Name: set_label
==============================================================================
*/

void  WidgetComboBox::set_label (const std::string & label)
{
   if (_widget_label_sptr->get_label () != label)
   {
      _widget_label_sptr->set_label (label);
   }
}



/*
==============================================================================
Name : get_label
==============================================================================
*/

const std::string &  WidgetComboBox::get_label () const
{
   return _widget_label_sptr->get_label ();
}



/*
==============================================================================
Name: begin_edit
==============================================================================
*/

WidgetMenuRoot &  WidgetComboBox::begin_edit ()
{
   assert (_editing_flag == false);
   assert (_menu_root_ptr == 0);
   _editing_flag = true;
   
   opa::Point origin = _frame_aptr->get_origin ();

   _menu_root_ptr
      = &get_ancestor <BundleView> ().create_popup_menu (_menu_root_safe_link);
   
   assert (_menu_root_ptr != 0);
   
   return *_menu_root_ptr;
}



/*
==============================================================================
Name: end_edit
==============================================================================
*/

void  WidgetComboBox::end_edit ()
{
   assert (_editing_flag == true);
   assert (_menu_root_ptr != 0);
   _editing_flag = false;

   WidgetMenuNodeBase & menu_node_base = *_menu_root_ptr;
   
   WidgetMenuNodeBase::iterator it = menu_node_base.begin ();
   const WidgetMenuNodeBase::iterator it_end = menu_node_base.end ();
   
   for (; it != it_end ; ++it)
   {
      WidgetBase * widget_ptr = &*it;
      assert (widget_ptr != 0);
      
      WidgetMenuItem * menu_item_ptr
         = dynamic_cast <WidgetMenuItem *> (widget_ptr);

      if (menu_item_ptr != 0)
      {
         // menu item cannot be a sub menu
         assert (menu_item_ptr->empty ());
         
         menu_item_ptr->bind <
            WidgetComboBox,
            &WidgetComboBox::req_item_selected
         > (*this);
         
         /*if (menu_item_ptr->is_marked ())
         {
            const std::string & label = menu_item_ptr->get_label ();
            
            set_label (label);
         }*/
      }
   }

   _menu_root_ptr = 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
   
/*
==============================================================================
Name: WidgetComboBox
==============================================================================
*/

WidgetComboBox::WidgetComboBox ()
:  _editing_flag (false)
,  _menu_root_ptr (0)
{
   _frame_aptr = new FrameComboBox (*this);
   
   _layer_aptr = new LayerWidget;
   _layer_aptr->_label_style = AspectWidgetLabel::Style_BUTTON;
   
   _prim_aptr = new PrimComboBox;
   _layer_aptr->push_back (*_prim_aptr);
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_aptr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_aptr);
   set_slot (0, *_layer_aptr);

   _widget_label_sptr = WidgetLabel::create ();
   _widget_label_sptr->set_relative_origin (opa::Point (0, -1));
   
   embed (*_widget_label_sptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetComboBox::do_get_min_bbox (Rect & min_bbox)
{
   opa::Rect label_min_bbox;
   _widget_label_sptr->get_min_bbox (label_min_bbox);
   
   min_bbox = opa::Rect (
      0, -14,
      label_min_bbox._size._width + _inner_hori_margin * 2.f,
      14 + 7
   );
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetComboBox::do_get_margin (Margin & margin)
{
   margin = Margin (4, 4, 4, 6);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetComboBox::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetComboBox::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);

   _prim_aptr->_size.stop ().push (bounds, tt, tf);

   opa::Point label_origin;
   
   opa::Rect label_min_bbox;
   _widget_label_sptr->get_min_bbox (label_min_bbox);
   
   opa::Rect button_min_bbox;
   get_min_bbox (button_min_bbox);
   
   label_origin._y = floor (
      (button_min_bbox._size._height - label_min_bbox._size._height) * .5f
   );
   
   if (_widget_label_sptr->is_width_auto ())
   {
      label_origin._x = _inner_hori_margin;
   }
   else
   {
      label_origin._x = floor (
         (button_min_bbox._size._width - label_min_bbox._size._width) * .5f
      );
   }
   
   _widget_label_sptr->set_origin (label_origin, tt, tf);
   _widget_label_sptr->set_bounds (bounds - opa::Size (_inner_hori_margin * 2.f, 0.f));
}



/*
==============================================================================
Name: do_enable
==============================================================================
*/

void WidgetComboBox::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetComboBox::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetComboBox::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}


   
/*
==============================================================================
Name: shake
==============================================================================
*/

void WidgetComboBox::shake ()
{
   const float x = _layer_aptr->_origin._x;
   const double t = 0.07;
   
   _layer_aptr->_origin._x
      .stop ()
      .push (x+6.f, t, opak::ease_in)
      .push (x-6.f, t, opak::linear)
      .push (x+5.f, t, opak::linear)
      .push (x-4.f, t, opak::linear)
      .push (x+3.f, t, opak::linear)
      .push (x-2.f, t, opak::linear)
      .push (x    , t, opak::ease_out);   
}



/*
==============================================================================
Name: req_open_menu
==============================================================================
*/

void WidgetComboBox::req_open_menu (opal::Event & event)
{
   assert (&event != 0);
   
   Event widget_event (*this);
   widget_event._type = Event::Type_OPENING;
   widget_event._menu_item_ptr = 0;
   widget_event._position = opa::Point (event._mouse._location.get_x (),
                                        event._mouse._location.get_y ());
   
   _delegate.param_ref_signal (widget_event);
}



/*
==============================================================================
Name: req_item_selected
==============================================================================
*/

void WidgetComboBox::req_item_selected (opa::WidgetMenuItem::Event & event)
{
   assert (&event != 0);
   
   Event widget_event (*this);
   widget_event._type = Event::Type_CLOSED;
   
   // might be null
   widget_event._menu_item_ptr
      = dynamic_cast <WidgetMenuItem *> (&event._widget);
   
   _delegate.param_ref_signal (widget_event);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



float WidgetComboBox::_inner_hori_margin = 6.f;



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

